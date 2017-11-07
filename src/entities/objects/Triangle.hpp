#ifndef RAYTRACER_TRIANGLE_HPP
#define RAYTRACER_TRIANGLE_HPP

#include <glm/glm.hpp>

#include "Object3D.hpp"

class Triangle : public Object3D  {
private:
	glm::vec3* vertex1;
	glm::vec3* vertex2;
	glm::vec3* vertex3;
	glm::vec3 normal;
	bool using_external_vertices;
	void setNormal();
public:
	Triangle(
		const glm::vec3& vertex1,
		const glm::vec3& vertex2,
		const glm::vec3& vertex3,
		const glm::vec3& ambient_color,
		const glm::vec3& diffuse_color,
		const glm::vec3& specular_color,
		const float& shininess
	);
	Triangle(
		glm::vec3* vertex1,
		glm::vec3* vertex2,
		glm::vec3* vertex3,
		Object3D* const& material_parent
	);
	~Triangle() override;
	bool doesRayIntersect(
		const glm::vec3& origin,
		const glm::vec3& direction,
		float* const& t,
		glm::vec3* const& normal
	) const override;
};


#endif //RAYTRACER_TRIANGLE_HPP
