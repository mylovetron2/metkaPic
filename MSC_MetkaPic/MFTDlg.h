// mftDlg.h : header file
//

#if !defined(AFX_MFTDLG_H__F9B86A48_EE17_4558_8666_6A60B7C0CCB8__INCLUDED_)
#define AFX_MFTDLG_H__F9B86A48_EE17_4558_8666_6A60B7C0CCB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Comm.h"
/////////////////////////////////////////////////////////////////////////////
// CMftDlg dialog

class CMftDlg : public CDialog
{
// Construction
public:
	BYTE* m_pDepth;
	double * m_dData;
	int ** m_pData;
	void Display();
	BYTE* m_pMsg;
	int m_iStop;
	void BackgroundThreadComm();
	CMftDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMftDlg)
	enum { IDD = IDD_MFT_DIALOG };
	CString	m_MA1;
	CString	m_MA10;
	CString	m_MA11;
	CString	m_MA12;
	CString	m_MA13;
	CString	m_MA14;
	CString	m_MA15;
	CString	m_MA2;
	CString	m_MA3;
	CString	m_MA4;
	CString	m_MA5;
	CString	m_MA6;
	CString	m_MA7;
	CString	m_MA8;
	CString	m_MA9;
	CString	m_MD1;
	CString	m_MD2;
	CString	m_MD3;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMftDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMftDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFTDLG_H__F9B86A48_EE17_4558_8666_6A60B7C0CCB8__INCLUDED_)
