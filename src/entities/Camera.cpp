#include <glm/glm.hpp>

#include "Camera.hpp"

Camera::Camera(
	const glm::vec3& position,
	float fov_y,
	float focal_length,
	float aspect_ratio
) : position(position),
    fov_y(fov_y),
    focal_length(focal_length),
    aspect_ratio(aspect_ratio) {}
