#ifndef RAYTRACER_PLANE_HPP
#define RAYTRACER_PLANE_HPP

#include <glm/glm.hpp>

#include "Object3D.hpp"

class Plane : public Object3D  {
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
};


#endif //RAYTRACER_PLANE_HPP
