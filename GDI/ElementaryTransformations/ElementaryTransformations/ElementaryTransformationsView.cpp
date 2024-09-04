
// ElementaryTransformationsView.cpp : implementation of the CElementaryTransformationsView class
//

#include "pch.h"
#include "framework.h"

#define _USE_MATH_DEFINES
#include "math.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ElementaryTransformations.h"
#endif

#include "ElementaryTransformationsDoc.h"
#include "ElementaryTransformationsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEG2RAD(x) ((x * 2. * M_PI) / 360.)

// CElementaryTransformationsView

IMPLEMENT_DYNCREATE(CElementaryTransformationsView, CView)

BEGIN_MESSAGE_MAP(CElementaryTransformationsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CElementaryTransformationsView construction/destruction

CElementaryTransformationsView::CElementaryTransformationsView() noexcept
{
	// TODO: add construction code here
	gridActive = FALSE;
	axesActive = FALSE;

	//CRect rcClient;
	//GetClientRect(&rcClient);

	//widthInGridUnits = (UINT)(rcClient.Width() / gridUnitSize),
	//	heightInGridUnits = (UINT)(rcClient.Height() / gridUnitSize),
	//	windowWidth = rcClient.Width(),
	//	windowHeight = rcClient.Height();
}

void CElementaryTransformationsView::DrawTriangles(CDC* pDC, CPoint center)
{
	const CPoint triangle[] = {
		CPoint{0, 0},
		CPoint{(int)(1.5 * gridUnitSize), 3 * gridUnitSize},
		CPoint{(int)(-1.5 * gridUnitSize), 3 * gridUnitSize},
	};

	CPen redPen{ PS_COSMETIC | PS_SOLID, 0, RGB(255, 0, 0) };
	CBrush redBrush{ RGB(255, 0, 0) };
	CPen greenPen{ PS_COSMETIC | PS_SOLID, 0, RGB(0, 255, 0) };
	CBrush greenBrush{ RGB(0, 255, 0) };
	CPen bluePen{ PS_COSMETIC | PS_SOLID, 0, RGB(0, 0, 255) };
	CBrush blueBrush{ RGB(0, 0, 255) };
	CPen yellowPen{ PS_COSMETIC | PS_SOLID, 0, RGB(255, 255, 0) };
	CBrush yellowBrush{ RGB(255, 255, 0) };

	Translate(pDC, center.x * gridUnitSize, center.y * gridUnitSize);

	CPen* oldPen = pDC->SelectObject(&redPen);
	CBrush* oldBrush = pDC->SelectObject(&redBrush);
	pDC->Polygon(triangle, 3);

	Rotate(pDC, (FLOAT)DEG2RAD(90.f));

	pDC->SelectObject(&greenPen);
	pDC->SelectObject(&greenBrush);
	pDC->Polygon(triangle, 3);

	Rotate(pDC, (FLOAT)DEG2RAD(90.f));

	pDC->SelectObject(&bluePen);
	pDC->SelectObject(&blueBrush);
	pDC->Polygon(triangle, 3);

	Rotate(pDC, (FLOAT)DEG2RAD(90.f));

	pDC->SelectObject(&yellowPen);
	pDC->SelectObject(&yellowBrush);
	pDC->Polygon(triangle, 3);

	// Cleanup and global drawings
	// =======================================================

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);

	redPen.DeleteObject();
	greenPen.DeleteObject();
	bluePen.DeleteObject();
	yellowPen.DeleteObject();
	redBrush.DeleteObject();
	greenBrush.DeleteObject();
	blueBrush.DeleteObject();
	yellowBrush.DeleteObject();
}

void CElementaryTransformationsView::DrawHearts(CDC* pDC, CPoint center)
{
	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	COLORREF red = RGB(255, 0, 0), pink = RGB(255, 0, 255), purple = RGB(127, 0, 255), blue = RGB(0, 0, 255);

	Translate(pDC, center.x * gridUnitSize, center.y * gridUnitSize);
	Scale(pDC, 1.f / 2, 1.f / 2);

	XFORM centerPointScaledNoRotations;
	pDC->GetWorldTransform(&centerPointScaledNoRotations);

	// Small hearts in the middle
	Translate(pDC, 0.f * gridUnitSize, 1.f * gridUnitSize);
	DrawHeart(pDC, red);
	Scale(pDC, 2.f, 2.f);
	Translate(pDC, 0.f * gridUnitSize, 2.f * gridUnitSize);
	DrawHeart(pDC, pink);

	pDC->SetWorldTransform(&centerPointScaledNoRotations);
	Rotate(pDC, (FLOAT)DEG2RAD(90.f));
	Translate(pDC, 0.f * gridUnitSize, 1.f * gridUnitSize);
	DrawHeart(pDC, pink);
	Scale(pDC, 2.f, 2.f);
	Translate(pDC, 0.f * gridUnitSize, 2.f * gridUnitSize);
	DrawHeart(pDC, purple);

	pDC->SetWorldTransform(&centerPointScaledNoRotations);
	Rotate(pDC, (FLOAT)DEG2RAD(180.f));
	Translate(pDC, 0.f * gridUnitSize, 1.f * gridUnitSize);
	DrawHeart(pDC, purple);
	Scale(pDC, 2.f, 2.f);
	Translate(pDC, 0.f * gridUnitSize, 2.f * gridUnitSize);
	DrawHeart(pDC, blue);

	pDC->SetWorldTransform(&centerPointScaledNoRotations);
	Rotate(pDC, (FLOAT)DEG2RAD(270.f));
	Translate(pDC, 0.f * gridUnitSize, 1.f * gridUnitSize);
	DrawHeart(pDC, blue);
	Scale(pDC, 2.f, 2.f);
	Translate(pDC, 0.f * gridUnitSize, 2.f * gridUnitSize);
	DrawHeart(pDC, red);

	pDC->SetWorldTransform(&centerPointScaledNoRotations);

	// First Ring

	pDC->SetWorldTransform(&oldWorldTransform);
}

