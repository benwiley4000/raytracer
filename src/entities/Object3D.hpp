#ifndef RAYTRACER_OBJECT3D_HPP
#define RAYTRACER_OBJECT3D_HPP

#include <glm/glm.hpp>

class Object3D {
private:
	glm::vec3* ambient_color;
	glm::vec3* diffuse_color;
	glm::vec3* specular_color;
	float shininess;
	const Object3D* material_parent;
public:
	Object3D(
		const glm::vec3& ambient_color,
		const glm::vec3& diffuse_color,
		const glm::vec3& specular_color,
		const float& shininess
	);
	explicit Object3D(const Object3D* const& material_parent);
	virtual ~Object3D();
};


#endif //RAYTRACER_OBJECT3D_HPP
