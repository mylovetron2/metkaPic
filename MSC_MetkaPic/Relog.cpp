// Relog.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "MainFrm.h"
#include "Relog.h"
#include "threads.h"
#include "EditData.h"
#include "Panel.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRelog dialog


CRelog::CRelog(CWnd* pParent /*=NULL*/)
	: CDialog(CRelog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRelog)
	//}}AFX_DATA_INIT
}


void CRelog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRelog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRelog, CDialog)
	//{{AFX_MSG_MAP(CRelog)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRelog message handlers
int  one=0;
BOOL CRelog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	COLORREF	cr[8]={RGB(0,0,0),RGB(0,0,255),RGB(0,160,0),RGB(0,255,255),
					   RGB(255,0,0),RGB(255,0,255),RGB(255,160,0),RGB(192,0,0)};
	CString cs,str;
	VARIANT varV,ch;
	m_isPrinting = 1;
	

	CEditData* pEdit = new CEditData(this);
	m_nIndex=0;
	m_nLen=0;
	ReadIniFile();
	LOGFONT	logf;
	memset(&logf,0,sizeof(LOGFONT));
	logf.lfHeight=16;	
	logf.lfWeight=FW_EXTRABOLD;
	lstrcpy(logf.lfFaceName,_T("Arial"));
	hFont.CreateFontIndirect(&logf);
	pTrackList= &trackListView2;
	pCurveList= &curveListView2;


	GetClientRect(m_cRect);
	ScreenToClient(m_cRect);
	
	
	
	float xx, yy,dy;
	
	
	int x,y,time[4];
	x = m_cRect.left;
	y = m_cRect.top;
	m_cRect.left -= x;
	m_cRect.right -= x;
	m_cRect.top -= y ;
	m_cRect.bottom -= y ;
	yPos=0;
	yOldPos = m_cRect.top;
//	if(!one){
		CDC *pdc = GetDC();
		m_pDC.CreateCompatibleDC(pdc);
		cBitmap.CreateCompatibleBitmap(pdc,m_cRect.Width(),m_cRect.Height());
		ReleaseDC(pdc);
		m_pDC.SelectObject(&cBitmap);
		one=1;
