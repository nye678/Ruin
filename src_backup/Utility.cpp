#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys\stat.h>

#include "Utility.h"

using namespace Ruin::Utility;

size_t Ruin::Utility::LoadTextFile(const char* fileName, char* &buffer)
{
	size_t read = 0;
	FILE* file = nullptr;
	errno_t error = fopen_s(&file, fileName, "r");

	if (error == 0)
	{
		int fd = _fileno(file);
		if (fd > -1)
		{
			struct _stat stats;
			if (_fstat(fd, &stats) != -1 && stats.st_size > 0)
			{
				buffer = new char[stats.st_size + 1];
				memset(buffer, '\0', stats.st_size + 1);

				read = fread(buffer, sizeof(char), stats.st_size, file);
				if (read == 0)
				{
					delete[] buffer;
				}
			}
		}

		fclose(file);
	}

	return read;
}

bool Ruin::Utility::TryCompileShader(GLenum type, const char* code, GLuint &handle, char* &errors)
{
	if (!code)
		return false;

	handle = glCreateShader(type);
	glShaderSource(handle, 1, &code, nullptr);
	glCompileShader(handle);

	GLint status;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);

		errors = new char[infoLogLength + 1];
		memset(errors, '\0', infoLogLength + 1);

		glGetShaderInfoLog(handle, infoLogLength, nullptr, errors);

		return false;
	}

	return true;
}

bool Ruin::Utility::CreateBasicShader(const char* vertexCode, const char* fragmentCode, GLuint &program, char* &errors)
{
	GLuint vertShader, fragShader;
	if (!TryCompileShader(GL_VERTEX_SHADER, vertexCode, vertShader, errors))
	{
		return false;
	}

	if (!TryCompileShader(GL_FRAGMENT_SHADER, fragmentCode, fragShader, errors))
	{
		return false;
	}

	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	return true;
}

GLint Ruin::Utility::GetShaderLocation(GLuint program, const char* name)
{
	GLint loc = -1;
	loc = glGetUniformLocation(program, name);

	if (loc == -1)
	{
		loc = glGetAttribLocation(program, name);
	}

	return loc;
}