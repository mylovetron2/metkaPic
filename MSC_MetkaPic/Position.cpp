// Position.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "Position.h"
#include  "Threads.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPosition dialog


CPosition::CPosition(CWnd* pParent /*=NULL*/)
	: CDialog(CPosition::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPosition)
	m_sALM = TRUE;
	m_sBK3 = FALSE;
	m_sDL60 = FALSE;
	m_sGK76 = FALSE;
	m_sBK3Test = FALSE;
	m_sDL60Test = FALSE;
	m_sALMTest = FALSE;
	m_sGK76Test = FALSE;
	m_sSKP = FALSE;
	m_sSKPTest = FALSE;
	m_sGK60 = FALSE;
	m_sGK60Test = FALSE;
	m_sMSC = FALSE;
	m_sMSCTest = FALSE;
	m_sAK41 = FALSE;
	m_sAK42 = FALSE;
	m_sAK43 = FALSE;
	m_sAK44 = FALSE;
	m_sAK4_T = FALSE;
	m_sMsc_60 = FALSE;
	//}}AFX_DATA_INIT
}


void CPosition::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPosition)
	DDX_Control(pDX, IDC_MSC_60, m_cMsc_60);
	DDX_Control(pDX, IDC_COMBO16, m_cCB16);
	DDX_Control(pDX, IDC_AK4_T, m_cAK4_T);
	DDX_Control(pDX, IDC_AK44, m_cAK44);
	DDX_Control(pDX, IDC_AK43, m_cAK43);
	DDX_Control(pDX, IDC_AK42, m_cAK42);
	DDX_Control(pDX, IDC_AK41, m_cAK41);
	DDX_Control(pDX, IDC_MSC_1, m_cMSCTest);
	DDX_Control(pDX, IDC_MSC, m_cMSC);
	DDX_Control(pDX, IDC_COMBO14, m_cCB14);
	DDX_Control(pDX, IDC_COMBO13, m_cCB13);
	DDX_Control(pDX, IDC_COMBO12, m_cCB12);
	DDX_Control(pDX, IDC_COMBO11, m_cCB11);
	DDX_Control(pDX, IDC_COMBO10, m_cCB10);
	DDX_Control(pDX, IDC_COMBO9, m_cCB9);
	DDX_Control(pDX, IDC_SKP_1, m_cSKPTest);
	DDX_Control(pDX, IDC_SKP, m_cSKP);
	DDX_Control(pDX, IDC_COMBO8, m_cCB8);
	DDX_Control(pDX, IDC_COMBO7, m_cCB7);
	DDX_Control(pDX, IDC_COMBO6, m_cCB6);
	DDX_Control(pDX, IDC_COMBO5, m_cCB5);
	DDX_Control(pDX, IDC_COMBO4, m_cCB4);
	DDX_Control(pDX, IDC_COMBO3, m_cCB3);
	DDX_Control(pDX, IDC_COMBO2, m_cCB2);
	DDX_Control(pDX, IDC_COMBO1, m_cCB1);
	DDX_Control(pDX, IDC_GK76, m_cgk76);
	DDX_Control(pDX, IDC_DL60, m_cdl60);
	DDX_Control(pDX, IDC_BK360, m_cbk3);
	DDX_Control(pDX, IDC_ALM, m_calm);
	DDX_Check(pDX, IDC_ALM, m_sALM);
	DDX_Check(pDX, IDC_BK360, m_sBK3);
	DDX_Check(pDX, IDC_DL60, m_sDL60);
	DDX_Check(pDX, IDC_GK76, m_sGK76);
	DDX_Check(pDX, IDC_BK361, m_sBK3Test);
	DDX_Check(pDX, IDC_DL60_TEST, m_sDL60Test);
	DDX_Check(pDX, IDC_ALM_TEST, m_sALMTest);
	DDX_Check(pDX, IDC_GK77, m_sGK76Test);
	DDX_Check(pDX, IDC_SKP, m_sSKP);
	DDX_Check(pDX, IDC_SKP_1, m_sSKPTest);
	DDX_Check(pDX, IDC_GK60, m_sGK60);
	DDX_Check(pDX, IDC_GK61, m_sGK60Test);
	DDX_Check(pDX, IDC_MSC, m_sMSC);
	DDX_Check(pDX, IDC_MSC_1, m_sMSCTest);
	DDX_Check(pDX, IDC_AK41, m_sAK41);
	DDX_Check(pDX, IDC_AK42, m_sAK42);
	DDX_Check(pDX, IDC_AK43, m_sAK43);
	DDX_Check(pDX, IDC_AK44, m_sAK44);
	DDX_Check(pDX, IDC_AK4_T, m_sAK4_T);
	DDX_Check(pDX, IDC_MSC_60, m_sMsc_60);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPosition, CDialog)
	//{{AFX_MSG_MAP(CPosition)
	ON_BN_CLICKED(IDC_ALM, OnAlm)
	ON_BN_CLICKED(IDC_DL60, OnDl60)
	ON_BN_CLICKED(IDC_BK360, OnBk360)
	ON_BN_CLICKED(IDC_GK76, OnGk76)
	ON_BN_CLICKED(IDC_ALM_TEST, OnAlmTest)
	ON_BN_CLICKED(IDC_DL60_TEST, OnDl60Test)
	ON_BN_CLICKED(IDC_BK361, OnBk361)
	ON_BN_CLICKED(IDC_GK77, OnGk77)
	ON_CBN_DROPDOWN(IDC_COMBO1, OnDropdownCombo1)
	ON_CBN_DROPDOWN(IDC_COMBO2, OnDropdownCombo2)
	ON_CBN_DROPDOWN(IDC_COMBO3, OnDropdownCombo3)
	ON_CBN_DROPDOWN(IDC_COMBO4, OnDropdownCombo4)
	ON_CBN_DROPDOWN(IDC_COMBO5, OnDropdownCombo5)
	ON_CBN_CLOSEUP(IDC_COMBO5, OnCloseupCombo5)
	ON_CBN_CLOSEUP(IDC_COMBO3, OnCloseupCombo3)
	ON_CBN_CLOSEUP(IDC_COMBO4, OnCloseupCombo4)
	ON_CBN_CLOSEUP(IDC_COMBO2, OnCloseupCombo2)
	ON_CBN_CLOSEUP(IDC_COMBO1, OnCloseupCombo1)
	ON_CBN_DROPDOWN(IDC_COMBO6, OnDropdownCombo6)
	ON_CBN_CLOSEUP(IDC_COMBO6, OnCloseupCombo6)
	ON_CBN_DROPDOWN(IDC_COMBO7, OnDropdownCombo7)
	ON_CBN_CLOSEUP(IDC_COMBO7, OnCloseupCombo7)
	ON_CBN_DROPDOWN(IDC_COMBO8, OnDropdownCombo8)
	ON_CBN_CLOSEUP(IDC_COMBO8, OnCloseupCombo8)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SKP, OnSkp)
	ON_BN_CLICKED(IDC_SKP_1, OnSkp1)
	ON_CBN_DROPDOWN(IDC_COMBO9, OnDropdownCombo9)
	ON_CBN_CLOSEUP(IDC_COMBO9, OnCloseupCombo9)
	ON_CBN_DROPDOWN(IDC_COMBO10, OnDropdownCombo10)
	ON_CBN_CLOSEUP(IDC_COMBO10, OnCloseupCombo10)
	ON_BN_CLICKED(IDC_GK60, OnGk60)
	ON_BN_CLICKED(IDC_GK61, OnGk61)
	ON_CBN_DROPDOWN(IDC_COMBO11, OnDropdownCombo11)
	ON_CBN_CLOSEUP(IDC_COMBO11, OnCloseupCombo11)
	ON_CBN_DROPDOWN(IDC_COMBO12, OnDropdownCombo12)
	ON_CBN_CLOSEUP(IDC_COMBO12, OnCloseupCombo12)
	ON_BN_CLICKED(IDC_MSC, OnMsc)
	ON_BN_CLICKED(IDC_MSC_1, OnMsc1)
	ON_CBN_CLOSEUP(IDC_COMBO13, OnCloseupCombo13)
	ON_CBN_DROPDOWN(IDC_COMBO13, OnDropdownCombo13)
	ON_CBN_CLOSEUP(IDC_COMBO14, OnCloseupCombo14)
	ON_CBN_DROPDOWN(IDC_COMBO14, OnDropdownCombo14)
	ON_BN_CLICKED(IDC_AK41, OnAk41)
	ON_BN_CLICKED(IDC_AK42, OnAk42)
	ON_BN_CLICKED(IDC_AK43, OnAk43)
	ON_BN_CLICKED(IDC_AK44, OnAk44)
	ON_BN_CLICKED(IDC_AK4_T, OnAk4T)
	ON_CBN_DROPDOWN(IDC_COMBO16, OnDropdownCombo16)
	ON_CBN_CLOSEUP(IDC_COMBO16, OnCloseupCombo16)
	ON_BN_CLICKED(IDC_MSC_60, OnMsc60)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPosition message handlers

