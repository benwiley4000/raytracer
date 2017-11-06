#include <glm/glm.hpp>
#include <boost/filesystem.hpp>

#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

#include "entities/Camera.hpp"
#include "entities/Light.hpp"
#include "entities/objects/Object3D.hpp"
#include "loadScene.hpp"
#include "constants.hpp"

namespace fs = boost::filesystem;

// run in main thread
std::string getSceneFilename();

// run in separate thread
void raytraceScene();

// run in main thread
void waitForInput();

std::thread t;
std::mutex mut;

Camera camera;
std::vector<Light> lights;
std::vector<Object3D*> scene_objects;

bool done = false;

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
		waitForInput();
	}

	// deallocate our objects
	for (Object3D* object : scene_objects) {
		delete object;
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
	while (!done) {
		std::cout << "Ray tracing scene... (enter any input to pause)" << std::endl;
		std::this_thread::sleep_for(trace_duration);
		mut.unlock(); // possibly defer to waitForInput
		// sleep for extremely small duration to give main thread time to register inputs
		std::this_thread::sleep_for(wait_duration);
		mut.lock(); // pick up when waitForInput has given back control
	}
}

void waitForInput()
{
	std::cout << "Pausing ray trace..." << std::endl;
	mut.lock();
	std::cout << "Enter 'q' to quit or anything else to continue." << std::endl;
	auto c = (char)getchar();
	if (c == 'q' || c == 'Q') {
		done = true;
		mut.unlock();
		if (t.joinable()) {
			// wait for raytraceScene thread to finish
			t.join();
		}
		return;
	}
	std::cout << "Resuming ray trace..." << std::endl;
	mut.unlock(); // return control to raytraceScene
}
