
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

#define DEG2RAD(x) ((x * 2. * 3.14) / 360.)

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
		Mx ? -1.f : 1.f, 0.f,
		0.f, My ? -1.f : 1.f,
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CMonaPuzzleView::AntiFlicker(CDC* pDC)
{
	const XFORM matrix{
		1.f, 0.f,
		0.f, 1.f,
		0.f, 0.f,
	};
	pDC->SetWorldTransform(&matrix);
}

void CMonaPuzzleView::CopyBitmap(CDC* pDC, CBitmap* dstBmp, CBitmap* srcBmp)
{
	// Get bitmap information from the source bitmap
	BITMAP bitmapInfo;
	srcBmp->GetBitmap(&bitmapInfo);

	// Create a memory device context (CDC) compatible with the screen
	CDC sourceDC, destDC;
	sourceDC.CreateCompatibleDC(pDC);  // Source DC
	destDC.CreateCompatibleDC(pDC);    // Destination DC

	// Select the source bitmap into the source device context
	CBitmap* pOldSourceBitmap = sourceDC.SelectObject(srcBmp);

	// Create a new destination bitmap with the same size as the source
	dstBmp->CreateCompatibleBitmap(&sourceDC, bitmapInfo.bmWidth, bitmapInfo.bmHeight);

	// Select the destination bitmap into the destination device context
	CBitmap* pOldDestBitmap = destDC.SelectObject(dstBmp);

	// Copy the source bitmap to the destination bitmap
	destDC.BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &sourceDC, 0, 0, SRCCOPY);

	// Clean up: restore the old bitmaps and delete the device contexts
	sourceDC.SelectObject(pOldSourceBitmap);
	destDC.SelectObject(pOldDestBitmap);
}

void CMonaPuzzleView::DrawPuzzle(CDC* pDC, DImage& image, int x, int y, BOOL blueFilter)
{
	int bitmapWidth = image.Width(), bitmapHeight = image.Height();

	CBitmap copy;
	CopyBitmap(pDC, &copy, image.GetBitmap());

	CBitmap mask;
	mask.CreateBitmap(bitmapWidth, bitmapHeight, 1, 1, NULL);

	// Part 1 - create DC that will contain source image (bitmap) - source DC,
	// and a DC where we will create our filtering mask - destination DC.

	CDC srcDC{};
	srcDC.CreateCompatibleDC(pDC);
	CDC dstDC{};
	dstDC.CreateCompatibleDC(pDC);

	CBitmap* oldSrcBitmap = srcDC.SelectObject(&copy);
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

	ConvertToGreyscale(&copy);
	if (blueFilter)
		BlueFilter(&copy);

	// Final drawing to pDC - mask and then picture;
	pDC->BitBlt(x, y, bitmapWidth, bitmapHeight, &dstDC, 0, 0, SRCAND);
	pDC->BitBlt(x, y, bitmapWidth, bitmapHeight, &srcDC, 0, 0, SRCPAINT);

	// Releasing resources.
	dstDC.SelectObject(dstOldBitmap);
	dstDC.DeleteDC();
	srcDC.SelectObject(oldSrcBitmap);
	srcDC.DeleteDC();
}

void CMonaPuzzleView::ConvertToGreyscale(CBitmap* bitmap)
{
	BITMAP bitmapInfo;
	bitmap->GetBitmap(&bitmapInfo);

	if (bitmapInfo.bmBitsPixel != 32)
	{
		AfxMessageBox(L"Not a BGRA image!");
		exit(-1);
	}

	int colorsPerPixel = bitmapInfo.bmBitsPixel / CHAR_BIT; // CHAR_BIT == number of bits in a byte.
	long bmpSize = bitmapInfo.bmWidth * bitmapInfo.bmHeight * colorsPerPixel;

	BYTE* lpBits = new BYTE[bmpSize];
	bitmap->GetBitmapBits(bmpSize, lpBits);

	int threshold = 30;
	BYTE transparencyMask[3]{ lpBits[0], lpBits[1], lpBits[2], };

	for (long i = 0; i < bmpSize; i += colorsPerPixel)
	{
		BYTE b = lpBits[i];
		BYTE g = lpBits[i + 1];
		BYTE r = lpBits[i + 2];

		if (transparencyMask[0] == r &&
			transparencyMask[1] == g &&
			transparencyMask[2] == b)
			continue;

		BYTE gray = threshold + (b + g + r) / 3;

		if (gray > threshold) {
			lpBits[i] = gray;
			lpBits[i + 1] = gray;
			lpBits[i + 2] = gray;
		}
	}

	bitmap->SetBitmapBits(bmpSize, lpBits);
	delete[] lpBits;
}

