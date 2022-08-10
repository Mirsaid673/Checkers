#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "VAO.h"
#include "buffer.h"
#include "texture.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	VAO vao;
	Buffer vbo;
	Buffer ebo;
	Texture texture;

public:
	Mesh(const std::vector<Vertex> &v, const std::vector<GLuint> &i, const Texture &t);
	void draw();
	void destroy();
};