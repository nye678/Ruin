#include "MaterialManager.h"
#include "Utility.h"
#include "Services.h"

#include <assert.h>
#include <rapidjson\document.h>

using namespace std;
using namespace rapidjson;
using namespace Ruin;

unsigned int MaterialManager::loadResource(const string &filepath)
{
	if (materialExists(filepath))
	{
		return materials[filepath];
	}

	char* buffer = nullptr;
	size_t read = Utility::LoadTextFile(filepath.c_str(), buffer);
	assert(read > 0);

	Document materialFile;
	materialFile.Parse<0>(buffer);

	delete buffer;
	read = 0;

	char* errors;
	GLuint vertShader = 0, fragShader = 0;
	
	if (materialFile.HasMember("vertex"))
	{
		string vertpath = materialFile["vertex"].GetString();

		read = Utility::LoadTextFile(vertpath.c_str(), buffer);
		assert(read > 0);

		if (!Utility::TryCompileShader(GL_VERTEX_SHADER, buffer, vertShader, errors))
		{
			if (errors)
			{
				Services::Messages()->DisplayNativeDialog("Shader Compilation Error!", std::string(errors));
				delete errors;
			}
		}

		delete buffer;
		read = 0;
	}

	if (materialFile.HasMember("fragment"))
	{
		string vertpath = materialFile["fragment"].GetString();

		read = Utility::LoadTextFile(vertpath.c_str(), buffer);
		assert(read > 0);

		if (!Utility::TryCompileShader(GL_FRAGMENT_SHADER, buffer, fragShader, errors))
		{
			if (errors)
			{
				Services::Messages()->DisplayNativeDialog("Shader Compilation Error!", std::string(errors));
				delete errors;
			}
		}

		delete buffer;
		read = 0;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	materials.insert(make_pair(filepath, program));
	return program;
}

void MaterialManager::unloadResource(unsigned int resourceId)
{}

void MaterialManager::setActiveMaterial(unsigned int materialId)
{}

bool MaterialManager::materialExists(const string &filepath)
{
	return materials.count(filepath) > 0;
}

void MaterialManager::setProjectionMatrix(float* matrix)
{
	memcpy(&transformBlock[16], matrix, sizeof(float) * 16);
}

void MaterialManager::setModelViewMatrix(float* matrix)
{
	memcpy(transformBlock, matrix, sizeof(float) * 16);
}

void MaterialManager::updateUniforms()
{
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 32, transformBlock, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, TransformBlockIndex, uniformBuffer);
}

void MaterialManager::initialize()
{
	glGenBuffers(1, &uniformBuffer);
}