#include "StdAfx.h"
#include "GLRenderer.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	ELEMENTARY_WIDTH = 0.4 * GRID_UNIT_SIZE;
	ELEMENTARY_HEIGHT = 1.5 * GRID_UNIT_SIZE;

	m_viewRadius = 3 * SCENE_SIZE, m_viewAlpha = 0., m_viewBeta = 90.;
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
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_LIGHTING);
	glClearColor(128. / 255., 192. / 255., 255. / 255., 0.f);
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

	glPushMatrix();
	{
		if (m_axesActive)
			DrawAxes(SCENE_SIZE);

		if (m_gridActive)
			DrawGrid(SCENE_SIZE, SCENE_SIZE, SCENE_SIZE / GRID_UNIT_SIZE, SCENE_SIZE / GRID_UNIT_SIZE);

		glPolygonMode(GL_FRONT_AND_BACK, m_onlyOutlines ? GL_LINE : GL_FILL);

		glColor3f(0.f, 0.f, 0.f);
		glLineWidth(1.f);
		DrawFigure(m_rotatingPartAngle);
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
	gluPerspective(45., (GLdouble)w / h, 1., 100.);
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

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg)
{
	const GLdouble deltaBetaRad = Deg2Rad(360. / nSeg);

	// Top base.
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0., 0., 0.);
	for (GLdouble betaRad = 0.; betaRad < Deg2Rad(360. + deltaBetaRad); betaRad += deltaBetaRad)
		glVertex3d(r1 * cos(betaRad), 0., r1 * sin(betaRad));
	glEnd();

	// Sides.
	glBegin(GL_TRIANGLE_STRIP);
	for (GLdouble betaRad = 0.; betaRad < Deg2Rad(360. + deltaBetaRad); betaRad += deltaBetaRad)
		glVertex3d(r1 * cos(betaRad), 0., r1 * sin(betaRad)),
		glVertex3d(r2 * cos(betaRad), h, r2 * sin(betaRad));
	glEnd();

	// Bottom base.
	glBegin(GL_TRIANGLE_FAN);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0., h, 0.);
	for (GLdouble betaRad = 0.; betaRad < Deg2Rad(360. + deltaBetaRad); betaRad += deltaBetaRad)
		glVertex3d(r2 * cos(betaRad), h, r2 * sin(betaRad));
	glEnd();
}

void CGLRenderer::DrawCone(double h, double r, int nSeg)
{
	const GLdouble deltaBetaRad = Deg2Rad(360. / nSeg);

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0., h, 0.);
	for (GLdouble betaRad = 0.; betaRad < Deg2Rad(360. + deltaBetaRad); betaRad += deltaBetaRad)
		glVertex3d(r * cos(betaRad), 0., r * sin(betaRad));
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0., 0., 0.);
	for (GLdouble betaRad = 0.; betaRad < Deg2Rad(360. + deltaBetaRad); betaRad += deltaBetaRad)
		glVertex3d(r * cos(betaRad), 0., r * sin(betaRad));
	glEnd();
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	const GLdouble deltaAlphaRad = Deg2Rad(360. / nSegAlpha);
	const GLdouble deltaBetaRad = Deg2Rad(180. / nSegBeta);

	for (GLdouble betaRad = 0.; betaRad < Deg2Rad(180. + deltaBetaRad); betaRad += deltaBetaRad)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (GLdouble alphaRad = 0.; alphaRad < Deg2Rad(360. + deltaAlphaRad); alphaRad += deltaAlphaRad)
		{
			glVertex3d(r * cos(alphaRad) * cos(betaRad), r * sin(alphaRad), r * cos(alphaRad) * sin(betaRad));
			glVertex3d(r * cos(alphaRad) * cos(betaRad + deltaBetaRad), r * sin(alphaRad), r * cos(alphaRad) * sin(betaRad + deltaBetaRad));
		}
		glEnd();
	}
}

void CGLRenderer::DrawAxes(double length)
{
	glLineWidth(3.f);
	glBegin(GL_LINES);
	{
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(length, 0.f, 0.f);

		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, length, 0.f);

		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, length);
	}
	glEnd();
}

void CGLRenderer::DrawGrid(double width, double height, int nSegW, int nSegH)
{
	int loopEnd = max(width, height);
	glColor3f(1.f, 1.f, 1.f);
	glLineWidth(1.f);
	glBegin(GL_LINES);
	for (double i = -width / 2.; i <= width / 2.; i += width / nSegW)
		glVertex3d(i, 0., -width / 2.),
		glVertex3d(i, 0., width / 2.);
	glEnd();
	glBegin(GL_LINES);
	for (double i = -height / 2.; i <= height / 2.; i += height / nSegH)
		glVertex3d(-height / 2., 0., i),
		glVertex3d(height / 2., 0., i);
	glEnd();
}

void CGLRenderer::ChangeView(double deltaRadius, double deltaAlpha, double deltaBeta)
{
	m_viewRadius += deltaRadius;
	m_viewAlpha += deltaAlpha;
	m_viewBeta += deltaBeta;

	if (m_viewRadius > MAX_RADIUS)
		m_viewRadius = MAX_RADIUS;

	if (m_viewRadius < MIN_RADIUS)
		m_viewRadius = MIN_RADIUS;

	if (m_viewAlpha > MAX_ALPHA)
		m_viewAlpha = MAX_ALPHA;

	if (m_viewAlpha < MIN_ALPHA)
		m_viewAlpha = MIN_ALPHA;

	SetView();
}

