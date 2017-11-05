#ifndef RAYTRACER_LOADSCENE_HPP
#define RAYTRACER_LOADSCENE_HPP

#include <string>
#include <vector>
#include <stdexcept>

#include "entities/Object3D.hpp"

void loadScene(const std::string& filename, std::vector<Object3D*>* const& scene_objects);

#endif //RAYTRACER_LOADSCENE_HPP
