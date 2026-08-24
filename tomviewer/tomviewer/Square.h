#pragma once
#include <SDL3/SDL.h>

struct Point
{
	float x;
	float y;
};

class Square {
public:
	Square(float x0, float y0, float size);

	void Draw(SDL_Renderer* renderer);

	void Rotate(float angle);
private:
	Point m_points[4];
};