#if !defined(AFX_MYTOOLBAR_H__4E21F760_3F08_4220_A54D_2FD019EA152E__INCLUDED_)
#define AFX_MYTOOLBAR_H__4E21F760_3F08_4220_A54D_2FD019EA152E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyToolBar window
class CTBAnim : public CAnimateCtrl {
public:
	CTBAnim(LPCTSTR lpszURL=NULL){ }
	virtual ~CTBAnim() { }
	//CHyperlink m_link;						 // go here when clicked

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CTBAnim)
};

class CMyToolBar : public CToolBar
{
// Construction
public:
	CMyToolBar();
	CMyToolBar(UINT nWidthAnim, UINT nResID) 
	{
		m_nWidthAnim = nWidthAnim;
		m_nIDAnim = nResID;
	}
	//CTBAnim m_wndAnim;
	CStatic		m_Static;

	int m_nWidthAnim;		// width of animation
	int m_nIDAnim;			// resource ID of animation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyToolBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyToolBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_DYNAMIC(CMyToolBar)
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTOOLBAR_H__4E21F760_3F08_4220_A54D_2FD019EA152E__INCLUDED_)
