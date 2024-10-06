#pragma once

#include "GL/GL.h"
#include "GL/GLU.h"

#include "DImage.h"

class CGLTexture
{
private:
	UINT m_Id;
public:
	CGLTexture();
	~CGLTexture();

	void LoadFromFile(CString fileName, GLenum wrap = GL_REPEAT, GLenum mag = GL_LINEAR, GLenum min = GL_LINEAR_MIPMAP_LINEAR);
	void Select();
	void Release();

	static void PrepareTexturing(BOOL enableLight);
	static void DeselectTexture();
};