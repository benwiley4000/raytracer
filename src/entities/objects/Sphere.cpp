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
	// formula based on slides 5-6 at:
	// http://poullis.org/courses/2017/Fall/COMP371/resources/Lecture%2016_%20Ray%20Tracing_Geometric%20Queries.pdf
	double b = 2 * (
		              direction.x * (origin.x - this->position.x) +
		              direction.y * (origin.y - this->position.y) +
		              direction.z * (origin.z - this->position.z)
	              );
	double c = pow(origin.x - this->position.x, 2) +
	           pow(origin.y - this->position.y, 2) +
	           pow(origin.z - this->position.z, 2) +
	           pow(this->radius, 2);
	double sqrt_discriminant = sqrt(pow(b, 2) - 4 * c);
	// get whichever intersection t value is smallest (may be smaller than origin)
	*t = std::min((-b + sqrt_discriminant) / 2, (-b - sqrt_discriminant) / 2);

	*normal = (1 / this->radius) * (origin + direction * *t) - this->position;

	return *t >= t_threshold;
}
