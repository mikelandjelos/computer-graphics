
// AnisotropicMappingModeView.cpp : implementation of the CAnisotropicMappingModeView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AnisotropicMappingMode.h"
#endif

#include "AnisotropicMappingModeDoc.h"
#include "AnisotropicMappingModeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnisotropicMappingModeView

IMPLEMENT_DYNCREATE(CAnisotropicMappingModeView, CView)

BEGIN_MESSAGE_MAP(CAnisotropicMappingModeView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAnisotropicMappingModeView construction/destruction

CAnisotropicMappingModeView::CAnisotropicMappingModeView() noexcept
{
	// TODO: add construction code here

}

CAnisotropicMappingModeView::~CAnisotropicMappingModeView()
{
}

BOOL CAnisotropicMappingModeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CAnisotropicMappingModeView drawing

void CAnisotropicMappingModeView::OnDraw(CDC* pDC)
{
	CAnisotropicMappingModeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(100, -100);
	pDC->SetViewportExt(clientRect.right, clientRect.bottom);
	pDC->SetWindowOrg(-50, 50);

	pDC->Rectangle(-25, 25, 25, -25);
}


// CAnisotropicMappingModeView printing

BOOL CAnisotropicMappingModeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAnisotropicMappingModeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAnisotropicMappingModeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CAnisotropicMappingModeView diagnostics

#ifdef _DEBUG
void CAnisotropicMappingModeView::AssertValid() const
{
	CView::AssertValid();
}

void CAnisotropicMappingModeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnisotropicMappingModeDoc* CAnisotropicMappingModeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnisotropicMappingModeDoc)));
	return (CAnisotropicMappingModeDoc*)m_pDocument;
}
#endif //_DEBUG


// CAnisotropicMappingModeView message handlers
