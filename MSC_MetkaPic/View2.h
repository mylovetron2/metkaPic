#if !defined(AFX_VIEW2_H__FB4CE29D_C959_4949_88C7_F40BFB679C13__INCLUDED_)
#define AFX_VIEW2_H__FB4CE29D_C959_4949_88C7_F40BFB679C13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// View2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CView2 view
#include "MFTMonitorDoc.h"

class CView2 : public CView
{
protected:
	CView2();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CView2)

// Attributes
public:

// Operations
public:
	CMFTMonitorDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CView2)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CView2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CView2)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW2_H__FB4CE29D_C959_4949_88C7_F40BFB679C13__INCLUDED_)
