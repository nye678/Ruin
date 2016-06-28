#ifndef __RUIN_GLFW_WINDOW
#define __RUIN_GLFW_WINDOW

#include <memory>
#include <GL3\gl3w.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include "IWindow.h"

namespace Ruin
{
	class GLFWWindow;
	typedef std::shared_ptr<GLFWWindow> GLFWWindowPtr;

	class GLFWWindow : public IWindow
	{
		GLFWwindow* window;
	public:

		bool createWindow();

		glm::ivec2 getSize();

		bool closeRequested();

		void pollEvents();

		void swapBuffers();

		void closeWindow();

		GLFWWindow();
		~GLFWWindow();
	};
}

#endif