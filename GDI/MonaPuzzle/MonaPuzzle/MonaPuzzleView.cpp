
// MonaPuzzleView.cpp : implementation of the CMonaPuzzleView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MonaPuzzle.h"
#endif

#include "MonaPuzzleDoc.h"
#include "MonaPuzzleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMonaPuzzleView

IMPLEMENT_DYNCREATE(CMonaPuzzleView, CView)

BEGIN_MESSAGE_MAP(CMonaPuzzleView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CMonaPuzzleView construction/destruction

CMonaPuzzleView::CMonaPuzzleView() noexcept
{
	monaPuzzles[0][0].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_upper_left.bmp");
	monaPuzzles[0][1].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_upper_center.bmp");
	monaPuzzles[0][2].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_upper_right.bmp");

	monaPuzzles[1][0].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_middle_left.bmp");
	monaPuzzles[1][1].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_middle_center.bmp");
	monaPuzzles[1][2].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_middle_right.bmp");

	monaPuzzles[2][0].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_lower_left.bmp");
	monaPuzzles[2][1].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_lower_center.bmp");
	monaPuzzles[2][2].Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\mona_bitmape\\mona_lower_right.bmp");
}

void CMonaPuzzleView::CalculateWindowMeasures(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	windowWidth = rcClient.Width(), windowHeight = rcClient.Height();
}

void CMonaPuzzleView::DrawGrid(CDC* pDC)
{
	for (int i = gridUnitSize; i < windowWidth; i += gridUnitSize)
		pDC->MoveTo(i, 0), pDC->LineTo(i, windowHeight);

	for (int i = gridUnitSize; i < windowHeight; i += gridUnitSize)
		pDC->MoveTo(0, i), pDC->LineTo(windowWidth, i);
}

void CMonaPuzzleView::Translate(CDC* pDC, FLOAT Dx, FLOAT Dy, DWORD mode)
{
	const XFORM matrix{
		1.f, 0.f,
		0.f, 1.f,
		Dx, Dy,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CMonaPuzzleView::Scale(CDC* pDC, FLOAT Sx, FLOAT Sy, DWORD mode)
{
	const XFORM matrix{
		Sx, 0.f,
		0.f, Sy,
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CMonaPuzzleView::Rotate(CDC* pDC, FLOAT angleInRadians, DWORD mode)
{
	const XFORM matrix{
		   cosf(angleInRadians), sinf(angleInRadians),
		   -sinf(angleInRadians), cosf(angleInRadians),
		   0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CMonaPuzzleView::Mirror(CDC* pDC, BOOL Mx, BOOL My, DWORD mode)
{
	const XFORM matrix{
		My ? -1.f : 1.f, 0.f,
		0.f, Mx ? -1.f : 1.f,
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CMonaPuzzleView::DrawTransparent(CDC* pDC, DImage& image, int x, int y)
{
	int bitmapWidth = image.Width(), bitmapHeight = image.Height();

	CBitmap mask;
	mask.CreateBitmap(bitmapWidth, bitmapHeight, 1, 1, NULL);

	// Part 1 - create DC that will contain source image (bitmap) - source DC,
	// and a DC where we will create our filtering mask - destination DC.

	CDC srcDC{};
	srcDC.CreateCompatibleDC(pDC);
	CDC dstDC{};
	dstDC.CreateCompatibleDC(pDC);

	CBitmap* oldSrcBitmap = srcDC.SelectObject(image.GetBitmap());
	CBitmap* dstOldBitmap = dstDC.SelectObject(&mask);

	// Setting bk color for srcDC (this color will be filtered in the end).
	COLORREF srcTopLeftPixel = srcDC.GetPixel(0, 0);
	COLORREF oldSrcBkColor = srcDC.SetBkColor(srcTopLeftPixel);

	// Mask formed - pixel takes value 0 if the corresponding pixel from srcDC is colored
	// with the srcDC bk color, elsewhere 1 (the mask is kinda inverse, because default bk color is white).
	dstDC.BitBlt(0, 0, bitmapWidth, bitmapHeight, &srcDC, 0, 0, SRCCOPY);

	// Filtering takes place here. After this step, inside our source DC
	// there will be a filtered version of our source image.
	COLORREF oldSrcTextColor = srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));
	srcDC.BitBlt(0, 0, bitmapWidth, bitmapHeight, &dstDC, 0, 0, SRCAND);

	// Releasing resources.
	dstDC.SelectObject(dstOldBitmap);
	dstDC.DeleteDC();
	srcDC.SelectObject(oldSrcBitmap);
	srcDC.DeleteDC();

	// Final drawing. Copying mask and then source image to pDC.
	CDC memDC{};
	memDC.CreateCompatibleDC(pDC);

	// First drawing to pDC - the mask;
	CBitmap* oldMemBitmap = memDC.SelectObject(&mask);
	pDC->BitBlt(x, y, bitmapWidth, bitmapHeight, &memDC, 0, 0, SRCAND);

	// Final drawing - pDC now contains the filtered image.
	memDC.SelectObject(image.GetBitmap());
	pDC->BitBlt(x, y, bitmapWidth, bitmapHeight, &memDC, 0, 0, SRCPAINT);

	// Releasing resources.
	memDC.SelectObject(oldMemBitmap);
	memDC.DeleteDC();
}

void CMonaPuzzleView::DrawMonaPuzzle(CDC* pDC)
{
	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	ConvertToGreyscale(pDC, monaPuzzles[0][0]);
	DrawTransparent(pDC, monaPuzzles[0][0]);

	pDC->SetWorldTransform(&oldWorldTransform);
}

CMonaPuzzleView::~CMonaPuzzleView()
{
}

BOOL CMonaPuzzleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMonaPuzzleView drawing

void CMonaPuzzleView::OnDraw(CDC* pDC)
{
	CMonaPuzzleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int oldGraphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);

	DrawMonaPuzzle(pDC);

	pDC->SetGraphicsMode(oldGraphicsMode);

	if (gridActive)
		DrawGrid(pDC);

	if (windowHeight == -1 || windowWidth == -1)
		CalculateWindowMeasures(pDC);
}


// CMonaPuzzleView printing

BOOL CMonaPuzzleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMonaPuzzleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMonaPuzzleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMonaPuzzleView diagnostics

#ifdef _DEBUG
void CMonaPuzzleView::AssertValid() const
{
	CView::AssertValid();
}

void CMonaPuzzleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMonaPuzzleDoc* CMonaPuzzleView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMonaPuzzleDoc)));
	return (CMonaPuzzleDoc*)m_pDocument;
}
#endif //_DEBUG


// CMonaPuzzleView message handlers


void CMonaPuzzleView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G')
		gridActive = !gridActive, Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMonaPuzzleView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	windowWidth = cx, windowHeight = cy;
}
