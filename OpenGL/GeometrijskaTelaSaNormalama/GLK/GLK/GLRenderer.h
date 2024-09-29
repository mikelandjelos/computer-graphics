#pragma once

#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"

#define _USE_MATH_DEFINES
#include "math.h"

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);

	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	// Axes.
	CONST GLdouble SCENE_SIZE;
	BOOL m_axesActive = TRUE;
	void DrawAxes(GLdouble length);

	// Grid.
	BOOL m_gridActive = TRUE;
	CONST GLdouble GRID_UNIT_SIZE;
	void DrawGrid(GLdouble width, GLdouble height, GLint wNumSeg, GLint hNumSeg);

	// View manipulation (rotation and zoom).
	GLdouble m_eyeX, m_eyeY, m_eyeZ;
	GLdouble m_viewRadius, m_viewAlpha, m_viewBeta;
	CONST GLdouble MAX_RADIUS, MIN_RADIUS;
	CONST GLdouble MAX_ALPHA, MIN_ALPHA;
	void ChangeView(GLdouble deltaRadius, GLdouble deltaAlpha, GLdouble deltaBeta);
	void SetView();
	inline double Deg2Rad(double degrees) { return degrees * M_PI / 180.; }

	// Only outlines.
	BOOL m_onlyOutlines = FALSE;

	// Normals.
	BOOL m_showNormals = FALSE;

	// Implementation.
	void DrawConicalFrustum(GLfloat height, GLfloat bottomRadius, GLfloat topRadius, GLsizei nSeg);
	void DrawCone(GLfloat height, GLfloat baseRadius, GLsizei nSeg);
	void DrawSphere(GLfloat radius, GLsizei nSegVertical, GLsizei nSegHorizontal);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
