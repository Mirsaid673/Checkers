#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera2D
{
private:
	glm::mat4 projection;
	glm::mat4 view;

public:
	Camera2D() : view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)) {}

	void othographic(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f);

	void translate(glm::vec3 v);
	const glm::mat4 &getView() const { return view; }
	const glm::mat4 &getProjection() const { return projection; }
};