// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GLFrameBuffer.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGLFrameBuffer::CGLFrameBuffer()
{
	WISPFUN_DEBUG("c30_f1");
}
//----------------------------------------------------------------------------------
CGLFrameBuffer::~CGLFrameBuffer()
{
	WISPFUN_DEBUG("c30_f2");
	Free();
}
//----------------------------------------------------------------------------------
/*!
Инициализациия буфера
@param [__in] width Ширина буфера
@param [__in] height Высота буфера
@return true в случае успеха
*/
bool CGLFrameBuffer::Init(const int &width, const int &height)
{
	WISPFUN_DEBUG("c30_f3");

	return true;
}
//----------------------------------------------------------------------------------
/*!
Очистка фрэймбуфера
@return
*/
void CGLFrameBuffer::Free()
{
	WISPFUN_DEBUG("c30_f4");
	Texture.Clear();

	if (g_GL.CanUseFrameBuffer && m_FrameBuffer != 0)
	{
		m_FrameBuffer = 0;
	}

	m_OldFrameBuffer = 0;
}
//----------------------------------------------------------------------------------
/*!
Завершение использования фрэймбуфера
@return 
*/
void CGLFrameBuffer::Release()
{
	WISPFUN_DEBUG("c30_f5");
	if (g_GL.CanUseFrameBuffer)
	{
	}
}
//----------------------------------------------------------------------------------
/*!
Проверка готовности буфера с потенциальным пересозданием
@param [__in] width Ширина буфера
@param [__in] height Высота буфера
@return true в случае готовности
*/
bool CGLFrameBuffer::Ready(const int &width, const int &height)
{
	WISPFUN_DEBUG("c30_f6");
	return (g_GL.CanUseFrameBuffer && m_Ready && Texture.Width == width && Texture.Height == height);
}
//----------------------------------------------------------------------------------
bool CGLFrameBuffer::ReadyMinSize(const int &width, const int &height)
{
	WISPFUN_DEBUG("c30_f6");
	return (g_GL.CanUseFrameBuffer && m_Ready && Texture.Width >= width && Texture.Height >= height);
}
//----------------------------------------------------------------------------------
/*!
Использование буфера
@return true в случае успеха
*/
bool CGLFrameBuffer::Use()
{
	WISPFUN_DEBUG("c30_f7");
	bool result = false;

	return result;
}
//----------------------------------------------------------------------------------
/*!
Отрисовать текстуру буфера
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@return 
*/
void CGLFrameBuffer::Draw(const int &x, const int &y)
{
	WISPFUN_DEBUG("c30_f8");
	if (g_GL.CanUseFrameBuffer && m_Ready)
	{
		g_GL.OldTexture = 0;
		g_GL.GL1_Draw(Texture, x, y);
	}
}
//----------------------------------------------------------------------------------

