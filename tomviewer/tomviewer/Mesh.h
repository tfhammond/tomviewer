#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
};

struct Triangle
{
	glm::vec3 x;
	glm::vec3 y;
	glm::vec3 z;
};

class Mesh
{
public:
	Mesh() = default;
	void ParseObjFile();
private:
	std::vector<Triangle> triangles;
};