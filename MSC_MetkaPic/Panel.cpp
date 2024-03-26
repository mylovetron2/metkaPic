// Panel.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "Panel.h"
#include "GaugeCtrlSettings.h"
#include "Position.h"
#include "MainFrm.h"
#include "MemDC.h"
#include  "Threads.h"

#include <process.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
int TimeFlag=0;
int hour=0,minute=0,second=0,ss=0;
int h=0,m=0;
int power_flag = 0;

SYSTEMTIME systemtime;
/////////////////////////////////////////////////////////////////////////////
// CPanel

IMPLEMENT_DYNCREATE(CPanel, CFormView)

CPanel::CPanel()
	: CFormView(CPanel::IDD)
{
	//{{AFX_DATA_INIT(CPanel)
	m_sDepth = _T("");
	m_sSpeed = _T("");
	m_sSetDepth = _T("");
	m_sEncoder = _T("");
	m_sMarked = _T("");
	m_sTension = _T("");
	m_sCurrent = _T("");
	//}}AFX_DATA_INIT
	pReadDataThread=NULL;
	pReadDataThreadInfo=NULL;
	pWatch=NULL;
	bDisplWatch=TRUE;	
	

}

CPanel::~CPanel()
{
	if(!bDisplWatch && pWatch!=NULL)
	{	
		pWatch=NULL;
	}
	
}

