
// MetafajloviView.h : interface of the CMetafajloviView class
//

#pragma once


class CMetafajloviView : public CView
{
protected: // create from serialization only
	CMetafajloviView() noexcept;
	DECLARE_DYNCREATE(CMetafajloviView)

	// Attributes
	HENHMETAFILE emfCactus;
	HENHMETAFILE emfCactusLight;

	const int gridUnitSize = 500 / 20;
	const int cactusCircleRadius = 0.4 * gridUnitSize;
	const int cactusNodeHeight = 3 * gridUnitSize;
	const int cactusNodeWidthUnit = 0.4 * gridUnitSize;

	const int deltaAngle = 20; // In degrees.

	int lowerRotatingCactusNodeAngle;
	int upperRotatingCactusNodeAngle;

	BOOL gridActive;

	int windowWidth, windowHeight;
	int windowWidthInUnitSize, windowHeightInUnitSize;

	const COLORREF backgroundColor = RGB(135, 206, 235);
	const COLORREF potColor = RGB(222, 148, 0);
	const COLORREF cactusNodeColor = RGB(0, 204, 0);
	const COLORREF textColor = RGB(229, 244, 52);

	enum class CactusNodeSize {
		SMALL = 1,
		MEDIUM = 2,
		LARGE = 3,
	};
public:
	CMetafajloviDoc* GetDocument() const;

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
	virtual void ColorBackground(CDC* pDC);
	virtual void DrawTextSignature(CDC* pDC);
	virtual void DrawFigure(CDC* pDC);

	virtual void DrawCactus(CDC* pDC);
	virtual void DrawBase(CDC* pDC);

	virtual void Translate(CDC* pDC, FLOAT Dx, FLOAT Dy, DWORD mode = MWT_LEFTMULTIPLY);
	virtual void Rotate(CDC* pDC, FLOAT angle, DWORD mode = MWT_LEFTMULTIPLY);
	virtual void Scale(CDC* pDC, FLOAT Sx, FLOAT Sy, DWORD mode = MWT_LEFTMULTIPLY);

	virtual void DrawCactusNode(CDC* pDC, CactusNodeSize size, BOOL light = FALSE);
	virtual void DrawCactusCircle(CDC* pDC);

	virtual void DrawAxes(CDC* pDC);
	virtual void DrawGrid(CDC* pDC);
	virtual void CalculateClientRectangleMeasures(CDC* pDC);
public:
	virtual ~CMetafajloviView();
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

#ifndef _DEBUG  // debug version in MetafajloviView.cpp
inline CMetafajloviDoc* CMetafajloviView::GetDocument() const
{
	return reinterpret_cast<CMetafajloviDoc*>(m_pDocument);
}
#endif

