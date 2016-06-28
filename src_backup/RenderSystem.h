#ifndef __RUIN_RENDER_SYSTEM
#define __RUIN_RENDER_SYSTEM

#include <memory>
#include <vector>

#include "IRenderer.h"
#include "IComponentSystem.h"
#include "RenderComponent.h"

namespace Ruin
{
	class RenderSystem : public IComponentSystem<RenderComponent>
	{
		std::vector<RenderComponentPtr> components;

	public:

		void update(double time);

		void render(RendererPtr renderer);

		void addComponent(RenderComponentPtr component);

		RenderSystem();

		~RenderSystem();

	private:
	};

	typedef std::shared_ptr<RenderSystem> RenderSystemPtr;
}

#endif