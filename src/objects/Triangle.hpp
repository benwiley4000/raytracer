#ifndef RAYTRACER_TRIANGLE_HPP
#define RAYTRACER_TRIANGLE_HPP

#include <glm/glm.hpp>

#include "Object3D.hpp"

class Triangle : public Object3D  {
private:
	glm::vec3* v0;
	glm::vec3* v1;
	glm::vec3* v2;
	glm::vec3 normal;
	bool using_external_vertices;
	void setNormal();
public:
	Triangle(
		const glm::vec3& v0,
		const glm::vec3& v1,
		const glm::vec3& v2,
		const glm::vec3& ambient_color,
		const glm::vec3& diffuse_color,
		const glm::vec3& specular_color,
		const float& shininess
	);
	Triangle(
		glm::vec3* v0,
		glm::vec3* v1,
		glm::vec3* v2,
		Object3D* const& material_parent
	);
	~Triangle() override;
};


#endif //RAYTRACER_TRIANGLE_HPP
