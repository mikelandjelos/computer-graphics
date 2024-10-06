
// Algoritmi2DPrimitivaView.h : interface of the CAlgoritmi2DPrimitivaView class
//

#pragma once


class CAlgoritmi2DPrimitivaView : public CView
{
protected: // create from serialization only
	CAlgoritmi2DPrimitivaView() noexcept;
	DECLARE_DYNCREATE(CAlgoritmi2DPrimitivaView)

	// Attributes
public:
	CAlgoritmi2DPrimitivaDoc* GetDocument() const;

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
	void PostaviPiksel(CDC* pDC, int x, int y, COLORREF boja);
	void PostaviPiksel4(CDC* pDC, int x, int y, COLORREF boja);
	void PostaviPiksel8(CDC* pDC, int x, int y, COLORREF boja);

	void IscrtajScenu(CDC* pDC);

	// Linije.
	void TestirajIscrtavanjeLinija(CDC* pDC, void (CAlgoritmi2DPrimitivaView::* pFuncIscrtavanje)(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF boja), BOOL bDrawGdi, BOOL bDrawUser);
	void Linije(CDC* pDC);

	BOOL LinijaPreprocesiranjeKoordinata(int& x0, int& y0, int& x1, int& y1);
	void LinijaNagibni(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF boja = 0UL); // RGB(0, 0, 0) == 0UL
	void LinijaInkrementalni(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF boja = 0UL);
	void LinijaBresenham(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF boja = 0UL);

	// Kruznice.
	void Kruznice(CDC* pDC);

	void KruznicaTrigonometrijski(CDC* pDC, int r, COLORREF boja = 0UL);
	void KruznicaPolinomni(CDC* pDC, int r, COLORREF boja = 0UL);
	void KruznicaBresenham(CDC* pDC, int r, COLORREF boja = 0UL);

	// Elipse.
	void Elipse(CDC* pDC);

	void ElipsaTrigonometrijski(CDC* pDC, int a, int b, COLORREF boja = 0UL);
	void ElipsaPolinomni(CDC* pDC, int a, int b, COLORREF boja = 0UL);
	void ElipsaDiferencijalI(CDC* pDC, float  a, float b, COLORREF boja = 0UL);
	void ElipsaDiferencijalII(CDC* pDC, float a, float b, COLORREF boja = 0UL);
public:
	virtual ~CAlgoritmi2DPrimitivaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Algoritmi2DPrimitivaView.cpp
inline CAlgoritmi2DPrimitivaDoc* CAlgoritmi2DPrimitivaView::GetDocument() const
{
	return reinterpret_cast<CAlgoritmi2DPrimitivaDoc*>(m_pDocument);
}
#endif

