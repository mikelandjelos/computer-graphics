#include "StdAfx.h"
#include "GLRenderer.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	m_viewRadius = MAX_RADIUS, m_viewAlpha = 0., m_viewBeta = 90.;
	SetView();
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

	gluLookAt(m_eyeX, m_eyeY, m_eyeZ, 0., 0., 0., 0., 1., 0.);

	if (m_axesActive)
		DrawAxes();

	if (m_gridActive)
		DrawGrid();

	glPushMatrix();
	{
		glPolygonMode(GL_FRONT_AND_BACK, m_onlyOutlines ? GL_LINE : GL_FILL);
		glColor3f(1.f, 0.f, 0.f);
		glTranslatef(-5.f * gridUnitSize, 0.f, 0.f);
		DrawCylinder(12, 5.f * gridUnitSize, 1.5f * gridUnitSize, 2.f * gridUnitSize);
		glTranslatef((5.f + 5.f) * gridUnitSize, 0.f, 0.f);
		DrawCone(12, 5.f * gridUnitSize, 1.5f * gridUnitSize);
		glTranslatef(-5.f * gridUnitSize, 2.5f * gridUnitSize, 0.f);
		DrawSphere(12, 2.5f * gridUnitSize);
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
	gluPerspective(40., (GLdouble)w / (GLdouble)h, 1., 100.);
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

void CGLRenderer::DrawAxes()
{
	glLineWidth(2.f);
	glBegin(GL_LINES);
	{
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(SCENE_SIZE, 0.f, 0.f);

		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, SCENE_SIZE, 0.f);

		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, SCENE_SIZE);
	}
	glEnd();
}

void CGLRenderer::DrawGrid()
{
	// xOz.
	glColor3f(0.f, 0.f, 0.f);
	glBegin(GL_LINES);
	for (int i = -SCENE_SIZE / 2.; i <= SCENE_SIZE / 2.; i += gridUnitSize)
	{
		glVertex3f(i, 0.f, -SCENE_SIZE / 2);
		glVertex3f(i, 0.f, SCENE_SIZE / 2);
		glVertex3f(-SCENE_SIZE / 2, 0.f, i);
		glVertex3f(SCENE_SIZE / 2, 0.f, i);
	}
	glEnd();
}

void CGLRenderer::ChangeView(GLdouble deltaRadius, GLdouble deltaAlpha, GLdouble deltaBeta)
{
	m_viewRadius += deltaRadius;
	m_viewAlpha += deltaAlpha;
	m_viewBeta += deltaBeta;

	if (m_viewRadius > MAX_RADIUS)
		m_viewRadius = MAX_RADIUS;

	if (m_viewRadius < MIN_RADIUS)
		m_viewRadius = MIN_RADIUS;

	if (m_viewAlpha > MAX_ALPHA_ANGLE)
		m_viewAlpha = MAX_ALPHA_ANGLE;

	if (m_viewAlpha < MIN_ALPHA_ANGLE)
		m_viewAlpha = MIN_ALPHA_ANGLE;

	SetView();
}

void CGLRenderer::SetView()
{
	m_eyeX = m_viewRadius * cos(Deg2Rad(m_viewAlpha)) * cos(Deg2Rad(m_viewBeta));
	m_eyeY = m_viewRadius * sin(Deg2Rad(m_viewAlpha));
	m_eyeZ = m_viewRadius * cos(Deg2Rad(m_viewAlpha)) * sin(Deg2Rad(m_viewBeta));
}

void CGLRenderer::DrawCylinder(GLint nSides, GLfloat height, GLfloat topRadius, GLfloat bottomRadius)
{
	if (nSides < 3)
		AfxMessageBox(L"Minimal number of sides is 3!"), exit(ERROR);

	const float deltaAlpha = 360. / nSides;

	// Sides.
	glBegin(GL_TRIANGLE_STRIP);
	for (float alpha = 0.f; alpha <= Deg2Rad(360.f + deltaAlpha); alpha += Deg2Rad(deltaAlpha))
		glVertex3f(bottomRadius * cos(alpha), 0.f, bottomRadius * sin(alpha)),
		glVertex3f(topRadius * cos(alpha), height, topRadius * sin(alpha));
	glEnd();

	// Bottom base.
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.f, 0.f, 0.f);
	for (float alpha = 0.f; alpha <= Deg2Rad(360.f + deltaAlpha); alpha += Deg2Rad(deltaAlpha))
		glVertex3f(bottomRadius * cos(alpha), 0.f, bottomRadius * sin(alpha));
	glEnd();

	// Top base.
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.f, height, 0.f);
	for (float alpha = 0.f; alpha <= Deg2Rad(360.f + deltaAlpha); alpha += Deg2Rad(deltaAlpha))
		glVertex3f(topRadius * cos(alpha), height, topRadius * sin(alpha));
	glEnd();
}

void CGLRenderer::DrawCone(GLint nSides, GLfloat height, GLfloat radius)
{
	const float deltaAlpha = 360. / nSides;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.f, 0.f, 0.f);
	for (float alpha = 0.f; alpha <= Deg2Rad(360.f + deltaAlpha); alpha += Deg2Rad(deltaAlpha))
		glVertex3f(radius * cos(alpha), 0.f, radius * sin(alpha));
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.f, height, 0.f);
	for (float alpha = 0.f; alpha <= Deg2Rad(360.f + deltaAlpha); alpha += Deg2Rad(deltaAlpha))
		glVertex3f(radius * cos(alpha), 0.f, radius * sin(alpha));
	glEnd();
}

void CGLRenderer::DrawSphere(GLint nSides, GLfloat radius)
{
	const float deltaAngle = 360. / nSides;

	for (float beta = 0.f; beta < Deg2Rad(180. + deltaAngle); beta += Deg2Rad(deltaAngle))
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (float alpha = 0.f; alpha < Deg2Rad(360. + deltaAngle); alpha += Deg2Rad(deltaAngle))
			glVertex3f(radius * cos(alpha) * cos(beta),
				radius * sin(alpha),
				radius * cos(alpha) * sin(beta)),
			glVertex3f(radius * cos(alpha) * cos(beta + Deg2Rad(deltaAngle)),
				radius * sin(alpha),
				radius * cos(alpha) * sin(beta + Deg2Rad(deltaAngle)));
		glEnd();
	}
}
