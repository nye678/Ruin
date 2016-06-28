#ifndef __RUIN_ENTITY_H_
#define __RUIN_ENTITY_H_

#include <memory>
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

namespace Ruin 
{
	class Entity
	{
		glm::vec3 position;
		glm::vec3 scale;
		glm::fquat orientation;

	public:
		/* Position
		 * Gets or sets the position of the entity. */
		glm::vec3 getPosition() const;
		glm::vec3 &getPosition();
		void setPosition(const glm::vec3 &);

		/* Scale
		 * Gets or sets the scale of the entity. */
		glm::vec3 getScale() const;
		glm::vec3 &getScale();
		void setScale(const glm::vec3 &);

		/* Orientation
		 * Gets or sets the orientation of the entity. */
		glm::quat getOrientation() const;
		glm::quat &getOrientation();
		void setOrientation(const glm::quat &);

		Entity();
	};

	typedef std::shared_ptr<Entity> EntityPtr;
}

#endif