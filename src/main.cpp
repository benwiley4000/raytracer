#include <glm/glm.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "vendor/stb_image_write.h"

#include "entities/Camera.hpp"
#include "entities/Light.hpp"
#include "entities/objects/Object3D.hpp"
#include "loadScene.hpp"
#include "getColorForRay.hpp"
#include "constants.hpp"

namespace fs = boost::filesystem;

// run in main thread
std::string getSceneFilename();

// run in separate ray tracing thread
void raytraceScene();

// run in main thread
void waitForInput();

// run in main thread
void saveImage();

std::thread t;
std::mutex mut;

Camera camera;
std::vector<Light> lights;
std::vector<Object3D*> scene_objects;

const int image_channels = 3;

std::vector<char> image;
unsigned long image_width;
unsigned long image_height;

bool done = false;
bool force_quit = false;

int main()
{
	loadScene(getSceneFilename(), &camera, &lights, &scene_objects);

	// Print blank line before beginning ray tracing
	std::cout << std::endl;

	t = std::thread(raytraceScene);

	std::cin.clear(); // clear inputs that could be read immediately
	std::cin.ignore();

	std::string trash; // input that won't get used
	while (!done) {
		std::getline(std::cin, trash); // wait for user to hit enter
		if (!done) {
			waitForInput();
		} // else: ray tracing thread finished while we were waiting!
	}

	if (!force_quit) {
		// prompt user to save final image
		saveImage();
	}

	// deallocate our objects
	for (Object3D* object : scene_objects) {
		delete object;
	}

	if (t.joinable()) {
		// wait for raytraceScene thread to finish if it's still not closed
		t.join();
	}

	return 0;
}

std::string getSceneFilename()
{
	if (!fs::is_directory(scenes_dir)) {
		std::cerr << scenes_dir << " is not a directory!" << std::endl;
		exit(-1);
	}

	std::vector<std::string> filenames;
	fs::directory_iterator itr{fs::path(scenes_dir)};
	// iterate scenes directory to populate filenames vector
	while (itr != fs::directory_iterator{}) {
		std::string pathname = itr->path().string();
		// only add .txt files
		if (fs::is_regular_file(pathname) && pathname.find(".txt") != std::string::npos) {
			filenames.push_back(itr->path().string());
		}
		++itr;
	}
	// sort filenames alphabetically
	std::sort(filenames.begin(), filenames.end());

	int option = -1;
	// prompt user to select one of the scene files
	while (option < 0 || option >= filenames.size()) {
		std::cout << "Select a scene file from the options below." << std::endl;
		for (size_t i = 0, len = filenames.size(); i < len; i++) {
			std::cout << "[" << i << "] ";
			// trim the scenes directory path off the front of the filename for printing
			std::cout << filenames[i].substr(scenes_dir.string().length() + 1) << std::endl;
		}
		std::cout << "Your selection: ";
		std::cin >> option;
	}

	std::cout << std::endl << "Scene " << filenames[option] << " selected." << std::endl;
	return filenames[option];
}

void raytraceScene()
{
	std::chrono::seconds trace_duration(1);
	std::chrono::microseconds wait_duration(100);

	mut.lock();
	glm::vec3 center_of_projection = camera.getPosition();
	const std::vector<glm::vec3>& ray_unit_vectors = camera.getRays(
		&image_width,
		&image_height
	);
	image.assign(ray_unit_vectors.size() * image_channels, 0);
	std::cout << "Ray tracing scene... (enter any input to pause)" << std::endl;
	for (size_t i = 0; !done && i < ray_unit_vectors.size(); i++) {
		glm::vec3 color = getColorForRay(
			center_of_projection,
			ray_unit_vectors[i],
			lights,
			scene_objects
		);
		image[i * image_channels] = (char)(255 * color.r);
		image[i * image_channels + 1] = (char)(255 * color.g);
		image[i * image_channels + 2] = (char)(255 * color.b);

		// indicate progress
		if (i % 15000 == 0) {
			for (size_t j = 0, len = i / 15000; j < len; j++) {
				std::cout << ". ";
			}
			std::cout << i * 100 / ray_unit_vectors.size() << "%" << std::endl;
		}

		mut.unlock(); // possibly defer to waitForInput
		// sleep for extremely small duration to give main thread time to register inputs
		std::this_thread::sleep_for(wait_duration);
		mut.lock(); // pick up when waitForInput has given back control
	}

	done = true;
	// Main thread will take care of save after enter
	std::cout << "Ray tracing complete." << std::endl;
	if (!force_quit) {
		std::cout << " Press enter to save final image.";
	}
	std::cout << std::endl;
}

void waitForInput()
{
	std::cout << "Pausing ray trace..." << std::endl;
	mut.lock();
	std::cout << "Enter 's' to save an image snapshot, 'q' to quit, ";
	std::cout << "or anything else to continue." << std::endl;
	auto c = (char)getchar();
	if (c == 'q' || c == 'Q') {
		done = true;
		force_quit = true;
		mut.unlock();
		if (t.joinable()) {
			// wait for raytraceScene thread to finish
			t.join();
		}
		return;
	}
	if (c == 's' || c == 'S') {
		// save partial image to file
		saveImage();
	}
	std::cout << "Resuming ray trace... (enter any input to pause)" << std::endl;
	mut.unlock(); // return control to raytraceScene
}

void saveImage()
{
	std::cout << "Enter filename to save image: ";
	std::string filename;
	std::cin.clear(); // clear inputs that could be read immediately
	std::cin.ignore();
	std::getline(std::cin, filename);
	boost::trim(filename);
	if (!boost::algorithm::ends_with(boost::algorithm::to_lower_copy(filename), ".bmp")) {
		filename += ".bmp";
	}

	stbi_write_bmp(
		(renders_dir / fs::path(filename)).c_str(),
		(int)image_width,
		(int)image_height,
		image_channels,
		image.data()
	);

	std::cout << "Image saved to " << filename << "." << std::endl;
}
