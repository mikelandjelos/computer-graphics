#include "StdAfx.h"
#include "GLRenderer.h"

CGLRenderer::CGLRenderer(void)
	: m_hrc{ NULL },
	SCENE_SIZE{ 80.f }, GRID_UNIT_SIZE{ 80.f / 20.f },
	m_axesActive{ TRUE },
	m_eyeX{ -1. }, m_eyeY{ -1. }, m_eyeZ{ -1. },
	m_viewRadius{ 50. }, m_viewAlpha{ 20. }, m_viewBeta{ 90. },
	MAX_RADIUS{ 160. }, MIN_RADIUS{ 20. },
	MAX_ALPHA{ 90. }, MIN_ALPHA{ -90. }
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
	glClearColor(0.8f, 0.8f, 0.8f, 0.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	CGLTexture::PrepareTexturing(TRUE);

	m_matRedShiny.SetAmbient(0.2f, 0.f, 0.f, 0.f);
	m_matRedShiny.SetDiffuse(1.f, 0.f, 0.f, 0.f);
	m_matRedShiny.SetSpecular(0.9f, 0.f, 0.f, 0.f);
	m_matRedShiny.SetShininess(120.f);

	m_texGround.LoadFromFile(L"ground.jpeg");
	m_texTruck.LoadFromFile(L"truck.png");

	glMatrixMode(GL_TEXTURE);
	m_texTruck.Select();
	glRotatef(180.f, 1.f, 0.f, 0.f);
	CGLTexture::DeselectTexture();
	glMatrixMode(GL_MODELVIEW);

	PrepareLight();
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

	SetLight();

	if (m_axesActive)
		DrawAxes(3.f, SCENE_SIZE / 2);

	glPushMatrix();
	{
		DrawGround();
		glTranslatef(-5 * GRID_UNIT_SIZE, 0.f, 0.f);
		DrawMaterial();
		glTranslatef(10 * GRID_UNIT_SIZE, 0.f, 0.f);
		DrawTextured();
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
	gluPerspective(50.f, (GLdouble)w / h, 1., 200.);
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

void CGLRenderer::DrawAxes(GLfloat width, GLfloat lenght)
{
	glDisable(GL_LIGHTING);
	glLineWidth(width);
	glBegin(GL_LINES);
	{
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(lenght, 0.f, 0.f);

		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, lenght, 0.f);

		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, lenght);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::SetView()
{
	CONST GLdouble alphaRad = Deg2Rad(m_viewAlpha);
	CONST GLdouble betaRad = Deg2Rad(m_viewBeta);

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

void CGLRenderer::PrepareLight()
{
	// Setting up light model.
	GLfloat lmAmbient[] = { .3f, .3f, .3f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmAmbient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	GLfloat lightAmbient[] = { .7f, .7f, .7f, .1f };
	GLfloat lightDiffuse[] = { .9f, .9f, .9f, .1f };
	GLfloat lightSpecular[] = { .1f, .1f, .1f, .1f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}

void CGLRenderer::SetLight()
{
	GLfloat lightPosition[] = { SCENE_SIZE / 2.f, SCENE_SIZE / 2.f, SCENE_SIZE / 2.f, 1.f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0);
}

void CGLRenderer::DrawGround()
{
	m_texGround.Select();

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.f, 1.f);
		glVertex3f(-SCENE_SIZE / 2.f, 0.f, -SCENE_SIZE / 2.f);
		glTexCoord2f(0.f, 0.f);
		glVertex3f(-SCENE_SIZE / 2.f, 0.f, SCENE_SIZE / 2.f);
		glTexCoord2f(1.f, 0.f);
		glVertex3f(SCENE_SIZE / 2.f, 0.f, SCENE_SIZE / 2.f);
		glTexCoord2f(1.f, 1.f);
		glVertex3f(SCENE_SIZE / 2.f, 0.f, -SCENE_SIZE / 2.f);
	}
	glEnd();

	CGLTexture::DeselectTexture();
}

void CGLRenderer::DrawMaterial()
{
	m_matRedShiny.Select();

	CONST GLfloat deltaAngleRad = Deg2Rad(360. / 16.);

	CONST GLfloat radius = 3 * GRID_UNIT_SIZE;
	CONST GLfloat height = 3 * GRID_UNIT_SIZE;

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.f, -1.f, 0.f);
	for (GLfloat angleRad = 0.; angleRad <= Deg2Rad(360.); angleRad += deltaAngleRad)
		glVertex3f(radius * cosf(angleRad), 0.f, radius * sinf(angleRad));
	glEnd();

	CONST GLfloat r = radius / 2;
	CONST GLfloat L = sqrtf(r * r + height * height);
	CONST GLfloat ny = r / L;
	CONST GLfloat nr = height / L;

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, height, 0.f);
	for (GLfloat angleRad = Deg2Rad(360.) + deltaAngleRad; angleRad >= 0.f; angleRad -= deltaAngleRad)
		glNormal3f(nr * cosf(angleRad), ny, nr * sinf(angleRad)),
		glVertex3f(radius * cosf(angleRad), 0.f, radius * sinf(angleRad));
	glEnd();

	m_matDefault.Select();
}

void CGLRenderer::DrawTextured()
{
	CONST GLfloat deltaAngleRad = Deg2Rad(360. / 16.);

	CONST GLfloat radius = 3 * GRID_UNIT_SIZE;
	CONST GLfloat height = 3 * GRID_UNIT_SIZE;

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.f, -1.f, 0.f);
	for (GLfloat angleRad = 0.; angleRad <= Deg2Rad(360.); angleRad += deltaAngleRad)
		glVertex3f(radius * cosf(angleRad), 0.f, radius * sinf(angleRad));
	glEnd();

	CONST GLfloat r = radius / 2;
	CONST GLfloat L = sqrtf(r * r + height * height);
	CONST GLfloat ny = r / L;
	CONST GLfloat nr = height / L;

	m_texTruck.Select();

	CONST GLfloat gridUnitTex = { 1.f / 16.f };

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2f(8.f * gridUnitTex, 8.f * gridUnitTex);
	glVertex3f(0.f, height, 0.f);
	for (GLfloat angleRad = Deg2Rad(360.) + deltaAngleRad, i = 0.f; angleRad >= 0.f; angleRad -= deltaAngleRad, i += 1.f)
		glNormal3f(nr * cosf(angleRad), ny, nr * sinf(angleRad)),
		glTexCoord2f(i * gridUnitTex, 0.f * gridUnitTex),
		glVertex3f(radius * cosf(angleRad), 0.f, radius * sinf(angleRad));
	glEnd();

	CGLTexture::DeselectTexture();
}
