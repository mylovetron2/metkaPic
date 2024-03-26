// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MFTMonitor.h"

#include "MainFrm.h"
#include "View2.h"
#include "View3.h"
#include "MFTMonitorView.h"
#include "MFTMonitorDoc.h"
#include "Panel.h"

#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const CXANIMATION = 70;

void ResizeRect(CRect* rect);
int		nAnimateID=1;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_COMMAND(IDC_SCALE, OnScale)
	ON_UPDATE_COMMAND_UI(IDC_ONESHOT, OnUpdateOneshot)
	//}}AFX_MSG_MAP
	ON_MESSAGE (WM_READDATATHREAD_UPDATE, OnThreadUpdate)
	ON_COMMAND(IDC_DIRECTION,OnDirection)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_CURDEPTH,
	ID_CURTIME,
	ID_MOREINFO,
};
static UINT BASED_CODE format[]=
{
	IDC_DIRECTION,
	IDC_SCALE,
	IDC_STARTLOGGING,
	IDC_STOPLOGGING,
	IDC_SAVEDATA,
	IDC_STOPSAVING,
	IDC_ONESHOT
};
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame():m_wndToolBar(CXANIMATION, nAnimateID)
{
	// TODO: add member initialization code here
	//pWatch=NULL;
	//bDisplWatch=TRUE;	
	//pReadDataThread=NULL;
	//pReadDataThreadInfo=NULL;
}

CMainFrame::~CMainFrame()
{
	/*if(!bDisplWatch && pWatch!=NULL)
	{	
		pWatch=NULL;
	}*/
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CImageList		img;
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateMyToolBar();
	/*if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString		str;

	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(30,150);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	
	img.Create(IDB_COLDTOOLBAR, 20, 0, RGB(255, 0, 255));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach(); 

	
	m_wndToolBar.SetButtonInfo(0, IDC_DIRECTION, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 0);
	str= _T("Settings");
	m_wndToolBar.SetButtonText(0, str); 
	
	m_wndToolBar.SetButtonInfo(1, IDC_SCALE, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 1);
	str= _T("Scale");
	m_wndToolBar.SetButtonText(1, str);

	m_wndToolBar.SetButtonInfo(2, IDC_STARTLOGGING, TBSTYLE_BUTTON , 2);
	str= _T("Start");
	m_wndToolBar.SetButtonText(2, str);
	
	m_wndToolBar.SetButtonInfo(3, IDC_STOPLOGGING, TBSTYLE_BUTTON , 3);
	str= _T("Stop");
	m_wndToolBar.SetButtonText(3, str);

	m_wndToolBar.SetButtonInfo(4, IDC_SAVEDATA, TBSTYLE_BUTTON , 4);
	str= _T("Save Data");
	m_wndToolBar.SetButtonText(4, str);
	
	m_wndToolBar.SetButtonInfo(5, IDC_STOPSAVING, TBSTYLE_BUTTON , 5);
	str= _T("Stop Saving");
	m_wndToolBar.SetButtonText(5, str);

	m_wndToolBar.SetButtonInfo(6, IDC_ONESHOT, TBSTYLE_BUTTON , 6);
	str= _T("Ignite");
	m_wndToolBar.SetButtonText(6, str);

	CRect rectToolBar;
	// set up toolbar button sizes
	m_wndToolBar.GetItemRect(2, &rectToolBar);
	m_wndToolBar.SetSizes(rectToolBar.Size(), CSize(30,20));*/



	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	//SetTimer(IDC_MYTIMER,200,NULL);

	//pReadDataThread=AfxBeginThread(ReadCommDataThread,NULL,THREAD_PRIORITY_NORMAL,0,0);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.x=0;cs.y=0;
	cs.cx=::GetSystemMetrics(SM_CXSCREEN);
	cs.cy=::GetSystemMetrics(SM_CYSCREEN);
	cs.style=WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
	cs.style|=FWS_ADDTOTITLE;

	/*cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;*/


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	int		nWidth,nHeight;


	nWidth=GetSystemMetrics(SM_CXSCREEN);
	nHeight=GetSystemMetrics(SM_CYSCREEN);
	double xx = nWidth/1024.0;
	double yy = nHeight/768.0;
	//nHeight=0.33*nHeight;
	
	double sc0 = 0.225;
	double sc = (1-sc0)/2;
	
   
	if(!m_wndSpMain.CreateStatic(this,1,2,WS_CHILD|WS_VISIBLE,AFX_IDW_PANE_FIRST))
	{
		TRACE0("Failed to Create Static Splitter\n");
		return false;
	}
	if(!m_wndSpMain.CreateView(0,0,RUNTIME_CLASS(CPanel),CSize(nWidth/4/xx,nHeight/4/yy),pContext))
	
	{
		TRACE0("Failed to Create First pane\n");
		return false;
	}


	if(!m_wndSpSub.CreateStatic(&m_wndSpMain,3,1,WS_CHILD|WS_VISIBLE,m_wndSpMain.IdFromRowCol(0,1)))
	{
		TRACE0("Failed to Create nested splitter\n");
		return false;
	}
	
	
	if(!m_wndSpSub.CreateView(0,0,RUNTIME_CLASS(CMFTMonitorView),CSize(nHeight*sc,nHeight*sc),pContext))
	{
		TRACE0("Failed to Create second pane\n");
		return false;
	}
	if(!m_wndSpSub.CreateView(1,0,RUNTIME_CLASS(CView2),CSize(nHeight*sc,nHeight*sc),pContext))
	{
		TRACE0("Failed to Create first pane\n");
		return false;
	}
	if(!m_wndSpSub.CreateView(2,0,RUNTIME_CLASS(CView3),CSize(nHeight*sc0,nHeight*sc0),pContext))
	{
		TRACE0("Failed to Create third pane\n");
		return false;
	}
	

	pBufData = (MFTData_t*)malloc(filter*sizeof(MFTData_t));
	memset(pBufData,0,filter*sizeof(MFTData_t));
	memset(&gData,0,sizeof(RawData_t));
	memset(&gData1,0,sizeof(RawData_t));
	memset(&gData2,0,sizeof(RawData_t));
	memset(&gData_D,0,sizeof(RawData_t));

	return true;
}
LRESULT CMainFrame::OnThreadUpdate (WPARAM wParam, LPARAM lParam)
{
    return 0;
}

