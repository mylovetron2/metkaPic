// MFTMonitorDoc.cpp : implementation of the CMFTMonitorDoc class
//

#include "stdafx.h"
#include "MFTMonitor.h"

#include "MFTMonitorDoc.h"
#include "MainFrm.h"
#include "Threads.h"
#include "Presentation.h"
#include "Panel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFTMonitorDoc
CCurveList*		m_pCurve;			
CCurveListArray*		m_pCurveList;
ThreadInfo_t*		m_pThreadInfo;
CString provider[]={"Provider=MSDASQL.1;Extended Properties=",
					"Provider=MSDASQL.1;",	
					"Provider=Microsoft.Jet.OLEDB.4.0;Data Source="
					
};
IMPLEMENT_DYNCREATE(CMFTMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFTMonitorDoc, CDocument)
	//{{AFX_MSG_MAP(CMFTMonitorDoc)
	ON_COMMAND(IDC_STARTLOGGING, OnStartlogging)
	ON_UPDATE_COMMAND_UI(IDC_STARTLOGGING, OnUpdateStartlogging)
	ON_UPDATE_COMMAND_UI(IDC_STOPLOGGING, OnUpdateStoplogging)
	ON_COMMAND(IDC_STOPLOGGING, OnStoplogging)
	ON_COMMAND(ID_SCALE_VIEW2_5METERS, OnScaleView25meters)
	ON_UPDATE_COMMAND_UI(ID_SCALE_VIEW2_5METERS, OnUpdateScaleView25meters)
	ON_COMMAND(ID_SCALE_VIEW2_10METERS, OnScaleView210meters)
	ON_UPDATE_COMMAND_UI(ID_SCALE_VIEW2_10METERS, OnUpdateScaleView210meters)
	ON_COMMAND(ID_SCALE_VIEW2_20METERS, OnScaleView220meters)
	ON_UPDATE_COMMAND_UI(ID_SCALE_VIEW2_20METERS, OnUpdateScaleView220meters)
	ON_COMMAND(ID_SCALE_VIEW2_30METERS, OnScaleView230meters)
	ON_UPDATE_COMMAND_UI(ID_SCALE_VIEW2_30METERS, OnUpdateScaleView230meters)
	ON_COMMAND(ID_SCALE_VIEW2_50METERS, OnScaleView250meters)
	ON_UPDATE_COMMAND_UI(ID_SCALE_VIEW2_50METERS, OnUpdateScaleView250meters)
	ON_COMMAND(ID_SCALE_VIEW2_100METERS, OnScaleView2100meters)
	ON_UPDATE_COMMAND_UI(ID_SCALE_VIEW2_100METERS, OnUpdateScaleView2100meters)
	ON_COMMAND(ID_SCALE_VIEW3_30SECONDS, OnScaleView330seconds)
	ON_UPDATE_COMMAND_UI(ID_SCALE_VIEW3_30SECONDS, OnUpdateScaleView330seconds)
	ON_COMMAND(ID_SCALE_VIEW3_1MINUTE, OnScaleView31minute)
	ON_UPDATE_COMMAND_UI(ID_SCALE_VIEW3_1MINUTE, OnUpdateScaleView31minute)
	ON_COMMAND(ID_SCALE_VIEW3_5MINUTES, OnScaleView35minutes)
	ON_UPDATE_COMMAND_UI(ID_SCALE_VIEW3_5MINUTES, OnUpdateScaleView35minutes)
	ON_COMMAND(ID_SCALE_VIEW3_10MINUTES, OnScaleView310minutes)
	ON_UPDATE_COMMAND_UI(ID_SCALE_VIEW3_10MINUTES, OnUpdateScaleView310minutes)
	ON_COMMAND(ID_SCALE_VIEW3_20MINUTES, OnScaleView320minutes)
	ON_UPDATE_COMMAND_UI(ID_SCALE_VIEW3_20MINUTES, OnUpdateScaleView320minutes)
	ON_COMMAND(ID_SCALE_VIEW3_30MINUTES, OnScaleView330minutes)
	ON_UPDATE_COMMAND_UI(ID_SCALE_VIEW3_30MINUTES, OnUpdateScaleView330minutes)
	ON_COMMAND(IDC_SAVEDATA, OnSavedata)
	ON_COMMAND(IDC_STOPSAVING, OnStopsaving)
	ON_UPDATE_COMMAND_UI(IDC_SAVEDATA, OnUpdateSavedata)
	ON_UPDATE_COMMAND_UI(IDC_STOPSAVING, OnUpdateStopsaving)
	ON_COMMAND(IDC_CHANGEDVPRES, OnChangedvpres)
	ON_COMMAND(IDC_CHANGETIMEVIEWPRES, OnChangetimeviewpres)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TBN_DROPDOWN,AFX_IDW_TOOLBAR,OnDropDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFTMonitorDoc construction/destruction

CMFTMonitorDoc::CMFTMonitorDoc()
{
	// TODO: add one-time construction code here
	pView1Thread=NULL;
	pView2Thread=NULL;
//	pReadDataThread=NULL;
	pWriteDataThread=NULL;

	pThreadInfo1=NULL;
	pThreadInfo2=NULL;
	::CoInitialize(NULL);
	
	memset(&gData,0,sizeof(RawData_t));

	pReadDataThreadInfo=NULL;
	pWriteDataThreadInfo=NULL;

	bIsLogging=false;

	if(ReadMFTIniFile())
		return;

	//-------------------------------------------
	//
	//		VIEW 1: RIGHT UPPER VIEW
	//
	//-------------------------------------------
	//Create track
	CTrackList*		track;

	track=new CTrackList();
	//memset(track,0,sizeof(track));
	strcpy(track->szMnemonic,"T1");
	track->fLeftEdge=0;
	track->fRightEdge=0.30;
	track->bDepthTrack=false;
	track->bHorzOn=TRUE;
	track->fHFreq=2;
	track->bVertOn=TRUE;
	track->nVType=LINEAR;
	track->nDivisionNum=10;
	track->nDecadesNum=4;
	track->nSCycle=1;
	trackListView1.Add(track);

	track=new CTrackList();
	//memset(track,0,sizeof(track));
	strcpy(track->szMnemonic,"DEPTH");
	track->fLeftEdge=0.3;
	track->fRightEdge=0.4;
	track->bDepthTrack=TRUE;
	track->nVType=LINEAR;
	track->bHorzOn=false;
	track->nDivisionNum=0;
	track->nDecadesNum=4;
	track->nSCycle=1;
	trackListView1.Add(track);

	track=new CTrackList();
	//memset(track,0,sizeof(track));
	strcpy(track->szMnemonic,"T2");
	track->fLeftEdge=0.4;
	track->fRightEdge=0.70;
	track->bDepthTrack=false;
	track->bHorzOn=TRUE;
	track->fHFreq=2;
	track->bVertOn=TRUE;
	track->nVType=LINEAR;
	track->nDivisionNum=10;
	track->nDecadesNum=4;
	track->nSCycle=1;
	trackListView1.Add(track);

	track=new CTrackList();
	//memset(track,0,sizeof(track));
	strcpy(track->szMnemonic,"T3");
	track->fLeftEdge=0.7;
	track->bDepthTrack=false;
	track->fRightEdge=1;
	track->bHorzOn=TRUE;
	track->fHFreq=2;
	track->bVertOn=TRUE;
	track->nVType=LOGARIT;
	track->nSCycle=2;
	track->nDecadesNum=4;
	track->nDivisionNum=10;
	trackListView1.Add(track);

	//Create curve
	CCurveList*		curve;

	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve1");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=100;
	curve->nScale=LINEAR;
	curve->nIdx=0;//curve index
	curve->nTrackIdx=0;//track index;
	curve->nColor=0;
	curve->nWidth=3;
	curve->nStyle=PS_SOLID;
	curveListView1.Add(curve);

	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve2");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=100;
	curve->nScale=LINEAR;
	curve->nIdx=1;//curve index
	curve->nTrackIdx=0;//track index;
	curve->nColor=1;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView1.Add(curve);
	
	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve3");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=0.1;
	curve->nScale=LINEAR;
	curve->nIdx=2;//curve index
	curve->nTrackIdx=0;//track index;
	curve->nColor=2;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView1.Add(curve);

	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve4");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=5;
	curve->nScale=LINEAR;
	curve->nIdx=3;//curve index
	curve->nTrackIdx=1;//track index;
	curve->nColor=3;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView1.Add(curve);

	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve5");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=5;
	curve->nScale=LINEAR;
	curve->nIdx=4;//curve index
	curve->nTrackIdx=2;//track index;
	curve->nColor=4;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView1.Add(curve);

	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve6");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=5;
	curve->nScale=LINEAR;
	curve->nIdx=5;//curve index
	curve->nTrackIdx=2;//track index;
	curve->nColor=5;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView1.Add(curve);
	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve7");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=5;
	curve->nScale=LINEAR;
	curve->nIdx=5;//curve index
	curve->nTrackIdx=2;//track index;
	curve->nColor=5;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView1.Add(curve);
	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve8");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=5;
	curve->nScale=LINEAR;
	curve->nIdx=5;//curve index
	curve->nTrackIdx=2;//track index;
	curve->nColor=5;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView1.Add(curve);

	//-------------------------------------------
	//
	//		VIEW 1: RIGHT LOWER VIEW
	//
	//-------------------------------------------
	track=new CTrackList();
	//memset(track,0,sizeof(track));
	strcpy(track->szMnemonic,"T1");
	track->fLeftEdge=0;
	track->fRightEdge=0.30;
	track->bDepthTrack=false;
	track->bHorzOn=TRUE;
	track->fHFreq=2;
	track->bVertOn=TRUE;
	track->nVType=LINEAR;
	track->nDivisionNum=10;
	track->nDecadesNum=4;
	track->nSCycle=1;
	trackListView2.Add(track);

	track=new CTrackList();
	//memset(track,0,sizeof(track));
	strcpy(track->szMnemonic,"DEPTH");
	track->fLeftEdge=0.3;
	track->fRightEdge=0.4;
	track->bDepthTrack=TRUE;
	track->nVType=LINEAR;
	track->bHorzOn=false;
	track->nDivisionNum=0;
	track->nDecadesNum=4;
	track->nSCycle=1;
	trackListView2.Add(track);

	track=new CTrackList();
	//memset(track,0,sizeof(track));
	strcpy(track->szMnemonic,"T2");
	track->fLeftEdge=0.4;
	track->fRightEdge=0.70;
	track->bDepthTrack=false;
	track->bHorzOn=TRUE;
	track->fHFreq=2;
	track->bVertOn=TRUE;
	track->nVType=LINEAR;
	track->nDivisionNum=10;
	track->nDecadesNum=4;
	track->nSCycle=1;
	trackListView2.Add(track);

	track=new CTrackList();
	//memset(track,0,sizeof(track));
	strcpy(track->szMnemonic,"T3");
	track->fLeftEdge=0.7;
	track->bDepthTrack=false;
	track->fRightEdge=1;
	track->bHorzOn=TRUE;
	track->fHFreq=2;
	track->bVertOn=TRUE;
	track->nVType=LOGARIT;
	track->nSCycle=2;
	track->nDecadesNum=4;
	track->nDivisionNum=10;
	trackListView2.Add(track);


	
	//Create curve
	
	

	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve1");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=100;
	curve->nScale=LINEAR;
	curve->nIdx=0;//curve index
	curve->nTrackIdx=0;//track index;
	curve->nColor=0;
	curve->nWidth=3;
	curve->nStyle=PS_SOLID;
	curveListView2.Add(curve);

	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve2");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=100;
	curve->nScale=LINEAR;
	curve->nIdx=1;//curve index
	curve->nTrackIdx=0;//track index;
	curve->nColor=1;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView2.Add(curve);
	
	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve3");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=0.1;
	curve->nScale=LINEAR;
	curve->nIdx=2;//curve index
	curve->nTrackIdx=0;//track index;
	curve->nColor=2;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView2.Add(curve);

	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve4");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=5;
	curve->nScale=LINEAR;
	curve->nIdx=3;//curve index
	curve->nTrackIdx=1;//track index;
	curve->nColor=3;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView2.Add(curve);

	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve5");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=5;
	curve->nScale=LINEAR;
	curve->nIdx=4;//curve index
	curve->nTrackIdx=2;//track index;
	curve->nColor=4;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView2.Add(curve);

	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve6");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=5;
	curve->nScale=LINEAR;
	curve->nIdx=5;//curve index
	curve->nTrackIdx=2;//track index;
	curve->nColor=5;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView2.Add(curve);
	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve7");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=5;
	curve->nScale=LINEAR;
	curve->nIdx=5;//curve index
	curve->nTrackIdx=2;//track index;
	curve->nColor=5;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView2.Add(curve);
	curve=new CCurveList();
	//memset(curve,0,sizeof(curve));
	strcpy(curve->szMnemonic,"Curve8");
	curve->bActive=true;
	curve->fLeftScale=0;
	curve->fRightScale=5;
	curve->nScale=LINEAR;
	curve->nIdx=5;//curve index
	curve->nTrackIdx=2;//track index;
	curve->nColor=5;
	curve->nWidth=5;
	curve->nStyle=PS_SOLID;
	curveListView2.Add(curve);
}

CMFTMonitorDoc::~CMFTMonitorDoc()
{
}

BOOL CMFTMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMFTMonitorDoc serialization

void CMFTMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMFTMonitorDoc diagnostics

#ifdef _DEBUG
void CMFTMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFTMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFTMonitorDoc commands

/*void CMFTMonitorDoc::OnStartlogging() 
{
	// TODO: Add your command handler code here
	//----------------------------------------
	//
	//		Get File Name for the first time
	//
	//-----------------------------------------
	PauseFlag =1;
	RECT			rect;

	gData.fDepth=-999.25;
	
	if(pReadDataThreadInfo!=NULL)
		delete pReadDataThreadInfo;
	pReadDataThreadInfo=new ReadDataThreadInfo_t;

	if(pWriteDataThreadInfo!=NULL)
		delete pWriteDataThreadInfo;
	pWriteDataThreadInfo=new WriteDataThreadInfo_t;//new

	if(pThreadInfo1!=NULL)
		delete pThreadInfo1;
	pThreadInfo1=new ThreadInfo_t;
	m_pThreadInfo = pThreadInfo1;

	if(pThreadInfo2!=NULL)
		delete pThreadInfo2;
	pThreadInfo2=new ThreadInfo_t;
	
	if(nStartNum==0)
	{
		CString		strTxtFN;
		CString		strMdbFN;


		CFileDialog m_filedlg(FALSE,"txt",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Text Files (*.txt)|*.txt||");	
	
		if(m_filedlg.DoModal()==IDOK)
		{
			if(IsFileExist(m_filedlg.GetPathName().GetBuffer(m_filedlg.GetPathName().GetLength())))
			{
				AfxMessageBox("File already existed. Can't overwrite");
				PauseFlag =0;
				return;
			}
			else
				strTxtFN=m_filedlg.GetPathName();
		}
		else { 
			PauseFlag =0;
			return;
		}

		CFileDialog m_filedlg1(FALSE,"mdb",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				  "Access Database Files (*.mdb)|*.mdb||");	
		if(m_filedlg1.DoModal()==IDOK)
		{
			if(IsFileExist(m_filedlg1.GetPathName().GetBuffer(m_filedlg1.GetPathName().GetLength())))
			{
				AfxMessageBox("File already existed. Can't overwrite");
				PauseFlag =0;
				strcpy(CurName,"");
				return;
			}
			else{
				strMdbFN=m_filedlg1.GetPathName();
				strcpy(CurName,(LPCTSTR)strMdbFN);
			}
		}
		else{
			PauseFlag =0;
			return;
		}
		//--------------------------------------------------
		//
		//		Create files for the first times
		//
		//--------------------------------------------------
		if(pFile!=NULL)
			delete pFile;
		pFile=new CFile();	
		pFile->Open(strTxtFN,CFile::modeCreate|CFile::modeWrite);

		
		if(m_rs.IsOpen())	m_rs.Close();
		if(m_pDB.IsOpen())	m_pDB.Close();
		m_pDB.Create(strMdbFN);
	}
	CString			str;

	CDaoTableDef	tableDef(&m_pDB);

			
	str.Format("DataTable%d",nStartNum);
	tableDef.Create(str);



	tableDef.CreateField("Time",dbText,0);
	tableDef.CreateField("Depth",dbSingle,0);
	
	

	
	//------------------------------------------------------
	//
	//		Start Logging
	//
	//-------------------------------------------------------
	bIsLogging=true;
	
	CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	CSplitterWnd*	splitter;
	CWnd*			pWnd;
	CPanel* m_panel=(CPanel*)pMainFrm->m_wndSpMain.GetActivePane(0,0);
	m_panel->m_cPlayInDepth.EnableWindow(false);
	m_panel->m_cPlayInTime.EnableWindow(false);
	
	
	
	
	

	//Create Thread of Right Upper View
	splitter=&pMainFrm->m_wndSpSub;
	pWnd=splitter->GetPane(0,0);
	memset(pThreadInfo1,0,sizeof(ThreadInfo_t));
	pThreadInfo1->pWnd=pWnd;
	pThreadInfo1->pRect=&this->rectView1;
	pThreadInfo1->pRectPart=&this->rectPartView1;
	pThreadInfo1->pTrackList=&this->trackListView1;
	pThreadInfo1->pCurveList=&this->curveListView1;
	
	pView1Thread=AfxBeginThread(View1Thread,pThreadInfo1,THREAD_PRIORITY_LOWEST,0,0);

	CCurveList* curve;
	CCurveListArray* pCurveList = pThreadInfo1->pCurveList;

	
	for(int i=0;i< pCurveList->GetSize();i++){
		curve = pCurveList->GetAt(i);
		tableDef.CreateField(pInfoCalib[curve->nIdx].szNameChannel,dbSingle,0);	

	}
	tableDef.Append();

	CDaoIndexInfo		index;
	CDaoIndexFieldInfo	fld;

	index.m_strName="PrimaryKey";
	index.m_pFieldInfos=&fld;
	index.m_nFields=1;
	index.m_bPrimary=FALSE;
	index.m_bUnique=FALSE;
	index.m_bClustered=FALSE;
	index.m_bIgnoreNulls=FALSE;
	index.m_bRequired=TRUE;
	index.m_bForeign=FALSE;
	index.m_lDistinctCount=5;
			

	fld.m_strName="Time";
	fld.m_bDescending=FALSE;

	tableDef.CreateIndex(index);

	if(m_rs.IsOpen())	m_rs.Close();
	str.Format("SELECT * FROM DATATABLE%d",nStartNum);
	m_rs.Open(dbOpenDynaset,str);


	nStartNum++;

	//Create Thread of Right Lower View
	splitter=&pMainFrm->m_wndSpSub;
	pWnd=splitter->GetPane(1,0);
	memset(pThreadInfo2,0,sizeof(ThreadInfo_t));
	pThreadInfo2->pWnd=pWnd;
	pThreadInfo2->pRect=&this->rectView2;
	pThreadInfo2->pRectPart=&this->rectPartView2;
	pThreadInfo2->pTrackList=&this->trackListView2;
	pThreadInfo2->pCurveList=&this->curveListView2;
	
	pView2Thread=AfxBeginThread(View2Thread,pThreadInfo2,THREAD_PRIORITY_LOWEST,0,0);
	PauseFlag =0;
}*/
void CMFTMonitorDoc::DumpError(_com_error &e)
{
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	CString str;
	str.Format("\tCode = %08lx",e.Error());
	str += " Msg: ";
	str += e.ErrorMessage();
	str += " Source: ";
	str += bstrSource;
	str += " Discription: ";
	str += bstrDescription;
	AfxMessageBox(str);
}
BOOL CMFTMonitorDoc::CreateDatabase(CString ConString, CString DbName)
{
	CString cs;
	ADOX::_CatalogPtr Cat = NULL;
	cs = ConString + DbName;
	
	try
	{
		Cat.CreateInstance(__uuidof(ADOX::Catalog));
		Cat->Create(_bstr_t(cs));		
			
	}
	catch(_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}
		
	Cat.Release();
	return TRUE;
}
BOOL CMFTMonitorDoc::CreateTable(CString ConString, CString TableName,FieldInfo_t *fInfo,int nFields)
{
	
	CString cs;
	ADOX::_CatalogPtr Cat=NULL;
	ADOX::_TablePtr Tab=NULL;
	int i;				
	try
	{	
		Cat.CreateInstance(__uuidof(ADOX::Catalog));
		Tab.CreateInstance(__uuidof(ADOX::Table));

		Cat->PutActiveConnection(_bstr_t(ConString));
		Tab->PutName(_bstr_t(TableName));
		
	
		for(i=0; i<nFields; i++){
				
		Tab->Columns->Append(_bstr_t(fInfo[i].FieldName),
			      ADOX::DataTypeEnum(fInfo[i].FieldType),fInfo[i].Size);
		
		}
	
		Cat->Tables->Append(_variant_t((IDispatch *)Tab));
				

    }   
	catch(_com_error &e)
	{
		DumpError(e);
 		return FALSE;
	}
		
	return TRUE;
}
void CMFTMonitorDoc::OnStartlogging() 
{
	// TODO: Add your command handler code here
	//----------------------------------------
	//
	//		Get File Name for the first time
	//
	//-----------------------------------------
		PauseFlag =1;
	static int iStart=0;
	CString			str,sz;
	int i;
	if(!iStart)
	{
		CString		strTxtFN;
		CString		strMdbFN;


		CFileDialog m_filedlg(FALSE,"txt",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Text Files (*.txt)|*.txt||");	
	
		if(m_filedlg.DoModal()==IDOK){
		
			if(IsFileExist(m_filedlg.GetPathName().GetBuffer(m_filedlg.GetPathName().GetLength())))
			{
				AfxMessageBox("File already existed. Can't overwrite");
				PauseFlag =0;
				return;
			}
			else
				strTxtFN=m_filedlg.GetPathName();
		}
		else { 
			PauseFlag =0;
			return;
		}
		

		CFileDialog m_filedlg1(FALSE,"mdb",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				  "Access Database Files (*.mdb)|*.mdb||");	
		if(m_filedlg1.DoModal()==IDOK){
		
			if(IsFileExist(m_filedlg1.GetPathName().GetBuffer(m_filedlg1.GetPathName().GetLength())))
			{
				AfxMessageBox("File already existed. Can't overwrite");
				PauseFlag =0;
				strcpy(CurName,"");
				return;
			}
			else{
				strMdbFN=m_filedlg1.GetPathName();
				strcpy(CurName,(LPCTSTR)strMdbFN);
			}
		}
		else
			return;
		//--------------------------------------------------
		//
		//		Create files for the first times
		//
		//--------------------------------------------------
		if(pFile!=NULL)
			delete pFile;
		pFile=new CFile();	
		pFile->Open(strTxtFN,CFile::modeCreate|CFile::modeWrite);
		
		CreateDatabase(provider[2],strMdbFN);
		strcpy(CurName,(LPCTSTR)strMdbFN);
		
      
			
		

		
		
		FieldInfo_t fInfo[NUMCHANNEL];//new
		fInfo[0].FieldName = "TIME";
		fInfo[0].FieldType = adWChar;
		//fInfo[0].FieldType = adDBTime ;
		fInfo[0].Size = 0;
		fInfo[1].FieldName = "DEPTH";
		fInfo[1].FieldType = adSingle;
		fInfo[1].Size = sizeof(adSingle);
		for(i=2;i<curveListView2.GetSize()+2;i++){
			fInfo[i].FieldName = curveListView2.GetAt(i-2)->szMnemonic;
			fInfo[i].FieldType = adSingle;
			fInfo[i].Size = sizeof(adSingle);
			
		}
		sz.Format("DataTable%d",nStartNum);
		CreateTable(provider[2]+strMdbFN, sz,fInfo,curveListView2.GetSize()+2);
		
		
		CString sql(_T("Select * From ["+sz+"]"));	

		m_rs.CreateInstance(__uuidof(Recordset));
		m_rs->Open(_bstr_t(sql),
				 _bstr_t(provider[2]+strMdbFN),
				 adOpenDynamic,
				 adLockOptimistic,
				 adCmdUnknown);
		
//		if(m_rs.IsOpen())	m_rs.Close();
//		if(m_pDB.IsOpen())	m_pDB.Close();
//		m_pDB.Create(strMdbFN);

/*		CDaoTableDef	tableLog(&m_pDB);
		tableLog.Create("TableLog");

		tableLog.CreateField("Time",dbText,0);
		tableLog.CreateField("Description",dbText,0);
		tableLog.Append();

		CDaoIndexInfo	Lindex;
		CDaoIndexFieldInfo	Lfld;

		Lindex.m_strName="PrimaryKey";
		Lindex.m_pFieldInfos=&Lfld;
		Lindex.m_nFields=1;
		Lindex.m_bPrimary=FALSE;
		Lindex.m_bUnique=FALSE;
		Lindex.m_bClustered=FALSE;
		Lindex.m_bIgnoreNulls=FALSE;
		Lindex.m_bRequired=TRUE;
		Lindex.m_bForeign=FALSE;
		Lindex.m_lDistinctCount=20;
			

		Lfld.m_strName="Time";
		Lfld.m_bDescending=FALSE;
		tableLog.CreateIndex(Lindex);
		if(m_Lrs.IsOpen()) m_Lrs.Close();
		m_Lrs.Open(dbOpenDynaset,"SELECT * FROM TABLELOG");*/
		
	}
	
	//sz.Format("Current table:  DataTable%d",nStartNum);

	if(nOldStartNum != nStartNum){

		//FieldInfo_t fInfo[25];//new
		FieldInfo_t fInfo[NUMCHANNEL];//new
		fInfo[0].FieldName = "TIME";
		fInfo[0].FieldType = adWChar;
		//fInfo[0].FieldType = adDBTime ;
		fInfo[0].Size = 0;
		fInfo[1].FieldName = "DEPTH";
		//fInfo[1].FieldType = adWChar;
		//fInfo[1].Size = 0;
		fInfo[1].FieldType = adSingle;
		fInfo[1].Size = sizeof(adSingle);
		for(i=2;i<curveListView2.GetSize()+2;i++){
			fInfo[i].FieldName = curveListView2.GetAt(i-2)->szMnemonic;
			fInfo[i].FieldType = adSingle;
			fInfo[i].Size = sizeof(adSingle);
			
		}
		sz.Format("DataTable%d",nStartNum);
		CreateTable(provider[2]+CurName, sz,fInfo,curveListView2.GetSize()+2);
		CString sql(_T("Select * From ["+sz+"]"));	

		m_rs.CreateInstance(__uuidof(Recordset));
		m_rs->Open(_bstr_t(sql),
				 _bstr_t(provider[2]+CurName),
				 adOpenDynamic,
				 adLockOptimistic,
				 adCmdUnknown);


	/*	CDaoTableDef	tableDef(&m_pDB);
	
		str.Format("DataTable%d",nStartNum);
		tableDef.Create(str);
		tableDef.CreateField("Time",dbText,0);
		tableDef.CreateField("Depth",dbSingle,0);
	
	
		for(int i=0;i<curveListView2.GetSize();i++)
			tableDef.CreateField(curveListView2.GetAt(i)->szMnemonic,dbSingle,0);

		tableDef.Append();
		CDaoIndexInfo		index;
		CDaoIndexFieldInfo	fld;

		index.m_strName="PrimaryKey";
		index.m_pFieldInfos=&fld;
		index.m_nFields=1;
		index.m_bPrimary=FALSE;
		index.m_bUnique=FALSE;
		index.m_bClustered=FALSE;
		index.m_bIgnoreNulls=FALSE;
		index.m_bRequired=TRUE;
		index.m_bForeign=FALSE;
		index.m_lDistinctCount=20;
			

		fld.m_strName="Time";
		fld.m_bDescending=FALSE;
		tableDef.CreateIndex(index);
		if(m_rs.IsOpen()) m_rs.Close();
			str.Format("SELECT * FROM DATATABLE%d",nStartNum);
		m_rs.Open(dbOpenDynaset,str);*/

	}	
	
	
	//------------------------------------------------------
	//
	//		Start Logging
	//
	//-------------------------------------------------------
	
	bIsLogging=true;
	
	
	CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	pMainFrm->m_wndStatusBar.SetPaneText(pMainFrm->m_wndStatusBar.CommandToIndex(ID_MOREINFO),sz);
	CSplitterWnd*	splitter;
	CWnd*			pWnd;
	
	RECT			rect;

	//gData.fDepth=-999.25;
	
	gData.fDepth=0;
	
	if(pThreadInfo1!=NULL)
		delete pThreadInfo1;
	pThreadInfo1=new ThreadInfo_t;

	if(pThreadInfo2!=NULL)
		delete pThreadInfo2;
	pThreadInfo2=new ThreadInfo_t;

	//if(m_pThreadInfo !=NULL)
		//delete m_pThreadInfo;
	//m_pThreadInfo=new ThreadInfo_t;

	//memset(m_pThreadInfo,0,sizeof(ThreadInfo_t));

	m_pThreadInfo = pThreadInfo1;//new
	
	//Create Thread of Right Upper View
	splitter=&pMainFrm->m_wndSpSub;
	pWnd=splitter->GetPane(0,0);
	memset(pThreadInfo1,0,sizeof(ThreadInfo_t));
	pThreadInfo1->pWnd=pWnd;
	pThreadInfo1->pRect=&this->rectView1;
	pThreadInfo1->pRectPart=&this->rectPartView1;
	pThreadInfo1->pTrackList=&this->trackListView1;
	pThreadInfo1->pCurveList=&this->curveListView1;

	//Create Thread of Right Lower View
	splitter=&pMainFrm->m_wndSpSub;
	pWnd=splitter->GetPane(1,0);
	memset(pThreadInfo2,0,sizeof(ThreadInfo_t));
	pThreadInfo2->pWnd=pWnd;
	pThreadInfo2->pRect=&this->rectView2;
	pThreadInfo2->pRectPart=&this->rectPartView2;
	pThreadInfo2->pTrackList=&this->trackListView2;
	pThreadInfo2->pCurveList=&this->curveListView2;

	//m_pThreadInfo->pWnd=pWnd;
	//m_pThreadInfo->pRect=&this->rectView2;
	//m_pThreadInfo->pRectPart=&this->rectPartView2;
	//m_pThreadInfo->pTrackList=&this->trackListView2;
	//m_pThreadInfo->pCurveList=&this->curveListView2;
	
	pView1Thread=AfxBeginThread(View1Thread,pThreadInfo1,THREAD_PRIORITY_LOWEST,0,0);
	pView2Thread=AfxBeginThread(View2Thread,pThreadInfo2,THREAD_PRIORITY_LOWEST,0,0);
	pView1Thread->m_bAutoDelete = true;
	pView2Thread->m_bAutoDelete = true;
	iStart++;
	nOldStartNum = nStartNum;

}


/*void CMFTMonitorDoc::OnStartlogging() 
{
	// TODO: Add your command handler code here
	//----------------------------------------
	//
	//		Get File Name for the first time
	//
	//-----------------------------------------
	
	PauseFlag =1;
	static int iStart=0;
	CString			str,sz;
	if(!iStart)
	{
		CString		strTxtFN;
		CString		strMdbFN;


		CFileDialog m_filedlg(FALSE,"txt",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Text Files (*.txt)|*.txt||");	
	
		if(m_filedlg.DoModal()==IDOK){
		
			if(IsFileExist(m_filedlg.GetPathName().GetBuffer(m_filedlg.GetPathName().GetLength())))
			{
				AfxMessageBox("File already existed. Can't overwrite");
				PauseFlag =0;
				return;
			}
			else
				strTxtFN=m_filedlg.GetPathName();
		}
		else { 
			PauseFlag =0;
			return;
		}
		

		CFileDialog m_filedlg1(FALSE,"mdb",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				  "Access Database Files (*.mdb)|*.mdb||");	
		if(m_filedlg1.DoModal()==IDOK){
		
			if(IsFileExist(m_filedlg1.GetPathName().GetBuffer(m_filedlg1.GetPathName().GetLength())))
			{
				AfxMessageBox("File already existed. Can't overwrite");
				PauseFlag =0;
				strcpy(CurName,"");
				return;
			}
			else{
				strMdbFN=m_filedlg1.GetPathName();
				strcpy(CurName,(LPCTSTR)strMdbFN);
			}
		}
		else
			return;
		//--------------------------------------------------
		//
		//		Create files for the first times
		//
		//--------------------------------------------------
		if(pFile!=NULL)
			delete pFile;
		pFile=new CFile();	
		pFile->Open(strTxtFN,CFile::modeCreate|CFile::modeWrite);

		
		if(m_rs.IsOpen())	m_rs.Close();
		if(m_pDB.IsOpen())	m_pDB.Close();
		m_pDB.Create(strMdbFN);

		CDaoTableDef	tableLog(&m_pDB);
		tableLog.Create("TableLog");

		tableLog.CreateField("Time",dbText,0);
		tableLog.CreateField("Description",dbText,0);
		tableLog.Append();

		CDaoIndexInfo	Lindex;
		CDaoIndexFieldInfo	Lfld;

		Lindex.m_strName="PrimaryKey";
		Lindex.m_pFieldInfos=&Lfld;
		Lindex.m_nFields=1;
		Lindex.m_bPrimary=FALSE;
		Lindex.m_bUnique=FALSE;
		Lindex.m_bClustered=FALSE;
		Lindex.m_bIgnoreNulls=FALSE;
		Lindex.m_bRequired=TRUE;
		Lindex.m_bForeign=FALSE;
		Lindex.m_lDistinctCount=20;
			

		Lfld.m_strName="Time";
		Lfld.m_bDescending=FALSE;
		tableLog.CreateIndex(Lindex);
		if(m_Lrs.IsOpen()) m_Lrs.Close();
		m_Lrs.Open(dbOpenDynaset,"SELECT * FROM TABLELOG");
		
	}
	
	sz.Format("Current table:  DataTable%d",nStartNum);

	if(nOldStartNum != nStartNum){
		CDaoTableDef	tableDef(&m_pDB);
	
		str.Format("DataTable%d",nStartNum);
		tableDef.Create(str);
		tableDef.CreateField("Time",dbText,0);
		tableDef.CreateField("Depth",dbSingle,0);
	
	
		for(int i=0;i<curveListView2.GetSize();i++)
			tableDef.CreateField(curveListView2.GetAt(i)->szMnemonic,dbSingle,0);

		tableDef.Append();
		CDaoIndexInfo		index;
		CDaoIndexFieldInfo	fld;

		index.m_strName="PrimaryKey";
		index.m_pFieldInfos=&fld;
		index.m_nFields=1;
		index.m_bPrimary=FALSE;
		index.m_bUnique=FALSE;
		index.m_bClustered=FALSE;
		index.m_bIgnoreNulls=FALSE;
		index.m_bRequired=TRUE;
		index.m_bForeign=FALSE;
		index.m_lDistinctCount=20;
			

		fld.m_strName="Time";
		fld.m_bDescending=FALSE;
		tableDef.CreateIndex(index);
		if(m_rs.IsOpen()) m_rs.Close();
			str.Format("SELECT * FROM DATATABLE%d",nStartNum);
		m_rs.Open(dbOpenDynaset,str);

	}	
	
	
	//------------------------------------------------------
	//
	//		Start Logging
	//
	//-------------------------------------------------------
	
	bIsLogging=true;
	
	
	CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	pMainFrm->m_wndStatusBar.SetPaneText(pMainFrm->m_wndStatusBar.CommandToIndex(ID_MOREINFO),sz);
	CSplitterWnd*	splitter;
	CWnd*			pWnd;
	
	RECT			rect;

	gData.fDepth=-999.25;
	
	
	
	if(pThreadInfo1!=NULL)
		delete pThreadInfo1;
	pThreadInfo1=new ThreadInfo_t;

	if(pThreadInfo2!=NULL)
		delete pThreadInfo2;
	pThreadInfo2=new ThreadInfo_t;

	m_pThreadInfo = pThreadInfo1;
	
	//Create Thread of Right Upper View
	splitter=&pMainFrm->m_wndSpSub;
	pWnd=splitter->GetPane(0,0);
	memset(pThreadInfo1,0,sizeof(ThreadInfo_t));
	pThreadInfo1->pWnd=pWnd;
	pThreadInfo1->pRect=&this->rectView1;
	pThreadInfo1->pRectPart=&this->rectPartView1;
	pThreadInfo1->pTrackList=&this->trackListView1;
	pThreadInfo1->pCurveList=&this->curveListView1;

	//Create Thread of Right Lower View
	splitter=&pMainFrm->m_wndSpSub;
	pWnd=splitter->GetPane(1,0);
	memset(pThreadInfo2,0,sizeof(ThreadInfo_t));
	pThreadInfo2->pWnd=pWnd;
	pThreadInfo2->pRect=&this->rectView2;
	pThreadInfo2->pRectPart=&this->rectPartView2;
	pThreadInfo2->pTrackList=&this->trackListView2;
	pThreadInfo2->pCurveList=&this->curveListView2;

	
	
	pView1Thread=AfxBeginThread(View1Thread,pThreadInfo1,THREAD_PRIORITY_LOWEST,0,0);
	pView2Thread=AfxBeginThread(View2Thread,pThreadInfo2,THREAD_PRIORITY_LOWEST,0,0);
	pView1Thread->m_bAutoDelete = true;
	pView2Thread->m_bAutoDelete = true;
	iStart++;
	nOldStartNum = nStartNum;

}*/


void CMFTMonitorDoc::OnUpdateStartlogging(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	if(bIsLogging)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}

void CMFTMonitorDoc::OnStoplogging() 
{
	// TODO: Add your command handler code here
	CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CPanel* m_panel=(CPanel*)pMainFrm->m_wndSpMain.GetActivePane(0,0);

	bIsLogging=false;
	::WaitForSingleObject(pView1Thread,INFINITE);
	::WaitForSingleObject(pView2Thread,INFINITE);
	
	bIsSaving=false;
	::WaitForSingleObject(pWriteDataThread,INFINITE);
	m_panel->m_cPlayInDepth.EnableWindow(true);
	m_panel->m_cPlayInTime.EnableWindow(true);
	m_panel->m_cSetDepths.EnableWindow(true);
	m_panel->m_cEncoder.EnableWindow(true);
	m_panel->m_cEncoderR.EnableWindow(true);
	m_panel->m_cClrDepth.EnableWindow(true);
	m_panel->m_cSetDepthE.EnableWindow(true);

}

void CMFTMonitorDoc::OnUpdateStoplogging(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(!bIsLogging)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}




void CMFTMonitorDoc::OnDropDown(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	// this function handles the dropdown menus from the toolbar
	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNotifyStruct;
	CRect rect;

	// translate the current toolbar item rectangle into screen coordinates
	// so that we'll know where to pop up the menu
	CMainFrame*	pMainFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	pMainFrame->m_wndToolBar.GetToolBarCtrl().GetRect(pNMToolBar->iItem, &rect);
	rect.top = rect.bottom;
	::ClientToScreen(pNMToolBar->hdr.hwndFrom, &rect.TopLeft());
	if(pNMToolBar->iItem == IDC_DIRECTION)
	{
		CMenu menu;
		CMenu* pPopup;

		// the font popup is stored in a resource
		menu.LoadMenu(IDR_DIRECTION_POPUP);
		pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
	else if(pNMToolBar->iItem == IDC_SCALE)
	{
		CMenu menu;
		CMenu* pPopup;

		// the font popup is stored in a resource
		menu.LoadMenu(IDR_SCALE_POPUP);
		pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
}

/*void CMFTMonitorDoc::StopLogging()
{
	OnStoplogging();
	//OnStopsaving();
}*/
void CMFTMonitorDoc::StopLogging()
{
	if(pView1Thread) {
		bIsLogging=false;
		mwait(300);
//		::WaitForSingleObject(pView1Thread,INFINITE);
//		pView1Thread->m_bAutoDelete = true;
//		::WaitForSingleObject(pView2Thread,INFINITE);
//		pView2Thread->m_bAutoDelete = true;
	}
	if(pWriteDataThread){
		bIsSaving=false;
		mwait(500);
	//	::WaitForSingleObject(pWriteDataThread,INFINITE);
	//	pWriteDataThread->m_bAutoDelete = true;
	}
	
	
}
//------------------------------------------------------
//
//			HANDLE DIRECTION MENU
//
//------------------------------------------------------
/*void CMFTMonitorDoc::OnDirection() 
{
	// TODO: Add your command handler code here
	
}*/

//--------------------------------------------------------
//
//			HANDLE LEFT VIEW MENU
//
//--------------------------------------------------------
/*void CMFTMonitorDoc::OnScale() 
{
	// TODO: Add your command handler code here
	
}*/

//-------------------------------------------------------------
//
//			HANDLE RIGHT UPPER VIEW MENU
//
//-------------------------------------------------------------

void CMFTMonitorDoc::OnScaleView25meters() 
{
	// TODO: Add your command handler code here
	nRightUpperViewScale=SCALE_5M;
	WriteMFTIniFile();
}

void CMFTMonitorDoc::OnUpdateScaleView25meters(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(nRightUpperViewScale==SCALE_5M)
		pCmdUI->SetCheck(1);
}

void CMFTMonitorDoc::OnScaleView210meters() 
{
	// TODO: Add your command handler code here
	nRightUpperViewScale=SCALE_10M;	
	WriteMFTIniFile();
}

void CMFTMonitorDoc::OnUpdateScaleView210meters(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(nRightUpperViewScale==SCALE_10M)
		pCmdUI->SetCheck(1);
}

void CMFTMonitorDoc::OnScaleView220meters() 
{
	// TODO: Add your command handler code here
	nRightUpperViewScale=SCALE_20M;	
	WriteMFTIniFile();
}

void CMFTMonitorDoc::OnUpdateScaleView220meters(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(nRightUpperViewScale==SCALE_20M)
		pCmdUI->SetCheck(1);
}

void CMFTMonitorDoc::OnScaleView230meters() 
{
	// TODO: Add your command handler code here
	nRightUpperViewScale=SCALE_30M;	
	WriteMFTIniFile();
}

void CMFTMonitorDoc::OnUpdateScaleView230meters(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(nRightUpperViewScale==SCALE_30M)
		pCmdUI->SetCheck(1);
}

void CMFTMonitorDoc::OnScaleView250meters() 
{
	// TODO: Add your command handler code here
	nRightUpperViewScale=SCALE_50M;	
	WriteMFTIniFile();
}

void CMFTMonitorDoc::OnUpdateScaleView250meters(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(nRightUpperViewScale==SCALE_50M)
		pCmdUI->SetCheck(1);
}

void CMFTMonitorDoc::OnScaleView2100meters() 
{
	// TODO: Add your command handler code here
	nRightUpperViewScale=SCALE_100M;	
	WriteMFTIniFile();
}

void CMFTMonitorDoc::OnUpdateScaleView2100meters(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(nRightUpperViewScale==SCALE_100M)
		pCmdUI->SetCheck(1);	
}
//------------------------------------------------------
//
//			HANDLE RIGHT LOWER VIEW MENU
//
//------------------------------------------------------

void CMFTMonitorDoc::OnScaleView330seconds() 
{
	// TODO: Add your command handler code here
	nRightLowerViewScale=SCALE_30SEC;	
	WriteMFTIniFile();
}

void CMFTMonitorDoc::OnUpdateScaleView330seconds(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(nRightLowerViewScale==SCALE_30SEC)
		pCmdUI->SetCheck(1);
}

void CMFTMonitorDoc::OnScaleView31minute() 
{
	// TODO: Add your command handler code here
	nRightLowerViewScale=SCALE_1MIN;	
	WriteMFTIniFile();
}

void CMFTMonitorDoc::OnUpdateScaleView31minute(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(nRightLowerViewScale==SCALE_1MIN)
		pCmdUI->SetCheck(1);
}

void CMFTMonitorDoc::OnScaleView35minutes() 
{
	// TODO: Add your command handler code here
	nRightLowerViewScale=SCALE_5MIN;
	WriteMFTIniFile();
}

void CMFTMonitorDoc::OnUpdateScaleView35minutes(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(nRightLowerViewScale==SCALE_5MIN)
		pCmdUI->SetCheck(1);
}

void CMFTMonitorDoc::OnScaleView310minutes() 
{
	// TODO: Add your command handler code here
	nRightLowerViewScale=SCALE_10MIN;
	WriteMFTIniFile();
}

void CMFTMonitorDoc::OnUpdateScaleView310minutes(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(nRightLowerViewScale==SCALE_10MIN)
		pCmdUI->SetCheck(1);
}

void CMFTMonitorDoc::OnScaleView320minutes() 
{
	// TODO: Add your command handler code here
	nRightLowerViewScale=SCALE_20MIN;
	WriteMFTIniFile();
}

void CMFTMonitorDoc::OnUpdateScaleView320minutes(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(nRightLowerViewScale==SCALE_20MIN)
		pCmdUI->SetCheck(1);
}

void CMFTMonitorDoc::OnScaleView330minutes() 
{
	// TODO: Add your command handler code here
	nRightLowerViewScale=SCALE_30MIN;
	WriteMFTIniFile();
}

void CMFTMonitorDoc::OnUpdateScaleView330minutes(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(nRightLowerViewScale==SCALE_30MIN)
		pCmdUI->SetCheck(1);
}

void CMFTMonitorDoc::OnSavedata() 
{
	// TODO: Add your command handler code here
	
	bIsSaving=true;
	CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CPanel* m_panel=(CPanel*)pMainFrm->m_wndSpMain.GetActivePane(0,0);

	/*if(pWriteDataThreadInfo!=NULL)
		delete pWriteDataThreadInfo;
	pWriteDataThreadInfo=new WriteDataThreadInfo_t;*/

	//if(pWriteDataThreadInfo == NULL)
	//	pWriteDataThreadInfo=new WriteDataThreadInfo_t;
	
	//pWriteDataThreadInfo->pWnd=pMainFrm;
//	if(pMainFrm==NULL)
//	{
//		AfxMessageBox("OK");
//	}

	m_panel->m_cPlayInDepth.EnableWindow(false);
	m_panel->m_cPlayInTime.EnableWindow(false);
	m_panel->m_cSetDepths.EnableWindow(false);
	m_panel->m_cEncoder.EnableWindow(false);
	m_panel->m_cEncoderR.EnableWindow(false);
	m_panel->m_cClrDepth.EnableWindow(false);
	m_panel->m_cSetDepthE.EnableWindow(false);

	//if(pWriteDataThread != NULL){
//		OnStartlogging();
//	}
	pWriteDataThread=AfxBeginThread(WriteDataThread,NULL,THREAD_PRIORITY_LOWEST,0,0);
	pWriteDataThread->m_bAutoDelete = true;
	//pWriteDataThread=AfxBeginThread(WriteDataThread,NULL,THREAD_PRIORITY_LOWEST,0,0);
}

void CMFTMonitorDoc::OnStopsaving() 
{
	// TODO: Add your command handler code here
	bIsSaving=false;
	CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CPanel* m_panel=(CPanel*)pMainFrm->m_wndSpMain.GetActivePane(0,0);
	
	
	m_panel->m_cPlayInDepth.EnableWindow(true);
	m_panel->m_cPlayInTime.EnableWindow(true);
	m_panel->m_cSetDepths.EnableWindow(true);
	m_panel->m_cEncoder.EnableWindow(true);
	m_panel->m_cEncoderR.EnableWindow(true);
	m_panel->m_cClrDepth.EnableWindow(true);
	m_panel->m_cSetDepthE.EnableWindow(true);

	//strMdbFN=m_filedlg1.GetPathName();
	//WriteMFTIniFile();
	OnStoplogging();
	//::WaitForSingleObject(pWriteDataThread,500);
}

void CMFTMonitorDoc::OnUpdateSavedata(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(bIsLogging)
	{
		if(bIsSaving==false)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMFTMonitorDoc::OnUpdateStopsaving(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(bIsLogging)
	{
		if(bIsSaving)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);	
}

void CMFTMonitorDoc::OnChangedvpres() 
{
	// TODO: Add your command handler code here
	CPresentation	editPres("Change Depth View Presentation");	
	int i;
	CTrackList*		track;
	for(i=0;i<trackListView1.GetSize();i++)
	{
		track=trackListView1.GetAt(i);
		editPres.trackList.Add(new CTrackList(track));
	}
	CCurveList*		curve;
	for(i=0;i<this->curveListView1.GetSize();i++)
	{
		curve=curveListView1.GetAt(i);
		editPres.curveList.Add(new CCurveList(curve));
	}

	if(editPres.DoModal()==IDOK)
	{
		int		iOldTrackSize=trackListView1.GetSize();
		int		iNewTrackSize=editPres.trackList.GetSize();
		//copy old track List
		CTrackList* track;
		CTrackList* track1;
		for(i=0;i<trackListView1.GetSize();i++)
		{
			if(i>=iNewTrackSize) break;
			track=trackListView1.GetAt(i);
			track1=editPres.trackList.GetAt(i);
			
			strcpy(track->szMnemonic,track1->szMnemonic);
			track->bDepthTrack=track1->bDepthTrack;
			track->bHorzOn=track1->bHorzOn;
			track->bVertOn=track1->bVertOn;
			track->fHFreq=track1->fHFreq;
			track->fLeftEdge=track1->fLeftEdge;
			track->fRightEdge=track1->fRightEdge;
			track->nDecadesNum=track1->nDecadesNum;
			track->nDivisionNum=track1->nDivisionNum;
			track->nSCycle=track1->nSCycle;
			track->nVType=track1->nVType;
		}
		//Add new track if size of new track greater than old track size
		for(i=0;i<iNewTrackSize-iOldTrackSize;i++)
		{
			trackListView1.Add(new CTrackList(editPres.trackList.GetAt(i+iOldTrackSize)));
		}


		CCurveList*	curve;
		CCurveList*	curve1;
		int		iOldCurveSize=curveListView1.GetSize();
		int		iNewCurveSize=editPres.curveList.GetSize();
		
		for(i=0;i<curveListView1.GetSize();i++)
		{
			if(i>=iNewCurveSize)	break;
			curve=curveListView1.GetAt(i);
			curve1=editPres.curveList.GetAt(i);
			strcpy(curve->szMnemonic,curve1->szMnemonic);
			curve->bActive=curve1->bActive;
			curve->nTrackIdx=curve1->nTrackIdx;
			curve->nScale=curve1->nScale;
			curve->fLeftScale=curve1->fLeftScale;
			curve->fRightScale=curve1->fRightScale;
			curve->nColor=curve1->nColor;
			curve->nIdx=curve1->nIdx;
			curve->nWidth=curve1->nWidth;
			curve->nStyle=curve1->nStyle;
		}
		//Add new curve if size of new curve greater than old curve size
		for(i=0;i<iNewCurveSize-iOldCurveSize;i++)
			curveListView1.Add(new CCurveList(editPres.curveList.GetAt(i+iOldCurveSize)));
		//delete if size of new curve less than old curve size
		for(i=iOldCurveSize-iNewCurveSize-1;i>=0;i--)
		{	
			curve=curveListView1.GetAt(i+iNewCurveSize);
			curveListView1.RemoveAt(i+iNewCurveSize);
			delete curve;
		}
		
		for(i=iOldTrackSize-iNewTrackSize-1;i>=0;i--)
		{	
			track=trackListView1.GetAt(i+iNewTrackSize);
			trackListView1.RemoveAt(i+iNewTrackSize);
			delete track;
		}


		for(i=editPres.trackList.GetSize()-1;i>=0;i--)
		{
			delete editPres.trackList.GetAt(i);
			editPres.trackList.RemoveAt(i);
		}
		for(i=editPres.curveList.GetSize()-1;i>=0;i--)
		{
			delete editPres.curveList.GetAt(i);
			editPres.curveList.RemoveAt(i);
		}
		

		CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	
		CSplitterWnd*	splitter;
		CWnd*			pWnd;
	
		splitter=&pMainFrm->m_wndSpSub;
		pWnd=splitter->GetPane(0,0);
		pWnd->Invalidate();

		WriteMFTIniFile();
	}
}

void CMFTMonitorDoc::OnChangetimeviewpres() 
{
	// TODO: Add your command handler code here
	CPresentation	editPres("Change Time View Presentation");	
	int i;
	CTrackList*		track;
	for(i=0;i<trackListView2.GetSize();i++)
	{
		track=trackListView2.GetAt(i);
		editPres.trackList.Add(new CTrackList(track));
	}
	CCurveList*		curve;
	for(i=0;i<this->curveListView2.GetSize();i++)
	{
		curve=curveListView2.GetAt(i);
		editPres.curveList.Add(new CCurveList(curve));
	}

	if(editPres.DoModal()==IDOK)
	{
		int		iOldTrackSize=trackListView2.GetSize();
		int		iNewTrackSize=editPres.trackList.GetSize(),i;
		//copy old track List
		CTrackList* track;
		CTrackList* track1;
		for(i=0;i<trackListView2.GetSize();i++)
		{
			if(i>=iNewTrackSize) break;
			track=trackListView2.GetAt(i);
			track1=editPres.trackList.GetAt(i);
			
			strcpy(track->szMnemonic,track1->szMnemonic);
			track->bDepthTrack=track1->bDepthTrack;
			track->bHorzOn=track1->bHorzOn;
			track->bVertOn=track1->bVertOn;
			track->fHFreq=track1->fHFreq;
			track->fLeftEdge=track1->fLeftEdge;
			track->fRightEdge=track1->fRightEdge;
			track->nDecadesNum=track1->nDecadesNum;
			track->nDivisionNum=track1->nDivisionNum;
			track->nSCycle=track1->nSCycle;
			track->nVType=track1->nVType;
		}
		//Add new track if size of new track greater than old track size
		for(i=0;i<iNewTrackSize-iOldTrackSize;i++)
		{
			trackListView2.Add(new CTrackList(editPres.trackList.GetAt(i+iOldTrackSize)));
		}


		CCurveList*	curve;
		CCurveList*	curve1;
		int		iOldCurveSize=curveListView2.GetSize();
		int		iNewCurveSize=editPres.curveList.GetSize();
		
		for(i=0;i<curveListView2.GetSize();i++)
		{
			if(i>=iNewCurveSize)	break;
			curve=curveListView2.GetAt(i);
			curve1=editPres.curveList.GetAt(i);
			strcpy(curve->szMnemonic,curve1->szMnemonic);
			curve->bActive=curve1->bActive;
			curve->nTrackIdx=curve1->nTrackIdx;
			curve->nScale=curve1->nScale;
			curve->fLeftScale=curve1->fLeftScale;
			curve->fRightScale=curve1->fRightScale;
			curve->nColor=curve1->nColor;
			curve->nIdx=curve1->nIdx;	
			curve->nWidth=curve1->nWidth;
			curve->nStyle=curve1->nStyle;
		}
		//Add new curve if size of new curve greater than old curve size
		for(i=0;i<iNewCurveSize-iOldCurveSize;i++)
			curveListView2.Add(new CCurveList(editPres.curveList.GetAt(i+iOldCurveSize)));
		//delete if size of new curve less than old curve size
		for(i=iOldCurveSize-iNewCurveSize-1;i>=0;i--)
		{	
			curve=curveListView2.GetAt(i+iNewCurveSize);
			curveListView2.RemoveAt(i+iNewCurveSize);
			delete curve;
		}
		
		for(i=iOldTrackSize-iNewTrackSize-1;i>=0;i--)
		{	
			track=trackListView2.GetAt(i+iNewTrackSize);
			trackListView2.RemoveAt(i+iNewTrackSize);
			delete track;
		}


		for(i=editPres.trackList.GetSize()-1;i>=0;i--)
		{			
			track=editPres.trackList.GetAt(i);
			editPres.trackList.RemoveAt(i);
			delete track;
		}
		

		for(i=editPres.curveList.GetSize()-1;i>=0;i--)
		{
			delete editPres.curveList.GetAt(i);
			editPres.curveList.RemoveAt(i);
		}

		// them vao

		int iOld = curveListView1.GetSize();
		int iOld_T = trackListView1.GetSize();

		if(trackListView2.GetSize() > trackListView1.GetSize()){
			for(i=0;i<trackListView2.GetSize()-iOld_T;i++)
			trackListView2.Add(new CTrackList(trackListView2.GetAt(i+iOld_T)));
		}

		if( curveListView2.GetSize() > curveListView1.GetSize()){
			for(i=0;i<curveListView2.GetSize() - iOld;i++)
			curveListView1.Add(new CCurveList(curveListView2.GetAt(i+iOld)));
		}

		for(i=0;i<curveListView2.GetSize();i++)
		{
			
			curve1=curveListView2.GetAt(i);
			curve=curveListView1.GetAt(i);
			strcpy(curve->szMnemonic,curve1->szMnemonic);
			curve->bActive=curve1->bActive;
			curve->nTrackIdx=curve1->nTrackIdx;
			curve->nScale=curve1->nScale;
			curve->fLeftScale=curve1->fLeftScale;
			curve->fRightScale=curve1->fRightScale;
			curve->nColor=curve1->nColor;
			curve->nIdx=curve1->nIdx;
			curve->nWidth=curve1->nWidth;
			curve->nStyle=curve1->nStyle;
			
		}
		

		CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	
		CSplitterWnd*	splitter;
		CWnd*			pWnd;
	
		splitter=&pMainFrm->m_wndSpSub;
		pWnd=splitter->GetPane(1,0);
		pWnd->Invalidate();

		pWnd=splitter->GetPane(2,0);
		pWnd->Invalidate();

		WriteMFTIniFile();
	}	
}

/*int CMFTMonitorDoc::ReadMFTIniFile()
{
	char		lpFileName[150];
	char		lpAppName[80];
	char		lpKeyName[80];
	char		lpString[80];
	char		lpDefault[80];


	char		lpDrive[10];
	char		lpDir[150];

	::GetModuleFileName(NULL,lpFileName,80);
	_splitpath(lpFileName,lpDrive,lpDir,NULL,NULL);	
	strcpy(lpFileName,lpDrive);
	strcat(lpFileName,lpDir);
	strcat(lpFileName,"mft.ini");
	strcpy(lpDefault,"0");

	if(!IsFileExist(lpFileName))
		return 0;

	CTrackList*		track;
	CCurveList*		curve;
	int				nTrackNum1;
	int				nTrackNum2;
	int				nCurveNum1;
	int				nCurveNum2;

	strcpy(lpAppName,"Files");
	strcpy(lpKeyName,"Last_File");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,CurName,150,lpFileName);
	
	strcpy(lpAppName,"General");
	strcpy(lpKeyName,"TrackNum1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nTrackNum1=atoi(lpString);
	strcpy(lpKeyName,"CurveNum1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nCurveNum1=atoi(lpString);

	strcpy(lpKeyName,"TrackNum2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nTrackNum2=atoi(lpString);
	strcpy(lpKeyName,"CurveNum2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nCurveNum2=atoi(lpString);

	strcpy(lpKeyName,"Scale1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nRightUpperViewScale=atoi(lpString);

	strcpy(lpKeyName,"Scale2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nRightLowerViewScale=atoi(lpString);

	strcpy(lpKeyName,"GaugeHydraulicMax1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeHydraulicMax1=atoi(lpString);
	strcpy(lpKeyName,"GaugeHydraulicMin1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeHydraulicMin1=atoi(lpString);
	strcpy(lpKeyName,"GaugeHydraulicMax2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeHydraulicMax2=atoi(lpString);
	strcpy(lpKeyName,"GaugeHydraulicMin2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeHydraulicMin2=atoi(lpString);
	strcpy(lpKeyName,"GaugeHydraulicLogarit");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	bGaugeHydraulicLogarit=BOOL(atoi(lpString));


	strcpy(lpKeyName,"GaugeSampleMax1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeSampleMax1=atoi(lpString);
	strcpy(lpKeyName,"GaugeSampleMin1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeSampleMin1=atoi(lpString);
	strcpy(lpKeyName,"GaugeSampleMax2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeSampleMax2=atoi(lpString);
	strcpy(lpKeyName,"GaugeSampleMin2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeSampleMin2=atoi(lpString);
	strcpy(lpKeyName,"GaugeSampleLogarit");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	bGaugeSampleLogarit=BOOL(atoi(lpString));

	
	strcpy(lpKeyName,"QuartzPretestMax1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nQuartzPretestMax1=atoi(lpString);
	strcpy(lpKeyName,"QuartzPretestMin1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nQuartzPretestMin1=atoi(lpString);
	strcpy(lpKeyName,"QuartzPretestMax2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nQuartzPretestMax2=atoi(lpString);
	strcpy(lpKeyName,"QuartzPretestMin2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nQuartzPretestMin2=atoi(lpString);
	strcpy(lpKeyName,"QuartzPretestLogarit");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	bQuartzPretestLogarit=BOOL(atoi(lpString));

	strcpy(lpKeyName,"PositionMax");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nPositionMax=atoi(lpString);
	strcpy(lpKeyName,"PositionMin");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nPositionMin=atoi(lpString);

	strcpy(lpKeyName,"EncoderConst");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	fEncoderConst=atof(lpString);

	for(int i=0;i<nTrackNum1;i++)
	{
		track=new CTrackList();
		//memset(track,0,sizeof(track));

		sprintf(lpAppName,"Track1_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		strcpy(track->szMnemonic,lpString);

		strcpy(lpKeyName,"DepthTrack");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->bDepthTrack=BOOL(atoi(lpString));

		strcpy(lpKeyName,"VertLine");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->bVertOn=BOOL(atoi(lpString));

		strcpy(lpKeyName,"HorzLine");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->bHorzOn=BOOL(atoi(lpString));

		strcpy(lpKeyName,"Left");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->fLeftEdge=atof(lpString);

		strcpy(lpKeyName,"Right");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->fRightEdge=atof(lpString);

		strcpy(lpKeyName,"Type");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nVType=atoi(lpString);

		strcpy(lpKeyName,"DivNum");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nDivisionNum=atoi(lpString);

		strcpy(lpKeyName,"DecadesNum");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nDecadesNum=atoi(lpString);

		strcpy(lpKeyName,"StartCyc");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nSCycle=atoi(lpString);
	
		trackListView1.Add(track);
	}

	for(i=0;i<nCurveNum1;i++)
	{
		curve=new CCurveList();
		//memset(curve,0,sizeof(curve));

		sprintf(lpAppName,"Curve1_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		strcpy(curve->szMnemonic,lpString);

		strcpy(lpKeyName,"Display");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->bActive=BOOL(atoi(lpString));

		strcpy(lpKeyName,"Idx");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nIdx=atoi(lpString);

		strcpy(lpKeyName,"TrackIdx");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nTrackIdx=atoi(lpString);

		strcpy(lpKeyName,"Type");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nScale=atoi(lpString);

		strcpy(lpKeyName,"Left");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->fLeftScale=atof(lpString);

		strcpy(lpKeyName,"Right");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->fRightScale=atof(lpString);

		strcpy(lpKeyName,"Color");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nColor=atoi(lpString);

		curveListView1.Add(curve);
	}

	for(i=0;i<nTrackNum2;i++)
	{
		track=new CTrackList();
		//memset(track,0,sizeof(track));

		sprintf(lpAppName,"Track2_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		strcpy(track->szMnemonic,lpString);

		strcpy(lpKeyName,"DepthTrack");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->bDepthTrack=BOOL(atoi(lpString));

		strcpy(lpKeyName,"VertLine");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->bVertOn=BOOL(atoi(lpString));

		strcpy(lpKeyName,"HorzLine");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->bHorzOn=BOOL(atoi(lpString));

		strcpy(lpKeyName,"Left");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->fLeftEdge=atof(lpString);

		strcpy(lpKeyName,"Right");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->fRightEdge=atof(lpString);

		strcpy(lpKeyName,"Type");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nVType=atoi(lpString);

		strcpy(lpKeyName,"DivNum");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nDivisionNum=atoi(lpString);

		strcpy(lpKeyName,"DecadesNum");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nDecadesNum=atoi(lpString);

		strcpy(lpKeyName,"StartCyc");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nSCycle=atoi(lpString);
	
		trackListView2.Add(track);
	}

	for(i=0;i<nCurveNum2;i++)
	{
		curve=new CCurveList();
		//memset(curve,0,sizeof(curve));

		sprintf(lpAppName,"Curve2_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		strcpy(curve->szMnemonic,lpString);

		strcpy(lpKeyName,"Display");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->bActive=BOOL(atoi(lpString));

		strcpy(lpKeyName,"Idx");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nIdx=atoi(lpString);

		strcpy(lpKeyName,"TrackIdx");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nTrackIdx=atoi(lpString);

		strcpy(lpKeyName,"Type");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nScale=atoi(lpString);

		strcpy(lpKeyName,"Left");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->fLeftScale=atof(lpString);

		strcpy(lpKeyName,"Right");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->fRightScale=atof(lpString);

		strcpy(lpKeyName,"Color");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nColor=atoi(lpString);

		curveListView2.Add(curve);
	}

	for(i = 0; i < 8; i++){
		sprintf(lpKeyName,"Syn_Ch%d",i+1);
		GetPrivateProfileString("Last_Channels",lpKeyName,lpDefault,pInfoCalib[i].szNameChannel,20,lpFileName);
		for(int j = 0; j<3;j++){
			sprintf(lpKeyName,"Raw%d%d",i+1,j+1);
			GetPrivateProfileString("Calibration",lpKeyName,lpDefault,lpString,80,lpFileName);
			pInfoCalib[i].pCh.RawVal[j] = atof(lpString);
			sprintf(lpKeyName,"Real%d%d",i+1,j+1);
			GetPrivateProfileString("Calibration",lpKeyName,lpDefault,lpString,80,lpFileName);
			pInfoCalib[i].pCh.RealVal[j] = atof(lpString);
		}
	}
	return 1;
}*/
int CMFTMonitorDoc::ReadMFTIniFile()
{
	char		lpFileName[150];
	char		lpAppName[80];
	char		lpKeyName[80];
	char		lpString[80];
	char		lpDefault[80];


	char		lpDrive[10];
	char		lpDir[150];

	::GetModuleFileName(NULL,lpFileName,80);
	_splitpath(lpFileName,lpDrive,lpDir,NULL,NULL);	
	strcpy(lpFileName,lpDrive);
	strcat(lpFileName,lpDir);
	strcat(lpFileName,"mft.ini");
	strcpy(lpDefault,"0");

	if(!IsFileExist(lpFileName))
		return 0;

	CTrackList*		track;
	CCurveList*		curve;
	int				nTrackNum1;
	int				nTrackNum2;
	int				nCurveNum1;
	int				nCurveNum2,i;

	strcpy(lpAppName,"General");
	strcpy(lpKeyName,"QuartzIndex");	
//	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
//	Quartz_1st_flag=atoi(lpString);
	

	strcpy(lpKeyName,"TrackNum1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nTrackNum1=atoi(lpString);
	strcpy(lpKeyName,"CurveNum1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nCurveNum1=atoi(lpString);

	strcpy(lpKeyName,"TrackNum2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nTrackNum2=atoi(lpString);
	strcpy(lpKeyName,"CurveNum2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nCurveNum2=atoi(lpString);

	strcpy(lpKeyName,"Scale1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nRightUpperViewScale=atoi(lpString);

	strcpy(lpKeyName,"Scale2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nRightLowerViewScale=atoi(lpString);

	strcpy(lpKeyName,"GaugeHydraulicMax1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeHydraulicMax1=atoi(lpString);
	strcpy(lpKeyName,"GaugeHydraulicMin1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeHydraulicMin1=atoi(lpString);
	strcpy(lpKeyName,"GaugeHydraulicMax2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeHydraulicMax2=atoi(lpString);
	strcpy(lpKeyName,"GaugeHydraulicMin2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeHydraulicMin2=atoi(lpString);
	strcpy(lpKeyName,"GaugeHydraulicLogarit");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	bGaugeHydraulicLogarit=BOOL(atoi(lpString));


	strcpy(lpKeyName,"GaugeSampleMax1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeSampleMax1=atoi(lpString);
	strcpy(lpKeyName,"GaugeSampleMin1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeSampleMin1=atoi(lpString);
	strcpy(lpKeyName,"GaugeSampleMax2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeSampleMax2=atoi(lpString);
	strcpy(lpKeyName,"GaugeSampleMin2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nGaugeSampleMin2=atoi(lpString);
	strcpy(lpKeyName,"GaugeSampleLogarit");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	bGaugeSampleLogarit=BOOL(atoi(lpString));

	
	strcpy(lpKeyName,"QuartzPretestMax1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nQuartzPretestMax1=atoi(lpString);
	strcpy(lpKeyName,"QuartzPretestMin1");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nQuartzPretestMin1=atoi(lpString);
	strcpy(lpKeyName,"QuartzPretestMax2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nQuartzPretestMax2=atoi(lpString);
	strcpy(lpKeyName,"QuartzPretestMin2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nQuartzPretestMin2=atoi(lpString);
	strcpy(lpKeyName,"QuartzPretestLogarit");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	bQuartzPretestLogarit=BOOL(atoi(lpString));

	strcpy(lpKeyName,"PositionMax");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nPositionMax=atoi(lpString);
	strcpy(lpKeyName,"PositionMin");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nPositionMin=atoi(lpString);
	
	strcpy(lpKeyName,"ALM");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	mbALM=atoi(lpString);
	strcpy(lpKeyName,"DL60");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	mbDL60=atoi(lpString);
	strcpy(lpKeyName,"BK3");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	mbBK3=atoi(lpString);
	strcpy(lpKeyName,"GK76");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	mbGK76=atoi(lpString);
	strcpy(lpKeyName,"GK60");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	mbGK60=atoi(lpString);

	strcpy(lpKeyName,"SKP");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	mbSKP=atoi(lpString);

	strcpy(lpKeyName,"MSC");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	mbMSC=atoi(lpString);

	strcpy(lpKeyName,"AK");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	mbAK_T=atoi(lpString);
	mbAK1_1 = mbAK1_2 = mbAK2_1 = mbAK2_2 = mbAK_T;

	strcpy(lpKeyName,"EncoderConst");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	fEncoderConst=atof(lpString);

	strcpy(lpKeyName,"Msc60_Flag");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	msc60_flag=atoi(lpString);

	strcpy(lpKeyName,"Msc_iCalib");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	m_sMSCCalib =lpString;


	strcpy(lpAppName,"Files");
	strcpy(lpKeyName,"Last_File");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,CurName,80,lpFileName);
//	Quartz_1st_flag=atoi(lpString);

	for(i=0;i<nTrackNum1;i++)
	{
		track=new CTrackList();
		//memset(track,0,sizeof(track));

		sprintf(lpAppName,"Track1_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		strcpy(track->szMnemonic,lpString);

		strcpy(lpKeyName,"DepthTrack");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->bDepthTrack=BOOL(atoi(lpString));

		strcpy(lpKeyName,"VertLine");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->bVertOn=BOOL(atoi(lpString));

		strcpy(lpKeyName,"HorzLine");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->bHorzOn=BOOL(atoi(lpString));

		strcpy(lpKeyName,"Left");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->fLeftEdge=atof(lpString);

		strcpy(lpKeyName,"Right");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->fRightEdge=atof(lpString);

		strcpy(lpKeyName,"Type");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nVType=atoi(lpString);

		strcpy(lpKeyName,"DivNum");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nDivisionNum=atoi(lpString);

		strcpy(lpKeyName,"DecadesNum");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nDecadesNum=atoi(lpString);

		strcpy(lpKeyName,"StartCyc");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nSCycle=atoi(lpString);
	
		trackListView1.Add(track);
	}

	for(i=0;i<nCurveNum1;i++)
	{
		curve=new CCurveList();
		//memset(curve,0,sizeof(curve));

		sprintf(lpAppName,"Curve1_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		strcpy(curve->szMnemonic,lpString);

		strcpy(lpKeyName,"Display");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->bActive=BOOL(atoi(lpString));

		strcpy(lpKeyName,"Idx");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nIdx=atoi(lpString);

		strcpy(lpKeyName,"TrackIdx");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nTrackIdx=atoi(lpString);

		strcpy(lpKeyName,"Type");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nScale=atoi(lpString);

		strcpy(lpKeyName,"Left");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->fLeftScale=atof(lpString);

		strcpy(lpKeyName,"Right");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->fRightScale=atof(lpString);

		strcpy(lpKeyName,"Color");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nColor=atoi(lpString);

		strcpy(lpKeyName,"Width");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nWidth=atoi(lpString);

		strcpy(lpKeyName,"Style");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nStyle=atoi(lpString);

		curveListView1.Add(curve);
	}

	for(i=0;i<nTrackNum2;i++)
	{
		track=new CTrackList();
		//memset(track,0,sizeof(track));

		sprintf(lpAppName,"Track2_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		strcpy(track->szMnemonic,lpString);

		strcpy(lpKeyName,"DepthTrack");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->bDepthTrack=BOOL(atoi(lpString));

		strcpy(lpKeyName,"VertLine");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->bVertOn=BOOL(atoi(lpString));

		strcpy(lpKeyName,"HorzLine");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->bHorzOn=BOOL(atoi(lpString));

		strcpy(lpKeyName,"Left");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->fLeftEdge=atof(lpString);

		strcpy(lpKeyName,"Right");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->fRightEdge=atof(lpString);

		strcpy(lpKeyName,"Type");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nVType=atoi(lpString);

		strcpy(lpKeyName,"DivNum");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nDivisionNum=atoi(lpString);

		strcpy(lpKeyName,"DecadesNum");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nDecadesNum=atoi(lpString);

		strcpy(lpKeyName,"StartCyc");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		track->nSCycle=atoi(lpString);
	
		trackListView2.Add(track);
	}

	for(i=0;i<nCurveNum2;i++)
	{
		curve=new CCurveList();
		//memset(curve,0,sizeof(curve));

		sprintf(lpAppName,"Curve2_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		strcpy(curve->szMnemonic,lpString);

		strcpy(lpKeyName,"Display");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->bActive=BOOL(atoi(lpString));
		
		strcpy(lpKeyName,"Idx");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nIdx=atoi(lpString);

		strcpy(lpKeyName,"TrackIdx");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nTrackIdx=atoi(lpString);

		strcpy(lpKeyName,"Type");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nScale=atoi(lpString);

		strcpy(lpKeyName,"Left");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->fLeftScale=atof(lpString);

		strcpy(lpKeyName,"Right");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->fRightScale=atof(lpString);

		strcpy(lpKeyName,"Color");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nColor=atoi(lpString);

		strcpy(lpKeyName,"Width");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nWidth=atoi(lpString);

		strcpy(lpKeyName,"Style");
		GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
		curve->nStyle=atoi(lpString);

		curveListView2.Add(curve);
	}
	if(mbMSC){
		for(i = 0; i < NUMCHANNEL; i++){
		sprintf(lpKeyName,"Syn_Ch%d",i+1);
		GetPrivateProfileString("Last_Channels",lpKeyName,lpDefault,pInfoCalib[i].szNameChannel,20,lpFileName);
		for(int j = 0; j<3;j++){
			sprintf(lpKeyName,"Raw%d%d",i+1,j+1);
			GetPrivateProfileString(m_sMSCCalib,lpKeyName,lpDefault,lpString,80,lpFileName);
			pInfoCalib[i].pCh.RawVal[j] = atof(lpString);
			sprintf(lpKeyName,"Real%d%d",i+1,j+1);
			GetPrivateProfileString(m_sMSCCalib,lpKeyName,lpDefault,lpString,80,lpFileName);
			pInfoCalib[i].pCh.RealVal[j] = atof(lpString);
		}
	}
	}
	
	else{
		for(i = 0; i < NUMCHANNEL; i++){
		sprintf(lpKeyName,"Syn_Ch%d",i+1);
		GetPrivateProfileString("Last_Channels",lpKeyName,lpDefault,pInfoCalib[i].szNameChannel,20,lpFileName);
		for(int j = 0; j<3;j++){
			sprintf(lpKeyName,"Raw%d%d",i+1,j+1);
			GetPrivateProfileString("Calibration",lpKeyName,lpDefault,lpString,80,lpFileName);
			pInfoCalib[i].pCh.RawVal[j] = atof(lpString);
			sprintf(lpKeyName,"Real%d%d",i+1,j+1);
			GetPrivateProfileString("Calibration",lpKeyName,lpDefault,lpString,80,lpFileName);
			pInfoCalib[i].pCh.RealVal[j] = atof(lpString);
		}
	}
	
	}

	

	return 1;
}





/*
void CMFTMonitorDoc::WriteMFTIniFile()
{
	char		lpFileName[150];
	char		lpAppName[80];
	char		lpKeyName[80];
	char		lpString[80];
	char		lpDefault[80];


	char		lpDrive[10];
	char		lpDir[150];

	::GetModuleFileName(NULL,lpFileName,80);
	_splitpath(lpFileName,lpDrive,lpDir,NULL,NULL);	
	strcpy(lpFileName,lpDrive);
	strcat(lpFileName,lpDir);
	strcat(lpFileName,"mft.ini");
	strcpy(lpDefault,"0");

	strcpy(lpAppName,"General");
	strcpy(lpKeyName,"QuartzIndex");
	sprintf(lpString,"%d",Quartz_1st_flag);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"TrackNum1");
	sprintf(lpString,"%d",trackListView1.GetSize());
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"CurveNum1");
	sprintf(lpString,"%d",curveListView1.GetSize());
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"TrackNum2");
	sprintf(lpString,"%d",trackListView2.GetSize());
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"CurveNum2");
	sprintf(lpString,"%d",curveListView2.GetSize());
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"Scale1");
	sprintf(lpString,"%d",nRightUpperViewScale);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"Scale2");
	sprintf(lpString,"%d",nRightLowerViewScale);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"GaugeHydraulicMax1");
	sprintf(lpString,"%d",nGaugeHydraulicMax1);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"GaugeHydraulicMin1");
	sprintf(lpString,"%d",nGaugeHydraulicMin1);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"GaugeHydraulicMax2");
	sprintf(lpString,"%d",nGaugeHydraulicMax2);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"GaugeHydraulicMin2");
	sprintf(lpString,"%d",nGaugeHydraulicMin2);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"GaugeHydraulicLogarit");
	sprintf(lpString,"%d",int(bGaugeHydraulicLogarit));
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);


	strcpy(lpKeyName,"GaugeSampleMax1");
	sprintf(lpString,"%d",nGaugeSampleMax1);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"GaugeSampleMin1");
	sprintf(lpString,"%d",nGaugeSampleMin1);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"GaugeSampleMax2");
	sprintf(lpString,"%d",nGaugeSampleMax2);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"GaugeSampleMin2");
	sprintf(lpString,"%d",nGaugeSampleMin2);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"GaugeSampleLogarit");
	sprintf(lpString,"%d",int(bGaugeSampleLogarit));
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"QuartzPretestMax1");
	sprintf(lpString,"%d",nQuartzPretestMax1);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"QuartzPretestMin1");
	sprintf(lpString,"%d",nQuartzPretestMin1);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"QuartzPretestMax2");
	sprintf(lpString,"%d",nQuartzPretestMax2);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"QuartzPretestMin2");
	sprintf(lpString,"%d",nQuartzPretestMin2);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"QuartzPretestLogarit");
	sprintf(lpString,"%d",int(bQuartzPretestLogarit));
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	

	strcpy(lpKeyName,"PositionMax");
	sprintf(lpString,"%d",nPositionMax);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"PositionMin");
	sprintf(lpString,"%d",nPositionMin);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"EncoderConst");
	sprintf(lpString,"%f",fEncoderConst);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	CTrackList*		track;
	for(int i=0;i<trackListView1.GetSize();i++)
	{
		track=trackListView1.GetAt(i);
		
		sprintf(lpAppName,"Track1_%d",i+1);
		
		strcpy(lpKeyName,"Mnemonic");
		strcpy(lpString,track->szMnemonic);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DepthTrack");
		sprintf(lpString,"%d",int(track->bDepthTrack));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"VertLine");
		sprintf(lpString,"%d",int(track->bVertOn));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"HorzLine");
		sprintf(lpString,"%d",int(track->bHorzOn));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Left");
		sprintf(lpString,"%.2f",track->fLeftEdge);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Right");
		sprintf(lpString,"%.2f",track->fRightEdge);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Type");
		sprintf(lpString,"%d",track->nVType);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DivNum");
		sprintf(lpString,"%d",track->nDivisionNum);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DecadesNum");
		sprintf(lpString,"%d",track->nDecadesNum);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"StartCyc");
		sprintf(lpString,"%d",track->nSCycle);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	}


	CCurveList*		curve;
	for(i=0;i<curveListView1.GetSize();i++)
	{
		curve=curveListView1.GetAt(i);

		sprintf(lpAppName,"Curve1_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		strcpy(lpString,curve->szMnemonic);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Display");
		sprintf(lpString,"%d",int(curve->bActive));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Idx");
		sprintf(lpString,"%d",curve->nIdx);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"TrackIdx");
		sprintf(lpString,"%d",curve->nTrackIdx);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Type");
		sprintf(lpString,"%d",curve->nScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Left");
		sprintf(lpString,"%f",curve->fLeftScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Right");
		sprintf(lpString,"%f",curve->fRightScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Color");
		sprintf(lpString,"%d",curve->nColor);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Width");
		sprintf(lpString,"%d",curve->nWidth);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Style");
		sprintf(lpString,"%d",curve->nStyle);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	}

	for(i=0;i<trackListView2.GetSize();i++)
	{
		track=trackListView2.GetAt(i);
		
		sprintf(lpAppName,"Track2_%d",i+1);
		
		strcpy(lpKeyName,"Mnemonic");
		strcpy(lpString,track->szMnemonic);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DepthTrack");
		sprintf(lpString,"%d",int(track->bDepthTrack));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"VertLine");
		sprintf(lpString,"%d",int(track->bVertOn));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"HorzLine");
		sprintf(lpString,"%d",int(track->bHorzOn));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Left");
		sprintf(lpString,"%.2f",track->fLeftEdge);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Right");
		sprintf(lpString,"%.2f",track->fRightEdge);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Type");
		sprintf(lpString,"%d",track->nVType);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DivNum");
		sprintf(lpString,"%d",track->nDivisionNum);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DecadesNum");
		sprintf(lpString,"%d",track->nDecadesNum);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"StartCyc");
		sprintf(lpString,"%d",track->nSCycle);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	}


	for(i=0;i<curveListView2.GetSize();i++)
	{
		curve=curveListView2.GetAt(i);

		sprintf(lpAppName,"Curve2_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		strcpy(lpString,curve->szMnemonic);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Display");
		sprintf(lpString,"%d",int(curve->bActive));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"TrackIdx");
		sprintf(lpString,"%d",curve->nTrackIdx);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Idx");
		sprintf(lpString,"%d",curve->nIdx);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Type");
		sprintf(lpString,"%d",curve->nScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Left");
		sprintf(lpString,"%f",curve->fLeftScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Right");
		sprintf(lpString,"%f",curve->fRightScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Color");
		sprintf(lpString,"%d",curve->nColor);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Width");
		sprintf(lpString,"%d",curve->nWidth);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Style");
		sprintf(lpString,"%d",curve->nStyle);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	}
}*/
void CMFTMonitorDoc::WriteMFTIniFile()
{
	char		lpFileName[150];
	char		lpAppName[80];
	char		lpKeyName[80];
	char		lpString[80];
	char		lpDefault[80];


	char		lpDrive[10];
	char		lpDir[150];
	int i;
	
	
	::GetModuleFileName(NULL,lpFileName,80);
	_splitpath(lpFileName,lpDrive,lpDir,NULL,NULL);	
	strcpy(lpFileName,lpDrive);
	strcat(lpFileName,lpDir);
	strcat(lpFileName,"mft.ini");
	strcpy(lpDefault,"0");

	
	char lptemp[150];
	if(strcmp(CurName,"")){
		strcpy(lpAppName,"Files");
		strcpy(lpKeyName,"Last_File");
		WritePrivateProfileString(lpAppName,lpKeyName,CurName,lpFileName);
		_splitpath(CurName,lpDrive,lpDir,NULL,NULL);
		strcpy(lpString,lpDrive);
		strcat(lpString,lpDir);
		strcpy(lpKeyName,"Current_Path");
		WritePrivateProfileString(lpAppName,lpKeyName,lpString ,lpFileName);
	}

	strcpy(lpAppName,"General");

	strcpy(lpKeyName,"TrackNum1");
	sprintf(lpString,"%d",trackListView1.GetSize());
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"CurveNum1");
	sprintf(lpString,"%d",curveListView1.GetSize());
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"TrackNum2");
	sprintf(lpString,"%d",trackListView2.GetSize());
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"CurveNum2");
	sprintf(lpString,"%d",curveListView2.GetSize());
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"Scale1");
	sprintf(lpString,"%d",nRightUpperViewScale);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"Scale2");
	sprintf(lpString,"%d",nRightLowerViewScale);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"GaugeHydraulicMax1");
	sprintf(lpString,"%d",nGaugeHydraulicMax1);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"GaugeHydraulicMin1");
	sprintf(lpString,"%d",nGaugeHydraulicMin1);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"GaugeHydraulicMax2");
	sprintf(lpString,"%d",nGaugeHydraulicMax2);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"GaugeHydraulicMin2");
	sprintf(lpString,"%d",nGaugeHydraulicMin2);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"GaugeHydraulicLogarit");
	sprintf(lpString,"%d",int(bGaugeHydraulicLogarit));
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);


	strcpy(lpKeyName,"GaugeSampleMax1");
	sprintf(lpString,"%d",nGaugeSampleMax1);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"GaugeSampleMin1");
	sprintf(lpString,"%d",nGaugeSampleMin1);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"GaugeSampleMax2");
	sprintf(lpString,"%d",nGaugeSampleMax2);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"GaugeSampleMin2");
	sprintf(lpString,"%d",nGaugeSampleMin2);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"GaugeSampleLogarit");
	sprintf(lpString,"%d",int(bGaugeSampleLogarit));
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"QuartzPretestMax1");
	sprintf(lpString,"%d",nQuartzPretestMax1);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"QuartzPretestMin1");
	sprintf(lpString,"%d",nQuartzPretestMin1);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"QuartzPretestMax2");
	sprintf(lpString,"%d",nQuartzPretestMax2);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"QuartzPretestMin2");
	sprintf(lpString,"%d",nQuartzPretestMin2);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"QuartzPretestLogarit");
	sprintf(lpString,"%d",int(bQuartzPretestLogarit));
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	

	strcpy(lpKeyName,"PositionMax");
	sprintf(lpString,"%d",nPositionMax);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"PositionMin");
	sprintf(lpString,"%d",nPositionMin);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"ALM");
	sprintf(lpString,"%d",mbALM);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"DL60");
	sprintf(lpString,"%d",mbDL60);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"BK3");
	sprintf(lpString,"%d",mbBK3);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"GK76");
	sprintf(lpString,"%d",mbGK76);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"GK60");
	sprintf(lpString,"%d",mbGK60);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"SKP");
	sprintf(lpString,"%d",mbSKP);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"MSC");
	sprintf(lpString,"%d",mbMSC);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"AK");
	sprintf(lpString,"%d",mbAK_T);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);


	strcpy(lpKeyName,"EncoderConst");
	sprintf(lpString,"%f",fEncoderConst);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"Msc60_Flag");
	sprintf(lpString,"%d",msc60_flag);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"Msc_iCalib");
	sprintf(lpString,"%s",m_sMSCCalib);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	
	CTrackList*		track;
	for(i=0;i<trackListView1.GetSize();i++)
	{
		track=trackListView1.GetAt(i);
		
		sprintf(lpAppName,"Track1_%d",i+1);
		
		strcpy(lpKeyName,"Mnemonic");
		strcpy(lpString,track->szMnemonic);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DepthTrack");
		sprintf(lpString,"%d",int(track->bDepthTrack));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"VertLine");
		sprintf(lpString,"%d",int(track->bVertOn));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"HorzLine");
		sprintf(lpString,"%d",int(track->bHorzOn));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Left");
		sprintf(lpString,"%.2f",track->fLeftEdge);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Right");
		sprintf(lpString,"%.2f",track->fRightEdge);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Type");
		sprintf(lpString,"%d",track->nVType);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DivNum");
		sprintf(lpString,"%d",track->nDivisionNum);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DecadesNum");
		sprintf(lpString,"%d",track->nDecadesNum);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"StartCyc");
		sprintf(lpString,"%d",track->nSCycle);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	}


	CCurveList*		curve;
	for(i=0;i<curveListView1.GetSize();i++)
	{
		curve=curveListView1.GetAt(i);

		sprintf(lpAppName,"Curve1_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		strcpy(lpString,curve->szMnemonic);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Display");
		sprintf(lpString,"%d",int(curve->bActive));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Idx");
		sprintf(lpString,"%d",curve->nIdx);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"TrackIdx");
		sprintf(lpString,"%d",curve->nTrackIdx);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Type");
		sprintf(lpString,"%d",curve->nScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Left");
		sprintf(lpString,"%f",curve->fLeftScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Right");
		sprintf(lpString,"%f",curve->fRightScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Color");
		sprintf(lpString,"%d",curve->nColor);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
		
		strcpy(lpKeyName,"Width");
		sprintf(lpString,"%d",curve->nWidth);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Style");
		sprintf(lpString,"%d",curve->nStyle);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	}

	for(i=0;i<trackListView2.GetSize();i++)
	{
		track=trackListView2.GetAt(i);
		
		sprintf(lpAppName,"Track2_%d",i+1);
		
		strcpy(lpKeyName,"Mnemonic");
		strcpy(lpString,track->szMnemonic);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DepthTrack");
		sprintf(lpString,"%d",int(track->bDepthTrack));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"VertLine");
		sprintf(lpString,"%d",int(track->bVertOn));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"HorzLine");
		sprintf(lpString,"%d",int(track->bHorzOn));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Left");
		sprintf(lpString,"%.2f",track->fLeftEdge);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Right");
		sprintf(lpString,"%.2f",track->fRightEdge);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Type");
		sprintf(lpString,"%d",track->nVType);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DivNum");
		sprintf(lpString,"%d",track->nDivisionNum);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DecadesNum");
		sprintf(lpString,"%d",track->nDecadesNum);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"StartCyc");
		sprintf(lpString,"%d",track->nSCycle);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	}


	for(i=0;i<curveListView2.GetSize();i++)
	{
		curve=curveListView2.GetAt(i);

		sprintf(lpAppName,"Curve2_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		strcpy(lpString,curve->szMnemonic);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Display");
		sprintf(lpString,"%d",int(curve->bActive));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"TrackIdx");
		sprintf(lpString,"%d",curve->nTrackIdx);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Idx");
		sprintf(lpString,"%d",curve->nIdx);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Type");
		sprintf(lpString,"%d",curve->nScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Left");
		sprintf(lpString,"%f",curve->fLeftScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Right");
		sprintf(lpString,"%f",curve->fRightScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Color");
		sprintf(lpString,"%d",curve->nColor);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Width");
		sprintf(lpString,"%d",curve->nWidth);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Style");
		sprintf(lpString,"%d",curve->nStyle);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	}

	/*if(mbMSC){
		

		
		for(i = 0; i < NUMCHANNEL; i++){
			sprintf(lpKeyName,"Syn_Ch%d",i+1);
			WritePrivateProfileString("Last_Channels",lpKeyName,pInfoCalib[i].szNameChannel,lpFileName);

		
			for(int j = 0; j<3;j++){
				sprintf(lpKeyName,"Raw%d%d",i+1,j+1);
				sprintf(lpString,"%.3f",pInfoCalib[i].pCh.RawVal[j]);
				WritePrivateProfileString(m_sMSCCalib,lpKeyName,lpString,lpFileName);
			
				sprintf(lpKeyName,"Real%d%d",i+1,j+1);
				sprintf(lpString,"%.3f",pInfoCalib[i].pCh.RealVal[j]);
				WritePrivateProfileString(m_sMSCCalib,lpKeyName,lpString,lpFileName);
			
			}
		}
		
	}*/
	
	
	for(i = 0; i < NUMCHANNEL; i++){
		sprintf(lpKeyName,"Syn_Ch%d",i+1);
		WritePrivateProfileString("Last_Channels",lpKeyName,pInfoCalib[i].szNameChannel,lpFileName);

		
		for(int j = 0; j<3;j++){
			sprintf(lpKeyName,"Raw%d%d",i+1,j+1);
			sprintf(lpString,"%.3f",pInfoCalib[i].pCh.RawVal[j]);
			WritePrivateProfileString("Calibration",lpKeyName,lpString,lpFileName);
			
			sprintf(lpKeyName,"Real%d%d",i+1,j+1);
			sprintf(lpString,"%.3f",pInfoCalib[i].pCh.RealVal[j]);
			WritePrivateProfileString("Calibration",lpKeyName,lpString,lpFileName);
			
		}
	}
	
	
	
}

int CMFTMonitorDoc::ReadMFTIniFile1()
{
	char		lpFileName[150];
	char		lpAppName[80];
	char		lpKeyName[80];
	char		lpString[80];
	char		lpDefault[80];

	int i;
	char		lpDrive[10];
	char		lpDir[150];

	::GetModuleFileName(NULL,lpFileName,80);
	_splitpath(lpFileName,lpDrive,lpDir,NULL,NULL);	
	strcpy(lpFileName,lpDrive);
	strcat(lpFileName,lpDir);
	strcat(lpFileName,"mft.ini");
	strcpy(lpDefault,"0");

	if(!IsFileExist(lpFileName))
		return 0;

	
	//strcpy(lpAppName,"General");
	

	//strcpy(lpKeyName,"MSC");	
	//GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	//mbMSC=atoi(lpString);

	
	//strcpy(lpKeyName,"Msc60_Flag");	
	//GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	//msc60_flag=atoi(lpString);

	//strcpy(lpKeyName,"Msc_iCalib");	
	//GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	//m_sMSCCalib =lpString;


	strcpy(lpAppName,"Files");
	strcpy(lpKeyName,"Last_File");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,CurName,80,lpFileName);


	
	if(mbMSC){
		for(i = 0; i < NUMCHANNEL; i++){
		sprintf(lpKeyName,"Syn_Ch%d",i+1);
		GetPrivateProfileString("Last_Channels",lpKeyName,lpDefault,pInfoCalib[i].szNameChannel,20,lpFileName);
		for(int j = 0; j<3;j++){
			sprintf(lpKeyName,"Raw%d%d",i+1,j+1);
			GetPrivateProfileString(m_sMSCCalib,lpKeyName,lpDefault,lpString,80,lpFileName);
			pInfoCalib[i].pCh.RawVal[j] = atof(lpString);
			sprintf(lpKeyName,"Real%d%d",i+1,j+1);
			GetPrivateProfileString(m_sMSCCalib,lpKeyName,lpDefault,lpString,80,lpFileName);
			pInfoCalib[i].pCh.RealVal[j] = atof(lpString);
		}
	}
	}
	
	
	
	

	

	return 1;
}

