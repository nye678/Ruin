#ifndef __RUIN_INPUT_SYSTEM
#define __RUIN_INPUT_SYSTEM

#include <vector>

#include "IComponentSystem.h"
#include "InputComponent.h"

namespace Ruin
{
	class InputSystem : IComponentSystem<InputComponent>
	{
		std::vector<InputComponentPtr> components;

	public:
		void update(double time);

		void addComponent(InputComponentPtr component);
	};
}

#endif