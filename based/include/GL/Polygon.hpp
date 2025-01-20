#pragma once

#include <memory>
#include <span>
#include <vector>

#include <glad/gl.h>

#include "ClassHelper.hpp"
#include "Geometry.hpp"

namespace Based::GL {

using VBOSpan = std::span<const GLfloat>;
using EBOSpan = std::span<const GLuint>;

class Polygon {
	GLuint VAO = 0, VBO = 0, EBO = 0;
	GLsizei vertexCount = 0;
	GLenum drawMode;
public:
	/// @brief Create graphical polygon
	/// @param vertexCount Number of vertices of the polygon
	/// @param drawMode GL draw mode of the polygon
	/// @param VBO_usage Memory usage of the VBO
	/// @param VBO_array std::span* wrap for VBO array
	/// @param EBO_array std::span* wrap for EBO array (null if EBO not used)
	/// @param startVAOBatch true if should leave VAO binded
	Polygon (GLsizei vertexCount, GLenum drawMode, GLenum VBO_usage, const VBOSpan* VBO_array, const EBOSpan* EBO_array, bool startVAOBatch = false);

	/// @brief Bind VAO
	/// \return if successful
	inline bool start_VAO_batch () {
		if (VAO == 0) [[unlikely]]
			return false;
		glBindVertexArray (VAO);
		return true;
	}

	/// @brief Add generic vertex attribute
	/// @param index Index of the generic vertex attribute to be modified
	/// @param size Nnumber of components per generic vertex attribute
	/// @param stride Item count offset between consecutive generic vertex attributes
	/// @param pointer Item count offset of the first component of the first generic vertex attribute in the VBO
	void addAttribute (GLuint index, GLint size, int stride, int pointer);
	/// @brief Draw polygon
	void draw ();

	/// @brief Unbind VAO
	inline void end_VAO_batch() {
		glBindVertexArray (0);
	}

	~Polygon ();
	BASED_CLASS_NO_COPY_DEFAULT_MOVE (Polygon);
};

class ConvexQuad : public Polygon {
public:
	ConvexQuad (GLenum VBO_usage, const VBOSpan* VBO_array, bool startVAOBatch = false)
		: Polygon (4, GL_TRIANGLE_FAN, VBO_usage, VBO_array, nullptr, startVAOBatch) { }
};

class Rect : public ConvexQuad {
public:
	Rect (GLenum VBO_usage, const VBOSpan* VBO_array, bool startVAOBatch = false)
		: ConvexQuad (VBO_usage, VBO_array, startVAOBatch) { }

	/// @brief Create a Rect from a vector containing VBO, auto generate VAO
	static std::unique_ptr<Rect> make (GLenum VBO_usage, const std::vector<GLfloat>& VBO_vec, bool startVAObatch = false);
	/// @brief Create a Rect with auto generated VBO & VAO
	/// @param xy Screen coordinates (GL space from -1 to 1)
	/// @param st Texture coordinates (GL space from 0 to 1)
	static std::unique_ptr<Rect> make (GLenum VBO_usage, const Rect2D<GLfloat>& xy, const Rect2D<GLfloat>& st, bool startVAObatch = false);
	
	/// @brief Generate VBO for Rect using screen & texture coordinates
	/// @param xy Screen coordinates (GL space from -1 to 1)
	/// @param st Texture coordinates (GL space from 0 to 1)
	static std::vector<GLfloat> generateVBO (const Rect2D<GLfloat>& xy, const Rect2D<GLfloat>& st);
};

class ConvexHex : public Polygon {
public:
	ConvexHex (GLenum VBO_usage, const VBOSpan* VBO_array, bool startVAOBatch = false)
		: Polygon (6, GL_TRIANGLE_FAN, VBO_usage, VBO_array, nullptr, startVAOBatch) { }
};
using Hex = ConvexHex;

}
