#ifndef RAYTRACER_PLANE_HPP
#define RAYTRACER_PLANE_HPP

#include <glm/glm.hpp>

#include "Object3D.hpp"
#include "Triangle.hpp"

class Plane : public Object3D  {
	friend Triangle;
private:
	glm::vec3 normal;
	glm::vec3 position;
public:
	Plane(
		const glm::vec3& normal,
		const glm::vec3& position,
		const glm::vec3& ambient_color,
		const glm::vec3& diffuse_color,
		const glm::vec3& specular_color,
		const float& shininess
	);
	bool doesRayIntersect(
		const glm::vec3& origin,
		const glm::vec3& direction,
		float* const& t,
		glm::vec3* const& normal
	) const override;
};


#endif //RAYTRACER_PLANE_HPP
