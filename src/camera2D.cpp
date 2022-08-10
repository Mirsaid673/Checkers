#include "camera2D.h"

void Camera2D::othographic(float left, float right, float bottom, float top, float near, float far)
{
	projection = glm::orthoLH(left, right, bottom, top, near, far);
}

void Camera2D::translate(glm::vec3 v)
{
	view = glm::translate(view, -v);
}