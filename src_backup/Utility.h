#ifndef __RUIN_UTILITY_H_
#define __RUIN_UTILITY_H_

#include <GL3\gl3w.h>

namespace Ruin { 
namespace Utility {
	/*Loads the contents of a text file into a char buffer. If the is an error
	  reading the file a zero is returned and the buffer is invalid.
	  param	const char*		The file name of the file to load.
	  out		char*			A pointer for the character buffer.
	  return	size_t			Number of characters read into the buffer.*/
	size_t LoadTextFile(const char* fileName, char* &buffer);
	
	/*Compiles shader code. If the shader fails to compile, error messages will be written
	  to the errors parameter. Make sure to delete these afterwards.
	  param		GLenum			The type of shader to compile.
	  param		const char*		The shader source code.
	  out		GLuint			The OpenGL handle to the compiled shader.
	  out		char*&			Error messages from failed compilations.
	  return	bool			Returns true if the shader compiled correctly.*/
	bool TryCompileShader(GLenum type, const char* code, GLuint &handle, char* &errors);
	
	/*Compiles and links vertex shader code and fragment shader code to create a basic shader program.
	  param		const char*		The vertex shader code.
	  param		const char*		The fragment shader code.
	  out		GLuint			The program handle of the compiled shader, if successful.
	  out		char*&			Error messages from failed compilations.
	  return	bool			Returns true if the shader was created successfully.*/
	bool CreateBasicShader(const char* vertexCode, const char* fragmentCode, GLuint &program, char* &errors);
	
	/*Gets a uniform for attribute location from the shader.
	  param		GLuint			The shader program to find the uniform or attribute in.
	  param		const char*		The name of the uniform or attribute.
	  return	GLint			The location of the uniform or attribute, or -1 if it doesn't exist.*/
	GLint GetShaderLocation(GLuint program, const char* name);
}}

#endif