#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "VAO.h"
#include "buffer.h"
#include "texture.h"

struct Vertex2D
{
	glm::vec2 position;
	glm::vec2 uv;
};

class Mesh2D
{
private:
	std::vector<Vertex2D> vertices;
	std::vector<GLuint> indices;
	VAO vao;
	Buffer vbo;
	Buffer ebo;

public:
	Mesh2D(const std::vector<Vertex2D> &v, const std::vector<GLuint> &i);
	void draw();
	void destroy();
};