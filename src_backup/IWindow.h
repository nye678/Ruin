#ifndef __RUIN_I_WINDOW
#define __RUIN_I_WINDOW

#include <memory>
#include <glm\glm.hpp>

namespace Ruin
{
	class IWindow;
	typedef std::shared_ptr<IWindow> WindowPtr;

	class IWindow
	{
	public:
		virtual bool createWindow() = 0;

		virtual glm::ivec2 getSize() = 0;

		virtual bool closeRequested() = 0;

		virtual void pollEvents() = 0;

		virtual void swapBuffers() = 0;

		virtual void closeWindow() = 0;
	};
}

#endif