void CPosition::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(false);
	CDialog::OnOK();
}

void CPosition::OnAlm() 
{
	// TODO: Add your control notification handler code here
	//m_sALM = ~m_sALM ;
	mbALM = (mbALM+1)%2;
	//if(mbALM) m_cCB1.AddString("0.01");
	//m_sALM = (BOOL)mbALM;
	
}

void CPosition::OnDl60() 
{
	// TODO: Add your control notification handler code here
	mbDL60 = (mbDL60+1)%2;
}

void CPosition::OnBk360() 
{
	// TODO: Add your control notification handler code here
	mbBK3 = (mbBK3+1)%2;

	//mbBK79 = (mbBK79+1)%2;
}

void CPosition::OnGk76() 
{
	// TODO: Add your control notification handler code here
	mbGK76 = (mbGK76+1)%2;
	
}

void CPosition::OnAlmTest() 
{
	// TODO: Add your control notification handler code here
	mbALMTest = (mbALMTest+1)%2;
}

void CPosition::OnDl60Test() 
{
	// TODO: Add your control notification handler code here
	mbDL60Test = (mbDL60Test+1)%2;
	
}

void CPosition::OnBk361()	//BK79
{
	// TODO: Add your control notification handler code here
	mbBK3Test = (mbBK3Test+1)%2;
	mbBK79 = (mbBK79+1)%2;
}

