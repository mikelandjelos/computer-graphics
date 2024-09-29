#include "stdafx.h"
#include "CGLMaterial.h"

CGLMaterial::CGLMaterial() :
	m_vAmbient{ 0.2f, 0.2f , 0.2f , 1.f },
	m_vDiffuse{ 0.8f, 0.8f, 0.8f, 1.f },
	m_vSpecular{ 1.f, 1.f, 1.f, 1.f },
	m_vEmission{ 0.f, 0.f, 0.f, 1.f },
	m_fShininess{ 64.f }
{
}

void CGLMaterial::Select(GLenum face)
{
	glMaterialfv(face, GL_AMBIENT, m_vAmbient);
	glMaterialfv(face, GL_DIFFUSE, m_vDiffuse);
	glMaterialfv(face, GL_SPECULAR, m_vSpecular);
	glMaterialfv(face, GL_EMISSION, m_vEmission);
	glMaterialf(face, GL_SHININESS, m_fShininess);
}

void CGLMaterial::SetAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_vAmbient[0] = r;
	m_vAmbient[1] = g;
	m_vAmbient[2] = b;
	m_vAmbient[3] = a;
}

void CGLMaterial::SetDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_vDiffuse[0] = r;
	m_vDiffuse[1] = g;
	m_vDiffuse[2] = b;
	m_vDiffuse[3] = a;
}

void CGLMaterial::SetSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_vSpecular[0] = r;
	m_vSpecular[1] = g;
	m_vSpecular[2] = b;
	m_vSpecular[3] = a;
}

void CGLMaterial::SetEmission(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_vEmission[0] = r;
	m_vEmission[1] = g;
	m_vEmission[2] = b;
	m_vEmission[3] = a;
}

void CGLMaterial::SetShininess(GLfloat s)
{
	m_fShininess = s < 0.f
		? 0.f
		: s > 256.f ? 256.f : s;
}

void CGLMaterial::SetArray(GLfloat dest[4], const GLfloat src[4])
{
	for (int i = 0; i < 4; ++i) {
		dest[i] = src[i];
	}
}
