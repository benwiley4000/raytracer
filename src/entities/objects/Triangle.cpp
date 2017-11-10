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
	// formula based on slides 11-12 at:
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

	glm::vec3 point3 = origin + direction * *t;

	/*

	glm::vec2 point2;

	glm::vec2 projected_vertex1;
	glm::vec2 projected_vertex2;
	glm::vec2 projected_vertex3;
	bool all_x_equal = this->vertex1->x == this->vertex2->x && this->vertex1->x == this->vertex3->x;
	bool all_y_equal = this->vertex1->y == this->vertex2->y && this->vertex1->y == this->vertex3->y;
	if (all_x_equal) {
		// x = 0
		projected_vertex1 = glm::vec2(this->vertex1->y, this->vertex1->z);
		projected_vertex2 = glm::vec2(this->vertex2->y, this->vertex2->z);
		projected_vertex3 = glm::vec2(this->vertex3->y, this->vertex3->z);
		point2 = glm::vec2(point3.y, point3.z);
	} else if (all_y_equal) {
		// y = 0
		projected_vertex1 = glm::vec2(this->vertex1->x, this->vertex1->z);
		projected_vertex2 = glm::vec2(this->vertex2->x, this->vertex2->z);
		projected_vertex3 = glm::vec2(this->vertex3->x, this->vertex3->z);
		point2 = glm::vec2(point3.x, point3.z);
	} else {
		// z = 0
		projected_vertex1 = glm::vec2(this->vertex1->x, this->vertex1->y);
		projected_vertex2 = glm::vec2(this->vertex2->x, this->vertex2->y);
		projected_vertex3 = glm::vec2(this->vertex3->x, this->vertex3->y);
		point2 = glm::vec2(point3.x, point3.y);
	}

	// Thanks to http://blackpawn.com/texts/pointinpoly/ for help thinking about
	// barycentric coordinates!

	glm::vec2 v0 = projected_vertex3 - projected_vertex1;
	glm::vec2 v1 = projected_vertex2 - projected_vertex1;

	glm::vec2 point_vec = point2 - projected_vertex1;

	float zero_dot_zero = glm::dot(v0, v0);
	float zero_dot_one = glm::dot(v0, v1);
	float zero_dot_point_vec = glm::dot(v0, point_vec);
	float one_dot_one = glm::dot(v1, v1);
	float one_dot_point_vec = glm::dot(v1, point_vec);

	// Compute barycentric coordinates
	float inverse_denominator =
		1.0f / float(zero_dot_zero * one_dot_one - pow(zero_dot_one, 2));
	float u = (one_dot_one * zero_dot_point_vec - zero_dot_one * one_dot_point_vec) *
		inverse_denominator;
	float v = (zero_dot_zero * one_dot_point_vec - zero_dot_one * zero_dot_point_vec) *
		inverse_denominator;

	// TODO: use barycentric coordinates to interpolate normals from 3 points!
	// (requires reading normals for vertices, otherwise this is pointless)
	*normal = this->normal;

	// Is the plane intersection point actually in the triangle?
	return 0 <= u && u < 1 && 0 <= v && v < 1;

	 */

	// temp solution from:
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution

	// Step 2: inside-outside test
	glm::vec3 C; // vector perpendicular to triangle's plane

	// edge 0
	glm::vec3 edge0 = *this->vertex2 - *this->vertex1;
	glm::vec3 vp0 = point3 - *this->vertex1;
	C = glm::cross(edge0, vp0);
	if (glm::dot(this->normal, C) < 0) return false; // P is on the right side

	// edge 1
	glm::vec3 edge1 = *this->vertex3 - *this->vertex2;
	glm::vec3 vp1 = point3 - *this->vertex2;
	C = glm::cross(edge1, vp1);
	if (glm::dot(this->normal, C) < 0)  return false; // P is on the right side

	// edge 2
	glm::vec3 edge2 = *this->vertex1 - *this->vertex3;
	glm::vec3 vp2 = point3 - *this->vertex3;
	C = glm::cross(edge2, vp2);
	if (glm::dot(this->normal, C) < 0) return false; // P is on the right side;

	return true;
}

void Triangle::setNormal()
{
	this->normal = glm::normalize(
		glm::cross(*this->vertex2 - *this->vertex1, *this->vertex3 - *this->vertex1)
	);
}
