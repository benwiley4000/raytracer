#ifndef RAYTRACER_OBJMODEL_HPP
#define RAYTRACER_OBJMODEL_HPP

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Object3D.hpp"
#include "Triangle.hpp"

// .obj model requirements:
// * at least one shape (first shape will be used, others discarded)
// * specification of faces by vertices
// * all other information will be ignored

class ObjModel : public Object3D {
private:
	std::vector<glm::vec3> vertices;
	std::vector<Triangle> triangles;
public:
	explicit ObjModel(
		const std::string& filename,
		const glm::vec3& ambient_color,
		const glm::vec3& diffuse_color,
		const glm::vec3& specular_color,
		const float& shininess
	);
};


#endif //RAYTRACER_OBJMODEL_HPP
