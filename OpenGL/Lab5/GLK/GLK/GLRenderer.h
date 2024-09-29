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

	// Scene manipulation.
	CONST GLdouble SCENE_SIZE;
	GLdouble m_viewRadius, m_viewAlpha, m_viewBeta;
	CONST GLdouble MIN_RADIUS, MAX_RADIUS, MIN_ALPHA, MAX_ALPHA;
	GLdouble m_eyeX, m_eyeY, m_eyeZ;

	inline double Deg2Rad(double degrees) { return M_PI * degrees / 180.; }

	void SetView();
	void ChangeView(GLdouble deltaRadius, GLdouble deltaAlpha, GLdouble deltaBeta);

	// Axes.
	BOOL m_axesActive;
	void DrawAxes(GLdouble width, GLdouble lenght);

	// Grid.
	BOOL m_gridActive;
	CONST GLdouble GRID_UNIT_SIZE;
	void DrawGrid(GLdouble size, GLsizei nUnits);

	// Polygon mode.
	BOOL m_fill;

	// Geometric shapes.
	BOOL m_normalsActive;
	void DrawTruncatedCone(GLdouble height, GLdouble bottomRadius, GLdouble topRadius, GLsizei nSeg);
	void DrawHalfSphere(GLdouble radius, GLsizei nSeg);

	// Materials.
	CGLMaterial m_materialPink, m_materialCyan, m_materialLightGrey;

	// Room.
	CONST GLdouble BOX_SIZE;
	void DrawBox();
	void DrawStand();

	// Vase.
	enum class VasePartWidth { SMALL = 2, MEDIUM = 3, BIG = 4 };
	CONST GLdouble PART_HEIGHT;
	void DrawVasePart(VasePartWidth bottom, VasePartWidth top);
	void DrawVase();

	// Lighting.
	void PrepareLighting();
	void PositionLighting();
	BOOL m_redLightActive, m_greenLightActive, m_blueLightActive;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
