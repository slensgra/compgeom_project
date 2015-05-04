#include "Shader.h"
#include <iostream>
#include <stdio.h>
#include <GL/glew.h>

namespace Practice
{
	char* Shader::ReadShaderSource(const char *path)
	{
		FILE* fp = fopen(path, "r");

		if(fp == NULL)
		{
			std::cout << "Shader::ReadShaderSource Error in reading file: " << path << std::endl;
			return NULL; 
		} 

		fseek(fp, 0L, SEEK_END);
		long size = ftell(fp);

		fseek(fp, 0L, SEEK_SET);

		char* buf = new char[size + 1];

		fread(buf, 1, size, fp);

		buf[size] = ' ';
		fclose(fp);

		return buf;
	}
	
	Shader::Shader(const char *fragShaderPath, const char *vertShaderPath)
	{
	
		m_fragShaderPath = fragShaderPath;
		m_vertShaderPath = vertShaderPath;

		m_vertShaderString = ReadShaderSource(m_vertShaderPath);
		m_fragShaderString = ReadShaderSource(m_fragShaderPath);	
	}

	void Shader::ReportCompileError(GLuint shader, const char* fname)
	{
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if(!compiled)
		{
			std::cerr << fname << " has failed to compile." << std::endl;
			GLint logSize;
			glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
			char * logMsg = new char[logSize];
			glGetShaderInfoLog( shader, logSize, NULL, logMsg );
			std::cerr << logMsg <<std::endl;
			delete [] logMsg;
		}
	}

	void Shader::ReportLinkError(GLuint program)
	{
		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		
		if(!linked)
		{
			std::cerr << "Shader program failed to link" << std::endl;
		
			GLint logSize;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
			char *logMsg = new char[logSize];
			
			glGetProgramInfoLog(program, logSize, NULL, logMsg );
			std::cerr << logMsg << std::endl;

			delete [] logMsg;
		}
	}

	GLuint Shader::InitShader()
	{
		GLuint program = glCreateProgram();
		GLuint sf, sv;

		sf = glCreateShader( GL_FRAGMENT_SHADER );
		sv = glCreateShader( GL_VERTEX_SHADER );

		glShaderSource(sv, 1, (const GLchar**) &m_vertShaderString, NULL);
		glCompileShader( sv );

		ReportCompileError(sv, m_vertShaderPath );
		glAttachShader(program, sv);

		glShaderSource( sf, 1, (const GLchar**) &m_fragShaderString, NULL);
		glCompileShader( sf );

		ReportCompileError(sf, m_fragShaderPath);
		glAttachShader(program, sf);

		//link em up bby!!
	
		glBindAttribLocation (program, 0, "vPosition");

		glLinkProgram(program);
		ReportLinkError(program);

		return program;	
	}
}
