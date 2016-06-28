#include "InputSystem.h"

using namespace std;
using namespace Ruin;

void InputSystem::update(double time)
{
	for (auto component : components)
	{
		component->update(time);
	}
}

void InputSystem::addComponent(InputComponentPtr component)
{
	components.push_back(component);
}