//	}
	
	hPen.CreatePen(PS_SOLID,2,RGB(50,50,50));
	for(int i=0;i<pCurveList->GetSize();i++){
			curve=pCurveList->GetAt(i);
			curve->Pen.CreatePen(curve->nStyle,curve->nWidth,cr[curve->nColor]);
		
			if(curve->nTrackIdx<0)
				curve->nTrackIdx=0;
			if(curve->nTrackIdx>=pTrackList->GetSize())
				curve->nTrackIdx=pTrackList->GetSize()-1;
	}



	CFileDialog m_filedlg1(TRUE,"mdb",NULL,OFN_EXPLORER,
				  "Access Database Files (*.mdb)|*.mdb||");	
	if(m_filedlg1.DoModal()==IDOK){
			str = m_filedlg1.GetPathName();
			SetWindowText(str);
			m_rPs = pEdit->GetRecorset(str,&m_lFieldsNumber,&m_lRecordsNumber);
			m_rPs->MoveFirst();
			m_nIndex=0;
	}
	
	for(i=0;i<pCurveList->GetSize();i++){
			curve=pCurveList->GetAt(i);
			track=pTrackList->GetAt(curve->nTrackIdx);
			ch.vt = VT_BSTR;
			ch.bstrVal = _bstr_t(curve->szMnemonic);
			varV = m_rPs->GetFields()->GetItem(ch)->Value;
			xOldPos[i]=DataToScr(varV.fltVal,curve,track,m_cRect.Width());

	}
		m_rPs->MoveFirst();
		varV = m_rPs->GetFields()->GetItem(0L)->Value;
		nprev = TimeStringToInt(varV,time);
		m_rPs->MoveNext();
		varV = m_rPs->GetFields()->GetItem(0L)->Value;
		ncur = TimeStringToInt(varV,time);
		RightLowerViewScale = m_cRect.Height()*100.0/(ncur-nprev);
		m_nPage = RightLowerViewScale;
		m_nSize = m_nPage;
		RightLowerViewScale /= 4;
		if(m_nPage > (m_lRecordsNumber-1)) m_nPage = (m_lRecordsNumber-1);
		m_rPs->MoveFirst();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRelog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
	
	
	if(!m_isPrinting) 	{
		
		return;
	}
	
	
	int i,j;
	CString cs;
	int				nDepthMark=0;
	int				idx=0;
	float			deltaPixel;
	CString strOutput, str;
	
	int time[4];
		
	VARIANT index,varV,ch;
	index.vt = VT_I2;
	float ft;
	
	int nstartsime = nprev;
	int k;
	int nseconds;
	CRect rect;
	
	m_pDC.FillRect(m_cRect,(CBrush*)m_pDC.SelectStockObject(WHITE_BRUSH));
	DrawGrid(&m_pDC,5,&trackListView2,&m_cRect);
	if(m_isPrinting ==1){
		for(j=0; j< m_nPage;j++){
			if(j >= m_lRecordsNumber-1) break;
			m_nLen++;
			m_rPs->MoveNext();
			varV = m_rPs->GetFields()->GetItem(0L)->Value;
			ncur = TimeStringToInt(varV,time);
			deltaPixel=((ncur-nprev)/1000.0)*m_cRect.Height()/RightLowerViewScale;
			
			
			if(fabs(deltaPixel)>=1 && fabs(deltaPixel)<m_cRect.Height())	{
			
				nprev=ncur;
				yPos=yOldPos+fabs(deltaPixel);
				//Draw curve 
				for(i=0;i<pCurveList->GetSize();i++){
					curve=pCurveList->GetAt(i);
					if(!curve->bActive) continue;
					track=pTrackList->GetAt(curve->nTrackIdx);
					ch.vt = VT_BSTR;
					ch.bstrVal = _bstr_t(curve->szMnemonic);
					varV = m_rPs->GetFields()->GetItem(ch)->Value;
					xPos[i]=DataToScr(varV.fltVal,curve,track,m_cRect.Width());
					if((curve->bIsOldWrap != curve->bIsWrap)){
						xOldPos[i]= WrapConvert(varV.fltVal, curve,track,m_cRect.Width());
					}
					m_pDC.SelectObject(&curve->Pen);
					m_pDC.MoveTo(xOldPos[i],yOldPos);
					m_pDC.LineTo(xPos[i],yPos);
					xOldPos[i]=xPos[i];
					curve->bIsOldWrap = curve->bIsWrap;
				
				}
				yOldPos=yPos;
				
			}
			
			
			//Draw depth
			if(yPos>=nDepthMark*m_cRect.Height()/5.0){
				
				for(k=0;k<pTrackList->GetSize();k++){
					CTrackList*	track;
					track=pTrackList->GetAt(k);
					if(track->bDepthTrack==false)	continue;
					CString		str;
					float		x;
					int			nSeconds;
					//nseconds=int(float(ncur-nstartsime)/1000.0);
					m_pDC.SetTextColor(RGB(0,0,0));
					m_pDC.SetTextAlign(TA_TOP|TA_CENTER);
					x=(track->fLeftEdge+(track->fRightEdge-track->fLeftEdge)/2)*(m_cRect.right-m_cRect.left);
					str.Format("%d:%02d",time[1], time[2]);
					m_pDC.TextOut(x,nDepthMark*m_cRect.Height()/5.0,str);
				}
				nDepthMark++;
			}

			//Scroll up	
			dc.BitBlt(0,0,m_cRect.Width(),m_cRect.Height(),&m_pDC,0,0,SRCCOPY);	
			/*if(yPos>m_cRect.bottom)
			{
				//pDC->BitBlt(0,0,pRectPart->Width(),pRectPart->Height(),
					//	pDC,0,pRect->bottom-pRectPart->Height(),SRCCOPY);
				rect.left=m_cRect.left;
				rect.right=m_cRect.right;
				rect.bottom=m_cRect.bottom;
				rect.top=m_cRect.Height()*3.0/5.0;
				dc.FillRect(&rect,(CBrush*)dc.SelectStockObject(WHITE_BRUSH));				
				DrawGrid(&dc,5,&trackListView2,&m_cRect);
				//gridRect.left=pRect->left;
				//gridRect.right=pRect->right;
				//gridRect.bottom=pRect->bottom;
				//gridRect.top=int(0.6*(pRect->bottom-pRect->top));
				//DrawGrid(pDC,2,pTrackList,&gridRect);	
				dc.BitBlt(0,0,rect.Width(),rect.Height(),
						m_pDC,0,0,SRCCOPY);
				//yPos=pRectPart->Height();
				//yOldPos=yPos;
				//nDepthMark=3;				
			}*/				
			
		
		
		}
		m_nIndex++;
		m_isPrinting=0;
	}
	else if(m_isPrinting ==2){
		m_nIndex--;
		dc.BitBlt(0,0,m_cRect.Width(),m_cRect.Height(),&m_pDC,0,0,SRCCOPY);
		m_isPrinting=1;
	}

