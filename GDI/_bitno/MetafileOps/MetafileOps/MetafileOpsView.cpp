
// MetafileOpsView.cpp : implementation of the CMetafileOpsView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MetafileOps.h"
#endif

#include "MetafileOpsDoc.h"
#include "MetafileOpsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMetafileOpsView

IMPLEMENT_DYNCREATE(CMetafileOpsView, CView)

BEGIN_MESSAGE_MAP(CMetafileOpsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CMetafileOpsView construction/destruction

CMetafileOpsView::CMetafileOpsView() noexcept
	: m_sceneTranslating{ FALSE },
	m_gridActive{ TRUE }, GRID_UNIT_SIZE{ 500 / 20 },
	m_origin{ 0, 0 }, m_oldMousePoint{}, m_scaleFactor{ 1.f },
	CLIO_WIDTH{ 645 }, CLIO_HEIGHT{ 260 },
	m_angleWheel{ 0.f }, m_clioPosition{ 0, 0 }, m_alreadyFiltered{ FALSE }
{
	m_emfClio = GetEnhMetaFileW(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\Resursi za zadatke\\Zad3\\clio.emf");
	m_imgWheel.Load(L"C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\Resursi za zadatke\\Zad3\\wheel.png");
}

CMetafileOpsView::~CMetafileOpsView()
{
	DeleteEnhMetaFile(m_emfClio);
}

BOOL CMetafileOpsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMetafileOpsView drawing

void CMetafileOpsView::OnDraw(CDC* pDC)
{
	CMetafileOpsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int oldGraphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);

	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	Translate(pDC, m_origin.x, m_origin.y);
	Scale(pDC, m_scaleFactor, m_scaleFactor);

	DrawScene(pDC);

	pDC->SetWorldTransform(&oldWorldTransform);
	pDC->SetGraphicsMode(oldGraphicsMode);

	if (m_gridActive)
		DrawGrid(pDC, GRID_UNIT_SIZE);
}


// CMetafileOpsView printing

BOOL CMetafileOpsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMetafileOpsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMetafileOpsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMetafileOpsView::DrawClio(CDC* pDC)
{
	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	Scale(pDC, -1.f, 1.f);
	Translate(pDC, -CLIO_WIDTH, 0);
	pDC->PlayMetaFile(m_emfClio, CRect{ 0, 0, CLIO_WIDTH, CLIO_HEIGHT });
	pDC->SetWorldTransform(&oldWorldTransform);

	pDC->SetWorldTransform(&oldWorldTransform);
}

void CMetafileOpsView::FilterBitmapColor(CBitmap& bmp, COLORREF color, long threshold)
{
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	if (bmpInfo.bmBitsPixel != 4 * CHAR_BIT)
	{
		AfxMessageBox(L"Not a RGBA bitmap!");
		exit(-1);
	}

	long bytesPerPixel = bmpInfo.bmBitsPixel / CHAR_BIT;
	long numOfBytes = bmpInfo.bmWidth * bmpInfo.bmHeight * bytesPerPixel;

	// BGRA - 1st B, 2nd G, 3rd R, 4th A.
	BYTE* bmpBytes = new BYTE[numOfBytes];
	bmp.GetBitmapBits(numOfBytes, bmpBytes);

	BYTE transparencyColor[3] = { bmpBytes[0], bmpBytes[1], bmpBytes[2] };

	for (long i = 0; i < numOfBytes; i += bytesPerPixel)
	{
		BYTE b = bmpBytes[i];
		BYTE g = bmpBytes[i + 1];
		BYTE r = bmpBytes[i + 2];

		if (b == transparencyColor[0] &&
			g == transparencyColor[1] &&
			r == transparencyColor[2])
			continue;

		long intensity = threshold + (b + g + r) / 3;

		bmpBytes[i] = intensity * GetBValue(color) / 255,
			bmpBytes[i + 1] = intensity * GetGValue(color) / 255,
			bmpBytes[i + 2] = intensity * GetRValue(color) / 255;
	}

	bmp.SetBitmapBits(numOfBytes, bmpBytes);
	delete[] bmpBytes;
}

void CMetafileOpsView::DrawBitmapTransparent(CDC* pDC, CBitmap& bmp, int x, int y)
{
	BITMAP bmInfo;
	bmp.GetBitmap(&bmInfo);
	INT width = bmInfo.bmWidth, height = bmInfo.bmHeight;

	CBitmap mask;
	mask.CreateBitmap(width, height, 1, 1, NULL);

	CDC srcDC, dstDC;
	srcDC.CreateCompatibleDC(pDC);
	dstDC.CreateCompatibleDC(pDC);

	CBitmap* oldSrcBmp = srcDC.SelectObject(&bmp);
	CBitmap* oldDstBmp = dstDC.SelectObject(&mask);

	COLORREF transparencyColor = srcDC.GetPixel(0, 0);
	srcDC.SetBkColor(transparencyColor);

	dstDC.BitBlt(0, 0, width, height, &srcDC, 0, 0, SRCCOPY);

	srcDC.SetBkColor(RGB(0, 0, 0));
	srcDC.SetTextColor(RGB(255, 255, 255));

	srcDC.BitBlt(0, 0, width, height, &dstDC, 0, 0, SRCAND);

	if (!m_alreadyFiltered)
		FilterBitmapColor(bmp, RGB(0, 255, 255), 64), m_alreadyFiltered = TRUE;

	pDC->BitBlt(x, y, width, height, &dstDC, 0, 0, SRCAND);
	pDC->BitBlt(x, y, width, height, &srcDC, 0, 0, SRCPAINT);

	dstDC.DeleteDC();
	srcDC.DeleteDC();
}

void CMetafileOpsView::DrawWheel(CDC* pDC)
{
	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	Rotate(pDC, Deg2Rad(m_angleWheel));
	DrawBitmapTransparent(pDC, *m_imgWheel.GetBitmap(), -9.6f * GRID_UNIT_SIZE, -8.2f * GRID_UNIT_SIZE);

	pDC->SetWorldTransform(&oldWorldTransform);
}

void CMetafileOpsView::DrawClioWithWheels(CDC* pDC)
{
	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	Translate(pDC, m_clioPosition.x, m_clioPosition.y);
	XFORM clioPositionTransform;
	pDC->GetWorldTransform(&clioPositionTransform);

	DrawClio(pDC);

	Translate(pDC, 4.f * GRID_UNIT_SIZE, 9.5f * GRID_UNIT_SIZE);
	Scale(pDC, 0.25f, 0.25f);
	DrawWheel(pDC);

	pDC->SetWorldTransform(&clioPositionTransform);
	Translate(pDC, 20.7f * GRID_UNIT_SIZE, 9.5f * GRID_UNIT_SIZE);
	Scale(pDC, 0.25f, 0.25f);
	DrawWheel(pDC);

	pDC->SetWorldTransform(&oldWorldTransform);
}

void CMetafileOpsView::DrawScene(CDC* pDC)
{
	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	DrawClioWithWheels(pDC);

	pDC->SetWorldTransform(&oldWorldTransform);
}

void CMetafileOpsView::DrawGrid(CDC* pDC, INT gridUnitSize)
{
	for (int i = gridUnitSize; i < m_clientRect.Width(); i += gridUnitSize)
		pDC->MoveTo(i, 0), pDC->LineTo(i, m_clientRect.Height());

	for (int i = gridUnitSize; i < m_clientRect.Height(); i += gridUnitSize)
		pDC->MoveTo(0, i), pDC->LineTo(m_clientRect.Width(), i);
}

void CMetafileOpsView::Translate(CDC* pDC, FLOAT Dx, FLOAT Dy, DWORD mode)
{
	XFORM translationMatrix{
		1.f, 0.f,
		0.f, 1.f,
		Dx, Dy,
	};
	pDC->ModifyWorldTransform(&translationMatrix, mode);
}

void CMetafileOpsView::Rotate(CDC* pDC, FLOAT angle, DWORD mode)
{
	XFORM rotationMatrix{
		cosf(angle), sinf(angle),
		-sinf(angle), cosf(angle),
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&rotationMatrix, mode);
}

void CMetafileOpsView::Scale(CDC* pDC, FLOAT Sx, FLOAT Sy, DWORD mode)
{
	XFORM scalingMatrix{
	Sx, 0.f,
	0.f, Sy,
	0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&scalingMatrix, mode);
}


// CMetafileOpsView diagnostics

#ifdef _DEBUG
void CMetafileOpsView::AssertValid() const
{
	CView::AssertValid();
}

void CMetafileOpsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMetafileOpsDoc* CMetafileOpsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMetafileOpsDoc)));
	return (CMetafileOpsDoc*)m_pDocument;
}
#endif //_DEBUG


// CMetafileOpsView message handlers


void CMetafileOpsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'G':
		m_gridActive = !m_gridActive, Invalidate();
		break;
	case VK_LEFT:
		m_clioPosition.x -= 10, m_angleWheel -= 10.f, Invalidate();
		break;
	case VK_RIGHT:
		m_clioPosition.x += 10, m_angleWheel += 10.f, Invalidate();
		break;
	default:
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CMetafileOpsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_scaleFactor += zDelta > 0 ? -1e-2f : 1e-2f, Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CMetafileOpsView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_sceneTranslating)
		m_origin.x += point.x - m_oldMousePoint.x,
		m_origin.y += point.y - m_oldMousePoint.y,
		Invalidate();

	m_oldMousePoint = point;

	CView::OnMouseMove(nFlags, point);
}


void CMetafileOpsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_sceneTranslating = TRUE;
	CView::OnLButtonDown(nFlags, point);
}


void CMetafileOpsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_sceneTranslating = FALSE;
	CView::OnLButtonUp(nFlags, point);
}


void CMetafileOpsView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	GetClientRect(&m_clientRect);
}
