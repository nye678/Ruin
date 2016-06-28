#ifndef __RUIN_GL_
#define __RUIN_GL_

#include <GL/gl3w.h>

GLuint CompileBasicShader(const char* vectexCode, const char* fragmentCode);
GLuint CompileShader(GLenum type, const char* code);

#endif