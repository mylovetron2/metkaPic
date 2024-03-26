// TrackAttr.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "TrackAttr.h"
#include "Presentation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackAttr property page

IMPLEMENT_DYNCREATE(CTrackAttr, CPropertyPage)

CTrackAttr::CTrackAttr() : CPropertyPage(CTrackAttr::IDD)
{
	//{{AFX_DATA_INIT(CTrackAttr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	bFlag=false;
}

CTrackAttr::~CTrackAttr()
{
}

void CTrackAttr::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrackAttr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
	//DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CTrackAttr, CPropertyPage)
	//{{AFX_MSG_MAP(CTrackAttr)
	ON_BN_CLICKED(IDC_NEWTRACK, OnNewtrack)
	ON_BN_CLICKED(IDC_DELETETRACK, OnDeletetrack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackAttr message handlers
void CTrackAttr::CGridEx::FillListItems(int nCol, LPARAM cltList)
{
	CListBox*	pList = (CListBox*)cltList;
	BOOL bFlag=((CTrackAttr*)GetParent())->bFlag;
	if (nCol == 2)
	{
			pList->ResetContent();
			pList->AddString("Yes");
			pList->AddString("No");	
	}
	else if (nCol == 3)
	{
		pList->AddString("On");
		pList->AddString("Off");	
	}
	else if (nCol == 4)
	{
		pList->AddString("On");
		pList->AddString("Off");	
	}
	else if (nCol == 7)
	{
		pList->AddString("Linear");
		pList->AddString("Logarit");	
	}
}

BOOL CTrackAttr::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	trackList=&((CPresentation*)GetParent())->trackList;
	curveList=&((CPresentation*)GetParent())->curveList;

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(FALSE);
	m_Grid.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));
	//m_Grid.SetSingleRowSelection(true);
	

	try {
		m_Grid.SetRowCount(trackList->GetSize()+1);
		m_Grid.SetColumnCount(11);
		m_Grid.SetFixedRowCount(1);
		m_Grid.SetFixedColumnCount(1);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	CString		str;
	for(int i=1;i<=trackList->GetSize();i++)
	{
		str.Format("Track %d",i);
		m_Grid.SetItemText(i,0,str);
	}
	m_Grid.SetItemText(0,1,"Mnemonic");
	m_Grid.SetColumnType(1, GVET_EDITBOX);

	m_Grid.SetItemText(0,2,"Depth Track");
	m_Grid.SetColumnType(2, GVET_LISTBOX);

	m_Grid.SetItemText(0,3,"Vert. Lines");
	m_Grid.SetColumnType(3, GVET_LISTBOX);

	m_Grid.SetItemText(0,4,"Horz. Lines");
	m_Grid.SetColumnType(4, GVET_LISTBOX);

	m_Grid.SetItemText(0,5,"Left(%)");
	m_Grid.SetColumnType(5, GVET_EDITBOX);

	m_Grid.SetItemText(0,6,"Right(%)");
	m_Grid.SetColumnType(6, GVET_EDITBOX);

	m_Grid.SetItemText(0,7,"Type   ");
	m_Grid.SetColumnType(7, GVET_LISTBOX);

	m_Grid.SetItemText(0,8,"Div. Num.");
	m_Grid.SetColumnType(8, GVET_EDITBOX);

	m_Grid.SetItemText(0,9,"Decades. Num");
	m_Grid.SetColumnType(9, GVET_EDITBOX);

	m_Grid.SetItemText(0,10,"Start Cyc.");
	m_Grid.SetColumnType(10, GVET_EDITBOX);
	

	CTrackList*		track;
	//Display Info
	for(i=0;i<trackList->GetSize();i++)
	{
		track=trackList->GetAt(i);

		m_Grid.SetItemText(i+1,1,track->szMnemonic);
		if(track->bDepthTrack)
			m_Grid.SetItemText(i+1,2,"Yes");
		else
			m_Grid.SetItemText(i+1,2,"No");

		if(track->bVertOn)
			m_Grid.SetItemText(i+1,3,"On");
		else
			m_Grid.SetItemText(i+1,3,"Off");

		if(track->bHorzOn)
			m_Grid.SetItemText(i+1,4,"On");
		else
			m_Grid.SetItemText(i+1,4,"Off");

		str.Format("%.0f",track->fLeftEdge*100);
		m_Grid.SetItemText(i+1,5,str);
		str.Format("%.0f",track->fRightEdge*100);
		m_Grid.SetItemText(i+1,6,str);

		if(track->nVType==LINEAR)
			m_Grid.SetItemText(i+1,7,"Linear");
		else
			m_Grid.SetItemText(i+1,7,"Logarit");

		str.Format("%d",track->nDivisionNum);
		m_Grid.SetItemText(i+1,8,str);
		str.Format("%d",track->nDecadesNum);
		m_Grid.SetItemText(i+1,9,str);
		str.Format("%d",track->nSCycle);
		m_Grid.SetItemText(i+1,10,str);

		//m_Grid.SetItemBkColour(i+1,2,RGB(0,0,255));
		//m_Grid.SetItemFgColour(i+1,2, RGB(255,255,255));
	}


	m_Grid.AutoSize();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTrackAttr::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
    {
        if(pMsg->wParam == VK_RETURN
            || pMsg->wParam == VK_ESCAPE )
        {
            ::TranslateMessage(pMsg);
            ::DispatchMessage(pMsg);
            return TRUE;                    // DO NOT process further
        }
    }
	return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL CTrackAttr::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	//MessageBox("Track Kill Active");
	
	CString		str;
	CTrackList*		track;

	for(int i=0;i<trackList->GetSize();i++)
	{
		track=trackList->GetAt(i);

		str=m_Grid.GetItemText(i+1,1);
		strcpy(track->szMnemonic,str);

		str=m_Grid.GetItemText(i+1,2);
		if(str=="No")
			track->bDepthTrack=false;
		else
			track->bDepthTrack=true;

		str=m_Grid.GetItemText(i+1,3);
		if(str=="On")
			track->bVertOn=true;
		else
			track->bVertOn=false;

		str=m_Grid.GetItemText(i+1,4);
		if(str=="On")
			track->bHorzOn=true;
		else
			track->bHorzOn=false;

		str=m_Grid.GetItemText(i+1,5);
		track->fLeftEdge=atof(str)/100.0;
		str=m_Grid.GetItemText(i+1,6);
		track->fRightEdge=atof(str)/100.0;

		str=m_Grid.GetItemText(i+1,7);
		if(str=="Linear")
			track->nVType=LINEAR;
		else
			track->nVType=LOGARIT;

		str=m_Grid.GetItemText(i+1,8);
		track->nDivisionNum=atoi(str);

		str=m_Grid.GetItemText(i+1,9);
		track->nDecadesNum=atoi(str);

		str=m_Grid.GetItemText(i+1,10);
		track->nSCycle=atoi(str);
	}
	//return true;
	return CPropertyPage::OnKillActive();
}