void CElementaryTransformationsView::DrawHeart(CDC* pDC, COLORREF color)
{
	int widthUnit = gridUnitSize, heightUnit = gridUnitSize;

	LOGBRUSH redPenAttributes{ BS_SOLID, color };
	CPen redPen{ PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 5, &redPenAttributes };

	CBrush redBrush{ color };

	CPen* oldPen = pDC->SelectObject(&redPen);
	CBrush* oldBrush = pDC->SelectObject(&redBrush);

	constexpr int heartPoints = 3 * 2 + 1;
	const CPoint leftHalf[heartPoints] = {
		CPoint{0 * widthUnit, 0 * heightUnit},
		CPoint{-4 * widthUnit, 2 * heightUnit},
		CPoint{-1 * widthUnit, 4 * heightUnit},
		CPoint{0 * widthUnit, 2 * heightUnit},
		CPoint{1 * widthUnit, 4 * heightUnit},
		CPoint{4 * widthUnit, 2 * heightUnit},
		CPoint{0 * widthUnit, 0 * heightUnit}
	};

	pDC->BeginPath();

	pDC->PolyBezier(leftHalf, heartPoints);

	pDC->EndPath();
	pDC->FillPath();

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	redPen.DeleteObject();
}

void CElementaryTransformationsView::Translate(CDC* pDC, FLOAT Dx, FLOAT Dy, DWORD mode)
{
	const XFORM translationMatrix{
		1.f, 0.f,
		0.f, 1.f,
		Dx, Dy,
	};
	pDC->ModifyWorldTransform(&translationMatrix, mode);
}

void CElementaryTransformationsView::Rotate(CDC* pDC, FLOAT angle, DWORD mode)
{
	const XFORM rotationMatrix{
		cosf(angle), sinf(angle),
		-sinf(angle), cosf(angle),
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&rotationMatrix, mode);
}

void CElementaryTransformationsView::Scale(CDC* pDC, FLOAT Sx, FLOAT Sy, DWORD mode)
{
	const XFORM scalingMatrix{
		Sx, 0.f,
		0.f, Sy,
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&scalingMatrix, mode);
}

void CElementaryTransformationsView::DrawGrid(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	for (int cursor = gridUnitSize; cursor < rcClient.Width(); cursor += gridUnitSize)
		pDC->MoveTo(cursor, 0), pDC->LineTo(cursor, rcClient.Height());

	for (int cursor = gridUnitSize; cursor < rcClient.Height(); cursor += gridUnitSize)
		pDC->MoveTo(0, cursor), pDC->LineTo(rcClient.Width(), cursor);

	TCHAR gridSizeTextString[32];
	wsprintf(gridSizeTextString, _T("%dx%d"), widthInGridUnits, heightInGridUnits);
	CString gridSizeText(gridSizeTextString);

	CPoint gridSizeTextExtent = pDC->GetTextExtent(gridSizeText);

	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW(windowWidth - gridSizeTextExtent.x - 5, windowHeight - gridSizeTextExtent.y - 5, gridSizeText);
	pDC->SetBkMode(oldBkMode);
}

void CElementaryTransformationsView::DrawAxes(CDC* pDC)
{
	CPen redPen{ PS_COSMETIC | PS_SOLID, 0, RGB(255, 0, 0) };
	CPen greenPen{ PS_COSMETIC | PS_SOLID, 0, RGB(0, 255, 0) };

	// x axis
	CPen* oldPen = pDC->SelectObject(&greenPen);
	pDC->MoveTo(0, 0);
	pDC->LineTo(100, 0);

	// y axis
	pDC->SelectObject(&redPen);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 100);

	pDC->SelectObject(oldPen);

	redPen.DeleteObject();
	greenPen.DeleteObject();
}

CElementaryTransformationsView::~CElementaryTransformationsView()
{
}

BOOL CElementaryTransformationsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CElementaryTransformationsView drawing

void CElementaryTransformationsView::OnDraw(CDC* pDC)
{
	CElementaryTransformationsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	const int oldGraphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	DrawTriangles(pDC, CPoint{ 20, 10 });

	pDC->SetWorldTransform(&oldWorldTransform);

	DrawHearts(pDC, CPoint{ 10, 10 });

	if (gridActive)
		DrawGrid(pDC);

	if (axesActive)
		DrawAxes(pDC);

	pDC->SetWorldTransform(&oldWorldTransform);
	pDC->SetGraphicsMode(oldGraphicsMode);
}


// CElementaryTransformationsView printing

BOOL CElementaryTransformationsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CElementaryTransformationsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CElementaryTransformationsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CElementaryTransformationsView diagnostics

#ifdef _DEBUG
void CElementaryTransformationsView::AssertValid() const
{
	CView::AssertValid();
}

void CElementaryTransformationsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CElementaryTransformationsDoc* CElementaryTransformationsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CElementaryTransformationsDoc)));
	return (CElementaryTransformationsDoc*)m_pDocument;
}
#endif //_DEBUG


// CElementaryTransformationsView message handlers


void CElementaryTransformationsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G')
		gridActive = !gridActive, Invalidate();

	if (nChar == 'A')
		axesActive = !axesActive, Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CElementaryTransformationsView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	widthInGridUnits = (UINT)(cx / gridUnitSize),
		heightInGridUnits = (UINT)(cy / gridUnitSize),
		windowWidth = cx,
		windowHeight = cy;
	Invalidate();
}