void CPanel::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanel)
	DDX_Control(pDX, IDC_PLAYBACK_TIME7, m_cLogRTC);
	DDX_Control(pDX, IDC_PLAYBACK_TIME6, m_cRcordRTC);
	DDX_Control(pDX, IDC_PLAYBACK_TIME5, m_cSynchRTC);
	DDX_Control(pDX, IDC_PLAYBACK_TIME4, m_cViewHeader);
	DDX_Control(pDX, IDC_PLAYBACK_TIME3, m_cOnMemToolPower);
	DDX_Control(pDX, IDC_STATIC_CLOSE, m_cClose);
	DDX_Control(pDX, IDC_STATIC_OPEN, m_cOpen);
	DDX_Control(pDX, IDC_SCURRENT, m_cCurrent);
	DDX_Control(pDX, IDC_SINGLE_MARK, m_cSingleMark);
	DDX_Control(pDX, IDC_STENSION, m_cTension);
	DDX_Control(pDX, IDC_SMARKED, m_cMarked);
	DDX_Control(pDX, IDC_ENCODERRATIO, m_cEncoderR);
	DDX_Control(pDX, IDC_DEPTH, m_cSetDepthE);
	DDX_Control(pDX, IDC_SETDEPTH, m_cSetDepths);
	DDX_Control(pDX, IDC_SCLRDEPTH, m_cClrDepth);
	DDX_Control(pDX, IDC_PLAYBACK_DEPTH, m_cPlayInDepth);
	DDX_Control(pDX, IDC_PLAYBACK_TIME, m_cPlayInTime);
	DDX_Control(pDX, IDC_ENCODERCONST, m_cEncoder);
	DDX_Control(pDX, IDC_SSPEED, m_cSpeed);
	DDX_Control(pDX, IDC_SDEPTH, m_cDepth);
	DDX_Text(pDX, IDC_SDEPTH, m_sDepth);
	DDX_Text(pDX, IDC_SSPEED, m_sSpeed);
	DDX_Text(pDX, IDC_DEPTH, m_sSetDepth);
	DDX_Text(pDX, IDC_ENCODERCONST, m_sEncoder);
	DDX_Text(pDX, IDC_SMARKED, m_sMarked);
	DDX_Text(pDX, IDC_STENSION, m_sTension);
	DDX_Text(pDX, IDC_SCURRENT, m_sCurrent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanel, CFormView)
	//{{AFX_MSG_MAP(CPanel)
	ON_COMMAND(IDC_GAUGE_HYDRAULIC, OnGaugeHydraulic)
	ON_COMMAND(IDC_GAUGE_SAMPLE, OnGaugeSample)
	ON_COMMAND(ID_QUARZT_PRETEST, OnQuarztPretest)
	ON_COMMAND(IDC_POSITION, OnPosition)
	ON_COMMAND(IDC_WATCH, OnWatch)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SETDEPTH, OnSetdepth)
	ON_BN_CLICKED(IDC_ENCODERRATIO, OnEncoderratio)
	ON_COMMAND(IDC_STOPCOM, OnStopcom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TIMER, OnStartTimer)
	ON_COMMAND(IDC_ONESHOT, OnOneshot)
	ON_BN_CLICKED(IDC_SCLRDEPTH, OnSclrdepth)
	ON_BN_CLICKED(IDC_STATIC_CALIB, OnStaticCalib)
	ON_BN_CLICKED(IDC_STATIC_EDIT, OnStaticEdit)
	ON_BN_CLICKED(IDC_PLAYBACK_TIME, OnPlaybackTime)
	ON_BN_CLICKED(IDC_PLAYBACK_DEPTH, OnPlaybackDepth)
	ON_BN_CLICKED(IDC_MULTI_MARK, OnMultiMark)
	ON_BN_CLICKED(IDC_SINGLE_MARK, OnSingleMark)
	ON_BN_CLICKED(IDC_STATIC_OPEN, OnStaticOpen)
	ON_BN_CLICKED(IDC_STATIC_CLOSE, OnStaticClose)
	ON_BN_CLICKED(IDC_PLAYBACK_TIME2, OniPageofMark)
	ON_BN_CLICKED(IDC_PLAYBACK_TIME3, OnMemToolPower)
	ON_BN_CLICKED(IDC_PLAYBACK_TIME4, OnViewHeaderMem)
	ON_BN_CLICKED(IDC_PLAYBACK_TIME5, OnSynchRTC)
	ON_BN_CLICKED(IDC_PLAYBACK_TIME6, OnRecordRTC)
	ON_BN_CLICKED(IDC_PLAYBACK_TIME7, OnLogRTC)
	ON_BN_CLICKED(IDC_PLAYBACK_TIME8, OnCreateHeader)
	ON_BN_CLICKED(IDC_PLAYBACK_TIME9, OnFormatSD)
	ON_BN_CLICKED(IDC_PLAYBACK_TIME10, OnLogSD)
	ON_BN_CLICKED(IDC_PLAYBACK_TIME12, OnStopLogging)
	ON_BN_CLICKED(IDC_PLAYBACK_TIME11, OnRTCStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanel diagnostics
DWORD dwExStylesDepth;
#ifdef _DEBUG
void CPanel::AssertValid() const
{

	CFormView::AssertValid();
}

void CPanel::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPanel message handlers

void CPanel::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	CRect rect;
	GetClientRect(rect);
	ScreenToClient(rect);
	

	LOGFONT	logf;
	memset(&logf,0,sizeof(LOGFONT));
	logf.lfHeight=32;	
	logf.lfWeight=FW_EXTRABOLD;
	lstrcpy(logf.lfFaceName,_T("Arial"));
	hFont.CreateFontIndirect(&logf);
	//m_cDepth.SetFont(&hFont,true);
	//m_cSpeed.SetFont(&hFont,true);
	//m_cEditDepth.SetFont(&hFont,true);
	//m_cEditSpeed.SetFont(&hFont,true);
	
	
	m_cDepth.GetClientRect(mRectDepth);
	m_cDepth.ClientToScreen(mRectDepth);
	
	m_cTension.GetClientRect(mRectTension);
	m_cTension.ClientToScreen(mRectTension);

	m_cMarked.GetClientRect(mRectMarked);
	m_cMarked.ClientToScreen(mRectMarked);

	m_cSpeed.GetClientRect(mRectSpeed);
	m_cSpeed.ClientToScreen(mRectSpeed);

	m_cCurrent.GetClientRect(mRectCurrent);
	m_cCurrent.ClientToScreen(mRectCurrent);
	
	int xs = rect.left, ys = rect.top;

	mRectDepth.left += xs;
	mRectDepth.right += xs;
	mRectDepth.top += ys;
	mRectDepth.bottom += ys;

	mRectSpeed.left += xs;
	mRectSpeed.right += xs;
	mRectSpeed.top += ys;
	mRectSpeed.bottom += ys;

	mRectTension.left += xs;
	mRectTension.right += xs;
	mRectTension.top += ys;
	mRectTension.bottom += ys;

	mRectMarked.left += xs;
	mRectMarked.right += xs;
	mRectMarked.top += ys;
	mRectMarked.bottom += ys;
	
	mRectCurrent.left += xs;
	mRectCurrent.right += xs;
	mRectCurrent.top += ys;
	mRectCurrent.bottom += ys;
	
	/*int		nWidth,nHeight;


	nWidth=GetSystemMetrics(SM_CXSCREEN);
	nHeight=GetSystemMetrics(SM_CYSCREEN);
	double xx = nWidth/1024.0;
	double yy = nHeight/768.0;
	
	mRectDepth.left *= xx;
	mRectDepth.right *= xx;
	mRectDepth.top *= yy;
	mRectDepth.bottom *= yy;

	mRectSpeed.left *= xx;
	mRectSpeed.right *= xx;
	mRectSpeed.top *= yy;
	mRectSpeed.bottom *= yy;*/

	// TODO: Add your specialized code here and/or call the base class
	/*m_GaugeCtrl1.SetMaxValue1(nGaugeHydraulicMax1);
	m_GaugeCtrl1.SetMinValue1(nGaugeHydraulicMin1);
	m_GaugeCtrl1.SetMaxValue2(nGaugeHydraulicMax2);
	m_GaugeCtrl1.SetMinValue2(nGaugeHydraulicMin1);
	m_GaugeCtrl1.SetLogaritScale2(bGaugeHydraulicLogarit);
	m_GaugeCtrl1.Invalidate();	

	m_GaugeCtrl2.SetMaxValue1(nGaugeSampleMax1);
	m_GaugeCtrl2.SetMinValue1(nGaugeSampleMin1);
	m_GaugeCtrl2.SetMaxValue2(nGaugeSampleMax2);
	m_GaugeCtrl2.SetMinValue2(nGaugeSampleMin1);
	m_GaugeCtrl2.SetLogaritScale2(bGaugeSampleLogarit);
	m_GaugeCtrl2.Invalidate();	

	m_GaugeCtrl3.SetMaxValue1(nQuartzPretestMax1);
	m_GaugeCtrl3.SetMinValue1(nQuartzPretestMin1);
	m_GaugeCtrl3.SetMaxValue2(nQuartzPretestMax2);
	m_GaugeCtrl3.SetMinValue2(nQuartzPretestMin1);
	m_GaugeCtrl3.SetLogaritScale2(bQuartzPretestLogarit);
	m_GaugeCtrl3.Invalidate();	


	m_PosCtrl.SetMaxPos(nPositionMax);
	m_PosCtrl.SetMinPos(nPositionMin);
	m_PosCtrl.Invalidate();*/

	CString  str;

	str.Format("%f",fEncoderConst);
	SetDlgItemText(IDC_ENCODERCONST,str);

	GetLocalTime(&systemtime);

	buft[0] = systemtime.wDay;
	buft[1] = systemtime.wMonth;
	buft[2] = systemtime.wYear;
	buft[3] = systemtime.wHour;
	buft[4] = systemtime.wMinute;
	buft[5] = systemtime.wSecond;

	str.Format("%02d",systemtime.wDay);
	SetDlgItemText(IDC_ENCODERCONST2,str);
	str.Format("%02d",systemtime.wHour);
	SetDlgItemText(IDC_ENCODERCONST3,str);
	str.Format("%02d",systemtime.wMinute);
	SetDlgItemText(IDC_ENCODERCONST4,str);

	int itx,itt;
	itt = systemtime.wHour;
	itt +=6;
	itx = systemtime.wDay;
	if(itt>24){
		 itt -=24;
		 itx +=1;
		
	}
	str.Format("%02d",itt);
	SetDlgItemText(IDC_ENCODERCONST7,str);
	str.Format("%02d",itx);
	SetDlgItemText(IDC_ENCODERCONST6,str);
	str.Format("%02d",systemtime.wMinute);
	SetDlgItemText(IDC_ENCODERCONST8,str);

	SetDlgItemText(IDC_DEPTH,"0");
	CString sz;
	sz.Format("%.1f",0);
	//m_Speed.SetTime((LPCTSTR)sz);
	//m_Speed.DrawDepthTime();

	//m_Speed.SetDepth(0);
	//m_Speed.DrawDepthTime();
	//m_Speed.SetColor2(false);
	//m_Speed.SetColor1(false);
//	m_DepthCtrl.SetColor1(false);
//	m_DepthCtrl.SetColor2(true);
	
	//SetTimer(NULL,75,NULL);

	m_cOpen.EnableWindow(mbSKP|mbMSC);
	m_cClose.EnableWindow(mbSKP|mbMSC);


	PauseFlag=0;
	CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;


	if(pReadDataThreadInfo!=NULL)
		delete pReadDataThreadInfo;
	pReadDataThreadInfo=new ReadDataThreadInfo_t;


	pReadDataThreadInfo->pWnd=pMainFrm;
	if(pMainFrm==NULL)
	{
		AfxMessageBox("OK");
	}
	LPDWORD lpExitCode;
	UINT nExitCode;
	pReadDataThread=AfxBeginThread(ReadCommDataThread,this,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	pReadDataThread->m_bAutoDelete = true;
	/*CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(pReadDataThreadInfo!=NULL)
		delete pReadDataThreadInfo;
	pReadDataThreadInfo=new ReadDataThreadInfo_t;


	pReadDataThreadInfo->pWnd=pMainFrm;
	if(pMainFrm==NULL)
	{
		AfxMessageBox("OK");
	}
	
	pReadDataThread=AfxBeginThread(ReadCommDataThread,this,THREAD_PRIORITY_HIGHEST,0,0);
	*/
	//m_cEditData =new CEditData(this);
	

}

void CPanel::OnGaugeHydraulic() 
{
	// TODO: Add your command handler code here
	CGaugeCtrlSettings	dlg;
	
/*	dlg.m_fMaxValue1=m_GaugeCtrl1.GetMaxValue1();
	dlg.m_fMinValue1=m_GaugeCtrl1.GetMinValue1();
	dlg.m_fMaxValue2=m_GaugeCtrl1.GetMaxValue2();
	dlg.m_fMinValue2=m_GaugeCtrl1.GetMinValue2();
	dlg.m_bLogarit=m_GaugeCtrl1.GetLogaritScale2();
	if(dlg.DoModal()==IDOK)
	{
		m_GaugeCtrl1.SetMaxValue1(dlg.m_fMaxValue1);
		m_GaugeCtrl1.SetMinValue1(dlg.m_fMinValue1);
		m_GaugeCtrl1.SetMaxValue2(dlg.m_fMaxValue2);
		m_GaugeCtrl1.SetMinValue2(dlg.m_fMinValue2);
		m_GaugeCtrl1.SetLogaritScale2(dlg.m_bLogarit);
		m_GaugeCtrl1.Invalidate();

		CMFTMonitorDoc*	pDoc=GetDocument();
		nGaugeHydraulicMax1=dlg.m_fMaxValue1;
		nGaugeHydraulicMin1=dlg.m_fMinValue1;
		nGaugeHydraulicMax2=dlg.m_fMaxValue2;
		nGaugeHydraulicMin2=dlg.m_fMinValue2;
		bGaugeHydraulicLogarit=dlg.m_bLogarit;
		pDoc->WriteMFTIniFile();
	}*/
}

void CPanel::OnGaugeSample() 
{
	// TODO: Add your command handler code here
	CGaugeCtrlSettings	dlg;
	
/*	dlg.m_fMaxValue1=m_GaugeCtrl2.GetMaxValue1();
	dlg.m_fMinValue1=m_GaugeCtrl2.GetMinValue1();
	dlg.m_fMaxValue2=m_GaugeCtrl2.GetMaxValue2();
	dlg.m_fMinValue2=m_GaugeCtrl2.GetMinValue2();
	dlg.m_bLogarit=m_GaugeCtrl2.GetLogaritScale2();
	if(dlg.DoModal()==IDOK)
	{
		m_GaugeCtrl2.SetMaxValue1(dlg.m_fMaxValue1);
		m_GaugeCtrl2.SetMinValue1(dlg.m_fMinValue1);
		m_GaugeCtrl2.SetMaxValue2(dlg.m_fMaxValue2);
		m_GaugeCtrl2.SetMinValue2(dlg.m_fMinValue2);
		m_GaugeCtrl2.SetLogaritScale2(dlg.m_bLogarit);
		m_GaugeCtrl2.Invalidate();

		CMFTMonitorDoc*	pDoc=GetDocument();
		nGaugeSampleMax1=dlg.m_fMaxValue1;
		nGaugeSampleMin1=dlg.m_fMinValue1;
		nGaugeSampleMax2=dlg.m_fMaxValue2;
		nGaugeSampleMin2=dlg.m_fMinValue2;
		bGaugeSampleLogarit=dlg.m_bLogarit;
		pDoc->WriteMFTIniFile();
	}*/
	
}

void CPanel::OnQuarztPretest() 
{
	// TODO: Add your command handler code here
	CGaugeCtrlSettings	dlg;

/*	dlg.m_fMaxValue1=m_GaugeCtrl3.GetMaxValue1();
	dlg.m_fMinValue1=m_GaugeCtrl3.GetMinValue1();
	dlg.m_fMaxValue2=m_GaugeCtrl3.GetMaxValue2();
	dlg.m_fMinValue2=m_GaugeCtrl3.GetMinValue2();
	dlg.m_bLogarit=m_GaugeCtrl3.GetLogaritScale2();
	if(dlg.DoModal()==IDOK)
	{
		m_GaugeCtrl3.SetMaxValue1(dlg.m_fMaxValue1);
		m_GaugeCtrl3.SetMinValue1(dlg.m_fMinValue1);
		m_GaugeCtrl3.SetMaxValue2(dlg.m_fMaxValue2);
		m_GaugeCtrl3.SetMinValue2(dlg.m_fMinValue2);
		m_GaugeCtrl3.SetLogaritScale2(dlg.m_bLogarit);
		m_GaugeCtrl3.Invalidate();


		CMFTMonitorDoc*	pDoc=GetDocument();
		nQuartzPretestMax1=dlg.m_fMaxValue1;
		nQuartzPretestMin1=dlg.m_fMinValue1;
		nQuartzPretestMax2=dlg.m_fMaxValue2;
		nQuartzPretestMin2=dlg.m_fMinValue2;
		bQuartzPretestLogarit=dlg.m_bLogarit;
		pDoc->WriteMFTIniFile();
	}*/
}

void CPanel::OnPosition() 
{
	// TODO: Add your command handler code here
	CPosition	dlg;
	
	
	dlg.m_sALM= (BOOL)mbALM;
	dlg.m_sALMTest= (BOOL)mbALMTest;
	dlg.m_sDL60= (BOOL)mbDL60;
	dlg.m_sDL60Test= (BOOL)mbDL60Test;
	dlg.m_sGK76 = (BOOL)mbGK76;
	dlg.m_sGK76Test = (BOOL)mbGK76Test;
	dlg.m_sGK60 = (BOOL)mbGK60;
	dlg.m_sGK60Test = (BOOL)mbGK60Test;
	dlg.m_sBK3= (BOOL)mbBK3;
	dlg.m_sBK3Test= (BOOL)mbBK3Test;
	dlg.m_sSKP= (BOOL)mbSKP;	
	dlg.m_sSKPTest= (BOOL)mbSKPTest;
	

	m_cOpen.EnableWindow(mbSKP|mbMSC);
	m_cClose.EnableWindow(mbSKP|mbMSC);

	if(dlg.DoModal()==IDOK)
	{
		//m_PosCtrl.SetMaxPos(dlg.m_sALM);
		//m_PosCtrl.SetMinPos(dlg.m_cdl60);
		//dlg.Invalidate();

		CMFTMonitorDoc*	pDoc=GetDocument();
		//nPositionMax=dlg.m_fMax;
		//nPositionMin=dlg.m_fMin;
		pDoc->WriteMFTIniFile();
	}
}

void CPanel::DisplayDataInPanel(UpdateInfo_t	info)
{
	

	//CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	//if(pMainFrm==NULL)	return;

	//if(bStopFirmware)		return;
	
	//CPanel	*panel=this;
	//if(panel==NULL)		return;
	
	UpdateInfo_t*	pInfo=&info;

	//static int first = 0;


	m_fDepth = pInfo->fDepth;

	m_fDepth = gData.fDepth;
	m_fSpeed = pInfo->fSpeed;
	m_sDepth.Format("%.1f",m_fDepth);
	m_sSpeed.Format("%.1f",m_fSpeed);
//	if(!first) info.fTemp1 = 0;
	//m_sMarked.Format("%.2f",info.fTemp1);
//	first = 1;
	//CPanel	*panel=this;
	//if(panel==NULL)		return;
	//CDC* m_pdc = GetDC();

	
	//m_pdc->SelectObject(&hFont);
	//OnDraw(m_pdc);
	//char str[20];
	//SetDlgItemText(IDC_EDIT_DEPTH,m_sDepth);
	//SetDlgItemText(IDC_EDIT_SPEED,m_sSpeed);
	//SetDlgItemText(IDC_SDEPTH,m_sDepth);
	//SetDlgItemText(IDC_SSPEED,m_sSpeed);
	
	
	//for(int i=0;i<8;i++){
	//	sprintf(str,"%.3f",gData.fData[20+i]);
	//	SetDlgItemText(IDC_EDIT1+i,str);
	//}
	
	/*CDC*	m_pdc = this->GetDC();
	if(m_pdc == NULL) return;
	m_pdc->SelectObject(&hFont);
	
	
	
	
	m_pdc->SetBkColor(RGB(0,0,0));
	m_pdc->SetTextAlign(TA_CENTER|TA_TOP);
	m_pdc->SetBkMode(TRANSPARENT);
	
	
	
	m_pdc->FillSolidRect(mRectDepth,RGB(0,0,0));
	m_pdc->FillSolidRect(mRectSpeed,RGB(0,0,0));
	
	
	m_pdc->SetTextColor(RGB(255,0,0));
	if(abs(pInfo->fDepth < 10000))
	m_pdc->TextOut(mRectDepth.left+(mRectDepth.right-mRectDepth.left)/2,
								mRectDepth.top,m_sDepth);

	
	m_pdc->SetTextColor(RGB(0,255,0));
	if(abs(pInfo->fSpeed < 2000))
	m_pdc->TextOut(mRectSpeed.left+(mRectSpeed.right-mRectSpeed.left)/2,
								mRectSpeed.top,m_sSpeed);*/
	

	//panel->m_DepthCtrl.SetTime(pInfo->szCurTime);
	
//	panel->m_DepthCtrl.SetDepth(pInfo->fDepth);

//	m_DepthCtrl.DrawDepthTime();
	
/*	m_GaugeCtrl1.SetCurValue1(pInfo->fTemp1);
	m_GaugeCtrl1.SetCurValue2(pInfo->fPres1);
	m_GaugeCtrl1.DrawBars();

	m_GaugeCtrl2.SetCurValue1(pInfo->fTemp2);
	m_GaugeCtrl2.SetCurValue2(pInfo->fPres2);
	m_GaugeCtrl2.DrawBars();

	m_GaugeCtrl3.SetCurValue1(pInfo->fTemp3);
	m_GaugeCtrl3.SetCurValue2(pInfo->fPres3);
	m_GaugeCtrl3.DrawBars();

	m_PosCtrl.SetCurPos(pInfo->fPistonPos);
	m_PosCtrl.SetValve1Open(pInfo->bValve1Open);
	m_PosCtrl.SetValve2Open(pInfo->bValve2Open);
	m_PosCtrl.DrawAll();
	
	m_Speed.SetCurValue1(pInfo->fSpeed);
	m_Speed.SetCurValue2(pInfo->fMotorVolt);
	m_Speed.DrawBars();*/
	//m_Speed.SetTime((LPCTSTR)sz);
	//m_Speed.DrawDepthTime();

	//m_Speed.SetDepth(pInfo->fSpeed);
	//m_Speed.DrawDepthTime();


	SetDlgItemText(IDC_ENCODERCONST5,szTime);
	
	Invalidate(false);	
		
	if(!bDisplWatch)	
		pWatch->DisplayData();
	
}

void CPanel::OnWatch() 
{
	// TODO: Add your command handler code here
	static int t=0;
	
	//if(!bDisplWatch) return;
	if(!t){

		pWatch=new CWatch(this);

		pWatch->Create();
		
		pWatch->ShowWindow(SW_SHOW);
		t=1;
	} else
		pWatch->ShowWindow(SW_SHOW);
	
	bDisplWatch=FALSE;

}

void CPanel::StopFirmware()
{
	//AfxMessageBox("Close ReadData Thread");

//	CMFTMonitorDoc*	pDoc=GetDocument();
//	pDoc->WriteMFTIniFile();
	
//	pDoc->OnCloseDocument();
//	::WaitForSingleObject(pReadDataThread,INFINITE);

	DWORD  l;

	//cmd_flag=3;
	//bSendOK=true;
	//pWatch=NULL;
	//bDisplWatch=TRUE;

	mwait(300);
	bStopFirmware=true;
	mwait(300);
	//l = ::WaitForSingleObject(pReadDataThread,INFINITE) ;
	//	pReadDataThread->m_bAutoDelete = true;

	
	bDisplWatch=TRUE;
	if(pWatch)
	pWatch->CloseWindow();
	
}

CMFTMonitorDoc* CPanel::GetDocument()
{
	return (CMFTMonitorDoc*)m_pDocument;
}

void CPanel::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//AfxMessageBox("Close ReadData Thread");
	//::WaitForSingleObject(pReadDataThread,INFINITE);
	CFormView::OnClose();
}

