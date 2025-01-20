#pragma once

#include <memory>
#include <span>

#include "Logger.hpp"
#include "Scene.hpp"
#include "GL/Polygon.hpp"
#include "GL/Shader.hpp"
#include "GL/Texture.hpp"

class SceneMain : public Based::Scene {
	std::unique_ptr<Based::GL::Rect> bgRect {nullptr}, sprite1Rect {nullptr};
	std::unique_ptr<Based::GL::Texture> bgTex {nullptr}, sprite1Tex {nullptr};
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
		sprite1Rect = std::make_unique<Based::GL::Rect> (GL_STATIC_DRAW, &gridVBO, true);
		sprite1Rect->add_attribute (0, 2, 4, 0);
		sprite1Rect->add_attribute (1, 2, 4, 2);
		sprite1Rect->end_VAO_batch ();

		bgShader = &engine->client->defaultShaders[Based::GL::Default::SP_2D_ForwardSampler];

		bgTex = std::make_unique<Based::GL::Texture>();
		bgTex->load (BG_PATH);
		sprite1Tex = std::make_unique<Based::GL::Texture>();
		sprite1Tex->load (SPRITE1_PATH);
		if (!bgTex->prepare() || !sprite1Tex->prepare())
			Based::log.fatal ("Failed to prepare texture!");
		bgTex->bind (0);
		sprite1Tex->bind (1);

		// For this demo, we are managing depth visibility by draw order
		glDisable (GL_DEPTH_TEST);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void handle_events (SDL_Event *event) override final { }

	void update () override final { }

	void render () override final {
		bgShader->use();
		bgShader->setUniform ("tex", bgTex->unit);
		bgRect->bind_draw();
		bgShader->setUniform ("tex", sprite1Tex->unit);
		sprite1Rect->bind_draw();
	}

	void gui () override final { }

	~SceneMain () { }
};
