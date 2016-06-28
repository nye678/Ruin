#include "InputComponent.h"

using namespace std;
using namespace Ruin;

const char* InputComponent::InputComponentName = "InputComponent";

void InputComponent::update(double time)
{}

string InputComponent::getTypeName()
{
	return string(InputComponentName);
}