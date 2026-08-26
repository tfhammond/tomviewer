#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Mesh::ParseObjFile()
{
	
	std::ifstream file("meshes/monkey.obj");
	if (!file)
	{
		std::cerr << "Error: Could not open .obj file.\n";
		return;
	}

	this->m_triangles.clear();
	std::vector<Vertex> vertices;
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
			Vertex vertex{};
			if (iss >> vertex.position.x >> vertex.position.y >> vertex.position.z)
			{
				vertices.push_back(vertex);
			}
			else
			{
				std::cerr << "Format of line unexpected: " << line << "\n";
			}
		}
		else if (tag == "f")
		{
			// process faces (with position indices -> create triangles then store in mesh)
			std::vector<int> positionIndices;
			std::string faceVertex;
			while (iss >> faceVertex)
			{
				std::istringstream faceStream(faceVertex);
				std::string positionIndexText;
				if (std::getline(faceStream, positionIndexText, '/'))
				{
					// TODO: Handle malformed face tokens that std::stoi cannot parse.
					int positionIndex = std::stoi(positionIndexText);
					positionIndices.push_back(positionIndex - 1);
				}

			}
			if (positionIndices.size() < 3)
			{
				std::cerr << "Face must have at least 3 vertices: " << line << "\n";
				continue;
			}

			for (size_t i = 1; i + 1 < positionIndices.size(); ++i)
			{
				// TODO: Validate face indices are in range before indexing vertices.
				Triangle triangle{
					vertices[positionIndices[0]],
					vertices[positionIndices[i]],
					vertices[positionIndices[i + 1]]
				};
				m_triangles.push_back(triangle);
			}
		}
		else
		{
			// either unknown tag or unsupported tag (will add more when updating complexity of parsing)
		}
	}
}


void Mesh::Draw(SDL_Renderer* renderer)
{
	// moveForwardZ();

	int width;
	int height;

	SDL_GetRenderOutputSize(renderer, &width, &height);

	constexpr float cameraZ = 5.0f;
	constexpr float focalLength = 300.0f;

	for (const Triangle& triangle : m_triangles)
	{
		if (triangle.x.position.z + cameraZ <= 0.0f || triangle.y.position.z + cameraZ <= 0.0f || triangle.z.position.z + cameraZ <= 0.0f)
		{
			continue;
		}

		std::vector<glm::vec2> projected = triangle.project(cameraZ);

		SDL_FPoint screen[3];
		for (int i = 0; i < 3; ++i)
		{
			screen[i] = {
				width * 0.5f + projected[i].x * focalLength,
				height * 0.5f - projected[i].y * focalLength
			};
		}

		SDL_RenderLine(
			renderer,
			screen[0].x, screen[0].y,
			screen[1].x, screen[1].y
		);
		SDL_RenderLine(
			renderer,
			screen[1].x, screen[1].y,
			screen[2].x, screen[2].y
		);
		SDL_RenderLine(
			renderer,
			screen[2].x, screen[2].y,
			screen[0].x, screen[0].y
		);
	}
}