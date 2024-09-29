#include "StdAfx.h"
#include "GLRenderer.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
	: SCENE_SIZE(50.), GRID_UNIT_SIZE(20. / 10.),
	MAX_RADIUS(20. * 4.), MIN_RADIUS(20. / 2.), MAX_ALPHA(90.), MIN_ALPHA(0.)
{
	m_viewRadius = MAX_RADIUS / 2, m_viewAlpha = 20., m_viewBeta = 90.;
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
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE); // Be careful - performance hit.
	// glEnable(GL_LIGHTING);
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
		DrawAxes(SCENE_SIZE);

	if (m_gridActive)
	{
		DrawGrid(SCENE_SIZE, SCENE_SIZE, SCENE_SIZE / GRID_UNIT_SIZE, SCENE_SIZE / GRID_UNIT_SIZE);
	}

	glPolygonMode(GL_FRONT, m_onlyOutlines ? GL_LINE : GL_FILL);

	glPushMatrix();
	{
		glColor3f(1.f, 0.f, 0.f);
		glTranslatef(-5.f * GRID_UNIT_SIZE, 0.f, -5.f * GRID_UNIT_SIZE);
		DrawConicalFrustum(2 * GRID_UNIT_SIZE, 2 * GRID_UNIT_SIZE, 1 * GRID_UNIT_SIZE, 12);
		glTranslatef(+10.f * GRID_UNIT_SIZE, 0.f, 0.f);
		glColor3f(1.f, 0.f, 0.f);
		DrawSphere(2 * GRID_UNIT_SIZE, 6, 12);
		glTranslatef(-10.f * GRID_UNIT_SIZE, 0.f, +10.f * GRID_UNIT_SIZE);
		glColor3f(1.f, 0.f, 0.f);
		DrawConicalFrustum(2 * GRID_UNIT_SIZE, 1 * GRID_UNIT_SIZE, 2 * GRID_UNIT_SIZE, 12);
		glTranslatef(+10.f * GRID_UNIT_SIZE, 0.f, 0.f);
		glColor3f(1.f, 0.f, 0.f);
		DrawCone(2 * GRID_UNIT_SIZE, 1 * GRID_UNIT_SIZE, 12);
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

void CGLRenderer::DrawAxes(GLdouble length)
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

void CGLRenderer::DrawGrid(GLdouble width, GLdouble height, GLint wNumSeg, GLint hNumSeg)
{
	glLineWidth(1.f);
	glColor3f(0.f, 0.f, 0.f);
	glBegin(GL_LINES);
	for (int i = -width / 2.; i <= width / 2.; i += width / wNumSeg)
		glVertex3f(i, 0, -width / 2.),
		glVertex3f(i, 0, width / 2.);
	glEnd();
	glBegin(GL_LINES);
	for (int i = -height / 2.; i <= height / 2.; i += height / hNumSeg)
		glVertex3f(-height / 2., 0, i),
		glVertex3f(height / 2., 0, i);
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

void CGLRenderer::DrawConicalFrustum(GLfloat height, GLfloat bottomRadius, GLfloat topRadius, GLsizei nSeg)
{
	CONST GLfloat deltaAngleRad = Deg2Rad(360. / nSeg);

	// Bottom base.
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.f, -1.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	for (GLfloat alphaRad = 0.; alphaRad < Deg2Rad(360. + deltaAngleRad); alphaRad += deltaAngleRad)
		glVertex3f(bottomRadius * cos(alphaRad), 0.f, bottomRadius * sin(alphaRad));
	glEnd();

	// Top base.
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, height, 0.f);
	for (GLfloat alphaRad = Deg2Rad(360. + deltaAngleRad); alphaRad > 0.; alphaRad -= deltaAngleRad)
		glVertex3f(topRadius * cos(alphaRad), height, topRadius * sin(alphaRad));
	glEnd();

	GLfloat r = (bottomRadius - topRadius) / 2;
	GLfloat L = sqrt(r * r + height * height);
	GLfloat ny = r / L;
	GLfloat nr = height / L;

	// Sides.
	glBegin(GL_TRIANGLE_STRIP);
	for (GLfloat alphaRad = 0.; alphaRad < Deg2Rad(360. + deltaAngleRad); alphaRad += deltaAngleRad)
		glNormal3f(nr * cos(alphaRad), ny, nr * sin(alphaRad)),
		glVertex3f(bottomRadius * cos(alphaRad), 0.f, bottomRadius * sin(alphaRad)),
		glVertex3f(topRadius * cos(alphaRad), height, topRadius * sin(alphaRad));
	glEnd();

	CONST GLfloat normalLength = GRID_UNIT_SIZE;
	glLineWidth(3.f);
	glColor3f(0.f, 1.f, 0.f);
	if (m_showNormals)
	{
		glBegin(GL_LINES);
		{
			// Bottom base normal.
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(0.f, 0 - normalLength, 0.f);

			// Top base normal.
			glVertex3f(0.f, height, 0.f);
			glVertex3f(0.f, height + normalLength, 0.f);

			// Side normals.
			for (GLfloat alphaRad = 0.; alphaRad < Deg2Rad(360. + deltaAngleRad); alphaRad += deltaAngleRad)
				glVertex3f(bottomRadius * cos(alphaRad), 0.f, bottomRadius * sin(alphaRad)),
				glVertex3f((bottomRadius + nr) * cos(alphaRad), 0.f + ny, (bottomRadius + nr) * sin(alphaRad)),
				glVertex3f(topRadius * cos(alphaRad), height, topRadius * sin(alphaRad)),
				glVertex3f((topRadius + nr) * cos(alphaRad), height + ny, (topRadius + nr) * sin(alphaRad));
		}
		glEnd();
	}
}

void CGLRenderer::DrawCone(GLfloat height, GLfloat baseRadius, GLsizei nSeg)
{
	CONST GLfloat deltaAlphaRad = Deg2Rad(360. / nSeg);

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.f, -1.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	for (GLfloat alphaRad = 0.; alphaRad < Deg2Rad(360. + baseRadius); alphaRad += deltaAlphaRad)
		glVertex3f(baseRadius * cos(alphaRad), 0.f, baseRadius * sin(alphaRad));
	glEnd();

	GLfloat r = baseRadius;
	GLfloat L = sqrt(height * height + baseRadius * baseRadius);
	GLfloat ny = r / L;
	GLfloat nr = height / L;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.f, height, 0.f);
	for (GLfloat alphaRad = Deg2Rad(360. + baseRadius); alphaRad > 0.; alphaRad -= deltaAlphaRad)
		glNormal3f(nr * cos(alphaRad), ny, nr * sin(alphaRad)),
		glVertex3f(baseRadius * cos(alphaRad), 0.f, baseRadius * sin(alphaRad));
	glEnd();

	glLineWidth(2.f);
	if (m_showNormals)
	{
		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_LINES);
		{
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(0.f, -1.f, 0.f);

			for (GLfloat alphaRad = 0.; alphaRad < Deg2Rad(360. + baseRadius); alphaRad += deltaAlphaRad)
				glVertex3f(baseRadius * cos(alphaRad), 0.f, baseRadius * sin(alphaRad)),
				glVertex3f((baseRadius + nr) * cos(alphaRad), 0.f + ny, (baseRadius + nr) * sin(alphaRad));
		}
		glEnd();
	}
}

