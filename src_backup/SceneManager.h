#ifndef __RUIN_SCENE_MANAGER
#define __RUIN_SCENE_MANAGER

#include <string>
#include <memory>
#include <unordered_map>

#include "ISceneService.h"

namespace Ruin
{
	class SceneManager;
	typedef std::shared_ptr<SceneManager> SceneManagerPtr;

	class SceneManager : public ISceneService
	{
		std::unordered_map<int, ScenePtr> scenes;

		ScenePtr currentScene;

	public:
		void addScene(ScenePtr scene);

		void loadScene(int sceneId);

		void switchScene(int sceneId);

		ScenePtr getCurrent();
	};
}

#endif