//	dc.FillRect(rect1,(CBrush*)dc.SelectStockObject(WHITE_BRUSH));
/*	for(i=0;i<pCurveList->GetSize();i++)	{
		curve=pCurveList->GetAt(i);
		track=pTrackList->GetAt(curve->nTrackIdx);
		dc.SelectObject(&hPen);
		dc.MoveTo(track->fLeftEdge*rect1.Width(),rect1.top);
		dc.LineTo(track->fLeftEdge*rect1.Width(),rect1.bottom);
		dc.MoveTo(track->fRightEdge*rect1.Width(),rect1.top);
		dc.LineTo(track->fRightEdge*rect1.Width(),rect1.bottom);

	}
	dc.MoveTo(rect1.left,rect1.top);
	dc.LineTo(rect1.right,rect1.top);

	int ncurve[] = {0,0,0,0,0,0,0,0};
	int ntrack[]= {0,1,2,3,4,5,6,7,8};
	int nindex[] = {0,0,0,0,0,0,0,0,0};

	for(i=0;i<pCurveList->GetSize();i++)	{
		curve=pCurveList->GetAt(i);
		for(j=0;j<8;j++){
			if((curve->nTrackIdx == ntrack[j]) && (curve->bActive == true))
			ncurve[j]++;
		}
	}

	for(i=0;i<pCurveList->GetSize();i++)	{
		curve=pCurveList->GetAt(i);
		if(curve->bActive == false) continue;
		track=pTrackList->GetAt(curve->nTrackIdx);
		curve->Pen.CreatePen(curve->nStyle,curve->nWidth,cr[curve->nColor]);
		dc.SelectObject(&curve->Pen);
		dc.SetTextAlign(TA_BOTTOM|TA_CENTER);
		dc.SetBkMode(TRANSPARENT);
		for(j=0;j<8;j++){
			if(curve->nTrackIdx == ntrack[j]){
				dc.MoveTo(track->fLeftEdge*rect1.Width()+2,rect1.top+(nindex[i]+1)*rect1.Height()/(ncurve[j]+0.5));
				dc.LineTo(track->fRightEdge*rect1.Width()-2,rect1.top+(nindex[i]+1)*rect1.Height()/(ncurve[j]+0.5));
				dc.SetTextColor(cr[curve->nColor]);
				dc.TextOut((track->fRightEdge-track->fLeftEdge)*rect1.Width()/2+track->fLeftEdge*rect1.Width(),
						rect1.top+(nindex[i]+1)*rect1.Height()/(ncurve[j]+0.5),curve->szMnemonic);
				cs.Format("%.1f",curve->fLeftScale);
				dc.TextOut((track->fLeftEdge + cs.GetLength()*0.004)*rect1.Width(),
						rect1.top+(nindex[i]+1)*rect1.Height()/(ncurve[j]+0.5),cs);
				cs.Format("%.1f",curve->fRightScale);
				dc.TextOut((track->fRightEdge - cs.GetLength()*0.004)*rect1.Width(),
						rect1.top+(nindex[i]+1)*rect1.Height()/(ncurve[j]+0.5),cs);
				nindex[i]++;
			}
		}

	}*/
		
	//	q=1;
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

