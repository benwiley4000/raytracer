#include <glm/glm.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <iostream>

#include "objects/Object3D.hpp"
#include "objects/ObjModel.hpp"
#include "constants.hpp"
#include "loadScene.hpp"

namespace fs = boost::filesystem;

namespace scl {
	std::string
	getTrimmedLineFromFile(std::ifstream* const& file, int* const& line_number)
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
	)
	{
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
	)
	{
		return std::runtime_error(
			"Field '" + fieldname + "' " + bad_state + " (" +
			filename + " line " + std::to_string(line_number) + ")"
		);
	}

	std::runtime_error unknownFieldError(
		const std::string& fieldname,
		const std::string& filename,
		const int& line_number
	)
	{
		return scenefileFieldError(fieldname, filename, line_number, "unknown");
	}

	std::runtime_error duplicateFieldError(
		const std::string& fieldname,
		const std::string& filename,
		const int& line_number
	)
	{
		return scenefileFieldError(fieldname, filename, line_number, "already defined");
	}

	std::runtime_error missingFieldError(
		const std::string& fieldname,
		const std::string& filename,
		const int& line_number
	)
	{
		return scenefileFieldError(fieldname, filename, line_number, "missing");
	}

	std::runtime_error deformedFieldError(
		const std::string& fieldname,
		const std::string& filename,
		const int& line_number
	)
	{
		return scenefileFieldError(fieldname, filename, line_number, "deformed");
	}
}

void loadScene(const std::string& filename, std::vector<Object3D*>* const& scene_objects)
{
	try {
		int line_number = 0;

		std::ifstream scenefile(filename);

		std::string first_line = scl::getTrimmedLineFromFile(&scenefile, &line_number);
		int entity_count = std::stoi(first_line);

		// read in the number of entities specified at the top of the file
		for (int i = entity_count; i--; ) {
			std::string entity_type = scl::getTrimmedLineFromFile(&scenefile, &line_number);

			// for holding onto data while it's being parsed
			std::string temp;

			// different behavior depending on entity type
			if (entity_type == "camera") {
				for (int j = 0; j < 4; j++) {
					std::getline(scenefile, temp);
					line_number++;
				}
			} else if (entity_type == "light") {
				for (int j = 0; j < 2; j++) {
					std::getline(scenefile, temp);
					line_number++;
				}
			} else if (entity_type == "sphere") {
				for (int j = 0; j < 6; j++) {
					std::getline(scenefile, temp);
					line_number++;
				}
			} else if (entity_type == "plane") {
				for (int j = 0; j < 6; j++) {
					std::getline(scenefile, temp);
					line_number++;
				}
			} else if (entity_type == "triangle") {
				for (int j = 0; j < 7; j++) {
					std::getline(scenefile, temp);
					line_number++;
				}
			} else if (entity_type == "model") {
				// this is always on the line after 'model'
				std::string obj_filename = scl::getTrimmedLineFromFile(&scenefile, &line_number);
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
					temp = scl::getTrimmedLineFromFile(&scenefile, &line_number);
					unsigned long colon_pos = temp.find(':');
					std::string fieldname = temp.substr(0, temp.find(':'));
					if (!(fieldname == "amb" || fieldname == "dif" ||
					      fieldname == "spe" || fieldname == "shi")) {
						throw scl::unknownFieldError(fieldname, filename, line_number);
					}
					if ((fieldname == "amb" && amb) || (fieldname == "dif" && dif) ||
					    (fieldname == "spe" && spe) || (fieldname == "shi" && shi)) {
						throw scl::duplicateFieldError(fieldname, filename, line_number);
					}
					if (fieldname == "shi") {
						shi = true;
						shininess = std::stof(scl::getTrimmedSubstring(temp, colon_pos + 1));
					} else {
						if (fieldname == "amb") amb = true;
						else if (fieldname == "dif") dif = true;
						else spe = true;
						float r, g, b;
						int matches = sscanf(
							scl::getTrimmedSubstring(temp, colon_pos + 1).c_str(),
							"%f %f %f",
							&r, &g, &b
						);
						if (matches != 3) {
							throw scl::deformedFieldError(fieldname, filename, line_number);
						}
						glm::vec3 color(r, g, b);
						if (fieldname == "amb") ambient_color = color;
						else if (fieldname == "dif") diffuse_color = color;
						else specular_color = color;
					}
				}
				if (!amb) {
					throw scl::missingFieldError("amb", filename, line_number);
				}
				if (!dif) {
					throw scl::missingFieldError("dif", filename, line_number);
				}
				if (!spe) {
					throw scl::missingFieldError("spe", filename, line_number);
				}
				if (!shi) {
					throw scl::missingFieldError("shi", filename, line_number);
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
