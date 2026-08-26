#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera
{
	glm::vec3 position{0.0f, 0.0f, -5.0f};
	glm::vec3 target{ 0.0f, 0.0f, 0.0f };
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	float minDistance = 0.5f;

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

	glm::vec3 worldToCamera(const glm::mat4& viewMatrix, const glm::vec3& worldPosition) const
	{
		glm::vec4 cameraPosition = viewMatrix * glm::vec4(worldPosition, 1.0f);
		return glm::vec3(cameraPosition);
	}

	void Frame(const glm::vec3& center, float radius)
	{
		glm::vec3 direction = glm::normalize(position - target);
		radius = glm::max(radius, 0.1f);
		target = center;
		position = target + direction * radius * 2.2f;
		minDistance = radius * 1.01f;
	}

	void zoom(float amount)
	{
		glm::vec3 offset = position - target;
		float distance = glm::length(offset);
		float newDistance = glm::max(minDistance, distance - amount);

		position = target + glm::normalize(offset) * newDistance;
	}
};
