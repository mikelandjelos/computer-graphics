
// CurvedTextView.h : interface of the CCurvedTextView class
//

#pragma once


class CCurvedTextView : public CView
{
protected: // create from serialization only
	CCurvedTextView() noexcept;
	DECLARE_DYNCREATE(CCurvedTextView)

	// Attributes
	CONST INT GRID_UNIT_SIZE;
	BOOL m_gridActive;
	CRect m_clientRect;

	// View manipulation.
	BOOL m_translating;
	FLOAT m_scalingFactor;
	CPoint m_origin, m_oldMousePoint;
public:
	CCurvedTextDoc* GetDocument() const;

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
	void DrawCurvedText(CDC* pDC, CString text);

	void Translate(CDC* pDC, FLOAT dX, FLOAT dY, DWORD mode = MWT_LEFTMULTIPLY);
	void Rotate(CDC* pDC, FLOAT angle, DWORD mode = MWT_LEFTMULTIPLY);
	void Scale(CDC* pDC, FLOAT sX, FLOAT sY, DWORD mode = MWT_LEFTMULTIPLY);
	void DrawGrid(CDC* pDC, int gridUnitSize);
public:
	virtual ~CCurvedTextView();
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in CurvedTextView.cpp
inline CCurvedTextDoc* CCurvedTextView::GetDocument() const
{
	return reinterpret_cast<CCurvedTextDoc*>(m_pDocument);
}
#endif

