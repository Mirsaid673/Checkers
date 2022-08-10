#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::bind()
{
	glBindVertexArray(ID);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}

void VAO::destroy()
{
	glDeleteVertexArrays(1, &ID);
}

void VAO::linkAttrib(GLuint layout, GLuint num_components, GLsizeiptr stride, GLvoid *offset)
{
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(layout, num_components, GL_FLOAT, GL_FALSE, stride, offset);
}