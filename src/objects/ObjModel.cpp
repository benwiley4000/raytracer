#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <stdexcept>

#include "../vendor/tiny_obj_loader.cc"

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

	tinyobj::shape_t shape = shapes[0];

	// Loop over faces (polygon)
	size_t index_offset = 0;
	for (int fv : shape.mesh.num_face_vertices) {
		// Loop over vertices in the face
		for (size_t v = 0; v < fv; v++) {
			// access to vertex
			tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
			this->vertices.emplace_back(
				attrib.vertices[3 * idx.vertex_index + 0],
				attrib.vertices[3 * idx.vertex_index + 1],
				attrib.vertices[3 * idx.vertex_index + 2]
			);
		}
		// Tessellate face into triangles
		for (size_t i = index_offset + 1, last = index_offset + fv - 1; i < last; i++) {
			this->triangles.emplace_back(
				&this->vertices[index_offset],
				&this->vertices[i],
				&this->vertices[i + 1],
				this
			);
		}
		index_offset += fv;
	}
}
