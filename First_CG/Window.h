#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
	
	private:
	
		GLFWwindow* window;

	public:
	
		Window(int width, int height);

		bool windowHasError();

		bool windowShouldClose();

		void swapAndPoll();

		GLFWwindow* getWindow();
};

#endif