void CGLRenderer::SetView()
{
	GLdouble deltaAlphaRad = Deg2Rad(m_viewAlpha);
	GLdouble deltaBetaRad = Deg2Rad(m_viewBeta);

	m_eyeX = m_viewRadius * cos(deltaAlphaRad) * cos(deltaBetaRad);
	m_eyeY = m_viewRadius * sin(deltaAlphaRad);
	m_eyeZ = m_viewRadius * cos(deltaAlphaRad) * sin(deltaBetaRad);
}

void CGLRenderer::DrawFigure(double angle)
{
	glPushMatrix();
	{
		DrawBase();
		glTranslated(0., ELEMENTARY_HEIGHT + ELEMENTARY_WIDTH * 1.5, 0.);
		glRotated(90., 0., 1., 0.);
		DrawCactus(angle);
	}
	glPopMatrix();
}

void CGLRenderer::DrawBase()
{
	glColor3f(221. / 255., 126. / 255., 40. / 255.);
	glPushMatrix();
	{
		DrawCylinder(ELEMENTARY_HEIGHT, ELEMENTARY_HEIGHT * 0.7, ELEMENTARY_HEIGHT * 0.7, 8);
		glTranslated(0., ELEMENTARY_HEIGHT, 0.);
		DrawCylinder(ELEMENTARY_WIDTH * 1.5, ELEMENTARY_HEIGHT, ELEMENTARY_HEIGHT, 8);
	}
	glPopMatrix();
}

void CGLRenderer::DrawCactus(double angle)
{
	DrawCactusCylinder();
	glTranslated(0., ELEMENTARY_HEIGHT + ELEMENTARY_WIDTH / 2., 0.);
	DrawCactusSphere();

	// Left branch.
	glPushMatrix();
	{
		glRotated(45., 0., 0., 1.);
		glTranslated(0., ELEMENTARY_WIDTH / 2., 0.);
		DrawCactusCylinder();
		glTranslated(0., ELEMENTARY_HEIGHT + ELEMENTARY_WIDTH / 2., 0.);
		DrawCactusSphere();
		glTranslated(0., ELEMENTARY_WIDTH / 2., 0.);
		DrawCactusCone();
		glTranslated(0., ELEMENTARY_HEIGHT + ELEMENTARY_WIDTH / 2., 0.);
		DrawCactusSphere();
	}
	glPopMatrix();

	// Middle branch.
	glPushMatrix();
	{
		glTranslated(0., ELEMENTARY_WIDTH / 2., 0.);
		DrawCactusCylinder();
		glTranslated(0., ELEMENTARY_HEIGHT + ELEMENTARY_WIDTH / 2., 0.);
		DrawCactusSphere();

		// Left branch.
		glPushMatrix();
		{
			glRotated(45. + angle, 0., 0., 1.);
			glTranslated(0., ELEMENTARY_WIDTH / 2., 0.);
			DrawCactusCylinder(TRUE); // YELLOW ONE!
			glTranslated(0., ELEMENTARY_HEIGHT + ELEMENTARY_WIDTH / 2., 0.);
			DrawCactusSphere();
			glTranslated(0., ELEMENTARY_WIDTH / 2., 0.);
			DrawCactusCone();
			glTranslated(0., ELEMENTARY_HEIGHT + ELEMENTARY_WIDTH / 2., 0.);
			DrawCactusSphere();
		}
		glPopMatrix();

		// Right branch.
		glPushMatrix();
		{
			glRotated(-45., 0., 0., 1.);
			glTranslated(0., ELEMENTARY_WIDTH / 2., 0.);
			DrawCactusCylinder();
			glTranslated(0., ELEMENTARY_HEIGHT + ELEMENTARY_WIDTH / 2., 0.);
			DrawCactusSphere();
			glTranslated(0., ELEMENTARY_WIDTH / 2., 0.);
			DrawCactusCylinder();
			glTranslated(0., ELEMENTARY_HEIGHT + ELEMENTARY_WIDTH / 2., 0.);
			DrawCactusSphere();
		}
		glPopMatrix();
	}
	glPopMatrix();

	// Right branch.
	glPushMatrix();
	{
		glRotated(-45., 0., 0., 1.);
		glTranslated(0., ELEMENTARY_WIDTH / 2., 0.);
		DrawCactusCylinder();
		glTranslated(0., ELEMENTARY_HEIGHT + ELEMENTARY_WIDTH / 2., 0.);
		DrawCactusSphere();
		glTranslated(0., ELEMENTARY_WIDTH / 2., 0.);
		DrawCactusCone();
		glTranslated(0., ELEMENTARY_HEIGHT + ELEMENTARY_WIDTH / 2., 0.);
		DrawCactusSphere();
		glTranslated(0., ELEMENTARY_WIDTH / 2., 0.);
		DrawCactusCone();
		glTranslated(0., ELEMENTARY_HEIGHT + ELEMENTARY_WIDTH / 2., 0.);
		DrawCactusSphere();
	}
	glPopMatrix();
}

void CGLRenderer::DrawCactusCylinder(BOOL isYellow)
{
	if (isYellow)
		glColor3f(1.f, 1.f, 0.f);
	else
		glColor3f(0.f, 1.f, 0.f);
	DrawCylinder(ELEMENTARY_HEIGHT, ELEMENTARY_WIDTH, ELEMENTARY_WIDTH, 12);
}

void CGLRenderer::DrawCactusCone()
{
	glColor3f(0.f, 1.f, 0.f);
	DrawCone(ELEMENTARY_HEIGHT, ELEMENTARY_WIDTH, 12);
}

void CGLRenderer::DrawCactusSphere()
{
	glColor3f(0.f, 170.f / 255.f, 0.f);
	DrawSphere(ELEMENTARY_WIDTH * 0.7, 12, 6);
}


