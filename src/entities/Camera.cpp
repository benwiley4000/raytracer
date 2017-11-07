#include <glm/glm.hpp>
#include <vector>
#include <cmath>

#include "Camera.hpp"

Camera::Camera(
	const glm::vec3& position,
	float fov_y,
	float focal_length,
	float aspect_ratio
) : position(position),
    fov_y(fov_y),
    focal_length(focal_length),
    aspect_ratio(aspect_ratio)
{
	// negative-Z axis unit vector
	static glm::vec3 forward(0.0f, 0.0f, -1.0f);

	double height = 2 * focal_length * tan((double)fov_y / 2.0);
	double width = aspect_ratio * height;

	this->pixel_height = (unsigned long)round(height);
	this->pixel_width = (unsigned long)round(width);

	glm::vec3 image_center = position + forward * focal_length;
	// bottom_left is a CENTER of bottom left pixel, not at its bottom left corner.
	glm::vec3 bottom_left = image_center -
		glm::vec3(this->pixel_width / 2.0f + 0.5f, this->pixel_height / 2.0f + 0.5f, 0.0f);

	for (int row = 0; row < this->pixel_height; row++) {
		for (int col = 0; col < this->pixel_width; col++) {
			this->rays.push_back(
				// unit vector pointing in direction from camera position to pixel
				glm::normalize(
					glm::vec3(bottom_left.x + col, bottom_left.y + row, bottom_left.z) -
						this->position
				)
			);
		}
	}
}

glm::vec3 Camera::getPosition() const
{
	return this->position;
}

const std::vector<glm::vec3>& Camera::getRays(
	unsigned long* const& pixel_width,
	unsigned long* const& pixel_height
) const
{
	(*pixel_width) = this->pixel_width;
	(*pixel_height) = this->pixel_height;
	return this->rays;
}
