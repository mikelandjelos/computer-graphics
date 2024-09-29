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

	// Drawing axes.
	const GLdouble SCENE_SIZE = 20.;
	BOOL m_axesActive = TRUE;
	void DrawAxes();

	// Drawing grid, xOz plane.
	BOOL m_gridActive = TRUE;
	const GLsizei gridUnitSize = SCENE_SIZE / 20.;
	void DrawGrid();

	// View movement.
	GLdouble m_viewRadius, m_viewAlpha, m_viewBeta;
	const GLdouble MAX_RADIUS = SCENE_SIZE * 2, MIN_RADIUS = SCENE_SIZE / 2., DELTA_RADIUS = 1.;
	const GLdouble MAX_ALPHA_ANGLE = 90., MIN_ALPHA_ANGLE = 0., DELTA_ANGLE = 5.;
	inline GLdouble Deg2Rad(GLdouble degrees) { return (GLdouble)degrees * M_PI / 180.; }

	void ChangeView(GLdouble deltaRadius, GLdouble deltaAlpha, GLdouble deltaBeta);
	void SetView();

	// Drawing geometric shapes.
	BOOL m_onlyOutlines = TRUE;
	void DrawCylinder(GLint nSides, GLfloat height, GLfloat topRadius, GLfloat bottomRadius);
	void DrawCone(GLint nSides, GLfloat height, GLfloat radius);
	void DrawSphere(GLint nSides, GLfloat radius);
protected:

	GLdouble m_eyeX, m_eyeY, m_eyeZ;
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
