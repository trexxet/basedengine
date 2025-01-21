#pragma once

#include <memory>
#include <span>

#include "Logger.hpp"
#include "Scene.hpp"
#include "GL/Polygon.hpp"
#include "GL/Shader.hpp"
#include "GL/Texture.hpp"

class SceneMain : public Based::Scene {
	/* For this demo, we are loading multiple textures (or, more correctly, PNG images) on our own.
	 * Note that for a real program you should consider:
	 * * 1) Using global or local ResourceManager
	 * * 2) Combining textures into a single texture and using texture atlas (for 2D) */
	std::unique_ptr<Based::GL::Texture>
		textureBackground {nullptr},
		texture1 {nullptr};
	enum {
		textureBackground_unit = 0,
		texture1_unit = 1
	};
	/* Polygons are wrappers for VBO/VAO/EBO. GL::Rect is one of them. */
	std::unique_ptr<Based::GL::Rect>
		rectBackground {nullptr},
		rectSprite1 {nullptr};
	/* Finally, polygon and texture are processed with GL::ShaderProgram. */
	Based::GL::ShaderProgram *shaderSprite {nullptr};
public:
	SceneMain (Based::Engine* engine, Based::Lua::File& conf) : Based::Scene(engine) {
		if (!engine->client) return;
	
		/* Textures can be created as a typical Resource (Create -> Load -> Prepare) */
		textureBackground = std::make_unique<Based::GL::Texture> (textureBackground_unit);
		textureBackground->load (BG_PATH);
		if (!textureBackground->prepare())
			Based::log.fatal ("Failed to prepare texture!");
		/* Alternatively, texture can be made with GL::Texture::make() */
		texture1 = Based::GL::Texture::make (SPRITE1_PATH, texture1_unit);

		/* Polygon can be created in a number of ways.
		 * 1) Use GL::Rect::make() to auto generate VBO & VAO */
		rectBackground = Based::GL::Rect::make (GL_STATIC_DRAW, Based::Window::Full(), Based::GL::Texture::Full());

		/* 2) Set VBO & VAO manually */
		GLfloat sprite1VBO_arr[] = {
			// x     y     s   t   (everything in GL space coordinates)
			  -0.8, -0.2,  0,  0,
			   0.2, -0.2,  1,  0,
			   0.2,  0.8,  1,  1,
			  -0.8,  0.8,  0,  1
		};
		Based::GL::VBOSpan sprite1VBO {sprite1VBO_arr};
		rectSprite1 = std::make_unique<Based::GL::Rect> (GL_STATIC_DRAW, &sprite1VBO, true);
		rectSprite1->add_attribute (0, 2, 4, 0); // X Y s t
		rectSprite1->add_attribute (1, 2, 4, 2); // x y S T
		rectSprite1->end_VAO_batch ();

		/* There are some built-in shader programs and shaders. One of them is SP_2D_ForwardSampler,
		 * which simply forwards X, Y, S and (1-T) coordinates (because images has inverted Y axis in GL space),
		 * and samples the texture unit in "tex" uniform. */
		shaderSprite = &engine->client->defaultShaders[Based::GL::Default::SP_2D_ForwardSampler];

		/* For this demo, we are managing depth visibility by draw order */
		glDisable (GL_DEPTH_TEST);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void handle_events (SDL_Event *event) override final { }

	void update () override final { }

	void render () override final {
		/* For this demo, we are rendering polygons (VBOs) one by one, making multiple glDraw* calls
		 * and switching VAO context, which can be slow.
		 * Note that for a real program you should consider:
		 * * 1) Combining multiple VAOs/VBOs into a single one
		 * * 2) Using instancing */
		shaderSprite->use();
		shaderSprite->setUniform ("tex", textureBackground->unit);
		rectBackground->bind_draw();
		shaderSprite->setUniform ("tex", texture1->unit);
		rectSprite1->bind_draw();
	}

	void gui () override final { }

	~SceneMain () { }
};
