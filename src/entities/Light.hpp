#ifndef RAYTRACER_LIGHT_HPP
#define RAYTRACER_LIGHT_HPP

#include <glm/glm.hpp>

class Light {
private:
	glm::vec3 position;
	glm::vec3 color;
public:
	Light(const glm::vec3& position, const glm::vec3& color);
	glm::vec3 getPosition() const;
	glm::vec3 getColor() const;
};


#endif //RAYTRACER_LIGHT_HPP
