#pragma once

#include "mesh2D.h"
#include "texture.h"
#include "shader.h"

#include <glm/glm.hpp>

class Model
{
private:
	Mesh2D &mesh;
	Texture &texture;
	Shader &shader;

public:
	Model(Mesh2D &m, Texture &t, Shader &s) : mesh(m), texture(t), shader(s) {}
	void draw(const glm::vec2 &position);
};