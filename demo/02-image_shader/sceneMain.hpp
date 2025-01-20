#pragma once

#include <memory>
#include <span>

#include "Logger.hpp"
#include "Scene.hpp"
#include "GL/Polygon.hpp"
#include "GL/Shader.hpp"
#include "GL/Texture.hpp"

class SceneMain : public Based::Scene {
	std::unique_ptr<Based::GL::Rect> bgRect {nullptr}, gridRect {nullptr};
	std::unique_ptr<Based::GL::Texture> bgTex {nullptr}, gridTex {nullptr};
	Based::GL::ShaderProgram *bgShader {nullptr};
public:
	SceneMain (Based::Engine* engine, Based::Lua::File& conf) : Based::Scene(engine) {
		if (!engine->client) return;

		// Use GL::Rect::make to auto generate VBO & VAO
		bgRect = Based::GL::Rect::make (GL_STATIC_DRAW, Based::Window::Full(), Based::GL::Texture::Full());

		// Set VBO & VAO manually
		GLfloat gridVBO_arr[] = {
			// x     y     s   t
			  -0.8, -0.2,  0,  0,
			   0.2, -0.2,  1,  0,
			   0.2,  0.8,  1,  1,
			  -0.8,  0.8,  0,  1
		};
		Based::GL::VBOSpan gridVBO {gridVBO_arr};
		gridRect = std::make_unique<Based::GL::Rect> (GL_STATIC_DRAW, &gridVBO, true);
		gridRect->addAttribute (0, 2, 4, 0);
		gridRect->addAttribute (1, 2, 4, 2);
		gridRect->end_VAO_batch ();

		bgShader = &engine->client->defaultShaders[Based::GL::Default::SP_2D_ForwardSampler];

		bgTex = std::make_unique<Based::GL::Texture>();
		bgTex->load (BG_PATH);
		gridTex = std::make_unique<Based::GL::Texture>();
		gridTex->load (GRID_PATH);
		if (!bgTex->prepare() || !gridTex->prepare())
			Based::log.fatal ("Failed to prepare texture!");

		// For this demo, we are managing depth visibility by draw order
		glDisable (GL_DEPTH_TEST);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void handle_events (SDL_Event *event) override final { }

	void update () override final { }

	void render () override final {
		bgTex->bind (0);
		gridTex->bind (1);
		bgShader->use();
		bgShader->setUniform ("tex", 0);
		bgRect->start_VAO_batch ();
		bgRect->draw();
		bgRect->end_VAO_batch ();
		bgShader->setUniform ("tex", 1);
		gridRect->start_VAO_batch ();
		gridRect->draw();
		gridRect->end_VAO_batch ();
	}

	void gui () override final { }

	~SceneMain () { }
};
