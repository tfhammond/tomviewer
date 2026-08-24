#include <SDL3/SDL.h>


int main(int argc, char* argv[])
{

	constexpr int height = 800;
	constexpr int width = 600;

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow(
		"TomViewer",
		height,
		width,
		0
	);

	SDL_Renderer* renderer = SDL_CreateRenderer(
		window,
		nullptr
	);

	bool running = true;

	
	// Execution loop
	while (running)
	{
		SDL_Event event{0};
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
			{
				running = false;
				break;
			}
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);


		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;

	



}