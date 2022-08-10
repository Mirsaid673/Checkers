#include "mesh.h"

#include <iostream>

Mesh::Mesh(const std::vector<Vertex> &v, const std::vector<GLuint> &i, const Texture &t) : vertices(v), indices(i), texture(t)
{
	vao.bind();

	vbo.set_data(GL_ARRAY_BUFFER, vertices.data(), vertices.size() * sizeof(Vertex));
	ebo.set_data(GL_ELEMENT_ARRAY_BUFFER, indices.data(), indices.size() * sizeof(GLuint));

	vao.linkAttrib(0, 3, sizeof(Vertex), (GLvoid *)offsetof(Vertex, position));
	vao.linkAttrib(1, 3, sizeof(Vertex), (GLvoid *)offsetof(Vertex, color));
	vao.linkAttrib(2, 3, sizeof(Vertex), (GLvoid *)offsetof(Vertex, normal));
	vao.linkAttrib(3, 2, sizeof(Vertex), (GLvoid *)offsetof(Vertex, uv));

	vao.unbind();
	vbo.unbind();
	ebo.unbind();
}

void Mesh::draw()
{
	// texture.use(GL_TEXTURE0);

	vao.bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::destroy()
{
	vao.destroy();
	vbo.destroy();
	ebo.destroy();
	texture.destroy();
}