// CalibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "CalibDlg.h"
#include "Threads.h"
#include "Panel.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalibDlg dialog


CCalibDlg::CCalibDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalibDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalibDlg)
	//}}AFX_DATA_INIT
}


void CCalibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalibDlg)
	DDX_Control(pDX, IDC_COMBO2, m_cMSCCalib);
	DDX_Control(pDX, IDC_CHECK_PAUSE, m_cCheckPause);
	DDX_Control(pDX, IDC_CALIB_SAVE, m_cSave);
	DDX_Control(pDX, IDC_EDIT1, m_cEditGrid);
	DDX_Control(pDX, IDC_MSFLEXGRID2, m_cGridCalib);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalibDlg, CDialog)
	//{{AFX_MSG_MAP(CCalibDlg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CALIB_SAVE, OnCalibSave)
	ON_BN_CLICKED(IDC_CHECK_PAUSE, OnCheckPause)
	ON_BN_CLICKED(IDC_UNDO, OnUndo)
	ON_BN_CLICKED(IDC_UNDO2, OnSetMSC)
	ON_BN_CLICKED(IDC_UNDO3, OnSetMsc2)
	ON_CBN_DROPDOWN(IDC_COMBO2, OnDropdownComboMSC)
	ON_CBN_CLOSEUP(IDC_COMBO2, OnCloseupComboMSC)
	ON_CBN_SELENDOK(IDC_COMBO2, OnSelendokComboMSC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalibDlg message handlers

BOOL CCalibDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect	rect;
	float	iWidth;
	char string[20];
	int i,j;
	m_cGridCalib.GetWindowRect(&rect);
	
	iWidth=rect.Width()/8;
	iWidth *= 15;
	

	m_cGridCalib.SetCol(0);
	m_cGridCalib.SetRow(0);
	m_cGridCalib.SetColWidth(0,iWidth);
	m_cGridCalib.SetText("Raw. Name");

	m_cGridCalib.SetCol(1);
	m_cGridCalib.SetColWidth(1,iWidth);
	m_cGridCalib.SetText("Syn. Name");

	m_cGridCalib.SetCol(2);
	m_cGridCalib.SetColWidth(2,iWidth);
	m_cGridCalib.SetText("Raw-1");

	m_cGridCalib.SetCol(3);
	m_cGridCalib.SetColWidth(3,iWidth);
	m_cGridCalib.SetText("Real-1");

	m_cGridCalib.SetCol(4);
	m_cGridCalib.SetColWidth(4,iWidth);
	m_cGridCalib.SetText("Raw-2");

	m_cGridCalib.SetCol(5);
	m_cGridCalib.SetColWidth(5,iWidth);
	m_cGridCalib.SetText("Real-2");

	m_cGridCalib.SetCol(6);
	m_cGridCalib.SetColWidth(6,iWidth);
	m_cGridCalib.SetText("Raw-3");

	m_cGridCalib.SetCol(7);
	m_cGridCalib.SetColWidth(7,iWidth);
	m_cGridCalib.SetText("Real-3");

	//m_cGridCalib.SetCol(8);
	//m_cGridCalib.SetColWidth(8,iWidth);
	//m_cGridCalib.SetText("Unit. Name");

	m_cGridCalib.SetRow(0);
    m_cGridCalib.SetCol(0);
    m_lBorderWidth = m_cGridCalib.GetCellLeft();
    m_lBorderHeight = m_cGridCalib.GetCellTop();

    // To convert grid rect from twips to DC units you need
    // pixels per inch.
    CDC* pDC = GetDC();
    m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
    m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);
    ReleaseDC(pDC);


	
	for(i=0;i<NUMCHANNEL;i++)
	{
		sprintf(string,"Ain%d",i+1);
		m_cGridCalib.SetCol(0);
		m_cGridCalib.SetRow(i+1);
		m_cGridCalib.SetText(string);
	}
	
	
	for(i=0;i<NUMCHANNEL;i++){
		m_cGridCalib.SetCol(1);
		m_cGridCalib.SetRow(i+1);
		m_cGridCalib.SetText(pInfoCalib[i].szNameChannel);	
	}
	

	for(i=0;i<NUMCHANNEL;i++){
		for(j=2;j<8;j++){
			
			m_cGridCalib.SetRow(i+1);
			if((j+1)%2){
				m_cGridCalib.SetCol(j);
				sprintf(string,"%.3f",pInfoCalib[i].pCh.RawVal[((j/2)-1)]);
				m_cGridCalib.SetText(string);
			}
			else{
				m_cGridCalib.SetCol(j);
				sprintf(string,"%.3f",pInfoCalib[i].pCh.RealVal[((j/2)-1)]);
				m_cGridCalib.SetText(string);
			}
			
		}
		for(j=0;j<3;j++){
			
			
			lInfoCalib[i].pCh.RawVal[j] = pInfoCalib[i].pCh.RawVal[j];
			lInfoCalib[i].pCh.RealVal[j] = pInfoCalib[i].pCh.RealVal[j];
			
		}
		strcpy(lInfoCalib[i].szNameChannel,pInfoCalib[i].szNameChannel);  
	}

	//for(i=0;i<NUMCHANNEL;i++){
	//	m_cGridCalib.SetCol(8);
	//	m_cGridCalib.SetRow(i+1);
	//	m_cGridCalib.SetText(pInfoCalib[i].szNameChannel);	
	//}
	if(mbMSC) m_cMSCCalib.SetWindowText(m_sMSCCalib);
	//CComboBox::SetWindowText(
	SetTimer(NULL,300,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int nDisplayFlag = 0;
CString m_sMSCCalib = "MSC601";
void CCalibDlg::OnOK() 
{
	// TODO: Add extra validation here
	char str[20];
	int iRow,iCol = m_cGridCalib.GetColSel();
	iRow = m_cGridCalib.GetRow();
	switch(iCol){
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			GetDlgItemText(IDC_EDIT1,str,20);

			//sprintf(str,"%.3f",RealData.fData[iRow-1]);
			m_cGridCalib.SetText(str);
			
			break;

		default : 
			break;
	}

	//CDialog::OnOK();
}

void CCalibDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nDisplayFlag){
		char str[20];
		int iRow,iCol = m_cGridCalib.GetColSel();
		iRow = m_cGridCalib.GetRow();
		switch(iCol){
			case 2:
			case 4:
			case 6:
				sprintf(str,"%.3f",RealData.fData[iRow-1]);
				m_cGridCalib.SetText(str);
				break;

			default : 
				break;
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CCalibDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(NULL);
	nDisplayFlag = 0;
	CDialog::OnClose();
}



BEGIN_EVENTSINK_MAP(CCalibDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCalibDlg)
	ON_EVENT(CCalibDlg, IDC_MSFLEXGRID2, -605 /* MouseDown */, OnMouseDownMsflexgrid2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CCalibDlg, IDC_MSFLEXGRID2, -601 /* DblClick */, OnDblClickMsflexgrid2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCalibDlg::OnMouseDownMsflexgrid2(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	char str[20];
	int iRow,iCol = m_cGridCalib.GetColSel();
	iRow = m_cGridCalib.GetRow();
	switch(iCol){
		case 2:
		case 4:
		case 6:
			sprintf(str,"%.3f",RealData.fData[iRow-1]);
			m_cGridCalib.SetText(str);
			nDisplayFlag = 1;
			break;

		default : 
			nDisplayFlag = 0;
			break;
	}
}



void CCalibDlg::OnDblClickMsflexgrid2() 
{
	// TODO: Add your control notification handler code here
	char str[20];
	m_cEditGrid.ShowWindow(SW_SHOW);
	int iRow,iCol = m_cGridCalib.GetCol();
	iRow = m_cGridCalib.GetRow();
	m_cEditGrid.SetWindowText("");
	float ft =1440;

	float l = ((m_cGridCalib.GetCellLeft() - m_lBorderWidth) * m_nLogX)/ft;
	float t = ((m_cGridCalib.GetCellTop() - m_lBorderHeight) * m_nLogY)/ft;
	float w = (m_cGridCalib.GetCellWidth()* m_nLogX)/ft;
	float h = (m_cGridCalib.GetCellHeight()* m_nLogY)/ft;

	m_cEditGrid.MoveWindow(l+14,t+14,w,h+2, true);
	m_cEditGrid.SetFocus();
	
}

void CCalibDlg::OnCalibSave() 
{
	// TODO: Add your control notification handler code here
	char string[80];
	int i,j;
	for(i=0;i<NUMCHANNEL;i++){
		m_cGridCalib.SetCol(1);
		m_cGridCalib.SetRow(i+1);
		strcpy(pInfoCalib[i].szNameChannel,(LPCTSTR)m_cGridCalib.GetText());
	}
	for(i=0;i<NUMCHANNEL;i++){
		for(j=2;j<8;j++){
			m_cGridCalib.SetRow(i+1);
			if((j+1)%2){
				m_cGridCalib.SetCol(j);
				strcpy(string,(LPCTSTR)m_cGridCalib.GetText());
				pInfoCalib[i].pCh.RawVal[((j/2)-1)] = atof(string);
				
			}
			else{
				m_cGridCalib.SetCol(j);
				strcpy(string,(LPCTSTR)m_cGridCalib.GetText());
				pInfoCalib[i].pCh.RealVal[((j/2)-1)] = atof(string);
	
			}
		}
	}
	CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMFTMonitorDoc*	pDoc=(CMFTMonitorDoc*)pMainFrm->GetActiveDocument();
	if(mbMSC) pDoc->WriteMFTIniFile1(); 
	else pDoc->WriteMFTIniFile();
}


void CCalibDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CCalibDlg::OnCheckPause() 
{
	// TODO: Add your control notification handler code here
	static int pause=1;
	if(pause){
		KillTimer(NULL);
	}
	else SetTimer(NULL,300,NULL);
	pause = (pause+1)%2;
}

void CCalibDlg::OnUndo() 
{
	// TODO: Add your control notification handler code here
	int i,j;
	char string[20];
	for(i=0;i<NUMCHANNEL;i++){
		for(j=0;j<3;j++){
			pInfoCalib[i].pCh.RawVal[j] = lInfoCalib[i].pCh.RawVal[j];
			pInfoCalib[i].pCh.RealVal[j] = lInfoCalib[i].pCh.RealVal[j];
		}
		strcpy(pInfoCalib[i].szNameChannel,lInfoCalib[i].szNameChannel); 
	}
		for(i=0;i<NUMCHANNEL;i++){
		m_cGridCalib.SetCol(1);
		m_cGridCalib.SetRow(i+1);
		m_cGridCalib.SetText(pInfoCalib[i].szNameChannel);	
	}
	for(i=0;i<NUMCHANNEL;i++){
		for(j=2;j<8;j++){
			
			m_cGridCalib.SetRow(i+1);
			if((j+1)%2){
				m_cGridCalib.SetCol(j);
				sprintf(string,"%.3f",pInfoCalib[i].pCh.RawVal[((j/2)-1)]);
				m_cGridCalib.SetText(string);
			}
			else{
				m_cGridCalib.SetCol(j);
				sprintf(string,"%.3f",pInfoCalib[i].pCh.RealVal[((j/2)-1)]);
				m_cGridCalib.SetText(string);
			}
			
		}
	}
}

void CCalibDlg::OnSetMSC() 
{
	// TODO: Add your control notification handler code here
	int i,j;
	char string[20];
	float ft,ft1,ft2;

	m_cGridCalib.SetCol(7);
	m_cGridCalib.SetRow(49);
	strcpy(string,(LPCTSTR)m_cGridCalib.GetText());
	ft = atof(string);
	m_cGridCalib.SetCol(5);
	m_cGridCalib.SetRow(49);
	strcpy(string,(LPCTSTR)m_cGridCalib.GetText());
	ft1 = atof(string);

	m_cGridCalib.SetCol(3);
	m_cGridCalib.SetRow(49);
	strcpy(string,(LPCTSTR)m_cGridCalib.GetText());
	ft2 = atof(string);
	

	for(i=48;i<108;i++){
			
		pInfoCalib[i].pCh.RealVal[0] = ft2;	
		pInfoCalib[i].pCh.RealVal[1] = ft1;
		pInfoCalib[i].pCh.RealVal[2] = ft;
		
		
	}

/*	for(i=0;i<NUMCHANNEL;i++){
		m_cGridCalib.SetCol(1);
		m_cGridCalib.SetRow(i+1);
		m_cGridCalib.SetText(pInfoCalib[i].szNameChannel);	
	}*/
	

/*	for(i=96;i<116;i++)
	{
		sprintf(string,"ARM%d",i-96+41);
		m_cGridCalib.SetCol(1);
		m_cGridCalib.SetRow(i+1);
		m_cGridCalib.SetText(string);
	}



	for(i=96;i<116;i++){
			
		pInfoCalib[i].pCh.RealVal[0] = ft2;	
		pInfoCalib[i].pCh.RealVal[1] = ft1;
		pInfoCalib[i].pCh.RealVal[2] = ft;
		
		
	}

	m_cGridCalib.SetCol(6);
	m_cGridCalib.SetRow(49);
	strcpy(string,(LPCTSTR)m_cGridCalib.GetText());
	ft = atof(string);
	m_cGridCalib.SetCol(4);
	m_cGridCalib.SetRow(49);
	strcpy(string,(LPCTSTR)m_cGridCalib.GetText());
	ft1 = atof(string);

	m_cGridCalib.SetCol(2);
	m_cGridCalib.SetRow(49);
	strcpy(string,(LPCTSTR)m_cGridCalib.GetText());
	ft2 = atof(string);

	for(i=96;i<116;i++){
			
		pInfoCalib[i].pCh.RawVal[0] = ft2;	
		pInfoCalib[i].pCh.RawVal[1] = ft1;
		pInfoCalib[i].pCh.RawVal[2] = ft;
		
		
	}*/

	for(i=48;i<116;i++){
		for(j=2;j<8;j++){
			
			m_cGridCalib.SetRow(i+1);
			if((j+1)%2){
				m_cGridCalib.SetCol(j);
				sprintf(string,"%.3f",pInfoCalib[i].pCh.RawVal[((j/2)-1)]);
				m_cGridCalib.SetText(string);
			}
			else{
				m_cGridCalib.SetCol(j);
				sprintf(string,"%.3f",pInfoCalib[i].pCh.RealVal[((j/2)-1)]);
				m_cGridCalib.SetText(string);
			}
			
		}
	}
}

void CCalibDlg::OnSetMsc2() 
{
	// TODO: Add your control notification handler code here
	int i,j;
	char string[20];
	float ft;

	int iRow,iCol = m_cGridCalib.GetColSel();
		iRow = m_cGridCalib.GetRow();

	if((iCol==2)||(iCol==4)||(iCol==6)){
		for(i=49;i<109;i++){
			sprintf(string,"%.3f",RealData.fData[i-1]);
			m_cGridCalib.SetCol(iCol);
			m_cGridCalib.SetRow(i);
			m_cGridCalib.SetText(string);
		}
		
	
	}

		
}

void CCalibDlg::OnDropdownComboMSC() 
{
	// TODO: Add your control notification handler code here
	int size=10;
	char str[50];
	int i;

	
	if(mbMSC) {
		
			sprintf(str,"MSC601");

			m_cMSCCalib.AddString(str);
			
			sprintf(str,"MSC401");

			m_cMSCCalib.AddString(str);
			
			
			sprintf(str,"MSC402");

			m_cMSCCalib.AddString(str);
		
	}
	else m_cMSCCalib.ResetContent();
	
}

void CCalibDlg::OnCloseupComboMSC() 
{
	// TODO: Add your control notification handler code here
	//int size=10;
	//int i;
	//m_cMSCCalib.ResetContent();
	
}

/* BOOL CCalibDlg::OnReInitDialog() 
{
	
	
	// TODO: Add extra initialization here
	
	
	

	CRect	rect;
	float	iWidth;
	char string[20];
	int i,j;
	m_cGridCalib.GetWindowRect(&rect);
	
	
	
	for(i=0;i<NUMCHANNEL;i++){
		m_cGridCalib.SetCol(1);
		m_cGridCalib.SetRow(i+1);
		m_cGridCalib.SetText(pInfoCalib[i].szNameChannel);	
	}
	

	for(i=0;i<NUMCHANNEL;i++){
		for(j=2;j<8;j++){
			
			m_cGridCalib.SetRow(i+1);
			if((j+1)%2){
				m_cGridCalib.SetCol(j);
				sprintf(string,"%.3f",pInfoCalib[i].pCh.RawVal[((j/2)-1)]);
				m_cGridCalib.SetText(string);
			}
			else{
				m_cGridCalib.SetCol(j);
				sprintf(string,"%.3f",pInfoCalib[i].pCh.RealVal[((j/2)-1)]);
				m_cGridCalib.SetText(string);
			}
			
		}
		for(j=0;j<3;j++){
			
			
			lInfoCalib[i].pCh.RawVal[j] = pInfoCalib[i].pCh.RawVal[j];
			lInfoCalib[i].pCh.RealVal[j] = pInfoCalib[i].pCh.RealVal[j];
			
		}
		strcpy(lInfoCalib[i].szNameChannel,pInfoCalib[i].szNameChannel);  
	}

	
	
	return TRUE;  
}
*/


void CCalibDlg::OnSelendokComboMSC() 
{
	// TODO: Add your control notification handler code here
	CString cs;
	char string[80];
    int index,i,j;

    index = m_cMSCCalib.GetCurSel();
	m_cMSCCalib.GetLBText(index,cs);
	m_sMSCCalib = cs;
	CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMFTMonitorDoc*	pDoc=(CMFTMonitorDoc*)pMainFrm->GetActiveDocument();
	pDoc->ReadMFTIniFile1(); 

	OnInitDialog();

	/*for(i=0;i<NUMCHANNEL;i++){
		for(j=0;j<3;j++){
			pInfoCalib[i].pCh.RawVal[j] = lInfoCalib[i].pCh.RawVal[j];
			pInfoCalib[i].pCh.RealVal[j] = lInfoCalib[i].pCh.RealVal[j];
		}
		strcpy(pInfoCalib[i].szNameChannel,lInfoCalib[i].szNameChannel); 
	}
		for(i=0;i<NUMCHANNEL;i++){
		m_cGridCalib.SetCol(1);
		m_cGridCalib.SetRow(i+1);
		m_cGridCalib.SetText(pInfoCalib[i].szNameChannel);	
	}
	for(i=0;i<NUMCHANNEL;i++){
		for(j=2;j<8;j++){
			
			m_cGridCalib.SetRow(i+1);
			if((j+1)%2){
				m_cGridCalib.SetCol(j);
				sprintf(string,"%.3f",pInfoCalib[i].pCh.RawVal[((j/2)-1)]);
				m_cGridCalib.SetText(string);
			}
			else{
				m_cGridCalib.SetCol(j);
				sprintf(string,"%.3f",pInfoCalib[i].pCh.RealVal[((j/2)-1)]);
				m_cGridCalib.SetText(string);
			}
			
		}
	}*/

	//if(cs == "MSC601") {
			
		
	//	}
//	else if (cs == "MSC401"){
			
	       

//		}
//	else if(cs == "MSC402"){
			
//		{
	
	
}
