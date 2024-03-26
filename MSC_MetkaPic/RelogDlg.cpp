// RelogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "RelogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RelogDlg dialog


RelogDlg::RelogDlg(CWnd* pParent /*=NULL*/)
	//: CDialog(RelogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(RelogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void RelogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RelogDlg)
	//DDX_Control(pDX, IDC_STATIC_RELOG, m_cRelogGraph);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RelogDlg, CDialog)
	//{{AFX_MSG_MAP(RelogDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RelogDlg message handlers
