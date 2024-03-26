// GaugeCtrlSettings.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "GaugeCtrlSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGaugeCtrlSettings dialog


CGaugeCtrlSettings::CGaugeCtrlSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CGaugeCtrlSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGaugeCtrlSettings)
	m_fMaxValue1 = 0.0f;
	m_fMaxValue2 = 0.0f;
	m_fMinValue1 = 0.0f;
	m_fMinValue2 = 0.0f;
	m_bLogarit = FALSE;
	//}}AFX_DATA_INIT
}


void CGaugeCtrlSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGaugeCtrlSettings)
	DDX_Text(pDX, IDC_MAXVALUE1, m_fMaxValue1);
	DDX_Text(pDX, IDC_MAXVALUE2, m_fMaxValue2);
	DDX_Text(pDX, IDC_MINVALUE1, m_fMinValue1);
	DDX_Text(pDX, IDC_MINVALUE2, m_fMinValue2);
	DDX_Check(pDX, IDC_LOGARIT, m_bLogarit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGaugeCtrlSettings, CDialog)
	//{{AFX_MSG_MAP(CGaugeCtrlSettings)
	ON_BN_CLICKED(IDC_LOGARIT, OnLogarit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGaugeCtrlSettings message handlers

void CGaugeCtrlSettings::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	CDialog::OnOK();
}

void CGaugeCtrlSettings::OnLogarit() 
{
	// TODO: Add your control notification handler code here
	
}
