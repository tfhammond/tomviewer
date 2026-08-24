#include "Square.h"

Square::Square(float x0, float y0, float size)
{
	Point topLeft{x0, y0};
	Point topRight{ (size + x0), y0 };
	Point bottomLeft{ x0, (size + y0) };
	Point bottomRight{ (size + x0), (size + y0) };

	m_points[0] = topLeft;
	m_points[1] = topRight;
	m_points[2] = bottomLeft;
	m_points[3] = bottomRight;
}

void Square::Draw(SDL_Renderer* renderer) 
{

	SDL_RenderLine(renderer, m_points[0].x, m_points[0].y, m_points[1].x, m_points[1].y); // top left to top right
	SDL_RenderLine(renderer, m_points[0].x, m_points[0].y, m_points[2].x, m_points[2].y); // top left to bottom left
	SDL_RenderLine(renderer, m_points[1].x, m_points[1].y, m_points[3].x, m_points[3].y); // top right to bottom right
	SDL_RenderLine(renderer, m_points[2].x, m_points[2].y, m_points[3].x, m_points[3].y); // bottom left to bottom right

}

void Square::Rotate(float angle)
{
	float centerX = (m_points[0].x + m_points[1].x + m_points[2].x + m_points[3].x) / 4.0f;
	float centerY = (m_points[0].y + m_points[1].y + m_points[2].y + m_points[3].y) / 4.0f;

	for (int i = 0; i < 4; ++i)
	{
		float dx = m_points[i].x - centerX;
		float dy = m_points[i].y - centerY;

		float rotatedX = dx * cos(angle) - dy * sin(angle);
		float rotatedY = dx * sin(angle) + dy * cos(angle);

		m_points[i].x = centerX + rotatedX;
		m_points[i].y = centerY + rotatedY;
	}
}