void CTrackAttr::OnNewtrack() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetRowCount(m_Grid.GetRowCount()+1);
	CTrackList*	track;
	track=new CTrackList();

	//memset(track,0,sizeof(CTrackList));
	strcpy(track->szTitle1,"");
	strcpy(track->szTitle2,"");
	strcpy(track->szMnemonic,"New Track");
	track->fLeftEdge=1.0;
	track->fRightEdge=1.0;
	track->bDepthTrack=false;
	track->bHorzOn=TRUE;
	track->fHFreq=2;
	track->bVertOn=TRUE;
	track->nVType=LINEAR;
	track->nDivisionNum=10;
	track->nDecadesNum=4;
	track->nSCycle=1;
	trackList->Add(track);

	int i=m_Grid.GetRowCount()-2;
	CString	str;

	str.Format("Track %d",m_Grid.GetRowCount()-1);
	m_Grid.SetItemText(i+1,0,str);
	m_Grid.SetItemText(i+1,1,track->szMnemonic);
	if(track->bDepthTrack)
		m_Grid.SetItemText(i+1,2,"Yes");
	else
		m_Grid.SetItemText(i+1,2,"No");

	if(track->bVertOn)
		m_Grid.SetItemText(i+1,3,"On");
	else
		m_Grid.SetItemText(i+1,3,"Off");

	if(track->bHorzOn)
		m_Grid.SetItemText(i+1,4,"On");
	else
		m_Grid.SetItemText(i+1,4,"Off");

	str.Format("%.0f",track->fLeftEdge*100);
	m_Grid.SetItemText(i+1,5,str);
	str.Format("%.0f",track->fRightEdge*100);
	m_Grid.SetItemText(i+1,6,str);

	if(track->nVType==LINEAR)
		m_Grid.SetItemText(i+1,7,"Linear");
	else
		m_Grid.SetItemText(i+1,7,"Logarit");

	str.Format("%d",track->nDivisionNum);
	m_Grid.SetItemText(i+1,8,str);
	str.Format("%d",track->nDecadesNum);
	m_Grid.SetItemText(i+1,9,str);
	str.Format("%d",track->nSCycle);
	m_Grid.SetItemText(i+1,10,str);
}

void CTrackAttr::OnDeletetrack() 
{
	// TODO: Add your control notification handler code here
	int nRow=m_Grid.GetFocusCell().row;
	CString		str;

	//str.Format("%d",nRow);
	//MessageBox(str);

	CTrackList*		track;
	if (nRow >= 0)
    {
		track=trackList->operator[] (nRow-1);
		delete track;
		trackList->RemoveAt(nRow-1);
	    m_Grid.DeleteRow(nRow);	
	    m_Grid.Invalidate();
    }
	else
		return;
	//delete trackList->GetAt(nRow-1);
	//trackList->RemoveAt(nRow-1);

	CCurveList*		curve;
	int				nIdx=nRow-1;

	for(int i=0;i<curveList->GetSize();i++)
	{
		curve=curveList->GetAt(i);
		if(curve->nTrackIdx==nRow-1)
		{
			curve->nTrackIdx=0;
			curve->bActive=false;
		}
		else if(curve->nTrackIdx>nRow-1)
		{
			curve->nTrackIdx--;
		}
	}
}
