#include "CameraService.h"

using namespace Ruin;

CameraService::CameraService()
{}

void CameraService::setCurrent(const Camera &camera)
{
	_current = camera;
}

Camera CameraService::getCurrent()
{
	return _current;
}