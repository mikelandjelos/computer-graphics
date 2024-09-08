#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	RotateView(0., 0.);
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glEnable(GL_DEPTH_TEST);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(m_xRot, m_yRot, m_zRot, 0., 0., 0., 0., 1., 0.);

	glPushMatrix();
	{
		if (m_axesActive)
			DrawAxes(3.f, 3.f);

		glBegin(GL_QUADS);
		{
			glColor3f(1.f, 1.f, 0.f);
			glVertex3f(-1.f, 1.f, 0.f);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(-1.f, -1.f, 0.f);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(1.f, -1.f, 0.f);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(1.f, 1.f, 0.f);
		}
		glEnd();
	}
	glPopMatrix();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(10., (double)w / (double)h, 1., 100.);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ...
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

inline double CGLRenderer::Deg2Rad(double degrees)
{
	return degrees * M_PI / 180;
}

void CGLRenderer::DrawAxes(GLfloat length, GLfloat width)
{
	glLineWidth(width);

	glBegin(GL_LINES);
	{
		// X
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(length, 0.f, 0.f);

		// Y
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, length, 0.f);

		// Z
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, length);
	}
	glEnd();

	glLineWidth(1.f);
}

void CGLRenderer::RotateView(GLdouble dX, GLdouble dY)
{
	m_XZrotAngle += dX;
	m_XYrotAngle += dY;

	if (m_XYrotAngle > 90.) m_XYrotAngle = 90.;
	else if (m_XYrotAngle < 0.) m_XYrotAngle = 0.;

	double alphaRad = Deg2Rad(m_XYrotAngle);
	double betaRad = Deg2Rad(m_XZrotAngle);

	m_xRot = (double)m_zoomDistance * cos(betaRad) * cos(alphaRad);
	m_yRot = (double)m_zoomDistance * sin(alphaRad);
	m_zRot = (double)m_zoomDistance * sin(betaRad) * cos(alphaRad);
}

void CGLRenderer::Zoom(GLint distance)
{
	m_zoomDistance += distance;
	if (m_zoomDistance < MIN_ZOOM) m_zoomDistance = MIN_ZOOM;
	else if (m_zoomDistance > MAX_ZOOM) m_zoomDistance = MAX_ZOOM;
	RotateView(0, 0);
}