void CPanel::OnSetdepth() 
{
	// TODO: Add your control notification handler code here
	CString		str;
	double		fDepth;

	GetDlgItemText(IDC_DEPTH,str);
	fDepth=atof(str);
	
	nSetDepth=int(fDepth*fEncoderConst);

	nEncoder = int(10*fEncoderConst);
	
	dmt_flag = 1;
	m_cSingleMark.EnableWindow(true);

	load_flag = 0;
	mwait(200);
	bSendOK=true;
	cmd_flag = 4;
	mwait(500);
	load_flag=1;
	bSendOK=true;
	cmd_flag = 1;
}

void CPanel::OnEncoderratio() 
{
	// TODO: Add your control notification handler code here
	CString		str;
	
	GetDlgItemText(IDC_ENCODERCONST,str);
	fEncoderConst=atof(str);
	nEncoder = int(10*fEncoderConst);
	//int i = nATension;
	//int j = nBTension;
	
	dmt_flag = 2;
	load_flag = 0;
	mwait(200);
	bSendOK=true;
	cmd_flag = 4;
	mwait(500);
	load_flag=1;
	bSendOK=true;
	cmd_flag = 1;
	//bSendOK=true;
	//load_flag=2;
	//CMFTMonitorDoc* pDoc=GetDocument();
	//pDoc->WriteMFTIniFile(); new
}

