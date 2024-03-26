#if !defined(AFX_TRACKATTR_H__D81A5064_162E_4754_9568_003DA503FB88__INCLUDED_)
#define AFX_TRACKATTR_H__D81A5064_162E_4754_9568_003DA503FB88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrackAttr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrackAttr dialog
#include "GridCtrl.h"
#include "Threads.h"

class CTrackAttr : public CPropertyPage
{
	class CGridEx : public CGridCtrl
	{
		// Override this function to fill InPlaceListBoxes
		void FillListItems(int nCol, LPARAM cltList);
	};
	
	DECLARE_DYNCREATE(CTrackAttr)

// Construction
public:
	CTrackAttr();
	~CTrackAttr();
	CGridEx		m_Grid;
	CTrackListArray*	trackList;
	CCurveListArray*	curveList;

	bool		bFlag;
// Dialog Data
	//{{AFX_DATA(CTrackAttr)
	enum { IDD = IDD_TRACKATTR };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTrackAttr)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTrackAttr)
	virtual BOOL OnInitDialog();
	afx_msg void OnNewtrack();
	afx_msg void OnDeletetrack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACKATTR_H__D81A5064_162E_4754_9568_003DA503FB88__INCLUDED_)
