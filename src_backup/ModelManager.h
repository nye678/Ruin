#ifndef __RUIN_MODEL_MANAGER_H_
#define __RUIN_MODEL_MANAGER_H_

#include "IResourceLoader.h"
#include "Model.h"
#include "ObjParser.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <GL3\gl3w.h>

namespace Ruin
{
	class ModelManager : public IResourceLoader
	{
	public:
		static const int MAX_VERTEX_BUFFER_SIZE = 349525;
		static const int MAX_INDEX_BUFFER_SIZE = 2097152;

		struct ModelBufferInfo
		{
			unsigned int modelId;
			GLuint vertexBuffer;
			GLintptr baseVertex;
			unsigned int numVerts;

			GLuint elementBuffer;
			GLintptr baseIndex;
			unsigned int numIndicies;

			GLuint vertexArrayObject;
		};

		struct VertexBufferInfo
		{
			GLuint vertexBuffer;
			unsigned int vertexBufferCapacity;
			unsigned int vertexBufferCount;

			GLuint indexBuffer;
			unsigned int indexBufferCapacity;
			unsigned int indexBufferCount;

			GLuint vertexArrayObject;
		};

		typedef std::shared_ptr<ModelBufferInfo> ModelBufferInfoPtr;
		typedef std::shared_ptr<VertexBufferInfo> VertexBufferInfoPtr;

	private:
		typedef std::unordered_map<unsigned int, ModelBufferInfoPtr> ModelBufferInfoMap;
		typedef std::vector<VertexBufferInfoPtr> VertexBufferInfoVec;

		ModelBufferInfoMap modelBufferInfos;
		VertexBufferInfoVec vertexBufferInfos;

		unsigned int modelIdSource;
		GLuint currentVAO;
		ModelBufferInfoPtr currentModel;

		void createNewVertexBuffer();

	public:
		unsigned int loadResource(const std::string &filepath);

		void unloadResource(unsigned int resourceId);

		void setActiveModel(unsigned int modelId);

		ModelBufferInfoPtr getActiveModel();

		void initialize();

		ModelManager();
		~ModelManager();
	};

	typedef std::shared_ptr<ModelManager> ModelManagerPtr;
}

#endif