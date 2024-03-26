#if !defined(AFX_RELOGDLG_H__5FF5EE1D_1B00_4729_97DC_C4FDB882DDF2__INCLUDED_)
#define AFX_RELOGDLG_H__5FF5EE1D_1B00_4729_97DC_C4FDB882DDF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RelogDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RelogDlg dialog

class RelogDlg : public CDialog
{
// Construction
public:
	RelogDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RelogDlg)
	//enum { IDD = IDD_DLG_RELOG };
	//CStatic	m_cRelogGraph;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RelogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RelogDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RELOGDLG_H__5FF5EE1D_1B00_4729_97DC_C4FDB882DDF2__INCLUDED_)
