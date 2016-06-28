#ifndef __RUIN_RENDERER_H_
#define __RUIN_RENDERER_H_

#include <GL3\gl3w.h>
#include <GLFW\glfw3.h>
#include "IRenderer.h"
#include "ModelManager.h"
#include "MaterialManager.h"

namespace Ruin
{
	typedef void(*RenderFuncPtr)(int*);

	class OGLRenderer : public IRenderer
	{
		static const int MAX_COMMAND_BUFFER_SIZE = 1024;
		static const int MAX_MATRIX_BUFFER_SIZE = 512;

		GLuint program;
		GLuint vao;

		static ModelManagerPtr modelManager;
		static MaterialManagerPtr materialManager;

		glm::mat4 projMat;

		int numIndicies = 0;

		unsigned int vertCount;

		bool swapCommandBuffer();

		struct RenderCommand
		{
			RenderFuncPtr command;
			int args[4];
		};

		RenderCommand commandBuffer[MAX_COMMAND_BUFFER_SIZE];
		int bufferIndex;

		glm::mat4 matrixBuffer[MAX_MATRIX_BUFFER_SIZE];
		int matrixIndex;

		static void setMaterialInternal(int* args);
		static void setPerspectiveMatrixInternal(int* args);
		static void drawElementsInternal(int* args);
		static void clearBufferInternal(int* args);
		static void setViewPortInternal(int* args);
		static void setModelViewMatrixInternal(int* args);
		static void setModelInternal(int* args);

	public:

		void setViewPort(int x, int y, int width, int height);
		void clearBuffer();
		void setMaterial(int materialId);
		void drawElements(int numIndicies);
		void setPerspectiveMatrix(const glm::mat4 &matrix);
		void setModelViewMatrix(const glm::mat4 &matrix);
		void setModel(unsigned int modelHandle);

		bool initialize();

		void render();

		void clearCommandBuffer();

		OGLRenderer();

		~OGLRenderer();
	};
}

#endif