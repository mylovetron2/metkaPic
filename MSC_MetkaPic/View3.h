#if !defined(AFX_VIEW3_H__1B394CE5_29D3_4594_9E03_0CB4279262A4__INCLUDED_)
#define AFX_VIEW3_H__1B394CE5_29D3_4594_9E03_0CB4279262A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// View3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CView3 view
#include "MFTMonitorDoc.h"

class CView3 : public CView
{
protected:
	CView3();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CView3)

// Attributes
public:

// Operations
public:
	CMFTMonitorDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CView3)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CView3();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CView3)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW3_H__1B394CE5_29D3_4594_9E03_0CB4279262A4__INCLUDED_)
