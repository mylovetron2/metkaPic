// mftDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mft.h"
#include "mftDlg.h"
#include "Comm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
DWORD WINAPI CommWatchProc();


DWORD ThreadID;

int Index = 0,ReadyFlag=0,filter = 1;
int DepthFlag=0;

int MFtFlag=0;
int UpdateFlag=0;
int SendFlag=0;
UINT iCurrentDepth=0;
int BufSize=68;



int Count_d=0;


/////////////////////////////////////////////////////////////////////////////
// CMftDlg dialog

CMftDlg::CMftDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMftDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMftDlg)
	m_MA1 = _T("");
	m_MA10 = _T("");
	m_MA11 = _T("");
	m_MA12 = _T("");
	m_MA13 = _T("");
	m_MA14 = _T("");
	m_MA15 = _T("");
	m_MA2 = _T("");
	m_MA3 = _T("");
	m_MA4 = _T("");
	m_MA5 = _T("");
	m_MA6 = _T("");
	m_MA7 = _T("");
	m_MA8 = _T("");
	m_MA9 = _T("");
	m_MD1 = _T("");
	m_MD2 = _T("");
	m_MD3 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMftDlg)
	DDX_Text(pDX, IDC_MA1, m_MA1);
	DDX_Text(pDX, IDC_MA10, m_MA10);
	DDX_Text(pDX, IDC_MA11, m_MA11);
	DDX_Text(pDX, IDC_MA12, m_MA12);
	DDX_Text(pDX, IDC_MA13, m_MA13);
	DDX_Text(pDX, IDC_MA14, m_MA14);
	DDX_Text(pDX, IDC_MA15, m_MA15);
	DDX_Text(pDX, IDC_MA2, m_MA2);
	DDX_Text(pDX, IDC_MA3, m_MA3);
	DDX_Text(pDX, IDC_MA4, m_MA4);
	DDX_Text(pDX, IDC_MA5, m_MA5);
	DDX_Text(pDX, IDC_MA6, m_MA6);
	DDX_Text(pDX, IDC_MA7, m_MA7);
	DDX_Text(pDX, IDC_MA8, m_MA8);
	DDX_Text(pDX, IDC_MA9, m_MA9);
	DDX_Text(pDX, IDC_MD1, m_MD1);
	DDX_Text(pDX, IDC_MD2, m_MD2);
	DDX_Text(pDX, IDC_MD3, m_MD3);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMftDlg, CDialog)
	//{{AFX_MSG_MAP(CMftDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMftDlg message handlers

BOOL CMftDlg::OnInitDialog()
{
	
	// TODO: Add extra initialization here
	m_iStop=1;
	BackgroundThreadComm();
	SetTimer(NULL,100,NULL);
	//m_dData = (double*)malloc(16*sizeof(double));
	//m_pData = (int**)malloc(15*filter*sizeof(int));
	pBufData = (MFTData_t*)malloc(filter*sizeof(MFTData_t));
						
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CMftDlg::BackgroundThreadComm()
{
	DWORD dwThreadId; 
	HANDLE        hCommWatchThread ;
	
	hCommWatchThread=CreateThread( 
								    (LPSECURITY_ATTRIBUTES) NULL,
									0,
   									(LPTHREAD_START_ROUTINE) CommWatchProc,
									NULL,
									0, &dwThreadId );

	ThreadID = dwThreadId;

	               

}

DWORD WINAPI CommWatchProc()
{
	
	OVERLAPPED  os ;
	BYTE buf,cc=0xaa;
	
	CMftDlg* p=(CMftDlg*)AfxGetApp()->m_pMainWnd;
	
	memset( &os, 0, sizeof( OVERLAPPED ) ) ;
	
   // create I/O event used for overlapped read
	//m_cComm.Open(2,19200,8,2,1);
	os.hEvent = CreateEvent( NULL,    // no security
				              TRUE,    // explicit reset req
								FALSE,   // initial event reset
						      NULL ) ; // no name
   
	
	ASSERT(os.hEvent); 
	int i=0,j=1,temp;
		
	
	while(p->m_iStop){
			
		DepthFlag= (DepthFlag+1)%2;
		if(PauseFlag & DepthFlag ) continue;
		
		if(DepthFlag) cc = 0x55;
		else cc=0xaa;
		//m_cComm.WriteComm(&cc,1);
		//WaitCommEvent(m_cComm.m_hComm,&m_cComm.m_ComMask,&os);
		
		if((m_cComm.m_ComMask & EV_RXCHAR) == EV_RXCHAR){
							
				//nReadByte=m_cComm.ReadComm(&buf,68);
			//	if(DepthFlag)	iDepth = CheckFrameDepth(0xaa,0x55,&buf);
			//	else   CheckFrameMft(0xaa,0x55,&buf,Index);
			//	p->m_pMsg=&buf;
			//	p->Display();
				//ReadyFlag = 1;
				//Index++;
				if(Index == filter) {
					//Index = 0;
					
				}
			 									
		}
		
	}		
	
	// get rid of event handle
	m_cComm.Close();

//	CloseHandle( os.hEvent ) ;
	
	
   // clear information in structure (kind of a "we're done flag")

   return( TRUE ) ;

}

void CMftDlg::Display() 
{
 int low,temp,qq,i,j,nG,nL;
 CString cs;
 int temp2;
 double tt,v1,v2,v3,max,min;
	//if(!ReadyFlag) return;	
DWORD cb=0;	
	double dt;
		cs.Format("%d",iDepth);
		SetDlgItemText(1020,cs); //IDC_MA1 = 1000 ... IDC_MA15 = 1014
	
		for(i=0;i<15;i++){
				
		cs.Format("%x %x   %d", BufData[4*i+1],BufData[4*i+2],pBufData[0].MA[i]);
		SetDlgItemText(1000+i,cs);
		}
					
		cs.Format("%d", pBufData[0].MD[0]);
		SetDlgItemText(1015,cs);
		cs.Format("%.1f", pBufData[0].dPressFreq);
		SetDlgItemText(1017,cs);
		cs.Format("%.1f", pBufData[0].dTempeFreq);
		SetDlgItemText(1016,cs);
		if(!DepthFlag){
			for(i=0;i<68;i++){
				cs.Format("%x", BufTest[i]);
				WriteFile(hFile,cs ,cs.GetLength(),&cb,NULL);
			}
	
			WriteFile(hFile,"\n",1,&cb,NULL);
		}
}

void CMftDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//Display();
	BYTE cc;
	static long nT=0;
	
	if(!ReadyFlag && (nT>2)){ 
		cc = 0x55;
		//m_cComm.WriteComm(&cc,1);
		nT=0;
	}
	nT++;
	CDialog::OnTimer(nIDEvent);
}

void CMftDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(NULL);
	free(m_pData);
	free(m_dData);
	free(pBufData);
	m_cComm.Close();
	
	CDialog::OnClose();
}

