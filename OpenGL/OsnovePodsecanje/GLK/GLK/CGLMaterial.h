#pragma once

#include "GL/GL.h"
#include "GL/GLU.h"

class CGLMaterial
{
private:
	GLfloat m_vAmbient[4];
	GLfloat m_vDiffuse[4];
	GLfloat m_vSpecular[4];
	GLfloat m_vEmission[4];
	GLfloat m_fShininess;
public:
	CGLMaterial();
	CGLMaterial(GLfloat ambient[4], GLfloat diffuse[4], GLfloat specular[4], GLfloat emission[4], GLfloat shininess) {
		SetArray(m_vAmbient, ambient);
		SetArray(m_vDiffuse, diffuse);
		SetArray(m_vSpecular, specular);
		SetArray(m_vEmission, emission);
		m_fShininess = shininess;
	}

	void Select(GLenum face = GL_FRONT);
	void SetAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void SetDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void SetSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void SetEmission(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void SetShininess(GLfloat s);
private:
	void SetArray(GLfloat dest[4], const GLfloat src[4]);
};