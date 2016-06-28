#ifndef __RUIN_I_SCENE_SERVICE_H_
#define __RUIN_I_SCENE_SERVICE_H_

#include <memory>
#include <string>

#include "Id.h"
#include "Scene.h"

namespace Ruin {

	class ISceneService;
	typedef std::shared_ptr<ISceneService> ISceneServicePtr;

	class ISceneService
	{
	public:
		virtual void loadScene(Id sceneId) = 0;

		virtual void switchScene(Id sceneId) = 0;
	};
}

#endif