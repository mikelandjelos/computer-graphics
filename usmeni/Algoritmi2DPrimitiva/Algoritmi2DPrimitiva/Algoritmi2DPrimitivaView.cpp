
// Algoritmi2DPrimitivaView.cpp : implementation of the CAlgoritmi2DPrimitivaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Algoritmi2DPrimitiva.h"
#endif

#include "Algoritmi2DPrimitivaDoc.h"
#include "Algoritmi2DPrimitivaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _USE_MATH_DEFINES
#include "math.h"

#define DEG2RAD(x) (x * M_PI / 180.f)

// CAlgoritmi2DPrimitivaView

IMPLEMENT_DYNCREATE(CAlgoritmi2DPrimitivaView, CView)

BEGIN_MESSAGE_MAP(CAlgoritmi2DPrimitivaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAlgoritmi2DPrimitivaView construction/destruction

CAlgoritmi2DPrimitivaView::CAlgoritmi2DPrimitivaView() noexcept
{
	// TODO: add construction code here

}

void CAlgoritmi2DPrimitivaView::IscrtavanjeLinija(CDC* pDC, void(CAlgoritmi2DPrimitivaView::* pFuncIscrtavanje)(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF boja), BOOL bDrawGdi, BOOL bDrawUser)
{
	CONST INT N = 9;
	// Definisanje linija.
	INT linije[N][4] = {
		{ 0, 50, 50, 0, },
		{ 0, 50, -50, 0, },
		{ -50, 0, 0, -50, },
		{ 0, -50, 50, 0, },
		{ 10, 10, 100, 100, },
		{ 10, 10, 30, 100, },
		{ 100, 30, 10, 10, },
		{ 10, 10, 10, 100, },
		{ 10, 10, 100, 10, },
	};

	for (int i = 0; i < N; i++)
	{
		// GDI API.
		if (bDrawGdi)
		{
			pDC->MoveTo(linije[i][0], linije[i][1]);
			pDC->LineTo(linije[i][2], linije[i][3]);
		}

		// Moja funkcija.
		if (bDrawUser)
			(this->*pFuncIscrtavanje)(pDC, linije[i][0], linije[i][1], linije[i][2], linije[i][3], RGB(255, 0, 0));
	}
}

void CAlgoritmi2DPrimitivaView::Linije(CDC* pDC)
{
	XFORM oldTransform;
	pDC->GetWorldTransform(&oldTransform);

	XFORM translacijaLevo{
		1.f, 0.f,
		0.f, 1.f,
		0.f, -150.f,
	};

	XFORM translacijaDesno{
		1.f, 0.f,
		0.f, 1.f,
		0.f, 150.f,
	};

	pDC->ModifyWorldTransform(&translacijaLevo, MWT_LEFTMULTIPLY);
	IscrtavanjeLinija(pDC, &CAlgoritmi2DPrimitivaView::LinijaNagibni, FALSE, TRUE);

	pDC->ModifyWorldTransform(&translacijaDesno, MWT_LEFTMULTIPLY);
	IscrtavanjeLinija(pDC, &CAlgoritmi2DPrimitivaView::LinijaInkrementalni, FALSE, TRUE);

	pDC->ModifyWorldTransform(&translacijaDesno, MWT_LEFTMULTIPLY);
	IscrtavanjeLinija(pDC, &CAlgoritmi2DPrimitivaView::LinijaBresenham, FALSE, TRUE);

	pDC->SetWorldTransform(&oldTransform);
}

BOOL CAlgoritmi2DPrimitivaView::LinijaPreprocesiranjeKoordinata(int& x0, int& y0, int& x1, int& y1)
{
	// Izracunaj ako je nagib linije veci od 45 stepeni.
	BOOL velikiNagib = abs(y1 - y0) > abs(x1 - x0);
	int xchgBuff;

	// Zamena koordinata.
	if (velikiNagib)
	{
		xchgBuff = x0;
		x0 = y0;
		y0 = xchgBuff;

		xchgBuff = x1;
		x1 = y1;
		y1 = xchgBuff;
	}

	// Crtamo uvek sleva => udesno.
	if (x0 > x1)
	{
		xchgBuff = x0;
		x0 = x1;
		x1 = xchgBuff;

		xchgBuff = y0;
		y0 = y1;
		y1 = xchgBuff;
	}

	return velikiNagib;
}

void CAlgoritmi2DPrimitivaView::LinijaNagibni(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF boja)
{
	BOOL osaSemplovanjaZamenjena = LinijaPreprocesiranjeKoordinata(x0, y0, x1, y1);

	float dx = x1 - x0, dy = y1 - y0;
	float k = dy / dx, b = y1 - k * x1;
	float y;

	for (int x = x0; x <= x1; x++)
		y = k * x + b + 0.5f,
		osaSemplovanjaZamenjena
		? PostaviPiksel(pDC, int(y), x, boja)
		: PostaviPiksel(pDC, x, int(y), boja);
}

void CAlgoritmi2DPrimitivaView::LinijaInkrementalni(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF boja)
{
	BOOL osaSemplovanjaZamenjena = LinijaPreprocesiranjeKoordinata(x0, y0, x1, y1);

	float dx = x1 - x0, dy = y1 - y0;
	float k = dy / dx;
	float y = y0;

	for (int x = x0; x <= x1; x++, y += k)
		osaSemplovanjaZamenjena
		? PostaviPiksel(pDC, int(y + 0.5f), x, boja)
		: PostaviPiksel(pDC, x, int(y + 0.5f), boja);
}

void CAlgoritmi2DPrimitivaView::LinijaBresenham(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF boja)
{
	BOOL osaSemplovanjaZamenjena = LinijaPreprocesiranjeKoordinata(x0, y0, x1, y1);

	int dx = x1 - x0;
	int dy = abs(y1 - y0);

	int d = (dy << 1) - dx;
	int yStep = y0 > y1 ? -1 : 1;
	int y = y0;

	int incr1 = (dy - dx) << 1, incr2 = dy << 1;

	for (int x = x0; x <= x1; x++) {
		osaSemplovanjaZamenjena
			? PostaviPiksel(pDC, y, x, boja)
			: PostaviPiksel(pDC, x, y, boja);

		d > 0
			? y += yStep, d += incr1
			: d += incr2;
	}
}

void CAlgoritmi2DPrimitivaView::Kruznice(CDC* pDC)
{
	COLORREF plava{ RGB(0, 0, 255u) };
	KruznicaTrigonometrijski(pDC, 50, plava);
	KruznicaPolinomni(pDC, 75, plava);
	KruznicaBresenham(pDC, 100, plava);
}

void CAlgoritmi2DPrimitivaView::KruznicaTrigonometrijski(CDC* pDC, int r, COLORREF boja)
{
	const float deltaAngleRad = 2 * M_PI / (7.f * r);
	int x, y;
	for (float angleRad = 0.f; angleRad < M_PI_4; angleRad += deltaAngleRad)
		x = int(r * cosf(angleRad) + 0.5f), y = int(r * sinf(angleRad) + 0.5f),
		PostaviPiksel8(pDC, x, y, boja);
}

void CAlgoritmi2DPrimitivaView::KruznicaPolinomni(CDC* pDC, int r, COLORREF boja)
{
	const int xEnd = int(r / M_SQRT2 + 0.5f);
	for (int x = 0, y = r; x <= xEnd; ++x, y = int(sqrtf(r * r - x * x) + 0.5f))
		PostaviPiksel8(pDC, x, y, boja);
}

void CAlgoritmi2DPrimitivaView::KruznicaBresenham(CDC* pDC, int r, COLORREF boja)
{
	int d;
	d = 3 - (r << 1);

	for (int y = 0, x = r; y <= x; ++y)
	{
		PostaviPiksel8(pDC, x, y, boja);
		d < 0
			? d += 6 + (y << 2)
			: (d += 10 + ((y - x) << 2), x--);
	}
}

void CAlgoritmi2DPrimitivaView::Elipse(CDC* pDC)
{

	XFORM oldTransform;
	pDC->GetWorldTransform(&oldTransform);

	const float angleRad = DEG2RAD(90.f);
	XFORM rotacija90{
		cosf(angleRad), sinf(angleRad),
		-sinf(angleRad), cosf(angleRad),
		0.f, 0.f,
	};

	pDC->ModifyWorldTransform(&rotacija90, MWT_LEFTMULTIPLY);

	COLORREF zelena{ RGB(0, 255, 0) };

	ElipsaTrigonometrijski(pDC, 100, 50, zelena);
	ElipsaTrigonometrijski(pDC, 50, 100, zelena);

	ElipsaPolinomni(pDC, 125, 62, zelena);
	ElipsaPolinomni(pDC, 62, 125, zelena);

	ElipsaDiferencijalI(pDC, 150.f, 75.f, zelena);
	ElipsaDiferencijalI(pDC, 75.f, 150.f, zelena);

	ElipsaDiferencijalII(pDC, 175.f, 87.f, zelena);
	ElipsaDiferencijalII(pDC, 87.f, 175.f, zelena);

	pDC->SetWorldTransform(&oldTransform);
}

void CAlgoritmi2DPrimitivaView::ElipsaTrigonometrijski(CDC* pDC, int a, int b, COLORREF boja)
{
	int x, y;
	const float deltaAngleRad = 2 * M_PI / (7 * max(a, b));
	for (float angleRad = 0; angleRad < M_PI_2; angleRad += deltaAngleRad)
		x = a * cosf(angleRad), y = b * sinf(angleRad),
		PostaviPiksel4(pDC, x, y, boja);
}

void CAlgoritmi2DPrimitivaView::ElipsaPolinomni(CDC* pDC, int a, int b, COLORREF boja)
{
	int aKraj = a > b ? a : b - a;
	int bKraj = b > a ? b : a - b;
	float y, x, aKvadrat = a * a, bKvadrat = b * b;
	for (int x = 0; x < aKraj; ++x)
		y = int(b * sqrtf(1 - x * x / aKvadrat) + 0.5f),
		PostaviPiksel4(pDC, x, y, boja);
	for (int y = 0; y < bKraj; ++y)
		x = int(a * sqrtf(1 - y * y / bKvadrat) + 0.5f),
		PostaviPiksel4(pDC, x, y, boja);
}

void CAlgoritmi2DPrimitivaView::ElipsaDiferencijalI(CDC* pDC, float a, float b, COLORREF boja)
{
	const float deltaPhi = DEG2RAD(1.f);
	const float ba = deltaPhi * b / a, ab = deltaPhi * a / b;
	float x0 = a, y0 = 0.f, x1, y1;
	for (int i = 0; i < 90; ++i)
	{
		x1 = x0 - ab * y0;
		y1 = y0 + ba * x0;

		LinijaBresenham(pDC, x0, y0, x1, y1, boja);
		LinijaBresenham(pDC, x0, -y0, x1, -y1, boja);
		LinijaBresenham(pDC, -x0, y0, -x1, y1, boja);
		LinijaBresenham(pDC, -x0, -y0, -x1, -y1, boja);

		x0 = x1;
		y0 = y1;
	}
}

void CAlgoritmi2DPrimitivaView::ElipsaDiferencijalII(CDC* pDC, float a, float b, COLORREF boja)
{
	const float deltaPhi = DEG2RAD(1.f);
	const float k = 2 - deltaPhi * deltaPhi;
	float x0 = a, y0 = 0.f;
	float x1 = a * cosf(deltaPhi), y1 = b * sinf(deltaPhi);
	float x2, y2;
	for (int i = 0; i < 90; ++i)
	{
		LinijaBresenham(pDC, x0, y0, x1, y1, boja);
		LinijaBresenham(pDC, x0, -y0, x1, -y1, boja);
		LinijaBresenham(pDC, -x0, y0, -x1, y1, boja);
		LinijaBresenham(pDC, -x0, -y0, -x1, -y1, boja);

		x2 = k * x1 - x0;
		y2 = k * y1 - y0;
		x0 = x1;
		y0 = y1;
		x1 = x2;
		y1 = y2;
	}
}

CAlgoritmi2DPrimitivaView::~CAlgoritmi2DPrimitivaView()
{
}

BOOL CAlgoritmi2DPrimitivaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CAlgoritmi2DPrimitivaView drawing

void CAlgoritmi2DPrimitivaView::OnDraw(CDC* pDC)
{
	CAlgoritmi2DPrimitivaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	int oldGraphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);

	XFORM oldWorldTransform;
	pDC->GetWorldTransform(&oldWorldTransform);

	float angleRad = DEG2RAD(-90.f);

	XFORM newTransform = {
		2.f * cosf(angleRad), sinf(angleRad),
		-sinf(angleRad), 2.f * cosf(angleRad),
		clientRect.Width() / 2.f, clientRect.Height() / 2.f,
	};

	pDC->ModifyWorldTransform(&newTransform, MWT_LEFTMULTIPLY);

	int coordLen = 10;

	pDC->MoveTo(-coordLen, 0);
	pDC->LineTo(coordLen, 0);

	pDC->MoveTo(0, -coordLen);
	pDC->LineTo(0, coordLen);

	IscrtajScenu(pDC);

	pDC->SetWorldTransform(&oldWorldTransform);
	pDC->SetGraphicsMode(oldGraphicsMode);
}


