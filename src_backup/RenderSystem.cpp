#include "RenderSystem.h"

using namespace std;
using namespace Ruin;

RenderSystem::RenderSystem()
{}

RenderSystem::~RenderSystem()
{
	for (auto component : components)
	{
		component.reset();
	}
	components.clear();
}

void RenderSystem::update(double time)
{
	for (auto component : components)
	{
		component->update(time);
	}
}

void RenderSystem::render(RendererPtr renderer)
{	
	for (auto component : components)
	{
		component->draw(renderer);
	}
}

void RenderSystem::addComponent(RenderComponentPtr component)
{
	components.push_back(component);
}