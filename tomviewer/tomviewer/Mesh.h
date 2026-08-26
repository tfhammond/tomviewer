#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>

struct Vertex
{
	glm::vec3 position;

	glm::vec2 projectVertex(glm::vec3 p, float cameraZ = 0.0f) const
	{
		p.z += cameraZ;

		return {
			p.x / p.z,
			p.y / p.z
		};
	}

};

struct Triangle
{
	Vertex x;
	Vertex y;
	Vertex z;

	std::vector<glm::vec2> project(float cameraZ = 0.0f) const
	{
		glm::vec2 xproject = x.projectVertex(x.position, cameraZ);
		glm::vec2 yproject = y.projectVertex(y.position, cameraZ);
		glm::vec2 zproject = z.projectVertex(z.position, cameraZ);
		//return std::vector<glm::vec2>{ xproject, yproject, zproject };
		return { xproject, yproject, zproject };
	}
};

class Mesh
{
public:
	Mesh() = default;
	void ParseObjFile();
	void Draw(SDL_Renderer* renderer);
	
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