void CPosition::OnGk77() 
{
	// TODO: Add your control notification handler code here
	mbGK76Test = (mbGK76Test+1)%2;
}

//void CPosition::OnEditchangeCombo1() 
//{
	// TODO: Add your control notification handler code here
	
//}

void CPosition::OnDropdownCombo1() 
{
	// TODO: Add your control notification handler code here

	int size=2;

	char str[50];
	int i;



	if(mbALM) {
		
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataALM[i]);

			m_cCB1.AddString(str);
		}
	}
	else m_cCB1.ResetContent();

	
}

void CPosition::OnCloseupCombo1() 
{
	// TODO: Add your control notification handler code here
	int size=2;
	//char str[50];
	int i;
	m_cCB1.ResetContent();
	//for(i=size;i>=0;i--) m_cCB1.DeleteString(i);
	
}

//void CPosition::OnEditchangeCombo2() 
//{
	// TODO: Add your control notification handler code here

	
	
//}

void CPosition::OnDropdownCombo2() 
{
	// TODO: Add your control notification handler code here
	int size=16;

	char str[50];
	int i;

	


	if(mbALMTest) {
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataALM1[i]);

			m_cCB2.AddString(str);
		}
	}
	else m_cCB2.ResetContent();
	
	
}

void CPosition::OnCloseupCombo2() 
{
	// TODO: Add your control notification handler code here
			int size=16;
	//char str[50];
	int i;
	m_cCB2.ResetContent();
	//for(i=size;i>=0;i--) m_cCB2.DeleteString(i);
	
}

