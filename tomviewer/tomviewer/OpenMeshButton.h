#pragma once

#include <SDL3/SDL.h>

#include <mutex>
#include <string>

#include "Mesh.h"

class OpenMeshButton
{
public:
	OpenMeshButton(SDL_Window* window, Mesh& mesh);

	void HandleEvent(const SDL_Event& event);
	void Update();
	void Draw(SDL_Renderer* renderer);

private:
	enum class DialogResult
	{
		None,
		Canceled,
		Selected,
		Error
	};

	static void SDLCALL OnFileSelected(void* userdata, const char* const* filelist, int filter);

	bool IsInside(float x, float y) const;
	std::string FileNameFromPath(const std::string& path) const;
	void OpenDialog();

	SDL_Window* m_window;
	Mesh& m_mesh;
	SDL_FRect m_bounds{ 16.0f, 16.0f, 96.0f, 32.0f };
	std::mutex m_mutex;
	DialogResult m_result = DialogResult::None;
	std::string m_dialogValue;
	std::string m_defaultLocation;
	std::string m_status;
	bool m_dialogOpen = false;
	bool m_pressed = false;
};