void CMonaPuzzleView::BlueFilter(CBitmap* bitmap)
{
	BITMAP bitmapInfo;
	bitmap->GetBitmap(&bitmapInfo);

	if (bitmapInfo.bmBitsPixel != 32)
	{
		AfxMessageBox(L"Not a BGRA image!");
		exit(-1);
	}

	int colorsPerPixel = bitmapInfo.bmBitsPixel / CHAR_BIT;
	long bmpSize = bitmapInfo.bmWidth * bitmapInfo.bmHeight * colorsPerPixel;

	BYTE* lpBits = new BYTE[bmpSize];
	bitmap->GetBitmapBits(bmpSize, lpBits);

	int threshold = 30;
	BYTE transparencyMask[3]{ lpBits[0], lpBits[1], lpBits[2], };

	for (long i = 0; i < bmpSize; i += colorsPerPixel)
	{
		BYTE b = lpBits[i];
		BYTE g = lpBits[i + 1];
		BYTE r = lpBits[i + 2];

		if (transparencyMask[0] == r &&
			transparencyMask[1] == g &&
			transparencyMask[2] == b)
			continue;

		BYTE gray = threshold + (b + g + r) / 3;

		if (gray > threshold) {
			lpBits[i] = gray;
			lpBits[i + 1] = 0;
			lpBits[i + 2] = 0;
		}
	}

	bitmap->SetBitmapBits(bmpSize, lpBits);
	delete[] lpBits;
}

void CMonaPuzzleView::AssembleMona(CDC* pDC)
{
	int puzzleSize = 6 * gridUnitSize;

	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	Translate(pDC, gridUnitSize, 3 * puzzleSize + gridUnitSize);
	Mirror(pDC, FALSE, TRUE);

	XFORM monaLisaTransform;
	pDC->GetWorldTransform(&monaLisaTransform);

	Translate(pDC, 0.f, 2 * puzzleSize);
	Rotate(pDC, (FLOAT)DEG2RAD(25.));
	DrawPuzzle(pDC, monaPuzzles[0][0], -1 * gridUnitSize, -4 * gridUnitSize);

	pDC->SetWorldTransform(&monaLisaTransform);

	Translate(pDC, 0.f, 2 * puzzleSize);
	Rotate(pDC, (FLOAT)DEG2RAD(-73.3));
	DrawPuzzle(pDC, monaPuzzles[0][1], (int)(-1.4 * gridUnitSize), (int)(-3.4 * gridUnitSize));

	pDC->SetWorldTransform(&monaLisaTransform);

	Translate(pDC, 1 * puzzleSize, 1 * puzzleSize);
	Rotate(pDC, (FLOAT)DEG2RAD(19. + 196.));
	DrawPuzzle(pDC, monaPuzzles[0][2], (int)(-1.2 * gridUnitSize), (int)(-3.65 * gridUnitSize));

	pDC->SetWorldTransform(&monaLisaTransform);

	Translate(pDC, 2 * puzzleSize, 2 * puzzleSize);
	Rotate(pDC, (FLOAT)DEG2RAD(-35. + 61.));
	DrawPuzzle(pDC, monaPuzzles[1][0], (int)(-4.27 * gridUnitSize), (int)(-1.26 * gridUnitSize));

	pDC->SetWorldTransform(&monaLisaTransform);

	Translate(pDC, 1 * puzzleSize, 2 * puzzleSize);
	Rotate(pDC, (FLOAT)DEG2RAD(-32. - 117.));
	DrawPuzzle(pDC, monaPuzzles[1][1], (int)(-4.1 * gridUnitSize), (int)(-1.3 * gridUnitSize));

	pDC->SetWorldTransform(&monaLisaTransform);

	Translate(pDC, 1 * puzzleSize, 1 * puzzleSize);
	Rotate(pDC, (FLOAT)DEG2RAD(23. - 70.));
	DrawPuzzle(pDC, monaPuzzles[1][2], (int)(-1.1 * gridUnitSize), (int)(-3.85 * gridUnitSize), TRUE);

	pDC->SetWorldTransform(&monaLisaTransform);

	Translate(pDC, 2 * puzzleSize, 0 * puzzleSize);
	Rotate(pDC, (FLOAT)DEG2RAD(-19.));
	DrawPuzzle(pDC, monaPuzzles[2][0], (int)(-3.15 * gridUnitSize), (int)(-1.6 * gridUnitSize));

	pDC->SetWorldTransform(&monaLisaTransform);

	Translate(pDC, 2 * puzzleSize, 2 * puzzleSize);
	Rotate(pDC, (FLOAT)DEG2RAD(-18. - 105.));
	DrawPuzzle(pDC, monaPuzzles[2][1], (int)(-3.05 * gridUnitSize), (int)(-1.7 * gridUnitSize));

	pDC->SetWorldTransform(&monaLisaTransform);

	Translate(pDC, 3 * puzzleSize, 2 * puzzleSize);
	Rotate(pDC, (FLOAT)DEG2RAD(14.5 + 102.));
	DrawPuzzle(pDC, monaPuzzles[2][2], (int)(-1.4 * gridUnitSize), (int)(-3.3 * gridUnitSize));

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

	if (windowHeight == -1 || windowWidth == -1)
		CalculateWindowMeasures(pDC);

	int oldGraphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	Translate(pDC, center.x, center.y);
	AssembleMona(pDC);

	pDC->SetWorldTransform(&oldWorldTransform);
	pDC->SetGraphicsMode(oldGraphicsMode);

	if (gridActive)
		DrawGrid(pDC);
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

	if (nChar == 'A')
		center.x += gridUnitSize / 2, Invalidate();

	if (nChar == 'D')
		center.x -= gridUnitSize / 2, Invalidate();

	if (nChar == 'W')
		center.y += gridUnitSize / 2, Invalidate();

	if (nChar == 'S')
		center.y -= gridUnitSize / 2, Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMonaPuzzleView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	windowWidth = cx, windowHeight = cy;
}
