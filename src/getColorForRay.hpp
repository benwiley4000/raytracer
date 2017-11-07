#ifndef RAYTRACER_GETCOLORFORRAY_HPP
#define RAYTRACER_GETCOLORFORRAY_HPP

#include <glm/vec3.hpp>
#include <vector>

#include "entities/Light.hpp"
#include "entities/objects/Object3D.hpp"

glm::vec3 getColorForRay(
	const glm::vec3& origin,
	const glm::vec3& direction,
	const std::vector<Light>& lights,
	const std::vector<Object3D*>& scene_objects
);

#endif //RAYTRACER_GETCOLORFORRAY_HPP
