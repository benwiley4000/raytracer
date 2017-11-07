#ifndef RAYTRACER_SPHERE_HPP
#define RAYTRACER_SPHERE_HPP

#include <glm/glm.hpp>

#include "Object3D.hpp"

class Sphere : public Object3D  {
private:
	glm::vec3 position;
	float radius;
public:
	Sphere(
		const glm::vec3& position,
		const float& radius,
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


#endif //RAYTRACER_SPHERE_HPP
