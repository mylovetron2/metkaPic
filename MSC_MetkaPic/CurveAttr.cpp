// CurveAttr.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "CurveAttr.h"
#include "Presentation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCurveAttr property page
char	*szColor[]={"Black","Blue","Green","Cyan","Red","Magenta","Yellow","Brown"};



char	*szBrush[]={"SOLID","DASH","DOT","DASH DOT"};
char	*szCurveName[]={"MA1","Hydraulic Pres.","Hydraulic Temp.",
						"Sample Pres.","Sample Temp.","Motor Volt.",
						"Pretest POT","Signal REF","Gamma Ray",
						"Gauge REF-","Gauge REF+","MA12",
						"MA13","MA14","MA15",
						"Position","Quartz Pres. Freq.","Quartz Temp. Freq",
						"Quartz Pres.","Quartz Temp.",
						"Ain1","Ain2","Ain3","Ain4",
						"Ain5","Ain6","Ain7","Ain8"};

IMPLEMENT_DYNCREATE(CCurveAttr, CPropertyPage)

CCurveAttr::CCurveAttr() : CPropertyPage(CCurveAttr::IDD)
{
	//{{AFX_DATA_INIT(CCurveAttr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCurveAttr::~CCurveAttr()
{
}

void CCurveAttr::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCurveAttr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCurveAttr, CPropertyPage)
	//{{AFX_MSG_MAP(CCurveAttr)
	ON_BN_CLICKED(IDC_NEWCURVE, OnNewcurve)
	ON_BN_CLICKED(IDC_DELETECURVE, OnDeletecurve)
	ON_BN_CLICKED(IDC_UPDATEMSC, OnUpdatemsc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurveAttr message handlers
void CCurveAttr::CGridEx::FillListItems(int nCol, LPARAM cltList)
{
	CListBox*	pList = (CListBox*)cltList;
	CTrackListArray*	trackList=((CCurveAttr*)GetParent())->trackList;
	if (nCol == 1)
	{
			pList->ResetContent();
			for(int i=0;i<NUMCHANNEL;i++)
			{
				pList->AddString(pInfoCalib[i].szNameChannel);
			}
			
	}
	else if (nCol == 2)
	{
		pList->AddString("Yes");
		pList->AddString("No");	
	}
	else if (nCol == 3)
	{
		CTrackList	*track;
		for(int i=0;i<trackList->GetSize();i++)
		{
			track=trackList->GetAt(i);
			pList->AddString(track->szMnemonic);
		}		
	}
	else if (nCol == 4)
	{
		pList->AddString("Linear");
		pList->AddString("Logarit");	
	}
	else if (nCol == 7)
	{
		pList->AddString("Black");
		pList->AddString("Red");
		pList->AddString("Blue");
		pList->AddString("Green");
		pList->AddString("Magenta");
		pList->AddString("Cyan");	
		pList->AddString("Yellow");
		pList->AddString("Brown");
	}
	else if (nCol == 9)
	{
		pList->AddString("SOLID");
		pList->AddString("DASH");
		pList->AddString("DOT");
		pList->AddString("DASH DOT");
		
	}
}
/*void CCurveAttr::CGridEx::FillListItems(int nCol, LPARAM cltList)
{
	CListBox*	pList = (CListBox*)cltList;
	CTrackListArray*	trackList=((CCurveAttr*)GetParent())->trackList;
	if (nCol == 1)
	{
			pList->ResetContent();
			for(int i=0;i<28;i++)
			{
				pList->AddString(szCurveName[i]);
			}
			
	}
	else if (nCol == 2)
	{
		pList->AddString("Yes");
		pList->AddString("No");	
	}
	else if (nCol == 3)
	{
		CTrackList	*track;
		for(int i=0;i<trackList->GetSize();i++)
		{
			track=trackList->GetAt(i);
			pList->AddString(track->szMnemonic);
		}		
	}
	else if (nCol == 4)
	{
		pList->AddString("Linear");
		pList->AddString("Logarit");	
	}
	else if (nCol == 7)
	{
		pList->AddString("Black");
		pList->AddString("Blue");
		pList->AddString("Green");
		pList->AddString("Cyan");
		pList->AddString("Red");
		pList->AddString("Magenta");
		pList->AddString("Yellow");
		pList->AddString("White");
	}
}*/

BOOL CCurveAttr::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	trackList=&((CPresentation*)GetParent())->trackList;
	curveList=&((CPresentation*)GetParent())->curveList;

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(FALSE);
	m_Grid.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));

	try {
		m_Grid.SetRowCount(curveList->GetSize()+1);
		//m_Grid.SetColumnCount(8);
		m_Grid.SetColumnCount(10);
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
	for(int i=1;i<=curveList->GetSize();i++)
	{
		str.Format("Curve %d",i);
		m_Grid.SetItemText(i,0,str);
	}

	m_Grid.SetItemText(0,1,"M n e m o n i c");
	m_Grid.SetColumnType(1, GVET_LISTBOX);

	m_Grid.SetItemText(0,2,"Display");
	m_Grid.SetColumnType(2, GVET_LISTBOX);

	m_Grid.SetItemText(0,3,"Track  ");
	m_Grid.SetColumnType(3, GVET_LISTBOX);

	m_Grid.SetItemText(0,4,"Type  ");
	m_Grid.SetColumnType(4, GVET_LISTBOX);

	m_Grid.SetItemText(0,5,"Left Scale");
	m_Grid.SetColumnType(5, GVET_EDITBOX);

	m_Grid.SetItemText(0,6,"Right Scale");
	m_Grid.SetColumnType(6, GVET_EDITBOX);

	m_Grid.SetItemText(0,7,"Color");
	m_Grid.SetColumnType(7, GVET_LISTBOX);
	
	m_Grid.SetItemText(0,8,"Pen Width");
	m_Grid.SetColumnType(8, GVET_EDITBOX);

	m_Grid.SetItemText(0,9,"Brush Shape");
	m_Grid.SetColumnType(9, GVET_LISTBOX);

	CCurveList*		curve;
	CTrackList*		track;
	

	for(i=0;i<curveList->GetSize();i++)
	{
		curve=curveList->GetAt(i);
	
		//m_Grid.SetItemText(i+1,1,szCurveName[curve->nIdx]);
		m_Grid.SetItemText(i+1,1,pInfoCalib[curve->nIdx].szNameChannel);

		//if((!mbMSC)&&(i>20)) curve->bActive=0;

		if(curve->bActive)
			m_Grid.SetItemText(i+1,2,"Yes");
		else
			m_Grid.SetItemText(i+1,2,"No");

		if(curve->nTrackIdx>=0 && curve->nTrackIdx<= trackList->GetSize())
		{
			track=trackList->GetAt(curve->nTrackIdx);
			m_Grid.SetItemText(i+1,3,track->szMnemonic);
		}
		
		if(curve->nScale==LINEAR)
			m_Grid.SetItemText(i+1,4,"Linear");
		else
			m_Grid.SetItemText(i+1,4,"Logarit");

		str.Format("%.2f",curve->fLeftScale);
		m_Grid.SetItemText(i+1,5,str);

		str.Format("%.2f",curve->fRightScale);
		m_Grid.SetItemText(i+1,6,str);

		m_Grid.SetItemText(i+1,7,szColor[curve->nColor]);

		str.Format("%d",curve->nWidth);
		m_Grid.SetItemText(i+1,8,str);

		m_Grid.SetItemText(i+1,9,szBrush[curve->nStyle]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCurveAttr::OnNewcurve() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetRowCount(m_Grid.GetRowCount()+1);
	CCurveList*	curve;
	CTrackList*	track;
	curve=new CCurveList();

	//memset(curve,0,sizeof(curve));
	//strcpy(curve->szMnemonic,szCurveName[0]);
	strcpy(curve->szMnemonic,pInfoCalib[0].szNameChannel);
	curve->bActive=true;
	curve->nTrackIdx=0;
	curve->nIdx=0;
	curve->nScale=LINEAR;
	curve->fLeftScale=0;
	curve->fRightScale=10;
	curve->nColor=0;
	curve->nWidth=1;
	curve->nStyle=0;

	curveList->Add(curve);
	int i=m_Grid.GetRowCount()-2;
	CString	str;

	str.Format("Curve %d",m_Grid.GetRowCount()-1);
	m_Grid.SetItemText(i+1,0,str);
	m_Grid.SetItemText(i+1,1,curve->szMnemonic);

	if(curve->bActive)
		m_Grid.SetItemText(i+1,2,"Yes");
	else
		m_Grid.SetItemText(i+1,2,"No");

	if(curve->nTrackIdx>=0 && curve->nTrackIdx<= trackList->GetSize())
	{
		track=trackList->GetAt(curve->nTrackIdx);
		m_Grid.SetItemText(i+1,3,track->szMnemonic);
	}
	
	if(curve->nScale==LINEAR)
		m_Grid.SetItemText(i+1,4,"Linear");
	else
		m_Grid.SetItemText(i+1,4,"Logarit");

	str.Format("%.2f",curve->fLeftScale);
	m_Grid.SetItemText(i+1,5,str);

	str.Format("%.2f",curve->fRightScale);
	m_Grid.SetItemText(i+1,6,str);

	m_Grid.SetItemText(i+1,7,szColor[curve->nColor]);
	str.Format("%d",curve->nWidth);
	m_Grid.SetItemText(i+1,8,str);

	m_Grid.SetItemText(i+1,9,szBrush[curve->nStyle]);
}

void CCurveAttr::OnDeletecurve() 
{
	// TODO: Add your control notification handler code here
	int nRow=m_Grid.GetFocusCell().row;
	if (nRow >= 0)
    {
	    m_Grid.DeleteRow(nRow);	
	    m_Grid.Invalidate();
    }
	delete curveList->GetAt(nRow-1);
	curveList->RemoveAt(nRow-1);
}

BOOL CCurveAttr::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	//MessageBox("Curve Kill Active");

	CString			str;
	CCurveList*		curve;
	CTrackList*		track;
	int j;

	for(int i=0;i<curveList->GetSize();i++)
	{
		curve=curveList->GetAt(i);

		int k = curveList->GetSize();

		str=m_Grid.GetItemText(i+1,1);
		strcpy(curve->szMnemonic,str);
		curve->nIdx=0;
		//for(int j=0;j<28;j++)
		for(j=0;j<NUMCHANNEL;j++)
			//if(!strcmp(curve->szMnemonic,szCurveName[j]))
			if(!strcmp(curve->szMnemonic,pInfoCalib[j].szNameChannel))
			{
				curve->nIdx=j;
				break;
			}

		str=m_Grid.GetItemText(i+1,2);
		if(str=="Yes")
			curve->bActive=true;
		else
			curve->bActive=false;

		curve->nTrackIdx=0;
		str=m_Grid.GetItemText(i+1,3);
		for(j=0;j<trackList->GetSize();j++)
		{
			track=trackList->GetAt(j);
			if(!strcmp(track->szMnemonic,str))
			{
				curve->nTrackIdx=j;
				break;
			}
		}

		str=m_Grid.GetItemText(i+1,4);
		if(str=="Linear")
			curve->nScale=LINEAR;
		else
			curve->nScale=LOGARIT;

		str=m_Grid.GetItemText(i+1,5);
		curve->fLeftScale=atof(str);
		str=m_Grid.GetItemText(i+1,6);
		curve->fRightScale=atof(str);

		str=m_Grid.GetItemText(i+1,7);
		curve->nColor=0;
		for(j=0;j<8;j++)
		{
			if(!strcmp(szColor[j],str))
			{
				curve->nColor=j;
				break;
			}
		}
		str=m_Grid.GetItemText(i+1,8);
		curve->nWidth=atof(str);

		str=m_Grid.GetItemText(i+1,9);
		curve->nStyle=0;
		for(j=0;j<4;j++)
		{
			if(!strcmp(szBrush[j],str))
			{
				curve->nStyle=j;
				break;
			}
		}
	}

	return CPropertyPage::OnKillActive();
}

BOOL CCurveAttr::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CCurveList*		curve;
	CTrackList*		track;
	CString			str;

	for(int i=0;i<curveList->GetSize();i++)
	{
		curve=curveList->GetAt(i);

		//m_Grid.SetItemText(i+1,1,szCurveName[curve->nIdx]);
		m_Grid.SetItemText(i+1,1,pInfoCalib[curve->nIdx].szNameChannel);
		if(curve->bActive)
			m_Grid.SetItemText(i+1,2,"Yes");
		else
			m_Grid.SetItemText(i+1,2,"No");

		if(curve->nTrackIdx>=0 && curve->nTrackIdx<= trackList->GetSize())
		{
			track=trackList->GetAt(curve->nTrackIdx);
			m_Grid.SetItemText(i+1,3,track->szMnemonic);
		}
		
		if(curve->nScale==LINEAR)
			m_Grid.SetItemText(i+1,4,"Linear");
		else
			m_Grid.SetItemText(i+1,4,"Logarit");

		
		
		str.Format("%.2f",curve->fLeftScale);

		m_Grid.SetItemText(i+1,5,str);

		

		str.Format("%.2f",curve->fRightScale);
		m_Grid.SetItemText(i+1,6,str);

		m_Grid.SetItemText(i+1,7,szColor[curve->nColor]);
		str.Format("%d",curve->nWidth);
		m_Grid.SetItemText(i+1,8,str);
		
		m_Grid.SetItemText(i+1,9,szBrush[curve->nStyle]);
	}
	
	return CPropertyPage::OnSetActive();
}

