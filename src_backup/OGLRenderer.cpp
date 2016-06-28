#include <cassert>
#include <cstdio>
#include <GL3\gl3w.h>
#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <GLFW\glfw3.h>

#include "OGLRenderer.h"
#include "Utility.h"
#include "ObjParser.h"
#include "Services.h"
#include "ResourceCache.h"

using namespace Ruin;
using namespace Ruin::FileTypes;
using namespace Ruin::Utility;

ModelManagerPtr OGLRenderer::modelManager = nullptr;
MaterialManagerPtr OGLRenderer::materialManager = nullptr;

OGLRenderer::OGLRenderer()
	: numIndicies(0), vertCount(0)
{}

OGLRenderer::~OGLRenderer()
{}

bool OGLRenderer::initialize()
{
	modelManager = ModelManagerPtr(new ModelManager());
	modelManager->initialize();
	Services::ResourceCache()->registerResourceLoader(modelManager, ResourceType::Model);

	materialManager = MaterialManagerPtr(new MaterialManager());
	materialManager->initialize();
	Services::ResourceCache()->registerResourceLoader(materialManager, ResourceType::Material);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void OGLRenderer::render()
{

	for (int i = 0; i < bufferIndex; ++i)
	{
		RenderFuncPtr command = commandBuffer[i].command;
		int* args = commandBuffer[i].args;

		command(args);
	}
}

void OGLRenderer::clearCommandBuffer()
{
	bufferIndex = 0;
	matrixIndex = 0;
}

void OGLRenderer::setViewPort(int x, int y, int width, int height)
{
	assert(bufferIndex + 1 < MAX_COMMAND_BUFFER_SIZE);
	commandBuffer[bufferIndex].command = OGLRenderer::setViewPortInternal;
	commandBuffer[bufferIndex].args[0] = x;
	commandBuffer[bufferIndex].args[1] = y;
	commandBuffer[bufferIndex].args[2] = width;
	commandBuffer[bufferIndex].args[3] = height;
	bufferIndex++;
}

void OGLRenderer::setViewPortInternal(int* args)
{
	int x = args[0];
	int y = args[1];
	int width = args[2];
	int height = args[3];

	glViewport(x, y, width, height);
}

void OGLRenderer::clearBuffer()
{
	assert(bufferIndex + 1 < MAX_COMMAND_BUFFER_SIZE);
	commandBuffer[bufferIndex].command = OGLRenderer::clearBufferInternal;
	bufferIndex++;
}

void OGLRenderer::clearBufferInternal(int* args)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderer::setMaterial(int materialId)
{
	assert(bufferIndex + 1 < MAX_COMMAND_BUFFER_SIZE);
	commandBuffer[bufferIndex].command = OGLRenderer::setMaterialInternal;
	commandBuffer[bufferIndex].args[0] = materialId;
	bufferIndex++;
}

void OGLRenderer::setMaterialInternal(int* args)
{
	GLuint shaderProgram = (GLuint)args[0];
	glUseProgram(shaderProgram);
}

void OGLRenderer::drawElements(int numIndices)
{
	assert(bufferIndex + 1 < MAX_COMMAND_BUFFER_SIZE);
	commandBuffer[bufferIndex].command = OGLRenderer::drawElementsInternal;
	bufferIndex++;
}

void OGLRenderer::drawElementsInternal(int* args)
{
	materialManager->updateUniforms();
	auto model = modelManager->getActiveModel();
	glDrawElementsBaseVertex(
		GL_TRIANGLES,
		model->numIndicies,
		GL_UNSIGNED_SHORT,
		(void*)(sizeof(GLushort) * model->baseIndex),
		model->baseVertex);
}

void OGLRenderer::setPerspectiveMatrix(const glm::mat4 &matrix)
{
	assert(bufferIndex + 1 < MAX_COMMAND_BUFFER_SIZE);
	assert(matrixIndex + 1 < MAX_MATRIX_BUFFER_SIZE);
	commandBuffer[bufferIndex].command = OGLRenderer::setPerspectiveMatrixInternal;
	memcpy(&matrixBuffer[matrixIndex], &matrix, sizeof(glm::mat4));
	commandBuffer[bufferIndex].args[0] = (int)glm::value_ptr(matrixBuffer[matrixIndex]);
	matrixIndex++;
	bufferIndex++;

	projMat = matrix;
}

void OGLRenderer::setPerspectiveMatrixInternal(int* args)
{
	GLfloat* matrixPtr = (GLfloat*)args[0];
	materialManager->setProjectionMatrix(matrixPtr);
}

void OGLRenderer::setModelViewMatrix(const glm::mat4 &matrix)
{
	assert(bufferIndex + 1 < MAX_COMMAND_BUFFER_SIZE);
	assert(matrixIndex + 1 < MAX_MATRIX_BUFFER_SIZE);
	commandBuffer[bufferIndex].command = OGLRenderer::setModelViewMatrixInternal;
	memcpy(&matrixBuffer[matrixIndex], &matrix, sizeof(glm::mat4));
	commandBuffer[bufferIndex].args[0] = (int)glm::value_ptr(matrixBuffer[matrixIndex]);
	matrixIndex++;
	bufferIndex++;
}

void OGLRenderer::setModelViewMatrixInternal(int* args)
{
	GLfloat* matrixPtr = (GLfloat*)args[0];
	materialManager->setModelViewMatrix(matrixPtr);
}

void OGLRenderer::setModel(unsigned int modelHandle)
{
	assert(bufferIndex + 1 < MAX_COMMAND_BUFFER_SIZE);
	commandBuffer[bufferIndex].command = OGLRenderer::setModelInternal;
	commandBuffer[bufferIndex].args[0] = (int)modelHandle;
	bufferIndex++;
}

void OGLRenderer::setModelInternal(int* args)
{
	unsigned int modelHandle = (unsigned int)args[0];
	modelManager->setActiveModel(modelHandle);
}