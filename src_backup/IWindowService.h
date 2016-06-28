#ifndef __RUIN_I_WINDOW_SERVICE
#define __RUIN_I_WINDOW_SERVICE

#include <memory>

#include "IWindow.h"

namespace Ruin
{
	class IWindowService
	{
	public:
		virtual bool createMainWindow() = 0;

		virtual WindowPtr getMainWindow() = 0;
	};

	typedef std::shared_ptr<IWindowService> WindowServicePtr;
}

#endif