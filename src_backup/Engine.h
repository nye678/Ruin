#ifndef __RUIN_ENGINE_H_
#define __RUIN_ENGINE_H_

#include <vector>

#include "IRenderer.h"
#include "IWindow.h"
#include "RenderSystem.h"
#include "Entity.h"

namespace Ruin
{
	class Engine
	{
		RendererPtr renderer;
		WindowPtr window;

		RenderSystemPtr renderSystem;
		std::vector<EntityPtr> entities;

	public:
		bool initialize();
		void run();
		void shutdown();

		Engine();
		~Engine();
	};
}
#endif