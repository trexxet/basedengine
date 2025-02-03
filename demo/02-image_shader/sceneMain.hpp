#pragma once

#include <memory>
#include <span>

#include "Logger.hpp"
#include "Scene.hpp"
#include "GL/Polygon.hpp"
#include "GL/Shader.hpp"
#include "GL/Sprite.hpp"
#include "GL/Texture.hpp"

#include "config.hpp"
#include "hexShaderVert.hpp"

class SceneMain : public Based::Scene {
	Based::Window* window {nullptr};
	/* For this demo, we are loading multiple textures (or, more correctly, PNG images) on our own.
	 * Note that for a real program you should consider:
	 * * 1) Using global or local ResourceManager
	 * * 2) Combining textures into a single texture and using texture atlas (for 2D) */
	std::unique_ptr<Based::GL::Texture>
		textureBackground {nullptr},
		texture1 {nullptr},
		texture2 {nullptr};
	/* Also, note that all Textures are created and loaded being binded to texture unit 0.
	 * We can use other units for rendering though. */
	enum {
		texUnit_default =0,
		texUnit_hex
	};
	/* Polygons are wrappers for VBO/VAO/EBO. GL::Rect is one of them. */
	std::unique_ptr<Based::GL::Rect>
		rectBackground {nullptr},
		rect1 {nullptr};
	/* GL::Hex is another Polygon type */
	std::unique_ptr<Based::GL::Hex>
		hex1 {nullptr};
	/* Finally, polygon and texture are processed with GL::ShaderProgram. A built-in one is referenced
	 * by a simple pointer (non-owning). */
	Based::GL::ShaderProgram *shader {nullptr};
	std::unique_ptr<Based::GL::ShaderProgram> hexShader {nullptr};
	/* Sprite combines the texture, GL::Rect polygon and default shader into a single object. */
	std::unique_ptr<Based::GL::Sprite>
		sprite1 {nullptr},
		sprite2 {nullptr},
		sprite3 {nullptr};
public:
	SceneMain (Based::Engine* engine, Config& config) : Based::Scene(engine) {
		if (!engine->client) return;
		window = engine->client->window();
	
		/* Textures can be created as a typical Resource (Create -> Load -> Prepare) */
		textureBackground = std::make_unique<Based::GL::Texture>();
		textureBackground->load (config.path.textureBg);
		if (!textureBackground->prepare())
			Based::log.fatal ("Failed to prepare texture!");
		/* Alternatively, texture can be made with GL::Texture::make() */
		texture1 = Based::GL::Texture::make (config.path.texture1);
		texture2 = Based::GL::Texture::make (config.path.texture2);

		/* Polygon can be created in a number of ways.
		 * 1) Use GL::Rect::make() to auto generate VBO & VAO */
		/* Note that to draw the background we set the polygon's rectange to be the same as window's one.
		 * GL::Texture::full() is a rectangle covering entire tetxure (in the GL texture coordinates). */
		rectBackground = Based::GL::Rect::make (GL_STATIC_DRAW, window->rect, Based::GL::Texture::full());

		/* 2) Set VBO & VAO manually. This way you can make any polygon. */
		Based::Vec2D<GLfloat> s1_topleft {16.f, 16.f};
		Based::Vec2D<GLfloat> s1_size {256.f, 256.f};
		/* Note that X and Y are defined in screen pixel coordinates,
		   while S and T are defined in GL texture coordinates (from 0 to 1, T axis inverted) */
		GLfloat sprite1VBO_arr[] = {
			//             x                          y             s   t
			  s1_topleft.x + s1_size.x,  s1_topleft.y,              1,  1,
			  s1_topleft.x,              s1_topleft.y,              0,  1,
			  s1_topleft.x,              s1_topleft.y + s1_size.y,  0,  0,
			  s1_topleft.x + s1_size.x,  s1_topleft.y + s1_size.y,  1,  0
		};
		Based::GL::VBOSpan sprite1VBO {sprite1VBO_arr};
		rect1 = std::make_unique<Based::GL::Rect> (GL_STATIC_DRAW, &sprite1VBO, true);
		rect1->add_attribute (0, 2, 4, 0); // X Y s t
		rect1->add_attribute (1, 2, 4, 2); // x y S T
		rect1->end_VAO_batch ();

		/* Hex can be created in a similar way. Instead of Rect2D you need to specify it's orientation
		 * and outer circle for both X Y (screen space) and S T (texture space) */
		hex1 = Based::GL::Hex::make (GL_STATIC_DRAW, Based::GL::Hex::Orientation::topFlat,
		                             Based::Circle2D<GLfloat> (window->center(), config.hex1R),
		                             Based::Circle2D<GLfloat> (Based::GL::Texture::center(), 0.5f));

		/* There are some built-in shader programs and shaders. One of them is SP_2D_MVPSampler, which applies
		 * MVP matrix in "mvp" uniform to (X, Y), passes (S, 1 - T) coordinates (because images Y axis is inverted
		 * to GL texture space T axis), and samples the texture unit in "tex" uniform. */
		shader = &Based::GL::Default::shaders [Based::GL::Default::SP_2D_MVPSampler];

		/* Shader can be created as a resource (Create -> Load -> Prepare), using a file or 
		 * a string as a source. Note that we are scoping the Shader to the current constructor only, as it's no
		 * longer needed after linking the ShaderProgram. */
		std::unique_ptr<Based::GL::Shader> hexShaderVert = std::make_unique<Based::GL::Shader> (GL_VERTEX_SHADER, std::move(hexShaderVertSrc));
		hexShaderVert->load ();
		if (!hexShaderVert->prepare())
			Based::log.fatal ("Failed to prepare vertex shader!");
		
		/* Two make_*() functions are avaliable: for a string source and for a file source */
		std::unique_ptr<Based::GL::Shader> hexShaderFrag = Based::GL::Shader::make_from_file (GL_FRAGMENT_SHADER, config.path.hexShaderFrag);

		/* Making ShaderProgram is similar. Note that you can reference default shaders in ShaderVec, e.g.:
		 * {&Based::GL::Default::shaders(Based::GL::Default::S_2D_MVPVert), hexShaderFrag.get()} */
		hexShader = Based::GL::ShaderProgram::make ({hexShaderVert.get(), hexShaderFrag.get()}, hexShaderVertAttributes);

		/* Finally, Sprite combines everything needed for a simple image drawing.
		 * Again, it can be created as a resource (Create -> Load -> Prepare) */
		/* Note that the width here is negative, making the image horizontally flipped,
		 * and Rect2D being extended left from the X coordinate. */
		sprite1 = std::make_unique<Based::GL::Sprite> (Based::Rect2D<GLfloat> {500.f, 16.f, -256.f, 256.f},
		                                               &window->ortho);
		sprite1->load (config.path.texture3);
		if (!sprite1->prepare())
			Based::log.fatal ("Failed to prepare sprite!");
		/* Sprite has a make() as well. */
		/* Note that the width here is negative, making the image vertically flipped,
		 * and Rect2D being extended top from the Y coordinate. */
		sprite2 = Based::GL::Sprite::make (config.path.texture3,
		                                   Based::Rect2D<GLfloat> {16.f, 500.f, 256.f, -256.f},
		                                   &window->ortho);
		/* Also, note that the same texture is being loaded twice. To avoid this, we can reuse
		 * an already existing texture. However, it should be done carefully, as the texture is not
		 * managed by Sprite in that case. Sprite shouldn't outlive the texture! */
		/* Note that now we are flipping on both X and Y (check config)*/
		sprite3 = Based::GL::Sprite::make (texture1.get(), config.sprite3_rect, &window->ortho);

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
		shader->use();
		/* 2) Set the MVP matrix as orthoghraphic projection of entire window.
		 * In other words, go from GL NDC coordinates to screen pixel coordinates. */
		shader->set_uniform ("mvp", window->ortho);
		/* 3) Set the sampler (in the fragment shader) to the texture unit of required texture */
		shader->set_uniform ("tex", texUnit_default);
		/* 4) Bind the required texture to the texture unit */
		textureBackground->use (texUnit_default);
		/* 5) Bind VAO and draw polygon */
		rectBackground->bind_draw();
		/* 6) Subsequent draws can be made starting from step 3 (or 4, if no texture unit switch required) */
		texture1->use ();
		rect1->bind_draw();
		/* 7) Alternatively, Sprite can be used. It performs steps 1-5 for GL::Rect and SP_2D_MVPSampler. */
		sprite1->draw();
		/* Note that these steps are performed for every Sprite::draw(), inducing context switching and
		 * being potentially slow. */
		sprite2->draw();
		sprite3->draw();
		/* 8) To draw hex, we'll use our own shaders as an example */
		hexShader->use();
		hexShader->set_uniform ("mvp", window->ortho);
		hexShader->set_uniform ("center", window->center());
		/* Also, we'll use different texture unit */
		hexShader->set_uniform ("tex", texUnit_hex);
		texture2->use (texUnit_hex);
		hex1->bind_draw();
	}

	void gui () override final { }

	~SceneMain () { }
};