void CPosition::OnDropdownCombo3() 
{
	// TODO: Add your control notification handler code here
	int size=14;
	char str[50];
	int i;

	
	if(mbDL60) {
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataDL60[i]);

			m_cCB3.AddString(str);
		}
	}
	else m_cCB3.ResetContent();
	
	
}

void CPosition::OnCloseupCombo3() 
{
	// TODO: Add your control notification handler code here
		int size=14;
	//char str[50];
	int i;
	m_cCB3.ResetContent();
	//for(i=size;i>=0;i--) m_cCB3.DeleteString(i);
	
}

void CPosition::OnDropdownCombo4() 
{
	// TODO: Add your control notification handler code here
	int size=16;
	char str[50];
	int i;

	
	if(mbDL60Test) {
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataDL601[i]);

			m_cCB4.AddString(str);
		}
	}
	else m_cCB4.ResetContent();

	
}

void CPosition::OnCloseupCombo4() 
{
	// TODO: Add your control notification handler code here
		int size=16;
	//char str[50];
	int i;
	m_cCB4.ResetContent();
	//for(i=size;i>=0;i--) m_cCB4.DeleteString(i);
	
}



void CPosition::OnDropdownCombo5() 
{
	// TODO: Add your control notification handler code here
	int size=24;
	//int size=260;
	char str[50];
	int i;

	
	if(mbBK3) {
	//if(mbBK79) {
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataBK3[i]);
		//	sprintf(str,"W%d : %d",i,iBK[i+236]);

			m_cCB5.AddString(str);
		}
	}
	else m_cCB5.ResetContent();
	
}

void CPosition::OnCloseupCombo5() 
{
	// TODO: Add your control notification handler code here
	int size=24;
	//int size=260;
	//char str[50];
	int i;
	m_cCB5.ResetContent();
	//for(i=size;i>=0;i--) m_cCB5.DeleteString(i);
	
}


void CPosition::OnDropdownCombo6() 
{
	// TODO: Add your control notification handler code here
	//int size=11;
	int size=32;
	char str[50];
	int i;

	
	//if(mbBK3Test) {
	if(mbBK79) {
		for(i=0;i<size;i++){

			//sprintf(str,"W%d : %d",i,pTempDataBK31[i]);
			sprintf(str,"W%d : %d",i,iBK[i]);

			m_cCB6.AddString(str);
		}
	}
	else m_cCB6.ResetContent();
	
}

void CPosition::OnCloseupCombo6() 
{
	// TODO: Add your control notification handler code here
	//int size=11;
	int size=32;
	int i;
	m_cCB6.ResetContent();
	//for(i=size;i>=0;i--) m_cCB6.DeleteString(i);
	
}

void CPosition::OnDropdownCombo7() 
{
	// TODO: Add your control notification handler code here
	int size=6;
	char str[50];
	int i;

	
	if(mbGK76) {
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataGK76[i]);

			m_cCB7.AddString(str);
		}
	}
	else m_cCB7.ResetContent();
	
}

void CPosition::OnCloseupCombo7() 
{
	// TODO: Add your control notification handler code here
	int size=6;
	int i;
	m_cCB7.ResetContent();
	//for(i=size;i>=0;i--) m_cCB7.DeleteString(i);
	
}

void CPosition::OnDropdownCombo8() 
{
	// TODO: Add your control notification handler code here
	int size=11;
	char str[50];
	int i;
	

	if(mbGK76Test) {
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataGK761[i]);

			m_cCB8.AddString(str);
		}
	}
	else m_cCB8.ResetContent();
	
}

