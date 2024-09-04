
// MetafajloviView.cpp : implementation of the CMetafajloviView class
//

#include "pch.h"
#include "framework.h"

#define _USE_MATH_DEFINES
#include "math.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Metafajlovi.h"
#endif

#include "MetafajloviDoc.h"
#include "MetafajloviView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEG2RAD(x) ((2. * M_PI * x) / 360.)

// CMetafajloviView

IMPLEMENT_DYNCREATE(CMetafajloviView, CView)

BEGIN_MESSAGE_MAP(CMetafajloviView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CMetafajloviView construction/destruction

CMetafajloviView::CMetafajloviView() noexcept
{
	// TODO: add construction code here
	gridActive = FALSE;
	windowWidth = -1, windowHeight = -1;
	windowWidthInUnitSize = -1, windowHeightInUnitSize = -1;
	emfCactus = GetEnhMetaFile(CString(_T("C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\kaktus_metafajlovi\\cactus_part.emf")));
	emfCactusLight = GetEnhMetaFile(CString(_T("C:\\Users\\Mihajlo\\Videos\\_rg\\novo\\kaktus_metafajlovi\\cactus_part_light.emf")));
	lowerRotatingCactusNodeAngle = 0;
	upperRotatingCactusNodeAngle = 0;
}

void CMetafajloviView::DrawAxes(CDC* pDC)
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

void CMetafajloviView::DrawGrid(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	for (int cursor = gridUnitSize; cursor < rcClient.Width(); cursor += gridUnitSize)
		pDC->MoveTo(cursor, 0), pDC->LineTo(cursor, rcClient.Height());

	for (int cursor = gridUnitSize; cursor < rcClient.Height(); cursor += gridUnitSize)
		pDC->MoveTo(0, cursor), pDC->LineTo(rcClient.Width(), cursor);

	TCHAR gridSizeTextString[32];
	wsprintf(gridSizeTextString, _T("%dx%d"), windowWidthInUnitSize, windowHeightInUnitSize);
	CString gridSizeText(gridSizeTextString);

	CPoint gridSizeTextExtent = pDC->GetTextExtent(gridSizeText);

	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW(windowWidth - gridSizeTextExtent.x - 5, windowHeight - gridSizeTextExtent.y - 5, gridSizeText);
	pDC->SetBkMode(oldBkMode);
}

void CMetafajloviView::CalculateClientRectangleMeasures(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	windowWidth = rcClient.Width(), windowHeight = rcClient.Height();
	windowWidthInUnitSize = windowWidth / gridUnitSize;
	windowHeightInUnitSize = windowHeight / gridUnitSize;
}

CMetafajloviView::~CMetafajloviView()
{
	DeleteEnhMetaFile(emfCactus);
	DeleteEnhMetaFile(emfCactusLight);
}

BOOL CMetafajloviView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMetafajloviView drawing

void CMetafajloviView::OnDraw(CDC* pDC)
{
	CMetafajloviDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int oldGraphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);

	ColorBackground(pDC);
	DrawTextSignature(pDC);
	DrawFigure(pDC);

	pDC->SetGraphicsMode(oldGraphicsMode);

	if (gridActive)
		DrawGrid(pDC);
}


// CMetafajloviView printing

BOOL CMetafajloviView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMetafajloviView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMetafajloviView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMetafajloviView::ColorBackground(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CBrush backgroundBrush{ backgroundColor };
	CBrush* oldBrush = pDC->SelectObject(&backgroundBrush);
	pDC->Rectangle(rcClient);

	pDC->SelectObject(oldBrush);

	backgroundBrush.DeleteObject();
}

void CMetafajloviView::DrawTextSignature(CDC* pDC)
{
	CString textSignature{ _T("18234 Mihajlo Madić") };

	CFont signatureFont;
	signatureFont.CreateFontW(gridUnitSize * 1.2, 0, -900, -900, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));

	CFont* oldFont = pDC->SelectObject(&signatureFont);

	int oldTextAlign = pDC->SetTextAlign(TA_LEFT);

	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	COLORREF oldTextColor = pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOutW(19 * gridUnitSize, 1 * gridUnitSize, textSignature);
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->TextOutW(19 * gridUnitSize - 3, 1 * gridUnitSize - 3, textSignature);

	pDC->SetBkMode(oldBkMode);
	pDC->SetTextAlign(oldTextAlign);
	pDC->SetTextColor(oldTextColor);

	pDC->SelectObject(oldFont);

	signatureFont.DeleteObject();
}

