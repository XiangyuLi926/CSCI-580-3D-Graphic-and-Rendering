// FractalOptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFractalOptionsDlg dialog

class CFractalOptionsDlg : public CDialog
{
// Construction
public:
	CFractalOptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFractalOptionsDlg)
	enum { IDD = IDD_FRACTAL_OPTIONS };
	UINT	m_iterations;
	CString	m_renderType;
	BOOL	m_aaLines;
	UINT	m_randomSeed;
	UINT	m_tile;
	BOOL	m_invert;
	UINT	m_cloudIter;
	UINT	m_teximageIter;
	BOOL	m_textureLinear;
	float	m_cloudH;
	float	m_surfaceH;
	float	m_teximageH;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFractalOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFractalOptionsDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};