/*BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	int		nWidth,nHeight;


	nWidth=GetSystemMetrics(SM_CXSCREEN);
	nHeight=GetSystemMetrics(SM_CYSCREEN);
	nHeight=3.0*nHeight/5.0;

	if(!m_wndSpMain.CreateStatic(this,1,2,WS_CHILD|WS_VISIBLE,AFX_IDW_PANE_FIRST))
	{
		TRACE0("Failed to Create Static Splitter\n");
		return false;
	}
	if(!m_wndSpMain.CreateView(0,0,RUNTIME_CLASS(CPanel),CSize(nWidth/4.75,nWidth/4.75),pContext))
	{
		TRACE0("Failed to Create First pane\n");
		return false;
	}


	if(!m_wndSpSub.CreateStatic(&m_wndSpMain,2,1,WS_CHILD|WS_VISIBLE,m_wndSpMain.IdFromRowCol(0,1)))
	{
		TRACE0("Failed to Create nested splitter\n");
		return false;
	}
	if(!m_wndSpSub.CreateView(0,0,RUNTIME_CLASS(CMFTMonitorView),CSize(nHeight,nHeight),pContext))
	{
		TRACE0("Failed to Create second pane\n");
		return false;
	}
	if(!m_wndSpSub.CreateView(1,0,RUNTIME_CLASS(CView2),CSize(nHeight,nHeight),pContext))
	{
		TRACE0("Failed to Create third pane\n");
		return false;
	}

	pBufData = (MFTData_t*)malloc(filter*sizeof(MFTData_t));
	memset(pBufData,0,filter*sizeof(MFTData_t));
	memset(&gData,0,sizeof(RawData_t));

	return true;
}*/



