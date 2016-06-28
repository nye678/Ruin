#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>

#include "ruin_config.h"
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "rapidjson/document.h"

#include "memory.h"
#include "utility.h"

GLFWwindow* window = nullptr;
int windowHeight = 1024;
int windowWidth = 768;
float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
size_t numMats = 0;
GLuint* mats = nullptr;

void renderDemo1();
void renderDemo2();
void renderDemo3(double currentTime);
void renderDemo4(GLuint program, double currentTime);
void renderDemo5(GLuint program, double currentTime);

int main(int argc, char* argv[])
{
    fprintf(stdout, "Ruin Engine - Version %d.%d\n", Ruin_VERSION_MAJOR, Ruin_VERSION_MINOR);

	char* settings = nullptr;
	loadTextFile("D:\\Projects\\Ruin\\data\\settings.json", settings);
	rapidjson::Document d;
	d.Parse(settings);
    delete [] settings;

	assert(d.IsObject());

    //const rapidjson::Value& displaySettings = d["display"];
    rapidjson::Value::MemberIterator displaySettings = d.FindMember("display");
    assert(displaySettings != d.MemberEnd());
    assert(displaySettings->value.IsObject());

    windowHeight = displaySettings->value["height"].GetInt();
    windowWidth = displaySettings->value["width"].GetInt();

	auto colorSetting = displaySettings->value.FindMember("color");
	assert(colorSetting != displaySettings->value.MemberEnd());
	assert(colorSetting->value.IsArray());
	assert(colorSetting->value.Size() == 4);
	for (size_t i = 0; i < 4; ++i)
	{
		clearColor[i] = (float)colorSetting->value[i].GetDouble();
	}

    if (!glfwInit()) return 1;

    window = glfwCreateWindow(windowWidth, windowHeight, "Ruin", nullptr, nullptr);
    if (!window) return 1;

    fprintf(stdout, "Initializing Graphics\n");

    glfwMakeContextCurrent(window);
    if (gl3wInit())
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    int major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    fprintf(stdout, "OpenGL - Version: %d.%d", major, minor);

    rapidjson::Value::MemberIterator materials = d.FindMember("materials");
    assert(materials != d.MemberEnd());
    assert(materials->value.IsArray());

	numMats = materials->value.Size();
    mats = new GLuint[numMats];

    const char* shaderPath = "D:\\Projects\\Ruin\\data\\shaders\\";
    char filepath[1024] = {};


    for (size_t i = 0; i < materials->value.Size(); ++i)
    {
        const rapidjson::Value& m = materials->value[i];
        assert(m.IsObject());

        auto compileShader = [&m, &filepath, &shaderPath](const char* name, GLenum shadertype) -> GLuint
        {
			GLuint shader = 0;
			auto s = m.FindMember(name);
			if (s != m.MemberEnd())
			{
				memset(filepath, 0x0, 1024);
				strncat(filepath, shaderPath, 1024);
				strncat(filepath, s->value.GetString(), 1024);

				char* shaderCode = nullptr;
				if (loadTextFile(filepath, shaderCode))
				{
					shader = glCreateShader(shadertype);
					glShaderSource(shader, 1, &shaderCode, nullptr);
					glCompileShader(shader);

                    GLint status;
                    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
                    if (status == GL_FALSE)
                    {
                        GLint infoLogLength = 0;
                        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

                        char* errors = new char[infoLogLength + 1];
                        memset(errors, 0x0, infoLogLength + 1);

                        glGetShaderInfoLog(shader, infoLogLength, nullptr, errors);
                        fprintf(stdout, "Shader Compile Error\n%s\n", errors);

                        delete [] errors;
                    }

                    delete [] shaderCode;
				}
			}

            return shader;
        };

		GLuint vertexShader = compileShader("vert", GL_VERTEX_SHADER);
        GLuint tcsShader = compileShader("tcs", GL_TESS_CONTROL_SHADER);
        GLuint tesShader = compileShader("tes", GL_TESS_EVALUATION_SHADER);
		GLuint fragmentShader = compileShader("frag", GL_FRAGMENT_SHADER);

		if (vertexShader && fragmentShader)
		{
            GLuint program = glCreateProgram();
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            
            if (tcsShader && tesShader)
            {
                glAttachShader(program, tcsShader);
                glAttachShader(program, tesShader);
            }
			
            glLinkProgram(program);
			mats[i] = program;
		}

		if (vertexShader) glDeleteShader(vertexShader);
		if (fragmentShader) glDeleteShader(fragmentShader);
        if (tcsShader) glDeleteShader(tcsShader);
        if (tesShader) glDeleteShader(tesShader);
    }

	GLuint vertexArray = 0;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	
	glClearColor(1.0f, 0.5f, 0.5f, 0.5f);

    while (!glfwWindowShouldClose(window))
    {
        glClearBufferfv(GL_COLOR, 0, clearColor);
		double time = glfwGetTime();

		//renderDemo1();
		//renderDemo2();
		//renderDemo3(time);
		//renderDemo4(mats[3], time);
		//renderDemo4(mats[4], time);
        renderDemo5(mats[5], time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

	glDeleteVertexArrays(1, &vertexArray);
	for (size_t i = 0; i < numMats; ++i)
	{
		if (mats[i] > 0) glDeleteProgram(mats[i]);
	}

    glfwDestroyWindow(window);
    glfwTerminate();

    rMemoryManager::Destroy();

    return 0;
}

void renderDemo1()
{
	glUseProgram(mats[0]);
	glPointSize(40.0f);
	glDrawArrays(GL_POINTS, 0, 1);
	glPointSize(1.0f);
}

void renderDemo2()
{
	glUseProgram(mats[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void renderDemo3(double currentTime)
{
	glUseProgram(mats[2]);

	float attrib[] = {
		(float)sin(currentTime) * 0.5f,
		(float)cos(currentTime) * 0.6f,
		0.0f, 0.0f };
	glVertexAttrib4fv(0, attrib);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void renderDemo4(GLuint program, double currentTime)
{
	glUseProgram(program);

	float attrib[] = {
		(float)sin(currentTime) * 0.5f,
		(float)cos(currentTime) * 0.6f,
		0.0f, 0.0f };
	glVertexAttrib4fv(0, attrib);

	float color[] = {
		(float)sin(currentTime) * 0.5f + 0.5f,
		(float)cos(currentTime) * 0.5f + 0.5f,
		0.0f, 1.0f
	};
	glVertexAttrib4fv(1, color);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void renderDemo5(GLuint program, double currentTime)
{
    glUseProgram(program);

    float attrib[] = {
        (float)sin(currentTime) * 0.5f,
        (float)cos(currentTime) * 0.6f,
        0.0f, 0.0f };
    glVertexAttrib4fv(0, attrib);

    float color[] = {
        (float)sin(currentTime) * 0.5f + 0.5f,
        (float)cos(currentTime) * 0.5f + 0.5f,
        0.0f, 1.0f
    };
    glVertexAttrib4fv(1, color);

    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_PATCHES, 0, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}