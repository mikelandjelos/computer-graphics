#include "StdAfx.h"
#include "GLRenderer.h"
#include "GLRenderer.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
	: m_hrc(NULL),
	SCENE_SIZE(80.), GRID_UNIT_SIZE(80. / 20.),
	MIN_RADIUS(10.), MAX_RADIUS(80.), MIN_ALPHA(-90.), MAX_ALPHA(90.),
	m_eyeX(-1.), m_eyeY(-1.), m_eyeZ(-1.),
	m_viewRadius(50.), m_viewAlpha(30.), m_viewBeta(90.),
	m_axesActive(TRUE), m_gridActive(TRUE), m_fill(TRUE), m_normalsActive(FALSE),
	BOX_SIZE(SCENE_SIZE * 0.6), PART_HEIGHT(1.5),
	m_redLightActive(FALSE), m_greenLightActive(FALSE), m_blueLightActive(FALSE)
{
	SetView();

	GLfloat pink[3] = { 1.f, 0.f, 1.f };

	m_materialPink.SetAmbient(0.5f * pink[0], 0.5f * pink[1], 0.5f * pink[2], 1.f);
	m_materialPink.SetDiffuse(1.f * pink[0], 1.f * pink[1], 1.f * pink[2], 1.f);
	m_materialPink.SetSpecular(0.8f * pink[0], 0.8f * pink[1], 0.8f * pink[2], 1.f);

	GLfloat cyan[3] = { 0.f, 1.f, 1.f };

	m_materialCyan.SetAmbient(0.5f * cyan[0], 0.5f * cyan[1], 0.5f * cyan[2], 1.f);
	m_materialCyan.SetDiffuse(1.f * cyan[0], 1.f * cyan[1], 1.f * cyan[2], 1.f);
	m_materialCyan.SetSpecular(0.8f * cyan[0], 0.8f * cyan[1], 0.8f * cyan[2], 1.f);

	GLfloat lightGray[3] = { 211.f / 255.f, 211.f / 255.f, 211.f / 255.f };

	m_materialLightGrey.SetAmbient(lightGray[0], lightGray[1], lightGray[2], 1.f);
	m_materialLightGrey.SetDiffuse(lightGray[0], lightGray[1], lightGray[2], 1.f);
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
	glTranslated(0., -GRID_UNIT_SIZE * 7., 0.);

	if (m_axesActive)
		DrawAxes(3., SCENE_SIZE / 2.);

	if (m_gridActive)
		DrawGrid(SCENE_SIZE, SCENE_SIZE / GRID_UNIT_SIZE);

	glPolygonMode(GL_FRONT, m_fill ? GL_FILL : GL_LINE);

	glPushMatrix();
	{
		PositionLighting();
		DrawBox();
		DrawStand();
		DrawVase();
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
	gluPerspective(45., (GLdouble)w / h, 1., 150.);
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

void CGLRenderer::SetView()
{
	GLdouble alphaRad = Deg2Rad(m_viewAlpha);
	GLdouble betaRad = Deg2Rad(m_viewBeta);

	m_eyeX = m_viewRadius * cos(alphaRad) * cos(betaRad);
	m_eyeY = m_viewRadius * sin(alphaRad);
	m_eyeZ = m_viewRadius * cos(alphaRad) * sin(betaRad);
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

void CGLRenderer::DrawAxes(GLdouble width, GLdouble lenght)
{
	glDisable(GL_LIGHTING);
	glLineWidth(width);
	glBegin(GL_LINES);
	{
		// x
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(lenght, 0.f, 0.f);

		// y
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, lenght, 0.f);

		// z
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, lenght);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawGrid(GLdouble size, GLsizei nUnits)
{
	GLdouble unitSize = size / nUnits;
	glDisable(GL_LIGHTING);
	glLineWidth(1.f);
	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_LINES);
	for (GLdouble i = -size / 2.; i <= size / 2.; i += unitSize)
		glVertex3d(i, 0., -size / 2.),
		glVertex3d(i, 0., size / 2.);
	glEnd();
	glBegin(GL_LINES);
	for (GLdouble i = -size / 2.; i <= size / 2.; i += unitSize)
		glVertex3d(-size / 2., 0., i),
		glVertex3d(size / 2., 0., i);
	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawTruncatedCone(GLdouble height, GLdouble bottomRadius, GLdouble topRadius, GLsizei nSeg)
{
	CONST GLdouble deltaBeta = Deg2Rad(360. / nSeg);

	CONST GLdouble r = (bottomRadius - topRadius) / 2.;
	CONST GLdouble L = sqrt(height * height + r * r);
	CONST GLdouble ny = r / L;
	CONST GLdouble nr = height / L;

	// Drawing the sides.
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (GLdouble beta = 0.; beta <= Deg2Rad(360.) + deltaBeta; beta += deltaBeta)
			glNormal3d(nr * cos(beta), ny, nr * sin(beta)),
			glVertex3d(bottomRadius * cos(beta), 0., bottomRadius * sin(beta)),
			glVertex3d(topRadius * cos(beta), height, topRadius * sin(beta));
	}
	glEnd();

	// Drawing normals.
	if (m_normalsActive)
	{
		glDisable(GL_LIGHTING);
		glLineWidth(3.f);
		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_LINES);
		{
			for (GLdouble beta = 0.; beta <= Deg2Rad(360.) + deltaBeta; beta += deltaBeta)
				glVertex3d(bottomRadius * cos(beta), 0., bottomRadius * sin(beta)),
				glVertex3d((bottomRadius + nr) * cos(beta), 0. + ny, (bottomRadius + nr) * sin(beta)),
				glVertex3d(topRadius * cos(beta), height, topRadius * sin(beta)),
				glVertex3d((topRadius + nr) * cos(beta), height + ny, (topRadius + nr) * sin(beta));
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
}

void CGLRenderer::DrawHalfSphere(GLdouble radius, GLsizei nSeg)
{
	CONST GLdouble deltaAngle = Deg2Rad(360. / nSeg);

	for (GLdouble beta = 0.; beta <= Deg2Rad(360.); beta += deltaAngle)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (GLdouble alpha = Deg2Rad(90.); alpha >= 0.; alpha -= deltaAngle)
		{
			GLdouble x = radius * cos(alpha) * cos(beta),
				y = radius * sin(alpha),
				z = radius * cos(alpha) * sin(beta);

			GLdouble xNext = radius * cos(alpha) * cos(beta + deltaAngle),
				yNext = radius * sin(alpha),
				zNext = radius * cos(alpha) * sin(beta + deltaAngle);

			glNormal3d(x / radius, y / radius, z / radius);
			glVertex3d(x, y, z);
			glNormal3d(xNext / radius, yNext / radius, zNext / radius);
			glVertex3d(xNext, yNext, zNext);
		}
		glEnd();
	}

	if (m_normalsActive)
	{
		glDisable(GL_LIGHTING);
		glLineWidth(3.f);
		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_LINES);
		for (GLdouble beta = 0.; beta <= Deg2Rad(360.); beta += deltaAngle)
			for (GLdouble alpha = 0.; alpha <= Deg2Rad(90.) + deltaAngle; alpha += deltaAngle)
			{
				GLdouble x = radius * cos(alpha) * cos(beta),
					y = radius * sin(alpha),
					z = radius * cos(alpha) * sin(beta);

				glVertex3d(x, y, z);
				glVertex3d(x + x / radius, y + y / radius, z + z / radius);
			}
		glEnd();
		glEnable(GL_LIGHTING);
	}
}

void CGLRenderer::PrepareLighting()
{
	// Setting light model.
	GLfloat lightModelAmbient[] = { .15f, .15f, .15f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);

	// Setting white directional light.
	GLfloat directionalLightAmbient[] = { .3f, .3f, .3f, 0.f };
	GLfloat directionalLightDiffuse[] = { .8f, .8f, .8f, 0.f };
	GLfloat directionalLightSpecular[] = { .2f, .2f, .2f, 0.f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, directionalLightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directionalLightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, directionalLightSpecular);

	// Setting red positional light.
	GLfloat redLightAmbient[] = { .3f, 0.f, 0.f, 1.f };
	GLfloat redLightDiffuse[] = { 1.f, 0.f, 0.f, 1.f };
	GLfloat redLightSpecular[] = { .2f, 0.f, 0.f, 1.f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, redLightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, redLightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, redLightSpecular);

	// Setting green positional light.
	GLfloat greenLightAmbient[] = { 0.f, .3f, 0.f, 1.f };
	GLfloat greenLightDiffuse[] = { 0.f, 1.f, 0.f, 1.f };
	GLfloat greenLightSpecular[] = { 0.f, .2f, 0.f, 1.f };

	glLightfv(GL_LIGHT2, GL_AMBIENT, greenLightAmbient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, greenLightDiffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, greenLightSpecular);

	// Setting blue positional light.
	GLfloat blueLightAmbient[] = { 0.f, 0.f, .3f, 1.f };
	GLfloat blueLightDiffuse[] = { 0.f, 0.f, 1.f, 1.f };
	GLfloat blueLightSpecular[] = { 0.f, 0.f, .2f, 1.f };

	glLightfv(GL_LIGHT3, GL_AMBIENT, blueLightAmbient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, blueLightDiffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, blueLightSpecular);
}

void CGLRenderer::PositionLighting()
{
	// Positioning directional white light.
	GLfloat directionalLightPosition[] = { .5f * BOX_SIZE, .5f * BOX_SIZE, .5f * BOX_SIZE, 0.f };
	glLightfv(GL_LIGHT0, GL_POSITION, directionalLightPosition);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.f);
	glEnable(GL_LIGHT0);

	// Setting up and positioning red light.
	GLfloat redLightPosition[] = { .0f * BOX_SIZE, .5f * BOX_SIZE, 1.f * BOX_SIZE, 1.f };
	GLfloat redLightDirection[] = { 0.f, 0.f, -1.f };
	glLightfv(GL_LIGHT1, GL_POSITION, redLightPosition);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, redLightDirection);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.f);
	m_redLightActive ? glEnable(GL_LIGHT1) : glDisable(GL_LIGHT1);
	if (m_redLightActive)
	{
		CGLMaterial materialRedLight;
		materialRedLight.SetAmbient(1.f, 0.f, 0.f, 1.f);
		materialRedLight.SetDiffuse(1.f, 0.f, 0.f, 1.f);
		materialRedLight.SetEmission(1.f, 0.f, 0.f, 1.f);
		glPushMatrix();
		{
			glTranslatef(.0f * BOX_SIZE, .5f * BOX_SIZE, 1.f * BOX_SIZE);
			glRotatef(-90.f, 1.f, 0.f, 0.f);
			materialRedLight.Select();
			DrawHalfSphere(4., 30);
		}
		glPopMatrix();
	}

	// Setting up and positioning light.
	GLfloat greenLightPosition[] = { 1.f * BOX_SIZE, .5f * BOX_SIZE, 0.f * BOX_SIZE, 1.f };
	GLfloat greenLightDirection[] = { -1.f, 0.f, 0.f };
	glLightfv(GL_LIGHT2, GL_POSITION, greenLightPosition);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, greenLightDirection);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.f);
	m_greenLightActive ? glEnable(GL_LIGHT2) : glDisable(GL_LIGHT2);
	if (m_greenLightActive)
	{
		CGLMaterial materialGreenLight;
		materialGreenLight.SetAmbient(0.f, 1.f, 0.f, 1.f);
		materialGreenLight.SetDiffuse(0.f, 1.f, 0.f, 1.f);
		materialGreenLight.SetEmission(0.f, 1.f, 0.f, 1.f);
		glPushMatrix();
		{
			glTranslatef(1.f * BOX_SIZE, .5f * BOX_SIZE, 0.f * BOX_SIZE);
			glRotatef(90.f, 0.f, 0.f, 1.f);
			materialGreenLight.Select();
			DrawHalfSphere(4., 30);
		}
		glPopMatrix();
	}

	// Setting up and positioning blue light.
	GLfloat blueLightPosition[] = { 0.f * BOX_SIZE, 1.f * BOX_SIZE, 0.f * BOX_SIZE, 1.f };
	GLfloat blueLightDirection[] = { 0.f, -1.f, 0.f };
	glLightfv(GL_LIGHT3, GL_POSITION, blueLightPosition);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, blueLightDirection);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.f);
	m_blueLightActive ? glEnable(GL_LIGHT3) : glDisable(GL_LIGHT3);
	if (m_blueLightActive)
	{
		CGLMaterial materialBlueLight;
		materialBlueLight.SetAmbient(0.f, 0.f, 1.f, 1.f);
		materialBlueLight.SetDiffuse(0.f, 0.f, 1.f, 1.f);
		materialBlueLight.SetEmission(0.f, 0.f, 1.f, 1.f);
		glPushMatrix();
		{
			glTranslatef(0.f * BOX_SIZE, 2.f * BOX_SIZE, 0.f * BOX_SIZE);
			glRotatef(180.f, 1.f, 0.f, 0.f);
			materialBlueLight.Select();
			DrawHalfSphere(4., 30);
		}
		glPopMatrix();
	}

}

