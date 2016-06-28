#ifndef __RUIN_MATERIAL_MANAGER_H_
#define __RUIN_MATERIAL_MANAGER_H_

#include "IResourceLoader.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <GL3\glcorearb.h>
#include <glm\glm.hpp>

namespace Ruin
{
	class MaterialManager : public IResourceLoader
	{
		typedef std::unordered_map<std::string, unsigned int> MaterialDictionary;

		MaterialDictionary materials;

		const int TransformBlockIndex = 0;
		GLuint uniformBuffer;
		float transformBlock[32];

		bool materialExists(const std::string &filepath);

	public:
		unsigned int loadResource(const std::string &filepath);

		void unloadResource(unsigned int resourceId);

		void setActiveMaterial(unsigned int materialId);

		void setProjectionMatrix(float* matrix);

		void setModelViewMatrix(float* matrix);

		void updateUniforms();

		void initialize();
	};

	typedef std::shared_ptr<MaterialManager> MaterialManagerPtr;
}

#endif