/*void CRelog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
	//m_pDC->SelectObject(&cBitmap);
	int i,j;
	CString cs;
	int				nDepthMark=0;
	int				idx=0;
	float			deltaPixel;
	CString strOutput, str;
	
	int time[4];
		
	VARIANT index,varV,ch;
	index.vt = VT_I2;
	float ft;
	static int q=0;
	int nstartsime = nprev;
	int k;
	int nseconds;
	CRect rect;
	dc.FillRect(m_cRect,(CBrush*)dc.SelectStockObject(WHITE_BRUSH));
	DrawGrid(&dc,5,&trackListView2,&m_cRect);
		for(j=0; j< m_nPage;j++){
			if(j >= m_lRecordsNumber-1) break;
			m_nLen++;
			m_rPs->MoveNext();
			varV = m_rPs->GetFields()->GetItem(0L)->Value;
			ncur = TimeStringToInt(varV,time);
			deltaPixel=((ncur-nprev)/1000.0)*m_cRect.Height()/RightLowerViewScale;
			
			
			if(fabs(deltaPixel)>=1 && fabs(deltaPixel)<m_cRect.Height())	{
			
				nprev=ncur;
				yPos=yOldPos+fabs(deltaPixel);
				//Draw curve 
				for(i=0;i<pCurveList->GetSize();i++){
					curve=pCurveList->GetAt(i);
					if(!curve->bActive) continue;
					track=pTrackList->GetAt(curve->nTrackIdx);
					ch.vt = VT_BSTR;
					ch.bstrVal = _bstr_t(curve->szMnemonic);
					varV = m_rPs->GetFields()->GetItem(ch)->Value;
					xPos[i]=DataToScr(varV.fltVal,curve,track,m_cRect.Width());
					if((curve->bIsOldWrap != curve->bIsWrap)){
						xOldPos[i]= WrapConvert(varV.fltVal, curve,track,m_cRect.Width());
					}
					dc.SelectObject(&curve->Pen);
					dc.MoveTo(xOldPos[i],yOldPos);
					dc.LineTo(xPos[i],yPos);
					xOldPos[i]=xPos[i];
					curve->bIsOldWrap = curve->bIsWrap;
				
				}
				yOldPos=yPos;
				
			}
			
				
			//Draw depth
			if(yPos>=nDepthMark*m_cRect.Height()/5.0){
				
				for(k=0;k<pTrackList->GetSize();k++){
					CTrackList*	track;
					track=pTrackList->GetAt(k);
					if(track->bDepthTrack==false)	continue;
					CString		str;
					float		x;
					int			nSeconds;
					//nseconds=int(float(ncur-nstartsime)/1000.0);
					dc.SetTextColor(RGB(0,0,0));
					dc.SetTextAlign(TA_TOP|TA_CENTER);
					x=(track->fLeftEdge+(track->fRightEdge-track->fLeftEdge)/2)*(m_cRect.right-m_cRect.left);
					str.Format("%d:%02d",time[1], time[2]);
					dc.TextOut(x,nDepthMark*m_cRect.Height()/5.0,str);
				}
				nDepthMark++;
			}

			//Scroll up	
			m_pDC->StretchBlt(0,0,m_cRect.Width(),m_cRect.Height(),
						&dc,0,0,m_cRect.Width(),m_cRect.Height(),SRCCOPY);
			
			
		
		
	}
		m_nIndex++;

/
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}*/