void CPosition::OnCloseupCombo8() 
{
	// TODO: Add your control notification handler code here
	int size=11;
	int i;
	m_cCB8.ResetContent();
	//for(i=size;i>=0;i--) m_cCB8.DeleteString(i);
}



void CPosition::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int size=11;
	char str[150];
	int i;

	/////ALM

	size=2;
	if(mbALM) {
		m_cCB1.ResetContent();
		//for(i=size;i>=0;i--) m_cCB1.DeleteString(i);
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataALM[i]);
			m_cCB1.AddString(str);
		

		}
	}
	
	size=16;
	if(mbALMTest) {
		m_cCB2.ResetContent();
		//for(i=size;i>=0;i--) m_cCB2.DeleteString(i);
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataALM1[i]);
			m_cCB2.AddString(str);

		}
	}
	
	///////////////////////////DL60
	size=14;
	if(mbDL60) {
		m_cCB3.ResetContent();
		//for(i=size;i>=0;i--) m_cCB3.DeleteString(i);
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataDL60[i]);
			m_cCB3.AddString(str);

		}
	}
	
	size=16;
	if(mbDL60Test) {
		m_cCB4.ResetContent();
		//for(i=size;i>=0;i--) m_cCB4.DeleteString(i);
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataDL601[i]);
			m_cCB4.AddString(str);

		}
	}
	////////////////////// BK3
	size=24;
	if(mbBK3) {
		m_cCB5.ResetContent();
		//for(i=size;i>=0;i--) m_cCB5.DeleteString(i);
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataBK3[i]);
			
			m_cCB5.AddString(str);

		}
	}
	
	//size=11;
	size=66;
	//if(mbBK3Test) {
	if(mbBK79) {
		m_cCB6.ResetContent();
		//for(i=size;i>=0;i--) m_cCB6.DeleteString(i);
		for(i=0;i<22;i++){

			sprintf(str,"W%02d: %05d  W%d: %05d  W%d: %05d",i,pTempDataBK31[i],i+22,pTempDataBK31[i+22],i+44,pTempDataBK31[i+44]);


			//printf(str,"W%d : %ld",i,iBK[i]);
			m_cCB6.AddString(str);

		}
	}
////////////GK76
	size=6;
	if(mbGK76) {
		m_cCB7.ResetContent();
		//for(i=size;i>=0;i--) m_cCB7.DeleteString(i);
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataGK76[i]);
			m_cCB7.AddString(str);

		}
	}
	
	size=11;
	if(mbGK76Test) {
		m_cCB8.ResetContent();
		//for(i=size;i>=0;i--) m_cCB8.DeleteString(i);
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataGK761[i]);
			m_cCB8.AddString(str);

		}
	}

