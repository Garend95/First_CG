#ifndef INPUTS_H
#define INPUTS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Global.h"

class Inputs {

		Inputs();
	public:
		void processInput(GLFWwindow* window);

		void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};

#endif // !INPUTS_H

