#ifndef __RUIN_I_COMPONENT_SYSTEM_H_
#define __RUIN_I_COMPONENT_SYSTEM_H_

#include <memory>

#include "Component.h"

namespace Ruin
{
	template<typename ComponentType>
	class IComponentSystem
	{
	public:
		virtual void update(double time) = 0;

		virtual void addComponent(std::shared_ptr<ComponentType> component) = 0;
	};
}

#endif