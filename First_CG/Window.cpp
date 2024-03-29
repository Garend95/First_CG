#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

Window::Window(int width, int height) {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* initialize the library */
	/*if (!glfwInit())
	{
		return -1;
	};*/

	/* create a windowed mode window and its opengl context */
	window = glfwCreateWindow(width, height, "hello world", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		//return -1;
	}
	glfwSwapInterval(1);
	/* make the window's context current */
	glfwMakeContextCurrent(window);

	// call glewinit after creating the context...
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* problem: glewinit failed, something is seriously wrong. */
		fprintf(stderr, "error: %s\n", glewGetErrorString(err));
	}
}

bool Window::windowHasError() {
	bool x = !glfwInit, y = !window;
	return (!glfwInit || !window);
}

bool Window::windowShouldClose() {
	return glfwWindowShouldClose(window);
}

void Window::swapAndPoll() {
	//swap front and back buffers 
	glfwSwapBuffers(window);
	// poll for and process events 
	glfwPollEvents();
}

GLFWwindow* Window::getWindow() {
	return window;
}