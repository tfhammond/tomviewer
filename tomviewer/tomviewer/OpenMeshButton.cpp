#include "OpenMeshButton.h"

OpenMeshButton::OpenMeshButton(SDL_Window* window, Mesh& mesh)
	: m_window(window),
	m_mesh(mesh),
	m_status("Loaded: monkey.obj")
{
	// Use the application's mesh directory as the initial file-dialog location.
	char* currentDirectory = SDL_GetCurrentDirectory();
	if (currentDirectory)
	{
		m_defaultLocation = std::string(currentDirectory) + "meshes";
		SDL_free(currentDirectory);
	}
}

void OpenMeshButton::HandleEvent(const SDL_Event& event)
{
	// A click opens the dialog only when both press and release occur on the button.
	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT)
	{
		m_pressed = !m_dialogOpen && IsInside(event.button.x, event.button.y);
	}
	else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT)
	{
		const bool shouldOpenDialog = m_pressed && IsInside(event.button.x, event.button.y) && !m_dialogOpen;
		m_pressed = false;

		if (shouldOpenDialog)
		{
			OpenDialog();
		}
	}
}

bool OpenMeshButton::Update()
{
	DialogResult result = DialogResult::None;
	std::string value;
	{
		// Copy the dialog callback's result while holding the mutex, then process it on the main thread.
		std::lock_guard<std::mutex> lock(m_mutex);
		result = m_result;
		if (result != DialogResult::None)
		{
			value = m_dialogValue;
			m_result = DialogResult::None;
			m_dialogValue.clear();
		}
	}

	if (result == DialogResult::None)
	{
		return false;
	}

	m_dialogOpen = false;
	if (result == DialogResult::Selected)
	{
		// Replace the current mesh and report whether loading succeeded.
		const std::string fileName = FileNameFromPath(value);
		if (m_mesh.ParseObjFile(value))
		{
			m_status = "Loaded: " + fileName;
			return true;
		}
		else
		{
			m_status = "Failed to load: " + fileName;
		}
	}
	else if (result == DialogResult::Error)
	{
		m_status = "File dialog failed";
		SDL_Log("Open file dialog failed: %s", value.c_str());
	}
	return false;
}

void OpenMeshButton::Draw(SDL_Renderer* renderer)
{
	// Select a visual state, then draw the button and its current load status.
	float mouseX = 0.0f;
	float mouseY = 0.0f;
	SDL_GetMouseState(&mouseX, &mouseY);
	const bool hovered = !m_dialogOpen && IsInside(mouseX, mouseY);

	if (m_dialogOpen)
	{
		SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
	}
	else if (m_pressed)
	{
		SDL_SetRenderDrawColor(renderer, 75, 105, 145, 255);
	}
	else if (hovered)
	{
		SDL_SetRenderDrawColor(renderer, 80, 120, 170, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 60, 90, 130, 255);
	}

	SDL_RenderFillRect(renderer, &m_bounds);
	SDL_SetRenderDrawColor(renderer, 210, 220, 235, 255);
	SDL_RenderRect(renderer, &m_bounds);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDebugText(renderer, m_bounds.x + 12.0f, m_bounds.y + 12.0f, "Open OBJ");
	SDL_RenderDebugText(renderer, m_bounds.x + m_bounds.w + 12.0f, m_bounds.y + 12.0f, m_status.c_str());
}

void SDLCALL OpenMeshButton::OnFileSelected(void* userdata, const char* const* filelist, int)
{
	// Translate SDL's asynchronous callback into a result for Update() to consume.
	OpenMeshButton& button = *static_cast<OpenMeshButton*>(userdata);
	std::lock_guard<std::mutex> lock(button.m_mutex);

	if (!filelist)
	{
		button.m_result = DialogResult::Error;
		button.m_dialogValue = SDL_GetError();
	}
	else if (!filelist[0])
	{
		button.m_result = DialogResult::Canceled;
		button.m_dialogValue.clear();
	}
	else
	{
		button.m_result = DialogResult::Selected;
		button.m_dialogValue = filelist[0];
	}
}

bool OpenMeshButton::IsInside(float x, float y) const
{
	return x >= m_bounds.x && x <= m_bounds.x + m_bounds.w
		&& y >= m_bounds.y && y <= m_bounds.y + m_bounds.h;
}

std::string OpenMeshButton::FileNameFromPath(const std::string& path) const
{
	const size_t separator = path.find_last_of("\\/");
	return separator == std::string::npos ? path : path.substr(separator + 1);
}

void OpenMeshButton::OpenDialog()
{
	// Restrict the native picker to Wavefront OBJ files and start it asynchronously. (Can expand on later)
	static const SDL_DialogFileFilter objFilter{ "Wavefront OBJ", "obj" };
	m_dialogOpen = true;
	SDL_ShowOpenFileDialog(
		OnFileSelected,
		this,
		m_window,
		&objFilter,
		1,
		m_defaultLocation.empty() ? nullptr : m_defaultLocation.c_str(),
		false
	);
}
