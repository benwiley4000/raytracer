#include <glm/glm.hpp>

#include "Object3D.hpp"

Object3D::Object3D(
	const glm::vec3& ambient_color,
	const glm::vec3& diffuse_color,
	const glm::vec3& specular_color,
	const float& shininess
) {
	this->ambient_color = new glm::vec3(ambient_color);
	this->diffuse_color = new glm::vec3(diffuse_color);
	this->specular_color = new glm::vec3(specular_color);
	this->shininess = shininess;
	this->material_parent = nullptr;
}

Object3D::Object3D(const Object3D* const& material_parent)
{
	this->ambient_color = material_parent->ambient_color;
	this->diffuse_color = material_parent->diffuse_color;
	this->specular_color = material_parent->specular_color;
	this->shininess = material_parent->shininess;
	this->material_parent = material_parent;
}

Object3D::~Object3D()
{
	if (!this->material_parent) {
		delete this->ambient_color;
		delete this->diffuse_color;
		delete this->specular_color;
	}
}