void CPanel::OnStopcom() 
{
	// TODO: Add your command handler code here
	//StopFirmware();
	//bStopFirmware ^=1;
	//::Sleep(1000);
	::WaitForSingleObject(pReadDataThread,500);
	
}

void CPanel::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	SYSTEMTIME		SystemTime;
	GetLocalTime(&SystemTime);
	//bSendOK=true;
	//int h=0,m=0;
	if(!TimeFlag) sprintf(info.szCurTime,"%02d:%02d:%02d",SystemTime.wHour,
			SystemTime.wMinute,SystemTime.wSecond);
	else {
		
		
		if(abs(SystemTime.wSecond - second)>0) {
			ss++;
			second = SystemTime.wSecond;
			
		}
		if(ss == 60 ) {
			m++;
			ss =0;
			
		}
		if(m == 60) {
			m = 0;
			h++;
		}
		if(h == 24) h=0;
		sprintf(info.szCurTime,"%02d:%02d:%02d - %d:%02d:%02d",SystemTime.wHour,
				SystemTime.wMinute,SystemTime.wSecond,h,m,ss);
	}
	
//	m_DepthCtrl.SetTime(info.szCurTime);
	//m_DepthCtrl.SetDepth(info.fDepth);
//	m_DepthCtrl.DrawDepthTime();
	/*
	m_Speed.SetCurValue1(info.fSpeed);
	
	m_Speed.SetCurValue2(info.fMotorVolt);
	m_Speed.DrawBars();*/
	
	/*if(!bDisplWatch){
		pWatch->m_fDepth=info.fDepth;
		pWatch->m_strTime=info.szCurTime;
		pWatch->DisplayData();
	}*/
		
	  CFormView::OnTimer(nIDEvent);
}

