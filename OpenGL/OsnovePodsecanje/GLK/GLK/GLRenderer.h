#pragma once

#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"

#define _USE_MATH_DEFINES
#include "math.h"

#include "CGLMaterial.h"

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

	// View manipulation (rotation and zoom).
	CONST GLdouble SCENE_SIZE;
	CONST GLdouble MAX_RADIUS, MIN_RADIUS;
	CONST GLdouble MAX_ALPHA, MIN_ALPHA;
	GLdouble m_eyeX, m_eyeY, m_eyeZ;
	GLdouble m_viewRadius, m_viewAlpha, m_viewBeta;
	void ChangeView(GLdouble deltaRadius, GLdouble deltaAlpha, GLdouble deltaBeta);
	void SetView();
	inline double Deg2Rad(double degrees) { return degrees * M_PI / 180.; }

	// Axes.
	BOOL m_axesActive;
	void DrawAxes(GLdouble width, GLdouble length);

	// Grid.
	BOOL m_gridActive;
	void DrawGrid(GLdouble sceneSize, GLdouble gridUnit);

	// 3D geometric shapes.
	void DrawCylinder(GLdouble height, GLdouble bottomRadius, GLdouble topRadius, GLsizei nSeg);

	// Outlines.
	BOOL m_onlyOutlines;

	// Normals.
	BOOL m_normalsActive;

	// Lighting.
	CGLMaterial m_redShinyMaterial, m_grayMattedMaterial;
	void PrepareLighting();

	// Walls.
	void DrawWalls(GLdouble size);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
