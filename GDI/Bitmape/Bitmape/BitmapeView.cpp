
// BitmapeView.cpp : implementation of the CBitmapeView class
//

#include "pch.h"
#include "framework.h"
#include "strsafe.h"

#define _USE_MATH_DEFINES
#include "math.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Bitmape.h"
#endif

#include "BitmapeDoc.h"
#include "BitmapeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEG2RAD(x) ((2. * M_PI * x) / 360.)

// CBitmapeView

IMPLEMENT_DYNCREATE(CBitmapeView, CView)

BEGIN_MESSAGE_MAP(CBitmapeView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CBitmapeView construction/destruction

CBitmapeView::CBitmapeView() noexcept
{
	gridActive = FALSE;
	windowWidth = -1;
	windowHeight = -1;

	monaImages[0][0].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_upper_left.bmp");
	monaImages[0][1].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_upper_center.bmp");
	monaImages[0][2].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_upper_right.bmp");
	monaImages[1][0].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_middle_left.bmp");
	monaImages[1][1].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_middle_center.bmp");
	monaImages[1][2].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_middle_right.bmp");
	monaImages[2][0].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_lower_left.bmp");
	monaImages[2][1].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_lower_center.bmp");
	monaImages[2][2].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_lower_right.bmp");
}

CBitmapeView::~CBitmapeView()
{
}

BOOL CBitmapeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBitmapeView drawing

void CBitmapeView::OnDraw(CDC* pDC)
{
	CBitmapeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int oldGraphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);

	DrawMonaImages(pDC, scaleFactor, center);

	pDC->SetGraphicsMode(oldGraphicsMode);

	if (windowWidth == -1 || windowHeight == -1)
		CalculateWindowMetrics(pDC);

	if (gridActive)
		DrawGrid(pDC);
}


// CBitmapeView printing

BOOL CBitmapeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBitmapeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBitmapeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CBitmapeView::DrawMonaImages(CDC* pDC, FLOAT scaleFactor, CPoint center)
{
	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	int imgWidth = monaImages[0][0].Width(), imgHeight = monaImages[0][0].Height();
	CRect imgRect{ 0, 0, imgWidth, imgHeight };

	Translate(pDC, center.x, center.y);
	Rotate(pDC, (FLOAT)DEG2RAD(90.f));
	Scale(pDC, scaleFactor, scaleFactor);
	Mirror(pDC, TRUE, FALSE);

	DrawTransparent(pDC, monaImages[0][0], 0 * imgWidth, 0 * imgHeight);
	DrawTransparent(pDC, monaImages[0][1], 1 * imgWidth, 0 * imgHeight);
	DrawTransparent(pDC, monaImages[0][2], 2 * imgWidth, 0 * imgHeight);
	DrawTransparent(pDC, monaImages[1][0], 0 * imgWidth, 1 * imgHeight);
	DrawTransparent(pDC, monaImages[1][1], 1 * imgWidth, 1 * imgHeight);
	DrawTransparent(pDC, monaImages[1][2], 2 * imgWidth, 1 * imgHeight);
	DrawTransparent(pDC, monaImages[2][0], 0 * imgWidth, 2 * imgHeight);
	DrawTransparent(pDC, monaImages[2][1], 1 * imgWidth, 2 * imgHeight);
	DrawTransparent(pDC, monaImages[2][2], 2 * imgWidth, 2 * imgHeight);

	pDC->SetWorldTransform(&oldWorldTransform);
}