void CPanel::OnStartTimer() 
{
	// TODO: Add your control notification handler code here
	SYSTEMTIME		SystemTime;
	GetLocalTime(&SystemTime);
	TimeFlag = (TimeFlag+1)%2;
	ss=0;
	hour = SystemTime.wHour;
	minute = SystemTime.wMinute;
	second = SystemTime.wSecond;
	h=0;
	m=0;
}

void CPanel::OnOneshot() 
{
	// TODO: Add your command handler code here

	/*PauseFlag=0;
	CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(pReadDataThreadInfo!=NULL)
		delete pReadDataThreadInfo;
	pReadDataThreadInfo=new ReadDataThreadInfo_t;


	pReadDataThreadInfo->pWnd=pMainFrm;
	if(pMainFrm==NULL)
	{
		AfxMessageBox("OK");
	}
	
	pReadDataThread=AfxBeginThread(ReadCommDataThread,this,THREAD_PRIORITY_HIGHEST,0,0);*/
}

void CPanel::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	static int nc=0;

	pDC->SelectObject(&hFont);
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextAlign(TA_CENTER|TA_TOP);
	pDC->SetBkMode(TRANSPARENT);
		
	
	if(abs(m_fDepth) < 100000){
		pDC->SetTextColor(RGB(255,0,0));
		pDC->FillSolidRect(mRectDepth,RGB(0,0,0));
		pDC->TextOut(mRectDepth.left+(mRectDepth.right-mRectDepth.left)/2,
								mRectDepth.top,m_sDepth);
	}
	else {
		m_sDepth.Format("%.1f",9999.9);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->TextOut(mRectDepth.left+(mRectDepth.right-mRectDepth.left)/2,
								mRectDepth.top,m_sDepth);
	}
	



	if(abs(m_fSpeed )< 100000){
		
		pDC->SetTextColor(RGB(0,255,0));
		pDC->FillSolidRect(mRectSpeed,RGB(0,0,0));
		pDC->TextOut(mRectSpeed.left+(mRectSpeed.right-mRectSpeed.left)/2,
								mRectSpeed.top,m_sSpeed);
	
	}
	else{
		m_sSpeed.Format("%.1f",999.9);
	
		pDC->SetTextColor(RGB(0,255,0));
		pDC->FillSolidRect(mRectSpeed,RGB(0,0,0));
		pDC->TextOut(mRectSpeed.left+(mRectSpeed.right-mRectSpeed.left)/2,
								mRectSpeed.top,m_sSpeed);
	
	}

	
	//m_sTension.Format("%.f",gData.fData[21]);
	m_sTension.Format("%.1f",gData.fData[0]);
	//if(gData.fData[21] > 0){
		
		pDC->SetTextColor(RGB(0,255,255));
		pDC->FillSolidRect(mRectTension,RGB(0,0,0));
		pDC->TextOut(mRectTension.left+(mRectTension.right-mRectTension.left)/2,
								mRectTension.top,m_sTension);
	
	//}
