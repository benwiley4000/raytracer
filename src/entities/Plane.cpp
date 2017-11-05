#include <glm/glm.hpp>

#include "Object3D.hpp"
#include "Plane.hpp"

Plane::Plane(
	const glm::vec3& normal,
	const glm::vec3& position,
	const glm::vec3& ambient_color,
	const glm::vec3& diffuse_color,
	const glm::vec3& specular_color,
	const float& shininess
) : Object3D(ambient_color, diffuse_color, specular_color, shininess),
    normal(normal),
    position(position) {}