void CCurveAttr::OnUpdatemsc() 
{
	// TODO: Add your control notification handler code here
	CString			str;
	CCurveList*		curve;
	CTrackList*		track;
	int i,j;
	float fleft,fright;

	

	str=m_Grid.GetItemText(22,5);
	fleft=atof(str);
	str=m_Grid.GetItemText(22,6);
	fright=atof(str);

	for(i=0;i<curveList->GetSize();i++)
	{
		curve=curveList->GetAt(i);

		if(!msc60_flag){
			if((i>60)&&(i<81)){
				curve->bActive = false;
				m_Grid.SetItemText(i+1,2,"No");

			}
		}
		else {
			if((i>60)&&(i<81)){
				curve->bActive = true;
				m_Grid.SetItemText(i+1,2,"Yes");

			}
		}
		
		
		if((i>20)&&(i<81)) 
			curve->fLeftScale = fleft + (i-21);
		
		
		if((i==81)||(i==83)) 
			curve->fLeftScale = fleft;

		if(i==82) 
			curve->fLeftScale = fright;
		
		str.Format("%.2f",curve->fLeftScale);
		m_Grid.SetItemText(i+1,5,str);


		
		if((i>20)&&(i<81)) 
			curve->fRightScale = fright + (i-21);

		if((i==81)||(i==83)) 
			curve->fRightScale = fright;

		if(i==82) 
			curve->fRightScale = fleft;

		str.Format("%.2f",curve->fRightScale);
		m_Grid.SetItemText(i+1,6,str);

		
	}
	
	CPropertyPage::OnSetActive();

	for(i=0;i<curveList->GetSize();i++)
	{
		curve=curveList->GetAt(i);

		

		str=m_Grid.GetItemText(i+1,5);
		curve->fLeftScale=atof(str);
		str=m_Grid.GetItemText(i+1,6);
		curve->fRightScale=atof(str);
		

	}
	CPropertyPage::OnKillActive();
	
}
