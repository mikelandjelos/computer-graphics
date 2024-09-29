#pragma once

#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"

#define _USE_MATH_DEFINES
#include "math.h"

#include "CGLTexture.h"

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

	// View manipulation.
	CONST GLfloat SCENE_SIZE, GRID_UNIT_SIZE;
	CONST GLfloat MAX_RADIUS, MIN_RADIUS, MAX_ALPHA, MIN_ALPHA;
	GLfloat m_viewRadius, m_viewAlpha, m_viewBeta;
	GLfloat m_eyeX, m_eyeY, m_eyeZ;

	inline FLOAT Deg2Rad(FLOAT degrees) { return degrees * M_PI / 180.f; }
	void SetView();
	void ChangeView(FLOAT deltaRadius, FLOAT deltaAlpha, FLOAT deltaBeta);

	// Axes.
	BOOL m_axesActive;
	void DrawAxes(GLfloat width, GLfloat length);

	// Grid.
	BOOL m_gridActive;
	void DrawGrid(GLfloat size, GLsizei nSeg);

	// Lighting.
	void PrepareLighting();
	void SetLighting();

	// Textures.
	CONST GLfloat GRID_NUM_UNITS_TEX_TRUCK, GRID_UNIT_SIZE_TEX_TRUCK;
	CGLTexture m_texGround, m_texTruck;

	// Scene drawing.
	void DrawGround();
	void DrawTruckWheel();

	void DrawTruckBodyTexture();
	void DrawTruckBody();

	void DrawTruckHalf();
	void DrawTruck();

	void DrawCargo();
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
