#include "stdafx.h"
#include "CGLTexture.h"

CGLTexture::CGLTexture() : m_Id{ 0 }
{
}

CGLTexture::~CGLTexture()
{
	Release();
}

void CGLTexture::LoadFromFile(CString fileName, GLenum wrap)
{
	// Releasing previously stored texture.
	if (m_Id != 0)
		Release();

	// Loading new texture bitmap.
	DImage img;
	img.Load(fileName);

	// Generating texture resource ID and selecting texture.
	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_2D, m_Id);

	// Setting up texture parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	// Building texture mipmaps.
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());

	// Deselecting texture.
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
	DeselectTexture();
}

void CGLTexture::PrepareTexturing(BOOL bEnableLighting)
{
	// 4 => RGBA channels.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	bEnableLighting
		? glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE)
		: glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void CGLTexture::DeselectTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
