#if !defined(AFX_POSITION_H__1548406C_83B0_4BC7_B985_57F50058DEA7__INCLUDED_)
#define AFX_POSITION_H__1548406C_83B0_4BC7_B985_57F50058DEA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Position.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPosition dialog

class CPosition : public CDialog
{
// Construction
public:
	CPosition(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPosition)
	enum { IDD = IDD_POSITION };
	CButton	m_cMsc_60;
	CComboBox	m_cCB16;
	CButton	m_cAK4_T;
	CButton	m_cAK44;
	CButton	m_cAK43;
	CButton	m_cAK42;
	CButton	m_cAK41;
	CButton	m_cMSCTest;
	CButton	m_cMSC;
	CComboBox	m_cCB14;
	CComboBox	m_cCB13;
	CComboBox	m_cCB12;
	CComboBox	m_cCB11;
	CComboBox	m_cCB10;
	CComboBox	m_cCB9;
	CButton	m_cSKPTest;
	CButton	m_cSKP;
	CComboBox	m_cCB8;
	CComboBox	m_cCB7;
	CComboBox	m_cCB6;
	CComboBox	m_cCB5;
	CComboBox	m_cCB4;
	CComboBox	m_cCB3;
	CComboBox	m_cCB2;
	CComboBox	m_cCB1;
	CButton	m_cgk76;
	CButton	m_cdl60;
	CButton	m_cbk3;
	CButton	m_calm;
	BOOL	m_sALM;
	BOOL	m_sBK3;
	BOOL	m_sDL60;
	BOOL	m_sGK76;
	BOOL	m_sBK3Test;
	BOOL	m_sDL60Test;
	BOOL	m_sALMTest;
	BOOL	m_sGK76Test;
	BOOL	m_sSKP;
	BOOL	m_sSKPTest;
	BOOL	m_sGK60;
	BOOL	m_sGK60Test;
	BOOL	m_sMSC;
	BOOL	m_sMSCTest;
	BOOL	m_sAK41;
	BOOL	m_sAK42;
	BOOL	m_sAK43;
	BOOL	m_sAK44;
	BOOL	m_sAK4_T;
	BOOL	m_sMsc_60;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPosition)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPosition)
	virtual void OnOK();
	afx_msg void OnAlm();
	afx_msg void OnDl60();
	afx_msg void OnBk360();
	afx_msg void OnGk76();
	afx_msg void OnAlmTest();
	afx_msg void OnDl60Test();
	afx_msg void OnBk361();
	afx_msg void OnGk77();
	afx_msg void OnDropdownCombo1();
	afx_msg void OnDropdownCombo2();
	afx_msg void OnDropdownCombo3();
	afx_msg void OnDropdownCombo4();
	afx_msg void OnDropdownCombo5();
	afx_msg void OnCloseupCombo5();
	afx_msg void OnCloseupCombo3();
	afx_msg void OnCloseupCombo4();
	afx_msg void OnCloseupCombo2();
	afx_msg void OnCloseupCombo1();
	afx_msg void OnDropdownCombo6();
	afx_msg void OnCloseupCombo6();
	afx_msg void OnDropdownCombo7();
	afx_msg void OnCloseupCombo7();
	afx_msg void OnDropdownCombo8();
	afx_msg void OnCloseupCombo8();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSkp();
	afx_msg void OnSkp1();
	afx_msg void OnDropdownCombo9();
	afx_msg void OnCloseupCombo9();
	afx_msg void OnDropdownCombo10();
	afx_msg void OnCloseupCombo10();
	afx_msg void OnGk60();
	afx_msg void OnGk61();
	afx_msg void OnDropdownCombo11();
	afx_msg void OnCloseupCombo11();
	afx_msg void OnDropdownCombo12();
	afx_msg void OnCloseupCombo12();
	afx_msg void OnMsc();
	afx_msg void OnMsc1();
	afx_msg void OnCloseupCombo13();
	afx_msg void OnDropdownCombo13();
	afx_msg void OnCloseupCombo14();
	afx_msg void OnDropdownCombo14();
	afx_msg void OnAk41();
	afx_msg void OnAk42();
	afx_msg void OnAk43();
	afx_msg void OnAk44();
	afx_msg void OnAk4T();
	afx_msg void OnDropdownCombo16();
	afx_msg void OnCloseupCombo16();
	afx_msg void OnMsc60();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POSITION_H__1548406C_83B0_4BC7_B985_57F50058DEA7__INCLUDED_)
