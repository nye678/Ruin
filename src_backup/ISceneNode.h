#ifndef __RUIN_I_SCENE_NODE_H_
#define __RUIN_I_SCENE_NODE_H_

#include "IRenderer.h"

namespace Ruin
{

	class ISceneNode
	{
	public:
		virtual void render(RendererPtr renderer)=0;
	};

}

#endif