/*void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default	
	//bStopFirmware=true;
	CPanel* m_panel=(CPanel*) m_wndSpMain.GetActivePane(0,0);
	if(m_panel != NULL)
		m_panel->StopFirmware();
	CMFTMonitorDoc*	pDoc=(CMFTMonitorDoc*)this->GetActiveDocument();
	
	if(pDoc != NULL){
		pDoc->StopLogging();
		pDoc->OnCloseDocument();
	}
	CloseFlag=1;

	//CButton cb; cb.EnableWindow(false);
	//if(m_rs.IsOpen())
	//	m_rs.Close();
	//if(m_pDB.IsOpen())
	//		m_pDB.Close();
	KillTimer(IDC_MYTIMER);
	
	if(pBufData)
	free(pBufData);
	
	
   // if(pDoc)	pDoc->OnCloseDocument();
	CFrameWnd::OnClose();
	exit(0);
	
}*/

/*void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default	
	bStopFirmware=true;
	bIsLogging=false;
	bIsSaving=false;
	CPanel	*panel=(CPanel*) m_wndSpMain.GetActivePane(0,0);
	if(panel)
		panel->StopFirmware();


	CMFTMonitorDoc*	pDoc=(CMFTMonitorDoc*)this->GetActiveDocument();
	pDoc->StopLogging();
	
	if(pDoc->pView1Thread) pDoc->pView1Thread = NULL;
	if(pDoc->pView2Thread) pDoc->pView2Thread = NULL;
	
	CloseFlag=1;
	if(pFile!=NULL){
			pFile->Close();
			delete pFile;
	}
	
	
	free(pBufData);
	ExitProcess(0);
//	exit(0);
//	CFrameWnd::OnClose();
	
}*/

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default	
	//bStopFirmware=true;
	CPanel	*panel=(CPanel*) m_wndSpMain.GetActivePane(0,0);
	if(panel)
		panel->StopFirmware();
	

	CMFTMonitorDoc*	pDoc=(CMFTMonitorDoc*)this->GetActiveDocument();
//	pDoc->WriteMFTIniFile();
	pDoc->StopLogging();
	bStopFirmware = 1;
	if(pcFile)
		pcFile->Close();

	if(m_pThreadInfo) {
		delete m_pThreadInfo;
	}
	free(pBufData);
	if(pFile)
		pFile->Close();
	
	
	::CoUninitialize();
/*	if(m_rs.IsOpen()) m_rs.Close();
	if(m_Lrs.IsOpen()) m_Lrs.Close();
	if(m_pDB.IsOpen()) {

		m_pDB.m_pDAODatabase=NULL;
		m_pDB.m_pWorkspace->m_pDAOWorkspace=NULL;
		m_pDB.Close();
	}	
	if(m_rRs1.IsOpen()) {
		m_rRs1.Close();
	}
	if(m_pDB1.IsOpen()) {
		m_pDB1.Close();
		m_pDB1.m_pWorkspace = NULL;
		
	}	*/
	ExitProcess(0);
	//CFrameWnd::OnClose();
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	/*BYTE cc;
	static long nT=0;
	
	if(!ReadyFlag && (nT==5))
	{ 
		nT++;
		AfxMessageBox("Turn On Firmware and press ignite");

	}
	nT++;*/
	char sz[80];
	sprintf(sz,"%.1f",info.fDepth);
	SetDlgItemText(ID_CURDEPTH,sz);
	CFrameWnd::OnTimer(nIDEvent);
}

/*void CMainFrame::OnOneshot() //Ignite
{
	// TODO: Add your command handler code here
		PauseFlag=0;
}*/

