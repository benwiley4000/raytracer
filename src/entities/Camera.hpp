#ifndef RAYTRACER_CAMERA_HPP
#define RAYTRACER_CAMERA_HPP

#include <glm/glm.hpp>
#include <vector>
#include <utility>

// always points along negative-Z axis

class Camera {
private:
	glm::vec3 position;
	float fov_y; // radians
	float focal_length;
	float aspect_ratio; // width / height
	unsigned int pixel_width;
	unsigned int pixel_height;
	// randomly-ordered vector, containing pairs of:
	//  - ray direction vector
	//  - int indicating position in image
	std::vector<std::pair<glm::vec3, int>> rays;
public:
	Camera() : Camera(glm::vec3(0.0f, 0.0f, 0.0f), (float)M_PI / 4, 1.0f, 1.3) {}
	Camera(const glm::vec3& position, float fov_y, float focal_length, float aspect_ratio);
	glm::vec3 getPosition() const;
	const std::vector<std::pair<glm::vec3, int>>& getRays(
		unsigned int* const& pixel_width,
		unsigned int* const& pixel_height
	) const;
};


#endif //RAYTRACER_CAMERA_HPP