void CGLRenderer::DrawSphere(GLfloat radius, GLsizei nSegVertical, GLsizei nSegHorizontal)
{
	CONST GLfloat deltaBetaRad = Deg2Rad(360. / nSegHorizontal);
	CONST GLfloat deltaAlphaRad = Deg2Rad(180. / nSegVertical);

	for (GLfloat betaRad = 0.; betaRad < Deg2Rad(360. + deltaBetaRad); betaRad += deltaBetaRad)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (GLfloat alphaRad = Deg2Rad(90.); alphaRad < Deg2Rad(270. + deltaAlphaRad); alphaRad += deltaAlphaRad)
		{
			GLfloat vertexLeft[3] = {
				radius * cos(alphaRad) * cos(betaRad),
				radius * sin(alphaRad),
				radius * cos(alphaRad) * sin(betaRad),
			};
			GLfloat leftNorm = sqrt(vertexLeft[0] * vertexLeft[0] + vertexLeft[1] * vertexLeft[1] + vertexLeft[2] * vertexLeft[2]);
			GLfloat vertexRight[3] = {
				radius * cos(alphaRad) * cos(betaRad + deltaBetaRad),
				radius * sin(alphaRad),
				radius * cos(alphaRad) * sin(betaRad + deltaBetaRad),
			};
			GLfloat rightNorm = sqrt(vertexRight[0] * vertexRight[0] + vertexRight[1] * vertexRight[1] + vertexRight[2] * vertexRight[2]);
			glNormal3f(vertexLeft[0] / leftNorm, vertexLeft[1] / leftNorm, vertexLeft[2] / leftNorm);
			glVertex3fv(vertexLeft);
			glNormal3f(vertexRight[0] / rightNorm, vertexRight[1] / rightNorm, vertexRight[2] / rightNorm);
			glVertex3fv(vertexRight);
		}
		glEnd();
	}

	glLineWidth(3.f);
	glColor3f(0.f, 1.f, 0.f);
	if (m_showNormals)
		for (GLfloat betaRad = 0.; betaRad < Deg2Rad(360. + deltaBetaRad); betaRad += deltaBetaRad)
		{
			for (GLfloat alphaRad = Deg2Rad(90.); alphaRad < Deg2Rad(270. + deltaAlphaRad); alphaRad += deltaAlphaRad)
			{
				glBegin(GL_LINES);
				GLfloat vertexLeft[3] = {
					radius * cos(alphaRad) * cos(betaRad),
					radius * sin(alphaRad),
					radius * cos(alphaRad) * sin(betaRad),
				};
				GLfloat leftNorm = sqrt(vertexLeft[0] * vertexLeft[0] + vertexLeft[1] * vertexLeft[1] + vertexLeft[2] * vertexLeft[2]);
				GLfloat vertexRight[3] = {
					radius * cos(alphaRad) * cos(betaRad + deltaBetaRad),
					radius * sin(alphaRad),
					radius * cos(alphaRad) * sin(betaRad + deltaBetaRad),
				};
				GLfloat rightNorm = sqrt(vertexRight[0] * vertexRight[0] + vertexRight[1] * vertexRight[1] + vertexRight[2] * vertexRight[2]);
				glVertex3fv(vertexLeft);
				glVertex3f(vertexLeft[0] + (vertexLeft[0] / leftNorm),
					vertexLeft[1] + (vertexLeft[1] / leftNorm),
					vertexLeft[2] + (vertexLeft[2] / leftNorm));
				glVertex3fv(vertexRight);
				glNormal3f(vertexRight[0] + (vertexRight[0] / rightNorm),
					vertexRight[1] + (vertexRight[1] / rightNorm),
					vertexRight[2] + (vertexRight[2] / rightNorm));
				glEnd();
			}
		}
}
