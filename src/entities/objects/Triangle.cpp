#include <glm/glm.hpp>
#include <cmath>

#include <src/constants.hpp>

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

bool Triangle::doesRayIntersect(
	const glm::vec3& origin,
	const glm::vec3& direction,
	float* const& t,
	glm::vec3* const& normal
) const
{
	// ray-plane intersected based on slides 11-12 at:
	// http://poullis.org/courses/2017/Fall/COMP371/resources/Lecture%2016_%20Ray%20Tracing_Geometric%20Queries.pdf

	float normal_dot_position = glm::dot(this->normal, *this->vertex1);

	float normal_dot_origin = glm::dot(this->normal, origin);

	float normal_dot_direction = glm::dot(this->normal, direction);

	// negative sign negated since camera looks down negative-Z axis
	// (thanks https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
	// for the tip)
	// TODO: take look direction into account to handle general case
	*t = (normal_dot_origin + normal_dot_position) / normal_dot_direction;
	if (*t < t_threshold || std::isnan(*t)) {
		return false;
	}

	glm::vec3 point = origin + direction * *t;

	// Inside-Outside test based on geometric solution at:
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution

	glm::vec3 edge1_2 = *this->vertex2 - *this->vertex1;
	glm::vec3 vec1_p = point - *this->vertex1;
	if (glm::dot(this->normal, glm::cross(edge1_2, vec1_p)) < 0) {
		// wrong side
		return false;
	}

	glm::vec3 edge2_3 = *this->vertex3 - *this->vertex2;
	glm::vec3 vec2_p = point - *this->vertex2;
	if (glm::dot(this->normal, glm::cross(edge2_3, vec2_p)) < 0) {
		// wrong side
		return false;
	}

	glm::vec3 edge3_1 = *this->vertex1 - *this->vertex3;
	glm::vec3 vec3_p = point - *this->vertex3;
	if (glm::dot(this->normal, glm::cross(edge3_1, vec3_p)) < 0) {
		// wrong side
		return false;
	}

	// TODO: use barycentric coordinates to interpolate lighting normals from 3 points!
	// (requires reading normals for vertices, otherwise this is pointless)
	*normal = this->normal;

	return true;
}

void Triangle::setNormal()
{
	this->normal = glm::normalize(
		glm::cross(*this->vertex2 - *this->vertex1, *this->vertex3 - *this->vertex1)
	);
}
