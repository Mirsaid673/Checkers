#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char *filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return (contents);
	}
	throw(errno);
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char *vertexSource = vertexCode.c_str();
	const char *fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	// Checks if Shader compiled succesfully
	compileErrors(vertexShader, "VERTEX");

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
	// Checks if Shader compiled succesfully
	compileErrors(fragmentShader, "FRAGMENT");

	// Create Shader Program Object and get its reference
	ID = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	// Checks if Shaders linked succesfully
	compileErrors(ID, "PROGRAM");

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// Activates the Shader Program
void Shader::use()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::destroy()
{
	glDeleteProgram(ID);
}
// Checks if the different Shaders have compiled properly
void Shader::compileErrors(unsigned int shader, const char *type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n"
					  << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n"
					  << infoLog << std::endl;
		}
	}
}

void Shader::setInt(const char *name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setUint(const char *name, unsigned int value)
{
	glUniform1ui(glGetUniformLocation(ID, name), value);
}

void Shader::setFloat(const char *name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::setDouble(const char *name, double value)
{
	glUniform1d(glGetUniformLocation(ID, name), value);
}

void Shader::setVec2i(const char *name, int value1, int value2)
{
	glUniform2i(glGetUniformLocation(ID, name), value1, value2);
}
void Shader::setVec2ui(const char *name, unsigned int value1, unsigned int value2)
{
	glUniform2ui(glGetUniformLocation(ID, name), value1, value2);
}

void Shader::setVec2(const char *name, float value1, float value2)
{
	glUniform2f(glGetUniformLocation(ID, name), value1, value2);
}

void Shader::setVec2d(const char *name, double value1, double value2)
{
	glUniform2d(glGetUniformLocation(ID, name), value1, value2);
}

void Shader::setVec2i(const char *name, int *values)
{
	glUniform1iv(glGetUniformLocation(ID, name), 2, values);
}

void Shader::setVec2ui(const char *name, unsigned int *values)
{
	glUniform1uiv(glGetUniformLocation(ID, name), 2, values);
}

void Shader::setVec2(const char *name, float *values)
{
	glUniform1fv(glGetUniformLocation(ID, name), 2, values);
}

void Shader::setVec2(const char *name, const glm::vec2 &v)
{
	glUniform1fv(glGetUniformLocation(ID, name), 2, &v.x);
}

void Shader::setVec2d(const char *name, double *values)
{
	glUniform1dv(glGetUniformLocation(ID, name), 2, values);
}

void Shader::setVec3i(const char *name, int value1, int value2, int value3)
{
	glUniform3i(glGetUniformLocation(ID, name), value1, value2, value3);
}
void Shader::setVec3ui(const char *name, unsigned int value1, unsigned int value2, unsigned int value3)
{
	glUniform3ui(glGetUniformLocation(ID, name), value1, value2, value3);
}

void Shader::setVec3(const char *name, float value1, float value2, float value3)
{
	glUniform3f(glGetUniformLocation(ID, name), value1, value2, value3);
}

void Shader::setVec3(const char *name, const glm::vec3 &v)
{
	glUniform3fv(glGetUniformLocation(ID, name), 3, &v.x);
}

void Shader::setVec3d(const char *name, double value1, double value2, double value3)
{
	glUniform3d(glGetUniformLocation(ID, name), value1, value2, value3);
}

void Shader::setVec3i(const char *name, int *values)
{
	glUniform1iv(glGetUniformLocation(ID, name), 3, values);
}

void Shader::setVec3ui(const char *name, unsigned int *values)
{
	glUniform1uiv(glGetUniformLocation(ID, name), 3, values);
}

void Shader::setVec3(const char *name, float *values)
{
	glUniform1fv(glGetUniformLocation(ID, name), 3, values);
}

void Shader::setVec3d(const char *name, double *values)
{
	glUniform1dv(glGetUniformLocation(ID, name), 3, values);
}

void Shader::setMat3(const char *name, float *m)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, m);
}

void Shader::setMat3(const char *name, const glm::mat3 &m)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::setMat4(const char *name, float *m)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, m);
}

void Shader::setMat4(const char *name, const glm::mat4 &m)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::setCamera(const Camera2D& c)
{
	setMat4("v", c.getView());
	setMat4("p", c.getProjection());
}