void CMFTMonitorDoc::WriteMFTIniFile1()
{
	char		lpFileName[150];
	char		lpAppName[80];
	char		lpKeyName[80];
	char		lpString[80];
	char		lpDefault[80];


	char		lpDrive[10];
	char		lpDir[150];
	int i;
	
	
	::GetModuleFileName(NULL,lpFileName,80);
	_splitpath(lpFileName,lpDrive,lpDir,NULL,NULL);	
	strcpy(lpFileName,lpDrive);
	strcat(lpFileName,lpDir);
	strcat(lpFileName,"mft.ini");
	strcpy(lpDefault,"0");

	
	char lptemp[150];
	if(strcmp(CurName,"")){
		strcpy(lpAppName,"Files");
		strcpy(lpKeyName,"Last_File");
		WritePrivateProfileString(lpAppName,lpKeyName,CurName,lpFileName);
		_splitpath(CurName,lpDrive,lpDir,NULL,NULL);
		strcpy(lpString,lpDrive);
		strcat(lpString,lpDir);
		strcpy(lpKeyName,"Current_Path");
		WritePrivateProfileString(lpAppName,lpKeyName,lpString ,lpFileName);
	}

	

	
	for(i = 0; i < NUMCHANNEL; i++){
		sprintf(lpKeyName,"Syn_Ch%d",i+1);
		WritePrivateProfileString("Last_Channels",lpKeyName,pInfoCalib[i].szNameChannel,lpFileName);

		
		for(int j = 0; j<3;j++){
			sprintf(lpKeyName,"Raw%d%d",i+1,j+1);
			sprintf(lpString,"%.3f",pInfoCalib[i].pCh.RawVal[j]);
			WritePrivateProfileString(m_sMSCCalib,lpKeyName,lpString,lpFileName);
			
			sprintf(lpKeyName,"Real%d%d",i+1,j+1);
			sprintf(lpString,"%.3f",pInfoCalib[i].pCh.RealVal[j]);
			WritePrivateProfileString(m_sMSCCalib,lpKeyName,lpString,lpFileName);
			
		}
	}
}

