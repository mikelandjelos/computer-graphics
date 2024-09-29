#include "StdAfx.h"
#include "GLRenderer.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
	: SCENE_SIZE(50.),
	MAX_RADIUS(80.), MIN_RADIUS(10.),
	MAX_ALPHA(90.), MIN_ALPHA(-90.),
	m_viewRadius(80. / 2.), m_viewAlpha(0.), m_viewBeta(90.),
	m_axesActive(TRUE), m_gridActive(TRUE), m_onlyOutlines(FALSE), m_normalsActive(FALSE)
{
	SetView();

	m_redShinyMaterial.SetAmbient(.3f, 0.f, 0.f, 1.f);
	m_redShinyMaterial.SetDiffuse(.6f, 0.f, 0.f, 1.f);
	m_redShinyMaterial.SetSpecular(.4f, 0.f, 0.f, 1.f);
	m_redShinyMaterial.SetShininess(32.f);

	m_grayMattedMaterial.SetAmbient(.1f, .1f, .1f, 1.f);
	m_grayMattedMaterial.SetDiffuse(.4f, .4f, .4f, 1.f);
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
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE); // Performance hit - be careful!
	glEnable(GL_LIGHTING);

	PrepareLighting();
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
		DrawAxes(3., SCENE_SIZE);

	CONST GLdouble gridUnitSize = SCENE_SIZE / 10.;

	if (m_gridActive)
		DrawGrid(SCENE_SIZE, gridUnitSize);

	glPolygonMode(GL_FRONT, m_onlyOutlines ? GL_LINE : GL_FILL);

	glPushMatrix();
	{
		m_grayMattedMaterial.Select();
		DrawWalls(SCENE_SIZE);

		m_redShinyMaterial.Select();
		DrawCylinder(gridUnitSize * 2., gridUnitSize * 3., gridUnitSize * 1., 30);
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
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45., (double)w / h, 1., 150.);
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

void CGLRenderer::ChangeView(GLdouble deltaRadius, GLdouble deltaAlpha, GLdouble deltaBeta)
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
	GLdouble alphaRad = Deg2Rad(m_viewAlpha);
	GLdouble betaRad = Deg2Rad(m_viewBeta);

	m_eyeX = m_viewRadius * cos(alphaRad) * cos(betaRad);
	m_eyeY = m_viewRadius * sin(alphaRad);
	m_eyeZ = m_viewRadius * cos(alphaRad) * sin(betaRad);
}

void CGLRenderer::DrawAxes(GLdouble width, GLdouble length)
{
	glDisable(GL_LIGHTING);
	glLineWidth(width);
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
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawGrid(GLdouble sceneSize, GLdouble gridUnit)
{
	glDisable(GL_LIGHTING);
	glLineWidth(1.f);
	glColor3f(1.f, 1.f, 1.f);
	// xOz
	glBegin(GL_LINES);
	for (int cursor = -sceneSize / 2.; cursor <= sceneSize / 2.; cursor += gridUnit)
		glVertex3f(cursor, 0, -sceneSize / 2.),
		glVertex3f(cursor, 0, sceneSize / 2.);
	glEnd();
	glBegin(GL_LINES);
	for (int cursor = -sceneSize / 2.; cursor <= sceneSize / 2.; cursor += gridUnit)
		glVertex3f(-sceneSize / 2., 0, cursor),
		glVertex3f(sceneSize / 2., 0, cursor);
	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawCylinder(GLdouble height, GLdouble bottomRadius, GLdouble topRadius, GLsizei nSeg)
{
	CONST GLdouble deltaBetaRad = Deg2Rad(360. / nSeg);

	glLineWidth(1.f);

	// Bottom base.
	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0., -1., 0.);
	for (GLdouble betaRad = 0.; betaRad <= Deg2Rad(360.); betaRad += deltaBetaRad)
		glVertex3d(bottomRadius * cos(betaRad), 0., bottomRadius * sin(betaRad));
	glEnd();

	// Top base.
	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0., 1., 0.);
	for (GLdouble betaRad = Deg2Rad(360.); betaRad >= 0.; betaRad -= deltaBetaRad)
		glVertex3d(topRadius * cos(betaRad), height, topRadius * sin(betaRad));
	glEnd();

	CONST GLdouble r = (bottomRadius - topRadius) / 2;
	CONST GLdouble L = sqrt(r * r + height * height);
	CONST GLdouble ny = r / L;
	CONST GLdouble nr = height / L;

	// Sides.
	glBegin(GL_TRIANGLE_STRIP);
	for (GLdouble betaRad = 0.; betaRad <= Deg2Rad(360.) + deltaBetaRad; betaRad += deltaBetaRad)
		glNormal3d(nr * cos(betaRad), ny, nr * sin(betaRad)),
		glVertex3d(bottomRadius * cos(betaRad), 0., bottomRadius * sin(betaRad)),
		glVertex3d(topRadius * cos(betaRad), height, topRadius * sin(betaRad));
	glEnd();

	if (m_normalsActive)
	{
		glDisable(GL_LIGHTING);
		glColor3f(0.f, 1.f, 1.f);
		glLineWidth(2.f);
		glBegin(GL_LINES);
		{
			// Bottom base.
			glVertex3d(0., 0., 0.);
			glVertex3d(0., 0. - 1., 0.);
			// Top base.
			glVertex3d(0., height, 0.);
			glVertex3d(0., height + 1., 0.);
			// Sides.
			for (GLdouble betaRad = 0.; betaRad <= Deg2Rad(360.) + deltaBetaRad; betaRad += deltaBetaRad)
				glVertex3d(bottomRadius * cos(betaRad), 0., bottomRadius * sin(betaRad)),
				glVertex3d((bottomRadius + nr) * cos(betaRad), 0. + ny, (bottomRadius + nr) * sin(betaRad)),
				glVertex3d(topRadius * cos(betaRad), height, topRadius * sin(betaRad)),
				glVertex3d((topRadius + nr) * cos(betaRad), height + ny, (topRadius + nr) * sin(betaRad));
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
}

void CGLRenderer::PrepareLighting()
{
	GLfloat lightModelAmbient[] = { .05f, .05f, .05f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);

	GLfloat lightYellowAmbient[] = { .5f, .5f, 0.f, 1.f };
	GLfloat lightYellowDiffuse[] = { 1.f, 1.f, 0.f, 1.f };
	GLfloat lightYellowSpecular[] = { .7f, .7f, 0.f, 1.f };
	GLfloat lightYellowPosition[] = { 0.f, SCENE_SIZE / 2.f, SCENE_SIZE / 2.f };
	GLfloat lightYellowDirection[] = { 0.f, 0.f, -1.f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightYellowAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightYellowDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightYellowSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightYellowPosition);

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightYellowDirection);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 75.f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 5.f);

	glEnable(GL_LIGHT0);
}

void CGLRenderer::DrawWalls(GLdouble size)
{
	glBegin(GL_QUADS);
	{
		glNormal3d(0., 0., 1.);
		glVertex3d(-size / 2., 0., -size / 2.);
		glVertex3d(size / 2., 0., -size / 2.);
		glVertex3d(size / 2., size, -size / 2.);
		glVertex3d(-size / 2., size, -size / 2.);
	}
	glEnd();
}
