
// ElementaryTransformationsView.h : interface of the CElementaryTransformationsView class
//

#pragma once


class CElementaryTransformationsView : public CView
{
protected: // create from serialization only
	CElementaryTransformationsView() noexcept;
	DECLARE_DYNCREATE(CElementaryTransformationsView)

	int windowWidth, windowHeight;
	int widthInGridUnits, heightInGridUnits;
public:
	CElementaryTransformationsDoc* GetDocument() const;

	const int gridUnitSize = 500 / 20;
	BOOL gridActive;
	BOOL axesActive;

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
	virtual void DrawTriangles(CDC* pDC, CPoint center);
	virtual void DrawHearts(CDC* pDC, CPoint center);

	virtual void DrawHeart(CDC* pDC, COLORREF color);

	virtual void Translate(CDC* pDC, FLOAT Dx, FLOAT Dy, DWORD mode = MWT_LEFTMULTIPLY);
	virtual void Rotate(CDC* pDC, FLOAT angle, DWORD mode = MWT_LEFTMULTIPLY);
	virtual void Scale(CDC* pDC, FLOAT Sx, FLOAT Sy, DWORD mode = MWT_LEFTMULTIPLY);

	virtual void DrawGrid(CDC* pDC);
	virtual void DrawAxes(CDC* pDC);
public:
	virtual ~CElementaryTransformationsView();
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

#ifndef _DEBUG  // debug version in ElementaryTransformationsView.cpp
inline CElementaryTransformationsDoc* CElementaryTransformationsView::GetDocument() const
{
	return reinterpret_cast<CElementaryTransformationsDoc*>(m_pDocument);
}
#endif

