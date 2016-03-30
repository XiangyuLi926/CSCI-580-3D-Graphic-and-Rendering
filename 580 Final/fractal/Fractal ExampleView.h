// Fractal ExampleView.h : interface of the CFractalExampleView class
//
/////////////////////////////////////////////////////////////////////////////

//
// Copyright 1997 Paul Martz
//  All Rights Reserved
//

#include "COpenGLView.h"

typedef enum twoDRenderModes {
	twoDR_lines,
	twoDR_clouds,
	twoDR_teximage,
	twoDR_rendered
};

class CFractalExampleView : public COpenGLView
{
protected: // create from serialization only
	CFractalExampleView();
	DECLARE_DYNCREATE(CFractalExampleView)

// Attributes
public:
	float teximageH;
	float surfaceH;
	float cloudH;
	BOOL cloudFAdirty;
	float * cloudFA;
	BOOL teximageFAdirty;
	float * teximageFA;
	BOOL surfFAdirty;
	float * surfFA;
	GLfloat zTrans;
	GLfloat yTrans;
	GLfloat yRot;
	BOOL shiftKeyStatus;
	UINT teximageIter;
	UINT cloudIter;
	BOOL colorInvert;
	UINT tile;
	UINT randomSeed;
	BOOL textureLinear;
	BOOL aaLines;
	virtual void OnSize (UINT nType, int cx, int cy);
	float aspectRatio;
	int currentHeight;
	int currentWidth;
	BOOL renderCloudMap ();
	BOOL renderTeximageMap ();
	BOOL renderFullImage ();
	virtual BOOL RenderScene();
	CFractalExampleDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFractalExampleView)
	public:
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFractalExampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFractalExampleView)
	afx_msg void OnViewOptions();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	UINT iterations;
	twoDRenderModes renderMode;
	BOOL arrayIs2D;
};

#ifndef _DEBUG  // debug version in Fractal ExampleView.cpp
inline CFractalExampleDoc* CFractalExampleView::GetDocument()
   { return (CFractalExampleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
