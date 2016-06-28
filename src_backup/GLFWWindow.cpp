#include <string>

#include "GLFWWindow.h"
#include "Services.h"

using namespace std;
using namespace glm;
using namespace Ruin;

GLFWWindow::GLFWWindow()
	: window(nullptr)
{}

GLFWWindow::~GLFWWindow()
{}

bool GLFWWindow::createWindow()
{
	glfwSetErrorCallback([](int error, const char* description)
	{
		fputs(description, stderr);
	});

	if (!glfwInit())
	{
		return false;
	}

	auto settings = Services::Settings();

	int width = settings->GetIntSetting("WindowWidth");
	int height = settings->GetIntSetting("WindowHeight");
	string title = settings->GetSetting("Title");
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (gl3wInit() == -1)
	{
		return false;
	}

	return true;
}

ivec2 GLFWWindow::getSize()
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	return ivec2(width, height);
}

bool GLFWWindow::closeRequested()
{
	return glfwWindowShouldClose(window) == 1;
}

void GLFWWindow::pollEvents()
{
	glfwPollEvents();
}

void GLFWWindow::swapBuffers()
{
	glfwSwapBuffers(window);
}

void GLFWWindow::closeWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}