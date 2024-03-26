// MFTMonitorView.h : interface of the CMFTMonitorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFTMONITORVIEW_H__730A811F_01FF_445B_8739_AD869DF1D52D__INCLUDED_)
#define AFX_MFTMONITORVIEW_H__730A811F_01FF_445B_8739_AD869DF1D52D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MFTMonitorDoc.h"

class CMFTMonitorView : public CView
{
protected: // create from serialization only
	CMFTMonitorView();
	DECLARE_DYNCREATE(CMFTMonitorView)

// Attributes
public:
	CMFTMonitorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFTMonitorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMFTMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMFTMonitorView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MFTMonitorView.cpp
inline CMFTMonitorDoc* CMFTMonitorView::GetDocument()
   { return (CMFTMonitorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFTMONITORVIEW_H__730A811F_01FF_445B_8739_AD869DF1D52D__INCLUDED_)
