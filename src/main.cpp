#include <glm/glm.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <stdexcept>

#include "objects/Object3D.hpp"
#include "objects/Triangle.hpp"
#include "objects/ObjModel.hpp"

namespace fs = boost::filesystem;

// run in main thread
std::string getSceneFilename();

// run in main thread
void loadScene(const std::string& filename, std::vector<Object3D*>* const& scene_objects);

// run in separate thread
void raytraceScene();

// run in main thread
void waitForInput();

// constants
fs::path scenes_dir = fs::path("..") / fs::path("scenes");
fs::path models_dir = fs::path("..") / fs::path("models");

std::thread t;
std::mutex mut;

std::vector<Object3D*> scene_objects;

bool done = false;

int main()
{
	loadScene(getSceneFilename(), &scene_objects);

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

std::string getTrimmedLineFromFile(std::ifstream* const& file, int* const& line_number)
{
	std::string line;
	std::getline(*file, line);
	(*line_number)++;
	return boost::trim_copy(line);
}

std::string getTrimmedSubstring(
	const std::string& str,
	const unsigned long& pos,
	const unsigned long& n
) {
	return boost::trim_copy(str.substr(pos, n));
}

std::string getTrimmedSubstring(const std::string& str, const unsigned long& pos)
{
	return boost::trim_copy(str.substr(pos));
}

std::runtime_error scenefileFieldError(
	const std::string& fieldname,
	const std::string& filename,
	const int& line_number,
	const std::string& bad_state
) {
	return std::runtime_error(
		"Field '" + fieldname + "' " + bad_state + " (" +
			filename + " line " + std::to_string(line_number) + ")"
	);
}

std::runtime_error unknownFieldError(
	const std::string& fieldname,
	const std::string& filename,
	const int& line_number
) {
	return scenefileFieldError(fieldname, filename, line_number, "unknown");
}

std::runtime_error duplicateFieldError(
	const std::string& fieldname,
	const std::string& filename,
	const int& line_number
) {
	return scenefileFieldError(fieldname, filename, line_number, "already defined");
}

std::runtime_error missingFieldError(
	const std::string& fieldname,
	const std::string& filename,
	const int& line_number
) {
	return scenefileFieldError(fieldname, filename, line_number, "missing");
}

std::runtime_error deformedFieldError(
	const std::string& fieldname,
	const std::string& filename,
	const int& line_number
) {
	return scenefileFieldError(fieldname, filename, line_number, "deformed");
}

void loadScene(const std::string& filename, std::vector<Object3D*>* const& scene_objects)
{
	try {
		int line_number = 0;

		std::ifstream scenefile(filename);

		std::string first_line = getTrimmedLineFromFile(&scenefile, &line_number);
		int entity_count = std::stoi(first_line);

		// read in the number of entities specified at the top of the file
		for (int i = entity_count; i--; ) {
			std::string entity_type = getTrimmedLineFromFile(&scenefile, &line_number);

			// for holding onto data while it's being parsed
			std::string temp;

			// different behavior depending on entity type
			if (entity_type == "camera") {
				for (int j = 0; j < 4; j++) {
					std::getline(scenefile, temp);
				}
			} else if (entity_type == "light") {
				for (int j = 0; j < 2; j++) {
					std::getline(scenefile, temp);
				}
			} else if (entity_type == "sphere") {
				for (int j = 0; j < 6; j++) {
					std::getline(scenefile, temp);
				}
			} else if (entity_type == "plane") {
				for (int j = 0; j < 6; j++) {
					std::getline(scenefile, temp);
				}
			} else if (entity_type == "triangle") {
				for (int j = 0; j < 7; j++) {
					std::getline(scenefile, temp);
				}
			} else if (entity_type == "model") {
				// this is always on the line after 'model'
				std::string obj_filename = getTrimmedLineFromFile(&scenefile, &line_number);
				// get rid of quotation marks
				if (obj_filename[0] == '"' && obj_filename[obj_filename.length() - 1] == '"') {
					obj_filename = obj_filename.substr(1, obj_filename.length() - 2);
				}

				// to be read in
				glm::vec3 ambient_color, diffuse_color, specular_color;
				float shininess = 0.0f;

				// will be marked true if corresponding values are read from file
				bool amb = false, dif = false, spe = false, shi = false;

				for (int j = 0; j < 4; j++) {
					temp = getTrimmedLineFromFile(&scenefile, &line_number);
					unsigned long colon_pos = temp.find(':');
					std::string fieldname = temp.substr(0, temp.find(':'));
					if (!(fieldname == "amb" || fieldname == "dif" ||
						fieldname == "spe" || fieldname == "shi")) {
						throw unknownFieldError(fieldname, filename, line_number);
					}
					if ((fieldname == "amb" && amb) || (fieldname == "dif" && dif) ||
						(fieldname == "spe" && spe) || (fieldname == "shi" && shi)) {
						throw duplicateFieldError(fieldname, filename, line_number);
					}
					if (fieldname == "shi") {
						shi = true;
						shininess = stof(getTrimmedSubstring(temp, colon_pos + 1));
					} else {
						if (fieldname == "amb") amb = true;
						else if (fieldname == "dif") dif = true;
						else spe = true;
						float r, g, b;
						int matches = sscanf(
							getTrimmedSubstring(temp, colon_pos + 1).c_str(),
							"%f %f %f",
							&r, &g, &b
						);
						if (matches != 3) {
							throw deformedFieldError(fieldname, filename, line_number);
						}
						glm::vec3 color(r, g, b);
						if (fieldname == "amb") ambient_color = color;
						else if (fieldname == "dif") diffuse_color = color;
						else specular_color = color;
					}
				}
				if (!amb) {
					throw missingFieldError("amb", filename, line_number);
				}
				if (!dif) {
					throw missingFieldError("dif", filename, line_number);
				}
				if (!spe) {
					throw missingFieldError("spe", filename, line_number);
				}
				if (!shi) {
					throw missingFieldError("shi", filename, line_number);
				}

				scene_objects->push_back(
					new ObjModel(
						(models_dir / fs::path(obj_filename)).string(),
						ambient_color,
						diffuse_color,
						specular_color,
						shininess
					)
				);
			} else {
				throw std::runtime_error("Unknown entity type '" + entity_type + "'.");
			}
		}
	} catch(error_t e) {
		std::cerr << "Scene loading failed!" << std::endl;
		throw e;
	}
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
