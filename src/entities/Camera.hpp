#ifndef RAYTRACER_CAMERA_HPP
#define RAYTRACER_CAMERA_HPP

#include <glm/glm.hpp>
#include <vector>

// always points along negative-Z axis

class Camera {
private:
	glm::vec3 position;
	float fov_y; // radians
	float focal_length;
	float aspect_ratio; // width / height
	unsigned long pixel_width;
	unsigned long pixel_height;
	std::vector<glm::vec3> rays;
public:
	Camera() : Camera(glm::vec3(0.0f, 0.0f, 0.0f), (float)M_PI / 4, 1.0f, 1.3) {}
	Camera(const glm::vec3& position, float fov_y, float focal_length, float aspect_ratio);
	glm::vec3 getPosition() const;
	const std::vector<glm::vec3>& getRays(
		unsigned long* const& pixel_width,
		unsigned long* const& pixel_height
	) const;
};


#endif //RAYTRACER_CAMERA_HPP
