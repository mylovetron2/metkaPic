// MyToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "MyToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyToolBar

CMyToolBar::CMyToolBar()
{
}

CMyToolBar::~CMyToolBar()
{
}

IMPLEMENT_DYNAMIC(CMyToolBar, CToolBar)
BEGIN_MESSAGE_MAP(CMyToolBar, CToolBar)
	//{{AFX_MSG_MAP(CMyToolBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyToolBar message handlers


//#define DEFAULT_ANIM_STYLE WS_CHILD | WS_VISIBLE | ACS_CENTER |WS_BORDER

int CMyToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//m_wndAnim.Create(DEFAULT_ANIM_STYLE, CRect(0,0,50,50), this, m_nIDAnim);
	m_Static.Create("My Bitmap",WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_CENTERIMAGE,
		CRect(0,0,0,0), this);
	return 0;
}



IMPLEMENT_DYNCREATE(CTBAnim, CAnimateCtrl)
BEGIN_MESSAGE_MAP(CTBAnim, CAnimateCtrl)
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////
// Normally, the animation control returns HTTRANSPARENT, which disallows
// mouse clicks; need to return HTCLIENT to get WM_LBUTTONDOWN, etc.
//
UINT CTBAnim::OnNcHitTest(CPoint point)
{
	return HTCLIENT;
}

//////////////////
// User clicked: launch the hyperlink
// If string is empty, load it from resource.
// 
void CTBAnim::OnLButtonDown(UINT nFlags, CPoint point)
{
	//if (m_link.IsEmpty())
		//m_link.LoadString(GetDlgCtrlID());
	//m_link.Navigate();
}

void CMyToolBar::OnSize(UINT nType, int cx, int cy) 
{
	CToolBar::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CToolBar::OnSize(nType, cx, cy);
	CRect rc(cx-m_nWidthAnim, 0, cx, cy);	// right edge
	//m_wndAnim.MoveWindow(&rc);					// do it
	m_Static.MoveWindow(&rc);
	//m_Static.SetBitmap( ::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_LOGO1)) );
	//m_Static.ShowWindow(SW_SHOW);
	//AfxMessageBox("On Size");
}
