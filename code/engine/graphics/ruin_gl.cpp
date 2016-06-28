#include "ruin_gl.h"

#include "ruin_assert.h"
#include "ruin_memory.h"

GLuint CompileBasicShader(const char* vertexCode, const char* fragmentCode)
{
    GLuint program, vertShader, fragShader;
    vertShader = CompileShader(GL_VERTEX_SHADER, vertexCode);
    fragShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    //glDeleteShader(vertShader);
    //glDeleteShader(fragShader);

    return program;
}

GLuint CompileShader(GLenum type, const char* code)
{
    GLuint handle = glCreateShader(type);
    glShaderSource(handle, 1, &code, nullptr);
    glCompileShader(handle);

    GLint status;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);

    //assert(status != GL_FALSE);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);

        //char* errors = new char[infoLogLength + 1];
        char* errors = (char*)Alloc(infoLogLength + 1);
        memset(errors, '\0', infoLogLength + 1);

        glGetShaderInfoLog(handle, infoLogLength, nullptr, errors);

        MessageBoxA(NULL, errors, "Error Compiling Shader", MB_OK);
        Release(errors);
    }

    return handle;
}