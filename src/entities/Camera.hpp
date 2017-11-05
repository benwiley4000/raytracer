#ifndef RAYTRACER_CAMERA_HPP
#define RAYTRACER_CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
private:
	glm::vec3 position;
	float fov_y; // radians
	float focal_length;
	float aspect_ratio; // width / height
public:
	Camera() : Camera(glm::vec3(0.0f, 0.0f, 0.0f), (float)M_PI / 4, 1.0f, 1.3) {}
	Camera(const glm::vec3& position, float fov_y, float focal_length, float aspect_ratio);
};


#endif //RAYTRACER_CAMERA_HPP
