// FractalOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Fractal Example.h"
#include "FractalOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFractalOptionsDlg dialog


CFractalOptionsDlg::CFractalOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFractalOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFractalOptionsDlg)
	m_iterations = 0;
	m_renderType = _T("");
	m_aaLines = FALSE;
	m_randomSeed = 0;
	m_tile = 0;
	m_invert = FALSE;
	m_cloudIter = 0;
	m_teximageIter = 0;
	m_textureLinear = FALSE;
	m_cloudH = 0.0f;
	m_surfaceH = 0.0f;
	m_teximageH = 0.0f;
	//}}AFX_DATA_INIT
}


void CFractalOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFractalOptionsDlg)
	DDX_Text(pDX, IDC_F_ITERATIONS, m_iterations);
	DDV_MinMaxUInt(pDX, m_iterations, 1, 10);
	DDX_CBString(pDX, IDC_F_RENDER_TYPE, m_renderType);
	DDV_MaxChars(pDX, m_renderType, 25);
	DDX_Check(pDX, IDC_F_AA_LINES, m_aaLines);
	DDX_Text(pDX, IDC_F_RSEED, m_randomSeed);
	DDV_MinMaxUInt(pDX, m_randomSeed, 0, 32767);
	DDX_Text(pDX, IDC_F_TILE, m_tile);
	DDV_MinMaxUInt(pDX, m_tile, 1, 10);
	DDX_Check(pDX, IDC_F_INVERT, m_invert);
	DDX_Text(pDX, IDC_F_CLOUD_ITERATIONS, m_cloudIter);
	DDV_MinMaxUInt(pDX, m_cloudIter, 1, 10);
	DDX_Text(pDX, IDC_F_TEXIMAGE_ITERATIONS, m_teximageIter);
	DDV_MinMaxUInt(pDX, m_teximageIter, 1, 10);
	DDX_Check(pDX, IDC_F_TEX_LINEAR, m_textureLinear);
	DDX_Text(pDX, ID_F_CLOUD_H, m_cloudH);
	DDV_MinMaxFloat(pDX, m_cloudH, 0.f, 1.f);
	DDX_Text(pDX, ID_F_SURFACE_H, m_surfaceH);
	DDV_MinMaxFloat(pDX, m_surfaceH, 0.f, 1.f);
	DDX_Text(pDX, ID_F_TEXIMAGE_H, m_teximageH);
	DDV_MinMaxFloat(pDX, m_teximageH, 0.f, 1.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFractalOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CFractalOptionsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFractalOptionsDlg message handlers

