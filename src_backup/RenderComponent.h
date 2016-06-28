#ifndef __RUIN_RENDER_COMPONENT_H_
#define __RUIN_RENDER_COMPONENT_H_

#include <string>
#include <memory>
#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Entity.h"
#include "Component.h"
#include "IRenderer.h"

namespace Ruin 
{
	class RenderComponent : public Component
	{
		unsigned int modelHandle;
		unsigned int material;

		glm::mat4 modelViewMatrix;

	public:
		static const char* RenderComponentTypeName;

		virtual std::string getTypeName();

		virtual void update(double time);

		virtual unsigned int getSortKey();

		virtual void draw(RendererPtr spriteBatch);

		void setModel(unsigned int modelId);

		void setMaterial(unsigned int materialId);

		RenderComponent(EntityPtr entity);
	};

	typedef std::shared_ptr<RenderComponent> RenderComponentPtr;
}

#endif