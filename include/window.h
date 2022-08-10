#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

class Window
{
private:
	GLFWwindow *window;

public:
	void init(int w, int h, const char *ttl);
	void cleanup();
	void swap_buffers();
	int getKey(int key);
	int shouldClose();
	operator GLFWwindow *();
};