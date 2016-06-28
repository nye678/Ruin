#include "Camera.h"

using namespace glm;
using namespace Ruin;

Camera::Camera()
	: _position(vec3(0.0f, 0.0f, 0.0f)), _orientation(fquat(1.0f, 0.0f, 0.0f, 0.0f))
{}

vec3 Camera::getPosition() const
{
	return _position;
}

vec3 &Camera::getPosition()
{
	return _position;
}

void Camera::setPosition(const vec3 &vec)
{
	_position = vec;
}