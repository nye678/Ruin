#ifndef __RUIN_CAMERA_H_
#define __RUIN_CAMERA_H_

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

namespace Ruin
{
	class Camera
	{
		glm::vec3 _position;
		glm::fquat _orientation;

	public:
		/* Gets the position of the camera. */
		glm::vec3 getPosition() const;

		/* Gets the position of the camera. */
		glm::vec3 &getPosition();

		/* Sets the position of the camera. */
		void setPosition(const glm::vec3 &);

		Camera();
	};
}

#endif