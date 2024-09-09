#include "StdAfx.h"
#include "GLRenderer.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	m_viewRadius = SCENE_SIZE * 2, m_viewAlpha = 0., m_viewBeta = Deg2Rad(90.);
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
		DrawAxes(3.f, SCENE_SIZE);

	DrawGrid(gridUnitSize, m_oGridXY, m_oGridXZ, m_oGridYZ);

	glPushMatrix();
	{
		glTranslatef(2.f * gridUnitSize, 2.f * gridUnitSize, 2.f * gridUnitSize);
		glRotatef(45., 0.f, 0.f, 1.f);
		DrawCube(2. * gridUnitSize, .5f, .5f, .5f);
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
	gluPerspective(45., (double)w / h, 1., 100.);
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
	m_eyeX = m_viewRadius * cos(m_viewAlpha) * cos(m_viewBeta);
	m_eyeY = m_viewRadius * sin(m_viewAlpha);
	m_eyeZ = m_viewRadius * cos(m_viewAlpha) * sin(m_viewBeta);
}

void CGLRenderer::DrawAxes(GLfloat width, GLfloat length)
{
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
	glLineWidth(1.);
}

void CGLRenderer::DrawGrid(GLdouble unitSize, GLboolean oXY, GLboolean oXZ, GLboolean oYZ)
{
	glColor3f(0.f, 0.f, 0.f);
	glLineWidth(1.f);
	glBegin(GL_LINES);
	for (double i = -SCENE_SIZE / 2; i <= SCENE_SIZE / 2; i += unitSize)
	{
		if (oXY)
		{
			glVertex3f(i, -SCENE_SIZE / 2, 0.f);
			glVertex3f(i, SCENE_SIZE / 2, 0.f);
			glVertex3f(-SCENE_SIZE / 2, i, 0.f);
			glVertex3f(SCENE_SIZE / 2, i, 0.f);
		}

		if (oXZ)
		{
			glVertex3f(i, 0.f, -SCENE_SIZE / 2);
			glVertex3f(i, 0.f, SCENE_SIZE / 2);
			glVertex3f(-SCENE_SIZE / 2, 0.f, i);
			glVertex3f(SCENE_SIZE / 2, 0.f, i);
		}

		if (oYZ)
		{
			glVertex3f(0.f, i, -SCENE_SIZE / 2);
			glVertex3f(0.f, i, SCENE_SIZE / 2);
			glVertex3f(0.f, -SCENE_SIZE / 2, i);
			glVertex3f(0.f, SCENE_SIZE / 2, i);
		}
	}
	glEnd();
}

void CGLRenderer::PrepareCube(GLdouble a)
{
	cubeVertices[0] = -a / 2, cubeVertices[1] = -a / 2, cubeVertices[2] = a / 2;		// vertex 0
	cubeVertices[3] = a / 2, cubeVertices[4] = -a / 2, cubeVertices[5] = a / 2;			// vertex 1
	cubeVertices[6] = a / 2, cubeVertices[7] = a / 2, cubeVertices[8] = a / 2;			// vertex 2
	cubeVertices[9] = -a / 2, cubeVertices[10] = a / 2, cubeVertices[11] = a / 2;		// vertex 3
	cubeVertices[12] = -a / 2, cubeVertices[13] = -a / 2, cubeVertices[14] = -a / 2;	// vertex 4
	cubeVertices[15] = a / 2, cubeVertices[16] = -a / 2, cubeVertices[17] = -a / 2;		// vertex 5
	cubeVertices[18] = a / 2, cubeVertices[19] = a / 2, cubeVertices[20] = -a / 2;		// vertex 6
	cubeVertices[21] = -a / 2, cubeVertices[22] = a / 2, cubeVertices[23] = -a / 2;		// vertex 7

	cubeIndices[0] = 0, cubeIndices[1] = 1, cubeIndices[2] = 2, cubeIndices[3] = 3;		// quad 0
	cubeIndices[4] = 1, cubeIndices[5] = 5, cubeIndices[6] = 6, cubeIndices[7] = 2;		// quad 1
	cubeIndices[8] = 5, cubeIndices[9] = 4, cubeIndices[10] = 7, cubeIndices[11] = 6;	// quad 2
	cubeIndices[12] = 4, cubeIndices[13] = 0, cubeIndices[14] = 3, cubeIndices[15] = 7;	// quad 3
	cubeIndices[20] = 2, cubeIndices[21] = 6, cubeIndices[22] = 7, cubeIndices[23] = 3;	// quad 4
	cubeIndices[16] = 1, cubeIndices[17] = 0, cubeIndices[18] = 4, cubeIndices[19] = 5;	// quad 5
}

void CGLRenderer::DrawCube(GLdouble size, GLclampf r, GLclampf g, GLclampf b)
{
	PrepareCube(size);
	// Define pointers.
	glVertexPointer(3, GL_DOUBLE, 0, cubeVertices);

	// Enable pointer.
	glEnableClientState(GL_VERTEX_ARRAY);

	// Draw elements.
	glColor3f(r, g, b);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);

	// Disable pointers.
	glDisableClientState(GL_VERTEX_ARRAY);
}
