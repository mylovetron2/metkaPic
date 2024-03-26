#if !defined(AFX_WATCH_H__1341FC51_7667_4046_B9F0_D1A9D84A0889__INCLUDED_)
#define AFX_WATCH_H__1341FC51_7667_4046_B9F0_D1A9D84A0889__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Watch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWatch dialog

class CWatch : public CDialog
{
public:
	CWnd*	m_pParent;
	int		m_nID;
	int		nNumRawCurves;
	int		nNumSynCurves;
// Construction
public:
	void DisplayData();
	virtual BOOL Create();
	void OnCancel();
	CWatch(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWatch)
	enum { IDD = IDD_WATCH };
	CEdit	m_cETime;
	CEdit	m_cEDepth;
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWatch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWatch)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WATCH_H__1341FC51_7667_4046_B9F0_D1A9D84A0889__INCLUDED_)
