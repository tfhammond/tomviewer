#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool Mesh::ParseObjFile(const std::string& path)
{
	
	std::ifstream file(path);
	if (!file)
	{
		std::cerr << "Error: Could not open .obj file.\n";
		return false;
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
	return true;
}


void Mesh::Draw(SDL_Renderer* renderer, const Camera& camera)
{
	int width;
	int height;

	SDL_GetRenderOutputSize(renderer, &width, &height);

	constexpr float cameraZ = 5.0f;
	constexpr float focalLength = 300.0f;

	glm::mat4 viewMatrix = camera.viewMatrix();

	for (const Triangle& triangle : m_triangles)
	{
		glm::vec3 xCamera = camera.worldToCamera(viewMatrix, triangle.x.position);
		glm::vec3 yCamera = camera.worldToCamera(viewMatrix, triangle.y.position);
		glm::vec3 zCamera = camera.worldToCamera(viewMatrix, triangle.z.position);

		if (xCamera.z >= 0.0f || yCamera.z >= 0.0f || zCamera.z >= 0.0f)
		{
			continue;
		}

		glm::vec2 projected[3] {
			triangle.x.projectVertex(xCamera),
			triangle.y.projectVertex(yCamera),
			triangle.z.projectVertex(zCamera)
		};

		SDL_FPoint screen[3];
		for (int i = 0; i < 3; ++i)
		{
			screen[i] = {
				width * 0.5f + projected[i].x * focalLength,
				height * 0.5f - projected[i].y * focalLength
			};
		}

		SDL_FPoint outline[4]{
			screen[0],
			screen[1],
			screen[2],
			screen[0]
		};

		SDL_RenderLines(renderer, outline, 4);
	}
}
