#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>

namespace Practice 
{
	class Shader
	{
	private:
		const char* 	m_fragShaderPath;	
		const char* 	m_vertShaderPath;
		char*		m_vertShaderString;
		char*		m_fragShaderString;

		char*	ReadShaderSource(const char *path);
		void 	ReportCompileError(GLuint shader, const char *fname);
		void	ReportLinkError(GLuint program);

	public:
		Shader(const char *fragShaderPath, const char *vertShaderPath);
		~Shader();

		//returns a GLSL program object from vert and frag shader files
		GLuint InitShader();

		
	};
}

#endif
