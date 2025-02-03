#pragma once

#include <memory>

#include <glm/mat4x4.hpp>

#include "Geometry.hpp"
#include "Resource.hpp"
#include "GL/Polygon.hpp"
#include "GL/Shader.hpp"
#include "GL/Texture.hpp"

namespace Based::GL {

class Sprite : public Resource {
	const glm::mat4* mvp {nullptr};
	bool textureManaged = false;
public:
	std::unique_ptr<Rect> glRect;
	Texture* texture {nullptr};

	Sprite () = delete;
	Sprite (const Rect2D<GLfloat>& rect, const glm::mat4* mvp);
	Sprite (Texture *texture, const Rect2D<GLfloat>& rect, const glm::mat4* mvp);

	void load (const std::string& path) override final;
	bool prepare () override final;
	void unload () override final;

	~Sprite () override final;

	void draw ();

	/// @brief Create, load, and prepare Sprite with managed texture
	/// @param path path to texture file
	/// @param rect Rect2D for the sprite position
	/// @param mvp MVP matrix for the shader
	static std::unique_ptr<Sprite> make (const std::string& path, const Rect2D<GLfloat>& rect, const glm::mat4* mvp);
	/// @brief Create, load, and prepare Sprite with referenced texture (unmanaged)
	/// @param texture used texture
	/// @param rect Rect2D for the sprite position
	/// @param mvp MVP matrix for the shader
	static std::unique_ptr<Sprite> make (Texture *texture, const Rect2D<GLfloat>& rect, const glm::mat4* mvp);

	static consteval GLuint textureUnit = 0;
};

}
