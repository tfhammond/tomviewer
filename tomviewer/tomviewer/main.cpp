#include <SDL3/SDL.h>

#include "Square.h"

#include "Mesh.h"
#include "OpenMeshButton.h"

int main(int argc, char* argv[])
{
	constexpr int width = 1440;
	constexpr int height = 900;

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow(
		"TomViewer",
		width,
		height,
		0
	);
	if (!window)
	{
		SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(
		window,
		nullptr
	);
	if (!renderer)
	{
		SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	bool running = true;

	float x = 0.0;
	float y = 50.0;
	
	Square square{ 50.0, 50.0, 300.0 };



	Camera camera;

	Mesh mesh;
	mesh.ParseObjFile("meshes/monkey.obj");
	OpenMeshButton openMeshButton(window, mesh);




	// Execution loop
	while (running)
	{
		SDL_Event event{0};
		while (SDL_PollEvent(&event))
		{
			openMeshButton.HandleEvent(event);

			switch (event.type)
			{
			case SDL_EVENT_QUIT:
				running = false;
				break;

			case SDL_EVENT_MOUSE_MOTION:
			{
				if ((event.motion.state & SDL_BUTTON_MMASK) || (SDL_GetModState() & SDL_KMOD_ALT))
				{
					constexpr float sensitivity = 0.005f;

					// Horizontal orbit
					camera.orbit(event.motion.xrel * sensitivity, camera.up);

					// Vertical orbit
					glm::vec3 forward =
						glm::normalize(camera.target - camera.position);

					glm::vec3 right =
						glm::normalize(glm::cross(forward, camera.up));

					camera.orbit(event.motion.yrel * sensitivity, right);
				}
				break;
			}
			}
		}
		openMeshButton.Update();
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_FPoint point{ x, y };
		if (!SDL_RenderPoint(renderer, point.x, point.y))
		{
			SDL_Log("SDL_RenderPoint failed: %s", SDL_GetError());
		}

		/*square.Draw(renderer);
		square.Rotate(0.0001f);*/

		mesh.Draw(renderer, camera);
		openMeshButton.Draw(renderer);
		

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
