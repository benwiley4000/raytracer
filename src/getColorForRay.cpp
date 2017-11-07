#include <glm/vec3.hpp>
#include <vector>

#include "entities/Light.hpp"
#include "entities/objects/Object3D.hpp"

glm::vec3 getColorForRay(
	const glm::vec3& origin,
	const glm::vec3& direction,
	const std::vector<Light>& lights,
	const std::vector<Object3D*>& scene_objects
) {
	glm::vec3 accumulated_color(0.3f, 0.5f, 0.7f);

	// TODO: get real color

	return accumulated_color;
}
