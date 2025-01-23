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
		texture1 {nullptr},
		texture2 {nullptr};
	enum {
		textureBackground_unit,
		texture1_unit,
		texture2_unit
	};
	/* Polygons are wrappers for VBO/VAO/EBO. GL::Rect is one of them. */
	std::unique_ptr<Based::GL::Rect>
		rectBackground {nullptr},
		rectSprite1 {nullptr};
	/* GL::Hex is another Polygon type */
	std::unique_ptr<Based::GL::Hex>
		hexSprite1 {nullptr};
	/* Finally, polygon and texture are processed with GL::ShaderProgram. */
	Based::GL::ShaderProgram *shaderSprite {nullptr};
public:
	SceneMain (Based::Engine* engine, Based::Lua::File& conf) : Based::Scene(engine) {
		if (!engine->client) return;
		Based::Window* window = engine->client->window();
	
		/* Textures can be created as a typical Resource (Create -> Load -> Prepare) */
		textureBackground = std::make_unique<Based::GL::Texture> (textureBackground_unit);
		textureBackground->load (BG_PATH);
		if (!textureBackground->prepare())
			Based::log.fatal ("Failed to prepare texture!");
		/* Alternatively, texture can be made with GL::Texture::make() */
		texture1 = Based::GL::Texture::make (SPRITE1_PATH, texture1_unit);
		texture2 = Based::GL::Texture::make (SPRITE2_PATH, texture2_unit);

		/* Polygon can be created in a number of ways.
		 * 1) Use GL::Rect::make() to auto generate VBO & VAO */
		rectBackground = Based::GL::Rect::make (GL_STATIC_DRAW, window->rect, Based::GL::Texture::full());

		/* 2) Set VBO & VAO manually. This way you can make any polygon. */
		Based::Point2D<GLfloat> s1_topleft {16.f, 16.f};
		Based::Size2D<GLfloat> s1_size {256.f, 256.f};
		/* Note that X and Y are defined in screen pixel coordinates,
		   while S and T are defined in GL texture coordinates (from 0 to 1, T axis inverted) */
		GLfloat sprite1VBO_arr[] = {
			//             x                          y             s   t
			  s1_topleft.x + s1_size.w,  s1_topleft.y,              1,  1,
			  s1_topleft.x,              s1_topleft.y,              0,  1,
			  s1_topleft.x,              s1_topleft.y + s1_size.h,  0,  0,
			  s1_topleft.x + s1_size.w,  s1_topleft.y + s1_size.h,  1,  0
		};
		Based::GL::VBOSpan sprite1VBO {sprite1VBO_arr};
		rectSprite1 = std::make_unique<Based::GL::Rect> (GL_STATIC_DRAW, &sprite1VBO, true);
		rectSprite1->add_attribute (0, 2, 4, 0); // X Y s t
		rectSprite1->add_attribute (1, 2, 4, 2); // x y S T
		rectSprite1->end_VAO_batch ();

		/* Hex can be created in a similar way. Instead of Rect2D you need to specify it's orientation
		 * and outer circle for both X Y (screen space) and S T (texture space) */
		hexSprite1 = Based::GL::Hex::make (GL_STATIC_DRAW, Based::GL::Hex::Orientation::topFlat,
		                                   Based::Circle2D<GLfloat> (window->center(), 100.f),
		                                   Based::Circle2D<GLfloat> (Based::GL::Texture::center(), 0.5f));

		/* There are some built-in shader programs and shaders. One of them is SP_2D_ForwardSampler, which
		 * simply forwards X, Y, S and (1-T) coordinates (because images has inverted Y axis in GL texture space),
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
		/* 1) Use (enable) the shader program */
		shaderSprite->use();
		/* 2) Set the MVP matrix as orthoghraphic projection of entire window.
		 * In other words, go from GL NDC coordinates to screen pixel coordinates. */
		shaderSprite->setUniform ("mvp", engine->client->window()->ortho);
		/* 3) Set the sampler (in fragment shader) to the texture unit of required texture */
		shaderSprite->setUniform ("tex", textureBackground->unit);
		/* 4) Bind VAO and draw polygon */
		rectBackground->bind_draw();
		shaderSprite->setUniform ("tex", texture1->unit);
		rectSprite1->bind_draw();
		shaderSprite->setUniform ("tex", texture2->unit);
		hexSprite1->bind_draw();
	}

	void gui () override final { }

	~SceneMain () { }
};