////////////GK60
	//size=10;
	size=10;
	if(mbGK60) {
		m_cCB11.ResetContent();
		//for(i=size;i>=0;i--) m_cCB7.DeleteString(i);
		for(i=0;i<size;i++){
			sprintf(str,"W%d : %d",i,pTempDataGK60[i]);
			
			m_cCB11.AddString(str);

		}
	}
	
	//size=10;
	size = 260;
	//if(mbGK60Test) {
	if(mbSGK){
		m_cCB12.ResetContent();
		//for(i=size;i>=0;i--) m_cCB8.DeleteString(i);
		for(i=0;i<26;i++){

			//sprintf(str,"W%d : %d",i,pTempDataGK601[i]);
			sprintf(str,"W%02d: %05d  W%2d: %05d  W%2d: %05d  W%03d: %05d  W%3d: %05d  W%3d: %05d  W%3d: %05d  W%3d: %05d  W%3d: %05d  W%3d: %05d",
						i,iSGK[i], i+26,iSGK[i+26],i+52,iSGK[i+52],i+78,iSGK[i+78],i+104,iSGK[i+104],i+130,iSGK[i+130],
						i+156,iSGK[i+156],i+182,iSGK[i+182],i+208,iSGK[i+208],i+234,iSGK[i+234]);
			
			m_cCB12.AddString(str);

		}
///view spectrum





		//////



	}

	size=48;
	if(msc60_flag) size=68;
	if(mbMSC) {
		//m_cCB13.ResetContent();
		
		for(i=0;i<size;i++){
			
			
			sprintf(str,"W%d : %d",i,pTempDataMSC[i]);
			m_cCB13.AddString(str);

		}
	}

	size=48;
	if(msc60_flag) size=68;
	if(mbMSCTest) {
		//m_cCB14.ResetContent();
		//for(i=size;i>=0;i--) m_cCB8.DeleteString(i);
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataMSC1[i]);
			m_cCB14.AddString(str);

		}
	}

	size=8;
	if(mbSKP) {
		m_cCB9.ResetContent();
		//for(i=size;i>=0;i--) m_cCB8.DeleteString(i);
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataSKP[i]);
			m_cCB9.AddString(str);

		}
	}

	size=11;
	if(mbSKPTest) {
		m_cCB10.ResetContent();
		//for(i=size;i>=0;i--) m_cCB8.DeleteString(i);
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataSKP1[i]);
			m_cCB10.AddString(str);

		}
	}


	
	CDialog::OnTimer(nIDEvent);
}

BOOL CPosition::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	SetTimer(NULL,1000,NULL);
	m_cMSC.SetCheck(mbMSC);
	m_cAK4_T.SetCheck(mbAK_T);
	m_cAK41.SetCheck(mbAK1_1);
	m_cAK42.SetCheck(mbAK1_2);
	m_cAK43.SetCheck(mbAK2_1);
	m_cAK44.SetCheck(mbAK2_2);
	m_cMSCTest.SetCheck(mbMSCTest);
	m_cMsc_60.SetCheck(msc60_flag);
	//m_cBK361.SetCheck(mbBK79);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPosition::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(NULL);
	
	CDialog::OnClose();
}

void CPosition::OnSkp() 
{
	// TODO: Add your control notification handler code here
	mbSKP = (mbSKP+1)%2;
	
}

void CPosition::OnSkp1() 
{
	// TODO: Add your control notification handler code here
	mbSKPTest = (mbSKPTest+1)%2;
	
}

void CPosition::OnDropdownCombo9() 
{
	// TODO: Add your control notification handler code here
	int size=8;
	char str[50];
	int i;

	
	if(mbSKP) {
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataSKP[i]);

			m_cCB9.AddString(str);
		}
	}
	else m_cCB9.ResetContent();
	
}

void CPosition::OnCloseupCombo9() 
{
	// TODO: Add your control notification handler code here
	int size=11;
	int i;
	m_cCB9.ResetContent();
	
}

void CPosition::OnDropdownCombo10() 
{
	// TODO: Add your control notification handler code here
	int size=11;
	char str[50];
	int i;
	

	if(mbSKPTest) {
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataSKP1[i]);

			m_cCB10.AddString(str);
		}
	}
	else m_cCB10.ResetContent();
	
}

void CPosition::OnCloseupCombo10() 
{
	// TODO: Add your control notification handler code here
	int size=11;
	int i;
	m_cCB10.ResetContent();
	
}

void CPosition::OnGk60() 
{
	// TODO: Add your control notification handler code here
	mbGK60 = (mbGK60+1)%2;
	// = (mbSGK+1)%2;
	
}

void CPosition::OnGk61() 
{
	// TODO: Add your control notification handler code here
	mbGK60Test = (mbGK60Test+1)%2;
	mbSGK = (mbSGK+1)%2;
}

void CPosition::OnDropdownCombo11() 
{
	// TODO: Add your control notification handler code here
	int size=10;
	
	char str[50];
	int i;

	
	if(mbGK60) {
	
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataGK60[i]);
			

			m_cCB11.AddString(str);
		}
	}
	else m_cCB11.ResetContent();
	
}