int CRelog::ReadIniFile()
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
	
	int				nTrackNum2;
	int				nCurveNum2;
	int i;
	CTrackList*		track;
	CCurveList*		curve;
	strcpy(lpAppName,"General");

	strcpy(lpKeyName,"TrackNum2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nTrackNum2=atoi(lpString);
	strcpy(lpKeyName,"CurveNum2");	
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpString,80,lpFileName);
	nCurveNum2=atoi(lpString);

	for(i=0;i<nTrackNum2;i++)
	{
		track=new CTrackList();
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
	
	
	return 1;

}

void CRelog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
    GetClientRect(rect);
	switch (nSBCode)
    {
        case SB_LINEDOWN:
            {
                
                if(m_nLen >= (m_lRecordsNumber-1)) break;
				m_isPrinting=1;
			yPos=0;
			yOldPos = m_cRect.top;
				
			if((m_nIndex+1)*m_nPage < (m_lRecordsNumber-1)) Invalidate(true);
			else {		
				m_nPage  = m_lRecordsNumber-1 - m_nIndex*m_nPage;
				
					Invalidate(true);
				
			  }
            }
            break;

        case SB_LINEUP:
            
            {	if(m_nLen <= m_nSize) break;
				m_isPrinting=1;
                VARIANT varV,ch;
				int time[4];
                
				yPos=0;
				yOldPos = m_cRect.top;
				m_nIndex -= 2;
				m_rPs->MoveFirst();
			
				for(int j=0 ;j<m_nLen-m_nPage - m_nSize;j++){
					m_rPs->MoveNext();
				}

				m_nLen -=(m_nPage + m_nSize);
				
				
				varV = m_rPs->GetFields()->GetItem(0L)->Value;
				nprev = TimeStringToInt(varV,time);
				m_nPage = m_nSize;
			
				for(int i=0;i<pCurveList->GetSize();i++){
					curve=pCurveList->GetAt(i);
					track=pTrackList->GetAt(curve->nTrackIdx);
					ch.vt = VT_BSTR;
					ch.bstrVal = _bstr_t(curve->szMnemonic);
					varV = m_rPs->GetFields()->GetItem(ch)->Value;
					xOldPos[i]=DataToScr(varV.fltVal,curve,track,m_cRect.Width());

				}
				varV = m_rPs->GetFields()->GetItem(0L)->Value;
				nprev = TimeStringToInt(varV,time);
				Invalidate(true);
            }
            break;

        case SB_PAGEDOWN:
            {
			if(m_nLen >= (m_lRecordsNumber-1)) break;
			m_isPrinting=1;
			yPos=0;
			yOldPos = m_cRect.top;
				
			if((m_nIndex+1)*m_nPage < (m_lRecordsNumber-1)) Invalidate(true);
			else {		
				m_nPage  = m_lRecordsNumber-1 - m_nIndex*m_nPage;
				
					Invalidate(true);
				
			  }
            }
            break;

        case SB_PAGEUP:
            {
                
			if(m_nLen <= m_nSize) break;
			m_isPrinting=1;
                VARIANT varV,ch;
				int time[4];
                
				yPos=0;
				yOldPos = m_cRect.top;
				m_nIndex -= 2;
				m_rPs->MoveFirst();
			
				for(int j=0 ;j<m_nLen-m_nPage - m_nSize;j++){
					m_rPs->MoveNext();
				}

				m_nLen -=(m_nPage + m_nSize);
				
				
				varV = m_rPs->GetFields()->GetItem(0L)->Value;
				nprev = TimeStringToInt(varV,time);
				m_nPage = m_nSize;
			
				for(int i=0;i<pCurveList->GetSize();i++){
					curve=pCurveList->GetAt(i);
					track=pTrackList->GetAt(curve->nTrackIdx);
					ch.vt = VT_BSTR;
					ch.bstrVal = _bstr_t(curve->szMnemonic);
					varV = m_rPs->GetFields()->GetItem(ch)->Value;
					xOldPos[i]=DataToScr(varV.fltVal,curve,track,m_cRect.Width());

				}
				varV = m_rPs->GetFields()->GetItem(0L)->Value;
				nprev = TimeStringToInt(varV,time);
				Invalidate(true);
            }
            break;

        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            {
                
            }
            break;

        case SB_TOP:
            
            break;

        case SB_BOTTOM:
           

        default: break;
    }
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}





