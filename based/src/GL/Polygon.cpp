#include "GL/Polygon.hpp"

#include "GL/GL_Util.hpp"

namespace Based::GL {

Polygon::Polygon (GLsizei vertexCount, GLenum drawMode, GLenum VBO_usage, 
                  const std::span<GLfloat>* VBO_array, const std::span<GLuint>* EBO_array, bool startVAOBatch) 
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

}
