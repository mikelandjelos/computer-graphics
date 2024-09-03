
// CrtanjePrimitivaView.cpp : implementation of the CCrtanjePrimitivaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CrtanjePrimitiva.h"
#endif

#include "CrtanjePrimitivaDoc.h"
#include "CrtanjePrimitivaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCrtanjePrimitivaView

IMPLEMENT_DYNCREATE(CCrtanjePrimitivaView, CView)

BEGIN_MESSAGE_MAP(CCrtanjePrimitivaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CCrtanjePrimitivaView construction/destruction

CCrtanjePrimitivaView::CCrtanjePrimitivaView() noexcept
{
	// TODO: add construction code here
	mGridUnitFraction = 0;
}

CCrtanjePrimitivaView::~CCrtanjePrimitivaView()
{
}

BOOL CCrtanjePrimitivaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCrtanjePrimitivaView drawing

void CCrtanjePrimitivaView::OnDraw(CDC* pDC)
{
	CCrtanjePrimitivaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawHearthBezier(pDC);
	DrawHatchedPurpleHeart(pDC);
	DrawSolidRedHeart(pDC);

	if (mGridUnitFraction > 1)
		DrawGrid(pDC);
}


// CCrtanjePrimitivaView printing

BOOL CCrtanjePrimitivaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCrtanjePrimitivaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCrtanjePrimitivaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CCrtanjePrimitivaView::DrawSolidRedHeart(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(clientRect);

	// Drawing in the upper right half of the client rectangle, 
	// with horizontal unit of 1/12th of client rectangle width and height.
	int widthHalf = clientRect.Width() / 2, heightHalf = clientRect.Height() / 2;
	int widthUnit = widthHalf / 6, heightUnit = heightHalf / 6;

	COLORREF redColor = RGB(255, 0, 0);

	LOGBRUSH redPenAttributes{ BS_SOLID, redColor };
	CPen redPen{ PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 5, &redPenAttributes };

	CBrush redBrush{ redColor };

	CPen* oldPen = pDC->SelectObject(&redPen);
	CBrush* oldBrush = pDC->SelectObject(&redBrush);

	const CPoint bottomHalfPolyline[] = {
		CPoint{1 * widthUnit, 2 * heightUnit},
		CPoint{3 * widthUnit, 6 * heightUnit},
		CPoint{5 * widthUnit, 2 * heightUnit},
	};

	const CRect upperLeftHalfRect{ 1 * widthUnit, 0 * heightUnit, 3 * widthUnit, 4 * heightUnit };
	const CPoint upperLeftHalfArc[] = {
		CPoint{ 1 * widthUnit, 2 * heightUnit },
		CPoint{ 3 * widthUnit, 2 * heightUnit },
	};

	const CRect upperRightHalfRect{ 3 * widthUnit, 0 * heightUnit, 5 * widthUnit, 4 * heightUnit };
	const CPoint upperRightHalfArc[] = {
		CPoint{ 3 * widthUnit, 2 * heightUnit },
		CPoint{ 5 * widthUnit, 2 * heightUnit },
	};

	int oldArcDirection = pDC->SetArcDirection(AD_CLOCKWISE);

	pDC->BeginPath(); // So you can fill it.

	pDC->Polyline(bottomHalfPolyline, 3);
	pDC->Arc(upperLeftHalfRect, upperLeftHalfArc[0], upperLeftHalfArc[1]);
	pDC->Arc(upperRightHalfRect, upperRightHalfArc[0], upperRightHalfArc[1]);

	pDC->EndPath();
	pDC->FillPath();

	// Cleanup.
	pDC->SetArcDirection(oldArcDirection);
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	redPen.DeleteObject();
	redBrush.DeleteObject();
}

void CCrtanjePrimitivaView::DrawHatchedPurpleHeart(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(clientRect);

	CPoint oldWindowOrg = pDC->SetViewportOrg(clientRect.Width() / 2, 0);

	// Drawing in the upper left half of the client rectangle, 
	// with horizontal unit of 1/12th of client rectangle width and height.
	int widthHalf = clientRect.Width() / 2, heightHalf = clientRect.Height() / 2;
	int widthUnit = widthHalf / 6, heightUnit = heightHalf / 6;

	COLORREF purpleColor = RGB(127, 0, 255);

	CBrush purpleDiagcrossBrush{ HS_DIAGCROSS, purpleColor };

	CBrush* oldBrush = pDC->SelectObject(&purpleDiagcrossBrush);
	CGdiObject* oldPen = pDC->SelectStockObject(NULL_PEN);

	const CPoint bottomHalfPolygon[] = {
		CPoint{1 * widthUnit, 2 * heightUnit},
		CPoint{3 * widthUnit, 6 * heightUnit},
		CPoint{5 * widthUnit, 2 * heightUnit},
	};

	const CRect upperLeftHalfRect{ 1 * widthUnit, 0 * heightUnit, 3 * widthUnit, 4 * heightUnit };
	const CPoint upperLeftHalfArc[] = {
		CPoint{ 1 * widthUnit, 2 * heightUnit },
		CPoint{ 3 * widthUnit, 2 * heightUnit },
	};

	const CRect upperRightHalfRect{ 3 * widthUnit, 0 * heightUnit, 5 * widthUnit, 4 * heightUnit };
	const CPoint upperRightHalfArc[] = {
		CPoint{ 3 * widthUnit, 2 * heightUnit },
		CPoint{ 5 * widthUnit, 2 * heightUnit },
	};

	int oldArcDirection = pDC->SetArcDirection(AD_CLOCKWISE);

	pDC->Polygon(bottomHalfPolygon, 3);
	pDC->Chord(upperLeftHalfRect, upperLeftHalfArc[0], upperLeftHalfArc[1]);
	pDC->Chord(upperRightHalfRect, upperRightHalfArc[0], upperRightHalfArc[1]);

	pDC->SetArcDirection(oldArcDirection);

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	pDC->SetViewportOrg(oldWindowOrg);

	purpleDiagcrossBrush.DeleteObject();
}

void CCrtanjePrimitivaView::DrawHearthBezier(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(clientRect);

	CPoint oldViewportOrg = pDC->SetViewportOrg(clientRect.Width() / 4, clientRect.Height() / 2);

	// Drawing in the bottom left half of the client rectangle, 
	// with horizontal unit of 1/12th of client rectangle width and height.
	int widthHalf = clientRect.Width() / 2, heightHalf = clientRect.Height() / 2;
	int widthUnit = widthHalf / 6, heightUnit = heightHalf / 6;

	COLORREF redColor = RGB(255, 0, 0);

	LOGBRUSH redPenAttributes{ BS_SOLID, redColor };
	CPen redPen{ PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 5, &redPenAttributes };

	CBrush redBrush{ redColor };

	CPen* oldPen = pDC->SelectObject(&redPen);
	CBrush* oldBrush = pDC->SelectObject(&redBrush);

	constexpr int heartPoints = 3 * 2 + 1;
	const CPoint leftHalf[heartPoints] = {
		CPoint{3 * widthUnit, 1 * heightUnit},
		CPoint{1 * widthUnit, -2 * heightUnit},
		CPoint{0 * widthUnit, 2 * heightUnit},
		CPoint{3 * widthUnit, 6 * heightUnit},
		CPoint{6 * widthUnit, 2 * heightUnit},
		CPoint{5 * widthUnit, -2 * heightUnit},
		CPoint{3 * widthUnit, 1 * heightUnit}
	};

	pDC->BeginPath();

	pDC->PolyBezier(leftHalf, heartPoints);

	pDC->EndPath();
	pDC->FillPath();

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	pDC->SetViewportOrg(oldViewportOrg);

	redPen.DeleteObject();
}

void CCrtanjePrimitivaView::DrawGrid(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(&clientRect);

	int width = clientRect.Width(), height = clientRect.Height();
	int gridUnitVertical = width / mGridUnitFraction;
	int gridUnitHorizontal = height / mGridUnitFraction;

	// Vertical.
	int cursor = gridUnitVertical;
	while (cursor < width) {
		pDC->MoveTo(cursor, 0);
		pDC->LineTo(cursor, height);
		cursor += gridUnitVertical;
	}

	// Horizontal.
	cursor = gridUnitHorizontal;
	while (cursor < height) {
		pDC->MoveTo(0, cursor);
		pDC->LineTo(width, cursor);
		cursor += gridUnitHorizontal;
	}

	char gridUnitLabelText[5];
	_itoa_s(mGridUnitFraction, gridUnitLabelText, 10);
	CString cStrGridUnitLabelText(gridUnitLabelText);

	pDC->TextOutW(5, 5, cStrGridUnitLabelText);
}

// CCrtanjePrimitivaView diagnostics

#ifdef _DEBUG
void CCrtanjePrimitivaView::AssertValid() const
{
	CView::AssertValid();
}

void CCrtanjePrimitivaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCrtanjePrimitivaDoc* CCrtanjePrimitivaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCrtanjePrimitivaDoc)));
	return (CCrtanjePrimitivaDoc*)m_pDocument;
}
#endif //_DEBUG


// CCrtanjePrimitivaView message handlers


void CCrtanjePrimitivaView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar >= '0' && nChar <= '9')
		mGridUnitFraction = nChar - '0', Invalidate();

	if (nChar == VK_UP && mGridUnitFraction <= 19)
		++mGridUnitFraction, Invalidate();


	if (nChar == VK_DOWN && mGridUnitFraction >= 2)
		--mGridUnitFraction, Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