// CAlgoritmi2DPrimitivaView printing

BOOL CAlgoritmi2DPrimitivaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAlgoritmi2DPrimitivaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAlgoritmi2DPrimitivaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CAlgoritmi2DPrimitivaView::PostaviPiksel(CDC* pDC, int x, int y, COLORREF boja)
{
	pDC->SetPixel(x, y, boja);
}

void CAlgoritmi2DPrimitivaView::PostaviPiksel4(CDC* pDC, int x, int y, COLORREF boja)
{
	pDC->SetPixel(x, y, boja);
	pDC->SetPixel(x, -y, boja);
	pDC->SetPixel(-x, y, boja);
	pDC->SetPixel(-x, -y, boja);
}

void CAlgoritmi2DPrimitivaView::PostaviPiksel8(CDC* pDC, int x, int y, COLORREF boja)
{
	pDC->SetPixel(x, y, boja);
	pDC->SetPixel(x, -y, boja);
	pDC->SetPixel(-x, y, boja);
	pDC->SetPixel(-x, -y, boja);
	pDC->SetPixel(y, x, boja);
	pDC->SetPixel(y, -x, boja);
	pDC->SetPixel(-y, x, boja);
	pDC->SetPixel(-y, -x, boja);
}

void CAlgoritmi2DPrimitivaView::IscrtajScenu(CDC* pDC)
{
	XFORM oldTransform;
	pDC->GetWorldTransform(&oldTransform);

	CRect clientRect;
	GetClientRect(&clientRect);

	XFORM linije{
		1.f, 0.f,
		0.f, 1.f,
		clientRect.Height() / 4, -clientRect.Width() / 4,
	};

	XFORM kruznice{
		1.f, 0.f,
		0.f, 1.f,
		clientRect.Height() / 4, clientRect.Width() / 4,
	};

	pDC->ModifyWorldTransform(&linije, MWT_LEFTMULTIPLY);
	Linije(pDC);

	pDC->SetWorldTransform(&oldTransform);
	pDC->ModifyWorldTransform(&kruznice, MWT_LEFTMULTIPLY);
	Kruznice(pDC);

	pDC->SetWorldTransform(&oldTransform);
	Elipse(pDC);

	pDC->SetWorldTransform(&oldTransform);
}


// CAlgoritmi2DPrimitivaView diagnostics

#ifdef _DEBUG
void CAlgoritmi2DPrimitivaView::AssertValid() const
{
	CView::AssertValid();
}

void CAlgoritmi2DPrimitivaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAlgoritmi2DPrimitivaDoc* CAlgoritmi2DPrimitivaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAlgoritmi2DPrimitivaDoc)));
	return (CAlgoritmi2DPrimitivaDoc*)m_pDocument;
}
#endif //_DEBUG


// CAlgoritmi2DPrimitivaView message handlers