/*void CMainFrame::CreateMyToolBar()
{
	

	if(!m_wndToolBar.CreateEx(this)||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
		return;

	CImageList		img;
	CString		str;

	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(30,150);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	
	img.Create(IDB_COLDTOOLBAR, 20, 0, RGB(255, 0, 255));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach(); 

	
	m_wndToolBar.SetButtonInfo(0, IDC_DIRECTION, TBSTYLE_FLAT | TBSTYLE_DROPDOWN, 0);
	str= _T("Settings");
	m_wndToolBar.SetButtonText(0, str); 
	
	m_wndToolBar.SetButtonInfo(1, IDC_SCALE, TBSTYLE_FLAT | TBSTYLE_DROPDOWN, 1);
	str= _T("Scale");
	m_wndToolBar.SetButtonText(1, str);
	

	m_wndToolBar.SetButtonInfo(2, IDC_STARTLOGGING, TBSTYLE_FLAT , 2);
	str= _T("Start");
	m_wndToolBar.SetButtonText(2, str);
	
	m_wndToolBar.SetButtonInfo(3, IDC_STOPLOGGING, TBSTYLE_FLAT , 3);
	str= _T("Stop");
	m_wndToolBar.SetButtonText(3, str);

	m_wndToolBar.SetButtonInfo(4, IDC_SAVEDATA, TBSTYLE_FLAT , 4);
	str= _T("Save Data");
	m_wndToolBar.SetButtonText(4, str);
	
	m_wndToolBar.SetButtonInfo(5, IDC_STOPSAVING, TBSTYLE_FLAT , 5);
	str= _T("Stop Saving");
	m_wndToolBar.SetButtonText(5, str);

	m_wndToolBar.SetButtonInfo(6, IDC_ONESHOT, TBSTYLE_FLAT , 6);
	str= _T("Connect");
	m_wndToolBar.SetButtonText(6, str);

	

	CRect rectToolBar;
	int		nWidth,nHeight;


	nWidth=GetSystemMetrics(SM_CXSCREEN);
	nHeight=GetSystemMetrics(SM_CYSCREEN);

	// set up toolbar button sizes
	m_wndToolBar.GetItemRect(2, &rectToolBar);
	//rectToolBar.bottom+=2;
	rectToolBar.InflateRect(0,0,47,2);
	
	m_wndToolBar.SetSizes(rectToolBar.Size(), CSize(30,20));
	m_wndToolBar.m_Static.SetBitmap( LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_LOGO1)) );
}*/

void CMainFrame::CreateMyToolBar()
{
	/*if(!m_wndToolBar.Create(this,WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|
						CBRS_SIZE_DYNAMIC,ID_MYTOOLBAR) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
		return;*/

	if(!m_wndToolBar.CreateEx(this)||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
		return;

	CImageList		img;
	CString		str;

	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(30,150);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	
	img.Create(IDB_COLDTOOLBAR, 20, 0, RGB(255, 0, 255));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach(); 

	
	m_wndToolBar.SetButtonInfo(0, IDC_DIRECTION, TBSTYLE_FLAT | TBSTYLE_DROPDOWN, 0);
	str= _T("Settings");
	m_wndToolBar.SetButtonText(0, str); 
	
	m_wndToolBar.SetButtonInfo(1, IDC_SCALE, TBSTYLE_FLAT | TBSTYLE_DROPDOWN, 1);
	str= _T("Scale");
	m_wndToolBar.SetButtonText(1, str);
	

	m_wndToolBar.SetButtonInfo(2, IDC_STARTLOGGING, TBSTYLE_FLAT , 2);
	str= _T("Start");
	m_wndToolBar.SetButtonText(2, str);
	
	m_wndToolBar.SetButtonInfo(3, IDC_STOPLOGGING, TBSTYLE_FLAT , 3);
	str= _T("Stop");
	m_wndToolBar.SetButtonText(3, str);

	m_wndToolBar.SetButtonInfo(4, IDC_SAVEDATA, TBSTYLE_FLAT , 4);
	str= _T("Save Data");
	m_wndToolBar.SetButtonText(4, str);
	
	m_wndToolBar.SetButtonInfo(5, IDC_STOPSAVING, TBSTYLE_FLAT , 5);
	str= _T("Stop Saving");
	m_wndToolBar.SetButtonText(5, str);

	//m_wndToolBar.SetButtonInfo(6, IDC_ONESHOT, TBSTYLE_FLAT , 6);
	//str= _T("Calibration");
	//m_wndToolBar.SetButtonText(6, str);

	

	CRect rectToolBar,rect;
	int		nWidth,nHeight;


	nWidth=GetSystemMetrics(SM_CXSCREEN);
	nHeight=GetSystemMetrics(SM_CYSCREEN);
	double xx=nWidth/1024.0;
	double yy=nHeight/768.0;
	
	// set up toolbar button sizes
	m_wndToolBar.GetItemRect(2, &rectToolBar);
	ResizeRect(&rectToolBar);
	//rectToolBar.bottom+=2;
	
	rectToolBar.InflateRect(0,0,47.5*xx,2);
	
	m_wndToolBar.SetSizes(rectToolBar.Size(), CSize(30*xx,20*yy));
	m_wndToolBar.m_Static.SetBitmap( LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_LOGO1)) );
}