/*	else{
		m_sTension.Format("%.1f",-1*gData.fData[21]);
	
		pDC->SetTextColor(RGB(0,255,255));
		pDC->FillSolidRect(mRectTension,RGB(0,0,0));
		pDC->TextOut(mRectTension.left+(mRectTension.right-mRectTension.left)/2,
								mRectTension.top,m_sTension);
	
	}*/

	//m_sMarked.Format("%.1f",gData.fData[22]);
	m_sMarked.Format("%.3f",gData.fData[45]);	// Volt
	//if((gData.fData[22] > 10)|power_flag){
		pDC->SetTextColor(RGB(255,255,0));
		pDC->FillSolidRect(mRectMarked,RGB(0,0,0));
		pDC->TextOut(mRectMarked.left+(mRectMarked.right-mRectMarked.left)/2,
								mRectMarked.top,m_sMarked);
/*	}
	else {
		//m_sMarked.Format("%.1f",gData.fData[22]);

		pDC->SetTextColor(RGB(255,255,0));
		pDC->FillSolidRect(mRectMarked,RGB(0,0,0));
		pDC->TextOut(mRectMarked.left+(mRectMarked.right-mRectMarked.left)/2,
								mRectMarked.top,m_sMarked);
	}*/

	m_sCurrent.Format("%.1f",gData.fData[46]);	//Current

	//m_sCurrent.Format("%.1f",info.fTemp1);
//	if(gData.fData[1] > 80){
		
		pDC->SetTextColor(RGB(255,255,0));
		pDC->FillSolidRect(mRectCurrent,RGB(0,0,0));
		pDC->TextOut(mRectCurrent.left+(mRectCurrent.right-mRectCurrent.left)/2,
								mRectCurrent.top,m_sCurrent);
	
//	}
//	else{
		//m_sCurrent.Format("%.1f",gData.fData[23]);
	
//		pDC->SetTextColor(RGB(255,255,0));
//		pDC->FillSolidRect(mRectCurrent,RGB(0,0,0));
//		pDC->TextOut(mRectCurrent.left+(mRectCurrent.right-mRectCurrent.left)/2,
//								mRectCurrent.top,m_sCurrent);
	
	
//	}