void CGLRenderer::DrawBox()
{
	m_materialLightGrey.Select();

	glBegin(GL_TRIANGLE_STRIP);
	{
		glNormal3d(0., 1., 0.);
		glVertex3d(-BOX_SIZE, 0., -BOX_SIZE);
		glVertex3d(BOX_SIZE, 0., -BOX_SIZE);

		glVertex3d(-BOX_SIZE, BOX_SIZE * 2., -BOX_SIZE);
		glVertex3d(BOX_SIZE, BOX_SIZE * 2., -BOX_SIZE);

		glNormal3d(0., 0., -1.);
		glVertex3d(-BOX_SIZE, BOX_SIZE * 2., BOX_SIZE);
		glVertex3d(BOX_SIZE, BOX_SIZE * 2., BOX_SIZE);

		glNormal3d(0., -1., 0.);
		glVertex3d(-BOX_SIZE, 0., BOX_SIZE);
		glVertex3d(BOX_SIZE, 0., BOX_SIZE);

		glNormal3d(0., 0., 1.);
		glVertex3d(-BOX_SIZE, 0., -BOX_SIZE);
		glVertex3d(BOX_SIZE, 0., -BOX_SIZE);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(1., 0., 0.);
		glVertex3d(-BOX_SIZE, 0., -BOX_SIZE);
		glVertex3d(-BOX_SIZE, BOX_SIZE * 2., -BOX_SIZE);
		glVertex3d(-BOX_SIZE, BOX_SIZE * 2., BOX_SIZE);
		glVertex3d(-BOX_SIZE, 0., BOX_SIZE);

		glNormal3d(-1., 0., 0.);
		glVertex3d(BOX_SIZE, 0., BOX_SIZE);
		glVertex3d(BOX_SIZE, BOX_SIZE * 2., BOX_SIZE);
		glVertex3d(BOX_SIZE, BOX_SIZE * 2., -BOX_SIZE);
		glVertex3d(BOX_SIZE, 0., -BOX_SIZE);
	}
	glEnd();
}

