#include <cassert>

#include "Services.h"

using namespace Ruin;

void Services::Initialize()
{
	SettingsInstance.Create();
	ResourceCacheInstance.Create();
	MessageServiceInstance.Create();
	CameraServiceInstance.Create();
}