
// AnisotropicMappingModeView.h : interface of the CAnisotropicMappingModeView class
//

#pragma once


class CAnisotropicMappingModeView : public CView
{
protected: // create from serialization only
	CAnisotropicMappingModeView() noexcept;
	DECLARE_DYNCREATE(CAnisotropicMappingModeView)

// Attributes
public:
	CAnisotropicMappingModeDoc* GetDocument() const;

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
public:
	virtual ~CAnisotropicMappingModeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AnisotropicMappingModeView.cpp
inline CAnisotropicMappingModeDoc* CAnisotropicMappingModeView::GetDocument() const
   { return reinterpret_cast<CAnisotropicMappingModeDoc*>(m_pDocument); }
#endif

