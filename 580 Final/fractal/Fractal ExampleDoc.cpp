// Fractal ExampleDoc.cpp : implementation of the CFractalExampleDoc class
//

#include "stdafx.h"
#include "Fractal Example.h"

#include "Fractal ExampleDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFractalExampleDoc

IMPLEMENT_DYNCREATE(CFractalExampleDoc, CDocument)

BEGIN_MESSAGE_MAP(CFractalExampleDoc, CDocument)
	//{{AFX_MSG_MAP(CFractalExampleDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFractalExampleDoc construction/destruction

CFractalExampleDoc::CFractalExampleDoc()
{
	// TODO: add one-time construction code here

}

CFractalExampleDoc::~CFractalExampleDoc()
{
}

BOOL CFractalExampleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFractalExampleDoc serialization

void CFractalExampleDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CFractalExampleDoc diagnostics

#ifdef _DEBUG
void CFractalExampleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFractalExampleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFractalExampleDoc commands