void CMetafajloviView::DrawFigure(CDC* pDC)
{
	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	DrawCactus(pDC);
	pDC->SetWorldTransform(&oldWorldTransform);
	DrawBase(pDC);

	pDC->SetWorldTransform(&oldWorldTransform);
}

void CMetafajloviView::DrawCactus(CDC* pDC)
{
	// Base Nodes

	Translate(pDC, 10 * gridUnitSize, 17 * gridUnitSize);
	XFORM centerLowerCactusCircle;
	pDC->GetWorldTransform(&centerLowerCactusCircle);

	Rotate(pDC, (FLOAT)DEG2RAD(180.f + deltaAngle * lowerRotatingCactusNodeAngle));
	DrawCactusNode(pDC, CactusNodeSize::LARGE, TRUE);

	Translate(pDC, 0.f * gridUnitSize, cactusNodeHeight);
	XFORM centerUpperCactusCircle;
	pDC->GetWorldTransform(&centerUpperCactusCircle);

	// Left branch

	Rotate(pDC, (FLOAT)DEG2RAD(-45.f));
	DrawCactusNode(pDC, CactusNodeSize::SMALL);

	Translate(pDC, 0.f * gridUnitSize, cactusNodeHeight);
	XFORM leftLowerCactusCircle;
	pDC->GetWorldTransform(&leftLowerCactusCircle);

	Rotate(pDC, (FLOAT)DEG2RAD(-45.f));
	DrawCactusNode(pDC, CactusNodeSize::MEDIUM);

	pDC->SetWorldTransform(&leftLowerCactusCircle);

	Rotate(pDC, (FLOAT)DEG2RAD(45.f));
	DrawCactusNode(pDC, CactusNodeSize::MEDIUM);

	Translate(pDC, 0.f * gridUnitSize, cactusNodeHeight);
	XFORM leftUpperCactusCircle;
	pDC->GetWorldTransform(&leftUpperCactusCircle);

	DrawCactusNode(pDC, CactusNodeSize::MEDIUM);

	// !Left branch

	pDC->SetWorldTransform(&centerUpperCactusCircle);

	// Middle branch

	DrawCactusNode(pDC, CactusNodeSize::SMALL);

	// !Middle branch

	pDC->SetWorldTransform(&centerUpperCactusCircle);

	// Right branch

	Rotate(pDC, (FLOAT)DEG2RAD(45.f));
	DrawCactusNode(pDC, CactusNodeSize::SMALL);

	Translate(pDC, 0.f * gridUnitSize, cactusNodeHeight);
	XFORM rightLowerCactusCircle;
	pDC->GetWorldTransform(&rightLowerCactusCircle);

	Rotate(pDC, (FLOAT)DEG2RAD(0.f + upperRotatingCactusNodeAngle * deltaAngle));
	DrawCactusNode(pDC, CactusNodeSize::MEDIUM, TRUE);

	pDC->SetWorldTransform(&rightLowerCactusCircle);

	Rotate(pDC, (FLOAT)DEG2RAD(45.f));
	DrawCactusNode(pDC, CactusNodeSize::MEDIUM);

	Translate(pDC, 0.f * gridUnitSize, cactusNodeHeight);
	XFORM rightUpperCactusCircle;
	pDC->GetWorldTransform(&rightUpperCactusCircle);

	Rotate(pDC, (FLOAT)DEG2RAD(45.f));
	DrawCactusNode(pDC, CactusNodeSize::MEDIUM);

	pDC->SetWorldTransform(&rightUpperCactusCircle);

	Rotate(pDC, (FLOAT)DEG2RAD(-45.f));
	DrawCactusNode(pDC, CactusNodeSize::MEDIUM);

	// !Right branch

	pDC->SetWorldTransform(&rightUpperCactusCircle);
	DrawCactusCircle(pDC);
	pDC->SetWorldTransform(&rightLowerCactusCircle);
	DrawCactusCircle(pDC);
	pDC->SetWorldTransform(&leftUpperCactusCircle);
	DrawCactusCircle(pDC);
	pDC->SetWorldTransform(&leftLowerCactusCircle);
	DrawCactusCircle(pDC);
	pDC->SetWorldTransform(&centerUpperCactusCircle);
	DrawCactusCircle(pDC);
	pDC->SetWorldTransform(&centerLowerCactusCircle);
	DrawCactusCircle(pDC);
}

