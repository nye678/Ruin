#ifndef __RUIN_COMPONENT_H_
#define __RUIN_COMPONENT_H_

#include <string>
#include <memory>

#include "Id.h"
#include "Entity.h"

namespace Ruin {

	class Component;
	typedef std::shared_ptr<Component> ComponentPtr;

	/* Component
	 * Base component class. */
	class Component
	{
		Id id;

	protected:
		EntityPtr entity;

		Component(EntityPtr entity);

	public:
		Id getId();

		/* Get Type Name
		 * Gets the name of the component type. */
		virtual std::string getTypeName() = 0;

		/* Update 
		 * Updates the current state of the component. */
		virtual void update(double time) = 0;
	};

}

#endif