// MFTMonitorView.cpp : implementation of the CMFTMonitorView class
//

#include "stdafx.h"
#include "MFTMonitor.h"

#include "MFTMonitorDoc.h"
#include "MFTMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFTMonitorView

IMPLEMENT_DYNCREATE(CMFTMonitorView, CView)

BEGIN_MESSAGE_MAP(CMFTMonitorView, CView)
	//{{AFX_MSG_MAP(CMFTMonitorView)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFTMonitorView construction/destruction

CMFTMonitorView::CMFTMonitorView()
{
	// TODO: add construction code here

}

CMFTMonitorView::~CMFTMonitorView()
{
}

BOOL CMFTMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMFTMonitorView drawing

void CMFTMonitorView::OnDraw(CDC* pDC)
{
	CMFTMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	DrawGrid(pDC,5,&pDoc->trackListView1,&pDoc->rectView1);
}

/////////////////////////////////////////////////////////////////////////////
// CMFTMonitorView printing

BOOL CMFTMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFTMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFTMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMFTMonitorView diagnostics

#ifdef _DEBUG
void CMFTMonitorView::AssertValid() const
{
	CView::AssertValid();
}

void CMFTMonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFTMonitorDoc* CMFTMonitorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFTMonitorDoc)));
	return (CMFTMonitorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFTMonitorView message handlers

void CMFTMonitorView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CString		str;

	str.Format("View1 x=%d y=%d",cx,cy);
	CMFTMonitorDoc*	pDoc=GetDocument();
	if(pDoc!=NULL)
	{
		pDoc->rectView1.left=0;
		pDoc->rectView1.top=0;
		pDoc->rectView1.right=cx;
		pDoc->rectView1.bottom=cy;

		//if(nDirection==DIRECTION_DOWN)
		{
			float	ftemp;
			pDoc->rectPartView1.left=0;
			pDoc->rectPartView1.right=cx;
			pDoc->rectPartView1.bottom=cy;
			ftemp=2.0*cy/5.0;
			pDoc->rectPartView1.top=int(ftemp);
		}
	}
	
	//MessageBox(str);
}

void CMFTMonitorView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//return;
	CView::OnLButtonDown(nFlags, point);
}