void CGLRenderer::DrawStand()
{
	m_materialLightGrey.Select();
	DrawHalfSphere(2. * GRID_UNIT_SIZE, 30);

	glTranslated(0., 2. * GRID_UNIT_SIZE - 1.5, 0.);
	DrawTruncatedCone(2. * GRID_UNIT_SIZE, GRID_UNIT_SIZE, GRID_UNIT_SIZE, 12);

	glRotated(45., 0., 1., 0.);
	glTranslated(0., 2. * GRID_UNIT_SIZE, 0.);
	DrawTruncatedCone(GRID_UNIT_SIZE, 4. * GRID_UNIT_SIZE, 4. * GRID_UNIT_SIZE, 4);

	glNormal3d(0., -1., 0.);
	glBegin(GL_TRIANGLE_FAN);
	for (GLdouble i = 0; i < Deg2Rad(360.); i += Deg2Rad(90.))
		glVertex3d(4. * GRID_UNIT_SIZE * cos(i), 0., 4. * GRID_UNIT_SIZE * sin(i));
	glEnd();

	glNormal3d(0., 1., 0.);
	glTranslated(0., GRID_UNIT_SIZE, 0.);
	glBegin(GL_TRIANGLE_FAN);
	for (GLdouble i = Deg2Rad(360.); i > 0.; i -= Deg2Rad(90.))
		glVertex3d(4. * GRID_UNIT_SIZE * cos(i), 0., 4. * GRID_UNIT_SIZE * sin(i));
	glEnd();
}

