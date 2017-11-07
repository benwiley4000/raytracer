#include <glm/vec3.hpp>
#include <vector>
#include <limits>
#include <algorithm>

#include "entities/Light.hpp"
#include "entities/objects/Object3D.hpp"

glm::vec3 getColorForRay(
	const glm::vec3& origin,
	const glm::vec3& direction,
	const std::vector<Light>& lights,
	const std::vector<Object3D*>& scene_objects
) {
	glm::vec3 accumulated_color(0.0f, 0.0f, 0.0f);

	bool does_intersect = false;

	float t = std::numeric_limits<float>::max();
	glm::vec3 normal;
	Object3D* illuminated_object = nullptr;

	float temp_t;
	glm::vec3 temp_normal;
	for (Object3D* const& object : scene_objects) {
		if (object->doesRayIntersect(origin, direction, &temp_t, &temp_normal)) {
			t = temp_t;
			normal = temp_normal;
			illuminated_object = object;
			does_intersect = true;
		}
	}

	if (!does_intersect) {
		return accumulated_color;
	}

	glm::vec3 point = origin + direction * t;

	glm::vec3 viewer_unit_vector = -direction;

	// Phong illumination model

	for (const Light& light : lights) {
		glm::vec3 light_position = light.getPosition();
		bool is_blocked = false;
		for (size_t i = scene_objects.size(); !is_blocked && i--; ) {
			if (scene_objects[i]->isBlockingSegment(point, light_position)) {
				is_blocked = true;
			}
		}
		if (!is_blocked) {
			glm::vec3 light_unit_vector = glm::normalize(light_position - point);
			float light_dot_normal = light_unit_vector.x * normal.x +
				light_unit_vector.y * normal.y +
				light_unit_vector.z * normal.z;
			glm::vec3 reflection_unit_vector =
				2 * light_dot_normal * normal - light_unit_vector;
			float reflection_dot_viewer =
				reflection_unit_vector.x * viewer_unit_vector.x +
					reflection_unit_vector.y * viewer_unit_vector.y +
					reflection_unit_vector.z * viewer_unit_vector.z;

			accumulated_color += light.getColor() *
				(
					illuminated_object->getDiffuseColor() * light_dot_normal +
						illuminated_object->getSpecularColor() *
							float(pow(
								reflection_dot_viewer,
								illuminated_object->getShininess()
							))
				);
		}
	}

	accumulated_color += illuminated_object->getAmbientColor();

	// clamp to [0, 1]
	accumulated_color.r = std::min(1.0f, accumulated_color.r);
	accumulated_color.g = std::min(1.0f, accumulated_color.g);
	accumulated_color.b = std::min(1.0f, accumulated_color.b);

	return accumulated_color;
}
