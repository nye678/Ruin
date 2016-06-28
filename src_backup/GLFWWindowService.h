#ifndef __RUIN_GLFW_WINDOW_SERVICE
#define __RUIN_GLFW_WINDOW_SERVICE

#include "IWindowService.h"
#include "GLFWWindow.h"

namespace Ruin
{
	class GLFWWindowService : public IWindowService
	{
		GLFWWindowPtr window;

	public:
		bool createMainWindow();

		WindowPtr getMainWindow();
	};
}

#endif