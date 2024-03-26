// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__73530A22_A9DF_4704_99EA_2C38133C160A__INCLUDED_)
#define AFX_MAINFRM_H__73530A22_A9DF_4704_99EA_2C38133C160A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Threads.h"
#include "LockSplitter.h"
#include "Watch.h"
#include "MyToolBar.h"


class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CWinThread*		pReadDataThread;
	ReadDataThreadInfo_t*	pReadDataThreadInfo;
	CWinThread*		pWriteDataThread;
	WriteDataThreadInfo_t*	pWriteDataThreadInfo;
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	void OnDirection();
	void CreateMyToolBar();
	CStatusBar		m_wndStatusBar;
	CMyToolBar		m_wndToolBar;
	//CSplitterWnd	m_wndSpMain;
	//CSplitterWnd	m_wndSpSub;
	CLockSplitter	m_wndSpMain;
	CLockSplitter	m_wndSpSub;
	CReBar			m_wndReBar;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnScale();
	afx_msg void OnUpdateOneshot(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnThreadUpdate (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__73530A22_A9DF_4704_99EA_2C38133C160A__INCLUDED_)
