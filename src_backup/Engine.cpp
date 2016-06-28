#include <cassert>
#include <random>
#include <time.h>

#include "Engine.h"
#include "Services.h"
#include "OGLRenderer.h"
#include "GLFWWindow.h"
#include "ResourceCache.h"

#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace std;
using namespace Ruin;

Engine::Engine()
{
	renderer = RendererPtr(new OGLRenderer());
	window = WindowPtr(new GLFWWindow());

	renderSystem = RenderSystemPtr(new RenderSystem());
}

Engine::~Engine()
{
	shutdown();
}

void testCreateEntities(vector<EntityPtr> &entities, RenderSystemPtr renderSystem)
{
	auto resources = Services::ResourceCache();
	unsigned int torus = resources->loadResource("torus");
	unsigned int sphere = resources->loadResource("sphere");
	unsigned int cube = resources->loadResource("cube");

	unsigned int basicMaterial = resources->loadResource("basic");
	unsigned int blueMaterial = resources->loadResource("blue");

	unsigned int m = basicMaterial;
	srand(time(NULL));

	for (int i = 0; i < 10; i++)
	{
		auto entity = EntityPtr(new Entity());
		entity->setPosition(glm::vec3(rand() % 10 - rand() % 10, rand() % 10 - rand() % 10, rand() % 5 * -1));
		//entity->setScale(glm::vec3(rand() % 5, rand() % 5, rand() % 5));

		auto renderComponent = RenderComponentPtr(new RenderComponent(entity));
		renderComponent->setModel(i % 3 + 1);
		renderComponent->setMaterial(m);
		m = m == basicMaterial ? blueMaterial : basicMaterial;
		
		entities.push_back(entity);
		renderSystem->addComponent(renderComponent);
	}
}

bool Engine::initialize()
{
	Services::Initialize();

	Services::ResourceCache()->readManifestFromFile("data\\manifest.json");
	Services::Settings()->Initialize();
	Services::Settings()->LoadSettings("");

	bool windowCreated = window->createWindow();
	assert(windowCreated);

	bool rendererInitialized = renderer->initialize();
	assert(rendererInitialized);

	testCreateEntities(entities, renderSystem);

	return windowCreated && rendererInitialized;
}

void Engine::run()
{
	double lastFrameTime = 0.0;
	double secPerFrame = 1.0 / 30.0;

	while (!window->closeRequested())
	{
		double time = glfwGetTime();
		double deltaTime = time - lastFrameTime;
		if (deltaTime >= secPerFrame)
		{
			renderSystem->update(deltaTime);

			renderer->clearCommandBuffer();

			auto windowSize = window->getSize();
			renderer->setViewPort(0, 0, windowSize.x, windowSize.y);
			renderer->clearBuffer();

			float ratio = (float)windowSize.x / (float)windowSize.y;
			auto perspectiveMatrix = glm::perspective(50.0f, ratio, 0.1f, 1000.0f);
			renderer->setPerspectiveMatrix(perspectiveMatrix);

			renderSystem->render(renderer);

			renderer->render();
			window->swapBuffers();

			lastFrameTime = time;
		}

		window->pollEvents();
	}
}

void Engine::shutdown()
{
	window->closeWindow();
}