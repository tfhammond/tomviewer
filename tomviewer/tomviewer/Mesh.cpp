#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Mesh::ParseObjFile()
{
	
	std::ifstream file("meshes/Cube.obj");
	if (!file)
	{
		std::cerr << "Error: Could not open .obj file.\n";
		return;
	}

	this->triangles.clear();
	std::vector<glm::vec3> vertices;
	std::string line;

	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string tag;

		if (!(iss >> tag) || tag.front() == '#')
		{
			continue;
		}
		else if (tag == "v")
		{
			// process vertex positions
			glm::vec3 vertexPosition{0.0, 0.0, 0.0};
			if (iss >> vertexPosition.x >> vertexPosition.y >> vertexPosition.z)
			{
				vertices.push_back(vertexPosition);
			}
			else
			{
				std::cerr << "Format of line unexpected: " << line << "\n";
			}
		}
		else if (tag == "f")
		{
			// process faces (with position indices -> create triangles then store in mesh)
		}
		else
		{
			// either unknown tag or unsupported tag (will add more when updating complexity of parsing)
		}

	}



}