#include "model.h"

void Model::draw(const glm::vec2 &position)
{
	shader.use();
	shader.setVec2("pos", position);

	shader.setInt("tex", 0);
	texture.use(GL_TEXTURE0);

	mesh.draw();
}