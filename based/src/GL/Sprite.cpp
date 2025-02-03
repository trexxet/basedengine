#include "GL/Sprite.hpp"

namespace Based::GL {

Sprite::Sprite (const Rect2D<GLfloat>& rect, const glm::mat4* mvp) : mvp (mvp) {
	glRect = Rect::make (GL_STATIC_DRAW, rect, Texture::full());
	texture = new Texture();
	textureManaged = true;
}

Sprite::Sprite (Texture *texture, const Rect2D<GLfloat>& rect, const glm::mat4* mvp) : texture (texture), mvp (mvp) {
	glRect = Rect::make (GL_STATIC_DRAW, rect, Texture::full());
	textureManaged = false;
	loaded = texture->loaded;
}

void Sprite::load (const std::string& path) {
	if (!textureManaged) return;
	texture->load (path);
	loaded = true;
}

bool Sprite::prepare () {
	if (ready || !loaded)
		return ready;

	if (!Default::shaders[Default::SP_2D_MVPSampler].ready || !texture)
		return false;
	
	ready = texture->ready;
	if (!ready && textureManaged)
		ready = texture->prepare();
	return ready;
}

void Sprite::unload () {
	if (textureManaged && texture) {
		delete texture;
		texture = nullptr;
		textureManaged = false;
	}
	loaded = false;
	ready = false;
}

Sprite::~Sprite () {
	if (loaded) unload();
}

void Sprite::draw () {
	ShaderProgram& sp = Default::shaders[Default::SP_2D_MVPSampler];
	sp.use();
	sp.set_uniform ("mvp", *mvp);
	sp.set_uniform ("tex", textureUnit);
	texture->use (textureUnit);
	glRect->bind_draw();
}

std::unique_ptr<Sprite> Sprite::make (const std::string& path, const Rect2D<GLfloat>& rect, const glm::mat4* mvp) {
	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite> (rect, mvp);
	sprite->load (path);
	if (!sprite->prepare()) [[unlikely]]
		log.fatal ("Failed to prepare sprite {}", path);
	return sprite;
}

std::unique_ptr<Sprite> Sprite::make (Texture *texture, const Rect2D<GLfloat>& rect, const glm::mat4* mvp) {
	// For some cases, texture may be prepared later, so we need only to check the shader status
	if (!Default::shaders[Default::SP_2D_MVPSampler].ready)
		log.fatal ("Failed to prepare sprite - shader not ready");

	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite> (texture, rect, mvp);
	sprite->ready = texture->ready;
	return sprite;
}

}
