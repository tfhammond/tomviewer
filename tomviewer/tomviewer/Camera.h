#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera
{
	glm::vec3 position{0.0f, 0.0f, -5.0f};
	glm::vec3 target{ 0.0f, 0.0f, 0.0f };
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };

	glm::mat4 viewMatrix() const
	{
		return glm::lookAt(position, target, up);
	}

	void orbit(float angle, const glm::vec3& axis)
	{
		glm::vec3 offset = position - target;
		glm::mat4 rotation = glm::rotate(
			glm::mat4(1.0f),
			angle,
			axis
		);
		position = target + glm::vec3(rotation * glm::vec4(offset, 0.0f));
	}

	glm::vec3 worldToCamera(const glm::vec3& worldPosition) const
	{
		glm::vec4 cameraPosition = viewMatrix() * glm::vec4(worldPosition, 1.0f);
		return glm::vec3(cameraPosition);
	}
};