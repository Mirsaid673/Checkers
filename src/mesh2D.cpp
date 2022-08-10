#include "mesh2D.h"

#include <iostream>

Mesh2D::Mesh2D(const std::vector<Vertex2D> &v, const std::vector<GLuint> &i) : vertices(v), indices(i)
{
	vao.bind();

	vbo.set_data(GL_ARRAY_BUFFER, vertices.data(), vertices.size() * sizeof(Vertex2D));
	ebo.set_data(GL_ELEMENT_ARRAY_BUFFER, indices.data(), indices.size() * sizeof(GLuint));

	vao.linkAttrib(0, 2, sizeof(Vertex2D), (GLvoid *)offsetof(Vertex2D, position));
	vao.linkAttrib(1, 3, sizeof(Vertex2D), (GLvoid *)offsetof(Vertex2D, color));

	vao.unbind();
	vbo.unbind();
	ebo.unbind();
}

void Mesh2D::draw()
{
	// texture.use(GL_TEXTURE0);

	vao.bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh2D::destroy()
{
	vao.destroy();
	vbo.destroy();
	ebo.destroy();
}