/*void CRelog::OnFilePrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your command handler code here
	
	CRect rect = pInfo->m_rectDraw;
	OnPaint();
	
}*/


void CRelog::OnFilePrint() 
{
	// TODO: Add your command handler code here
	CPrintInfo* pi;
	
	CPrintDialog dlgPrint(FALSE,PD_ALLPAGES,this);
	
	if(dlgPrint.DoModal() == IDOK){
		CDC dcPrint;
		
		dcPrint.Attach(dlgPrint.GetPrinterDC());
		CRect lR,lR1;
		
		
				
	

		
		// get the printer's pixel resolution
		int ixp=dcPrint.GetDeviceCaps(LOGPIXELSX);
		int iyp=dcPrint.GetDeviceCaps(LOGPIXELSY);

        // get the current window's resolution
		CDC* pddc = GetDC();
		int ixd=pddc->GetDeviceCaps(LOGPIXELSX);
		int iyd=pddc->GetDeviceCaps(LOGPIXELSY);
		lR.left = m_cRect.left;
		lR.bottom = m_cRect.bottom;
		// scale rect2 up by the ratio of the resolutions
		int width  = MulDiv(m_cRect.Width(),ixp, ixd);
		int height = MulDiv(m_cRect.Height(), iyp, iyd);
		lR.left = MulDiv(m_cRect.left, ixp, ixd);
		lR.top  = MulDiv(m_cRect.top,  ixp, ixd);
		lR.right  = lR.left + width;
		lR.bottom = lR.top + height;
				
		DOCINFO Info;	
		Info.cbSize = sizeof(Info);
		Info.lpszDocName = "Test Printer";
		Info.lpszOutput = NULL;
		Info.lpszDatatype = NULL;
		Info.fwType = NULL;
		
		if(dcPrint.StartDoc(&Info) >= 0){
			dcPrint.StartPage();
			
			dcPrint.DPtoLP(&lR);
			
			dcPrint.StretchBlt(lR.left,lR.top,lR.Width(),lR.Height(),&m_pDC,0,0,m_cRect.Width(),m_cRect.Height(),SRCCOPY);
						
			dcPrint.EndPage();
			dcPrint.EndDoc();
		}
		dcPrint.DeleteDC();
		
	/*	m_isPrinting=1;
        VARIANT varV,ch;
		int time[4];
                
		yPos=0;
		yOldPos = m_cRect.top;
		m_nIndex = 0;
		m_rPs->MoveFirst();
			
		m_nLen = m_nSize;
					
		varV = m_rPs->GetFields()->GetItem(0L)->Value;
		nprev = TimeStringToInt(varV,time);
		m_nPage = m_nSize;
			
		for(int i=0;i<pCurveList->GetSize();i++){
			curve=pCurveList->GetAt(i);
			track=pTrackList->GetAt(curve->nTrackIdx);
			ch.vt = VT_BSTR;
			ch.bstrVal = _bstr_t(curve->szMnemonic);
			varV = m_rPs->GetFields()->GetItem(ch)->Value;
			xOldPos[i]=DataToScr(varV.fltVal,curve,track,m_cRect.Width());
		}
		varV = m_rPs->GetFields()->GetItem(0L)->Value;
		nprev = TimeStringToInt(varV,time);*/
	
	}







}

void CRelog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame*	pMainFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CPanel* m_panel=(CPanel*)pMainFrm->m_wndSpMain.GetActivePane(0,0);
	
	
	m_panel->m_cPlayInDepth.EnableWindow(true);
	m_panel->m_cPlayInTime.EnableWindow(true);
	one=2;
	
	CDialog::OnClose();
}
