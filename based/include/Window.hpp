#pragma once

#include <memory>
#include <string>

#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_video.h>

#include "Defs.hpp"
#include "Geometry.hpp"
#include "RML/Font.hpp"
#include "RML/Interface.hpp"
#include "RML/DebugOverlay/Overlay.hpp"

namespace Based {

class EngineClient;
class Window {
public:
	enum Flags {
		NO_FLAGS = 0,
		DISABLE_RML = 1
	};
	Flags flags;

private:
	Vec2D<int> _size;
	Rect2D<int> _rect;
	GLfloat _aspect;
	glm::mat4 _ortho;

public:
	EngineClient& engineClient;
	SDL_Window *sdlWindow;
	
	/// @brief Window size and corresponding rectangle
	const decltype((_size)) size = _size;
	const decltype((_rect)) rect = _rect;
	/// @brief Window aspect (size.x / size.y)
	const decltype((_aspect)) aspect = _aspect;
	/// @brief Orthographic projection matrix for the window
	const decltype((_ortho)) ortho = _ortho;
	/// @brief Pixel for Window center
	Vec2D<int> center () { return size / 2; }
	/// @brief Set window size and related members
	void resize (const Vec2D<int>& size);

	RML::InterfaceStorage rml {nullptr};

	void render ();

	Window (EngineClient& engineClient, const std::string& title, const Vec2D<int>& size, const Flags flags);
	~Window ();

	BASED_CLASS_NO_COPY_MOVE (Window);

	class DebugOverlay {
		Window& window;
	public:
		RML::DebugOverlay::OverlayStorage debugOverlay {nullptr};
		RML::DebugOverlay::OverlayHandle operator()() { return debugOverlay.get(); }
		
		SDL_Keycode toggleKey = SDLK_UNKNOWN;
		bool visible = false;

		bool init (const RML::Font& font, SDL_Keycode toggleKey = SDLK_F3);
		inline void toggle () { visible = !visible; }
		inline void update () { if (debugOverlay && visible) [[unlikely]] debugOverlay->update(); }
		inline void render () { if (debugOverlay && visible) [[unlikely]] debugOverlay->render(); }
		void handle_event (SDL_Event *event);

		DebugOverlay (Window& window) : window(window) { }
		BASED_CLASS_NO_COPY_MOVE (DebugOverlay);
	} debugOverlay;
};

using WindowHandle = Window *;

}
