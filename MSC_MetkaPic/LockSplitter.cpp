// LockSplitter.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "LockSplitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLockSplitter

CLockSplitter::CLockSplitter()
{
	m_bLocked=true;
}

CLockSplitter::~CLockSplitter()
{
}


BEGIN_MESSAGE_MAP(CLockSplitter, CSplitterWnd)
	//{{AFX_MSG_MAP(CLockSplitter)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLockSplitter message handlers

void CLockSplitter::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bLocked)
		CSplitterWnd::OnLButtonDown(nFlags, point);
}

void CLockSplitter::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bLocked)
		CSplitterWnd::OnMouseMove(nFlags, point);
	else
		CWnd::OnMouseMove(nFlags, point);
}

BOOL CLockSplitter::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bLocked)
		return CSplitterWnd::OnSetCursor(pWnd, nHitTest, message);
	else
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
