
// RegioniPutanjeView.h : interface of the CRegioniPutanjeView class
//

#pragma once


class CRegioniPutanjeView : public CView
{
protected: // create from serialization only
	CRegioniPutanjeView() noexcept;
	DECLARE_DYNCREATE(CRegioniPutanjeView)

	// Attributes
	BOOL gridActive = FALSE;
	const INT32 gridUnitSize = 500 / 20;

	INT32 windowWidth = -1, windowHeight = -1;
	CPoint center{ 0, 0 };

	CPoint mouseCoordinates{ -1 , -1 };
public:
	CRegioniPutanjeDoc* GetDocument() const;

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
	virtual void DrawGrid(CDC* pDC);

	virtual void Translate(CDC* pDC, FLOAT Dx, FLOAT Dy, DWORD mode = MWT_LEFTMULTIPLY);
	virtual void Scale(CDC* pDC, FLOAT Sx, FLOAT Sy, DWORD mode = MWT_LEFTMULTIPLY);
	virtual void Rotate(CDC* pDC, FLOAT angle, DWORD mode = MWT_LEFTMULTIPLY);

	virtual void DrawRegions(CDC* pDC);
	virtual void FloodFilling(CDC* pDC);
public:
	virtual ~CRegioniPutanjeView();
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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in RegioniPutanjeView.cpp
inline CRegioniPutanjeDoc* CRegioniPutanjeView::GetDocument() const
{
	return reinterpret_cast<CRegioniPutanjeDoc*>(m_pDocument);
}
#endif

