#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "camera2D.h"

std::string get_file_contents(const char *filename);

class Shader
{
private:
	GLuint ID;

public:
	Shader(const char *vertexFile, const char *fragmentFile);
	void use();
	void destroy();
	operator GLuint() { return ID; }

	void setInt(const char *name, int value);
	void setUint(const char *name, unsigned int value);
	void setFloat(const char *name, float value);
	void setDouble(const char *name, double value);

	void setVec2i(const char *name, int value1, int value2);
	void setVec2ui(const char *name, unsigned int value1, unsigned int value2);
	void setVec2(const char *name, float value1, float value2);
	void setVec2d(const char *name, double value1, double value2);

	void setVec2i(const char *name, int *values);
	void setVec2ui(const char *name, unsigned int *values);
	void setVec2(const char *name, float *values);
	void setVec2(const char *name, const glm::vec2 &v);
	void setVec2d(const char *name, double *values);

	void setVec3i(const char *name, int value1, int value2, int value3);
	void setVec3ui(const char *name, unsigned int value1, unsigned int value2, unsigned int value3);
	void setVec3(const char *name, float value1, float value2, float value3);
	void setVec3(const char *name, const glm::vec3 &v);
	void setVec3d(const char *name, double value1, double value2, double value3);

	void setVec3i(const char *name, int *values);
	void setVec3ui(const char *name, unsigned int *values);
	void setVec3(const char *name, float *values);
	void setVec3d(const char *name, double *values);

	void setMat3(const char *name, float *m);
	void setMat3(const char *name, const glm::mat3 &m);

	void setMat4(const char *name, float *m);
	void setMat4(const char *name, const glm::mat4 &m);

	void setCamera(const Camera2D& c);

private:
	void compileErrors(unsigned int shader, const char *type);
};