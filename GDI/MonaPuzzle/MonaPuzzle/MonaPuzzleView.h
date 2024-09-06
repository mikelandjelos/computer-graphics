
// MonaPuzzleView.h : interface of the CMonaPuzzleView class
//

#pragma once

#include "DImage.h"

class CMonaPuzzleView : public CView
{
protected: // create from serialization only
	CMonaPuzzleView() noexcept;
	DECLARE_DYNCREATE(CMonaPuzzleView)

	// Attributes
	DImage monaPuzzles[3][3];

	const int gridUnitSize = 500 / 20;
	BOOL gridActive = FALSE;

	CPoint center{ 0, 0 };

	int windowWidth = -1, windowHeight = -1;
public:
	CMonaPuzzleDoc* GetDocument() const;

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

	// Implementation
	virtual void CalculateWindowMeasures(CDC* pDC);
	virtual void DrawGrid(CDC* pDC);

	virtual void Translate(CDC* pDC, FLOAT Dx, FLOAT Dy, DWORD mode = MWT_LEFTMULTIPLY);
	virtual void Scale(CDC* pDC, FLOAT Sx, FLOAT Sy, DWORD mode = MWT_LEFTMULTIPLY);
	virtual void Rotate(CDC* pDC, FLOAT angleInRadians, DWORD mode = MWT_LEFTMULTIPLY);
	virtual void Mirror(CDC* pDC, BOOL Mx, BOOL My, DWORD mode = MWT_LEFTMULTIPLY);

	virtual void AntiFlicker(CDC* pDC);

	virtual void CopyBitmap(CDC* pDC, CBitmap* dstBmp, CBitmap* srcBmp);
	virtual void DrawPuzzle(CDC* pDC, DImage& image, int x = 0, int y = 0, BOOL blueFilter = FALSE);
	virtual void ConvertToGreyscale(CBitmap* bitmap);
	virtual void BlueFilter(CBitmap* bitmap);

	virtual void AssembleMona(CDC* pDC);
public:
	virtual ~CMonaPuzzleView();
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

#ifndef _DEBUG  // debug version in MonaPuzzleView.cpp
inline CMonaPuzzleDoc* CMonaPuzzleView::GetDocument() const
{
	return reinterpret_cast<CMonaPuzzleDoc*>(m_pDocument);
}
#endif

