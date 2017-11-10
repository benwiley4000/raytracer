#include <glm/glm.hpp>
#include <cmath>

#include <src/constants.hpp>

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

bool Plane::doesRayIntersect(
	const glm::vec3& origin,
	const glm::vec3& direction,
	float* const& t,
	glm::vec3* const& normal
) const
{
	*normal = this->normal;

	// formula based on slides 11-12 at:
	// http://poullis.org/courses/2017/Fall/COMP371/resources/Lecture%2016_%20Ray%20Tracing_Geometric%20Queries.pdf

	float normal_dot_position = glm::dot(this->normal, this->position);

	float normal_dot_origin = glm::dot(this->normal, origin);

	float normal_dot_direction = glm::dot(this->normal, direction);

	// negative sign negated since camera looks down negative-Z axis
	// (thanks https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
	// for the tip)
	// TODO: take look direction into account to handle general case
	*t = (normal_dot_origin + normal_dot_position) / normal_dot_direction;

	return *t >= t_threshold && !std::isnan(*t);
}
