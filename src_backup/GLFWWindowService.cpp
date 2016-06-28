#include <string>

#include "GLFWWindowService.h"
#include "Services.h"

using namespace std;
using namespace Ruin;

bool GLFWWindowService::createMainWindow()
{
	//window = GLFWWindowPtr(new GLFWWindow());
	//return window->initialize();
	return true;
}

WindowPtr GLFWWindowService::getMainWindow()
{
	return window;
}