void CMetafajloviView::DrawBase(CDC* pDC)
{
	CBrush potBrush{ potColor };

	CBrush* oldBrush = pDC->SelectObject(&potBrush);

	const CPoint basePot[] = {
		CPoint{ (INT)(7.5f * gridUnitSize), (INT)(17.f * gridUnitSize)},
		CPoint{ (INT)(7.5f * gridUnitSize), (INT)(18.f * gridUnitSize)},
		CPoint{ (INT)(8.f * gridUnitSize), (INT)(18.f * gridUnitSize)},
		CPoint{ (INT)(8.5f * gridUnitSize), (INT)(20.f * gridUnitSize)},
		CPoint{ (INT)(11.5f * gridUnitSize), (INT)(20.f * gridUnitSize)},
		CPoint{ (INT)(12.f * gridUnitSize), (INT)(18.f * gridUnitSize)},
		CPoint{ (INT)(12.5f * gridUnitSize), (INT)(18.f * gridUnitSize)},
		CPoint{ (INT)(12.5f * gridUnitSize), (INT)(17.f * gridUnitSize)},
	};

	pDC->Polygon(basePot, 8);

	pDC->SelectObject(oldBrush);

	potBrush.DeleteObject();
}

void CMetafajloviView::Translate(CDC* pDC, FLOAT Dx, FLOAT Dy, DWORD mode)
{
	const XFORM matrix{
		1.f, 0.f,
		0.f, 1.f,
		Dx, Dy,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CMetafajloviView::Rotate(CDC* pDC, FLOAT angle, DWORD mode)
{
	const XFORM matrix{
		cosf(angle), sinf(angle),
		-sinf(angle), cosf(angle),
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CMetafajloviView::Scale(CDC* pDC, FLOAT Sx, FLOAT Sy, DWORD mode)
{
	const XFORM matrix{
		Sx, 0.f,
		0.f, Sy,
		0.f, 0.f,
	};
	pDC->ModifyWorldTransform(&matrix, mode);
}

void CMetafajloviView::DrawCactusNode(CDC* pDC, CactusNodeSize size, BOOL light)
{
	pDC->PlayMetaFile(light ? emfCactusLight : emfCactus,
		CRect{ -(INT)size * cactusNodeWidthUnit, 0, (INT)size * cactusNodeWidthUnit, cactusNodeHeight });
}

void CMetafajloviView::DrawCactusCircle(CDC* pDC)
{
	CGdiObject* oldPen = pDC->SelectStockObject(BLACK_PEN);
	CBrush greenBrush{ cactusNodeColor };

	CBrush* oldBrush = pDC->SelectObject(&greenBrush);

	pDC->Ellipse(CRect{ -cactusCircleRadius, -cactusCircleRadius, cactusCircleRadius, cactusCircleRadius });

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	greenBrush.DeleteObject();
}


// CMetafajloviView diagnostics

#ifdef _DEBUG
void CMetafajloviView::AssertValid() const
{
	CView::AssertValid();
}

void CMetafajloviView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMetafajloviDoc* CMetafajloviView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMetafajloviDoc)));
	return (CMetafajloviDoc*)m_pDocument;
}
#endif //_DEBUG


// CMetafajloviView message handlers


void CMetafajloviView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G')
		gridActive = !gridActive, Invalidate();

	if (nChar == 'A')
		--upperRotatingCactusNodeAngle, Invalidate();

	if (nChar == 'D')
		++upperRotatingCactusNodeAngle, Invalidate();

	if (nChar == VK_LEFT)
		--lowerRotatingCactusNodeAngle, Invalidate();

	if (nChar == VK_RIGHT)
		++lowerRotatingCactusNodeAngle, Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMetafajloviView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	windowWidth = cx, windowHeight = cy;
	windowWidthInUnitSize = cx / gridUnitSize, windowHeightInUnitSize = cy / gridUnitSize;
}
