#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

Window::Window(int width, int height) {

	/* initialize the library */
	/*if (!glfwInit())
	{
		return -1;
	};*/

	/* create a windowed mode window and its opengl context */
	window = glfwCreateWindow(1280, 800, "hello world", NULL, NULL);
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
	return (!glfwInit || !window);
}

bool Window::windowShouldClose() {
	return glfwWindowShouldClose;
}

void Window::swapAndPoll() {
	//swap front and back buffers 
	glfwSwapBuffers(window);
	// poll for and process events 
	glfwPollEvents();
}