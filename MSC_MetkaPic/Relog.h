#if !defined(AFX_RELOG_H__12EF0778_7D16_4CED_BC8F_BAB3E1AA7AEC__INCLUDED_)
#define AFX_RELOG_H__12EF0778_7D16_4CED_BC8F_BAB3E1AA7AEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Relog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRelog dialog
#include "MFTMonitorDoc.h"
class CRelog : public CDialog
{
// Construction
public:
	int m_isPrinting;
	CDC m_pDC;
	int m_nSize;
	int m_nLen;
	int m_nIndex;
	int m_nPage;
	CBitmap  cBitmap;
	long m_lRecordsNumber;
	long m_lFieldsNumber;
	CRelog(CWnd* pParent = NULL);   // standard constructor
	CFont	hFont;
	_RecordsetPtr m_rPs;
	int ReadIniFile();
	
	CTrackListArray	trackListView2;
	CCurveListArray	curveListView2;


	CTrackListArray*	pTrackList;
	CCurveListArray*	pCurveList;
	CCurveList*		curve;
	CTrackList*		track;
	CPen			hPen;
	float			yPos;
	float			yOldPos ;
	float			xPos[MAX_CURVE],xOldPos[MAX_CURVE];
	int nprev,ncur;
	float RightLowerViewScale;
	CRect m_cRect;
// Dialog Data
	//{{AFX_DATA(CRelog)
	enum { IDD = IDD_RELOG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRelog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRelog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnFilePrint();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RELOG_H__12EF0778_7D16_4CED_BC8F_BAB3E1AA7AEC__INCLUDED_)
