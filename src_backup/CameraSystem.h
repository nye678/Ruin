#ifndef __RUIN_CAMERA_SYSTEM
#define __RUIN_CAMERA_SYSTEM

#include <memory>
#include <unordered_map>

#include "IComponentSystem.h"
#include "CameraComponent.h"

namespace Ruin
{
	class CameraSystem : public IComponentSystem<CameraComponent>
	{
		std::unordered_map<EntityPtr, CameraComponentPtr> components;

	public:
		void update(double time);
		void addComponent(CameraComponentPtr component);
		void setActiveCamera(EntityPtr entity);
		CameraComponentPtr getActiveCamera();
		CameraSystem();
		~CameraSystem();
	};
}

#endif