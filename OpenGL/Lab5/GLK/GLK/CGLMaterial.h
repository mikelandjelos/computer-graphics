#pragma once

#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"

class CGLMaterial
{
private:
	GLfloat m_fvAmbient[4];
	GLfloat m_fvDiffuse[4];
	GLfloat m_fvSpecular[4];
	GLfloat m_fvEmission[4];
	GLfloat m_fShininess;
public:
	CGLMaterial();

	void Select(GLenum face = GL_FRONT);
	void SetAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void SetDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void SetSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void SetEmission(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void SetShininess(GLfloat s);
};