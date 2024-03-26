// Watch.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "Watch.h"
#include "Panel.h"
#include "Threads.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWatch dialog

char*	szRawCurveName[]={"Curve1","Curve2","Curve3","Curve4","Curve5","Curve6","Curve7","Curve8"};
CWatch::CWatch(CWnd* pParent /*=NULL*/)
	: CDialog(CWatch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWatch)
	//}}AFX_DATA_INIT
	m_pParent=pParent;
	m_nID=CWatch::IDD;	
	nNumRawCurves=6;
	nNumSynCurves=4;
}


void CWatch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWatch)
	DDX_Control(pDX, IDC_EDIT2, m_cETime);
	DDX_Control(pDX, IDC_EDIT1, m_cEDepth);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWatch, CDialog)
	//{{AFX_MSG_MAP(CWatch)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWatch message handlers

void CWatch::OnCancel()
{
	//CPanel* pParent=(CPanel*)m_pParent;
	//pParent->bDisplWatch=TRUE;
	//DestroyWindow();
}

BOOL CWatch::Create()
{
	return CDialog::Create(m_nID,m_pParent);
}

void CWatch::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	//CDialog::PostNcDestroy();
}

void CWatch::DisplayData()
{
	//UpdateData(false);
	CString		str;
	//str.Format("%.2f",this->m_fDepth);
	
	
	//SetDlgItemText(IDC_DEPTH,str);
	//SetDlgItemText(IDC_TIME,this->m_strTime);

	
	char		string[20];
	LV_ITEM		lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.state=0;
	lvItem.stateMask=0;

	


	
	//for(int i=0;i<24;i++)
	for(int i=0;i<NUMCHANNEL;i++)
	{
		sprintf(string,"%.3f",RealData.fData[i]);
		m_List.SetItemText(i,1,string);
		sprintf(string,"%.3f",gData.fData[i]);
		m_List.SetItemText(i,3,string);
		m_List.SetItemText(i,2,pInfoCalib[i].szNameChannel);
	}
	sprintf(string,"%.2f",gData.fDepth);
	SetDlgItemText(IDC_EDIT1,string);
	SetDlgItemText(IDC_EDIT2,info.szCurTime);
	
	
	
}
BOOL CWatch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect	rect;
	float	fWidth;
	m_List.GetWindowRect(&rect);
	fWidth=rect.Width()/4;

	LV_COLUMN		lvColumn;
	lvColumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt=LVCFMT_LEFT;
	lvColumn.cx=int(fWidth);

	lvColumn.iSubItem=0;
	lvColumn.pszText="Raw Name";
	m_List.InsertColumn(0,&lvColumn);

	lvColumn.iSubItem=1;
	lvColumn.pszText="Raw Value";
	m_List.InsertColumn(1,&lvColumn);

	lvColumn.iSubItem=2;
	lvColumn.pszText="Syn. Name";
	m_List.InsertColumn(2,&lvColumn);

	lvColumn.iSubItem=3;
	lvColumn.pszText="Syn. Value";
	m_List.InsertColumn(3,&lvColumn);

	
	char		string[20];
	LV_ITEM		lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.state=0;
	lvItem.stateMask=0;

	for(int i=0;i<NUMCHANNEL;i++)
	{
		sprintf(string,"Ain%d",i+1);
		lvItem.iItem=i;
		lvItem.iSubItem=0;
		lvItem.pszText=string;
		m_List.InsertItem(&lvItem);
		m_List.SetItemText(i,2,pInfoCalib[i].szNameChannel);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/*void CWatch::DisplayData()
{
	//UpdateData(false);
	CString		str;
	//str.Format("%.2f",this->m_fDepth);
	
	
	//SetDlgItemText(IDC_DEPTH,str);
	//SetDlgItemText(IDC_TIME,this->m_strTime);

	char	*szName[]={"MA1","Hydraulic Pres.","Hydraulic Temp.",
						"Sample Pres.","Sample Temp.","Motor Volt.",
						"Pretest POT","Signal REF","Gamma Ray",
						"Gauge REF-","Gauge REF+","MA12",
						"MA13","MA14","MA15","Position"};

	//m_List.DeleteAllItems();

	char		string[20];
	LV_ITEM		lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.state=0;
	lvItem.stateMask=0;


	
	for(int i=0;i<26;i++)
	{
		if(i<15){
		
			sprintf(string,"%d",pBufData[Index].MA[i]);
			m_List.SetItemText(i,1,string);
			sprintf(string,"%.3f",gData.fData[i]);
			m_List.SetItemText(i,3,string);
		
		}
		else if(i== 15) {
			
			sprintf(string,"%.0f",gData.fData[15]);
			m_List.SetItemText(i,1,string);
			m_List.SetItemText(i,3,string);
		}
		else if(i<18)
		{	
			
			sprintf(string,"%.1f",gData.fData[i]);
			m_List.SetItemText(i,1,string);
			
			sprintf(string,"%.2f",gData.fData[i+2]);
			m_List.SetItemText(i,3,string);
		}
		else 
		{
			sprintf(string,"%d",pBufData[Index].Ain[i-18]);
			m_List.SetItemText(i,1,string);
			sprintf(string,"%.3f",gData.fData[i+2]);
			m_List.SetItemText(i,3,string);
		}
	}
}
BOOL CWatch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect	rect;
	float	fWidth;
	m_List.GetWindowRect(&rect);
	fWidth=rect.Width()/5;

	LV_COLUMN		lvColumn;
	lvColumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt=LVCFMT_RIGHT;
	lvColumn.cx=int(fWidth);

	lvColumn.iSubItem=0;
	lvColumn.pszText="Name";
	m_List.InsertColumn(0,&lvColumn);

	lvColumn.iSubItem=1;
	lvColumn.pszText="Raw Value";
	m_List.InsertColumn(1,&lvColumn);

	lvColumn.iSubItem=2;
	lvColumn.pszText="Name";
	m_List.InsertColumn(2,&lvColumn);

	lvColumn.iSubItem=3;
	lvColumn.pszText="Syn. Value";
	m_List.InsertColumn(3,&lvColumn);

	char	*szName[]={"MA1","Hydraulic Pres.","Hydraulic Temp.",
						"Sample Pres.","Sample Temp.","Motor Volt.",
						"Pretest POT","Signal REF","Gamma Ray",
						"Gauge REF-","Gauge REF+","MA12",
						"MA13","MA14","MA15",
						"Position","Quartz Pres.","Quartz Temp.",
						};

	//m_List.DeleteAllItems();

	char		string[20];
	LV_ITEM		lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.state=0;
	lvItem.stateMask=0;

	for(int i=0;i<26;i++)
	{
		if(i<15)
			sprintf(string,"MA %d",i+1);
		else if(i<18)
			sprintf(string,"MD %d",i-15+1);
		else sprintf(string,"Ain %d",i-18+1);
		lvItem.iItem=i;
		lvItem.iSubItem=0;
		lvItem.pszText=string;
		m_List.InsertItem(&lvItem);
		if(i<18)
		m_List.SetItemText(i,2,szName[i]);
		else m_List.SetItemText(i,2,string);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}*/

void CWatch::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CPanel* pParent=(CPanel*)m_pParent;
	pParent->bDisplWatch=TRUE;
	
	ShowWindow(SW_HIDE);
	//CDialog::OnClose();
	
}
