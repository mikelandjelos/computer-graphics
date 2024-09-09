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

	// Implementacija

	const GLdouble MAX_RADIUS = 20., MIN_RADIUS = 3.;
	const GLdouble MAX_ALPHA = Deg2Rad(90.), MIN_ALPHA = Deg2Rad(0.);
	const GLdouble ANGLE_STEP = Deg2Rad(5.);
	const GLdouble DISTANCE_STEP = .5;
	inline GLdouble Deg2Rad(GLdouble degrees) { return degrees * M_PI / 180.; }
	void ChangeView(GLdouble deltaRadius, GLdouble deltaAlpha, GLdouble deltaBeta);
	void SetView();

	const GLdouble SCENE_SIZE = 10.;
	BOOL m_axesActive = TRUE;
	void DrawAxes(GLfloat width, GLfloat length);

	BOOL m_oGridXY = FALSE;
	BOOL m_oGridXZ = FALSE;
	BOOL m_oGridYZ = FALSE;
	GLdouble gridUnitSize = SCENE_SIZE / 10.;
	void DrawGrid(GLdouble unitSize, GLboolean oXY, GLboolean oXZ, GLboolean oYZ);

	GLdouble cubeVertices[24], cubeColor[24];
	GLubyte cubeIndices[24]; // 6 sides, every has 4 vertices = 6x4
	void DrawCube(GLdouble size, GLclampf r, GLclampf g, GLclampf b);
protected:
	void PrepareCube(GLdouble size);
	GLdouble m_viewRadius, m_viewAlpha, m_viewBeta; // Polar view coordinates
	GLdouble m_eyeX, m_eyeY, m_eyeZ; // Cartesian view coordinates

	HGLRC	 m_hrc; //OpenGL Rendering Context 
};
