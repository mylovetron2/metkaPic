#if !defined(AFX_CURVEATTR_H__E7770A6E_95C2_4172_9B12_5C6907CDF155__INCLUDED_)
#define AFX_CURVEATTR_H__E7770A6E_95C2_4172_9B12_5C6907CDF155__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurveAttr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCurveAttr dialog
#include "GridCtrl.h"
#include "Threads.h"

class CCurveAttr : public CPropertyPage
{
	DECLARE_DYNCREATE(CCurveAttr)
	class CGridEx : public CGridCtrl
	{
		// Override this function to fill InPlaceListBoxes
		void FillListItems(int nCol, LPARAM cltList);
	};
// Construction
public:
	CCurveAttr();
	~CCurveAttr();
	CGridEx		m_Grid;
	CCurveListArray*	curveList;
	CTrackListArray*	trackList;
// Dialog Data
	//{{AFX_DATA(CCurveAttr)
	enum { IDD = IDD_CURVEATTR };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCurveAttr)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCurveAttr)
	virtual BOOL OnInitDialog();
	afx_msg void OnNewcurve();
	afx_msg void OnDeletecurve();
	afx_msg void OnUpdatemsc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURVEATTR_H__E7770A6E_95C2_4172_9B12_5C6907CDF155__INCLUDED_)
