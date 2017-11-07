#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>

#include <src/vendor/tiny_obj_loader.cc>

#include "Object3D.hpp"
#include "ObjModel.hpp"

ObjModel::ObjModel(
	const std::string& filename,
	const glm::vec3& ambient_color,
	const glm::vec3& diffuse_color,
	const glm::vec3& specular_color,
	const float& shininess
) : Object3D(ambient_color, diffuse_color, specular_color, shininess)
{
	// .obj loading code adapted from sample usage code at:
	// https://github.com/syoyo/tinyobjloader/blob/b434c2497fcb52aa1497b84aa8aeb12bb590492d/README.md#example-code

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str());

	if (!err.empty()) {
		throw std::runtime_error("Obj load failed: " + err);
	}

	if (shapes.empty()) {
		throw std::runtime_error("Obj must contain shape!");
	}

	if (!ret) {
		throw std::runtime_error("Obj load failed.");
	}

	// populate this->vertices
	for (size_t i = 0, len = attrib.vertices.size(); i < len; i += 3) {
		this->vertices.emplace_back(i, i + 1, i + 2);
	}

	tinyobj::shape_t shape = shapes[0];

	// Loop over faces (polygon)
	size_t index_offset = 0;
	for (int fv : shape.mesh.num_face_vertices) {
		std::vector<size_t> vertex_indices;
		// Loop over vertices in the face
		for (size_t v = 0; v < fv; v++) {
			vertex_indices.emplace_back(shape.mesh.indices[index_offset + v].vertex_index);
		}
		// Tessellate face into triangles
		this->tessellateFace(vertex_indices);
		index_offset += fv;
	}
}

bool ObjModel::doesRayIntersect(
	const glm::vec3& origin,
	const glm::vec3& direction,
	float* const& t,
	glm::vec3* const& normal
) const
{
	*t = std::numeric_limits<float>::max();

	bool does_intersect = false;

	float temp_t;
	glm::vec3 temp_normal;
	// find nearest intersection point among triangles forming tessellation
	for (const Triangle& tri : this->triangles) {
		if (tri.doesRayIntersect(origin, direction, &temp_t, &temp_normal) && temp_t < *t) {
			*t = temp_t;
			*normal = temp_normal;
			does_intersect = true;
		}
	}

	return does_intersect;
}

void ObjModel::tessellateFace(const std::vector<size_t>& vertex_indices)
{
	// Triangle fan tessellation (TODO: smarter tessellation)
	for (size_t i = 1, last = vertex_indices.size() - 1; i < last; i++) {
		this->triangles.emplace_back(
			&this->vertices[vertex_indices[0]],
			&this->vertices[vertex_indices[i]],
			&this->vertices[vertex_indices[i + 1]],
			this
		);
	}
}
