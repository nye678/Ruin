#ifndef __RUIN_CAMERA_COMPONENT
#define __RUIN_CAMERA_COMPONENT

#include <memory>
#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Entity.h"
#include "Component.h"

namespace Ruin
{
	class CameraComponent : public Component
	{
		glm::mat4 cameraView;

	public:
		static const char* CameraComponentTypeName;

		const glm::mat4 &getCameraView() const;

		virtual std::string getTypeName() const;

		virtual void update(double time);

		void setOrthographic(float left, float top, float right, float bottom, float near, float far);
		void setPerspective(float fov, float aspectRatio, float near, float far);

		CameraComponent(EntityPtr entity);
	};

	typedef std::shared_ptr<CameraComponent> CameraComponentPtr;
}

#endif