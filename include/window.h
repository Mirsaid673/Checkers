#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

class Window
{
private:
	GLFWwindow *window;

	int width;
	int height;

public:
	void init(int w, int h, const char *ttl);
	void cleanup();
	void swapBuffers();
	int shouldClose();
	void updateSize();

	operator GLFWwindow *() const { return window; };
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	int getMouseButton(int button) { return glfwGetMouseButton(window, button); }
};