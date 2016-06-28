#ifndef __RUIN_I_RENDERER_H_
#define __RUIN_I_RENDERER_H_

#include <memory>
#include "SpriteBatch.h"

namespace Ruin
{
	class IRenderer;
	typedef std::shared_ptr<IRenderer> RendererPtr;

	class IRenderer
	{
	public:
		virtual void setViewPort(int x, int y, int width, int height) = 0;
		virtual void clearBuffer() = 0;
		virtual void setMaterial(int materialId) = 0;
		virtual void drawElements(int numIndicies) = 0;
		virtual void setPerspectiveMatrix(const glm::mat4 &matrix) = 0;
		virtual void setModelViewMatrix(const glm::mat4 &matrix) = 0;
		virtual void setModel(unsigned int modelHandle) = 0;

		virtual void clearCommandBuffer() = 0;

		virtual bool initialize() = 0;

		virtual void render() = 0;
	};
}

#endif