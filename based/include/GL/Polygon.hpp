#pragma once

#include <concepts>
#include <memory>
#include <span>
#include <vector>

#include <glad/gl.h>

#include "Defs.hpp"
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
	void add_attribute (GLuint index, GLint size, int stride, int pointer);
	/// @brief Draw polygon
	void draw ();
	/// @brief Bind VAO, draw polygon, unbind VAO (if needed) 
	void bind_draw (bool startVAOBatch = false);

	/// @brief Unbind VAO
	inline void end_VAO_batch() {
		glBindVertexArray (0);
	}

	~Polygon ();
	BASED_CLASS_NO_COPY_DEFAULT_MOVE (Polygon);
};

class ConvexPolygon;
template <class T>
concept ConvexPolygonClass = std::derived_from<T, ConvexPolygon>;

/// @note Any convex polygon can be rendered without EBO using GL_TRIANGLE_FAN
class ConvexPolygon : public Polygon {
public:
	ConvexPolygon (GLsizei vertexCount, GLenum VBO_usage, const VBOSpan* VBO_array, bool startVAOBatch = false)
		: Polygon (vertexCount, GL_TRIANGLE_FAN, VBO_usage, VBO_array, nullptr, startVAOBatch) { }

	/// @brief Create a ConvexPolygon from a vector containing VBO, auto generate VAO
	template <ConvexPolygonClass T>
	static std::unique_ptr<T> make (GLenum VBO_usage, const std::vector<GLfloat>& VBO_vec, bool startVAObatch = false) {
		std::span<const GLfloat> VBO_span {VBO_vec};
		std::unique_ptr<T> poly = std::make_unique<T> (VBO_usage, &VBO_span, true);
		poly->add_attribute (0, 2, 4, 0); // X Y s t
		poly->add_attribute (1, 2, 4, 2); // x y S T
		if (!startVAObatch)
			poly->end_VAO_batch ();
		return poly;
	}
};

class ConvexQuad : public ConvexPolygon {
public:
	ConvexQuad (GLenum VBO_usage, const VBOSpan* VBO_array, bool startVAOBatch = false)
		: ConvexPolygon (4, VBO_usage, VBO_array, startVAOBatch) { }
};

class Rect : public ConvexQuad {
public:
	Rect (GLenum VBO_usage, const VBOSpan* VBO_array, bool startVAOBatch = false)
		: ConvexQuad (VBO_usage, VBO_array, startVAOBatch) { }

	/// @brief Create a Rect with auto generated VBO & VAO
	/// @param xy Screen coordinates
	/// @param st Texture coordinates (GL texture space (0, 1))
	inline static std::unique_ptr<Rect> make (GLenum VBO_usage, const Rect2D<GLfloat>& xy, const Rect2D<GLfloat>& st, bool startVAObatch = false) {
		return ConvexPolygon::make<Rect> (VBO_usage, generateVBO (xy, st), startVAObatch);
	}
	
	/// @brief Generate VBO for Rect using screen & texture coordinates
	/// @param xy Screen coordinates
	/// @param st Texture coordinates (GL texture space (0, 1))
	static std::vector<GLfloat> generateVBO (const Rect2D<GLfloat>& xy, const Rect2D<GLfloat>& st);
};

class ConvexHex : public ConvexPolygon {
public:
	ConvexHex (GLenum VBO_usage, const VBOSpan* VBO_array, bool startVAOBatch = false)
		: ConvexPolygon (6, VBO_usage, VBO_array, startVAOBatch) { }
};

class Hex : public ConvexHex {
public:
	Hex (GLenum VBO_usage, const VBOSpan* VBO_array, bool startVAOBatch = false)
		: ConvexHex (VBO_usage, VBO_array, startVAOBatch) { }
	
	enum class Orientation { topPoint, topFlat };

	/// @brief Create a Hex with auto generated VBO & VAO
	/// @param top Orientation of the Hex (flat or point top)
	/// @param xy Outer cirlce for screen coordinates
	/// @param st Outer cirlce for texture coordinates (GL texture space (0, 1))
	inline static std::unique_ptr<Hex> make (GLenum VBO_usage, Orientation o, const Circle2D<GLfloat>& xy, const Circle2D<GLfloat>& st, bool startVAObatch = false) {
		return ConvexPolygon::make<Hex> (VBO_usage, generateVBO (o, xy, st), startVAObatch);
	}

	/// @brief Generate VBO for Hex using screen & texture coordinates
	/// @param top Orientation of the Hex (flat or point top)
	/// @param xy Outer cirlce for screen coordinates
	/// @param st Outer circle for texture coordinates (GL texture space (0, 1))
	static std::vector<GLfloat> generateVBO (Orientation o, const Circle2D<GLfloat>& xy, const Circle2D<GLfloat>& st);
};

}