void CMFTMonitorDoc::WriteMFTIniFileQ()
{
	char		lpFileName[150];
	char		lpAppName[80];
	char		lpKeyName[80];
	char		lpString[80];
	char		lpDefault[80];


	char		lpDrive[10];
	char		lpDir[150];
	int i;
	
	
	::GetModuleFileName(NULL,lpFileName,80);
	_splitpath(lpFileName,lpDrive,lpDir,NULL,NULL);	
	strcpy(lpFileName,lpDrive);
	strcat(lpFileName,lpDir);
	strcat(lpFileName,"mft.ini");
	strcpy(lpDefault,"0");

	
	char lptemp[150];
	if(strcmp(CurName,"")){
		strcpy(lpAppName,"Files");
		strcpy(lpKeyName,"Last_File");
		WritePrivateProfileString(lpAppName,lpKeyName,CurName,lpFileName);
		_splitpath(CurName,lpDrive,lpDir,NULL,NULL);
		strcpy(lpString,lpDrive);
		strcat(lpString,lpDir);
		strcpy(lpKeyName,"Current_Path");
		WritePrivateProfileString(lpAppName,lpKeyName,lpString ,lpFileName);
	}

	strcpy(lpAppName,"General");
	strcpy(lpKeyName,"TrackNum1");
	sprintf(lpString,"%d",trackListView1.GetSize());
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"CurveNum1");
	sprintf(lpString,"%d",curveListView1.GetSize());
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"TrackNum2");
	sprintf(lpString,"%d",trackListView2.GetSize());
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"CurveNum2");
	sprintf(lpString,"%d",curveListView2.GetSize());
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"Scale1");
	sprintf(lpString,"%d",nRightUpperViewScale);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"Scale2");
	sprintf(lpString,"%d",nRightLowerViewScale);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	
	strcpy(lpKeyName,"ALM");
	sprintf(lpString,"%d",mbALM);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"DL60");
	sprintf(lpString,"%d",mbDL60);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"BK3");
	sprintf(lpString,"%d",mbBK3);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	strcpy(lpKeyName,"GK76");
	sprintf(lpString,"%d",mbGK76);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"GK60");
	sprintf(lpString,"%d",mbGK60);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"SKP");
	sprintf(lpString,"%d",mbSKP);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"MSC");
	sprintf(lpString,"%d",mbMSC);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"AK");
	sprintf(lpString,"%d",mbAK_T);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);


	strcpy(lpKeyName,"EncoderConst");
	sprintf(lpString,"%f",fEncoderConst);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"Msc60_Flag");
	sprintf(lpString,"%d",msc60_flag);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

	strcpy(lpKeyName,"Msc_iCalib");
	sprintf(lpString,"%s",m_sMSCCalib);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);



	
	if(mbMSC){
		

		
		for(i = 0; i < NUMCHANNEL; i++){
			//sprintf(lpKeyName,"Syn_Ch%d",i+1);
			//WritePrivateProfileString(m_sMSCCalib,lpKeyName,pInfoCalib[i].szNameChannel,lpFileName);

		
			for(int j = 0; j<3;j++){
				sprintf(lpKeyName,"Raw%d%d",i+1,j+1);
				sprintf(lpString,"%.3f",pInfoCalib[i].pCh.RawVal[j]);
				WritePrivateProfileString(m_sMSCCalib,lpKeyName,lpString,lpFileName);
			
				sprintf(lpKeyName,"Real%d%d",i+1,j+1);
				sprintf(lpString,"%.3f",pInfoCalib[i].pCh.RealVal[j]);
				WritePrivateProfileString(m_sMSCCalib,lpKeyName,lpString,lpFileName);
			
			}
		}
		
	}




	
	CTrackList*		track;
	for(i=0;i<trackListView1.GetSize();i++)
	{
		track=trackListView1.GetAt(i);
		
		sprintf(lpAppName,"Track1_%d",i+1);
		
		strcpy(lpKeyName,"Mnemonic");
		strcpy(lpString,track->szMnemonic);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DepthTrack");
		sprintf(lpString,"%d",int(track->bDepthTrack));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"VertLine");
		sprintf(lpString,"%d",int(track->bVertOn));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"HorzLine");
		sprintf(lpString,"%d",int(track->bHorzOn));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Left");
		sprintf(lpString,"%.2f",track->fLeftEdge);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Right");
		sprintf(lpString,"%.2f",track->fRightEdge);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Type");
		sprintf(lpString,"%d",track->nVType);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DivNum");
		sprintf(lpString,"%d",track->nDivisionNum);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DecadesNum");
		sprintf(lpString,"%d",track->nDecadesNum);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"StartCyc");
		sprintf(lpString,"%d",track->nSCycle);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	}


	CCurveList*		curve;
	for(i=0;i<curveListView1.GetSize();i++)
	{
		curve=curveListView1.GetAt(i);

		sprintf(lpAppName,"Curve1_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		strcpy(lpString,curve->szMnemonic);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Display");
		sprintf(lpString,"%d",int(curve->bActive));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
		

		strcpy(lpKeyName,"Idx");
		sprintf(lpString,"%d",curve->nIdx);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"TrackIdx");
		sprintf(lpString,"%d",curve->nTrackIdx);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Type");
		sprintf(lpString,"%d",curve->nScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Left");
		sprintf(lpString,"%f",curve->fLeftScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Right");
		sprintf(lpString,"%f",curve->fRightScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Color");
		sprintf(lpString,"%d",curve->nColor);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
		
		strcpy(lpKeyName,"Width");
		sprintf(lpString,"%d",curve->nWidth);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Style");
		sprintf(lpString,"%d",curve->nStyle);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	}

	for(i=0;i<trackListView2.GetSize();i++)
	{
		track=trackListView2.GetAt(i);
		
		sprintf(lpAppName,"Track2_%d",i+1);
		
		strcpy(lpKeyName,"Mnemonic");
		strcpy(lpString,track->szMnemonic);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DepthTrack");
		sprintf(lpString,"%d",int(track->bDepthTrack));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"VertLine");
		sprintf(lpString,"%d",int(track->bVertOn));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"HorzLine");
		sprintf(lpString,"%d",int(track->bHorzOn));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Left");
		sprintf(lpString,"%.2f",track->fLeftEdge);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Right");
		sprintf(lpString,"%.2f",track->fRightEdge);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Type");
		sprintf(lpString,"%d",track->nVType);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DivNum");
		sprintf(lpString,"%d",track->nDivisionNum);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"DecadesNum");
		sprintf(lpString,"%d",track->nDecadesNum);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"StartCyc");
		sprintf(lpString,"%d",track->nSCycle);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	}


	for(i=0;i<curveListView2.GetSize();i++)
	{
		curve=curveListView2.GetAt(i);

		sprintf(lpAppName,"Curve2_%d",i+1);

		strcpy(lpKeyName,"Mnemonic");
		strcpy(lpString,curve->szMnemonic);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Display");
		sprintf(lpString,"%d",int(curve->bActive));
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"TrackIdx");
		sprintf(lpString,"%d",curve->nTrackIdx);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Idx");
		sprintf(lpString,"%d",curve->nIdx);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Type");
		sprintf(lpString,"%d",curve->nScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Left");
		sprintf(lpString,"%f",curve->fLeftScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Right");
		sprintf(lpString,"%f",curve->fRightScale);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Color");
		sprintf(lpString,"%d",curve->nColor);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Width");
		sprintf(lpString,"%d",curve->nWidth);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);

		strcpy(lpKeyName,"Style");
		sprintf(lpString,"%d",curve->nStyle);
		WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
	}

	
	
	for(i = 0; i < NUMCHANNEL; i++){
		sprintf(lpKeyName,"Syn_Ch%d",i+1);
		WritePrivateProfileString("Last_Channels",lpKeyName,pInfoCalib[i].szNameChannel,lpFileName);

		
		for(int j = 0; j<3;j++){
			sprintf(lpKeyName,"Raw%d%d",i+1,j+1);
			sprintf(lpString,"%.3f",pInfoCalib[i].pCh.RawVal[j]);
			WritePrivateProfileString("Calibration",lpKeyName,lpString,lpFileName);
			
			sprintf(lpKeyName,"Real%d%d",i+1,j+1);
			sprintf(lpString,"%.3f",pInfoCalib[i].pCh.RealVal[j]);
			WritePrivateProfileString("Calibration",lpKeyName,lpString,lpFileName);
			
		}
	}
	

	
}

void CMFTMonitorDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	/*if(pFile!=NULL){
			pFile->Close();
			delete pFile;
	}*/
	/*if(m_rs.IsOpen()){
		m_rs.Close();
	}*/
	//if(m_pDB.IsOpen())
	//	m_pDB.Close();
	//CDocument::OnCloseDocument();
}
