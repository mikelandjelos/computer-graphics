
// TangramView.h : interface of the CTangramView class
//

#pragma once


class CTangramView : public CView
{
protected: // create from serialization only
	CTangramView() noexcept;
	DECLARE_DYNCREATE(CTangramView)

	// Constants
	const INT cUnitSize = 500 / 20;
	const COLORREF colorLightGray = RGB(221, 221, 221);
	const COLORREF colorCyanBLue = RGB(0, 255, 255);
	const COLORREF colorYellow = RGB(255, 255, 0);
	const COLORREF colorPastelBlue = RGB(102, 153, 255);
	const COLORREF colorRed = RGB(255, 0, 0);
	const COLORREF colorPink = RGB(255, 153, 204);
	const COLORREF colorPurple = RGB(153, 0, 204);
	const COLORREF colorGreen = RGB(50, 205, 50);
	const COLORREF colorOrange = RGB(255, 153, 51);

	BOOL mGridActive;
	// Attributes
public:
	CTangramDoc* GetDocument() const;

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

	virtual void DrawGrid(CDC* pDC);
	virtual void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	// Implementation
public:
	virtual ~CTangramView();
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
};

#ifndef _DEBUG  // debug version in TangramView.cpp
inline CTangramDoc* CTangramView::GetDocument() const
{
	return reinterpret_cast<CTangramDoc*>(m_pDocument);
}
#endif

