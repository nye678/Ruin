#include "Entity.h"

using namespace std;
using namespace glm;
using namespace Ruin;

Entity::Entity()
{
	position = vec3(0.0f);
	scale = vec3(1.0f);
	orientation = quat(1.0f, 0.0f, 0.0f, 0.0f);
}

vec3 Entity::getPosition() const
{
	return position;
}

vec3 &Entity::getPosition()
{
	return position;
}

void Entity::setPosition(const vec3 &position)
{
	this->position = position;
}

vec3 Entity::getScale() const
{
	return scale;
}

vec3 &Entity::getScale()
{
	return scale;
}

void Entity::setScale(const vec3 &scale)
{
	this->scale = scale;
}

quat Entity::getOrientation() const
{
	return orientation;
}

quat &Entity::getOrientation()
{
	return orientation;
}

void Entity::setOrientation(const quat &orientation)
{
	this->orientation = orientation;
}