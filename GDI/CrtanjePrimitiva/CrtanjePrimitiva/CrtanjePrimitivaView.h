
// CrtanjePrimitivaView.h : interface of the CCrtanjePrimitivaView class
//

#pragma once


class CCrtanjePrimitivaView : public CView
{
protected: // create from serialization only
	CCrtanjePrimitivaView() noexcept;
	DECLARE_DYNCREATE(CCrtanjePrimitivaView)

	// Attributes

	int mGridUnitFraction;
public:
	CCrtanjePrimitivaDoc* GetDocument() const;

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

	// User functions

	virtual void DrawSolidRedHeart(CDC* pDC);
	virtual void DrawHatchedPurpleHeart(CDC* pDC);
	virtual void DrawHearthBezier(CDC* pDC);

	// Messages (Events)

	virtual void DrawGrid(CDC* pDC);

	// Implementation
public:
	virtual ~CCrtanjePrimitivaView();
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

#ifndef _DEBUG  // debug version in CrtanjePrimitivaView.cpp
inline CCrtanjePrimitivaDoc* CCrtanjePrimitivaView::GetDocument() const
{
	return reinterpret_cast<CCrtanjePrimitivaDoc*>(m_pDocument);
}
#endif

