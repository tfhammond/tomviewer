#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include "Camera.h"

struct Vertex
{
	glm::vec3 position;

	glm::vec2 projectVertex(const glm::vec3& cameraPosition) const
	{
		return {
			cameraPosition.x / -cameraPosition.z,
			cameraPosition.y / -cameraPosition.z
		};
	}

};

struct Triangle
{
	Vertex x;
	Vertex y;
	Vertex z;
};

class Mesh
{
public:
	Mesh() = default;
	bool ParseObjFile(const std::string& path);
	void Draw(SDL_Renderer* renderer, const Camera& camera);

private:
	std::vector<Triangle> m_triangles;
};
