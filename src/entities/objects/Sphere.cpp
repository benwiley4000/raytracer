#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>

#include <src/constants.hpp>

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

bool Sphere::doesRayIntersect(
	const glm::vec3& origin,
	const glm::vec3& direction,
	float* const& t,
	glm::vec3* const& normal
) const
{
	// geometric solution based on explation at:
	// https://www.scratchapixel.com/code.php?id=3&origin=/lessons/3d-basic-rendering/introduction-to-ray-tracing
	// TODO: implement analytic solution based on quadratic formula (previous implementations produced undesired visual artifacts)

	// a vector corresponding to segment from origin to sphere center
	glm::vec3 vec_to_sphere_center = this->position - origin;
	// t corresponding to point lying on sphere's center axis
	float t_center_axis = glm::dot(vec_to_sphere_center, direction);
	if (t_center_axis < t_threshold) {
		// if the sphere center is too low we shouldn't declare an intersection
		return false;
	}
	// the square of d, the distance from the sphere center to the cast ray
	float d_squared =
		glm::dot(vec_to_sphere_center, vec_to_sphere_center) - pow(t_center_axis, 2);
	if (d_squared > this->radius * this->radius) {
		// if d squared is greater than the radius squared, that would push the ray
		// outside the bounds of the sphere, so no intersection.
		return false;
	}

	// there is an intersection!

	// find t increment *just* for the segment between the intersection point
	// and the center axis
	auto t_from_point_to_center_axis =
		(float)sqrt(this->radius * this->radius - d_squared);
	*t = std::min(
		t_center_axis - t_from_point_to_center_axis,
		t_center_axis + t_from_point_to_center_axis
	);

	// compute normal pointing from sphere center to intersection point
	*normal = glm::normalize((origin + direction * *t) - this->position);

	return true;
}
