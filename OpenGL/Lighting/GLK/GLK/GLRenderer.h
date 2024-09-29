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

	// Drawing grid(s).
	const GLdouble GRID_UNIT_SIZE = SCENE_SIZE / 20.;
	BOOL m_gridActive = TRUE;
	void DrawGrid(); // xOz plane.

	// View management (rotation/zoom).
	void ChangeView(GLdouble deltaRadius, GLdouble deltaAlpha, GLdouble deltaBeta);

	// Draw conical frustum.
protected:
	GLdouble m_viewRadius, m_viewAlpha, m_viewBeta;
	GLdouble m_eyeX, m_eyeY, m_eyeZ;
	const GLdouble MAX_RADIUS = SCENE_SIZE * 2., MIN_RADIUS = SCENE_SIZE / 2.;
	const GLdouble MAX_ALPHA = 90., MIN_ALPHA = 0.;
	inline GLdouble Deg2Rad(GLdouble degree) { return degree * M_PI / 180.; }
	void SetView();

	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
