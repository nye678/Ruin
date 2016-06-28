#ifndef __RUIN_SCENE_H_
#define __RUIN_SCENE_H_

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "IRenderer.h"
#include "InputSystem.h"
#include "RenderSystem.h"

namespace Ruin
{
	class Scene;
	typedef std::shared_ptr<Scene> ScenePtr;

	class Scene
	{
		int id;

		std::vector<EntityPtr> entities;

	public:
		
		int getId();

		std::vector<EntityPtr> getActiveEntities();

		Scene();
	};
}

#endif