/*	if(!nc){
		m_cClose.SetWindowText("SKP Arm Close");
		m_cOpen.SetWindowText("SKP Arm Open");
		nc=1;
	}
*/
	
	if(mbClose|mbMSCClose) m_cClose.SetWindowText("Closing Arm!");
	
	if(mbOpen|mbMSCOpen) m_cOpen.SetWindowText("Opening Arm!");
	

	if(mbSKP){
			if(fTempDataSKP[8]==1) {
				m_cClose.EnableWindow(false);
				m_cOpen.EnableWindow(true);
				
				if(runingArm == 1) {
				
				m_cClose.SetWindowText("Close SKP Arm!");
				runingArm = 0;

				}
			}
			else if(fTempDataSKP[8]==2) {
				m_cClose.EnableWindow(true);
				m_cOpen.EnableWindow(false);

				if(runingArm == 2) {
					m_cOpen.SetWindowText("Open SKP Arm!");
					runingArm = 0;

			}

			}

			else if(fTempDataSKP[8]==3) {
				m_cClose.EnableWindow(true);
				m_cOpen.EnableWindow(true);
			}

			
			
			

		}
		else {
			m_cClose.EnableWindow(false);
			m_cOpen.EnableWindow(false);

			
		}

		//MSC

		if(mbMSC){
			
			m_cClose.EnableWindow(true);
			m_cOpen.EnableWindow(true);

			
		}
		
	
}

void CPanel::OnSclrdepth() 
{
	// TODO: Add your control notification handler code here
		
	nSetDepth=0;
	iSetMarked=0;
	dmt_flag = 1;
	//load_flag=1;
	m_cSingleMark.EnableWindow(true);
	
	load_flag = 0;
	mwait(200);
	bSendOK=true;
	cmd_flag = 4;
	mwait(500);
	load_flag=1;
	bSendOK=true;
	cmd_flag = 1;

}

void CPanel::SetDCMem(CDC *dc, CRect rect,CString cs)
{
	CBitmap  m_bitmap;      // Offscreen bitmap
    CBitmap  *m_pOldBitmap = NULL;  // bitmap originally found in CMemDC
    CDC		 *pDC = dc;         // Saves CDC passed in constructor
    
    	
		
		CreateCompatibleDC(dc->m_hDC);
        m_bitmap.CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
        m_pOldBitmap = pDC->SelectObject(&m_bitmap);
        pDC->SelectObject(&hFont);
		pDC->SetBkColor(RGB(0,0,0));
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		pDC->SetBkMode(TRANSPARENT);
		
	
		pDC->SetTextColor(RGB(255,0,0));
		pDC->FillSolidRect(rect,RGB(0,0,0));
		pDC->TextOut(rect.left+(rect.right-rect.left)/2,rect.top,cs);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
                          dc, rect.left, rect.top, SRCCOPY);

        //Swap back the original bitmap.
		pDC->SelectObject(m_pOldBitmap);
        
}

void CPanel::OnStaticCalib() 
{
	// TODO: Add your control notification handler code here
	m_cCalib =new CCalibDlg(this);
	m_cCalib->Create(IDD_CALIB,NULL);
		
	m_cCalib->ShowWindow(SW_SHOW);
	
}

void CPanel::OnStaticEdit() 
{
	// TODO: Add your control notification handler code here
	m_cEditData =new CEditData(this);
	m_cEditData->Create(IDD_EDITDATA,NULL);
	m_cEditData->ShowWindow(SW_SHOW);

}

void CPanel::OnPlaybackTime() 
{
	// TODO: Add your control notification handler code here
	
/*	m_cRelog =new CRelog(this);
	m_cRelog->Create(IDD_RELOG,NULL);
	CRect rect;
	m_cRelog->GetClientRect(rect);
	m_cRelog->ScreenToClient(rect);	
	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);
	float fx = cx/1024.0;
	float fy = cy/768.0;
	
	
	rect.left = 250*fx;
	rect.right = 750*fx;
	rect.top = 150*fy;
	rect.bottom = 650*fy;

	m_cPlayInDepth.EnableWindow(false);
	m_cPlayInTime.EnableWindow(false);
	
	m_cRelog->MoveWindow(&rect,TRUE);
	m_cRelog->ShowWindow(SW_SHOW);	*/
	
	
	
	dmt_flag = 4;
	m_cSingleMark.EnableWindow(true);

	load_flag = 0;
	mwait(200);
	bSendOK=true;
	cmd_flag = 4;
	mwait(500);
	load_flag=1;
	bSendOK=true;
	cmd_flag = 1;


}

void CPanel::OnMultiMark() 
{
	// TODO: Add your control notification handler code here
	
//	cmd_flag=3;
	
//	bSendOK=true;
}

void CPanel::OnSingleMark() 
{
	// TODO: Add your control notification handler code here
	
	static int on_off = 3;
	/*if(on_off == 3){
		if(MessageBox("Are you turn on Tool Power Supply?","Tool Power",MB_YESNO) == 6){
			cmd_flag=2;
			bSendOK=true;
			m_cSingleMark.SetWindowText("Tool - Power ON");
			on_off = 2;
			power_flag = 1;
		}
		else return;
	}
	else {
		cmd_flag=3;
		bSendOK=true;
		m_cSingleMark.SetWindowText("Tool - Power OFF");
		on_off = 3;
		power_flag = 0;
	}
	*/
	
	//m_cPosition =new CPosition(this);
	//m_cPosition->Create(IDD_POSITION,NULL);
	//m_cPosition->ShowWindow(SW_SHOW);
//	static int t=0;
	
	//if(!bDisplWatch) return;
//	if(!t){
//
		OnPosition();
//		t=1;
//	} else
//		pWatch->ShowWindow(SW_SHOW);
	
	
}

void CPanel::OnChangeAtensionst() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	
}

void CPanel::OnChangeBtensionst() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

int m_iAutoOpenClose=0, m_iManuOpenClose=0;

void CPanel::OnStaticOpen() 
{
	// TODO: Add your control notification handler code here
//	m_iAutoOpenClose = (m_iAutoOpenClose+1)%2;

	//load_flag = 0;
	//mwait(200);
	//cmd_flag = 17;

/*	if (m_iAutoOpenClose) {
		
		m_cOpen.SetWindowText("Full Arm Opening");
		cmd_flag = 15;

	}
		
	
	else {
		m_cOpen.SetWindowText("Full Arm Closing");
		cmd_flag = 16;

	}*/
		
	
	//bSendOK=true;
	
	//mwait(200);
	//load_flag=1;
	//bSendOK=true;
	//cmd_flag = 1;
	

	if(mbSKP) mbOpen = (mbOpen+1)%2;
	if(mbMSC) mbMSCOpen = (mbMSCOpen+1)%2;

	if(mbOpen|mbMSCOpen) m_cOpen.SetWindowText("Opening Arm!");
	else m_cOpen.SetWindowText("Arm Open");

	
}

