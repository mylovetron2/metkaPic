// MFTMonitor.h : main header file for the MFTMONITOR application
//

#if !defined(AFX_MFTMONITOR_H__1B8AB99F_5837_43F4_8A55_F9105FB44206__INCLUDED_)
#define AFX_MFTMONITOR_H__1B8AB99F_5837_43F4_8A55_F9105FB44206__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMFTMonitorApp:
// See MFTMonitor.cpp for the implementation of this class
//



class CMFTMonitorApp : public CWinApp
{
public:
	CMFTMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFTMonitorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMFTMonitorApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFTMONITOR_H__1B8AB99F_5837_43F4_8A55_F9105FB44206__INCLUDED_)
