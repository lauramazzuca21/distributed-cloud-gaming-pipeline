#ifndef _SHADERMAKER_H_
#define _SHADERMAKER_H_
#include <GL/glew.h>

namespace shader_maker
{
		const GLuint createProgram(char* vertexfilename, char *fragmentfilename);
		const char* readShaderSource(const char* shaderFile);
};
#endif