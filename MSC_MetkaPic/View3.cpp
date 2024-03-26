// View3.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "View3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CView3

IMPLEMENT_DYNCREATE(CView3, CView)

CView3::CView3()
{
}

CView3::~CView3()
{
}


BEGIN_MESSAGE_MAP(CView3, CView)
	//{{AFX_MSG_MAP(CView3)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CView3 drawing

/*void CView3::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}*/
void CView3::OnDraw(CDC* pDC)
{
	CMFTMonitorDoc* pDoc = GetDocument();
	// TODO: add draw code here
	ASSERT_VALID(pDoc);
	COLORREF	cr[8]={RGB(0,0,0),RGB(0,0,255),RGB(0,160,0),RGB(0,255,255),
					   RGB(255,0,0),RGB(255,0,255),RGB(255,160,0),RGB(192,0,0)};


	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);
	float fx = cx/1024.0;
	float fy = cy/768.0;


	CTrackListArray*	pTrackList= &pDoc->trackListView2;
	CCurveListArray*	pCurveList= &pDoc->curveListView2;
	CCurveList*		curve;
	CTrackList*		track;
	CPen			hPen;
	hPen.CreatePen(PS_SOLID,2,RGB(50,50,50));
	LOGFONT	logf;
	CFont	*oldFont,hFont;
	memset(&logf,0,sizeof(LOGFONT));
	logf.lfHeight=12*fy;;	
	//logf.lfItalic=true;
	logf.lfWeight=FW_BOLD;
	lstrcpy(logf.lfFaceName,_T("Arial"));
	hFont.CreateFontIndirect(&logf);
	oldFont=pDC->SelectObject(&hFont);
	int i,j;
	int t1=0,t2=0,t3=0,t4=0,t5=0; 
	CString cs;
	for(i=0;i<pCurveList->GetSize();i++)	{
		curve=pCurveList->GetAt(i);
	//	if(curve->bActive == false) continue;
		track=pTrackList->GetAt(curve->nTrackIdx);
		pDC->SelectObject(&hPen);
		pDC->MoveTo(track->fLeftEdge*pDoc->rectView3.Width(),pDoc->rectView3.top);
		pDC->LineTo(track->fLeftEdge*pDoc->rectView3.Width(),pDoc->rectView3.bottom);
		pDC->MoveTo(track->fRightEdge*pDoc->rectView3.Width(),pDoc->rectView3.top);
		pDC->LineTo(track->fRightEdge*pDoc->rectView3.Width(),pDoc->rectView3.bottom);

	}

	for(i=0;i<pCurveList->GetSize();i++)	{
		curve=pCurveList->GetAt(i);
		if((curve->nTrackIdx == 0) && (curve->bActive == true))
			t1++;
		if((curve->nTrackIdx == 2) && (curve->bActive == true))
			t2++;
		if((curve->nTrackIdx == 3) && (curve->bActive == true))
			t3++;
		if((curve->nTrackIdx == 4) && (curve->bActive == true))
			t4++;
		if((curve->nTrackIdx == 5) && (curve->bActive == true))
			t5++;
	}
	int i1=0,i2=0,i3=0,i4=0,i5=0;
	for(i=0;i<pCurveList->GetSize();i++)	{
		curve=pCurveList->GetAt(i);
		if(curve->bActive == false) continue;
		track=pTrackList->GetAt(curve->nTrackIdx);
		curve->Pen.CreatePen(curve->nStyle,curve->nWidth,cr[curve->nColor]);
		pDC->SelectObject(&curve->Pen);
		pDC->SetTextAlign(TA_BOTTOM|TA_CENTER);
		pDC->SetBkMode(TRANSPARENT);
		
		if(curve->nTrackIdx == 0){
			pDC->MoveTo(track->fLeftEdge*pDoc->rectView3.Width()+2,(i1+1)*pDoc->rectView3.Height()/(t1+0.5));
			pDC->LineTo(track->fRightEdge*pDoc->rectView3.Width()-2,(i1+1)*pDoc->rectView3.Height()/(t1+0.5));
			pDC->SetTextColor(cr[curve->nColor]);
			pDC->TextOut((track->fRightEdge-track->fLeftEdge)*pDoc->rectView3.Width()/2+track->fLeftEdge*pDoc->rectView3.Width(),
						(i1+1)*pDoc->rectView3.Height()/(t1+0.5),curve->szMnemonic);
			cs.Format("%.1f",curve->fLeftScale);
			pDC->TextOut((track->fLeftEdge + cs.GetLength()*0.004)*pDoc->rectView3.Width(),
						(i1+1)*pDoc->rectView3.Height()/(t1+0.5),cs);
			cs.Format("%.1f",curve->fRightScale);
			pDC->TextOut((track->fRightEdge - cs.GetLength()*0.004)*pDoc->rectView3.Width(),
						(i1+1)*pDoc->rectView3.Height()/(t1+0.5),cs);
			i1++;
		}
		if(curve->nTrackIdx == 2){
			pDC->MoveTo(track->fLeftEdge*pDoc->rectView3.Width()+2,(i2+1)*pDoc->rectView3.Height()/(t2+0.5));
			pDC->LineTo(track->fRightEdge*pDoc->rectView3.Width()-2,(i2+1)*pDoc->rectView3.Height()/(t2+0.5));
			pDC->SetTextColor(cr[curve->nColor]);
			pDC->TextOut((track->fRightEdge-track->fLeftEdge)*pDoc->rectView3.Width()/2+track->fLeftEdge*pDoc->rectView3.Width(),
				(i2+1)*pDoc->rectView3.Height()/(t2+0.5),curve->szMnemonic);
			cs.Format("%.1f",curve->fLeftScale);
			pDC->TextOut((track->fLeftEdge + cs.GetLength()*0.004)*pDoc->rectView3.Width(),
						(i2+1)*pDoc->rectView3.Height()/(t2+0.5),cs);
			cs.Format("%.1f",curve->fRightScale);
			pDC->TextOut((track->fRightEdge - cs.GetLength()*0.004)*pDoc->rectView3.Width(),
						(i2+1)*pDoc->rectView3.Height()/(t2+0.5),cs);
			i2++;
		}
		if(curve->nTrackIdx == 3){
			pDC->MoveTo(track->fLeftEdge*pDoc->rectView3.Width()+2,(i3+1)*pDoc->rectView3.Height()/(t3+0.5));
			pDC->LineTo(track->fRightEdge*pDoc->rectView3.Width()-2,(i3+1)*pDoc->rectView3.Height()/(t3+0.5));
			pDC->SetTextColor(cr[curve->nColor]);
			pDC->TextOut((track->fRightEdge-track->fLeftEdge)*pDoc->rectView3.Width()/2+track->fLeftEdge*pDoc->rectView3.Width(),
				(i3+1)*pDoc->rectView3.Height()/(t3+0.5),curve->szMnemonic);
			cs.Format("%.1f",curve->fLeftScale);
			pDC->TextOut((track->fLeftEdge + cs.GetLength()*0.004)*pDoc->rectView3.Width(),
						(i3+1)*pDoc->rectView3.Height()/(t3+0.5),cs);
			cs.Format("%.1f",curve->fRightScale);
			pDC->TextOut((track->fRightEdge - cs.GetLength()*0.004)*pDoc->rectView3.Width(),
						(i3+1)*pDoc->rectView3.Height()/(t3+0.5),cs);
			i3++;
		}
		if(curve->nTrackIdx == 4){
			pDC->MoveTo(track->fLeftEdge*pDoc->rectView3.Width()+2,(i4+1)*pDoc->rectView3.Height()/(t4+0.5));
			pDC->LineTo(track->fRightEdge*pDoc->rectView3.Width()-2,(i4+1)*pDoc->rectView3.Height()/(t4+0.5));
			pDC->SetTextColor(cr[curve->nColor]);
			pDC->TextOut((track->fRightEdge-track->fLeftEdge)*pDoc->rectView3.Width()/2.0 + track->fLeftEdge*pDoc->rectView3.Width(),
				(i4+1)*pDoc->rectView3.Height()/(t4+0.5),curve->szMnemonic);
			cs.Format("%.1f",curve->fLeftScale);
			pDC->TextOut((track->fLeftEdge + cs.GetLength()*0.004)*pDoc->rectView3.Width(),
						(i4+1)*pDoc->rectView3.Height()/(t4+0.5),cs);
			cs.Format("%.1f",curve->fRightScale);
			pDC->TextOut((track->fRightEdge - cs.GetLength()*0.004)*pDoc->rectView3.Width(),
						(i4+1)*pDoc->rectView3.Height()/(t4+0.5),cs);
			i4++;
		}
		if(curve->nTrackIdx == 5){
			pDC->MoveTo(track->fLeftEdge*pDoc->rectView3.Width()+2,(i5+1)*pDoc->rectView3.Height()/(t5+0.5));
			pDC->LineTo(track->fRightEdge*pDoc->rectView3.Width()-2,(i5+1)*pDoc->rectView3.Height()/(t5+0.5));
			pDC->SetTextColor(cr[curve->nColor]);
			pDC->TextOut((track->fRightEdge-track->fLeftEdge)*pDoc->rectView3.Width()/2+track->fLeftEdge*pDoc->rectView3.Width(),
				(i5+1)*pDoc->rectView3.Height()/(t5+0.5),curve->szMnemonic);
			cs.Format("%.1f",curve->fLeftScale);
			pDC->TextOut((track->fLeftEdge + cs.GetLength()*0.004)*pDoc->rectView3.Width(),
						(i5+1)*pDoc->rectView3.Height()/(t5+0.5),cs);
			cs.Format("%.1f",curve->fRightScale);
			pDC->TextOut((track->fRightEdge - cs.GetLength()*0.004)*pDoc->rectView3.Width(),
						(i5+1)*pDoc->rectView3.Height()/(t5+0.5),cs);
			i5++;
		}
		curve->Pen.DeleteObject();
		
	}
	hPen.DeleteObject();
	ReleaseDC(pDC);
}
/////////////////////////////////////////////////////////////////////////////
// CView3 diagnostics

#ifdef _DEBUG
void CView3::AssertValid() const
{
	CView::AssertValid();
}

void CView3::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CView3 message handlers

void CView3::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CString		str;

	str.Format("View3 x=%d y=%d",cx,cy);

	CMFTMonitorDoc*	pDoc=GetDocument();
	if(pDoc!=NULL)
	{
		pDoc->rectView3.left=0;
		pDoc->rectView3.top=0;
		pDoc->rectView3.right=cx;
		pDoc->rectView3.bottom=cy;

		//if(nDirection==DIRECTION_DOWN)
		{
			float	ftemp;
			pDoc->rectPartView3.left=0;
			pDoc->rectPartView3.right=cx;
			pDoc->rectPartView3.bottom=cy;
			ftemp=2.0*cy/5.0;
			pDoc->rectPartView3.top=int(ftemp);
		}
	}
	//MessageBox(str);
}

CMFTMonitorDoc* CView3::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFTMonitorDoc)));
	return (CMFTMonitorDoc*)m_pDocument;	
}
