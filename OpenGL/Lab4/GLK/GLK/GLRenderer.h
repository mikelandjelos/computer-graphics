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

	// Implementation.
	GLdouble m_rotatingPartAngle = 0.;
	void DrawFigure(double angle);

	GLdouble ELEMENTARY_WIDTH;
	GLdouble ELEMENTARY_HEIGHT;

	void DrawBase();
	void DrawCactus(double angle);
	void DrawCactusCylinder(BOOL isYellow = FALSE);
	void DrawCactusCone();
	void DrawCactusSphere();

	void DrawCylinder(double h, double r1, double r2, int nSeg);
	void DrawCone(double h, double r, int nSeg);
	void DrawSphere(double r, int nSegAlpha, int nSegBeta);

	BOOL m_axesActive = TRUE, m_gridActive = TRUE, m_onlyOutlines = FALSE;

	void DrawAxes(double length);
	void DrawGrid(double width, double height, int nSegW, int nSegH); // xOz.
	void ChangeView(double deltaRadius, double deltaAlpha, double deltaBeta);
	inline double Deg2Rad(double degrees) { return degrees * M_PI / 180.; }
protected:
	// Axes and grid.
	CONST GLdouble SCENE_SIZE = 20., GRID_UNIT_SIZE = SCENE_SIZE / 10.;
	CONST GLdouble MAX_RADIUS = SCENE_SIZE * 4., MIN_RADIUS = SCENE_SIZE / 2.;
	CONST GLdouble MAX_ALPHA = 90., MIN_ALPHA = 0.;

	void SetView();
	GLdouble m_eyeX, m_eyeY, m_eyeZ;
	GLdouble m_viewRadius, m_viewAlpha, m_viewBeta;

	HGLRC	 m_hrc; //OpenGL Rendering Context 
};
