#include "stdafx.h"
#include "CGLMaterial.h"

CGLMaterial::CGLMaterial()
	: m_fvAmbient{ .2f, .2f, .2f, 1.f },
	m_fvDiffuse{ .8f, .8f, .8f, 1.f },
	m_fvSpecular{ 0.f, 0.f, 0.f, 1.f },
	m_fvEmission{ 0.f, 0.f, 0.f, 1.f },
	m_fShininess{ 64.f }
{
}

void CGLMaterial::Select(GLenum face)
{
	glMaterialfv(face, GL_AMBIENT, m_fvAmbient);
	glMaterialfv(face, GL_DIFFUSE, m_fvDiffuse);
	glMaterialfv(face, GL_SPECULAR, m_fvSpecular);
	glMaterialfv(face, GL_EMISSION, m_fvEmission);
	glMaterialf(face, GL_SHININESS, m_fShininess);
}

void CGLMaterial::SetAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_fvAmbient[0] = r;
	m_fvAmbient[1] = g;
	m_fvAmbient[2] = b;
	m_fvAmbient[3] = a;
}

void CGLMaterial::SetDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_fvDiffuse[0] = r;
	m_fvDiffuse[1] = g;
	m_fvDiffuse[2] = b;
	m_fvDiffuse[3] = a;
}

void CGLMaterial::SetSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_fvSpecular[0] = r;
	m_fvSpecular[1] = g;
	m_fvSpecular[2] = b;
	m_fvSpecular[3] = a;
}

void CGLMaterial::SetEmission(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_fvEmission[0] = r;
	m_fvEmission[1] = g;
	m_fvEmission[2] = b;
	m_fvEmission[3] = a;
}

void CGLMaterial::SetShininess(GLfloat s)
{
	m_fShininess = s;
}
