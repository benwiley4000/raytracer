#include <glm/glm.hpp>

#include "Object3D.hpp"
#include "Sphere.hpp"

Sphere::Sphere(
	const glm::vec3& position,
	const float& radius,
	const glm::vec3& ambient_color,
	const glm::vec3& diffuse_color,
	const glm::vec3& specular_color,
	const float& shininess
) : Object3D(ambient_color, diffuse_color, specular_color, shininess),
    position(position),
    radius(radius) {}
