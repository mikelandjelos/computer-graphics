#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
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
	glClearColor(1., 1., 1., 0.);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0.f, 0.f, -5.f);
	switch (m_choice)
	{
	case 'A':
		TemplateVertices(GL_POINTS);
		break;
	case 'S':
		TemplateVertices(GL_LINES);
		break;
	case 'D':
		TemplateVertices(GL_LINE_STRIP);
		break;
	case 'F':
		TemplateVertices(GL_LINE_LOOP);
		break;
	case 'G':
		TemplateVertices(GL_TRIANGLES);
		break;
	case 'H':
		TemplateVertices(GL_TRIANGLE_STRIP);
		break;
	case 'J':
		TemplateVertices(GL_TRIANGLE_FAN);
		break;
	case 'K':
		TemplateVertices(GL_QUADS);
		break;
	case 'L':
		TemplateVertices(GL_QUAD_STRIP);
		break;
	case 'Q':
		TemplateVertices(GL_POLYGON);
		break;
	default:
		break;
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
	gluPerspective(40., (double)w / (double)h, 1., 100.);
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

void CGLRenderer::TemplateVertices(GLenum choice)
{
	glLineWidth(2.);
	glPointSize(10.);
	glBegin(choice);
	{
		glColor3f(.5f, .5f, 1.f);
		glVertex2f(-2., 1.);
		glVertex2f(-2., 0.);
		glVertex2f(-1., 1.);
		glVertex2f(-1., 0.);
		glVertex2f(0., 1.);
		glVertex2f(0., 0.);
		glVertex2f(1., 1.);
		glVertex2f(1., 0.);
		glVertex2f(2., 1.);
	}
	glEnd();
}