void CPosition::OnCloseupCombo11() 
{
	// TODO: Add your control notification handler code here
	int size=10;
	
	int i;
	m_cCB11.ResetContent();
	
}

void CPosition::OnDropdownCombo12() 
{
	// TODO: Add your control notification handler code here
	//int size=10;
	int size=260;
	char str[50];
	int i;
	

	//if(mbGK60Test) {
	if(mbSGK) {
		for(i=0;i<size;i++){

			//sprintf(str,"W%d : %d",i,pTempDataGK601[i]);
			sprintf(str,"W%d : %d",i,iSGK[i]);
			m_cCB12.AddString(str);
		}
	}
	else m_cCB12.ResetContent();
}

void CPosition::OnCloseupCombo12() 
{
	// TODO: Add your control notification handler code here
	//int size=10;
	int size=260;
	int i;
	m_cCB12.ResetContent();
}

void CPosition::OnMsc() 
{
	// TODO: Add your control notification handler code here
	mbMSC = (mbMSC+1)%2;
}

void CPosition::OnMsc1() 
{
	// TODO: Add your control notification handler code here
	mbMSCTest = (mbMSCTest+1)%2;
	
}

void CPosition::OnCloseupCombo13() 
{
	// TODO: Add your control notification handler code here
	int size=48;
	if(msc60_flag) size=68;
	int i;
	m_cCB13.ResetContent();
}

void CPosition::OnDropdownCombo13() 
{
	// TODO: Add your control notification handler code here
	int size=48;
	char str[50];
	int i;
	if(msc60_flag) size=68;
	if(mbMSC) {
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataMSC[i]);

			m_cCB13.AddString(str);
		}
	}
	else m_cCB13.ResetContent();
	
}

void CPosition::OnCloseupCombo14() 
{
	// TODO: Add your control notification handler code here
	int size=48;
	if(msc60_flag) size=68;
	int i;
	m_cCB14.ResetContent();
	
}

void CPosition::OnDropdownCombo14() 
{
	// TODO: Add your control notification handler code here
	int size=48;
	char str[50];
	int i;
	if(msc60_flag) size=68;

	if(mbMSCTest) {
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataMSC1[i]);

			m_cCB14.AddString(str);
		}
	}
	else m_cCB14.ResetContent();
	
}

void CPosition::OnAk41() 
{
	// TODO: Add your control notification handler code here
	mbAK1_1 = (mbAK1_1+1)%2;
}

void CPosition::OnAk42() 
{
	// TODO: Add your control notification handler code here
	mbAK1_2 = (mbAK1_2+1)%2;
}

void CPosition::OnAk43() 
{
	// TODO: Add your control notification handler code here
	mbAK2_1 = (mbAK2_1+1)%2;
}

void CPosition::OnAk44() 
{
	// TODO: Add your control notification handler code here
	mbAK2_2 = (mbAK2_2+1)%2;
}

void CPosition::OnAk4T() 
{
	// TODO: Add your control notification handler code here
	mbAK_T = (mbAK_T+1)%2;
}

void CPosition::OnDropdownCombo16() 
{
	// TODO: Add your control notification handler code here
	int size=3;
	char str[50];
	int i;
	if(mbAK_T) {
		for(i=0;i<size;i++){

			sprintf(str,"W%d : %d",i,pTempDataAK[i]);

			m_cCB16.AddString(str);
		}
	}
	else m_cCB16.ResetContent();
	
}

void CPosition::OnCloseupCombo16() 
{
	// TODO: Add your control notification handler code here
	int size=3;
	int i;
	m_cCB16.ResetContent();
	
}

void CPosition::OnMsc60() 
{
	// TODO: Add your control notification handler code here
	msc60_flag = (msc60_flag+1)%2;
}
	

