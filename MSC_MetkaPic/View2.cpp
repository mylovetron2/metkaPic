// View2.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "View2.h"

#include "MFTMonitorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CView2

IMPLEMENT_DYNCREATE(CView2, CView)

CView2::CView2()
{
}

CView2::~CView2()
{
}


BEGIN_MESSAGE_MAP(CView2, CView)
	//{{AFX_MSG_MAP(CView2)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CView2 drawing

void CView2::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	CMFTMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code here
	DrawGrid(pDC,5,&pDoc->trackListView2,&pDoc->rectView2);
}

/////////////////////////////////////////////////////////////////////////////
// CView2 diagnostics

#ifdef _DEBUG
void CView2::AssertValid() const
{
	CView::AssertValid();
}

void CView2::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CView2 message handlers

void CView2::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CString		str;

	str.Format("View2 x=%d y=%d",cx,cy);

	CMFTMonitorDoc*	pDoc=GetDocument();
	if(pDoc!=NULL)
	{
		pDoc->rectView2.left=0;
		pDoc->rectView2.top=0;
		pDoc->rectView2.right=cx;
		pDoc->rectView2.bottom=cy;

		//if(nDirection==DIRECTION_DOWN)
		{
			float	ftemp;
			pDoc->rectPartView2.left=0;
			pDoc->rectPartView2.right=cx;
			pDoc->rectPartView2.bottom=cy;
			ftemp=2.0*cy/5.0;
			pDoc->rectPartView2.top=int(ftemp);
		}
	}
	//MessageBox(str);	
}

CMFTMonitorDoc* CView2::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFTMonitorDoc)));
	return (CMFTMonitorDoc*)m_pDocument;	
}
