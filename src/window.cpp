#include "window.h"

#include <iostream>

void Window::init(int w, int h, const char *ttl)
{
	width = w;
	height = h;
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW!" << std::endl;
	}

	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, ttl, nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to init GLEW" << std::endl;
		glfwTerminate();
	}

	glViewport(0, 0, w, h);
	// glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void Window::cleanup()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::swapBuffers()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}
int Window::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void Window::updateSize()
{
	glfwGetWindowSize(window, &width, &height);
}