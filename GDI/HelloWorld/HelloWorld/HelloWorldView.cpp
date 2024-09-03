
// HelloWorldView.cpp : implementation of the CHelloWorldView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "HelloWorld.h"
#endif

#include "HelloWorldDoc.h"
#include "HelloWorldView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHelloWorldView

IMPLEMENT_DYNCREATE(CHelloWorldView, CView)

BEGIN_MESSAGE_MAP(CHelloWorldView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CHelloWorldView construction/destruction

CHelloWorldView::CHelloWorldView() noexcept
{
	// TODO: add construction code here

}

CHelloWorldView::~CHelloWorldView()
{
}

BOOL CHelloWorldView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CHelloWorldView drawing

void CHelloWorldView::OnDraw(CDC* pDC)
{
	CHelloWorldDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawLines(pDC);
	WriteHelloWorld(pDC);
}


// CHelloWorldView printing

BOOL CHelloWorldView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CHelloWorldView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CHelloWorldView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CHelloWorldView::DrawLines(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	int width = clientRect.Width();
	int height = clientRect.Height();

	pDC->MoveTo(0, 0);
	pDC->LineTo(width, height);

	pDC->MoveTo(width, 0);
	pDC->LineTo(0, height);

	pDC->MoveTo(width / 2, 0);
	pDC->LineTo(width / 2, height);

	pDC->MoveTo(0, height / 2);
	pDC->LineTo(width, height / 2);
}

void CHelloWorldView::WriteHelloWorld(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(&clientRect);

	int width = clientRect.Width(), height = clientRect.Height();

	CString helloMfcText = _T("Hello, MFC!");

	COLORREF pOldTextColor = pDC->SetTextColor(RGB(0, 0, 255));

	CFont font;
	font.CreatePointFont(500, _T("Helvetica"));
	CFont* pOldFont = pDC->SelectObject(&font);

	CSize textSize = pDC->GetTextExtent(_T("A"));
	int fontHeight = textSize.cy;
	CRect textLabel(width / 2 - 250, height / 2 - fontHeight / 2, width / 2 + 250, height / 2 + fontHeight / 2);

	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(helloMfcText, textLabel, DT_CENTER | DT_SINGLELINE);

	pDC->SetBkMode(oldBkMode);
	pDC->SetTextColor(pOldTextColor);
	pDC->SelectObject(pOldFont);
}



// CHelloWorldView diagnostics

#ifdef _DEBUG
void CHelloWorldView::AssertValid() const
{
	CView::AssertValid();
}

void CHelloWorldView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHelloWorldDoc* CHelloWorldView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHelloWorldDoc)));
	return (CHelloWorldDoc*)m_pDocument;
}
#endif //_DEBUG


// CHelloWorldView message handlers
