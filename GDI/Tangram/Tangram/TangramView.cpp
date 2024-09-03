
// TangramView.cpp : implementation of the CTangramView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Tangram.h"
#endif

#include "TangramDoc.h"
#include "TangramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTangramView

IMPLEMENT_DYNCREATE(CTangramView, CView)

BEGIN_MESSAGE_MAP(CTangramView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CTangramView construction/destruction

CTangramView::CTangramView() noexcept
{
	// TODO: add construction code here
	mGridActive = FALSE;
}

void CTangramView::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
	CPoint* points = new CPoint[n];

	for (int i = 0; i < n; ++i)
		points[i] = CPoint{ (int)(cx + r * cos(i * 6.28 / n + rotAngle)), (int)(cy + r * sin(i * 6.28 / n + rotAngle)) };

	pDC->Polygon(points, n);

	delete[] points;
}

CTangramView::~CTangramView()
{
}

BOOL CTangramView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTangramView drawing

void CTangramView::OnDraw(CDC* pDC)
{
	CTangramDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rcClient;
	GetClientRect(&rcClient);

	CBrush backgroundBrush{ colorLightGray };

	CBrush* oldBrush = pDC->SelectObject(&backgroundBrush);

	pDC->Rectangle(rcClient);

	CFont idFont;

	idFont.CreateFontW(23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));

	CFont* oldFont = pDC->SelectObject(&idFont);

	CString idLabelText = _T("18234");

	COLORREF oldTextColor = pDC->SetTextColor(colorRed);
	COLORREF oldBkColor = pDC->SetBkColor(colorLightGray);

	pDC->TextOut(5, 5, idLabelText);

	CPen boldLinesPen{ PS_GEOMETRIC | PS_SOLID, 5, colorCyanBLue };

	CPen* oldPen = pDC->SelectObject(&boldLinesPen);

	// Drawing 'outer' objects;

	const CPoint yellowTriangle[] = {
		CPoint{ cUnitSize * 10, cUnitSize * 1 },
		CPoint{ cUnitSize * 7, cUnitSize * 4 },
		CPoint{ cUnitSize * 10, cUnitSize * 7 },
	};

	CBrush yellowBrush{ colorYellow };

	pDC->SelectObject(yellowBrush);
	pDC->Polygon(yellowTriangle, 3);

	const CPoint hatchedParallelogram[] = {
		CPoint{ cUnitSize * 4, cUnitSize * 4 },
		CPoint{ cUnitSize * 7, cUnitSize * 4 },
		CPoint{ cUnitSize * 10, cUnitSize * 7 },
		CPoint{ cUnitSize * 7, cUnitSize * 7 },
	};

	pDC->SetBkColor(RGB(255, 255, 255));

	CBrush hatchedBrush{ HS_CROSS, colorPastelBlue };

	pDC->SelectObject(hatchedBrush);
	pDC->Polygon(hatchedParallelogram, 4);

	const CPoint redTriangle[] = {
		CPoint{ cUnitSize * 7, cUnitSize * 7 },
		CPoint{ cUnitSize * 13, cUnitSize * 7 },
		CPoint{ cUnitSize * 13, cUnitSize * 13 },
	};

	CBrush redBrush{ colorRed };

	pDC->SelectObject(redBrush);
	pDC->Polygon(redTriangle, 3);

	const CPoint pinkSquare[] = {
		CPoint{ cUnitSize * 13, cUnitSize * 7 },
		CPoint{ cUnitSize * 16, cUnitSize * 7 },
		CPoint{ cUnitSize * 13, cUnitSize * 10 },
		CPoint{ cUnitSize * 16, cUnitSize * 10 },
	};

	CBrush pinkBrush{ colorPink };

	pDC->SelectObject(pinkBrush);
	pDC->Polygon(pinkSquare, 3);

	const CPoint purpleTriangle[] = {
		CPoint{ cUnitSize * 10, cUnitSize * 10 },
		CPoint{ cUnitSize * 10, cUnitSize * 16 },
		CPoint{ cUnitSize * 16, cUnitSize * 16 },
	};

	CBrush purpleBrush{ colorPurple };

	pDC->SelectObject(purpleBrush);
	pDC->Polygon(purpleTriangle, 3);

	const CPoint orangeTriangle[] = {
		CPoint{ cUnitSize * 10, cUnitSize * 16 },
		CPoint{ cUnitSize * 7, cUnitSize * 16 },
		CPoint{ cUnitSize * 7, cUnitSize * 19 },
	};

	CBrush orangeBrush{ colorOrange };

	pDC->SelectObject(orangeBrush);
	pDC->Polygon(orangeTriangle, 3);

	const CPoint greenTriangle[] = {
		CPoint{ cUnitSize * 13, cUnitSize * 13 },
		CPoint{ cUnitSize * 16, cUnitSize * 13 },
		CPoint{ cUnitSize * 16, cUnitSize * 16 },
	};

	CBrush greenBrush{ colorGreen };

	pDC->SelectObject(greenBrush);
	pDC->Polygon(greenTriangle, 3);

	CPen thinLinesPen{ PS_GEOMETRIC | PS_SOLID, 2, colorCyanBLue };

	pDC->SelectStockObject(NULL_BRUSH);
	pDC->SelectObject(thinLinesPen);

	DrawRegularPolygon(pDC, cUnitSize * 8.8, cUnitSize * 4, 0.75 * cUnitSize, 6, 0.);
	DrawRegularPolygon(pDC, cUnitSize * 11.2, cUnitSize * 8.7, 1 * cUnitSize, 5, 0.);
	DrawRegularPolygon(pDC, cUnitSize * 11.9, cUnitSize * 14.2, 1 * cUnitSize, 4, 0.);
	DrawRegularPolygon(pDC, cUnitSize * 15.1, cUnitSize * 14, 0.5 * cUnitSize, 8, 0.);
	DrawRegularPolygon(pDC, cUnitSize * 8, cUnitSize * 16.9, 0.5 * cUnitSize, 7, 0.);

	if (mGridActive) {
		CPen gridPen{ PS_COSMETIC | PS_SOLID, 0, RGB(255, 255, 255) };
		CPen* oldPen = pDC->SelectObject(&gridPen);
		DrawGrid(pDC);
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldFont);

	pDC->SetBkColor(oldBkColor);
	pDC->SetTextColor(oldTextColor);

	thinLinesPen.DeleteObject();
	greenBrush.DeleteObject();
	orangeBrush.DeleteObject();
	purpleBrush.DeleteObject();
	pinkBrush.DeleteObject();
	redBrush.DeleteObject();
	hatchedBrush.DeleteObject();
	yellowBrush.DeleteObject();
	boldLinesPen.DeleteObject();
	idFont.DeleteObject();
	backgroundBrush.DeleteObject();
}


// CTangramView printing

BOOL CTangramView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTangramView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTangramView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTangramView::DrawGrid(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int width = rcClient.Width(), height = rcClient.Height();

	for (int cursor = cUnitSize; cursor < width; cursor += cUnitSize)
		pDC->MoveTo(cursor, 0), pDC->LineTo(cursor, height);

	for (int cursor = cUnitSize; cursor < height; cursor += cUnitSize)
		pDC->MoveTo(0, cursor), pDC->LineTo(width, cursor);
}

// CTangramView diagnostics

#ifdef _DEBUG
void CTangramView::AssertValid() const
{
	CView::AssertValid();
}

void CTangramView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTangramDoc* CTangramView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTangramDoc)));
	return (CTangramDoc*)m_pDocument;
}
#endif //_DEBUG


// CTangramView message handlers


void CTangramView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G')
		mGridActive = !mGridActive, Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
