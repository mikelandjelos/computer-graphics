
// Januar2024View.cpp : implementation of the CJanuar2024View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Januar2024.h"
#endif

#include "Januar2024Doc.h"
#include "Januar2024View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJanuar2024View

IMPLEMENT_DYNCREATE(CJanuar2024View, CView)

BEGIN_MESSAGE_MAP(CJanuar2024View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CJanuar2024View construction/destruction

CJanuar2024View::CJanuar2024View() noexcept
{
	// TODO: add construction code here

}

CJanuar2024View::~CJanuar2024View()
{
}

BOOL CJanuar2024View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CJanuar2024View drawing

void CJanuar2024View::OnDraw(CDC* /*pDC*/)
{
	CJanuar2024Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CJanuar2024View printing

BOOL CJanuar2024View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CJanuar2024View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CJanuar2024View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CJanuar2024View diagnostics

#ifdef _DEBUG
void CJanuar2024View::AssertValid() const
{
	CView::AssertValid();
}

void CJanuar2024View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CJanuar2024Doc* CJanuar2024View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJanuar2024Doc)));
	return (CJanuar2024Doc*)m_pDocument;
}
#endif //_DEBUG


// CJanuar2024View message handlers
