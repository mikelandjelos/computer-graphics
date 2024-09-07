
// RegioniPutanjeView.cpp : implementation of the CRegioniPutanjeView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RegioniPutanje.h"
#endif

#include "RegioniPutanjeDoc.h"
#include "RegioniPutanjeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRegioniPutanjeView

IMPLEMENT_DYNCREATE(CRegioniPutanjeView, CView)

BEGIN_MESSAGE_MAP(CRegioniPutanjeView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CRegioniPutanjeView construction/destruction

CRegioniPutanjeView::CRegioniPutanjeView() noexcept
{
	// TODO: add construction code here

}

void CRegioniPutanjeView::DrawGrid(CDC* pDC)
{
	for (int i = gridUnitSize; i < windowWidth; i += gridUnitSize)
		pDC->MoveTo(i, 0), pDC->LineTo(i, windowHeight);

	for (int i = gridUnitSize; i < windowHeight; i += gridUnitSize)
		pDC->MoveTo(0, i), pDC->LineTo(windowWidth, i);
}

void CRegioniPutanjeView::Translate(CDC* pDC, FLOAT Dx, FLOAT Dy, DWORD mode)
{
	const XFORM matrix{
		1.f, 0.f,
		0.f, 1.f,
		Dx, Dy,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CRegioniPutanjeView::Scale(CDC* pDC, FLOAT Sx, FLOAT Sy, DWORD mode)
{
	const XFORM matrix{
		Sx, 0.f,
		0.f, Sy,
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CRegioniPutanjeView::Rotate(CDC* pDC, FLOAT angle, DWORD mode)
{
	const XFORM matrix{
		cosf(angle), sinf(angle),
		-sinf(angle), cosf(angle),
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CRegioniPutanjeView::DrawRegions(CDC* pDC)
{
	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	CRgn starRgn;
	CPoint starRgnPoints[5] = {
		CPoint{(int)(4 * gridUnitSize), (int)(1 * gridUnitSize)},
		CPoint{(int)(2 * gridUnitSize), (int)(7 * gridUnitSize)},
		CPoint{(int)(7 * gridUnitSize), (int)(3 * gridUnitSize)},
		CPoint{(int)(1 * gridUnitSize), (int)(3 * gridUnitSize)},
		CPoint{(int)(6 * gridUnitSize), (int)(7 * gridUnitSize)},
	};

	CRgn circleRgn;
	CRgn xorRgn;

	CBrush fillBrush{ RGB(128, 128, 128) };
	CBrush frameBrush{ RGB(32, 32, 32) };

	starRgn.CreatePolygonRgn(starRgnPoints, 5, ALTERNATE);
	circleRgn.CreateEllipticRgn(
		(int)(2 * gridUnitSize), (int)(2 * gridUnitSize),
		(int)(6 * gridUnitSize), (int)(6 * gridUnitSize));
	xorRgn.CreateRectRgn(0, 0, 0, 0);
	xorRgn.CombineRgn(&starRgn, &circleRgn, RGN_XOR);

	circleRgn.OffsetRgn(7.f * gridUnitSize, 0.f);
	xorRgn.OffsetRgn(14.f * gridUnitSize, 0.f);

	pDC->FillRgn(&starRgn, &fillBrush);
	pDC->FrameRgn(&starRgn, &frameBrush, 3, 1);

	pDC->FillRgn(&circleRgn, &fillBrush);
	pDC->FrameRgn(&circleRgn, &frameBrush, 3, 1);

	CRect oldClipBoxRect;
	pDC->GetClipBox(&oldClipBoxRect);
	CRgn oldClipBoxRgn;
	oldClipBoxRgn.CreateRectRgnIndirect(&oldClipBoxRect);

	int rgnType = pDC->SelectClipRgn(&xorRgn);

	COLORREF oldTextColor = pDC->SetTextColor(RGB(0, 0, 0));
	COLORREF oldBkColor = pDC->SetBkColor(RGB(255, 255, 0));

	CString clippedText{ xorRgn.PtInRegion(mouseCoordinates)
		? "insideinsideinsideinsideinsideinsideinsideinsideinsideinsideinsideinsideinside"
		: "outsideoutsideoutsideoutsideoutsideoutsideoutsideoutsideoutsideoutsideoutside" };
	CPoint clippedTextExtent = pDC->GetTextExtent(clippedText);

	int oldTextAlign = pDC->SetTextAlign(TA_LEFT | TA_TOP);
	for (int i = 0; i < 7 * gridUnitSize; i += clippedTextExtent.y)
		pDC->TextOutW(14 * gridUnitSize, i, clippedText);

	pDC->SelectClipRgn(&oldClipBoxRgn);

	pDC->SetTextAlign(oldTextAlign);
	pDC->SetBkColor(oldBkColor);
	pDC->SetTextColor(oldTextColor);

	circleRgn.DeleteObject();
	starRgn.DeleteObject();

	fillBrush.DeleteObject();
	frameBrush.DeleteObject();
	oldClipBoxRgn.DeleteObject();

	pDC->SetWorldTransform(&oldWorldTransform);
}

void CRegioniPutanjeView::FloodFilling(CDC* pDC)
{
	COLORREF blue = RGB(0, 0, 255);
	CPen bluePen{ PS_GEOMETRIC | PS_SOLID, 4, blue };
	CBrush redBrush{ RGB(255, 0, 0) };

	CPen* oldPen = pDC->SelectObject(&bluePen);
	CBrush* oldBrush = pDC->SelectObject(&redBrush);

	pDC->MoveTo(1 * gridUnitSize, 1 * gridUnitSize);
	pDC->LineTo(7 * gridUnitSize, 7 * gridUnitSize);
	pDC->LineTo(13 * gridUnitSize, 1 * gridUnitSize);

	pDC->MoveTo(13 * gridUnitSize, 7 * gridUnitSize);
	pDC->LineTo(7 * gridUnitSize, 1 * gridUnitSize);
	pDC->LineTo(1 * gridUnitSize, 7 * gridUnitSize);

	pDC->FloodFill(7 * gridUnitSize, 3.5 * gridUnitSize, blue);

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	bluePen.DeleteObject();
	redBrush.DeleteObject();
}

CRegioniPutanjeView::~CRegioniPutanjeView()
{
}

BOOL CRegioniPutanjeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRegioniPutanjeView drawing

void CRegioniPutanjeView::OnDraw(CDC* pDC)
{
	CRegioniPutanjeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (windowWidth == -1 || windowHeight == -1)
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		windowWidth = rcClient.Width(), windowHeight = rcClient.Height();
	}

	int oldGraphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);

	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	DrawRegions(pDC);
	Translate(pDC, 0 * gridUnitSize, 10 * gridUnitSize);
	FloodFilling(pDC);

	pDC->SetWorldTransform(&oldWorldTransform);
	pDC->SetGraphicsMode(oldGraphicsMode);

	if (gridActive)
		DrawGrid(pDC);
}


// CRegioniPutanjeView printing

BOOL CRegioniPutanjeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRegioniPutanjeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRegioniPutanjeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRegioniPutanjeView diagnostics

#ifdef _DEBUG
void CRegioniPutanjeView::AssertValid() const
{
	CView::AssertValid();
}

void CRegioniPutanjeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRegioniPutanjeDoc* CRegioniPutanjeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRegioniPutanjeDoc)));
	return (CRegioniPutanjeDoc*)m_pDocument;
}
#endif //_DEBUG


// CRegioniPutanjeView message handlers


void CRegioniPutanjeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G')
		gridActive = !gridActive, Invalidate();

	if (nChar == VK_UP)
		center.y -= gridUnitSize, Invalidate();

	if (nChar == VK_DOWN)
		center.y += gridUnitSize, Invalidate();

	if (nChar == VK_LEFT)
		center.x -= gridUnitSize, Invalidate();

	if (nChar == VK_RIGHT)
		center.x += gridUnitSize, Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CRegioniPutanjeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	windowWidth = cx, windowHeight = cy;
}


void CRegioniPutanjeView::OnMouseMove(UINT nFlags, CPoint point)
{
	mouseCoordinates = point, Invalidate();
	CView::OnMouseMove(nFlags, point);
}
