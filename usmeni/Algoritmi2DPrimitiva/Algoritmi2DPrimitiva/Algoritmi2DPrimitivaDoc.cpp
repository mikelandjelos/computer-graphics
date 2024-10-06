
// Algoritmi2DPrimitivaDoc.cpp : implementation of the CAlgoritmi2DPrimitivaDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Algoritmi2DPrimitiva.h"
#endif

#include "Algoritmi2DPrimitivaDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAlgoritmi2DPrimitivaDoc

IMPLEMENT_DYNCREATE(CAlgoritmi2DPrimitivaDoc, CDocument)

BEGIN_MESSAGE_MAP(CAlgoritmi2DPrimitivaDoc, CDocument)
END_MESSAGE_MAP()


// CAlgoritmi2DPrimitivaDoc construction/destruction

CAlgoritmi2DPrimitivaDoc::CAlgoritmi2DPrimitivaDoc() noexcept
{
	// TODO: add one-time construction code here

}

CAlgoritmi2DPrimitivaDoc::~CAlgoritmi2DPrimitivaDoc()
{
}

BOOL CAlgoritmi2DPrimitivaDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CAlgoritmi2DPrimitivaDoc serialization

void CAlgoritmi2DPrimitivaDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CAlgoritmi2DPrimitivaDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CAlgoritmi2DPrimitivaDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAlgoritmi2DPrimitivaDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAlgoritmi2DPrimitivaDoc diagnostics

#ifdef _DEBUG
void CAlgoritmi2DPrimitivaDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAlgoritmi2DPrimitivaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAlgoritmi2DPrimitivaDoc commands
