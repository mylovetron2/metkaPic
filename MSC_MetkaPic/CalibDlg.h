//{{AFX_INCLUDES()
#include "msflexgrid.h"
#include "datagrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_CALIBDLG_H__19E6FAB7_2E71_46EB_ADCC_1DF5EB595120__INCLUDED_)
#define AFX_CALIBDLG_H__19E6FAB7_2E71_46EB_ADCC_1DF5EB595120__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalibDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalibDlg dialog
#include "MFTMonitorDoc.h"
#include "resource.h"
#include "Watch.h"
class CCalibDlg : public CDialog
{
// Construction
public:
	int m_nLogY;
	int m_nLogX;
	long m_lBorderHeight;
	long m_lBorderWidth;
	CCalibDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalibDlg)
	enum { IDD = IDD_CALIB };
	CComboBox	m_cMSCCalib;
	CButton	m_cCheckPause;
	CButton	m_cSave;
	CEdit	m_cEditGrid;
	CMSFlexGrid	m_cGridCalib;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalibDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalibDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnMouseDownMsflexgrid2(short Button, short Shift, long x, long y);
	afx_msg void OnDblClickMsflexgrid2();
	afx_msg void OnCalibSave();
	virtual void OnCancel();
	afx_msg void OnCheckPause();
	afx_msg void OnUndo();
	afx_msg void OnSetMSC();
	afx_msg void OnSetMsc2();
	afx_msg void OnDropdownComboMSC();
	afx_msg void OnCloseupComboMSC();
	afx_msg void OnSelendokComboMSC();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBDLG_H__19E6FAB7_2E71_46EB_ADCC_1DF5EB595120__INCLUDED_)
