#ifndef __RUIN_INPUT_COMPONENT_H_
#define __RUIN_INPUT_COMPONENT_H_

#include <string>
#include <memory>

#include "Component.h"

namespace Ruin {

	class InputComponent;
	typedef std::shared_ptr<InputComponent> InputComponentPtr;

	class InputComponent : public Component
	{
	public:
		static const char* InputComponentName;

		virtual std::string getTypeName();

		virtual void update(double time);
	};
}

#endif