void CPanel::OnStaticClose() 
{
	// TODO: Add your control notification handler code here
//	m_iManuOpenClose = (m_iManuOpenClose+1)%2;

	//load_flag = 0;
	//mwait(200);

	//cmd_flag = 18;

/*	if (m_iManuOpenClose) {
		m_cClose.SetWindowText("Manual Arm Opening");
		cmd_flag = 17;
	}
		
	
	else {
		m_cClose.SetWindowText("Manual Arm Closing");
		cmd_flag = 18;
	}*/

	
//	bSendOK=true;
	
//	mwait(200);
//	load_flag=1;
//	bSendOK=true;
//	cmd_flag = 1;
	if(mbSKP) mbClose = (mbClose+1)%2;
	if(mbMSC) mbMSCClose = (mbMSCClose+1)%2;

	if(mbClose|mbMSCClose) m_cClose.SetWindowText("Closing Arm!");
	else m_cClose.SetWindowText("Arm Close");
	
}

void CPanel::OnPlaybackDepth() 
{
	// TODO: Add your control notification handler code here
CString		str;
	float		fd;

	GetDlgItemText(IDC_DEPTH,str);
	fd=atoi(str);
	
	nSetDepth=int(fd);

	
	
	dmt_flag = 3;
	m_cSingleMark.EnableWindow(true);

	load_flag = 0;
	mwait(200);
	bSendOK=true;
	cmd_flag = 4;
	mwait(500);
	load_flag=1;
	bSendOK=true;
	cmd_flag = 1;

	
}

void CPanel::OniPageofMark() 
{
	// TODO: Add your control notification handler code here
	dmt_flag = 5;
	m_cSingleMark.EnableWindow(true);

	load_flag = 0;
	mwait(200);
	bSendOK=true;
	cmd_flag = 4;
	mwait(500);
	load_flag=1;
	bSendOK=true;
	cmd_flag = 1;
}
int kk=0,ll=0;
void CPanel::OnMemToolPower() 
{
	// TODO: Add your control notification handler code here

	kk = (kk+1)%2;
	
	
	if(kk) {
		mbOnPowerMem=1;
		m_cOnMemToolPower.SetWindowText("Off Mem Tool Power");
		cmd_flag = 37;
	}
	else {
		m_cOnMemToolPower.SetWindowText("On Mem Tool Power");
		mbOffPowerMem = 1;
		cmd_flag = 38;
	}
	load_flag = 0;
	bSendOK=true;
	mwait(250);
	load_flag=1;
	
	
}

void CPanel::OnViewHeaderMem() 
{
	// TODO: Add your control notification handler code here
	cmd_flag = 32;
	load_flag = 0;
	bSendOK=true;
	mwait(250);
	load_flag=1;
	
}

void CPanel::OnSynchRTC() 
{
	// TODO: Add your control notification handler code here
	cmd_flag = 39;
	load_flag = 0;
	bSendOK=true;
	mwait(250);
	load_flag=1;
}

void CPanel::OnRecordRTC() 
{
	// TODO: Add your control notification handler code here
	CString		str;
	
	GetDlgItemText(IDC_ENCODERCONST2,str);
	buft[0]=atoi(str);
	GetDlgItemText(IDC_ENCODERCONST3,str);
	buft[3]=atoi(str);
	GetDlgItemText(IDC_ENCODERCONST4,str);
	buft[4]=atoi(str);

	cmd_flag = 31;
	load_flag = 0;
	bSendOK=true;
	mwait(250);
	load_flag=1;
	
}

void CPanel::OnLogRTC() 
{
	// TODO: Add your control notification handler code here
	ll = (ll+1)%2;
	
	
	if(ll) {
		m_cLogRTC.SetWindowText("RTC Logoff");
		cmd_flag = 35;
	}
	else {
		m_cLogRTC.SetWindowText("RTC Logon");
		cmd_flag = 36;
	}
	load_flag = 0;
	bSendOK=true;
	mwait(250);
	load_flag=1;
	
}

void CPanel::OnCreateHeader() 
{
	// TODO: Add your control notification handler code here
	cmd_flag = 33;
	load_flag = 0;
	bSendOK=true;
	mwait(250);
	load_flag=1;
}

void CPanel::OnFormatSD() 
{
	// TODO: Add your control notification handler code here

	cmd_flag = 34;
	load_flag = 0;
	bSendOK=true;
	mwait(250);
	load_flag=1;
}

void CPanel::OnLogSD() 
{
	// TODO: Add your control notification handler code here

	cmd_flag = 30;
	load_flag = 0;
	bSendOK=true;
	mwait(250);
	load_flag=1;
}

void CPanel::OnStopLogging() 
{
	// TODO: Add your control notification handler code here
	cmd_flag = 40;
	load_flag = 0;
	bSendOK=true;
	mwait(250);
	load_flag=1;
}

void CPanel::OnRTCStop() 
{
	// TODO: Add your control notification handler code here
	CString		str;
	
	GetDlgItemText(IDC_ENCODERCONST6,str);
	buft[6]=atoi(str);
	GetDlgItemText(IDC_ENCODERCONST7,str);
	buft[9]=atoi(str);
	GetDlgItemText(IDC_ENCODERCONST8,str);
	buft[10]=atoi(str);

	cmd_flag = 41;
	load_flag = 0;
	bSendOK=true;
	mwait(250);
	load_flag=1;
}
