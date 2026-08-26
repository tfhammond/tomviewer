#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>

struct Vertex
{
	glm::vec3 position;
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
	void ParseObjFile();
	void Draw(SDL_Renderer* renderer);
private:
	std::vector<Triangle> triangles;
};