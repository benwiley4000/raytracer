#include <glm/glm.hpp>

#include "Object3D.hpp"
#include "Triangle.hpp"

Triangle::Triangle(
	const glm::vec3& v0,
	const glm::vec3& v1,
	const glm::vec3& v2,
	const glm::vec3& ambient_color,
	const glm::vec3& diffuse_color,
	const glm::vec3& specular_color,
	const float& shininess
) : Object3D(ambient_color, diffuse_color, specular_color, shininess)
{
	this->v0 = new glm::vec3(v0);
	this->v1 = new glm::vec3(v1);
	this->v2 = new glm::vec3(v2);
	this->setNormal();
	this->using_external_vertices = false;
}

Triangle::Triangle(
	glm::vec3* v0,
	glm::vec3* v1,
	glm::vec3* v2,
	Object3D* const& material_parent
) : Object3D(material_parent)
{
	this->v0 = v0;
	this->v1 = v1;
	this->v2 = v2;
	this->setNormal();
	this->using_external_vertices = true;
}

Triangle::~Triangle()
{
	if (!this->using_external_vertices) {
		delete this->v0;
		delete this->v1;
		delete this->v2;
	}
}

void Triangle::setNormal()
{
	this->normal = glm::normalize(glm::cross(*this->v1 - *this->v0, *this->v2 - *this->v0));
}
