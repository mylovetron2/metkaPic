//{{AFX_INCLUDES()
#include "datagrid.h"
#include "adodc.h"
//}}AFX_INCLUDES
#if !defined(AFX_EDITDATA_H__A09A0D01_C732_49B9_91F9_B9B3412B71BA__INCLUDED_)
#define AFX_EDITDATA_H__A09A0D01_C732_49B9_91F9_B9B3412B71BA__INCLUDED_
#define MAX 10

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditData dialog


class CEditData : public CDialog
{
// Construction
public:
	void selection_sort(double A[],int n);
	void swap(double &x,double &y);
	
	void ListAccessTables(CString cs);
	void ListAccessFields(CString cs);
	void ListAccessData(CString cs);
	_RecordsetPtr GetRecorset(CString cs,long *nfields,long *nrecords);
	void DumpError(_com_error &e);
	int InitRecordset(CString cs);
    void ListAccessListFields(CString cs);
    void ListAccessCombobox();
 

   
    void OnSqlDb(CString cs); 
	CEditData(CWnd* pParent = NULL);   // standard constructor
	CString m_sBrowse;
	CString m_sTemp;
	CString m_sLike;
	CString m_sWhere;
	CString m_sFrom;
	CString m_sCaption;
	CString m_sSQL;
	CString m_sQuery;
	CString m_sOrder;
	CString m_sql;
	double array[5000];


	_RecordsetPtr m_rRs;
	int m_id[120];
// Dialog Data
	//{{AFX_DATA(CEditData)
	enum { IDD = IDD_EDITDATA };
	CStatic	m_cMessage;
	CComboBox	m_cStep;
	CComboBox	m_cDirection;
	CListBox	m_cList;
	CComboBox	m_cTable;
	CComboBox	m_cField;
	CComboBox	m_cData;
    CAdodc	m_cAdodc;
	CDataGrid	m_cDataGrid;
	double	m_dA;
	double	m_dB;
	CString	m_sName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditData)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	afx_msg void OnClose();
	afx_msg void OnSelendokComboTable();
	afx_msg void OnSelendokComboField();
	afx_msg void OnSelchangeListField();
	afx_msg void OnExport();
	afx_msg void OnReport();
	afx_msg void OnSelendokComboDirection();
	afx_msg void OnSelendokComboStep();
	afx_msg void OnSelectMSC();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_iRecOpen;
	BYTE* m_mBuf;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITDATA_H__A09A0D01_C732_49B9_91F9_B9B3412B71BA__INCLUDED_)