void CBitmapeView::DrawTransparent(CDC* pDC, DImage& img, int x, int y)
{
	CBitmap mask;
	mask.CreateBitmap(img.Width(), img.Height(), 1, 1, NULL);

	CDC srcDC{};
	CDC dstDC{};
	srcDC.CreateCompatibleDC(pDC);
	dstDC.CreateCompatibleDC(pDC);

	CBitmap* srcOldBitmap = srcDC.SelectObject(img.GetBitmap());
	CBitmap* dstOldBitmap = dstDC.SelectObject(&mask);

	COLORREF srcTopLeftColor = srcDC.GetPixel(5, 5);
	COLORREF oldSrcBkColor = srcDC.SetBkColor(srcTopLeftColor);

	dstDC.BitBlt(0, 0, img.Width(), img.Height(), &srcDC, 0, 0, SRCCOPY);

	COLORREF oldSrcTextColor = srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));
	srcDC.BitBlt(0, 0, img.Width(), img.Height(), &dstDC, 0, 0, SRCAND);

	srcDC.SelectObject(srcOldBitmap);
	srcDC.SetBkColor(oldSrcBkColor);
	srcDC.DeleteDC();
	delete srcDC;

	dstDC.SelectObject(dstOldBitmap);
	dstDC.DeleteDC();
	delete dstDC;

	CDC* memDC = new CDC{};
	memDC->CreateCompatibleDC(pDC);

	CBitmap* oldMemDCBmp = memDC->SelectObject(&mask);

	pDC->BitBlt(x, y, img.Width(), img.Height(), memDC, 0, 0, SRCAND);
	memDC->SelectObject(img.GetBitmap());

	pDC->BitBlt(x, y, img.Width(), img.Height(), memDC, 0, 0, SRCPAINT);
	memDC->SelectObject(oldMemDCBmp);

	memDC->DeleteDC();
	delete memDC;
}

void CBitmapeView::Translate(CDC* pDC, FLOAT Dx, FLOAT Dy, DWORD mode)
{
	CONST XFORM matrix{
		1.f, 0.f,
		0.f, 1.f,
		Dx, Dy,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CBitmapeView::Scale(CDC* pDC, FLOAT Sx, FLOAT Sy, DWORD mode)
{
	CONST XFORM matrix{
		Sx, 0.f,
		0.f, Sy,
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CBitmapeView::Rotate(CDC* pDC, FLOAT angle, DWORD mode)
{
	CONST XFORM matrix{
		cosf(angle), sinf(angle),
		-sinf(angle), cosf(angle),
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CBitmapeView::Mirror(CDC* pDC, BOOL Mx, BOOL My, DWORD mode)
{
	CONST XFORM matrix{
		My ? -1.f : 1.f, 0.f,
		0.f, Mx ? -1.f : 1.f,
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CBitmapeView::DrawGrid(CDC* pDC)
{
	for (int cursor = gridUnitSize; cursor < windowWidth; cursor += gridUnitSize)
		pDC->MoveTo(cursor, 0), pDC->LineTo(cursor, windowHeight);

	for (int cursor = gridUnitSize; cursor < windowHeight; cursor += gridUnitSize)
		pDC->MoveTo(0, cursor), pDC->LineTo(windowWidth, cursor);

	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	TCHAR gridTextRaw[32];
	wsprintf(gridTextRaw, _T("%dx%d"), windowWidth / gridUnitSize, windowHeight / gridUnitSize);
	CString gridText(gridTextRaw);
	CPoint gridTextExtent = pDC->GetTextExtent(gridText);

	pDC->TextOutW(windowWidth - gridTextExtent.x - 5, windowHeight - gridTextExtent.y - 5, gridText);

	pDC->SetBkMode(oldBkMode);
}

void CBitmapeView::CalculateWindowMetrics(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	windowWidth = clientRect.Width(), windowHeight = clientRect.Height();
}


// CBitmapeView diagnostics

#ifdef _DEBUG
void CBitmapeView::AssertValid() const
{
	CView::AssertValid();
}

void CBitmapeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBitmapeDoc* CBitmapeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBitmapeDoc)));
	return (CBitmapeDoc*)m_pDocument;
}
#endif //_DEBUG


// CBitmapeView message handlers


void CBitmapeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G')
		gridActive = !gridActive, Invalidate();

	if (nChar == VK_UP && scaleFactor <= 2.f)
		scaleFactor += deltaScaleFactor, Invalidate();

	if (nChar == VK_DOWN && scaleFactor >= .5f)
		scaleFactor -= deltaScaleFactor, Invalidate();

	if (nChar == 'W')
		center.y -= gridUnitSize, Invalidate();

	if (nChar == 'S')
		center.y += gridUnitSize, Invalidate();

	if (nChar == 'A')
		center.x -= gridUnitSize, Invalidate();

	if (nChar == 'D')
		center.x += gridUnitSize, Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CBitmapeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	windowWidth = cx, windowHeight = cy;
}
