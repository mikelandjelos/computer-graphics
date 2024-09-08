#pragma once
#include "GL/GL.H"

#define MAX_ZOOM 50
#define MIN_ZOOM 10

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
	BOOL m_axesActive = FALSE;

	int m_zoomDistance = MAX_ZOOM;
	const int m_deltaZoom = 5;

	double m_xRot = 0.;
	double m_yRot = 0.;
	double m_zRot = 0.;
	double m_XZrotAngle = 90.;
	double m_XYrotAngle = 0.;

	inline double Deg2Rad(double degrees);

	void DrawAxes(GLfloat length, GLfloat width);
	void RotateView(GLdouble dX, GLdouble dY);
	void Zoom(GLint d);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
};
