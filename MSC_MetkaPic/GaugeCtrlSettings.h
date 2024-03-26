#if !defined(AFX_GAUGECTRLSETTINGS_H__37070D06_7EA2_4AF0_9E77_135E5CF8DFB1__INCLUDED_)
#define AFX_GAUGECTRLSETTINGS_H__37070D06_7EA2_4AF0_9E77_135E5CF8DFB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GaugeCtrlSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGaugeCtrlSettings dialog

class CGaugeCtrlSettings : public CDialog
{
// Construction
public:
	CGaugeCtrlSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGaugeCtrlSettings)
	enum { IDD = IDD_GAUGECTRLSETTINGS };
	float	m_fMaxValue1;
	float	m_fMaxValue2;
	float	m_fMinValue1;
	float	m_fMinValue2;
	BOOL	m_bLogarit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGaugeCtrlSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGaugeCtrlSettings)
	virtual void OnOK();
	afx_msg void OnLogarit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAUGECTRLSETTINGS_H__37070D06_7EA2_4AF0_9E77_135E5CF8DFB1__INCLUDED_)
