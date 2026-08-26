#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include "Camera.h"

struct Vertex
{
	glm::vec3 position;

	//glm::vec2 projectVertex(glm::vec3 p, float cameraZ = 0.0f) const
	//{
	//	p.z += cameraZ;

	//	return {
	//		p.x / p.z,
	//		p.y / p.z
	//	};
	//}
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

	std::vector<glm::vec2> project(const Camera& camera) const
	{
		glm::vec3 xCamera = camera.worldToCamera(x.position);
		glm::vec3 yCamera = camera.worldToCamera(y.position);
		glm::vec3 zCamera = camera.worldToCamera(z.position);


		glm::vec2 xproject = x.projectVertex(xCamera);
		glm::vec2 yproject = y.projectVertex(yCamera);
		glm::vec2 zproject = z.projectVertex(zCamera);
		//return std::vector<glm::vec2>{ xproject, yproject, zproject };
		return { xproject, yproject, zproject };
	}
};

class Mesh
{
public:
	Mesh() = default;
	bool ParseObjFile(const std::string& path);
	void Draw(SDL_Renderer* renderer, const Camera& camera);
	
	//void moveForwardZ()
	//{
	//	for (Triangle& triangle : m_triangles)
	//	{
	//		triangle.forwardZ();
	//	}
	//}
private:
	std::vector<Triangle> m_triangles;
};