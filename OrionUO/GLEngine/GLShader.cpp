// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GLShader.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CDeathShader g_DeathShader;
CColorizerShader g_ColorizerShader;
CColorizerShader g_FontColorizerShader;
CColorizerShader g_LightColorizerShader;
//----------------------------------------------------------------------------------
void UnuseShader()
{
	WISPFUN_DEBUG("c_uns_sdr");
	glUseProgram(0);
	ShaderColorTable = 0;
	g_ShaderDrawMode = 0;
}
//----------------------------------------------------------------------------------
//-----------------------------------CGLShader--------------------------------------
//----------------------------------------------------------------------------------
CGLShader::CGLShader()
{
	WISPFUN_DEBUG("c32_f1");
}
//----------------------------------------------------------------------------------
bool CGLShader::Init(const char *vertexShaderData, const char *fragmentShaderData)
{
	GLint val = GL_FALSE;

	if (vertexShaderData != NULL && fragmentShaderData != NULL)
	{
		m_Shader = glCreateProgram();

		m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_VertexShader, 1, (const GLchar**)&vertexShaderData, NULL);
		glCompileShader(m_VertexShader);


		glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &val);
		if (val != GL_TRUE)
		{
			LOG("CGLShader::Init vertex shader compilation error:\n");
			auto error = glGetError();
			if (error != 0)
			{
				auto errorStr = gluErrorString(error);
				LOG("CGLShader::Init vertex shader error Code: %i\n", error);
				LOG("CGLShader::Init vertex shader error string: %s\n", errorStr);
			}
			return false;
		}
		LOG("vertex shader compiled successfully\n");

		glAttachShader(m_Shader, m_VertexShader);


		m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_FragmentShader, 1, (const GLchar**)&fragmentShaderData, NULL);
		glCompileShader(m_FragmentShader);

		glGetShaderiv(m_FragmentShader, GL_COMPILE_STATUS, &val);
		if (val != GL_TRUE)
		{
			LOG("CGLShader::Init fragment shader compilation error:\n");
			auto error = glGetError();
			if (error != 0)
			{
				auto errorStr = gluErrorString(error);
				LOG("CGLShader::Init fragment shader error Code: %i\n", error);
				LOG("CGLShader::Init fragment shader error string: %s\n", errorStr);
			}
			return false;
		}
		LOG("fragment shader compiled successfully\n");

		glAttachShader(m_Shader, m_FragmentShader);

		glLinkProgram(m_Shader);
		glValidateProgram(m_Shader);
	}
	else
		return false;


	glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &val);
	if (val != GL_TRUE)
	{
		LOG("CGLShader::Init shader program compilation error:\n");
		auto error = glGetError();
		if (error != 0)
		{
			auto errorStr = gluErrorString(error);
			LOG("CGLShader::Init shader program error Code: %i\n", error);
			LOG("CGLShader::Init shader program error string: %s\n", errorStr);
		}
		return false;
	}

	GLint isLinked = 0;
	glGetProgramiv(m_Shader, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_Shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_Shader, maxLength, &maxLength, &infoLog[0]);

		// The program is useless now. So delete it.
		glDeleteProgram(m_Shader);

		LOG("shader program failed to link\n");
		LOG("%s\n", infoLog);
		std::wstring str(infoLog.begin(), infoLog.end());
		LOG("%s\n", str);
		return false;
	}

	LOG("shader program compiled successfully\n");
	return val == GL_TRUE;
}
//----------------------------------------------------------------------------------
CGLShader::~CGLShader()
{
	WISPFUN_DEBUG("c32_f2");
	if (m_Shader != 0)
	{
		glDeleteProgram(m_Shader);
		m_Shader = 0;
	}

	if (m_VertexShader != 0)
	{
		glDeleteProgram(m_VertexShader);
		m_VertexShader = 0;
	}

	if (m_FragmentShader != 0)
	{
		glDeleteProgram(m_FragmentShader);
		m_FragmentShader = 0;
	}

	m_TexturePointer = 0;
}
//----------------------------------------------------------------------------------
bool CGLShader::Use()
{
	WISPFUN_DEBUG("c32_f3");
	UnuseShader();

	bool result = false;

	if (m_Shader != 0)
	{
		glUseProgram(m_Shader);
		result = true;			
	}

	return result;
}
//----------------------------------------------------------------------------------
void CGLShader::Pause()
{
	WISPFUN_DEBUG("c32_f4");
	glUseProgram(0);
}
//----------------------------------------------------------------------------------
void CGLShader::Resume()
{
	WISPFUN_DEBUG("c32_f5");
	glUseProgram(m_Shader);
}
//----------------------------------------------------------------------------------
//-----------------------------------CDeathShader-----------------------------------
//----------------------------------------------------------------------------------
CDeathShader::CDeathShader()
: CGLShader()
{
	WISPFUN_DEBUG("c33_f1");
}
//----------------------------------------------------------------------------------
bool CDeathShader::Init(const char *vertexShaderData, const char *fragmentShaderData)
{
	if (CGLShader::Init(vertexShaderData, fragmentShaderData))
		m_TexturePointer = glGetUniformLocation(m_Shader, "usedTexture");
	else
		LOG("Failed to create DeathShader\n");

	return (m_Shader != 0);
}
//----------------------------------------------------------------------------------
//----------------------------------CColorizerShader--------------------------------
//----------------------------------------------------------------------------------
CColorizerShader::CColorizerShader()
: CGLShader()
{
	WISPFUN_DEBUG("c34_f1");
}
//----------------------------------------------------------------------------------
bool CColorizerShader::Init(const char *vertexShaderData, const char *fragmentShaderData)
{
	if (CGLShader::Init(vertexShaderData, fragmentShaderData))
	{
		m_TexturePointer = glGetUniformLocation(m_Shader, "usedTexture");
		m_ColorTablePointer = glGetUniformLocation(m_Shader, "colors");
		m_DrawModePointer = glGetUniformLocation(m_Shader, "drawMode");
	}
	else
		LOG("Failed to create ColorizerShader\n");

	return (m_Shader != 0);
}
//----------------------------------------------------------------------------------
bool CColorizerShader::Use()
{
	WISPFUN_DEBUG("c34_f2");
	bool result = CGLShader::Use();

	if (result)
	{
		ShaderColorTable = m_ColorTablePointer;
		g_ShaderDrawMode = m_DrawModePointer;
		glUniform1i(g_ShaderDrawMode, SDM_NO_COLOR);
	}

	return result;
}
//----------------------------------------------------------------------------------
