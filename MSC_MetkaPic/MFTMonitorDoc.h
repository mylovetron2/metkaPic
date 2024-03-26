// MFTMonitorDoc.h : interface of the CMFTMonitorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFTMONITORDOC_H__77A0FE10_0F80_4D52_B401_0E4A5913B17F__INCLUDED_)
#define AFX_MFTMONITORDOC_H__77A0FE10_0F80_4D52_B401_0E4A5913B17F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "Threads.h"
typedef struct	tagFIELDINFO { 
	CString		FieldName;
	DataTypeEnum FieldType;
	//ADODB::DataTypeEnum FieldType;
	long			Size;
} FieldInfo_t;

class CMFTMonitorDoc : public CDocument
{
public:
	//View1: right upper view
	//View2: right lower view
	CWinThread*		pView1Thread;
	CWinThread*		pView2Thread;
	CWinThread*		pWriteDataThread;
//	CWinThread*		pReadDataThread;

	CRect			rectView1;
	CRect			rectView2;
	CRect			rectView3;
	
	CRect			rectPartView1;
	CRect			rectPartView2;
	CRect			rectPartView3;

	CTrackListArray	trackListView1;
	CTrackListArray	trackListView2;
	
	CCurveListArray	curveListView1;
	CCurveListArray	curveListView2;

	ThreadInfo_t*	pThreadInfo1;
	ThreadInfo_t*	pThreadInfo2;
	
	

	ReadDataThreadInfo_t*	pReadDataThreadInfo;
	WriteDataThreadInfo_t*	pWriteDataThreadInfo;//new
protected: // create from serialization only
	CMFTMonitorDoc();
	DECLARE_DYNCREATE(CMFTMonitorDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFTMonitorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	void DumpError(_com_error &e);
	BOOL CreateDatabase(CString ConString, CString DbName);
	BOOL CreateTable(CString ConString, CString TableName,FieldInfo_t *fInfo,int nFields);
	void WriteMFTIniFile();
	void WriteMFTIniFileQ();
	void WriteMFTIniFile1();
	int ReadMFTIniFile1();
	int ReadMFTIniFile();
	void StopLogging();
	//BOOL OnReInitDialog();
	void OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult);
	virtual ~CMFTMonitorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMFTMonitorDoc)
	afx_msg void OnStartlogging();
	afx_msg void OnUpdateStartlogging(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStoplogging(CCmdUI* pCmdUI);
	afx_msg void OnStoplogging();
	afx_msg void OnScaleView130seconds();
	afx_msg void OnUpdateScaleView130seconds(CCmdUI* pCmdUI);
	afx_msg void OnScaleView11minute();
	afx_msg void OnUpdateScaleView11minute(CCmdUI* pCmdUI);
	afx_msg void OnScaleView15minutes();
	afx_msg void OnUpdateScaleView15minutes(CCmdUI* pCmdUI);
	afx_msg void OnScaleView110minutes();
	afx_msg void OnUpdateScaleView110minutes(CCmdUI* pCmdUI);
	afx_msg void OnScaleView120minutes();
	afx_msg void OnUpdateScaleView120minutes(CCmdUI* pCmdUI);
	afx_msg void OnScaleView130minutes();
	afx_msg void OnUpdateScaleView130minutes(CCmdUI* pCmdUI);
	afx_msg void OnScaleView25meters();
	afx_msg void OnUpdateScaleView25meters(CCmdUI* pCmdUI);
	afx_msg void OnScaleView210meters();
	afx_msg void OnUpdateScaleView210meters(CCmdUI* pCmdUI);
	afx_msg void OnScaleView220meters();
	afx_msg void OnUpdateScaleView220meters(CCmdUI* pCmdUI);
	afx_msg void OnScaleView230meters();
	afx_msg void OnUpdateScaleView230meters(CCmdUI* pCmdUI);
	afx_msg void OnScaleView250meters();
	afx_msg void OnUpdateScaleView250meters(CCmdUI* pCmdUI);
	afx_msg void OnScaleView2100meters();
	afx_msg void OnUpdateScaleView2100meters(CCmdUI* pCmdUI);
	afx_msg void OnScaleView330seconds();
	afx_msg void OnUpdateScaleView330seconds(CCmdUI* pCmdUI);
	afx_msg void OnScaleView31minute();
	afx_msg void OnUpdateScaleView31minute(CCmdUI* pCmdUI);
	afx_msg void OnScaleView35minutes();
	afx_msg void OnUpdateScaleView35minutes(CCmdUI* pCmdUI);
	afx_msg void OnScaleView310minutes();
	afx_msg void OnUpdateScaleView310minutes(CCmdUI* pCmdUI);
	afx_msg void OnScaleView320minutes();
	afx_msg void OnUpdateScaleView320minutes(CCmdUI* pCmdUI);
	afx_msg void OnScaleView330minutes();
	afx_msg void OnUpdateScaleView330minutes(CCmdUI* pCmdUI);
	afx_msg void OnSavedata();
	afx_msg void OnStopsaving();
	afx_msg void OnUpdateSavedata(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStopsaving(CCmdUI* pCmdUI);
	afx_msg void OnChangedvpres();
	afx_msg void OnChangetimeviewpres();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFTMONITORDOC_H__77A0FE10_0F80_4D52_B401_0E4A5913B17F__INCLUDED_)
