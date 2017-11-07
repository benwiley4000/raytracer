#ifndef RAYTRACER_OBJECT3D_HPP
#define RAYTRACER_OBJECT3D_HPP

#include <glm/glm.hpp>

// abstract class

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
	glm::vec3 getAmbientColor() const;
	glm::vec3 getDiffuseColor() const;
	glm::vec3 getSpecularColor() const;
	float getShininess() const;
	bool isBlockingSegment(const glm::vec3& point_a, const glm::vec3& point_b) const;
	// if return is true, *t is set to value of t in
	// ray parametric equation: p(t) = origin + direction * t
	virtual bool doesRayIntersect(
		const glm::vec3& origin,
		const glm::vec3& direction,
		float* const& t,
		glm::vec3* const& normal
	) const = 0;
};


#endif //RAYTRACER_OBJECT3D_HPP