/*void CMainFrame::OnDirection()
{
	CRect	rect;
	int		index=m_wndToolBar.CommandToIndex(IDC_DIRECTION);
	m_wndToolBar.GetItemRect(index,&rect);
	m_wndToolBar.ClientToScreen(&rect);
	{
		CMenu menu;
		CMenu* pPopup;

		// the font popup is stored in a resource
		menu.LoadMenu(IDR_DIRECTION_POPUP);
		pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.bottom, AfxGetMainWnd());
	}
}

void CMainFrame::OnScale() 
{
	// TODO: Add your command handler code here
	CRect	rect;
	int		index=m_wndToolBar.CommandToIndex(IDC_SCALE);
	m_wndToolBar.GetItemRect(index,&rect);
	m_wndToolBar.ClientToScreen(&rect);
	{
		CMenu menu;
		CMenu* pPopup;

		// the font popup is stored in a resource
		menu.LoadMenu(IDR_SCALE_POPUP);	
		pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.bottom, AfxGetMainWnd());
	}		
}*/
void CMainFrame::OnDirection()
{
	CRect	rect;
	int		index=m_wndToolBar.CommandToIndex(IDC_DIRECTION);
	m_wndToolBar.GetItemRect(index,&rect);
	m_wndToolBar.ClientToScreen(&rect);
	ResizeRect(&rect);
	{
		CMenu menu;
		CMenu* pPopup;

		// the font popup is stored in a resource
		menu.LoadMenu(IDR_DIRECTION_POPUP);
		pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.bottom, AfxGetMainWnd());
	}
}
void CMainFrame::OnScale() 
{
	// TODO: Add your command handler code here
	CRect	rect;
	int		index=m_wndToolBar.CommandToIndex(IDC_SCALE);
	m_wndToolBar.GetItemRect(index,&rect);
	m_wndToolBar.ClientToScreen(&rect);
	ResizeRect(&rect);
	{
		CMenu menu;
		CMenu* pPopup;

		// the font popup is stored in a resource
		menu.LoadMenu(IDR_SCALE_POPUP);	
		pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.bottom, AfxGetMainWnd());
	}		
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//m_wndToolBar.OnSize(nType, cx, cy);	
	//m_wndToolBar.SendMessage(WM_SIZE,nType,MAKELPARAM(cx,cy));
	//m_wndToolBar.m_Static.SetBitmap( LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_LOGO1)) );
}

void ResizeRect(CRect* rect)
{
	int	  nWidth,nHeight;
	nWidth=GetSystemMetrics(SM_CXSCREEN);
	nHeight=GetSystemMetrics(SM_CYSCREEN);
	double xx=nWidth/1024.0;
	double yy=nHeight/768.0;
	rect->top *= yy;
	rect->left *= xx;
	rect->bottom *= yy;
	rect->right *= xx;
}


void CMainFrame::OnUpdateOneshot(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	/*if(PauseFlag)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
	*/
}
