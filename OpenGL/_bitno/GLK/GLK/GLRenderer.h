#pragma once

#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

#define _USE_MATH_DEFINES
#include "math.h"

#include "CGLMaterial.h"
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

	// Axes.
	CONST GLfloat SCENE_SIZE, GRID_UNIT_SIZE;
	BOOL m_axesActive;
	void DrawAxes(GLfloat width, GLfloat lenght);

	// View manipulation.
	GLdouble m_eyeX, m_eyeY, m_eyeZ;
	GLdouble m_viewRadius, m_viewAlpha, m_viewBeta;
	CONST GLdouble MAX_RADIUS, MIN_RADIUS, MAX_ALPHA, MIN_ALPHA;

	inline double Deg2Rad(double degrees) { return degrees * M_PI / 180.; }
	void SetView();
	void ChangeView(GLdouble deltaRadius, GLdouble deltaAlpha, GLdouble deltaBeta);

	// Scene.
	void PrepareLight();
	void SetLight();

	CGLMaterial m_matDefault, m_matRedShiny;
	CGLTexture m_texGround, m_texTruck;

	void DrawGround();
	void DrawMaterial();
	void DrawTextured();
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
