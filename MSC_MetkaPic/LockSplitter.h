#if !defined(AFX_LOCKSPLITTER_H__79272007_39EC_49EC_A34E_F2CB8679B0F6__INCLUDED_)
#define AFX_LOCKSPLITTER_H__79272007_39EC_49EC_A34E_F2CB8679B0F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LockSplitter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLockSplitter window

class CLockSplitter : public CSplitterWnd
{
// Construction
public:
	CLockSplitter();

// Attributes
public:
	BOOL	m_bLocked;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLockSplitter)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLockSplitter();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLockSplitter)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCKSPLITTER_H__79272007_39EC_49EC_A34E_F2CB8679B0F6__INCLUDED_)
