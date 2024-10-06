
// MetafileOpsView.h : interface of the CMetafileOpsView class
//

#pragma once

#define _USE_MATH_DEFINES
#include "math.h"

#include "DImage.h"

class CMetafileOpsView : public CView
{
protected: // create from serialization only
	CMetafileOpsView() noexcept;
	DECLARE_DYNCREATE(CMetafileOpsView)

	// Attributes
	CRect m_clientRect;

	CONST INT GRID_UNIT_SIZE;
	BOOL m_gridActive;

	BOOL m_sceneTranslating;
	CPoint m_origin, m_oldMousePoint;
	FLOAT m_scaleFactor;

	HENHMETAFILE m_emfClio;
	CONST INT CLIO_WIDTH, CLIO_HEIGHT;

	DImage m_imgWheel;
	FLOAT m_angleWheel;
	CPoint m_clioPosition;

	BOOL m_alreadyFiltered;
public:
	CMetafileOpsDoc* GetDocument() const;

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
	void DrawClio(CDC* pDC);

	void FilterBitmapColor(CBitmap& bmp, COLORREF color = RGB(255, 0, 0), long threshold = 30);
	void DrawBitmapTransparent(CDC* pDC, CBitmap& bmp, int x, int y);
	void DrawWheel(CDC* pDC);

	void DrawClioWithWheels(CDC* pDC);

	void DrawScene(CDC* pDC);

	void DrawGrid(CDC* pDC, INT gridUnitSize);

	void Translate(CDC* pDC, FLOAT Dx, FLOAT Dy, DWORD mode = MWT_LEFTMULTIPLY);
	void Rotate(CDC* pDC, FLOAT angle, DWORD mode = MWT_LEFTMULTIPLY);
	void Scale(CDC* pDC, FLOAT Sx, FLOAT Sy, DWORD mode = MWT_LEFTMULTIPLY);

	inline FLOAT Deg2Rad(FLOAT degrees) { return degrees * M_PI / 180.f; }
public:
	virtual ~CMetafileOpsView();
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
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in MetafileOpsView.cpp
inline CMetafileOpsDoc* CMetafileOpsView::GetDocument() const
{
	return reinterpret_cast<CMetafileOpsDoc*>(m_pDocument);
}
#endif

