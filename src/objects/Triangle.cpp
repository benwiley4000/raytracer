#include <glm/glm.hpp>

#include "Object3D.hpp"
#include "Triangle.hpp"

Triangle::Triangle(
	const glm::vec3& vertex1,
	const glm::vec3& vertex2,
	const glm::vec3& vertex3,
	const glm::vec3& ambient_color,
	const glm::vec3& diffuse_color,
	const glm::vec3& specular_color,
	const float& shininess
) : Object3D(ambient_color, diffuse_color, specular_color, shininess)
{
	this->vertex1 = new glm::vec3(vertex1);
	this->vertex2 = new glm::vec3(vertex2);
	this->vertex3 = new glm::vec3(vertex3);
	this->setNormal();
	this->using_external_vertices = false;
}

Triangle::Triangle(
	glm::vec3* vertex1,
	glm::vec3* vertex2,
	glm::vec3* vertex3,
	Object3D* const& material_parent
) : Object3D(material_parent)
{
	this->vertex1 = vertex1;
	this->vertex2 = vertex2;
	this->vertex3 = vertex3;
	this->setNormal();
	this->using_external_vertices = true;
}

Triangle::~Triangle()
{
	if (!this->using_external_vertices) {
		delete this->vertex1;
		delete this->vertex2;
		delete this->vertex3;
	}
}

void Triangle::setNormal()
{
	this->normal = glm::normalize(
		glm::cross(*this->vertex2 - *this->vertex1, *this->vertex3 - *this->vertex1)
	);
}