void CGLRenderer::DrawVasePart(VasePartWidth bottom, VasePartWidth top)
{
	DrawTruncatedCone(PART_HEIGHT, (GLdouble)bottom * PART_HEIGHT, (GLdouble)top * PART_HEIGHT, 30);
}

void CGLRenderer::DrawVase()
{
	glPushMatrix();
	{
		m_materialCyan.Select();
		DrawVasePart(VasePartWidth::BIG, VasePartWidth::MEDIUM);

		m_materialPink.Select();
		glTranslatef(0.f, PART_HEIGHT, 0.f);
		DrawVasePart(VasePartWidth::MEDIUM, VasePartWidth::SMALL);

		m_materialCyan.Select();
		glTranslatef(0.f, PART_HEIGHT, 0.f);
		DrawVasePart(VasePartWidth::SMALL, VasePartWidth::SMALL);

		m_materialPink.Select();
		glTranslatef(0.f, PART_HEIGHT, 0.f);
		DrawVasePart(VasePartWidth::SMALL, VasePartWidth::SMALL);

		m_materialCyan.Select();
		glTranslatef(0.f, PART_HEIGHT, 0.f);
		DrawVasePart(VasePartWidth::SMALL, VasePartWidth::MEDIUM);

		m_materialPink.Select();
		glTranslatef(0.f, PART_HEIGHT, 0.f);
		DrawVasePart(VasePartWidth::MEDIUM, VasePartWidth::SMALL);

		m_materialCyan.Select();
		glTranslatef(0.f, PART_HEIGHT, 0.f);
		DrawVasePart(VasePartWidth::SMALL, VasePartWidth::MEDIUM);

		m_materialPink.Select();
		glTranslatef(0.f, PART_HEIGHT, 0.f);
		DrawVasePart(VasePartWidth::MEDIUM, VasePartWidth::BIG);

		m_materialCyan.Select();
		glTranslatef(0.f, PART_HEIGHT, 0.f);
		DrawVasePart(VasePartWidth::BIG, VasePartWidth::MEDIUM);

		m_materialPink.Select();
		glTranslatef(0.f, PART_HEIGHT, 0.f);
		DrawVasePart(VasePartWidth::MEDIUM, VasePartWidth::SMALL);

		m_materialCyan.Select();
		glTranslatef(0.f, PART_HEIGHT, 0.f);
		DrawVasePart(VasePartWidth::SMALL, VasePartWidth::MEDIUM);

		m_materialPink.Select();
		glTranslatef(0.f, PART_HEIGHT, 0.f);
		DrawVasePart(VasePartWidth::MEDIUM, VasePartWidth::SMALL);

		m_materialCyan.Select();
		glTranslatef(0.f, PART_HEIGHT, 0.f);
		DrawVasePart(VasePartWidth::SMALL, VasePartWidth::MEDIUM);

		m_materialPink.Select();
		glTranslatef(0.f, PART_HEIGHT, 0.f);
		DrawVasePart(VasePartWidth::MEDIUM, VasePartWidth::BIG);
	}
	glPopMatrix();
}

