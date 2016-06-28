#include "ModelManager.h"
#include "ObjParser.h"

#include <glm\glm.hpp>

using namespace std;
using namespace glm;
using namespace Ruin;
using namespace Ruin::FileTypes;

ModelManager::ModelManager()
	: modelIdSource(0), currentVAO(0)
{}

ModelManager::~ModelManager()
{}

unsigned int ModelManager::loadResource(const std::string &filepath)
{
	auto objFile = ObjParser::CreateModelFromObj(filepath);
	auto model = objFile.Objects.front();
	
	assert(model.Vertices.size() < MAX_VERTEX_BUFFER_SIZE);

	std::vector<GLushort> indicies;
	for (auto face : model.Faces)
	{
		for (auto index : face)
		{
			indicies.push_back((GLushort)index);
		}
	}

	assert(indicies.size() < MAX_INDEX_BUFFER_SIZE);

	VertexBufferInfoPtr buffer = vertexBufferInfos.back();
	if (buffer->vertexBufferCapacity - buffer->vertexBufferCount < model.Vertices.size() ||
		buffer->indexBufferCapacity - buffer->indexBufferCount < indicies.size())
	{
		createNewVertexBuffer();
		auto buffer = vertexBufferInfos.back();
	}

	ModelBufferInfoPtr info = ModelBufferInfoPtr(new ModelBufferInfo());
	info->modelId = ++modelIdSource;

	glBindBuffer(GL_ARRAY_BUFFER, buffer->vertexBuffer);
	GLintptr offset = sizeof(glm::vec3) * buffer->vertexBufferCount;
	GLintptr size = sizeof(glm::vec3) * model.Vertices.size();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, model.Vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	info->vertexBuffer = buffer->vertexBuffer;
	info->baseVertex = buffer->vertexBufferCount;
	info->numVerts = model.Vertices.size();
	buffer->vertexBufferCount += model.Vertices.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->indexBuffer);
	GLintptr indiciesOffset = sizeof(GLushort) * buffer->indexBufferCount;
	GLintptr indiciesSize = sizeof(GLushort) * indicies.size();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indiciesOffset, indiciesSize, indicies.data());
	info->elementBuffer = buffer->indexBuffer;
	info->baseIndex = buffer->indexBufferCount;
	info->numIndicies = indicies.size();
	buffer->indexBufferCount += indicies.size();

	info->vertexArrayObject = buffer->vertexArrayObject;

	modelBufferInfos.insert(make_pair(info->modelId, info));
	return info->modelId;
}

void ModelManager::unloadResource(unsigned int resourceId)
{}

void ModelManager::setActiveModel(unsigned int resourceId)
{
	if (modelBufferInfos.count(resourceId) > 0)
	{
		ModelBufferInfoPtr model = modelBufferInfos[resourceId];
		if (currentVAO != model->vertexArrayObject)
		{
			glBindVertexArray(model->vertexArrayObject);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->elementBuffer);
			currentVAO = model->vertexArrayObject;
		}

		currentModel = model;
	}
}

ModelManager::ModelBufferInfoPtr ModelManager::getActiveModel()
{
	return currentModel;
}

void ModelManager::initialize()
{
	createNewVertexBuffer();
}

void ModelManager::createNewVertexBuffer()
{
	VertexBufferInfoPtr buffer = VertexBufferInfoPtr(new VertexBufferInfo());

	glGenVertexArrays(1, &buffer->vertexArrayObject);
	glBindVertexArray(buffer->vertexArrayObject);
	currentVAO = buffer->vertexArrayObject;

	GLuint bufferHandles[2];
	glGenBuffers(2, bufferHandles);

	buffer->vertexBuffer = bufferHandles[0];
	buffer->vertexBufferCapacity = MAX_VERTEX_BUFFER_SIZE;
	buffer->vertexBufferCount = 0;

	glBindBuffer(GL_ARRAY_BUFFER, buffer->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_VERTEX_BUFFER_SIZE, nullptr, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	buffer->indexBuffer = bufferHandles[1];
	buffer->indexBufferCapacity = MAX_INDEX_BUFFER_SIZE;
	buffer->indexBufferCount = 0;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * MAX_INDEX_BUFFER_SIZE, nullptr, GL_STATIC_DRAW);

	vertexBufferInfos.push_back(buffer);
}


