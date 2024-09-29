#include "StdAfx.h"
#include "GLRenderer.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
	: m_hrc{ NULL },
	SCENE_SIZE{ 100.f }, GRID_UNIT_SIZE{ 2.5f },
	MAX_RADIUS{ 150.f }, MIN_RADIUS{ 30.f }, MAX_ALPHA{ 90.f }, MIN_ALPHA{ -90.f },
	m_viewRadius{ 80.f }, m_viewAlpha{ 20.f }, m_viewBeta{ 90.f },
	m_eyeX{ -1.f }, m_eyeY{ -1.f }, m_eyeZ{ -1.f },
	m_axesActive{ TRUE }, m_gridActive{ FALSE },
	GRID_NUM_UNITS_TEX_TRUCK{ 16.f }, GRID_UNIT_SIZE_TEX_TRUCK{ 1.f / 16.f }
{
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
	// Basic setup - depth, lightning, textures.
	glClearColor(.8f, .8f, .8f, 0.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE); // Performance hit, be careful!

	// Loading texture.
	CGLTexture::PrepareTexturing(TRUE);
	m_texGround.LoadFromFile(CString("ground.jpeg"));
	m_texTruck.LoadFromFile(CString("truck.png"));

	// Rotating texture for easier calculation of coordinates.
	glMatrixMode(GL_TEXTURE);
	m_texTruck.Select();
	glRotatef(180.f, 1.f, 0.f, 0.f);
	CGLTexture::DeselectTexture();
	glMatrixMode(GL_MODELVIEW);

	// Setting up light model and directional light.
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

	SetLighting();

	if (m_axesActive)
		DrawAxes(3.f, SCENE_SIZE);

	if (m_gridActive)
		DrawGrid(SCENE_SIZE, (GLsizei)(SCENE_SIZE / GRID_UNIT_SIZE));

	// Light is fucked up, you apparently cannot fucking do reflection.

	glPushMatrix();
	{
		DrawGround();
		DrawTruck();

		glTranslatef(1.5f * GRID_UNIT_SIZE, 6.5f * GRID_UNIT_SIZE, 0.f);
		glScalef(2.f, 1.f, 1.f);
		DrawCargo();
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
	gluPerspective(45., (GLdouble)w / h, 1., 200.);
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
	CONST GLfloat alphaRad = Deg2Rad(m_viewAlpha);
	CONST GLfloat betaRad = Deg2Rad(m_viewBeta);

	m_eyeX = m_viewRadius * cosf(alphaRad) * cosf(betaRad);
	m_eyeY = m_viewRadius * sinf(alphaRad);
	m_eyeZ = m_viewRadius * cosf(alphaRad) * sinf(betaRad);
}

void CGLRenderer::ChangeView(FLOAT deltaRadius, FLOAT deltaAlpha, FLOAT deltaBeta)
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

void CGLRenderer::DrawAxes(GLfloat width, GLfloat length)
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

void CGLRenderer::DrawGrid(GLfloat size, GLsizei nSeg)
{
	CONST GLfloat gridUnitSize = size / (GLfloat)nSeg;
	glDisable(GL_LIGHTING);
	glLineWidth(1.f);
	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_LINES);
	{
		for (GLfloat i = -size / 2.f; i <= size / 2.f; i += gridUnitSize)
			glVertex3f(i, 0.f, -size / 2.f), glVertex3f(i, 0.f, size / 2.f);

		for (GLfloat i = -size / 2.f; i <= size / 2.f; i += gridUnitSize)
			glVertex3f(-size / 2.f, 0.f, i), glVertex3f(size / 2.f, 0.f, i);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::PrepareLighting()
{
	GLfloat lightModelAmbient[] = { .8f, .8f , .8f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);

	GLfloat directionalColor[] = { 122.f / 255.f, 122.f / 255.f, 83.f / 255.f };

	GLfloat directionalAmbient[] = { .8f, .8f , .8f, 0.f };
	GLfloat directionalDiffuse[] = { 1.f,  1.f, 1.f, 0.f };
	GLfloat directionalSpecular[] = { .2f, .2f , .2f, 0.f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, directionalAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directionalDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, directionalSpecular);
}

void CGLRenderer::SetLighting()
{
	GLfloat position[] = { SCENE_SIZE, SCENE_SIZE, SCENE_SIZE, 0.f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
}

void CGLRenderer::DrawGround()
{
	GLfloat repeatFactor = 2.f;
	m_texGround.Select();
	glBegin(GL_QUADS);
	{
		glNormal3f(0.f, 1.f, 0.f);

		glTexCoord2f(0, 0);
		glVertex3f(-SCENE_SIZE / 2.f, 0.f, SCENE_SIZE / 2.f);

		glTexCoord2f(repeatFactor, 0);
		glVertex3f(SCENE_SIZE / 2.f, 0.f, SCENE_SIZE / 2.f);

		glTexCoord2f(repeatFactor, repeatFactor);
		glVertex3f(SCENE_SIZE / 2.f, 0.f, -SCENE_SIZE / 2.f);

		glTexCoord2f(0, repeatFactor);
		glVertex3f(-SCENE_SIZE / 2.f, 0.f, -SCENE_SIZE / 2.f);
	}
	glEnd();
	CGLTexture::DeselectTexture();
}

void CGLRenderer::DrawTruckBodyTexture()
{
	glPushMatrix();

	m_texTruck.Select();

	glNormal3f(0.f, 1.f, 0.f);

	glBegin(GL_TRIANGLES);
	{
		// -- Triangle up left.
		glTexCoord2f(0.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 2.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(0.f * GRID_UNIT_SIZE, 0.f, 2.f * GRID_UNIT_SIZE);

		glTexCoord2f(2.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 2.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(2.f * GRID_UNIT_SIZE, 0.f, 2.f * GRID_UNIT_SIZE);

		glTexCoord2f(2.f * GRID_UNIT_SIZE_TEX_TRUCK, GRID_NUM_UNITS_TEX_TRUCK * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(2.f * GRID_UNIT_SIZE, 0.f, 0.f * GRID_UNIT_SIZE);

		// -- Triangle left wheel left.
		glTexCoord2f(1.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(1.f * GRID_UNIT_SIZE, 0.f, 5.f * GRID_UNIT_SIZE);

		glTexCoord2f(1.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 6.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(1.f * GRID_UNIT_SIZE, 0.f, 6.f * GRID_UNIT_SIZE);

		glTexCoord2f(2.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(2.f * GRID_UNIT_SIZE, 0.f, 5.f * GRID_UNIT_SIZE);

		// -- Triangle left wheel right.
		glTexCoord2f(3.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(3.f * GRID_UNIT_SIZE, 0.f, 5.f * GRID_UNIT_SIZE);

		glTexCoord2f(4.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 6.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(4.f * GRID_UNIT_SIZE, 0.f, 6.f * GRID_UNIT_SIZE);

		glTexCoord2f(4.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(4.f * GRID_UNIT_SIZE, 0.f, 5.f * GRID_UNIT_SIZE);

		// -- Triangle right wheel left.
		glTexCoord2f(6.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(6.f * GRID_UNIT_SIZE, 0.f, 5.f * GRID_UNIT_SIZE);

		glTexCoord2f(6.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 6.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(6.f * GRID_UNIT_SIZE, 0.f, 6.f * GRID_UNIT_SIZE);

		glTexCoord2f(7.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(7.f * GRID_UNIT_SIZE, 0.f, 5.f * GRID_UNIT_SIZE);

		// -- Triangle right wheel right.
		glTexCoord2f(8.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(8.f * GRID_UNIT_SIZE, 0.f, 5.f * GRID_UNIT_SIZE);

		glTexCoord2f(9.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 6.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(9.f * GRID_UNIT_SIZE, 0.f, 6.f * GRID_UNIT_SIZE);

		glTexCoord2f(9.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(9.f * GRID_UNIT_SIZE, 0.f, 5.f * GRID_UNIT_SIZE);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		// -- Quad left to triangle.
		glTexCoord2f(2.f * GRID_UNIT_SIZE_TEX_TRUCK, GRID_NUM_UNITS_TEX_TRUCK * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(2.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE);

		glTexCoord2f(2.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 2.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(2.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 2.f * GRID_UNIT_SIZE);

		glTexCoord2f(4.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 2.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(4.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 2.f * GRID_UNIT_SIZE);

		glTexCoord2f(4.f * GRID_UNIT_SIZE_TEX_TRUCK, GRID_NUM_UNITS_TEX_TRUCK * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(4.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE);

		// -- Quad beneath previous triangle and quad.
		glTexCoord2f(0.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 2.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(0.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 2.f * GRID_UNIT_SIZE);

		glTexCoord2f(0.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 4.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(0.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 4.f * GRID_UNIT_SIZE);

		glTexCoord2f(4.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 4.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(4.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 4.f * GRID_UNIT_SIZE);

		glTexCoord2f(4.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 2.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(4.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 2.f * GRID_UNIT_SIZE);

		// -- Long strip quad.
		glTexCoord2f(0.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 4.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(0.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 4.f * GRID_UNIT_SIZE);

		glTexCoord2f(0.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(0.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 5.f * GRID_UNIT_SIZE);

		glTexCoord2f((GRID_NUM_UNITS_TEX_TRUCK - 3.f) * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f((GRID_NUM_UNITS_TEX_TRUCK - 3.f) * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 5.f * GRID_UNIT_SIZE);

		glTexCoord2f((GRID_NUM_UNITS_TEX_TRUCK - 3.f) * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 4.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f((GRID_NUM_UNITS_TEX_TRUCK - 3.f) * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 4.f * GRID_UNIT_SIZE);

		// -- Little quad bottom left.
		glTexCoord2f(0.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(0.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 5.f * GRID_UNIT_SIZE);

		glTexCoord2f(0.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 7.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(0.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 7.f * GRID_UNIT_SIZE);

		glTexCoord2f(1.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 7.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(1.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 7.f * GRID_UNIT_SIZE);

		glTexCoord2f(1.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(1.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 5.f * GRID_UNIT_SIZE);

		// -- Medium quad bottom middle.
		glTexCoord2f(4.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(4.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 5.f * GRID_UNIT_SIZE);

		glTexCoord2f(4.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 7.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(4.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 7.f * GRID_UNIT_SIZE);

		glTexCoord2f(6.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 7.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(6.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 7.f * GRID_UNIT_SIZE);

		glTexCoord2f(6.f * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 5.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f(6.f * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 5.f * GRID_UNIT_SIZE);

		// -- Big quad bottom right.
		glTexCoord2f((GRID_NUM_UNITS_TEX_TRUCK - 7.f) * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 4.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f((GRID_NUM_UNITS_TEX_TRUCK - 7.f) * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 4.f * GRID_UNIT_SIZE);

		glTexCoord2f((GRID_NUM_UNITS_TEX_TRUCK - 7.f) * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 7.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f((GRID_NUM_UNITS_TEX_TRUCK - 7.f) * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 7.f * GRID_UNIT_SIZE);

		glTexCoord2f((GRID_NUM_UNITS_TEX_TRUCK - 3.f) * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 7.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f((GRID_NUM_UNITS_TEX_TRUCK - 3.f) * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 7.f * GRID_UNIT_SIZE);

		glTexCoord2f((GRID_NUM_UNITS_TEX_TRUCK - 3.f) * GRID_UNIT_SIZE_TEX_TRUCK, (GRID_NUM_UNITS_TEX_TRUCK - 4.f) * GRID_UNIT_SIZE_TEX_TRUCK);
		glVertex3f((GRID_NUM_UNITS_TEX_TRUCK - 3.f) * GRID_UNIT_SIZE, 0.f * GRID_UNIT_SIZE, 4.f * GRID_UNIT_SIZE);
	}
	glEnd();

	glPopMatrix();

	CGLTexture::DeselectTexture();
}

void CGLRenderer::DrawTruckBody()
{
	glPushMatrix();

	CONST GLfloat truckBodyWidth{ 3.f * GRID_UNIT_SIZE };

	glBegin(GL_TRIANGLE_STRIP);
	{
		// 1.
		glVertex3f(GRID_UNIT_SIZE * 2.f, 0.f, GRID_UNIT_SIZE * 0.f);
		glVertex3f(GRID_UNIT_SIZE * 2.f, truckBodyWidth, GRID_UNIT_SIZE * 0.f);

		// 2.
		glVertex3f(GRID_UNIT_SIZE * 0.f, 0.f, GRID_UNIT_SIZE * 2.f);
		glVertex3f(GRID_UNIT_SIZE * 0.f, truckBodyWidth, GRID_UNIT_SIZE * 2.f);

		// 3.
		glVertex3f(GRID_UNIT_SIZE * 0.f, 0.f, GRID_UNIT_SIZE * 7.f);
		glVertex3f(GRID_UNIT_SIZE * 0.f, truckBodyWidth, GRID_UNIT_SIZE * 7.f);

		// 4.
		glVertex3f(GRID_UNIT_SIZE * 1.f, 0.f, GRID_UNIT_SIZE * 7.f);
		glVertex3f(GRID_UNIT_SIZE * 1.f, truckBodyWidth, GRID_UNIT_SIZE * 7.f);

		// 5.
		glVertex3f(GRID_UNIT_SIZE * 1.f, 0.f, GRID_UNIT_SIZE * 6.f);
		glVertex3f(GRID_UNIT_SIZE * 1.f, truckBodyWidth, GRID_UNIT_SIZE * 6.f);

		// 6.
		glVertex3f(GRID_UNIT_SIZE * 2.f, 0.f, GRID_UNIT_SIZE * 5.f);
		glVertex3f(GRID_UNIT_SIZE * 2.f, truckBodyWidth, GRID_UNIT_SIZE * 5.f);

		// 7.
		glVertex3f(GRID_UNIT_SIZE * 3.f, 0.f, GRID_UNIT_SIZE * 5.f);
		glVertex3f(GRID_UNIT_SIZE * 3.f, truckBodyWidth, GRID_UNIT_SIZE * 5.f);

		// 8.
		glVertex3f(GRID_UNIT_SIZE * 4.f, 0.f, GRID_UNIT_SIZE * 6.f);
		glVertex3f(GRID_UNIT_SIZE * 4.f, truckBodyWidth, GRID_UNIT_SIZE * 6.f);

		// 9.
		glVertex3f(GRID_UNIT_SIZE * 4.f, 0.f, GRID_UNIT_SIZE * 7.f);
		glVertex3f(GRID_UNIT_SIZE * 4.f, truckBodyWidth, GRID_UNIT_SIZE * 7.f);

		// 10.
		glVertex3f(GRID_UNIT_SIZE * 6.f, 0.f, GRID_UNIT_SIZE * 7.f);
		glVertex3f(GRID_UNIT_SIZE * 6.f, truckBodyWidth, GRID_UNIT_SIZE * 7.f);

		// 11.
		glVertex3f(GRID_UNIT_SIZE * 6.f, 0.f, GRID_UNIT_SIZE * 6.f);
		glVertex3f(GRID_UNIT_SIZE * 6.f, truckBodyWidth, GRID_UNIT_SIZE * 6.f);

		// 12.
		glVertex3f(GRID_UNIT_SIZE * 7.f, 0.f, GRID_UNIT_SIZE * 5.f);
		glVertex3f(GRID_UNIT_SIZE * 7.f, truckBodyWidth, GRID_UNIT_SIZE * 5.f);

		// 13.
		glVertex3f(GRID_UNIT_SIZE * 8.f, 0.f, GRID_UNIT_SIZE * 5.f);
		glVertex3f(GRID_UNIT_SIZE * 8.f, truckBodyWidth, GRID_UNIT_SIZE * 5.f);

		// 14.
		glVertex3f(GRID_UNIT_SIZE * 9.f, 0.f, GRID_UNIT_SIZE * 6.f);
		glVertex3f(GRID_UNIT_SIZE * 9.f, truckBodyWidth, GRID_UNIT_SIZE * 6.f);

		// 15.
		glVertex3f(GRID_UNIT_SIZE * 9.f, 0.f, GRID_UNIT_SIZE * 7.f);
		glVertex3f(GRID_UNIT_SIZE * 9.f, truckBodyWidth, GRID_UNIT_SIZE * 7.f);

		// 16.
		glVertex3f(GRID_UNIT_SIZE * 13.f, 0.f, GRID_UNIT_SIZE * 7.f);
		glVertex3f(GRID_UNIT_SIZE * 13.f, truckBodyWidth, GRID_UNIT_SIZE * 7.f);

		// 17.
		glVertex3f(GRID_UNIT_SIZE * 13.f, 0.f, GRID_UNIT_SIZE * 4.f);
		glVertex3f(GRID_UNIT_SIZE * 13.f, truckBodyWidth, GRID_UNIT_SIZE * 4.f);

		glNormal3f(0.f, 1.f, 0.f);

		// 18.
		glVertex3f(GRID_UNIT_SIZE * 4.f, 0.f, GRID_UNIT_SIZE * 4.f);
		glVertex3f(GRID_UNIT_SIZE * 4.f, truckBodyWidth, GRID_UNIT_SIZE * 4.f);

		// 19.
		glVertex3f(GRID_UNIT_SIZE * 4.f, 0.f, GRID_UNIT_SIZE * 0.f);
		glVertex3f(GRID_UNIT_SIZE * 4.f, truckBodyWidth, GRID_UNIT_SIZE * 0.f);

		// 20.
		glVertex3f(GRID_UNIT_SIZE * 2.f, 0.f, GRID_UNIT_SIZE * 0.f);
		glVertex3f(GRID_UNIT_SIZE * 2.f, truckBodyWidth, GRID_UNIT_SIZE * 0.f);
	}
	glEnd();

	glTranslatef(0.f, truckBodyWidth, 0.f);
	DrawTruckBodyTexture();

	glPopMatrix();
}

void CGLRenderer::DrawTruckWheel()
{
	CONST GLfloat deltaAngleRad = Deg2Rad(360. / 30.);

	m_texTruck.Select();

	CONST GLfloat wheelRadiusTex{ 1.5f * GRID_UNIT_SIZE_TEX_TRUCK };
	CONST GLfloat wheelWidth{ 1.5f * GRID_UNIT_SIZE };
	CONST GLfloat wheelRadius{ 1.5f * GRID_UNIT_SIZE };

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.f, -1.f, 0.f);
	for (GLfloat angleRad = 0.f; angleRad < Deg2Rad(360.f); angleRad += deltaAngleRad)
	{
		glTexCoord2f(6.f * GRID_UNIT_SIZE_TEX_TRUCK + wheelRadiusTex * cosf(angleRad),
			(GRID_NUM_UNITS_TEX_TRUCK - 1.5f) * GRID_UNIT_SIZE_TEX_TRUCK + wheelRadiusTex * sinf(angleRad));
		glVertex3f(wheelRadius * cosf(angleRad), 0.f, wheelRadius * sinf(angleRad));
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.f, 1.f, 0.f);
	for (GLfloat angleRad = 0.f; angleRad < Deg2Rad(360.f); angleRad += deltaAngleRad)
	{
		glTexCoord2f(6.f * GRID_UNIT_SIZE_TEX_TRUCK + wheelRadiusTex * cosf(angleRad),
			(GRID_NUM_UNITS_TEX_TRUCK - 1.5f) * GRID_UNIT_SIZE_TEX_TRUCK + wheelRadiusTex * sinf(angleRad));
		glVertex3f(wheelRadius * cosf(angleRad), wheelWidth, wheelRadius * sinf(angleRad));
	}
	glEnd();

	CGLTexture::DeselectTexture();

	glBegin(GL_TRIANGLE_STRIP);
	for (GLfloat angleRad = 0.f; angleRad < Deg2Rad(360.f); angleRad += deltaAngleRad)
	{
		glNormal3f(cosf(angleRad), 0.f, sinf(angleRad));
		glTexCoord2f(6.f * GRID_UNIT_SIZE_TEX_TRUCK + wheelRadiusTex * cosf(angleRad),
			(GRID_NUM_UNITS_TEX_TRUCK - 1.5f) * GRID_UNIT_SIZE_TEX_TRUCK + wheelRadiusTex * sinf(angleRad));
		glVertex3f(1.5f * GRID_UNIT_SIZE * cosf(angleRad), 0.f, 1.5f * GRID_UNIT_SIZE * sinf(angleRad));
		glVertex3f(1.5f * GRID_UNIT_SIZE * cosf(angleRad), wheelWidth, 1.5f * GRID_UNIT_SIZE * sinf(angleRad));
	}
	glEnd();
}

void CGLRenderer::DrawTruckHalf()
{
	glPushMatrix();

	DrawTruckBody();

	glTranslatef(2.5f * GRID_UNIT_SIZE, 1.f * GRID_UNIT_SIZE, 7.f * GRID_UNIT_SIZE);
	DrawTruckWheel();

	glTranslatef(5.f * GRID_UNIT_SIZE, 0.f, 0.f);
	DrawTruckWheel();

	glPopMatrix();
}

void CGLRenderer::DrawTruck()
{
	glPushMatrix();

	CONST GLfloat truckHeight{ 8.5f * GRID_UNIT_SIZE }, truckWidth{ 13.f * GRID_UNIT_SIZE };

	glRotatef(90.f, 1.f, 0.f, 0.f);
	glTranslatef(-truckWidth / 2.f, 0.f, -truckHeight);
	DrawTruckHalf();

	glScalef(1.f, -1.f, 1.f);
	DrawTruckHalf();

	glPopMatrix();
}

void CGLRenderer::DrawCargo()
{
	CONST GLfloat deltaAngle{ Deg2Rad(360.f / 16.f) };
	CONST GLfloat cargoRadius{ 2.f * GRID_UNIT_SIZE };

	m_texTruck.Select();

	for (GLfloat betaRad = 0., texCoordX = 16.f; betaRad <= Deg2Rad(360.f); betaRad += deltaAngle, texCoordX -= 1.f)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (GLfloat alphaRad = Deg2Rad(90.f), texCoordY = 8.f; alphaRad >= Deg2Rad(-90.f) - deltaAngle; alphaRad -= deltaAngle, texCoordY -= 1.f)
			glNormal3f(cosf(alphaRad) * cosf(betaRad), sinf(alphaRad), cosf(alphaRad) * sinf(betaRad)),
			glTexCoord2f((texCoordX - 1.f) * GRID_UNIT_SIZE_TEX_TRUCK, texCoordY * GRID_UNIT_SIZE_TEX_TRUCK),
			glVertex3f(cargoRadius * cosf(alphaRad) * cosf(betaRad),
				cargoRadius * sinf(alphaRad),
				cargoRadius * cosf(alphaRad) * sinf(betaRad)),
			glNormal3f(cosf(alphaRad) * cosf(betaRad - deltaAngle), sinf(alphaRad), cosf(alphaRad) * sinf(betaRad - deltaAngle)),
			glTexCoord2f(texCoordX * GRID_UNIT_SIZE_TEX_TRUCK, texCoordY * GRID_UNIT_SIZE_TEX_TRUCK),
			glVertex3f(cargoRadius * cosf(alphaRad) * cosf(betaRad - deltaAngle),
				cargoRadius * sinf(alphaRad),
				cargoRadius * cosf(alphaRad) * sinf(betaRad - deltaAngle));
		glEnd();
	}

	CGLTexture::DeselectTexture();
}
