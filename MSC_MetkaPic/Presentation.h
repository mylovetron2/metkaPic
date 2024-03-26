#if !defined(AFX_PRESENTATION_H__0222DF9D_9557_4FEB_A1A5_D2D7B6AA5D4F__INCLUDED_)
#define AFX_PRESENTATION_H__0222DF9D_9557_4FEB_A1A5_D2D7B6AA5D4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Presentation.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPresentation
#include "TrackAttr.h"
#include "CurveAttr.h"
#include "Threads.h"


class CPresentation : public CPropertySheet
{
	DECLARE_DYNAMIC(CPresentation)

// Construction
public:
	CPresentation(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPresentation(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTrackAttr		trackAttr;
	CCurveAttr		curveAttr;
	CTrackListArray	trackList;
	CCurveListArray	curveList;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPresentation)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPresentation();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPresentation)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRESENTATION_H__0222DF9D_9557_4FEB_A1A5_D2D7B6AA5D4F__INCLUDED_)
