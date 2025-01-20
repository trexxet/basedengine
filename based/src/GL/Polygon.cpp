#include "GL/Polygon.hpp"

#include "GL/GL_Util.hpp"

namespace Based::GL {

Polygon::Polygon (GLsizei vertexCount, GLenum drawMode, GLenum VBO_usage, 
                  const VBOSpan* VBO_array, const EBOSpan* EBO_array, bool startVAOBatch) 
	: drawMode(drawMode), vertexCount(vertexCount) {
	glGenVertexArrays (1, &VAO);
	glBindVertexArray (VAO);

	glGenBuffers (1, &VBO);
	glBindBuffer (GL_ARRAY_BUFFER, VBO);
	glBufferData (GL_ARRAY_BUFFER, VBO_array->size_bytes(), VBO_array->data(), VBO_usage);

	if (EBO_array) {
		glGenBuffers (1, &EBO);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData (GL_ELEMENT_ARRAY_BUFFER, EBO_array->size_bytes(), EBO_array->data(), GL_STATIC_DRAW);
	}

	if (!startVAOBatch)
		glBindVertexArray (0);

	BASED_GL_CHECK ("Error while creating a primitive");
}

// todo: normalized?
// todo: glEnableVertexAttribArray better be called before linking of SP
void Polygon::addAttribute (GLuint index, GLint size, int stride, int pointer) {
	glVertexAttribPointer (index, size, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void *)(pointer * sizeof(GLfloat)));
	glEnableVertexAttribArray (index);
	BASED_GL_CHECK ("Error setting attributes to a primitive");
}

void Polygon::draw () {
	if (EBO)
		glDrawElements (drawMode, vertexCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays (drawMode, 0, vertexCount);
	BASED_GL_CHECK ("Error drawing a primitive");
}

Polygon::~Polygon () {
	if (!start_VAO_batch()) return;
	if (EBO)
		glDeleteBuffers (1, &EBO);
	glDeleteBuffers (1, &VBO);
	glDeleteVertexArrays (1, &VAO);
	end_VAO_batch();
	BASED_GL_CHECK ("Error deleting a primitive");
}

std::unique_ptr<Rect> Rect::make (GLenum VBO_usage, const std::vector<GLfloat>& VBO_vec, bool startVAObatch) {
	std::span<const GLfloat> VBO_span {VBO_vec};
	std::unique_ptr<Rect> rect = std::make_unique<Rect> (VBO_usage, &VBO_span, true);
	rect->addAttribute (0, 2, 4, 0);
	rect->addAttribute (1, 2, 4, 2);
	if (!startVAObatch)
		rect->end_VAO_batch ();
	return rect;
}

std::unique_ptr<Rect> Rect::make (GLenum VBO_usage, const Rect2D<GLfloat>& xy, const Rect2D<GLfloat>& st, bool startVAObatch) {
	return make (VBO_usage, generateVBO (xy, st), startVAObatch);
}

std::vector<GLfloat> Rect::generateVBO (const Rect2D<GLfloat>& xy, const Rect2D<GLfloat>& st) {
	return {
		//    x            y            s            t
		   xy.x,        xy.y,        st.s,        st.t,
		   xy.x + xy.w, xy.y,        st.s + st.w, st.t,
		   xy.x + xy.w, xy.y + xy.h, st.s + st.w, st.t + st.h,
		   xy.x,        xy.y + xy.h, st.s,        st.t + st.h
	};
}

}
