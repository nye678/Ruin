#include "Component.h"

using namespace Ruin;

Component::Component(EntityPtr entity)
	: entity(entity), id(0)
{
}

Id Component::getId()
{
	return id;
}