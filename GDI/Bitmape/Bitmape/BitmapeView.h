
// BitmapeView.h : interface of the CBitmapeView class
//

#pragma once

#include "DImage.h"


class CBitmapeView : public CView
{
protected: // create from serialization only
	CBitmapeView() noexcept;
	DECLARE_DYNCREATE(CBitmapeView)

	// Attributes
	const int gridUnitSize = 500 / 10;
	BOOL gridActive;

	DImage monaImages[3][3];

	CPoint center{ 0, 0 };

	FLOAT scaleFactor = 1.f;
	const FLOAT deltaScaleFactor = .05f;

	int windowWidth, windowHeight;
public:
	CBitmapeDoc* GetDocument() const;

	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// Implementations
	virtual void DrawMonaImages(CDC* pDC, FLOAT scaleFactor, CPoint center);
	virtual void DrawTransparent(CDC* pDC, DImage& img, int x = 0, int y = 0);

	virtual void Translate(CDC* pDC, FLOAT Dx, FLOAT Dy, DWORD mode = MWT_LEFTMULTIPLY);
	virtual void Scale(CDC* pDC, FLOAT Sx, FLOAT Sy, DWORD mode = MWT_LEFTMULTIPLY);
	virtual void Rotate(CDC* pDC, FLOAT angle, DWORD mode = MWT_LEFTMULTIPLY);
	virtual void Mirror(CDC* pDC, BOOL Mx, BOOL My, DWORD mode = MWT_LEFTMULTIPLY);

	virtual void DrawGrid(CDC* pDC);
	virtual void CalculateWindowMetrics(CDC* pDC);
public:
	virtual ~CBitmapeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in BitmapeView.cpp
inline CBitmapeDoc* CBitmapeView::GetDocument() const
{
	return reinterpret_cast<CBitmapeDoc*>(m_pDocument);
}
#endif

