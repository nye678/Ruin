#ifndef __RUIN_SPRITE_BATCH_H_
#define __RUIN_SPRITE_BATCH_H_

#include <glm\glm.hpp>
#include <memory>
#include "Id.h"

namespace Ruin
{
	class SpriteBatch;
	typedef std::shared_ptr<SpriteBatch> SpriteBatchPtr;

	class SpriteBatch
	{
	public:
		void draw(Id textureId, glm::vec2 position, glm::vec2 size);
	};
}

#endif