
// CurvedTextView.cpp : implementation of the CCurvedTextView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CurvedText.h"
#endif

#include "CurvedTextDoc.h"
#include "CurvedTextView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _USE_MATH_DEFINES
#include "math.h"

#define DEG2RAD(fDegrees) (fDegrees * M_PI / 180.f)

// CCurvedTextView

IMPLEMENT_DYNCREATE(CCurvedTextView, CView)

BEGIN_MESSAGE_MAP(CCurvedTextView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CCurvedTextView construction/destruction

CCurvedTextView::CCurvedTextView() noexcept
	: GRID_UNIT_SIZE{ 500 / 20 }, m_gridActive{ TRUE },
	m_translating{ FALSE }, m_scalingFactor{ 1.f }, m_origin{ 0, 0 }
{
}

void CCurvedTextView::DrawCurvedText(CDC* pDC, CString text)
{
	CFont font;

	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	LOGFONT lf;

	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 20;
	wcscpy_s(lf.lfFaceName, 32, L"Arial");

	for (int i = 0; i < text.GetLength(); i++)
	{
		// lf.lfEscapement = i;
		font.CreateFontIndirect(&lf);
		CFont* oldFont = pDC->SelectObject(&font);
		pDC->TextOutW(m_clientRect.Width() / 2 + i * 10 * cos(i * 50),
			m_clientRect.Height() / 2 + i * 10 * sin(i * 50),
			CString{ text.GetAt(i) });
		pDC->SelectObject(oldFont);
		font.DeleteObject();
	}

	pDC->SetWorldTransform(&oldWorldTransform);

	pDC->SetBkMode(oldBkMode);
}

void CCurvedTextView::Translate(CDC* pDC, FLOAT dX, FLOAT dY, DWORD mode)
{
	XFORM translationMatrix{
		1.f, 0.f,
		0.f, 1.f,
		dX, dY,
	};
	pDC->ModifyWorldTransform(&translationMatrix, mode);
}

void CCurvedTextView::Rotate(CDC* pDC, FLOAT angle, DWORD mode)
{
	XFORM rotationMatrix{
		cosf(angle), sinf(angle),
		-sinf(angle), cosf(angle),
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&rotationMatrix, mode);
}

void CCurvedTextView::Scale(CDC* pDC, FLOAT sX, FLOAT sY, DWORD mode)
{
	XFORM scalingMatrix{
		sX, 0.f,
		0.f, sY,
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&scalingMatrix, mode);
}

void CCurvedTextView::DrawGrid(CDC* pDC, int gridUnitSize)
{
	for (int i = gridUnitSize; i < m_clientRect.Width(); i += gridUnitSize)
		pDC->MoveTo(i, 0), pDC->LineTo(i, m_clientRect.Height());

	for (int i = gridUnitSize; i < m_clientRect.Height(); i += gridUnitSize)
		pDC->MoveTo(0, i), pDC->LineTo(m_clientRect.Width(), i);
}

CCurvedTextView::~CCurvedTextView()
{
}

BOOL CCurvedTextView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCurvedTextView drawing

void CCurvedTextView::OnDraw(CDC* pDC)
{
	CCurvedTextDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CString text{ L"Mihajlo has been learning computer graphics!" };

	int oldGraphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);

	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	Translate(pDC, m_origin.x, m_origin.y);
	Scale(pDC, m_scalingFactor, m_scalingFactor);

	DrawCurvedText(pDC, text);

	pDC->SetWorldTransform(&oldWorldTransform);
	pDC->SetGraphicsMode(oldGraphicsMode);

	if (m_gridActive)
		DrawGrid(pDC, GRID_UNIT_SIZE);
}


// CCurvedTextView printing

BOOL CCurvedTextView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCurvedTextView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCurvedTextView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CCurvedTextView diagnostics

#ifdef _DEBUG
void CCurvedTextView::AssertValid() const
{
	CView::AssertValid();
}

void CCurvedTextView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCurvedTextDoc* CCurvedTextView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCurvedTextDoc)));
	return (CCurvedTextDoc*)m_pDocument;
}
#endif //_DEBUG


// CCurvedTextView message handlers


void CCurvedTextView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CONST INT translationUnit = GRID_UNIT_SIZE;

	switch (nChar)
	{
	case 'G':
		m_gridActive = !m_gridActive, Invalidate();
		break;
	default:
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CCurvedTextView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	GetClientRect(&m_clientRect);
}


void CCurvedTextView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_translating = TRUE;
	CView::OnLButtonDown(nFlags, point);
}


void CCurvedTextView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_translating = FALSE;
	CView::OnLButtonUp(nFlags, point);
}


BOOL CCurvedTextView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_scalingFactor += zDelta > 0 ? -1e-2f : 1e-2f, Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CCurvedTextView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_translating)
		m_origin.x += point.x - m_oldMousePoint.x,
		m_origin.y += point.y - m_oldMousePoint.y,
		Invalidate();

	m_oldMousePoint = point;

	CView::OnMouseMove(nFlags, point);
}
