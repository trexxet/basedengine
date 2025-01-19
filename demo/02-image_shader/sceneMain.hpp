#pragma once

#include <memory>
#include <span>

#include "Logger.hpp"
#include "Scene.hpp"
#include "GL/Polygon.hpp"
#include "GL/Shader.hpp"
#include "GL/Texture.hpp"

class SceneMain : public Based::Scene {
	std::unique_ptr<Based::GL::Rect> bgRect {nullptr};
	std::unique_ptr<Based::GL::Texture> bgTex {nullptr};
	Based::GL::ShaderProgram *bgShader {nullptr};
public:
	SceneMain (Based::Engine* engine, Based::Lua::File& conf) : Based::Scene(engine) {
		if (!engine->client) return;

		GLfloat bgVBO_arr[] = {
			// x   y   s   t
			  -1,  1,  0,  1,
			  -1, -1,  0,  0,
			   1, -1,  1,  0,
			   1,  1,  1,  1
		};
		std::span<GLfloat> bgVBO {bgVBO_arr};
		bgRect = std::make_unique<Based::GL::Rect> (GL_STATIC_DRAW, &bgVBO, true);
		bgRect->addAttribute (0, 2, 4, 0);
		bgRect->addAttribute (1, 2, 4, 2);
		bgRect->end_VAO_batch ();

		bgShader = &engine->client->defaultShaders[Based::GL::Default::SP_2D_ForwardSampler];
		bgShader->use();
		bgShader->setUniform ("tex", 0);

		bgTex = std::make_unique<Based::GL::Texture>();
		bgTex->load (BG_PATH);
		if (!bgTex->prepare())
			Based::log.fatal ("Failed to prepare texture!");
	}

	void handle_events (SDL_Event *event) override final { }

	void update () override final { }

	void render () override final {
		bgTex->bind (0);
		bgShader->use();
		bgRect->start_VAO_batch ();
		bgRect->draw();
		bgRect->end_VAO_batch ();
	}

	void gui () override final { }

	~SceneMain () {
		bgTex->unload();
	}
};
