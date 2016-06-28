#include "CameraComponent.h"

using namespace std;
using namespace glm;
using namespace Ruin;

const char* CameraComponent::CameraComponentTypeName = "CameraComponent";

CameraComponent::CameraComponent(EntityPtr entity)
	: Component(entity)
{
	cameraView = mat4(1.0f);
}

const glm::mat4 &CameraComponent::getCameraView() const
{
	return cameraView;
}

string CameraComponent::getTypeName() const
{
	return string(CameraComponentTypeName);
}

void CameraComponent::update(double time)
{
}

void CameraComponent::setOrthographic(float left, float top, float right, float bottom, float near, float far)
{
	cameraView = glm::ortho(left, right, bottom, top, near, far);
}

void CameraComponent::setPerspective(float fov, float aspectRatio, float near, float far)
{
	cameraView = glm::perspective(fov, aspectRatio, near, far);
}