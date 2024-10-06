#include "stdafx.h"
#include "CGLTexture.h"

CGLTexture::CGLTexture()
	: m_Id{ 0 }
{
}

CGLTexture::~CGLTexture()
{
	Release();
}

void CGLTexture::LoadFromFile(CString fileName, GLenum wrap, GLenum mag, GLenum min)
{
	if (m_Id != 0)
		Release();

	DImage img;
	img.Load(fileName);

	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_2D, m_Id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());

	DeselectTexture();
}

void CGLTexture::Select()
{
	glBindTexture(GL_TEXTURE_2D, m_Id);
}

void CGLTexture::Release()
{
	glDeleteTextures(1, &m_Id);
	m_Id = 0;
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLTexture::PrepareTexturing(BOOL enableLight)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	enableLight
		? glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE)
		: glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void CGLTexture::DeselectTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
