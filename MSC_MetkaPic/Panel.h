//{{AFX_INCLUDES()
#include "RelogDlg.h"	// Added by ClassView
#include "mftdepth.h"
#include "mftgauge.h"
#include "mftposition.h"
//}}AFX_INCLUDES
#if !defined(AFX_PANEL_H__38B31FD0_2FAD_4656_95DB_B1BA381861A7__INCLUDED_)
#define AFX_PANEL_H__38B31FD0_2FAD_4656_95DB_B1BA381861A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Panel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPanel form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Threads.h"
#include "resource.h"
#include "Watch.h"
#include "MFTMonitorDoc.h"

#include "CalibDlg.h"	// Added by ClassView
#include "EditData.h"	// Added by ClassView
#include "Position.h"	// Added by ClassView
#include "Relog.h"	// Added by ClassView
class CPanel : public CFormView
{
public:
	CWatch			*pWatch;
	CPosition		*pPosition;
	BOOL			bDisplWatch;
	CRelog* m_cRelog;

	CWinThread*		pReadDataThread;
	ReadDataThreadInfo_t*	pReadDataThreadInfo;

protected:
	CFont	hFont;
	CRect mRectDepth,mRectSpeed,mRectTension,mRectMarked,mRectCurrent;
	CPanel();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPanel)

// Form Data
public:
	//{{AFX_DATA(CPanel)
	enum { IDD = IDD_PANEL };
	CButton	m_cLogRTC;
	CButton	m_cRcordRTC;
	CButton	m_cSynchRTC;
	CButton	m_cViewHeader;
	CButton	m_cOnMemToolPower;
	CStatic	m_cClose;
	CStatic	m_cOpen;
	CStatic	m_cCurrent;
	CStatic	m_cSingleMark;
	CStatic	m_cTension;
	CStatic	m_cMarked;
	CButton	m_cEncoderR;
	CEdit	m_cSetDepthE;
	CStatic	m_cSetDepths;
	CStatic	m_cClrDepth;
	CButton	m_cPlayInDepth;
	CButton	m_cPlayInTime;
	CEdit	m_cEncoder;
	CStatic	m_cSpeed;
	CStatic	m_cDepth;
	CString	m_sDepth;
	CString	m_sSpeed;
	CString	m_sSetDepth;
	CString	m_sEncoder;
	CString	m_sMarked;
	CString	m_sTension;
	CString	m_sCurrent;
	//}}AFX_DATA

// Attributes
public:
	float m_fDepth,m_fSpeed;
	
// Operations
public:
	
	CEditData* m_cEditData;
	CCalibDlg* m_cCalib;
	CPosition* m_cPosition;
	
	void SetDCMem(CDC* dc,CRect rect,CString cs);
	CMFTMonitorDoc* GetDocument();
	void StopFirmware();
	void DisplayDataInPanel(UpdateInfo_t	info);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanel)
	public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPanel();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPanel)
	afx_msg void OnGaugeHydraulic();
	afx_msg void OnGaugeSample();
	afx_msg void OnQuarztPretest();
	afx_msg void OnPosition();
	afx_msg void OnWatch();
	afx_msg void OnClose();
	afx_msg void OnSetdepth();
	afx_msg void OnEncoderratio();
	afx_msg void OnStopcom();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStartTimer();
	afx_msg void OnOneshot();
	afx_msg void OnSclrdepth();
	afx_msg void OnStaticCalib();
	afx_msg void OnStaticEdit();
	afx_msg void OnPlaybackTime();
	afx_msg void OnPlaybackDepth();
	afx_msg void OnMultiMark();
	afx_msg void OnSingleMark();
	afx_msg void OnChangeAtensionst();
	afx_msg void OnChangeBtensionst();
	afx_msg void OnStaticOpen();
	afx_msg void OnStaticClose();
	afx_msg void OniPageofMark();
	afx_msg void OnMemToolPower();
	afx_msg void OnViewHeaderMem();
	afx_msg void OnSynchRTC();
	afx_msg void OnRecordRTC();
	afx_msg void OnLogRTC();
	afx_msg void OnCreateHeader();
	afx_msg void OnFormatSD();
	afx_msg void OnLogSD();
	afx_msg void OnStopLogging();
	afx_msg void OnRTCStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANEL_H__38B31FD0_2FAD_4656_95DB_B1BA381861A7__INCLUDED_)
