#include "stdafx.h"
#include "Threads.h"
#include "comm.h"
#include <math.h>
#include "Panel.h"
#include "Position.h"
#include "MainFrm.h"



#define MA1					0
#define HYDRAULIC_PRES		1
#define HYDRAULIC_TEMP		2
#define SAMPLE_PRES			3
#define SAMPLE_TEMP			4
#define MOTOR_VOLTAGE		5
#define PRETEST_POT			6
#define SIGNAL_REF			7
#define GR					8
#define GAUGE_REFMINUS		9
#define GAUGE_REFPLUS		10
#define MA12				11
#define MA13				12
#define MA14				13
#define MA15				14
#define POSITION			15
#define QUARZT_PRES_FREQ	16
#define QUARZT_TEMP_FREQ	17
#define QUARZT_PRES			18
#define QUARZT_TEMP			19



#include <process.h>


BOOL	bIsLogging=false;
BOOL	bStopFirmware=false;
BOOL	bIsSaving=false;
//int		nDirection=DIRECTION_DOWN;

//int		nLeftViewScale=SCALE_5MIN;
int		nRightUpperViewScale=SCALE_20M;
int		nRightLowerViewScale=SCALE_5MIN;
//int		nOldStartNum =1;
int		nOldStartNum =0;
//int		nTotalCurves=24;
int		nTotalCurves=NUMCHANNEL;

float		pTempData[NUMCHANNEL];

int		pTempDataALM[NUMCHANNEL];
int		pTempDataALM1[NUMCHANNEL];
int		pTempDataBK3[NUMCHANNEL];
int		pTempDataBK31[260];
int		pTempDataDL60[NUMCHANNEL];
int		pTempDataDL601[NUMCHANNEL];
int		pTempDataGK76[NUMCHANNEL];
int		pTempDataGK761[NUMCHANNEL];
int		pTempDataGK60[NUMCHANNEL];
int		pTempDataGK601[NUMCHANNEL];
int		pTempDataSKP[NUMCHANNEL];
int		pTempDataSKP1[NUMCHANNEL];
int		pTempDataMSC[NUMCHANNEL];
int		pTempDataMSC1[NUMCHANNEL];
int		pTempDataAK[NUMCHANNEL];

float	fTempDataMSC[NUMCHANNEL];

float	fTempDataSKP[NUMCHANNEL];

int		iSGK[260];
int		iBK[260];
int 	buft[12];

int		pTempDataDMT[NUMCHANNEL];


int		nGaugeHydraulicMin1=0;
int		nGaugeHydraulicMax1=100;
int		nGaugeHydraulicMin2=0;
int		nGaugeHydraulicMax2=100;
BOOL	bGaugeHydraulicLogarit=false;

int		nGaugeSampleMin1=0;
int		nGaugeSampleMax1=100;
int		nGaugeSampleMin2=0;
int		nGaugeSampleMax2=100;
BOOL	bGaugeSampleLogarit=false;

int		nQuartzPretestMin1=0;
int		nQuartzPretestMax1=100;
int		nQuartzPretestMin2=0;
int		nQuartzPretestMax2=100;
BOOL	bQuartzPretestLogarit=false;

int		nPositionMin=0;
int		nPositionMax=100,dmt_flag=0;
CString m_sDL60_msg,m_sALM_msg,m_sBK3_msg,m_sGK76_msg,m_sGK60_msg,m_sSKP_msg;

CFile *pcFile=NULL;
int end_flag=0;

DWORD dtoldcomm,dtcurcomm,gktold=0,gktcur=0,dtgk=0;

double	fEncoderConst=1000.0;
float WrapConvert(float dat, CCurveList *curve, CTrackList *track,int nScrWidth);
void CheckFrameDAQ(BYTE header,BYTE tailer,BYTE* str, int index);
float CalibChannel(float a,float rawm,float realm, float rawg, float realg, float rawp, float realp);								
double MergeChannel(double ain,int ch);
void mwait(long it);

void CheckFrameDMT(BYTE header,BYTE tailer,BYTE* str,int index);
void CheckFrameDMT_1(BYTE* str, int index);

void CheckFrameALM(BYTE* str, int index);
long CheckFrameALM_1(BYTE* str, int index);

void CheckFrameDL60(BYTE* str, int index);
long CheckFrameDL60_1(BYTE* str, int index);

void CheckFrameBK3(BYTE* str, int index);
long CheckFrameBK3_1(BYTE* str, int index);

void CheckFrameBK791(BYTE* str, int index);



void CheckFrameGK76(BYTE* str, int index);
long CheckFrameGK76_1(BYTE* str, int index);

void CheckFrameGK60(BYTE* str, int index);
long CheckFrameGK60_1(BYTE* str, int index);

void CheckFrameSGK901(BYTE* str, int index);

void CheckFrameSKP(BYTE* str, int index);
long CheckFrameSKP_1(BYTE* str, int index);

void CheckFrameMSC40(BYTE* str, int index);
long CheckFrameMSC40_1(BYTE* str, int index);

void CheckFrameAK(BYTE* str, int index);

void CheckFrameCopySD(BYTE* str);

void GetAllDataDMT();

RawData_t		gData;

RawData_t		gData1;
RawData_t		gData2;
RawData_t		gData_D;
RawData_t		RealData;

COLORREF	cr[8]={RGB(0,0,0),RGB(0,0,255),RGB(0,160,0),RGB(0,255,255),
					   RGB(255,0,0),RGB(255,0,255),RGB(255,160,0),RGB(192,0,0)};

//{RGB(0,0,0),RGB(255,0,0),RGB(0,255,0),RGB(0,0,255),RGB(255,0,255),RGB(0,255,255),
//					   RGB(255,255,0),RGB(255,128,160)};

//CDaoDatabase	m_pDB;
//CDaoRecordset	m_rs(&m_pDB);
//CDaoRecordset	m_Lrs(&m_pDB);
//CDaoDatabase	m_pDB1;
//CDaoRecordset	m_rRs1(&m_pDB1);
_RecordsetPtr m_rs;
ThreadInfo_t*	m_pthreadInfo2;
ThreadInfo_t*	m_pthreadInfo1;

CFile			*pFile=NULL;
int				nStartNum=0;
int Readflag=0;

UpdateInfo_t	info;
Calib_t fCalib[NUMCHANNEL];
LastInfoCalib_t pInfoCalib[NUMCHANNEL],lInfoCalib[NUMCHANNEL];
//--------------------------------------------------------
//
//			
//
//--------------------------------------------------------


UINT	View1Thread(LPVOID pParam)//Right Upper View
{
	ThreadInfo_t*	threadInfo=(ThreadInfo_t*)pParam;
	m_pthreadInfo1 = (ThreadInfo_t*)pParam;
	CWnd*			pWnd=threadInfo->pWnd;
	CRect*			pRect=threadInfo->pRect;	
	CRect*			pRectPart=threadInfo->pRectPart;
	CRect			gridRect;

	CDC*			pDC=pWnd->GetDC();
	CTrackListArray*	pTrackList=threadInfo->pTrackList;
	CCurveListArray*	pCurveList=threadInfo->pCurveList;

	DWORD			nStartTime=GetTickCount();
	DWORD			nCurTime=nStartTime;
	DWORD			nPrevTime=nStartTime;	
	CRect			rect;

	//pDC->FillRect(pRect,(CBrush*)pDC->SelectStockObject(WHITE_BRUSH));
	pDC->FillSolidRect(pRect,RGB(255,255,255));
	pWnd->Invalidate();

	int				nDepthMark=0;
	float			deltaPixel;
	float			yPos=0;
	float			yOldPos=0;
	int				idx=0;
	CPen			hPen[MAX_CURVE];
	float			xPos[MAX_CURVE],xOldPos[MAX_CURVE];
	CCurveList*		curve;
	CTrackList*		track;
	int i;

	//Create pen;
	for(i=0;i<pCurveList->GetSize();i++)
	{
		curve=pCurveList->GetAt(i);
		hPen[i].CreatePen(PS_SOLID,curve->nWidth,cr[curve->nColor]);
		//hPen[i].CreatePen(PS_SOLID,1,cr[curve->nColor]);
		
		if(curve->nTrackIdx<0)
			curve->nTrackIdx=0;
		if(curve->nTrackIdx>=pTrackList->GetSize())
			curve->nTrackIdx=pTrackList->GetSize()-1;
	}

//	while(gData.fDepth<=-999);
	
	float			fOldDepth=gData.fDepth;

	for(i=0;i<pCurveList->GetSize();i++)
	{
		curve=pCurveList->GetAt(i);
		track=pTrackList->GetAt(curve->nTrackIdx);
		xOldPos[i]=DataToScr(gData.fData[curve->nIdx],curve,track,pRect->Width());
	}
	

	while(bIsLogging)
	{
		nCurTime=GetTickCount();
		if(nCurTime-nPrevTime>TIMESLICE)
		{	
			deltaPixel=(gData.fDepth-fOldDepth)*pRect->Height()/nRightUpperViewScale;
			if(fabs(deltaPixel)>=1 && fabs(deltaPixel)<pRect->Height())
			{
				fOldDepth=gData.fDepth;	
				yPos=yOldPos+fabs(deltaPixel);
				//Draw curve 
				for(int i=0;i<pCurveList->GetSize();i++)
				{
					curve=pCurveList->GetAt(i);
					if(!curve->bActive) continue;
					track=pTrackList->GetAt(curve->nTrackIdx);
					xPos[i]=DataToScr(gData.fData[curve->nIdx],curve,track,pRect->Width());
					pDC->SelectObject(&hPen[i]);
					pDC->MoveTo(xOldPos[i],yOldPos);
					pDC->LineTo(xPos[i],yPos);
					xOldPos[i]=xPos[i];
				}
				//pDC->MoveTo(pRect->Height()*0.35,yOldPos);
				//pDC->LineTo(pRect->Height()*0.35,yPos);
				yOldPos=yPos;
			}
			else if(fabs(deltaPixel)>=pRect->Height())
			{
				fOldDepth=gData.fDepth;	
			}
				
			//Draw depth
			if(yPos>=nDepthMark*(pRect->bottom-pRect->top)/5.0)
			{
				
				for(int j=0;j<pTrackList->GetSize();j++)
				{
					CTrackList*	track;
					track=pTrackList->GetAt(j);
					if(track->bDepthTrack==false)	continue;
					CString		str;
					float		x;
					pDC->SetTextAlign(TA_TOP|TA_CENTER);
					x=(track->fLeftEdge+(track->fRightEdge-track->fLeftEdge)/2)*(pRect->right-pRect->left);
					str.Format("%.1f",gData.fDepth);
					pDC->TextOut(x,nDepthMark*(pRect->bottom-pRect->top)/5.0,str);
				}
				nDepthMark++;
			}

			//Scroll up	
			if(yPos>pRect->bottom)
			{
				pDC->BitBlt(0,0,pRectPart->Width(),pRectPart->Height(),
						pDC,0,pRect->bottom-pRectPart->Height(),SRCCOPY);
				rect.left=pRect->left;
				rect.right=pRect->right;
				rect.bottom=pRect->bottom;
				rect.top=pRectPart->Height();
				//pDC->FillRect(&rect,(CBrush*)pDC->SelectStockObject(WHITE_BRUSH));				
				pDC->FillSolidRect(&rect,RGB(255,255,255));
				gridRect.left=pRect->left;
				gridRect.right=pRect->right;
				gridRect.bottom=pRect->bottom;
				gridRect.top=int(0.6*(pRect->bottom-pRect->top));
				DrawGrid(pDC,2,pTrackList,&gridRect);	
				yPos=pRectPart->Height();
				yOldPos=yPos;
				nDepthMark=3;				
			}
				
			nPrevTime=nCurTime;
		}
	}

	//Delete pen
	for(i=0;i<pCurveList->GetSize();i++)
		hPen[i].DeleteObject();

	pWnd->ReleaseDC(pDC);
	
	return 0;

}



/*UINT	View2Thread(LPVOID pParam)//Right Lower View
{
	ThreadInfo_t*	threadInfo=(ThreadInfo_t*)pParam;
	

	CWnd*			pWnd=threadInfo->pWnd;
	CRect*			pRect=threadInfo->pRect;	
	CRect*			pRectPart=threadInfo->pRectPart;
	CRect			gridRect;

	CDC*			pDC=pWnd->GetDC();
	CTrackListArray*	pTrackList=threadInfo->pTrackList;
	CCurveListArray*	pCurveList=threadInfo->pCurveList;

	DWORD			nStartTime=GetTickCount();
	DWORD			nCurTime=nStartTime;
	DWORD			nPrevTime=nStartTime;	
	CRect			rect;

	//pDC->FillRect(pRect,(CBrush*)pDC->SelectStockObject(WHITE_BRUSH));
	pDC->FillSolidRect(pRect,RGB(255,255,255));
	pWnd->Invalidate();

	int				nDepthMark=0;
	float			deltaPixel;
	float			yPos=0;
	float			yOldPos=0;
	int				idx=0;
	CPen			hPen[MAX_CURVE];
	float			xPos[MAX_CURVE],xOldPos[MAX_CURVE];
	CCurveList*		curve;
	CTrackList*		track;

	//Create pen;
	for(int i=0;i<pCurveList->GetSize();i++)
	{
		curve=pCurveList->GetAt(i);
		//hPen[i].CreatePen(PS_SOLID,curve->nWidth,cr[curve->nColor]);
		hPen[i].CreatePen(PS_SOLID,1,cr[curve->nColor]);
		
		if(curve->nTrackIdx<0)
			curve->nTrackIdx=0;
		if(curve->nTrackIdx>=pTrackList->GetSize())
			curve->nTrackIdx=pTrackList->GetSize()-1;
	}

	while(gData.fDepth<=-999);
	
	//float			fOldDepth=gData.fDepth;

	for(i=0;i<pCurveList->GetSize();i++)
	{
		curve=pCurveList->GetAt(i);
		track=pTrackList->GetAt(curve->nTrackIdx);
		xOldPos[i]=DataToScr(gData.fData[curve->nIdx],curve,track,pRect->Width());
	}
	
	DWORD		nTime1;
	DWORD		nTime2;

	nTime1=GetTickCount();
	while(bIsLogging)
	{
		nCurTime=GetTickCount();
		nTime2=GetTickCount();
		if(nCurTime-nPrevTime>TIMESLICE)
		{	
			//deltaPixel=(gData.fDepth-fOldDepth)*pRect->Height()/nRightUpperViewScale;
			deltaPixel=((nTime2-nTime1)/1000.0)*pRect->Height()/nRightLowerViewScale;
			if(fabs(deltaPixel)>=1 && fabs(deltaPixel)<pRect->Height())
			{
				//fOldDepth=gData.fDepth;	
				nTime1=nTime2;
				yPos=yOldPos+fabs(deltaPixel);
				//Draw curve 
				for(i=0;i<pCurveList->GetSize();i++)
				{
					curve=pCurveList->GetAt(i);
					track=pTrackList->GetAt(curve->nTrackIdx);
					xPos[i]=DataToScr(gData.fData[curve->nIdx],curve,track,pRect->Width());
					pDC->SelectObject(&hPen[i]);
					pDC->MoveTo(xOldPos[i],yOldPos);
					pDC->LineTo(xPos[i],yPos);
					xOldPos[i]=xPos[i];
				}
				yOldPos=yPos;
			}
			
				
			//Draw depth
			if(yPos>=nDepthMark*(pRect->bottom-pRect->top)/5.0)
			{
				
				for(int j=0;j<pTrackList->GetSize();j++)
				{
					CTrackList*	track;
					track=pTrackList->GetAt(j);
					if(track->bDepthTrack==false)	continue;
					CString		str;
					float		x;
					int			nSeconds;
					nSeconds=int(float(nCurTime-nStartTime)/1000.0);
					pDC->SetTextAlign(TA_TOP|TA_CENTER);
					x=(track->fLeftEdge+(track->fRightEdge-track->fLeftEdge)/2)*(pRect->right-pRect->left);
					//str.Format("%.1f",gData.fDepth);
					str.Format("%d:%02d",nSeconds/60, nSeconds%60);
					pDC->TextOut(x,nDepthMark*(pRect->bottom-pRect->top)/5.0,str);
				}
				nDepthMark++;
			}

			//Scroll up	
			if(yPos>pRect->bottom)
			{
				pDC->BitBlt(0,0,pRectPart->Width(),pRectPart->Height(),
						pDC,0,pRect->bottom-pRectPart->Height(),SRCCOPY);
				rect.left=pRect->left;
				rect.right=pRect->right;
				rect.bottom=pRect->bottom;
				rect.top=pRectPart->Height();
				//pDC->FillRect(&rect,(CBrush*)pDC->SelectStockObject(WHITE_BRUSH));				
				pDC->FillSolidRect(&rect,RGB(255,255,255));
				gridRect.left=pRect->left;
				gridRect.right=pRect->right;
				gridRect.bottom=pRect->bottom;
				gridRect.top=int(0.6*(pRect->bottom-pRect->top));
				DrawGrid(pDC,2,pTrackList,&gridRect);	
				yPos=pRectPart->Height();
				yOldPos=yPos;
				nDepthMark=3;				
			}					
				
			nPrevTime=nCurTime;
		}
	}

	//Delete pen
	for(i=0;i<pCurveList->GetSize();i++)
		hPen[i].DeleteObject();

	pWnd->ReleaseDC(pDC);
	return 0;
}*/

UINT View2Thread(LPVOID pParam)//Right Lower View
{
	m_pthreadInfo2 = (ThreadInfo_t*)pParam;
	static int nfirst=0;
	static int count = 0;
	CWnd*			pWnd=m_pthreadInfo2->pWnd;
	CRect*			pRect=m_pthreadInfo2->pRect;	
	CRect*			pRectPart=m_pthreadInfo2->pRectPart;
	CRect			gridRect;
	
	

	CDC*			pDC=pWnd->GetDC();
	CTrackListArray*	pTrackList=m_pthreadInfo2->pTrackList;
	CCurveListArray*	pCurveList=m_pthreadInfo2->pCurveList;

	DWORD			nStartTime=GetTickCount();
	DWORD			nCurTime=nStartTime;
	DWORD			nPrevTime=nStartTime;	
	CRect			rect;

	pDC->FillRect(pRect,(CBrush*)pDC->SelectStockObject(WHITE_BRUSH));
	pWnd->Invalidate();

	

	int				nDepthMark=0;
	float			deltaPixel;
	float			yPos=0;
	float			yOldPos = pRect->top;
	int				idx=0;
	CPen			hPen[MAX_CURVE];
	float			xPos[MAX_CURVE],xOldPos[MAX_CURVE];
	CCurveList*		curve;
	CTrackList*		track;
	CString strOutput;
	SYSTEMTIME		systime;
	int i;
	//Create pen;


 	for(i=0;i<pCurveList->GetSize();i++)
	{
		curve=pCurveList->GetAt(i);
		curve->Pen.CreatePen(curve->nStyle,curve->nWidth,cr[curve->nColor]);
		
		if(curve->nTrackIdx<0)
			curve->nTrackIdx=0;
		if(curve->nTrackIdx>=pTrackList->GetSize())
			curve->nTrackIdx=pTrackList->GetSize()-1;
	}

//	while(gData.fDepth<=-999);
	
	for(i=0;i<pCurveList->GetSize();i++)
	{
		curve=pCurveList->GetAt(i);
		track=pTrackList->GetAt(curve->nTrackIdx);
		xOldPos[i]=DataToScr(gData.fData[curve->nIdx],curve,track,pRect->Width());
	}
	
	DWORD		nTime1;
	DWORD		nTime2;

	nTime1=GetTickCount();
	while(bIsLogging)
	{
		nCurTime=GetTickCount();
		nTime2=GetTickCount();
		if(nCurTime-nPrevTime>TIMESLICE)
		{	
			
			deltaPixel=((nTime2-nTime1)/1000.0)*pRect->Height()/nRightLowerViewScale;
			if(fabs(deltaPixel)>=1 && fabs(deltaPixel)<pRect->Height())
			{
			
				nTime1=nTime2;
				yPos=yOldPos+fabs(deltaPixel);
				//Draw curve 
				for(int i=0;i<pCurveList->GetSize();i++)
				{
					curve=pCurveList->GetAt(i);
					//if(!curve->nIdx) curve->nIdx = 8;// test
					if(!curve->bActive) continue;
					track=pTrackList->GetAt(curve->nTrackIdx);
					
					xPos[i]=DataToScr(gData.fData[curve->nIdx],curve,track,pRect->Width());
					if((curve->bIsOldWrap != curve->bIsWrap)&&(nfirst != 0)){
						xOldPos[i]= WrapConvert(gData.fData[curve->nIdx], curve,track,pRect->Width());
					}
					pDC->SelectObject(&curve->Pen);
					pDC->MoveTo(xOldPos[i],yOldPos);
					pDC->LineTo(xPos[i],yPos);
					xOldPos[i]=xPos[i];
					curve->bIsOldWrap = curve->bIsWrap;
					nfirst = 1;
				}
				yOldPos=yPos;
				
			}
			
				
			//Draw depth
			if(yPos>=nDepthMark*(pRect->bottom-pRect->top)/5.0)
			{
				
				for(int j=0;j<pTrackList->GetSize();j++)
				{
					CTrackList*	track;
					track=pTrackList->GetAt(j);
					if(track->bDepthTrack==false)	continue;
					CString		str;
					float		x;
					int			nSeconds;
					nSeconds=int(float(nCurTime-nStartTime)/1000.0);
					//pDC->SetTextColor(RGB(0,0,0));
					pDC->SetTextAlign(TA_TOP|TA_CENTER);
					x=(track->fLeftEdge+(track->fRightEdge-track->fLeftEdge)/2)*(pRect->right-pRect->left);
					str.Format("%d:%02d",nSeconds/60, nSeconds%60);
					pDC->TextOut(x,nDepthMark*(pRect->bottom-pRect->top)/5.0,str);
				}
				nDepthMark++;
			}

			//Scroll up	
			if(yPos>pRect->bottom)
			{
				pDC->BitBlt(0,0,pRectPart->Width(),pRectPart->Height(),
						pDC,0,pRect->bottom-pRectPart->Height(),SRCCOPY);
				rect.left=pRect->left;
				rect.right=pRect->right;
				rect.bottom=pRect->bottom;
				rect.top=pRectPart->Height();
				pDC->FillRect(&rect,(CBrush*)pDC->SelectStockObject(WHITE_BRUSH));				

				gridRect.left=pRect->left;
				gridRect.right=pRect->right;
				gridRect.bottom=pRect->bottom;
				gridRect.top=int(0.6*(pRect->bottom-pRect->top));
				DrawGrid(pDC,2,pTrackList,&gridRect);
				yPos=pRectPart->Height();
				yOldPos=yPos;
				nDepthMark=3;				
			}					
				
			nPrevTime=nCurTime;
			
		}
	}

	//Delete pen
	for(i=0;i<pCurveList->GetSize();i++){
		
		curve=pCurveList->GetAt(i);
		curve->Pen.DeleteObject();
	}

	pWnd->ReleaseDC(pDC);
	
	return 0;
}

UINT	WriteDataThread(LPVOID pParam)
{	
	
	DWORD			nStartTime=GetTickCount();
	DWORD			nCurTime=nStartTime;
	DWORD			nPrevTime=nStartTime;
	
	CString			str;
	CString			strOutput;
	SYSTEMTIME		systime;
	int				i;
	
	m_pCurveList = m_pthreadInfo2->pCurveList;
	//m_pCurveList = m_pThreadInfo->pCurveList;
	//CCurveListArray*	pCurveList=m_pthreadInfo->pCurveList;
	CCurveList*		curve;


	
	while(bIsLogging & bIsSaving)
	{
		nCurTime=GetTickCount();
		if(nCurTime-nPrevTime>TIMESLICE){		
			GetLocalTime(&systime);	
			strOutput.Format("%02d:%02d:%02d.03%d     %f      ",
				systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds,gData.fDepth);
			

			for(i=0;i<m_pCurveList->GetSize();i++){
				curve=m_pCurveList->GetAt(i);
				
				str.Format("%f    ",gData.fData[curve->nIdx]);
				strOutput+=str;
			}
			
			
			strOutput+="\r\n";
			pFile->Write(strOutput,strOutput.GetLength());

					
			strOutput.Format("%d:%d:%d.%d",
				systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds);





			_variant_t index, varV;

			//COleDateTime vtime;
			
			//vtime.GetAsSystemTime(&systime);

			index.vt = VT_I2;
			m_rs->AddNew();
			
			index.iVal = 0;
			varV.vt = VT_BSTR;
			varV.bstrVal = _bstr_t(strOutput);
			m_rs->Fields->GetItem(index)->PutValue(varV);

			



			index.iVal = 1;
			//strOutput.Format("%.2f",gData.fDepth);
			//varV.bstrVal = _bstr_t(strOutput);
			varV.vt = VT_R4;
			varV.fltVal = gData.fDepth;

			m_rs->Fields->GetItem(index)->PutValue(varV);

			for(i=0;i<m_pCurveList->GetSize();i++){	
				curve = m_pCurveList->GetAt(i);
				//index.iVal = curve->nIdx;
				varV.vt = VT_R4;
				varV.fltVal = gData.fData[m_pCurveList->GetAt(i)->nIdx];
				m_rs->Fields->GetItem(pInfoCalib[curve->nIdx].szNameChannel)->PutValue(varV);
				VariantClear(&varV);

			
			}
			
			m_rs->Update();
		/*	COleVariant		vTime;			
			vTime.SetString(strOutput,VT_BSTRT);

			m_rs.AddNew();
			m_rs.SetFieldValue("Time",vTime);
			
			COleVariant		vDepth(gData.fDepth);			
			m_rs.SetFieldValue("Depth",vDepth);
			m_rs.SetFieldValue("Time",vTime);


			for(i=0;i<m_pCurveList->GetSize();i++){	
				curve = m_pCurveList->GetAt(i);
				COleVariant	vtemp(gData.fData[m_pCurveList->GetAt(i)->nIdx]);
				
				m_rs.SetFieldValue(pInfoCalib[curve->nIdx].szNameChannel,vtemp);
			}
										
				

			m_rs.Update();*/
			
			
			nPrevTime=nCurTime;
		}
	}
	nStartNum++;
	
	return 0;
}

//------------------------------------------------
//
//			READ DATA FROM FILE
//
//--------------------------------------------------

int GetCurveNum(char str[])
{
	int nCount=0,i=0;
	while(str[i]==' ' || str[i]=='\t') i++;
	while(1)
	{
		while(str[i]=='.' || str[i]=='-' || str[i]=='+' ||
			(str[i]>='0' && str[i]<='9')) i++;
		nCount++;
		while(str[i]==' ' || str[i]=='\t') i++;
		if(str[i]=='\r' || str[i]=='\n' || str[i]=='\0') break;
		if(str[i]!='.' && str[i]!='-' && str[i]!='+' &&
			(str[i]<'0' || str[i]>'9')) break;
	}
	return nCount-1;
}
int ParseData(RawData_t &Data,char	str[],int nCurNum)
{
	char	szValue[40];
	int		index=0,id=0;
	int		len;
	
	len=strlen(str);
	while(str[index]==' ' || str[index]=='\t') index++;

	for(int j=0;j<=nCurNum;j++)
	{
		id=0;
		while(str[index]!=' ' && str[index]!='\t' && str[index]!='\r' &&
			str[index]!='\n' && index<len)
		{
			szValue[id++]=str[index++];
		}
		szValue[id]=0;
		if(j==0)	Data.fDepth=atof(szValue);
		else		Data.fData[j-1]=atof(szValue);
	
		if(index>=len-1)	break;

		if(str[index]=='\r' || str[index]=='\n' || str[index]=='\0') break;

		while(str[index]==' ' || str[index]=='\t') index++;
		if(index>=len-1)	break;
	}
	return 1;
}

UINT	ReadDataThread(LPVOID pParam)
{

ThreadInfo_t*	threadInfo=(ThreadInfo_t*)pParam;

	CWnd*			pWnd=threadInfo->pWnd;
	CRect*			pRect=threadInfo->pRect;	
	CRect*			pRectPart=threadInfo->pRectPart;
	CRect			gridRect;

	CDC*			pDC=pWnd->GetDC();
	CTrackListArray*	pTrackList=threadInfo->pTrackList;
	CCurveListArray*	pCurveList=threadInfo->pCurveList;

	DWORD			nStartTime=GetTickCount();
	DWORD			nCurTime=nStartTime;
	DWORD			nPrevTime=nStartTime;	
	CRect			rect;

	//pDC->FillRect(pRect,(CBrush*)pDC->SelectStockObject(WHITE_BRUSH));
	pDC->FillSolidRect(pRect,RGB(255,255,255));
	pWnd->Invalidate();

	int				nDepthMark=0;
	float			deltaPixel;
	float			yPos=0;
	float			yOldPos=0;
	int				idx=0,i;
	CPen			hPen[MAX_CURVE];
	float			xPos[MAX_CURVE],xOldPos[MAX_CURVE];
	CCurveList*		curve;
	CTrackList*		track;

	//Create pen;
	for(i=0;i<pCurveList->GetSize();i++)
	{
		curve=pCurveList->GetAt(i);
		//hPen[i].CreatePen(PS_SOLID,curve->nWidth,cr[curve->nColor]);
		hPen[i].CreatePen(PS_SOLID,1,cr[curve->nColor]);
		
		if(curve->nTrackIdx<0)
			curve->nTrackIdx=0;
		if(curve->nTrackIdx>=pTrackList->GetSize())
			curve->nTrackIdx=pTrackList->GetSize()-1;
	}

//	while(gData.fDepth<=-999);
	
	float			fOldDepth=gData.fDepth;

	for(i=0;i<pCurveList->GetSize();i++)
	{
 		curve=pCurveList->GetAt(i);
		track=pTrackList->GetAt(curve->nTrackIdx);
		xOldPos[i]=DataToScr(gData.fData[curve->nIdx],curve,track,pRect->Width());
	}
	

	while(bIsLogging)
	{
		nCurTime=GetTickCount();
		if(nCurTime-nPrevTime>TIMESLICE)
		{	
			deltaPixel=(gData.fDepth-fOldDepth)*pRect->Height()/nRightUpperViewScale;
			if(fabs(deltaPixel)>=1 && fabs(deltaPixel)<pRect->Height())
			{
				fOldDepth=gData.fDepth;	
				yPos=yOldPos+fabs(deltaPixel);
				//Draw curve 
				for(i=0;i<pCurveList->GetSize();i++)
				{
					curve=pCurveList->GetAt(i);
					track=pTrackList->GetAt(curve->nTrackIdx);
					xPos[i]=DataToScr(gData.fData[curve->nIdx],curve,track,pRect->Width());
					pDC->SelectObject(&hPen[i]);
					pDC->MoveTo(xOldPos[i],yOldPos);
					pDC->LineTo(xPos[i],yPos);
					xOldPos[i]=xPos[i];
				}
				//pDC->MoveTo(pRect->Height()*0.35,yOldPos);
				//pDC->LineTo(pRect->Height()*0.35,yPos);
				yOldPos=yPos;
			}
			else if(fabs(deltaPixel)>=pRect->Height())
			{
				fOldDepth=gData.fDepth;	
			}
				
			//Draw depth
			if(yPos>=nDepthMark*(pRect->bottom-pRect->top)/5.0)
			{
				
				for(int j=0;j<pTrackList->GetSize();j++)
				{
					CTrackList*	track;
					track=pTrackList->GetAt(j);
					if(track->bDepthTrack==false)	continue;
					CString		str;
					float		x;
					pDC->SetTextAlign(TA_TOP|TA_CENTER);
					x=(track->fLeftEdge+(track->fRightEdge-track->fLeftEdge)/2)*(pRect->right-pRect->left);
					str.Format("%.1f",gData.fDepth);
					pDC->TextOut(x,nDepthMark*(pRect->bottom-pRect->top)/5.0,str);
				}
				nDepthMark++;
			}

			//Scroll up	
			if(yPos>pRect->bottom)
			{
				pDC->BitBlt(0,0,pRectPart->Width(),pRectPart->Height(),
						pDC,0,pRect->bottom-pRectPart->Height(),SRCCOPY);
				rect.left=pRect->left;
				rect.right=pRect->right;
				rect.bottom=pRect->bottom;
				rect.top=pRectPart->Height();
				//pDC->FillRect(&rect,(CBrush*)pDC->SelectStockObject(WHITE_BRUSH));				
				pDC->FillSolidRect(&rect,RGB(255,255,255));
				gridRect.left=pRect->left;
				gridRect.right=pRect->right;
				gridRect.bottom=pRect->bottom;
				gridRect.top=int(0.6*(pRect->bottom-pRect->top));
				DrawGrid(pDC,2,pTrackList,&gridRect);	
				yPos=pRectPart->Height();
				yOldPos=yPos;
				nDepthMark=3;				
			}
				
			nPrevTime=nCurTime;
		}
	}

	//Delete pen
	for(i=0;i<pCurveList->GetSize();i++)
		hPen[i].DeleteObject();

	pWnd->ReleaseDC(pDC);
	return 0;
}

void DrawGrid(CDC *pDC,int nDivNum,CTrackListArray *trackList,CRect *rViewport)
{
	CPen hBPen,hGPen,hMPen,*hOldPen;
	hBPen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	hMPen.CreatePen(PS_SOLID,1,RGB(50,50,50));
	hGPen.CreatePen(PS_SOLID,1,RGB(150,150,150));
	hOldPen=pDC->SelectObject(&hBPen);

	float	fXScale;
	int		top,bottom;
	float   height,width;
	int		i,j,k;
	fXScale=rViewport->right-rViewport->left;
	top=rViewport->top;bottom=rViewport->bottom;
	
	CTrackList*	track;
	//Draw track
	for(i=0;i<trackList->GetSize();i++)
	{
		track=trackList->GetAt(i);
		//Draw boundary
		pDC->SelectObject(&hBPen);
		pDC->MoveTo(track->fLeftEdge*fXScale,top);
		pDC->LineTo(track->fLeftEdge*fXScale,bottom);
		pDC->MoveTo(track->fRightEdge*fXScale,top);
		pDC->LineTo(track->fRightEdge*fXScale,bottom);
		//Draw vertical grids
		if(track->bVertOn)
		{
			int nCenter=-1;

			pDC->SelectObject(&hGPen);
			if(track->nVType==LINEAR && track->nDivisionNum>0)
			{
				if(track->nDivisionNum % 2 ==0)
					nCenter=track->nDivisionNum/2;
				width=track->fRightEdge*fXScale;
				width-=track->fLeftEdge*fXScale;
				width/=track->nDivisionNum;
				for(j=1;j<=track->nDivisionNum-1;j++)
				{
					if(nCenter>0 && j==nCenter)
						pDC->SelectObject(&hMPen);
					else
						pDC->SelectObject(&hGPen);
					pDC->MoveTo(track->fLeftEdge*fXScale+width*j,top);
					pDC->LineTo(track->fLeftEdge*fXScale+width*j,bottom);
				}
			}
			else if(track->nVType==LOGARIT && track->nDecadesNum>0)
			{	
				float ftemp;
				float decWidth[10],logWidth[9];				
				float xpos;
				ftemp=track->fRightEdge-track->fLeftEdge;
				ftemp/=track->nDecadesNum;
				for(j=0;j<10;j++)
					decWidth[j]=ftemp*log10((float)j+1);
				for(j=0;j<9;j++)
					logWidth[j]=decWidth[j+1]-decWidth[j];
				k=track->nSCycle-1;
				xpos=track->fLeftEdge*fXScale;
				for(j=1;j<9*track->nDecadesNum;j++,k++)
				{
					if((k+1)%9==0)
						pDC->SelectObject(&hMPen);
					else
						pDC->SelectObject(&hGPen);
					xpos+=(logWidth[k%9]*fXScale);
					pDC->MoveTo(xpos,top);
					pDC->LineTo(xpos,bottom);
				}
			}
		}
	}
	pDC->SelectObject(&hBPen);
	pDC->MoveTo(rViewport->right-1,rViewport->top);
	pDC->LineTo(rViewport->right-1,rViewport->bottom);
	//Draw depth & horizontal grids
	float	yPos;
	height=bottom-top;height/=(nDivNum*5);
	yPos=top;
	for(i=0;i<nDivNum*5;i++)
	{	
		for(int j=0;j<trackList->GetSize();j++)
		{
			track=trackList->GetAt(j);
			if(track->bHorzOn==false)	continue;
			if(i % 5 == 0)
				pDC->SelectObject(&hMPen);
			else
				pDC->SelectObject(&hGPen);
			pDC->MoveTo(track->fLeftEdge*fXScale,int(yPos));
			pDC->LineTo(track->fRightEdge*fXScale,int(yPos));
		}
		yPos+=height;
	}
	/*pDC->SelectObject(&hBPen);
	pDC->MoveTo(rViewport->left,rViewport->bottom-1);
	pDC->LineTo(rViewport->right,rViewport->bottom-1);*/
	
	pDC->SelectObject(hOldPen);
	DeleteObject(hBPen);
	DeleteObject(hMPen);
	DeleteObject(hGPen);
}
float WrapConvert(float dat, CCurveList *curve, CTrackList *track,int nScrWidth)
{
	
		if(curve->fRightScale > curve->fLeftScale)
		{
			
			if(dat > curve->fRightScale) 	return track->fLeftEdge*nScrWidth;
							
			
			if(dat < curve->fLeftScale)		return track->fRightEdge*nScrWidth;

			
						

		
		}
		else
		{
			if(dat < curve->fRightScale) 	return track->fLeftEdge*nScrWidth;
				
			
			if(dat > curve->fLeftScale) 	return track->fRightEdge*nScrWidth;
		
		}

		return DataToScr(dat, curve, track,nScrWidth);
	
}
float DataToScr(float dat, CCurveList *curve, CTrackList *track, int nScrWidth)
{	
	float		Left,Right;
	float		fLeft,fRight;
	float		AFac,BFac;
	int			i,j;
	//int			lRebase=curve.nLeftRebase;
	//int			rRebase=curve.nRightRebase;
	if(!curve->bActive) return 0;
	
	
	float	width;
	float	curWidth;
	
	//wrap=0; 

	if(curve->nScale==LINEAR)
	{
		//Calculate
		if(curve->fRightScale>curve->fLeftScale)
		{
			//if(dat>curve->fRightScale) return (track->fRightEdge*nScrWidth);

			if(dat > curve->fRightScale){
			
				while(dat > curve->fRightScale) {
					dat -= curve->fRightScale;
				}
				
			}
					
			
			

			if(dat < curve->fLeftScale){
			
				while(dat < curve->fLeftScale) {
					dat += dat < curve->fLeftScale;
				}
								
			}


			//if(dat<curve->fLeftScale)  return (track->fLeftEdge*nScrWidth);
			fLeft=curve->fLeftScale;
			fRight=curve->fRightScale;
			Left=track->fLeftEdge*nScrWidth;
			Right=track->fRightEdge*nScrWidth;

			AFac=Right-Left;AFac/=(fRight-fLeft);
			BFac=Right-AFac*fRight;
		}
		else
		{
			//if(dat<curve->fRightScale) return (track->fRightEdge*nScrWidth);
			//if(dat>curve->fLeftScale)  return (track->fLeftEdge*nScrWidth);


			if(dat < curve->fRightScale){
			
				while(dat > curve->fRightScale) {
					dat += curve->fRightScale;
				}
				
			}
					
			
			

			if(dat > curve->fLeftScale){
			
				while(dat < curve->fLeftScale) {
					dat -= dat < curve->fLeftScale;
				}
								
			}


			fLeft=curve->fLeftScale;
			fRight=curve->fRightScale;
			//Left=track->fLeftEdge;
			//Right=track->fRightEdge;
			Left=track->fLeftEdge*nScrWidth;
			Right=track->fRightEdge*nScrWidth;


			AFac=Right-Left;
			AFac/=(fRight-fLeft);
			BFac=Left-AFac*fLeft;
		}


		//AFac=Right-Left;AFac/=(fRight-fLeft);
		//BFac=Right-AFac*fRight;
		return (AFac*dat+BFac);
	}
	else if(curve->nScale==LOGARIT)
	{
		//process rebase
		if(dat<=0) return (track->fLeftEdge*nScrWidth);
		float	ftemp;
		if(dat>curve->fRightScale) return (track->fRightEdge*nScrWidth);
		if(dat<curve->fLeftScale)  return (track->fLeftEdge*nScrWidth);		
		
		Left=track->fLeftEdge*nScrWidth;
		Right=track->fRightEdge*nScrWidth;
		fLeft=log10(curve->fLeftScale);
		fRight=log10(curve->fRightScale);
		AFac=Right-Left;AFac/=(fRight-fLeft);
		BFac=Right-AFac*fRight;
		return AFac*log10(dat)+BFac;
	}
}
int IsFileExist(char szFileName[])
{
	WIN32_FIND_DATA FindFileData;
	HANDLE			hFind;
	hFind=FindFirstFile(szFileName,&FindFileData);
	if(hFind==INVALID_HANDLE_VALUE) 
		return 0;
	return 1;
}
//---------------------------------------------------
//
//
//			COMM SECTION
//
//----------------------------------------------------
double coef[22]={12.585991,-1.1767254,-0.019036789,0.0000079483458,
			38.672087,-0.020535625,0.000023126341,-0.000000056544201,
			-0.0018490686,0.000010424942,-0.000000021365328,0.000000000079819019,
			0.00000055379881,-0.0000000031065980,0.000000000015640604,-0.00000000000003984891,
			25.11307,-0.74824963,-0.00091090794,-0.00000074765194,
			25632,50520};
double coef1[22]={13.124285,-0.1427102,-0.019754817,0.00001043726,
			40.772732,-0.021205066,0.000021875155,-0.00000007253378,
			-0.0017614899,0.0000113349394,-0.000000011532663,0.00000000013078242,
			0.00000059808637,-0.000000003782507,0.0000000000038160471,-0.00000000000010032947,
			25.115654,-0.74851051,-0.00091070947,-0.00000075148322,
			20345,51892};



double inc_101[48] ={
			-2.5361e-4,4.0528e-6,-5.5987e-8,5.3018e-10, //bhxi
			1.5314e-4,5.2353e-6,-1.2896e-7,6.2133e-10,	//bhyi
			4.5365e-4,-1.7721e-5,4.9542e-8,2.6592e-10,	//bhzi
			4.9959,-7.1294e-5,3.8275e-6,-1.7165e-8,		//shxi
			4.9962,7.8239e-5,1.7857e-6,-1.0358e-8,		//shyi
			4.9938,7.3878e-5,3.7398e-6,-1.9665e-8,		//shzi
			3.2122e-3,1.6669e-5,-1.888e-8,1.1843e-10,	//bgxi
			-3.337e-3,1.3318e-5,-4.150e-8,1.8785e-10,	//bgy3
			4.8165e-3,9.6275e-6,-2.1579e-7,9.8085e-10,	//bgz3
			4.5378,1.9263e-4,3.2828e-6,-9.0599e-9,		//sgx3
			4.5388,2.1739e-4,2.8427e-6,-7.5246e-9,		//sgy3
			4.5396,2.5762e-4,1.5343e-6,-5.2542e-10		//sgz3
};

double inc_102[48]={
			6.9495e-4,-2.5828e-6,2.4014e-8,7.8055e-11,
			3.6209e-4,1.0833e-5,-1.9280e-7,1.1557e-09,
			3.1883e-4,-3.0454e-6,-9.0374e-8,7.2506e-10,
			4.9974,1.1473e-4,4.3177e-7,-3.9621e-9,
			4.9943,2.418e-4,9.9006e-7,-8.5675e-9,
			4.9913,3.7994e-4,-7.9406e-8,-7.4984e-9,
			5.4022e-3,3.2552e-6,-5.0642e-8,2.4422e-10,
			-7.1426e-3,3.4455e-6,2.1519e-8,8.6704e-11,
			2.8108e-3,1.0391e-5,3.321e-8,-1.674e-10,
			4.5391,2.7667e-4,1.8154e-7,4.2477e-9,
			4.5349,4.2881e-4,-3.814e-8,4.1027e-9,
			4.5404,2.4155e-4,6.0996e-7,2.212e-9
};

double dic_105[48] = {
6.2567e-4,
-3.5311e-5,
1.8701e-7,
7.1686e-11,
-5.6459e-4,
9.5626e-6,
-1.9280e-7,
1.1557e-09,
3.1883e-4,
-3.0454e-6,
-9.0374e-8,
7.5339e-10,
4.9998,
2.2616e-5,
1.9235e-6,
-9.5794e-9,
5.0048,
-9.4124e-5,
3.8656e-6,
-1.8749e-8,
5.0005,
-8.6146e-6,
3.6884e-6,
-1.9663e-8,
-4.4988e-3,
3.3898e-5,
2.7577e-8,
-4.4373e-10,
5.9076e-3,
1.0339e-5,
4.8446e-8,
-1.4461e-10,
-4.2374e-3,
-1.1950e-6,
-3.5968e-8,
2.8023e-10,
4.5341,
4.7221e-4,
-4.7936e-7,
5.5682e-9,
4.5341,
4.6694e-4,
-3.3343e-7,
5.4888e-9,
4.5380,
3.6676e-4,
3.7681e-7,
1.9639e-9
};

double dic_106[48] = {
6.1299e-5,
-3.43060e-6,
1.0073e-7,
-5.1631e-10,
4.3128e-4,
-2.9031e-5,
1.7038e-7,
-1.2584e-10,
1.0802e-4,
-1.2687e-6,
-4.5173e-8,
4.9224e-10,
5.0017,
8.1546e-6,
1.5877e-6,
-8.4076e-9,
5.0088,
-1.1495e-4,
2.5365e-6,
-1.3328e-8,
5.0024,
-3.3159e-5,
3.1596e-6,
-1.5959e-8,
5.49740e-3,
1.8298e-5,
-5.4994e-8,
3.2411e-10,
-2.5954e-3,
7.0504e-6,
1.1708e-7,
-4.3933e-10,
4.0174e-3,
2.0139e-5,
-7.8767e-8,
3.8733e-10,
4.5356,
4.2294e-4,
-4.0981e-7,
6.1677e-9,
4.5370,
3.9956e-4,
-2.5820e-7,
5.7628e-9,
4.5362,
3.7680e-4,
4.0878e-7,
2.7146e-9
};

//float	Motor_Tab[12]={0,0,65,4.72,105,6.31,112,8.5,115,8.9,125,9.5};
float	Motor_Tab[12]={0,0,31,1.2,65,2.5,120,4.7,143,5.6,172,6.8};

int		tempBuf[NUMCHANNEL];
int		ofs = 0;
int		nReadByte,PauseFlag=1;
int iDepth=0;
BYTE	BufTest[68];
BYTE	BufData[68];
int		IgniteFlag=1;
int		CloseFlag=0;
SerialComm	m_cComm;

int			Index = 0,ReadyFlag=0,filter = 5;
int			DepthFlag=0;
bool		bSendOK=false;
int			nSetDepth;
int nATension,nBTension;
int nEncoder,nNumChannel=NUMCHANNEL;

//int mbALM=1, mbDL60=0, mbBK3=0, mbGK76=0;
int mbALM, mbALMTest=0,mbDL60,mbDL60Test=0, mbBK3,mbBK3Test=0, mbGK76, 
	mbGK76Test=0, mbGK60, mbGK60Test=0, mbSKP,mbSKPTest=0,mbOpenAll=0,mbCloseAll=0,mbOpen=0,mbClose=0,runingArm=0,mbMSC,
		   mbMSCTest=0,mbMSCOpenAll=0,mbMSCCloseAll=0,mbMSCOpen=0,mbMSCClose=0;

int mbCopyMem=0,mbRTCMem=0,	mbViewMem=0,mbCreateMem=0,mbFormatMem=0,mbOnTimerMem=0,mbOffTimerMem=0,mbOnPowerMem=0,mbOffPowerMem=0;
int mbBK79,mbBK79Test=0,mbNNK,mbNNKTest=0,mbGGK,mbGGKTest=0,mbAK1_1,mbAK1_2,mbAK2_1,mbAK2_2,mbAK_T,mbT_P,
			mbT_PTest=0,mbSGK,mbSGKTest=0, mbMBK,mbMBKTest=0,mbMBKOpen = 0, mbMBKClose=0,mbGGKOpen=0,mbGGKClose=0;

MFTData_t *pBufData;
int BufDaq[NUMCHANNEL];
char CurName[150];
BYTE iByte(BYTE b)
{
	BYTE w0=0,a=1,w,w1;
	w = b;
	for(int i=0;i<8;i++){
		w1 = w&a;
		w1 <<= 7-i;
		w0 |= w1;
		w >>= 1;
	}
	return w0;
}

UINT i12Bits(int b1,int b2)
{
	UINT w0=0,a=1,w,w1;
	w =  b1;
	w <<= 4;
	w |= b2>>4;
	
	for(int i=0;i<12;i++){
		w1 = w&a;
		w1 <<= 11-i;
		w0 |= w1;
		w >>= 1;
	}
	return w0;
}

UINT i24Bits(int b1,int b2,int b3)
{
	UINT w0=0,a=1,w,w1,w2;
	w =  b1;
	w <<= 16;
	w2 = b2;
	w2 <<= 8;
	w |= w2;
	w |= b3;
	for(int i=0;i<24;i++){
		w1 = w&a;
		w1 <<= 23-i;
		w0 |= w1;
		w >>= 1;
	}
	return w0;
}

double Press_Cal(int fp,int ft)
{
	double mp=0.01,fp0=coef[20],mt=0.01,ft0=coef[21];
	double xp,xt;
	double press;
	double a[4];
		
	xp=mp*(fp-fp0);	
	xt=mt*(ft-ft0);
	for(int i=0;i<4;i++)
		a[i]=coef[i*4] + xt*(coef[i*4+1] +xt*(coef[i*4+2] + xt*coef[i*4+3]));
	press= a[0] + xp*(a[1] + xp*(a[2] + xp*a[3]));

	return press;
}

double Press_Cal1(int fp,int ft)
{
	double mp=0.01,fp0=coef1[20],mt=0.01,ft0=coef1[21];
	double xp,xt;
	double press;
	double a[4];
	
	xp=mp*(fp-fp0);	
	xt=mt*(ft-ft0);
	for(int i=0;i<4;i++)
		a[i]=coef1[i*4] + xt*(coef1[i*4+1] +xt*(coef1[i*4+2] + xt*coef1[i*4+3]));
	press= a[0] + xp*(a[1] + xp*(a[2] + xp*a[3]));

	return press;
}

double Temperature_Cal(int ft)
{
	double mt=0.01,ft0=coef[21];
	double xt;
	double Temp;
	
	xt=mt*(ft-ft0);

	return	Temp= coef[16] + xt*(coef[17] + xt*(coef[18] + xt*coef[19]));
}

double Temperature_Cal1(int ft)
{
	double mt=0.01,ft0=coef1[21];
	double xt;
	double Temp;
	
	xt=mt*(ft-ft0);
	return	Temp= coef1[16] + xt*(coef1[17] + xt*(coef1[18] + xt*coef1[19]));
}

double Motor_Volt(double count)
{
	float a,b,gain,offset;
	
	for( int i=0;i<10;i+=2)
	{
		if((count > Motor_Tab[i+1])&&(count < Motor_Tab[i+3]))
		{
			a=(Motor_Tab[i+2]-Motor_Tab[i]);
			b=(Motor_Tab[i+3]-Motor_Tab[i+1]);
			gain=a/b;
			offset=Motor_Tab[i]-gain*Motor_Tab[i+1];
			break;
		}
	}	
	if(count <=Motor_Tab[1]) 
	{ 	
		a=(Motor_Tab[2]-Motor_Tab[0]);
		b=(Motor_Tab[3]-Motor_Tab[1]);
		gain=a/b;
		offset=Motor_Tab[0]-gain*Motor_Tab[1];
	}

	if(count >=Motor_Tab[11])
	{
		a=(Motor_Tab[10]-Motor_Tab[8]);
		b=(Motor_Tab[11]-Motor_Tab[9]);
		gain=a/b;
		offset=Motor_Tab[10]-gain*Motor_Tab[11];
	}
	return (gain*count + offset);	
}

float CalibADC(float a, float refm, float refg, float refp)
{
	float v1,v2,v3,gain,offset;
	
	v1=-7.470;
	v2=0.00;
	v3=7.520;
	if( a < refg){
		gain=(v1-v2);
		gain /= (refm-refg);
		offset=v1-gain*refm;
	}
	else{
		gain=(v2-v3);
		gain /= (refg-refp);
		offset=v2-gain*refg;
	}
	
	return (a*gain + offset);	
}
float CalibChannel(float a,float rawm,float realm, float rawg, float realg, float rawp, float realp)
{
	float v1,v2,v3,gain,offset;
	
	v1=realm;
	v2=realg;
	v3=realp;
	if( a < rawg){
		gain=(v1-v2);
		gain /= (rawm-rawg);
		offset=v1-gain*rawm;
	}
	else{
		gain=(v2-v3);
		gain /= (rawg-rawp);
		offset=v2-gain*rawg;
	}
	
	return (a*gain + offset);	
}

double MergeChannel(double ain,int ch)
{
	float v1,v2,v3,gain,offset;
	
	v1=pInfoCalib[ch].pCh.RealVal[0];
	v2=pInfoCalib[ch].pCh.RealVal[1];
	v3=pInfoCalib[ch].pCh.RealVal[2];
	if( ain < pInfoCalib[ch].pCh.RawVal[1]){
		gain=(v1-v2);
		if((pInfoCalib[ch].pCh.RawVal[1]-pInfoCalib[ch].pCh.RawVal[2]) != 0)
			gain /= (pInfoCalib[ch].pCh.RawVal[0]-pInfoCalib[ch].pCh.RawVal[1]);
		else return -9999;
		offset=v1-gain*pInfoCalib[ch].pCh.RawVal[0];
		if(ch==0){
			nATension = gain*1000;
			nBTension = offset*1000;
		}
	}
	else{
		gain=(v2-v3);
		if((pInfoCalib[ch].pCh.RawVal[1]-pInfoCalib[ch].pCh.RawVal[2]) != 0)
			gain /= (pInfoCalib[ch].pCh.RawVal[1]-pInfoCalib[ch].pCh.RawVal[2]);
		else return -9999;
		offset=v2-gain*pInfoCalib[ch].pCh.RawVal[1];
	}
	
	return (ain*gain + offset);	
}

int iMarked = 0;

UINT CheckFrameDepth(BYTE header,BYTE tailer,BYTE* str)
{
	UINT i,it,id=0,j;
	BYTE qbuf[53];
	if((str[0] == header) && (str[52] == tailer))
	{
		

		for(i=51;i>48;i--)
		{
			it = str[i];
			it <<= 8*(i-49);
			id |= it;
		}
	
		
		return id;
	} 
	else
	{
		for(i=0;i<53;i++)
		{
			if((str[i+1] == header) && (str[i] == tailer)) 
				break;
		}
		if(i==53) return 0;
		for(j=0;j<53;j++)
		{
			if(j<52-i)
				qbuf[j] = str[i+j+1];
			else
				qbuf[j] = str[j+i-52];
		}
	}
	id=0;
	
	
	for(i=51;i>48;i--)
		{
			it = qbuf[i];
			it <<= 8*(i-49);
			id |= it;
		}
	return id;	
}


void GetDepth()
{
	BYTE cc = 0x55;
//	m_cComm.WriteComm(&cc,1);
}

void GetMftData()
{
	BYTE cc=0xaa;
//	m_cComm.WriteComm(&cc,1);
}

void SetDepth(UINT idepth)
{
	BYTE cc[5];
	UINT lt;
	cc[0] = 0x56;
	lt = idepth & 0x000000ff;
	cc[1] = (BYTE) lt;
	lt = idepth & 0x0000ff00;
	lt >>= 8;
	cc[2] = (BYTE) lt;
	lt = idepth & 0x00ff0000;
	lt >>= 16;
	cc[3] = (BYTE) lt;
	lt = idepth & 0xff000000;
	lt >>= 24;
	cc[4] = (BYTE) lt;
	//m_cComm.WriteComm(cc,5);
}



int cmd_flag=1;
int load_flag=1,copy_flag=0;
int msc60_flag=0;
BYTE tool[8];
char szTime[80];
int		iSetMarked,irep=0;
long nask=0,nresp=0;
long qs=0;
int saq=0;
int lcur,lreco;

UINT	ReadCommDataThread(LPVOID pParam)
{

	OVERLAPPED  osWrite ;
	OVERLAPPED  osRead ;
	BYTE		cc=0xaa; //nbuf = inclinometer = 28 = 0x55 + 1 (dir) + 6 (dept_dec) + 5 (ftension) + 4(fspeed) + 4(idepth) + 2(iTension) + 2(iRes) + 0xaa
	BYTE buf[150];
	BOOL		bWaitingOnStat=false;

	CString		strTxtFN;
	CString		strMdbFN,sc;
	
	

	CWnd*		pWnd=(CWnd*)pParam;
	int k, itime=500,ngetdmt=0; 

    memset(&osWrite, 0, sizeof(OVERLAPPED));
	memset(&osRead, 0, sizeof(OVERLAPPED));

	//Open(int iPort, long BaudRate,int ByteSize,int Parity, int StopBits,bool bOverlapped)
	
     //create I/O event used for overlapped read
	m_cComm.Open(2,128000,8,2,1,true);
	//m_cComm.Open(2,128000,8,2,1,true);
	

	osWrite.hEvent = CreateEvent( NULL,    // no security
				              TRUE,    // explicit reset req
								FALSE,   // initial event reset
						      NULL ) ; // no name
	osRead.hEvent = CreateEvent( NULL,    // no security
				              TRUE,    // explicit reset req
								FALSE,   // initial event reset
						      NULL ) ; // no name
   
	//ASSERT(os.hEvent); 
	int i=0,j=1,temp;
	BOOL		bWaitingOnWrite=false;
	BOOL		bWaitingOnRead=false;
	DWORD		nBytesRead,nBytesWrite;
	DWORD		dwResRead,dwResWrite;
	BOOL		bReadOperationOK= true;
	BOOL		bWriteOperationOK=false;
	BOOL		bRead_Status;
	DWORD		dwEventMask;
	
	BYTE dd[17];
	UINT lt,read_flag=0;	
	char sz[20];
	long size = 584;
	
	SYSTEMTIME		systime;
	
	
	//DWORD dtoldcomm,dtcurcomm,gktold=0,gktcur=0,dtgk=0;
	gktold = dtoldcomm = GetTickCount();
	 
	dtcurcomm = dtoldcomm;
	static int nA,nB,nt=0,toolstring[100],inn=0;
	static int tsNum[]={1,		//1 truy cap du lieu DMT
						2,		//2 truy cap du lieu ALM (CCL)
						3,		//3 truy cap du lieu ALM test
						5,		//4 truy cap du lieu DL-60
						6,		//5 truy cap du lieu DL-60 test
						1,		//6 truy cap du lieu DMT
						11,		//7 truy xuat BK3-76
						12,		//8 truy xuat BK3-76 test
						7,		//9 truy xuat GK-76 
						8,		//10 truy xuat GK-76 test
						13,		//11 truy xuat SKP-76
						14,		//12 truy xuat SKP-76 test
						15,		//13 truy xuat SKP-76 Open all
						16,		//14 truy xuat SKP-76 Close all
						17,		//15 truy xuat SKP-76 Open
						18,		//16 truy xuat SKP-76 Close
						9,		//17 truy xuat GK-60
						10,		//18 truy xuat GK-60
						19,		//19 AK-76  T1R1
						20,		//20 AK-76  T1R2
						21,		//21 AK-76  T2R1
						22,		//22 AK-76  T2R2
						23,		//23 AK-76  Temperature



						1,	//toolstring[23] = mbT_P;
						1,	//toolstring[24] = mbT_PTest;

						12,	//toolstring[25] = mbBK79;
						1,	//toolstring[26] = mbBK79Test;

						11,	//toolstring[27] = mbMBK;
						1,	//toolstring[28] = mbMBKTest;
						1,	//toolstring[29] = mbMBKOpen;
						1,	//toolstring[30] = mbMBKClose;

						1,	//toolstring[31] = mbNNK;
						1,	//toolstring[32] = mbNNKTest;

						1,	//toolstring[33] = mbGGK;
						1,	//toolstring[34] = mbGGKTest;
						1,	//toolstring[35] = mbGGKOpen;
						1,	//toolstring[36] = mbGGKClose;

						10,	//toolstring[37]= mbSGK;
						1,	//toolstring[38]= mbSGKTest;


						24,		//39 truy xuat MSC40
						25,		//40 truy xuat MSC40 test
						26,		//41 truy xuat MSC40 Open all
						27,		//42 truy xuat MSC40 Close all
						28,		//43 truy xuat MSC40 Open
						29,		//44 truy xuat MSC40 Close

						30,		//mbCopyMem;			//0xee	copy file
						31,		//mbRTCMem;			//0x77 cap nhat RTC
						32,		//mbViewMem;			//0x88 view header file
						33,		//mbCreateMem;		//0xaa create new file
						34,		//mbFormatMem;		//0xbb format SD
						35,		//mbOnTimerMem;		//0x66 on timer mem
						36,		//mbOffTimerMem;		//0x55 off timer mem
						37,		//mbOnPowerMem;		//0x44 on tool power
						38,		//mbOffPowerMem;		//0x33 off tool power
						39		//Synch RTC
						
							};	


	mwait(600);
	DisplayDataInPanel(pWnd);

    while(!bStopFirmware)	{
		dtcurcomm = GetTickCount();	
		toolstring[0]  = 1;				
		toolstring[1]  = mbALM;
		toolstring[2]  = mbALMTest;	
		toolstring[3]  = mbDL60;
		toolstring[4]  = mbDL60Test;
		toolstring[5]  = 0;
		toolstring[6]  = mbBK3;
		toolstring[7]  = mbBK3Test;
		toolstring[8]  = mbGK76;
		toolstring[9]  = mbGK76Test;
		toolstring[10] = mbSKP;
		toolstring[11] = mbSKPTest;
		toolstring[12] = mbOpenAll;
		toolstring[13] = mbCloseAll;
		toolstring[14] = mbOpen;
		toolstring[15] = mbClose;
		toolstring[16] = mbGK60;
		toolstring[17] = mbGK60Test;
		toolstring[18] = mbAK1_1;
		toolstring[19] = mbAK1_2;
		toolstring[20] = mbAK2_1;
		toolstring[21] = mbAK2_2;
		toolstring[22] = mbAK_T;
		toolstring[23] = mbT_P;
		toolstring[24] = mbT_PTest;
		toolstring[25] = mbBK79;
		toolstring[26] = mbBK79Test;
		toolstring[27] = mbMBK;
		toolstring[28] = mbMBKTest;
		toolstring[29] = mbMBKOpen;
		toolstring[30] = mbMBKClose;
		toolstring[31] = mbNNK;
		toolstring[32] = mbNNKTest;
		toolstring[33] = mbGGK;
		toolstring[34] = mbGGKTest;
		toolstring[35] = mbGGKOpen;
		toolstring[36] = mbGGKClose;
		toolstring[37] = mbSGK;
		toolstring[38] = mbSGKTest;
		toolstring[39] = mbMSC;
		toolstring[40] = mbMSCTest;
		toolstring[41] = mbMSCOpenAll;
		toolstring[42] = mbMSCCloseAll;
		toolstring[43] = mbMSCOpen;
		toolstring[44] = mbMSCClose;
		//for(i=45;i<55;i++) toolstring[i] = false;
	
		if ((toolstring[1] +toolstring[2]+toolstring[3]+toolstring[4] > 0)&& 
			(toolstring[6]+toolstring[7] +toolstring[8]+toolstring[9] +toolstring[10]+toolstring[11]
			+toolstring[12]+toolstring[13]+toolstring[14]+toolstring[15]
			+toolstring[16]+toolstring[17]   +toolstring[18] +toolstring[19]+toolstring[20] +toolstring[21]+toolstring[22]
			+toolstring[23]+toolstring[24]+toolstring[25]+toolstring[26]
			+toolstring[27]+toolstring[28]    + toolstring[29]+toolstring[30]+toolstring[31]+toolstring[32]
			+toolstring[33]+toolstring[34]    + toolstring[35]+toolstring[36]+toolstring[37]+toolstring[38]
			+toolstring[39]+toolstring[40]    + toolstring[41]+toolstring[42]+toolstring[43]+toolstring[44]
			> 0)) {	
			toolstring[5] = 1;	
		}

		if(toolstring[12] +	toolstring[13] + toolstring[14] + toolstring[15] + toolstring[29] + toolstring[30]
			+toolstring[41] +	toolstring[42] + toolstring[43] + toolstring[44] > 0){
			toolstring[10] = 0;
			toolstring[11] = 0;
			toolstring[27] = 0;
			toolstring[28] = 0;
			toolstring[39] = 0;
			toolstring[40] = 0;
		}
	
		if((dtcurcomm-dtoldcomm >= itime)&&(load_flag==1)){		//160		
			dtoldcomm=dtcurcomm;
			bSendOK=true;
			for(i=inn;i<45;i++) {
			
				if (toolstring[i])  {
					cmd_flag = tsNum[i];
					break;
				}
			}	
			if (i==45) {
			
				inn = 1;
				cmd_flag = tsNum[0];	
			}
			else inn = i+1;
			if (inn > 45) inn = 0;	
			
		}	

		DepthFlag=1;	

					
		if(bSendOK){
			switch (cmd_flag) {
			case 1:				//truy cap du lieu DMT
				dd[0] = 0xde; //access to ext device
				dd[1] = 0;	//command word
				dd[2] = 0;	//address
				dd[3] = 0;	//size tool frame
				dd[4] = 0;
				dd[5] = 0;
				dd[6] = 0;
				dd[7] = 0;								
				//WriteFile(m_cComm.m_hComm,dd,8,&nBytesWrite,&osWrite);			
				bSendOK=false;
				nt = 1;
				//size=138;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK=true;
				if(mbMSC) itime = 330;
				else itime =200;
				break;

			case 2:				//truy cap du lieu ALM (CCL)
				tool[0] = 0xcc; //power on
				tool[1] = 0x20;	//command word
				tool[2] = 0x40;	//address    lenh = 0x2040
				tool[3] = 2;	//word size tool frame, so words tra loi tu may gieng
				tool[4] = 96;//58;	//command word clk 65472 so may moi la 
				tool[5] = 248;//172;	//read clk 65362 //mcu moi 178
				tool[6] = 152;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);			
				bSendOK=false;
				nt=2;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK=true;				
				break;

			case 3:				//truy cap status ALM (CCL)								
				tool[0] = 0xcc; //power on
				tool[1] = 0x25;	//command word
				tool[2] = 0x00;	//address
				tool[3] = 16;	//word size tool frame
				tool[4] = 96;  //58;	//command word clk 65472
				tool[5] = 248;  //172;	//read clk 65362
				tool[6] = 152;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);			
				bSendOK=false;
				nt=3;
				size=50;				
				bWriteOperationOK = false;
				bReadOperationOK=true;			
				break;

			case 4:				//set do sau
			
				if (dmt_flag == 1){			
					dd[0] = 0xdd;
					dd[1] = nSetDepth>>24; //set depth
					lt = nSetDepth & 0x00ffffff;
					lt >>= 16;
					dd[2] = (BYTE) lt;
					lt = nSetDepth & 0xffff;
					lt >>= 8;
					dd[3] = (BYTE) lt;
					dd[4] = nSetDepth & 0xff;
					dd[5] = 0;
					dd[6] = 0;
					dd[7] = 0;
				}
				else if (dmt_flag == 2){	//nEncoder=19685;				
					dd[0] = 0xcd;
					dd[1] = nEncoder>>24; //set encoder
					lt = nEncoder & 0x00ffffff;
					lt >>= 16;
					dd[2] = (BYTE) lt;
					lt = nEncoder & 0xffff;
					lt >>= 8;
					dd[3] = (BYTE) lt;
					dd[4] = nEncoder & 0xff;
					dd[5] = 0;
					dd[6] = 0;
					dd[7] = 0;				
				}
				else if (dmt_flag == 3){
					dd[0] = 0xdf;
					dd[1] = nSetDepth>>24; //set depth
					lt = nSetDepth & 0x00ffffff;
					lt >>= 16;
					dd[2] = (BYTE) lt;
					lt = nSetDepth & 0xffff;
					lt >>= 8;
					dd[3] = (BYTE) lt;
					dd[4] = nSetDepth & 0xff;
					dd[5] = 0;
					dd[6] = 0;
					dd[7] = 0;
				}
				else if (dmt_flag == 4){
					dd[0] = 0xdc;
					nA = nATension;
					dd[1] = nA>>8; //set a tension
					dd[2] = nA & 0xff;
					nB = nBTension;
					dd[3] = nB>>8; //set b tension
					dd[4] = nB & 0xff;
					dd[5] = 0;
					dd[6] = 0;
					dd[7] = 0;						
				}
				else if (dmt_flag == 5){			
					dd[0] = 0xdb;
					dd[1] = 0;
					dd[2] = 0;
					dd[3] = 0;
					dd[4] = 0;
					dd[5] = 0;
					dd[6] = 0;
					dd[7] = 0;
				}							
				WriteFile(m_cComm.m_hComm,dd,8,&nBytesWrite,&osWrite);
				dmt_flag = 0;
				bSendOK= false;
				bWriteOperationOK = false;
				bReadOperationOK=false;
				break;

			case 5:				//truy cap du lieu DL-60
				tool[0] = 0xcc; //power on
				tool[1] = 0x08;	//command word
				tool[2] = 0x00;	//address
				tool[3] = 14;	//word size tool frame
				tool[4] = 96;//58;	//command word clk 65472 tram 2
				tool[5] = 248;//171;	//read clk 65362 
				tool[6] = 146;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);								
				bSendOK=false;
				nt=5;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK=true;
				break;

			case 6:				//test DL-60 16 words
				tool[0] = 0xcc; //power on
				tool[1] = 0x0d;	//command word
				tool[2] = 0x00;	//address
				tool[3] = 16;	//word size tool frame
				tool[4] = 96;//58;	//command word clk 65472
				tool[5] = 8;//171;	//read clk 65362
				tool[6] = 60;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);						
				bSendOK=false;
				nt=6;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK=true;
				break;

			case 7:				//truy xuat GK-76 6 words
				tool[0] = 0xcc; //power on
				tool[1] = 0x38;	//command word
				tool[2] = 0x00;	//address
				tool[3] = 6;	//word size tool frame
				tool[4] = 96;//57;	//command word clk 65472
				tool[5] = 8;//171;	//read clk 65362
				tool[6] = 60;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);			
				gktcur = GetTickCount();
				dtgk = gktcur - gktold;	
				gktold = gktcur;
				bSendOK=false;
				nt=7;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK=true;
				break;

			case 8:				//test GK-76 11 words
				tool[0] = 0xcc; //power on
				tool[1] = 0x38;	//command word
				tool[2] = 0x09;	//address
				tool[3] = 11;	//word size tool frame
				tool[4] = 96;//57;	//command word clk 65472
				tool[5] = 8;//171;	//read clk 65362
				tool[6] = 60;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);								
				bSendOK=false;
				nt=8;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK=true;
				break;

			case 9:				//truy xuat GK-60 10 words
				tool[0] = 0xcc; //power on
				tool[1] = 0x38;	//command word
				tool[2] = 0x00;	//address
				tool[3] = 10;	//word size tool frame
				tool[4] = 159;	//command word clk 65472
				tool[5] = 8;	//read clk 65362
				tool[6] = 60;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);						
				bSendOK=false;
				nt=9;
				size=526;
				itime = 500;
				bWriteOperationOK = false;
				bReadOperationOK=true;
				break;

			case 10:			//test SGK-90 256 words
				tool[0] = 0xcc; //power on
				tool[1] = 0xaa;	//command word
				tool[2] = 0xaa;	//77//address
				tool[3] = 0; //3;	//word size tool frame
				tool[4] = 159;	//command word clk 65472
				tool[5] = 13;	//read clk 65362
				tool[6] = 85;	//64753 msb
				tool[7] = 1;	//64753 lsb				
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);							
				bSendOK=false;
				nt=10;
				size=526;
				itime = 750;
				bWriteOperationOK = false;
				bReadOperationOK=true;
				
				break;

			case 11:			//truy xuat BK35 24 words
				tool[0] = 0xcc; //power on
				tool[1] = 0xe2;//0xf1;	//command word
				tool[2] = 0x09;//0x14;	//address
				tool[3] = 11;//24;	//word size tool frame
				tool[4] = 160;	//command word clk 65472
				tool[5] = 8;	//read clk 65362
				tool[6] = 60;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);								
				bSendOK=false;
				nt=11;
				size=526;
				itime = 500;
				bWriteOperationOK = false;
				bReadOperationOK=true;				
				break;

			case 12:			// BK79-90 64 words
				tool[0] = 0xcc; //power on
				tool[1] = 0xe2;	//command word
				tool[2] = 0x06;	//address 05
				tool[3] = 10;	//word size tool frame
				tool[4] = 160;	//command word clk 65472
				tool[5] = 8;	//read clk 65362 248
				tool[6] = 60;	//64753 msb 153
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);								
				bSendOK=false;
				nt=12;
				size=526;
				itime = 500;
				bWriteOperationOK = false;
				bReadOperationOK= true;				
				break;

			case 13:			//acess SKP-76 8 words
				tool[0] = 0xcc; //power on
				tool[1] = 0x10;	//command word
				tool[2] = 0x00;	//address
				tool[3] = 8;	//word size tool frame
				tool[4] = 58;	//command word clk 65472
				tool[5] = 172;	//read clk 65362 
				tool[6] = 152;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);								
				bSendOK=false;
				nt=13;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK= true;
				break;

			case 14:			//test SKP-76 11 words								
				tool[0] = 0xcc; //power on
				tool[1] = 0x10;	//command word
				tool[2] = 0x5e;	//address
				tool[3] = 11;	//word size tool frame
				tool[4] = 58;	//command word clk 65472
				tool[5] = 172;	//read clk 65362 
				tool[6] = 152;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);						
				bSendOK=false;
				nt=14;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK= true;
				break;

			case 15:			//Open Arms all SKP-76 11 words
				tool[0] = 0xcc; //power on
				tool[1] = 0x10;	//command word
				tool[2] = 0xa0;	//address
				tool[3] = 8	;//word size tool frame
				tool[4] = 58;	//command word clk 65472
				tool[5] = 172;	//read clk 65362 
				tool[6] = 152;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);						
				bSendOK=false;
				nt=15;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK= true;
				break;

			case 16:			//Close Arm all SKP-76 11 words
								
				tool[0] = 0xcc; //power on
				tool[1] = 0x10;	//command word
				tool[2] = 0x50;	//address
				tool[3] = 8	;//word size tool frame
				tool[4] = 58;	//command word clk 65472
				tool[5] = 172;	//read clk 65362 
				tool[6] = 152;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);						
				bSendOK=false;
				nt=16;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK= true;
				break;

			case 17:			//Open Arms SKP-76 11 words
				tool[0] = 0xcc; //power on			
				tool[1] = 0x10;	//command word
				tool[2] = 0x0a;	//address			
				tool[3] = 8	;	//word size tool frame
				tool[4] = 58;	//command word clk 65472
				tool[5] = 172;	//read clk 65362 
				tool[6] = 152;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);						
				bSendOK=false;
				nt=17;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK= true;
				break;

			case 18:			//Close Arm SKP-76 11 words					
				tool[0] = 0xcc; //power on			
				tool[1] = 0x10;	//command word
				tool[2] = 0x05;	//address			
				tool[3] = 8	;	//word size tool frame
				tool[4] = 58;	//command word clk 65472
				tool[5] = 171;	//read clk 65362 
				tool[6] = 152;	//64753 msb
				tool[7] = 0;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);			
				bSendOK=false;
				nt=18;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK= true;
				break;

			case 19:			// AK-76 1 words T1R1								
				tool[0] = 0xcc; //power on			
				tool[1] = 0x18;	//command word
				tool[2] = 0x00;	//address			
				tool[3] = 1;	//word size tool frame
				tool[4] = 63;	//command word clk 65472
				tool[5] = 173;	//read clk 65362
				tool[6] = 3;	//64753 msb
				tool[7] = 14;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);
				bSendOK=false;
				nt= 19;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK= false;
				break;

			case 20:			// AK-76 1 words T1R2								
				tool[0] = 0xcc; //power on			
				tool[1] = 0x18;	//command word
				tool[2] = 0x01;	//address			
				tool[3] = 1;	//word size tool frame
				tool[4] = 63;	//command word clk 65472
				tool[5] = 173;	//read clk 65362
				tool[6] = 3;	//64753 msb
				tool[7] = 14;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);
				bSendOK=false;
				nt=20;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK= false;
				break;

			case 21:			// AK-76 1 words T2R1								
				tool[0] = 0xcc; //power on			
				tool[1] = 0x18;	//command word
				tool[2] = 0x08;	//address			
				tool[3] = 1;	//word size tool frame
				tool[4] = 63;	//command word clk 65472
				tool[5] = 173;	//read clk 65362
				tool[6] = 3;	//64753 msb
				tool[7] = 14;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);
				bSendOK=false;
				nt=21;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK= false;
				break;

			case 22:			// AK-76 1 words T2R2								
				tool[0] = 0xcc; //power on			
				tool[1] = 0x18;	//command word
				tool[2] = 0x09;	//address			
				tool[3] = 1;	//word size tool frame
				tool[4] = 63;	//command word clk 65472
				tool[5] = 173;	//read clk 65362
				tool[6] = 3;	//64753 msb
				tool[7] = 14;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);	
				bSendOK=false;
				nt=22;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK= false;
				break;

			case 23:			// AK-76 1 words Temper								
				tool[0] = 0xcc; //power on			
				tool[1] = 0x1f;	//command word
				tool[2] = 0x00;	//address			
				tool[3] = 3;	//word size tool frame
				tool[4] = 63;	//command word clk 65472
				tool[5] = 173;	//read clk 65362
				tool[6] = 3;	//64753 msb
				tool[7] = 14;	//64753 lsb								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);								
				bSendOK=false;
				nt=23;
				size=50;
				bWriteOperationOK = false;
				bReadOperationOK= true;
				break;

			case 24:			//acess MSC40 48 words
				tool[0] = 0xcc; //power on			
				tool[1] = 0x10;	//command word
				tool[2] = 0x00;	//address			
				tool[3] = 48;	//word size tool frame
				tool[4] = 59;	//command word clk 65472
				tool[5] = 172;	//read clk 65362 
				tool[6] = 152;	//64753 msb
				tool[7] = 75;	//64753 lsb			
				size=98;			//48x2  +2
				if (msc60_flag)		{
					tool[3] = 68;	//word size tool frame
					size=138;		//68x2 +2
				}								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);					
				bSendOK=false;	
				nt=24;
				bWriteOperationOK = false;			
				bReadOperationOK= true;
				break;

			case 25:			//test MSC40 11 words
				tool[0] = 0xcc; //power on			
				tool[1] = 0x10;	//command word
				tool[2] = 0x5f;	//address			
				tool[3] = 48;	//word size tool frame
				tool[4] = 59;	//command word clk 65472
				tool[5] = 172;	//read clk 65362 
				tool[6] = 152;	//64753 msb
				tool[7] = 0;	//64753 lsb
				size=98;			//48x2  +2
				if (msc60_flag)		{
					tool[3] = 68;	//word size tool frame
					size=138;		//68x2 +2							
				}								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);														
				bSendOK=false;			
				nt=25;
				bWriteOperationOK = false;
				bReadOperationOK= true;
				break;

			case 26:			//Open Arms all MSC40 48 words
				tool[0] = 0xcc; //power on			
				tool[1] = 0x10;	//command word
				tool[2] = 0xa0;	//address			
				tool[3] = 48	;//word size tool frame
				tool[4] = 59;	//command word clk 65472
				tool[5] = 172;	//read clk 65362 
				tool[6] = 152;	//64753 msb
				tool[7] = 0;	//64753 lsb
				size=98;			//48x2  +2
				if (msc60_flag)		{
					tool[3] = 68;	//word size tool frame
					size=138;		//68x2 +2
				}								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);
				bSendOK=false;			
				nt=26;
				bWriteOperationOK = false;
				bReadOperationOK= true;
				break;

			case 27:			//Close Arm all MSC40 48 words
				tool[0] = 0xcc; //power on			
				tool[1] = 0x10;	//command word
				tool[2] = 0x50;	//address			
				tool[3] = 48	;//word size tool frame
				tool[4] = 59;	//command word clk 65472
				tool[5] = 172;	//read clk 65362 
				tool[6] = 152;	//64753 msb
				tool[7] = 75;	//64753 lsb
				size=98;			//48x2  +2
				if (msc60_flag)		{
					tool[3] = 68;	//word size tool frame
					size=138;		//68x2 +2
				}				
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);						
				bSendOK=false;
				nt=27;
				bWriteOperationOK = false;
				bReadOperationOK= true;
				break;

			case 28:				//Open Arms MSC40 48 words ...cai nay mo cang
				tool[0] = 0xcc; //power on			
				tool[1] = 0x10;	//command word
				tool[2] = 0xa0;	//address			
				tool[3] = 48	;//word size tool frame
				tool[4] = 59;	//command word clk 65472
				tool[5] = 172;	//read clk 65362 
				tool[6] = 152;	//64753 msb
				tool[7] = 75;	//64753 lsb
				size=98;			//48x2  +2
				if (msc60_flag)		{
					tool[3] = 68;	//word size tool frame
					size=138;		//68x2 +2
				}								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);								
				bSendOK=false;
				nt=28;
				bWriteOperationOK = false;
				bReadOperationOK= true;	
				break;

			case 29:				//Close Arm MSC40 48 words
				tool[0] = 0xcc; //power on			
				tool[1] = 0x10;	//command word
				tool[2] = 0x05;	//address			
				tool[3] = 48	;//word size tool frame
				tool[4] = 59;	//command word clk 65472
				tool[5] = 172;	//read clk 65362 
				tool[6] = 152;	//64753 msb
				tool[7] = 75;	//64753 lsb
				size=98;			//48x2  +2
				if (msc60_flag)		{
					tool[3] = 68;	//word size tool frame
					size=138;		//68x2 +2				
				}								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);					
				bSendOK=false;			
				nt=29;
				bWriteOperationOK = false;
				bReadOperationOK= true;
				break;

			case 30:				//Copy data 
								
				if(copy_flag==0) {	
				
					CFileDialog m_filedlg(FALSE,"txt",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"Text Files (*.dxt)|*.dxt||");	
	
					if(m_filedlg.DoModal()==IDOK){
		
						if(IsFileExist(m_filedlg.GetPathName().GetBuffer(m_filedlg.GetPathName().GetLength())))	{
							AfxMessageBox("File already existed. Can't overwrite");
						
						}
						else 	strTxtFN=m_filedlg.GetPathName();
			
					}
					if(pcFile!=NULL)	delete pFile;
					pcFile=new CFile();	
					pcFile->Open(strTxtFN,CFile::modeCreate|CFile::modeWrite);
					
					copy_flag=1;				
					tool[0] = 0xee;
					for(i=1;i<8;i++	) tool[i] = 0;
					WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);
					
					
				}

				else {		
					
					tool[0] = 0x11;			//lenh doc tiep du lieu
					for(i=1;i<8;i++	) tool[i] = 0;			
					WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);
					
					
				}
				bSendOK= true;						
				load_flag=0;		//khong quet dmt			
				nt=30;				//tro den ham xu ly
				size=22;			//kich thuoc goi du lieu 
				cmd_flag = 30;		//doc tiep tuc cho den khi het file
				bWriteOperationOK = false;		//tiep tuc kiem soat doc ket thuc xu ly doc
				bReadOperationOK= true;
				
				break;

			case 31:				//RTC cap nhat 								
				GetLocalTime(&systime);		
				buft[5] = systime.wSecond;
				buft[11] = buft[5] ;
				buft[7] = systime.wMonth;
				buft[8] = systime.wYear-2000;
					
				if(buft[0] < systime.wDay) {
					buft[0] = systime.wDay;
					buft[3] = systime.wHour;
					buft[9] = buft[3] + 6;
					 
					buft[4] = systime.wMinute;
					buft[10] = buft[4];
				}
				else if(buft[3] < systime.wHour) {
					buft[3] = systime.wHour;
					buft[4] = systime.wMinute;
					buft[9] = buft[3] + 6;
					buft[10] = buft[4];
				}
				else if(buft[4] < systime.wMinute) {
					buft[4] = systime.wMinute + 1;
					buft[10] = buft[4];
				}

				if(buft[9] >= 24){
						buft[6] +=1;	//tang ngay
						buft[9] -=24;
				}

				tool[0] = 0x77;
				tool[1] = buft[0];
				tool[2] = systime.wMonth;
				tool[3] = systime.wYear-2000;
				tool[4] = buft[3];
				tool[5] = buft[4];
				tool[6] = buft[5];
				tool[7] = 0;
				
				sprintf(szTime,"%02d:%02d:%02d %d/%d/%d",buft[3],buft[4],buft[5],buft[0],buft[1],buft[2]);
				
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);
				bSendOK=false;
				nt=31;
				bWriteOperationOK = false;
				bReadOperationOK=false;
				break;

			case 32:				//view header log.dxt file								
				tool[0] = 0x88;		//tool[0] = 0x99;
				for(i=1;i<8;i++	) tool[i] = 0;
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);	
				bSendOK=false;
				nt=32;
				bWriteOperationOK = false;
				bReadOperationOK=false;
				break;

			case 33:		//create new log.dxt file			
				GetLocalTime(&systime);	
				buft[5] = systime.wSecond;
				if(buft[0] < systime.wDay) {
					buft[0] = systime.wDay;
					buft[3] = systime.wHour;
					buft[4] = systime.wMinute;
				}
				else if(buft[3] < systime.wHour) {
					buft[3] = systime.wHour;
					buft[4] = systime.wMinute;
				}
				else if(buft[4] < systime.wMinute) {
					buft[4] = systime.wMinute + 1;
				}
				tool[0] = 0xaa;
				tool[1] = systime.wDay;;
				tool[2] = systime.wMonth;
				tool[3] = systime.wYear-2000;
				tool[4] = systime.wHour;
				tool[5] = systime.wMinute;
				tool[6] = systime.wSecond;
				tool[7] = 0;
					

				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);	
				bSendOK=false;
				nt=33;
				bWriteOperationOK = false;
				bReadOperationOK=false;
				break;

			case 34:		//format sd 								
				tool[0] = 0xbb;
				for(i=1;i<8;i++	) tool[i] = 0;									
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);						
				bSendOK=false;
				nt=34;
				bWriteOperationOK = false;
				bReadOperationOK=false;
				break;

			case 35: 				//on timer mem 
				GetLocalTime(&systime);	
				tool[0] = 0x66;
				tool[1] = systime.wDay;
				tool[2] = systime.wMonth;
				tool[3] = systime.wYear-2000;
				tool[4] = systime.wHour;
				tool[5] = systime.wMinute;
				tool[6] = systime.wSecond;
				tool[7] = systime.wMilliseconds/10;			
								
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);					
				bSendOK=false;
				nt=35;
				bWriteOperationOK = false;
				bReadOperationOK=false;
				break;
			
			case 36:			//off timer mem 								
				tool[0] = 0x55;
				for(i=1;i<8;i++	) tool[i] = 0;										
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);					
				bSendOK=false;
				nt=36;
				bWriteOperationOK = false;
				bReadOperationOK=false;
				break;

			case 37:			//on tool power 								
				tool[0] = 0x44;
				for(i=1;i<8;i++	) tool[i] = 0;										
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);					
				bSendOK=false;
				nt=37;
				bWriteOperationOK = false;
				bReadOperationOK=false;
				mbOnPowerMem =0;
				break;

			case 38:			//off tool power
				tool[0] = 0x33;
				for(i=1;i<8;i++	) tool[i] = 0;									
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);						
				bSendOK=false;
				nt=38;
				bWriteOperationOK = false;
				bReadOperationOK=false;
				mbOffPowerMem=0;
				
				break;

			case 39:		//dong bo RTC 	
				GetLocalTime(&systime);	
				tool[0] = 0x22;
				tool[1] = systime.wDay;
				tool[2] = systime.wMonth;
				tool[3] = systime.wYear-2000;
				tool[4] = systime.wHour;
				tool[5] = systime.wMinute;
				tool[6] = systime.wSecond;
				tool[7] = systime.wMilliseconds/10;									
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);	
				sprintf(szTime,"%02d:%02d:%02d  %d/%d/%d",tool[4],tool[5],tool[6],tool[1],tool[2],tool[3]);
				bSendOK=false;
				nt=39;
				bWriteOperationOK = false;
				bReadOperationOK=false;
				break;

			case 40:			//on Off power 								
				tool[0] = 0xf4;
				for(i=1;i<8;i++	) tool[i] = 0;										
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);					
				bSendOK=false;
				nt=40;
				bWriteOperationOK = false;
				bReadOperationOK=false;
				break;

			case 41:				//RTC Stop 								
				GetLocalTime(&systime);		
				buft[7] = systime.wMonth;
				buft[8] = systime.wYear -2000;
				buft[11] = systime.wSecond;				

				tool[0] = 0xf7;
				tool[1] = buft[6];
				tool[2] = buft[7];
				tool[3] = buft[8];
				tool[4] = buft[9];
				tool[5] = buft[10];
				tool[6] = 0;
				tool[7] = 0;
				
				WriteFile(m_cComm.m_hComm,tool,8,&nBytesWrite,&osWrite);
				bSendOK=false;
				nt=41;
				bWriteOperationOK = false;
				bReadOperationOK=false;
				break;
			
			default: break;

			}
		}		

/*	if(bWriteOperationOK)		{

			dd[0] = 0xdb; //access to ext device
			
			dd[1] = 0;	//command word

			dd[2] = 255;	//address
								
			if(!WriteFile(m_cComm.m_hComm,dd,3,&nBytesWrite,&osWrite))
			
			//if(!WriteFile(m_cComm.m_hComm,&cc,1,&nBytesWrite,&osWrite))
				//Write not completed immediately
			{
				if(GetLastError()!=ERROR_IO_PENDING)
				{
					bReadOperationOK=false;
				}
				else
				{
					dwResWrite=WaitForSingleObject(osWrite.hEvent,INFINITE);
					switch(dwResWrite)
					{
					case WAIT_OBJECT_0:
						if(!GetOverlappedResult(m_cComm.m_hComm,&osWrite,&nBytesWrite,false))
						{
							//MessageBox(NULL,"OverlappedResult","Error",MB_OK);			
							bReadOperationOK=false;
						}
						else
						{
							if(nBytesWrite!=3)
							{
								//MessageBox(NULL,"Write Timeout1","Error",MB_OK);			
								bReadOperationOK=false;
							}
							else
							{
								//MessageBox(NULL,"Success1","Error",MB_OK);			
								bReadOperationOK=true;
								size = 53;
							}
						}
						break;
					default:
						break;
					}
				}
			}
			else//Write completed immediately
			{
				if(nBytesWrite!=3)//Write timeout
				{
					//MessageBox(NULL,"Write Timeout","Error",MB_OK);			
					bReadOperationOK=false;
				}
				else
				{
					bReadOperationOK=true;
					//MessageBox(NULL,"Success","Error",MB_OK);			
				}
			}
			
		}//end write operation*/

		if(bReadOperationOK){

			//if(nt==30) mwait(205);
			

			//else if(nt==12) mwait(13);
			//else if(nt==10) mwait(20);
			//else mwait(13);

			if(ReadFile(m_cComm.m_hComm,buf,size,&nBytesRead,&osRead))
			{
				
				if (nBytesRead == size){
					read_flag=1;
					
					switch(nt) {
					case 1:							//Get Data DMT
						CheckFrameDMT_1(buf,Index);
						break;
					case 2:
						if ((buf[size-2]==0x20)&&(buf[size-1]==0x40))  
							CheckFrameALM(buf,Index);	//Get Data ALM
						break;
					case 3:
						if ((buf[size-2]==0x25)&&(buf[size-1]==0x00))  
							CheckFrameALM_1(buf,Index);	//get status ALM
						break;
					case 5:
						if ((buf[size-2]==0x08)&&(buf[size-1]==0x00))  
							CheckFrameDL60(buf,Index);	//get Data DL60
						break;
					case 6:
						if ((buf[size-2]==0x0d)&&(buf[size-1]==0x00))  
							CheckFrameDL60_1(buf,Index);	//get status DL60
						break;
					case 7:
						if ((buf[size-2]==0x38)&&(buf[size-1]==0x00))  
							CheckFrameGK76(buf,Index);	//get Data GK76
						break;
					case 8:
						if ((buf[size-2]==tool[1])&&(buf[size-1]==tool[2]))	
							CheckFrameGK76_1(buf,Index);	//get status GK76
						break;
					case 9:
						//if ((buf[size-2]==0x38)&&(buf[size-1]==0x00))	
							CheckFrameGK60(buf,Index);	//get Data GK60
						break;
					case 10:
					//	if ((buf[size-2]==0xaa)&&(buf[size-1]==0xaa))	
							CheckFrameSGK901(buf,Index);	//get Data SGK
						break;
					case 11:
						//if ((buf[size-2]== 0xf1)&&(buf[size-1]== 0x14)) 
							CheckFrameBK3(buf,Index);	//get Data BK3
						break;
					case 12:
						//if ((buf[size-2]== 0xe2)&&(buf[size-1]== 0x05)) 
							CheckFrameBK791(buf,Index);	//get Data BK79
						break;
					case 13:
						if ((buf[size-2]== 0x10)&&(buf[size-1]== 0x00))
							CheckFrameSKP(buf,Index);	//get Data SKP
						break;
					case 14:
						if ((buf[size-2]== 0x10)&&(buf[size-1]== 0x5e))
							CheckFrameSKP_1(buf,Index);	//get status SKP
						break;
					case 15:
						if ((buf[size-2]== 0x10)&&(buf[size-1]== 0xa0))
							CheckFrameSKP(buf,Index);	//get Data SKP
						break;
					case 16:
						if ((buf[size-2]== 0x10)&&(buf[size-1]== 0x50))
							CheckFrameSKP(buf,Index);	//get Data SKP
						break;
					case 17:
						if ((buf[size-2]== 0x10)&&(buf[size-1]== 0x0a))
							CheckFrameSKP(buf,Index);	//get Data SKP
						break;
					case 18:
						if ((buf[size-2]== 0x10)&&(buf[size-1]== 0x05))
							CheckFrameSKP(buf,Index);	//get Data SKP
						break;	
					case 23:
						if ((buf[size-2]== 0x1f)&&(buf[size-1]== 0x00))
							CheckFrameAK(buf,Index);	//get Data AK Digital
						break;
					case 24:
						if (buf[size-2]== 0x10) 
							CheckFrameMSC40(buf,Index);	//get Data MSC40
						break;
					case 25:
						if (buf[size-2]== 0x10)
							CheckFrameMSC40_1(buf,Index);	//get status MSC40
						break;
					case 26:
						if (buf[size-2]== 0x10)
							CheckFrameMSC40(buf,Index);	//get Data MSC40
						break;
					case 27:
						if (buf[size-2]== 0x10)
							CheckFrameMSC40(buf,Index);	//get Data MSC40
						break;
					case 28:
						if (buf[size-2]== 0x10)
							CheckFrameMSC40(buf,Index);	//get Data MSC40
						break;
					case 29:
						if (buf[size-2]== 0x10)
							CheckFrameMSC40(buf,Index);	//get Data MSC40
						break;
					case 30:
						
						CheckFrameCopySD(buf);	//Copy SD log.dxt file
						
						break;

					default: break;
					}
					if(nt != 30) {

						GetAllDataDMT();

						DisplayDataInPanel(pWnd);

						Index++;
						if(Index >= filter) {
							ReadyFlag = 1;
							Index = 0;		
						}
					}
				}				
			}
			else {

				WaitForSingleObject(osRead.hEvent,60); 
				GetOverlappedResult(m_cComm.m_hComm,&osRead,&nBytesRead,false);
				
				if (nBytesRead == size){
					read_flag=2;
					
					switch(nt) {
					case 1:							//Get Data DMT
						CheckFrameDMT_1(buf,Index);
						break;
					case 2:
						if ((buf[size-2]==0x20)&&(buf[size-1]==0x40))  
							CheckFrameALM(buf,Index);	//Get Data ALM
						break;
					case 3:
						if ((buf[size-2]==0x25)&&(buf[size-1]==0x00))  
							CheckFrameALM_1(buf,Index);	//get status ALM
						break;
					case 5:
						if ((buf[size-2]==0x08)&&(buf[size-1]==0x00))  
							CheckFrameDL60(buf,Index);	//get Data DL60
						break;
					case 6:
						if ((buf[size-2]==0x0d)&&(buf[size-1]==0x00))  
							CheckFrameDL60_1(buf,Index);	//get status DL60
						break;
					case 7:
						if ((buf[size-2]==0x38)&&(buf[size-1]==0x00))  
							CheckFrameGK76(buf,Index);	//get Data GK76
						break;
					case 8:
						//if ((buf[size-2]==tool[1])&&(buf[size-1]==tool[2]))	
							CheckFrameGK76_1(buf,Index);	//get status GK76
						break;
					case 9:
					//	if ((buf[size-2]==0x38)&&(buf[size-1]==0x00))	
							CheckFrameGK60(buf,Index);	//get Data GK60
						break;
					case 10:
					//	if ((buf[size-2]==0xaa)&&(buf[size-1]==0xaa))	
							CheckFrameSGK901(buf,Index);	//get Data SGK
						break;
					case 11:
					//	if ((buf[size-2]== 0xf1)&&(buf[size-1]== 0x14)) 
							CheckFrameBK3(buf,Index);	//get Data BK3
						break;
					case 12:
						//if ((buf[size-2]== 0xe2)&&(buf[size-1]== 0x05)) 
							CheckFrameBK791(buf,Index);	//get Data BK79
						break;
					case 13:
						if ((buf[size-2]== 0x10)&&(buf[size-1]== 0x00))
							CheckFrameSKP(buf,Index);	//get Data SKP
						break;
					case 14:
						if ((buf[size-2]== 0x10)&&(buf[size-1]== 0x5e))
							CheckFrameSKP_1(buf,Index);	//get status SKP
						break;
					case 15:
						if ((buf[size-2]== 0x10)&&(buf[size-1]== 0xa0))
							CheckFrameSKP(buf,Index);	//get Data SKP
						break;
					case 16:
						if ((buf[size-2]== 0x10)&&(buf[size-1]== 0x50))
							CheckFrameSKP(buf,Index);	//get Data SKP
						break;
					case 17:
						if ((buf[size-2]== 0x10)&&(buf[size-1]== 0x0a))
							CheckFrameSKP(buf,Index);	//get Data SKP
						break;
					case 18:
						if ((buf[size-2]== 0x10)&&(buf[size-1]== 0x05))
							CheckFrameSKP(buf,Index);	//get Data SKP
						break;	
					case 23:
						if ((buf[size-2]== 0x1f)&&(buf[size-1]== 0x00))
							CheckFrameAK(buf,Index);	//get Data AK Digital
						break;
					case 24:
						if (buf[size-2]== 0x10) 
							CheckFrameMSC40(buf,Index);	//get Data MSC40
						break;
					case 25:
						if (buf[size-2]== 0x10)
							CheckFrameMSC40_1(buf,Index);	//get status MSC40
						break;
					case 26:
						if (buf[size-2]== 0x10)
							CheckFrameMSC40(buf,Index);	//get Data MSC40
						break;
					case 27:
						if (buf[size-2]== 0x10)
							CheckFrameMSC40(buf,Index);	//get Data MSC40
						break;
					case 28:
						if (buf[size-2]== 0x10)
							CheckFrameMSC40(buf,Index);	//get Data MSC40
						break;
					case 29:
						if (buf[size-2]== 0x10)
							CheckFrameMSC40(buf,Index);	//get Data MSC40
						break;
					case 30:
						
						CheckFrameCopySD(buf);	//Copy SD log.dxt file
						
						break;

					default: break;
					}
					if(nt != 30) {

						GetAllDataDMT();

						DisplayDataInPanel(pWnd);

						Index++;
						if(Index >= filter) {
							ReadyFlag = 1;
							Index = 0;		
						}
					}
				}

			}
		}
	}//end while		
	
	if(pcFile) pcFile->Close();
	CloseHandle(osWrite.hEvent);
	CloseHandle(osRead.hEvent);
	
	
	return 0;

}



void CheckFrameMft(BYTE header,BYTE tailer,BYTE* str, int index)
{
	UINT i,it,id=0,j;
	
	if((str[0] == header) && (str[67] == tailer))
	{
		for(i=0;i<68;i++) 
		{
			BufData[i]=str[i];	
			
		}
		
	} 
	else
	{
		for(i=0;i<68;i++)
		{
			if((str[i+1] == header) && (str[i] == tailer)) 
				break;
		}
		if(i==68) return;
		for(j=0;j<68;j++)
		{
			if(j<67-i)
				BufData[j] = str[i+j+1];
			else
				BufData[j] = str[j+i-67];
		}
	}
	
	for (i=0;i<15;i++)
	{
		tempBuf[i] = BufData[4*i+1] ;
		tempBuf[i] <<= 8;
		tempBuf[i] |= BufData[i*4+2];
		tempBuf[i] &= 0x3fff;
		if(tempBuf[i] > 0x1fff) tempBuf[i] -= (0x3fff);
		pBufData[index].MA[i]=tempBuf[i];
	}	
	int temp,temp2;
	temp = BufData[59]&0x07; 
	temp >>= 1;
	pBufData[index].MD[0]=temp;
	
	temp = BufData[63]&0x3f;
	temp <<= 13;
	temp2 = BufData[64] ;
	temp2 <<=5;	
	temp |= temp2;
	temp2 = BufData[65] >> 3;
	temp |= temp2;
	pBufData[index].MD[2] = temp;	

	double width = temp;
	width /= 4088.6;
	width = 1000000.0/width;
	pBufData[index].dPressFreq = width;
	

		
	temp = BufData[61]&0x7f ;
	int aa = temp >> 3;
	BYTE bb = iByte((BYTE)aa);
	bb >>= 1;
	temp &= 0x07;
	temp |= bb;
	temp <<= 5;
	temp2 = BufData[62]  ;
	temp2 >>=3;
	temp |= temp2;
	
	pBufData[index].MD[1]=temp;	

	width = temp;
	width *= 1000000./61440.0;
	pBufData[index].dTempeFreq = width;
}


void CheckFrameDMT(BYTE header,BYTE tailer,BYTE* str, int index)
{
	UINT i,it,id=0,j,ui;
	double ft;
	
	if((str[0] == header) && (str[27] == tailer))
	{
		for(i=0;i<28;i++) 
		{
			BufData[i]=str[i];	
			
		}
	} 
	else
	{
		for(i=0;i<28;i++)
		{
			if((str[i+1] == header) && (str[i] == tailer)) 
				break;
		}
		if(i==28) return;

		for(j=0;j<28;j++)
		{
			if(j<27-i)
				BufData[j] = str[i+j+1];
			else
				BufData[j] = str[j+i-27];
		}
	}
	
	//get depth
	id = BufData[20]<<24;
	id += BufData[19]<<16;
	id += BufData[18]<<8;
	id += BufData[17];

	//get tension
	ft = BufData[21] + BufData[22]*256;
	ft *= 5.0/1024.0;
	RealData.fData[0] = ft;
	
	//get metka
	ft = BufData[23] + BufData[24]*256;
	ft *= 5.0/1024.0;
	RealData.fData[1] = ft;
	

	//get reserved
	ft = BufData[25] + BufData[26]*256;
	ft *= 5.0/1024.0;
	RealData.fData[2] = ft;

	
	RealData.fData[3] = info.fSpeed;

	for (i=4;i<24;i++)
	{
		RealData.fData[i] = 0 ;
	}

	 
	iDepth = id;		
}

int magindex=0;
float fave,mag[20],iave=0;

void CheckFrameDMT_1(BYTE* str, int index)
{
	UINT i,it,j,ui, isiz =  50;
	int id=0;
	short idt=0;
	double ft;

	if ((str[isiz-2] == 0xaa) &&(str[isiz-1] == 0xaa) )	//50 bytes
	
	{
	
	//for(i=0;i<34;i++) 	BufData[i]=str[i];	
			
	i=0;
		j=0;
		while(j< 50) 	{
			pTempDataDMT[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
	
	//sprintf(szTime,"%d/%d/%d %02d:%02d:%02d ",str[1],str[2],str[3],str[4],str[5],str[6]);
		
	
	//get depth
	id = str[19]<<24;
	id += str[18]<<16;
	id += str[17]<<8;
	id += str[16];
	iDepth = id;

	//get speed
	it = str[15]*1000;
	it += str[14]*100;
	it += str[13]*10;
	it += str[12];
	pBufData[index].Ain[47] = it;	//luu toc do
	pBufData[index].Ain[47] /=10;
	
	//get tension	
	ft = str[11]*10000;
	ft += str[10]*1000;
	ft += str[9]*100;
	ft += str[8]*10;
	ft += str[7];
	pBufData[index].Ain[0] = ft;	
	
	//get metka
	ft = str[22] + str[23]*256;
	ft *= 5.0/1024.0;
	pBufData[index].Ain[1] = ft;	
	pBufData[index].Ain[45] = ft;

	
	mag[magindex]=pBufData[index].Ain[1];
	magindex++;
	if(magindex > 19){
		magindex=0;
		iave=0;
		for(i=0;i<20;i++) iave +=mag[i];
		fave =iave;
		fave /=20;
		
	}
	
	//get last marked depth
	//ft = str[26] + str[27]*256;
	//pBufData[index].Ain[46] = ft;

	id = str[39]<<24;
	id += str[38]<<16;
	id += str[37]<<8;
	id += str[36];
	if(fabs(pBufData[index].Ain[1]- fave) >0.1) {
		pBufData[index].Ain[46] = iDepth;
		pBufData[index].Ain[46] /=fEncoderConst;
	}

	//ft *= 5.0/1024.0;
	//RealData.fData[4] = ft;
	
	//get ADC 5
	//ft = str[28] + str[29]*256;
	//pBufData[index].Ain[43] = ft;
	//ft *= 5.0/1024.0;
	//RealData.fData[5] = ft;

	//get ADC 6
	ft = str[30] + str[31]*256;
	pBufData[index].Ain[44] = ft;
	ft *= 5.0/1024.0;
	//RealData.fData[6] = ft;



/*
	for (i=4;i<24;i++)
	{
		RealData.fData[i] = 0 ;
	}

	 
	
	idt = str[33]<<8;
	idt += str[32];

	lreco = str[131]<<24;
	lreco += str[130]<<16;
	lreco += str[129]<<8;
	lreco += str[128];
	
	lcur = str[135]<<24;
	lcur += str[134]<<16;
	lcur += str[133]<<8;
	lcur += str[132];
	pBufData[index].Ain[0]=lcur;
	pBufData[index].Ain[1]=lreco;*/

	
	}
}

void CheckFrameDMT_2(BYTE* str, int index)		//depthtime
{
	UINT i,it,j,ui, isiz =  138;
	int id=0;
	short idt=0;
	double ft;

	if ((str[isiz-2] == 0xaa) &&(str[isiz-1] == 0xaa) )	//138 bytes
	
	{
	
	//for(i=0;i<34;i++) 	BufData[i]=str[i];	
			
	i=0;
		j=0;
		while(j< NUMCHANNEL) 	{
			pTempDataDMT[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
	
	sprintf(szTime,"%d/%d/%d %02d:%02d:%02d ",str[1],str[2],str[3],str[4],str[5],str[6]);
		
	//get depth
	id = str[19]<<24;
	id += str[18]<<16;
	id += str[17]<<8;
	id += str[16];

	//get speed
	it = str[15]*1000;
	it += str[14]*100;
	it += str[13]*10;
	it += str[12];

	//pBufData[index].Ain[18] = it;	//luu toc do

	it = str[37]<<24;
	it += str[36]<<16;
	it += str[35]<<8;
	it += str[34];


	pBufData[index].Ain[47] = it;
	//pBufData[index].Ain[47] /= fEncoderConst;	//luu toc do
	//pBufData[index].Ain[47] *=10;
	pBufData[index].Ain[47] /=10;

	//get tension
	ft = str[20] + str[21]*256;
	//ft = str[32] + str[33]*256;
	pBufData[index].Ain[0] = ft/10;
	
	
	//get metka
	ft = str[22] + str[23]*256;
	pBufData[index].Ain[1] = ft;
	
	

	//get volt AC
	ft = str[24] + str[25]*256;
	pBufData[index].Ain[45] = ft;
	pBufData[index].Ain[45] /= 10;


	//RealData.fData[17] = info.fSpeed;
	//RealData.fData[30] = info.fSpeed;

	//get ampere AC
	ft = str[26] + str[27]*256;
	pBufData[index].Ain[46] = ft;
	//ft *= 5.0/1024.0;
	//RealData.fData[4] = ft;
	
	//get ADC 5
	//ft = str[28] + str[29]*256;
	//pBufData[index].Ain[43] = ft;
	//ft *= 5.0/1024.0;
	//RealData.fData[5] = ft;

	//get ADC 6
	ft = str[30] + str[31]*256;
	pBufData[index].Ain[44] = ft;
	ft *= 5.0/1024.0;
	//RealData.fData[6] = ft;



/*
	for (i=4;i<24;i++)
	{
		RealData.fData[i] = 0 ;
	}

*/	 
	
	idt = str[33]<<8;
	idt += str[32];

	lreco = str[131]<<24;
	lreco += str[130]<<16;
	lreco += str[129]<<8;
	lreco += str[128];
	
	lcur = str[135]<<24;
	lcur += str[134]<<16;
	lcur += str[133]<<8;
	lcur += str[132];
	pBufData[index].Ain[0]=lcur;
	pBufData[index].Ain[1]=lreco;
	iDepth = id;
	}
}

void CheckFrameALM(BYTE* str, int index)
{
	UINT i=0,it,id=0,j;
	float ft;

	
	if ((str[48] == tool[1]) &&(str[49] == tool[2]) ) {

		i=0;
		j=0;
		while(j< NUMCHANNEL) 	{
			pTempDataALM[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
	
	//	ft=0;

	//	for (i=2;i<32;i++)	{
	//		ft += str[i];
			
	//	}
	//	pBufData[index].Ain[15]=ft;
	//	pBufData[index].Ain[14]=str[32];
	//	pBufData[index].Ain[13]=str[33];

	
	//	ft = str[0] + str[1]*256;
	//	pBufData[index].Ain[19] = ft;
	
	
	
		ft = str[2] ;
		pBufData[index].Ain[2]=ft;
	

		ft = str[3] ;
		pBufData[index].Ain[3]=ft;

		ft = str[0]+str[1];
		pBufData[index].Ain[4]=ft;

		ft = str[2]-str[3];
		pBufData[index].Ain[5]=ft;


		 

	
	}
	
	
	
	
}

long CheckFrameALM_1(BYTE* str, int index)
{
	UINT i,it,id=0,j;
	long sum=0;
	CString st="", sz;

	if ((str[48] != 0x25) &&(str[49] != 0x00) ) return 0;

	i=0;
		j=0;
		while(j< NUMCHANNEL) 	{
			pTempDataALM1[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
	
	for(i=2;i<10;i++) 			{
		BufData[i]=str[2*i]+str[2*i+1]*256;	
		sz.Format("%2x%2x ",str[2*i+1],str[2*i]);
		sum += str[2*i]+str[2*i+1]*256;	
		st += sz;
	}
	m_sALM_msg = st;
	
	sum -= (0xc0aa + 0x3f55 + 0xf731 + 0x8000 + 0x0000 + 0xffe0 + 0x08ce + 0x2020);
	return sum;
	
	
}

void CheckFrameDL60(BYTE* str, int index)
{
	UINT i,it,id=0,j;
	float ft,fg,fa;
	float h1, h2;


	
	//if ((str[32] == tool[1]) &&(str[33] == tool[2]) ) {
	
	if ((str[48] == 0x08) &&(str[49] == 0x00) ) {

		i=0;
		j=0;
		while(j< NUMCHANNEL) 	{
			pTempDataDL60[i] = str[j] + str[j+1]*256;
			
			j = j+2;
			i++;
		}

		ft = str[2] + str[3]*256;
		pBufData[index].Ain[6] = ft/10;		//DEVI

		pBufData[index].Ain[6] += 2;	

		ft = str[4] + str[5]*256;
		pBufData[index].Ain[7] = ft/10;		//ROTA

		ft = str[6] + str[7]*256;
		pBufData[index].Ain[8] = ft/10;		//AZIM



		ft = str[8] + str[9]*256;
		pBufData[index].Ain[9] = ft/1000;	//FNOR

		ft = str[10] + str[11]*256;
		pBufData[index].Ain[10] = ft/1000;	//ANOR

		ft = str[12] + str[13]*256;
		pBufData[index].Ain[11] = ft/10;	//ROTM
		//pBufData[index].Ain[11] = ft;	//ROTM

		
		//fa = 370.0/5000;
		//fa *=(ft+500);
		//pBufData[index].Ain[11] = ft+fa;

		ft = str[26] + str[27]*256;
		pBufData[index].Ain[18] = ft/10 - 100;		//TEMPERATURE
		
		
		// inc = atan(sqrt(Ax^2 + Ay^2) / Az)

		ft = str[14] + str[15]*256 ;
	
		if(ft>32768) ft -= 65535;
		pBufData[index].Ain[12] = ft;		//RACX

		ft = str[16] + str[17]*256;
	
		if(ft>32768) ft -= 65535;
		pBufData[index].Ain[13] = ft;		//RACY

		ft = str[18] + str[19]*256;
	
		if(ft>32768) ft -= 65535;
		pBufData[index].Ain[14] = ft;		//RACZ

		//Normalize

		ft = pBufData[index].Ain[12]*pBufData[index].Ain[12] + pBufData[index].Ain[13]*pBufData[index].Ain[13] + 
			pBufData[index].Ain[14]*pBufData[index].Ain[14];
	
		ft = sqrt(ft);

		pBufData[index].Ain[12] /= ft;
		pBufData[index].Ain[13] /= ft;
		pBufData[index].Ain[14] /= ft;

	/*	ft = pBufData[index].Ain[12]*pBufData[index].Ain[12];
		ft += pBufData[index].Ain[13]*pBufData[index].Ain[13];
		ft = sqrt(ft);

		if (pBufData[index].Ain[14] != 0) {
			ft /= pBufData[index].Ain[14];
			ft = atan(ft);
			ft = ft*180/3.14159265;
			if(ft < 0) ft += 180;
			

			pBufData[index].Ain[24] = ft;	//Inc channel
			
			
		}
		
		else 	pBufData[index].Ain[24] = 90;*/

		ft = acos(-pBufData[index].Ain[14]);
		ft = ft*180/3.14159265;
		pBufData[index].Ain[24] = ft+2;	//Inc channel
		
		//Bering 

		// h1 = ay; h2 = ax; bering = atan(ay/ax);

		/*h1 = pBufData[index].Ain[13];
		h2 = pBufData[index].Ain[12];
		fg = h1/h2;
		fg = atan(fg);
		fg = fg*180/3.14159265;

		
		
		if(h2>0){

			fg += 180;
		}
		
		else if((h1>0)&&(h2<0)) {
			fg += 360;
		} */

		ft = atan2(pBufData[index].Ain[13],-pBufData[index].Ain[12]);
		ft = ft*180/3.14159265;
		if(ft<0) ft +=360;
		
		pBufData[index].Ain[25] = ft ;//- coef_inc;

	//	if(pBufData[index].Ain[25]>360) pBufData[index].Ain[25] -=360;

		//Calculate Azimuth


		/*h1 = ax*fy - ay*fx;

		h2 = fz-az*(ax*fx+ay*fy+ az*fz;)

		fg = h1/h2;
		fg = atan(fg);
		fg = fg*180/3.14159265;
		
		if((h1>0)&&(h2<0)) fg = 180 + fg;
		else if((h1<=0)&&(h2<0)) fg = 180 + fg;
		else if((h1<0)&&(h2>0)) fg = 360 + fg;

		RealData.fData[10] = 360- fg;// - coef_inc;
		if(RealData.fData[10]<0) RealData.fData[10] += 360;*/

		/*
		//ft = str[20] + str[21]*256 - 32768;
		//ft = str[22] + str[23]*256 - 32768;
		//ft = str[24] + str[25]*256 - 32768;
		pBufData[index].Ain[15] = ft;		//RFX

		//ft = str[20] + str[21]*256 - 32768;
		ft = str[22] + str[23]*256 - 32768;
		//ft = str[24] + str[25]*256 - 32768;
		pBufData[index].Ain[16] = ft;		//RFY

		//ft = str[20] + str[21]*256 - 32768;
		//ft = str[22] + str[23]*256 - 32768;
		ft = str[24] + str[25]*256 - 32768;
		
		pBufData[index].Ain[17] = ft;		//RFZ
		*/


	    ft = str[20] + str[21]*256;

		if(ft>32768) ft -= 65535;
		pBufData[index].Ain[15] = ft;		//RFX


		ft = str[22] + str[23]*256;

		if(ft>32768) ft -= 65535;
		pBufData[index].Ain[16] = ft;		//RFY


		ft = str[24] + str[25]*256;

		if(ft>32768) ft -= 65535;
		pBufData[index].Ain[17] = ft;		//RFZ

  

		//Normalize

		ft = pBufData[index].Ain[15]*pBufData[index].Ain[15] 
			+ pBufData[index].Ain[16]*pBufData[index].Ain[16]
			+ pBufData[index].Ain[17]*pBufData[index].Ain[17];

		ft = sqrt(ft);

		pBufData[index].Ain[15] /= ft;
		pBufData[index].Ain[16] /= ft;
		pBufData[index].Ain[17] /= ft;

		

		// h1 = ax*fy - ay*fx;
		// h2 = fz - az*(ax*fx+ay*fy+az*fz);
		// azi = atan(h1/h2);

		h1 = pBufData[index].Ain[12]*pBufData[index].Ain[16] - pBufData[index].Ain[13]*pBufData[index].Ain[15];

		float sinL = pBufData[index].Ain[12]*pBufData[index].Ain[15] 
					+ pBufData[index].Ain[13]*pBufData[index].Ain[16]
					+ pBufData[index].Ain[14]*pBufData[index].Ain[17];

		sinL *= pBufData[index].Ain[14];
		
		h2 = pBufData[index].Ain[17] - sinL;

		//fg = h1/h2;
		fg = atan2(h1,h2);
		fg = fg*180/3.14159265;
		
		/*if(h2<0){

			fg += 180;
		}
		
		if((h1>0)&&(h2<0)) fg = 180 + fg;
		else if((h1<=0)&&(h2<0)) fg = 180 + fg;

		else if((h1<0)&&(h2>0)) {
			fg += 360;
		}*/

		//pBufData[index].Ain[26] = 360- fg;// - coef_inc;
		pBufData[index].Ain[26] = fg;// - coef_inc;
		if(pBufData[index].Ain[26]<0) pBufData[index].Ain[26] += 360;

	}
	
	
	
	
}

long CheckFrameDL60_1(BYTE* str, int index)
{
	UINT i,it,id=0,j;
	long sum=0;
	CString st="", sz;

	if ((str[48] != 0x0d) &&(str[49] != 0x00) ) return 0;
	i=0;
		j=0;
		while(j< NUMCHANNEL) 	{
			pTempDataDL601[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
	for(i=0;i<10;i++) 			{
		BufData[i]=str[2*i]+str[2*i+1]*256;	
		sz.Format("%2x%2x ",str[2*i+1],str[2*i]);
		sum += str[2*i]+str[2*i+1]*256;	
		st += sz;
	}
	m_sDL60_msg = st;
	sum -= (0x0800 + 0x001c + 0xc0aa + 0x3f55 + 0xf731 + 0x8000 + 0x0000 + 0xffff + 0x08ce + 0x0808);

	return sum;
	
	
}



void CheckFrameBK3(BYTE* str, int index)
{
	UINT i,it,id=0,ix,j,ku=1,itemp;
	float ft;

	
//	if ((str[48] == 0xf1) &&(str[49] == 0x14) ) {

	
		i=0;
		j=0;
		while(j< 520) 	{
			pTempDataBK3[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
	//	pBufData[index].Ain[15]=ft;
	//	pBufData[index].Ain[14]=str[32];
	//	pBufData[index].Ain[13]=str[33];

		it = str[4] + str[5]*256;	//lay 2 bytes
		
		itemp = it & 0x0fff;		//lay adc 11 bits thap
		id = it & 0x7000;			//lay 3 bits gain
		id = id /4096;
		

		if (id == 0)	ku=128;
		else if (id==1)	ku=16;
		else if (id==2)	ku=136;
		else if (id==3)	ku=2;
		else if (id==4)	ku=1;

		ft = itemp*ku;

		

	
		pBufData[index].Ain[19] = ft;	//UAE 

//		pBufData[index].Ain[22] = id;	//IAE

		it = str[6] + str[7]*256;

		itemp = it & 0x0fff;		//lay adc 11 bits thap
		id = it & 0x7000;			//lay 3 bits gain
		id = id /4096;
		

		if (id == 0)	ku=128;
		else if (id==1)	ku=16;
		else if (id==2)	ku=136;
		else if (id==3)	ku=2;
		else if (id==4)	ku=1;

		ft = itemp*ku;

		pBufData[index].Ain[20] = ft;	//IAE

	//	pBufData[index].Ain[23] = id;	//IAE

		
		
		it = str[8] + str[9]*256;
		
		ix = it & 0x8000;

		ix >>= 15;

		itemp = it & 0x7fff;

		ft = itemp;

			
		if (ix) ft *= -1;

		pBufData[index].Ain[21] = ft;	//IA0

	
		it = str[10] + str[11]*256;

		itemp = it & 0x0fff;		//lay adc 12 bits thap
		id = it & 0x7000;			//lay 3 bits gain
		id = id /4096;

		if (id == 0)	ku=128;
		else if (id==1)	ku=16;
		else if (id==2)	ku=136;
		else if (id==3)	ku=2;
		else if (id==4)	ku=1;

		ft = itemp*ku;

		if (it > 0xffff) ft *= -1;

		pBufData[index].Ain[22] = ft;	//UA8M1N

		pBufData[index].Ain[40] = id;	//TCODE

		it = str[12] + str[13]*256;
		
		itemp = it & 0x0fff;		//lay adc 12 bits thap
		id = it & 0x7000;			//lay 3 bits gain
		id = id /4096;

		if (id == 0)	ku=128;
		else if (id==1)	ku=16;
		else if (id==2)	ku=136;
		else if (id==3)	ku=2;
		else if (id==4)	ku=1;

		ft = itemp*ku;

		if (it > 0xffff) ft *= -1;

		pBufData[index].Ain[23] = ft;	//UA4M05N

		pBufData[index].Ain[39] = id;	//TCODE

		it = str[14] + str[15]*256;
		
		itemp = it & 0x0fff;		//lay adc 12 bits thap
		id = it & 0x7000;			//lay 3 bits gain
		id = id /4096;

		if (id == 0)	ku=128;
		else if (id==1)	ku=16;
		else if (id==2)	ku=136;
		else if (id==3)	ku=2;
		else if (id==4)	ku=1;

		ft = itemp*ku;

		if (it > 0xffff) ft *= -1;

		pBufData[index].Ain[24] = ft;	//URES

		it = str[16] + str[17]*256;

		itemp = it & 0x0fff;		//lay adc 12 bits thap
		id = it & 0x7000;			//lay 3 bits gain
		id = id /4096;

		if (id == 0)	ku=128;
		else if (id==1)	ku=16;
		else if (id==2)	ku=136;
		else if (id==3)	ku=2;
		else if (id==4)	ku=1;

		ft = itemp*ku;

		if (it > 0xffff) ft *= -1;

		pBufData[index].Ain[25] = ft;	//IE15

		it = str[18] + str[19]*256;
		
		itemp = it & 0x0fff;		//lay adc 12 bits thap
		id = it & 0x7000;			//lay 3 bits gain
		id = id /4096;

		if (id == 0)	ku=128;
		else if (id==1)	ku=16;
		else if (id==2)	ku=136;
		else if (id==3)	ku=2;
		else if (id==4)	ku=1;

		ft = itemp*ku;

		if (it > 0xffff) ft *= -1;

		pBufData[index].Ain[26] = ft;	//UA2M05N	ZOND A2
		
		itemp = it & 0x0fff;		//lay adc 12 bits thap
		id = it & 0x7000;			//lay 3 bits gain
		id = id /4096;

		if (id == 0)	ku=128;
		else if (id==1)	ku=16;
		else if (id==2)	ku=136;
		else if (id==3)	ku=2;
		else if (id==4)	ku=1;

		ft = itemp*ku;

		if (it > 0xffff) ft *= -1;

		pBufData[index].Ain[27] = ft;	//UN05M2A	ZOND N05

		it = str[22] + str[23]*256;
		
		itemp = it & 0x0fff;		//lay adc 12 bits thap
		id = it & 0x7000;			//lay 3 bits gain
		id = id /4096;

		if (id == 0)	ku=128;
		else if (id==1)	ku=16;
		else if (id==2)	ku=136;
		else if (id==3)	ku=2;
		else if (id==4)	ku=1;

		ft = itemp*ku;

		if (it > 0xffff) ft *= -1;

		pBufData[index].Ain[28] = ft;	//UA1M01N

		it = str[24] + str[25]*256;
		
		itemp = it & 0x0fff;		//lay adc 12 bits thap
		id = it & 0x7000;			//lay 3 bits gain
		id = id /4096;

		if (id == 0)	ku=128;
		else if (id==1)	ku=16;
		else if (id==2)	ku=136;
		else if (id==3)	ku=2;
		else if (id==4)	ku=1;

		ft = itemp*ku;

		if (it > 0xffff) ft *= -1;

		pBufData[index].Ain[29] = ft;	//UA04M01N

		itemp = it & 0x0fff;		//lay adc 12 bits thap
		id = it & 0x7000;			//lay 3 bits gain
		id = id /4096;

		if (id == 0)	ku=128;
		else if (id==1)	ku=16;
		else if (id==2)	ku=136;
		else if (id==3)	ku=2;
		else if (id==4)	ku=1;

		ft = itemp*ku;

		if (it > 0xffff) ft *= -1;

		pBufData[index].Ain[30] = ft;	//UN6M05N   ZOND THE

		it = str[28] + str[29]*256;
		
		itemp = it & 0x0fff;		//lay adc 12 bits thap
		id = it & 0x7000;			//lay 3 bits gain
		id = id /4096;

		if (id == 0)	ku=128;
		else if (id==1)	ku=16;
		else if (id==2)	ku=136;
		else if (id==3)	ku=2;
		else if (id==4)	ku=1;

		ft = itemp*ku;

		if (it > 0xffff) ft *= -1;

		pBufData[index].Ain[31] = ft;	//IE8

		/////

		it = str[30] + str[31]*256;

		ix = it & 0x8000;

		ix >>= 15;

		itemp = it & 0x7fff;

		ft = itemp;

			
		if (ix) ft *= -1;

		pBufData[index].Ain[32] = ft;	//UPS

		it = str[34] + str[35]*256;

		ix = it & 0x8000;

		ix >>= 15;

		itemp = it & 0x7fff;

		ft = itemp;

			
		if (ix) ft *= -1;

		pBufData[index].Ain[33] = ft;	//UDPS

		/////

		it = str[38] + str[39]*256;

		ix = it & 0x0fff;				//lay adc 12 bits
		


		pBufData[index].Ain[34] = ix;	//UTEMP

		it = str[40] + str[41]*256;

	//	ix = it & 0x0fff;				//lay adc 12 bits
		

		pBufData[index].Ain[35] = it;	//ITEMP

		ft= str[42] + str[43]*256;
		
	//	ix = it & 0x2000;
	//	ix /= 4096;

	//	ft = 1;
	//	if (ix) ft = 8;
		
		pBufData[index].Ain[36] = ft;	//KA0

		ft = pBufData[index].Ain[19]/pBufData[index].Ain[21];
		pBufData[index].Ain[37] = ft;	//RBK3

		
	
	

	
	//}
	
	
	
	
}


long CheckFrameBK3_1(BYTE* str, int index)
{
	UINT i,it,id=0,j;
	long sum=0;
	CString st="", sz;

	if ((str[48] != 0x25) &&(str[49] != 0x00) ) return 0;
	i=0;
		j=0;
		while(j< NUMCHANNEL) 	{
			pTempDataBK31[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
	
	for(i=2;i<10;i++) 			{
		BufData[i]=str[2*i]+str[2*i+1]*256;	
		sz.Format("%2x%2x ",str[2*i+1],str[2*i]);
		sum += str[2*i]+str[2*i+1]*256;	
		st += sz;
	}
	m_sBK3_msg = st;
	
	sum -= (0xc0aa + 0x3f55 + 0xf731 + 0x8000 + 0x0000 + 0xffe0 + 0x08ce + 0x2020);
	return sum;
	
	
}




void CheckFrameGK76(BYTE* str, int index)
{
	UINT i,it,id=0,j;
	float ft;

	
	if ((str[48] == tool[1]) &&(str[49] == tool[2]) ) {

		i=0;
		j=0;
		while(j< NUMCHANNEL) 	{
			pTempDataGK76[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
		
		pBufData[index].Ain[42] = pTempDataGK76[1]*300/dtgk;
		pBufData[index].Ain[43] = pTempDataGK76[2];


	//	pBufData[index].Ain[15]=ft;
	//	pBufData[index].Ain[14]=str[32];
	//	pBufData[index].Ain[13]=str[33];

	
	/*	ft = str[0] + str[1]*256;
		pBufData[index].Ain[19] = ft;
	
	
	
		ft = str[2] ;
		pBufData[index].Ain[2]=ft;
	

		ft = str[3] ;
		pBufData[index].Ain[3]=ft;

		ft = str[2]+str[3];
		pBufData[index].Ain[4]=ft;

		ft = str[2]-str[3];
		pBufData[index].Ain[5]=ft;*/

	
	}
	
	
	
	
}

long CheckFrameGK76_1(BYTE* str, int index)
{
	UINT i,it,id=0,j;
	long sum=0;
	CString st="", sz;

	if ((str[48] != 0x38) &&(str[49] != 0x09) ) return 0;

	i=0;
		j=0;
		while(j< NUMCHANNEL) 	{
			pTempDataGK761[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}

	for(i=2;i<10;i++) 			{
		BufData[i]=str[2*i]+str[2*i+1]*256;	
		sz.Format("%2x%2x ",str[2*i+1],str[2*i]);
		sum += str[2*i]+str[2*i+1]*256;	
		st += sz;
	}
	m_sGK76_msg = st;
	
	sum -= (0xc0aa + 0x3f55 + 0xf731 + 0x8000 + 0x0000 + 0xffe0 + 0x08ce + 0x2020);
	return sum;
	
	
}

void CheckFrameGK60(BYTE* str, int index)
{
	UINT i,it,id=0,j;
	float ft;

	
	if ((str[48] == tool[1]) &&(str[49] == tool[2]) ) {

		i=0;
		j=0;
		while(j< NUMCHANNEL) 	{
			pTempDataGK60[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
		
		pBufData[index].Ain[42] = pTempDataGK60[0];
		pBufData[index].Ain[43] = pTempDataGK60[3];


	//	pBufData[index].Ain[15]=ft;
	//	pBufData[index].Ain[14]=str[32];
	//	pBufData[index].Ain[13]=str[33];

	
	/*	ft = str[0] + str[1]*256;
		pBufData[index].Ain[19] = ft;
	
	
	
		ft = str[2] ;
		pBufData[index].Ain[2]=ft;
	

		ft = str[3] ;
		pBufData[index].Ain[3]=ft;

		ft = str[2]+str[3];
		pBufData[index].Ain[4]=ft;

		ft = str[2]-str[3];
		pBufData[index].Ain[5]=ft;*/

	
	}
	
	
	
	
}

long CheckFrameGK60_1(BYTE* str, int index)
{
	UINT i,it,id=0,j;
	long sum=0;
	CString st="", sz;

	if ((str[48] != 0x38) &&(str[49] != 0xff) ) return 0;

	i=0;
		j=0;
		while(j< NUMCHANNEL) 	{
			pTempDataGK601[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}

	for(i=2;i<10;i++) 			{
		BufData[i]=str[2*i]+str[2*i+1]*256;	
		sz.Format("%2x%2x ",str[2*i+1],str[2*i]);
		sum += str[2*i]+str[2*i+1]*256;	
		st += sz;
	}
	m_sGK60_msg = st;
	
	sum -= (0xc0aa + 0x3f55 + 0xf731 + 0x8000 + 0x0000 + 0xffe0 + 0x08ce + 0x2020);
	return sum;
	
	
}

int iTemp[256];

void CheckFrameSGK901(BYTE* str, int index)
{
	UINT i,id=0,j,k;
	int it=0;
	float ft;

	//if ((str[523] != 0xaa) &&(str[522] != 0xaa) ) return ;

		i=0;
		j=0;
		while(j< 520) 	{
			iSGK[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
	//get depth
	id = str[523]<<24;
	id += str[522]<<16;
	id += str[521]<<8;
	id += str[520];

	iDepth = id;
	
	//get T, tension
	pBufData[index].Ain[0] = str[512] + str[513]*256;
	pBufData[index].Ain[0] /=10;
	
	//get volt AC
	pBufData[index].Ain[45] = str[514] + str[515]*256;
	pBufData[index].Ain[45] /= 10;

	//get ampere AC
	pBufData[index].Ain[46] = str[516] + str[517]*256;

	//get speed, vbat	
	pBufData[index].Ain[47] = str[518] + str[519]*256;
	pBufData[index].Ain[47] /=10;
	

	for(i=5;i<196;i++){
		if(iSGK[i] > 14000) {		//32768	
			break;
		}
	}
	k = i/13;
	it = 13*k + 8 - i;	


	for(i=0;i<256;i++){
		
			iTemp[i] = iSGK[i-it];
	}
	for(i=0;i<256;i++){
		iSGK[i] = iTemp[i];
	}
	
	
	pBufData[index].Ain[10]	= iSGK[151];				
	pBufData[index].Ain[15] = iSGK[169]+iSGK[222];				//Thori
	pBufData[index].Ain[16] = iSGK[161]+iSGK[172]+iSGK[187];				//Uran
	pBufData[index].Ain[17] = iSGK[181];				//Kali
	pBufData[index].Ain[42]	= pBufData[index].Ain[15]+pBufData[index].Ain[16]+pBufData[index].Ain[17];		//gk

	for(i=155;i<164;i++){
		
		pBufData[index].Ain[42] += iSGK[i];

	}
	for(i=165;i<177;i++){
		
		pBufData[index].Ain[42] += iSGK[i];

	}
	
	

	

	

	

	
	
	
}

int nz[9];

void CheckFrameBK791(BYTE* str, int index)
{
	
	UINT i,it,id=0,j;
	float ft;

	//if ((str[521] != 0xe2) &&(str[520] != 0x05) ) return ;

		i=0;
		j=0;
		while(j< 20) 	{
		//	iBK[i] = str[j] + str[j+1]*256;

			pTempDataBK31[i] = str[j+1]*256 + str[j];
			j = j+2;
			i++;
		}

	//get depth
	id = str[523]<<24;
	id += str[522]<<16;
	id += str[521]<<8;
	id += str[520];

	iDepth = id;
	
	//get T, tension
	pBufData[index].Ain[0] = str[513]*256 + str[512];
	pBufData[index].Ain[0] /=10;
	
	//get volt AC
	pBufData[index].Ain[45] = str[515]*256 + str[514];
	pBufData[index].Ain[45] /= 10;

	//get ampere AC
	pBufData[index].Ain[46] = str[517]*256 + str[516];

	//get speed, vbat	
	pBufData[index].Ain[47] = str[519]*256 + str[518];
	pBufData[index].Ain[47] /=10;

	//get ampere AC	
	//if(str[8] != 0)  pBufData[index].Ain[45] = pTempDataBK31[9];
	//for(i=0;i<9;i++){
//		nz[i] =32768;
//	}
	for(i=0;i<9;i++){
		//if(nz[i] > pTempDataBK31[i]) nz[i] = pTempDataBK31[i];
		pTempDataBK31[i] = pTempDataBK31[i] ;//- 32768;
	}

/*	if (pTempDataBK31[1]< 32768) pTempDataBK31[1] = 32769;
	if (pTempDataBK31[2]< 32768) pTempDataBK31[2] = 32769;
	if (pTempDataBK31[3]< 32768) pTempDataBK31[3] = 32769;
	if (pTempDataBK31[4]< 32768) pTempDataBK31[4] = 32769;
*/		
	pBufData[index].Ain[32] = pTempDataBK31[8]- 32768;		//PS
	pBufData[index].Ain[19] = pTempDataBK31[3]- 32768;		//UBK7
	pBufData[index].Ain[20] = pTempDataBK31[1]- 32768;		//IBK7
	pBufData[index].Ain[21] = pTempDataBK31[4]- 32768;		//UBK9
	pBufData[index].Ain[22] = pTempDataBK31[2]- 32768;		//IBK9
	
	//double ftt = pTempDataBK31[3]- 32768;
	//ftt /= (pTempDataBK31[1]- 32768);
	if(pBufData[index].Ain[20])
	pBufData[index].Ain[36] = 10*pBufData[index].Ain[19]/pBufData[index].Ain[20];					//RBK7
		
	//ftt = pTempDataBK31[4]- 32768;
	//ftt /= (pTempDataBK31[2]- 32768);
	if(pBufData[index].Ain[22])
	pBufData[index].Ain[37] = 100*pBufData[index].Ain[21]/pBufData[index].Ain[22];					//RBK9
	

		
}


void CheckFrameCopySD(BYTE* str)
{
	
	UINT i,it,id=0,j;
	float ft;
	
	CString sz,sc,ss;
	nresp++;

	

	if(saq == 0){					//ghi dau bang
		ss.Format("%s",str);
		id=ss.Find("DT");	
		if(id == 0) {
			saq = 1;			
			pcFile->Write(ss,ss.GetLength());
		}
	}

	else {								//ghi du lieu
		for(i=0;i<22;i++){
			sz.Format("%c",str[i]);
			sc += sz;
			if(str[i] == 0xff)	{		// cuoi file		
			
				copy_flag=0;
				bSendOK = false;
				load_flag=1;
				saq=0;				
				break;
			}		
		
		}
	
		//sc.Format("%s",str);
		
		pcFile->Write(sc,sc.GetLength());	
		if(!copy_flag) {
			pcFile->Close();
		
			
		}
	}
	/*

	
		for(i=0;i<22;i++){
			sz.Format("%c",str[i]);
			sc += sz;
			if(str[i] == 0xff)	{		
			
				copy_flag=0;
				bSendOK = false;
				load_flag=1;
				saq=0;			
				break;
			}	
		
		}		
		
		
		
		pcFile->Write(sc,sc.GetLength());	
		if(!copy_flag) {
			pcFile->Close();
		
		}	
		
	
	*/

}

float fOpenClose = 0;

void CheckFrameSKP(BYTE* str, int index)
{
	UINT i,it,id=0,j;
	float ft;
	static int nc=0;
	
	if ((str[48] == 0x10) ) {

		i=0;
		j=0;

		while(j< NUMCHANNEL) 	{
			pTempDataSKP[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
		
		float fa = pTempDataSKP[i]&0x0fff;
		float fb = pTempDataSKP[5]&0x0fff;

		
		for(i=0;i<8;i++){
		
			
			fTempDataSKP[i] = pTempDataSKP[i]&0x0fff;

		


		}
		
		fTempDataSKP[8] = pTempDataSKP[0]>>14;
		fTempDataSKP[9] = pTempDataSKP[9];
		fTempDataSKP[10] = pTempDataSKP[10];

		pBufData[index].Ain[38] = fTempDataSKP[1] + fTempDataSKP[2];
		pBufData[index].Ain[39] = fTempDataSKP[3] + fTempDataSKP[4];
		pBufData[index].Ain[40] = (fTempDataSKP[1] + fTempDataSKP[3] + fTempDataSKP[2] + fTempDataSKP[4])/2;
		pBufData[index].Ain[41] = fTempDataSKP[7];

		if((fOpenClose == 3)&(fTempDataSKP[8] == 1)) {
			mbClose = 0;
			runingArm=1;
			
			
		}
		else if((fOpenClose == 3)&(fTempDataSKP[8] == 2)) {
			mbOpen = 0;
			runingArm=2;
			
		}

		
		fOpenClose = fTempDataSKP[8];
		
		
	
	}
	
	
	
	
}

long CheckFrameSKP_1(BYTE* str, int index)
{
	UINT i,it,id=0,j;
	long sum=0;
	CString st="", sz;

	if ((str[48] != 0x10) &&(str[49] != 0x5e) ) return 0;
	i=0;
		j=0;
		while(j< NUMCHANNEL) 	{
			pTempDataSKP1[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
	for(i=0;i<9;i++) 			{
		BufData[i]=str[2*i]+str[2*i+1]*256;	
		sz.Format("%2x%2x ",str[2*i+1],str[2*i]);
		sum += str[2*i]+str[2*i+1]*256;	
		st += sz;
	}
	m_sSKP_msg = st;
	
	sum -= (0x0014 + 0xc0aa + 0x3f55 + 0xf731 + 0x8000 + 0x1010 + 0xffff + 0x08ce);
	return sum;
	
	
}
int nc=0,xmax=8010,xmin=290,ymax=7970,ymin=390;
void CheckFrameMSC40(BYTE* str, int index)
{
	UINT i,it,id=0,j,irot=0,isiz=96;
	float ft,fave=0,fmax=0,fmin=1024,zmax=412,zmin=75;
	//static int nc=0,xmax=8010,xmin=290,ymax=7970,ymin=390;

	if(msc60_flag) {
		isiz = 136;
		xmax=8220;
		xmin=1350;
		ymax=7860;
		ymin=1410;
		zmax=412;
		zmin=75;
	}
	
	if ((str[isiz] == 0x10) ) {

		i=0;
		j=0;

		while(j< isiz) 	{
			pTempDataMSC[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
		
		
		if(msc60_flag==0){	
			for(i=0;i<48;i++){

			
				if (i==41){
					if(pTempDataMSC[41]> zmax) zmax = pTempDataMSC[41];
					if(pTempDataMSC[41] < zmin) zmin = pTempDataMSC[41];

					ft = zmax + zmin;
					ft /= 2;

					
					fTempDataMSC[41] = pTempDataMSC[41]-ft;


					ft = zmax - zmin;
					ft /= 2;
					fTempDataMSC[41] /= ft;
				}
				else if (i==45){
					if((pTempDataMSC[45]> xmax)&&(pTempDataMSC[45]<10240)) xmax = pTempDataMSC[45];
					if((pTempDataMSC[45] < xmin)&&(pTempDataMSC[45]>0)) xmin = pTempDataMSC[45];

					ft = xmax + xmin;
					ft /= 2;

					
					fTempDataMSC[45] = pTempDataMSC[45]-ft;


					ft = xmax - xmin;
					ft /= 2;
					fTempDataMSC[45] /= ft;
				}

				else if (i==46){
					if((pTempDataMSC[46]> ymax)&&(pTempDataMSC[46]<10240)) ymax = pTempDataMSC[46];
					if((pTempDataMSC[46] < ymin)&&(pTempDataMSC[46]>0)) ymin = pTempDataMSC[46];

					ft = ymax + ymin;
					ft /= 2;

					fTempDataMSC[46] = pTempDataMSC[46]-ft;
					
					ft = ymax - ymin;
					ft /= 2;
					fTempDataMSC[46] /= ft;
				}

				else fTempDataMSC[i] = pTempDataMSC[i];

			
		
				
				if(i<40) 						
						pBufData[index].Ain[48+i]= fTempDataMSC[i];
				//else
				//		pBufData[index].Ain[108+i]= fTempDataMSC[i];
				
			
			
				//pBufData[index].Ain[48+i]= fTempDataMSC[i];


			}
			//for(i=0;i<40;i++){
				//pBufData[index].Ain[48+i]= fTempDataMSC[i];
		//	}
			
			pBufData[index].Ain[108]= fTempDataMSC[40];		//dien a dau may
			pBufData[index].Ain[109]= fTempDataMSC[41];
			pBufData[index].Ain[110]= fTempDataMSC[42];
			pBufData[index].Ain[111]= fTempDataMSC[43];		//nhiet do trong
			pBufData[index].Ain[112]= fTempDataMSC[44];		//nhiet do ngoai
			pBufData[index].Ain[113]= fTempDataMSC[45];		//nhiet do trong
			pBufData[index].Ain[114]= fTempDataMSC[46];		//nhiet do ngoai

			ft = fTempDataMSC[45]*fTempDataMSC[45]+fTempDataMSC[46]*fTempDataMSC[46];
	
			ft = sqrt(ft);

			pBufData[index].Ain[113] /= ft;
			pBufData[index].Ain[114] /= ft;
			

			ft = atan2(pBufData[index].Ain[114],-pBufData[index].Ain[113]);
		
			//ft = atan2(fTempDataMSC[46],-fTempDataMSC[45]);
			ft = ft*180/3.14159265;
			if(ft<0) ft +=360;
			
		
			pBufData[index].Ain[115]=ft;

			
		



		}
		else {	
			for(i=0;i<68;i++){

			
				if (i==61){
					if(pTempDataMSC[61]> zmax) zmax = pTempDataMSC[61];
					if(pTempDataMSC[61] < zmin) zmin = pTempDataMSC[61];

					ft = zmax + zmin;
					ft /= 2;

					
					fTempDataMSC[61] = pTempDataMSC[61]-ft;


					ft = zmax - zmin;
					ft /= 2;
					fTempDataMSC[61] /= ft;
				}
				else if (i==65){
					if((pTempDataMSC[65]> xmax)&&(pTempDataMSC[65]<10240)) xmax = pTempDataMSC[65];
					if((pTempDataMSC[65] < xmin)&&(pTempDataMSC[65]>0)) xmin = pTempDataMSC[65];

					ft = xmax + xmin;
					ft /= 2;

					
					fTempDataMSC[65] = pTempDataMSC[65]-ft;


					ft = xmax - xmin;
					ft /= 2;
					fTempDataMSC[65] /= ft;
					
				}

				else if (i==66){
					if((pTempDataMSC[66]> ymax)&&(pTempDataMSC[66]<10240)) ymax = pTempDataMSC[66];
					if((pTempDataMSC[66] < ymin)&&(pTempDataMSC[66]>0)) ymin = pTempDataMSC[66];

					ft = ymax + ymin;
					ft /= 2;

					fTempDataMSC[66] = pTempDataMSC[66]-ft;
					
					ft = ymax - ymin;
					ft /= 2;
					fTempDataMSC[66] /= ft;
					
				}

				else 
					fTempDataMSC[i] = pTempDataMSC[i];

			
		
		
			
						pBufData[index].Ain[48+i]= fTempDataMSC[i];
					
				
				
		
				


			}
		

		//ft = fTempDataMSC[65]*fTempDataMSC[65]+fTempDataMSC[66]*fTempDataMSC[66]+fTempDataMSC[61]*fTempDataMSC[61];
	
		//ft = sqrt(ft);
	//	pBufData[index].Ain[109] /= ft;
	//	pBufData[index].Ain[113] /= ft;
	//	pBufData[index].Ain[114] /= ft;
		
	//	ft = atan2(pBufData[index].Ain[114],-pBufData[index].Ain[113]);
	

		

			//ft = atan2(fTempDataMSC[66],-fTempDataMSC[65]);
		//	ft = ft*180/3.14159265;
		//	if(ft<0) ft +=360;
			
		
		//	pBufData[index].Ain[115]=ft;

		

		ft = pBufData[index].Ain[114]*pBufData[index].Ain[114];
		ft += pBufData[index].Ain[113]*pBufData[index].Ain[113];
		ft = sqrt(ft);

		if (pBufData[index].Ain[109] != 0) {
			ft /= pBufData[index].Ain[109];
			ft = atan(ft);
			ft = ft*180/3.14159265;
			if(ft < 0) ft += 180;
			

			pBufData[index].Ain[6] = ft;	//Inc channel
		
		}

		ft = atan2(pBufData[index].Ain[113],-pBufData[index].Ain[114]);
		ft = ft*180/3.14159265;
		if(ft<0) ft +=360;
		
		pBufData[index].Ain[115] = ft ;//- coef_inc;	
	
		
		}

		
	
	}
	
	
	
	
}

long CheckFrameMSC40_1(BYTE* str, int index)
{
	UINT i,it,id=0,j,isize=96;
	long sum=0;
	CString st="", sz;
	if(msc60_flag) isize = 136;

	if (str[isize] != 0x10)  return 0;
	i=0;
		j=0;
		while(j< isize) 	{
			pTempDataMSC1[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
	for(i=0;i<9;i++) 			{
		BufData[i]=str[2*i]+str[2*i+1]*256;	
		sz.Format("%2x%2x ",str[2*i+1],str[2*i]);
		sum += str[2*i]+str[2*i+1]*256;	
		st += sz;
	}
	m_sSKP_msg = st;
	
	sum -= (0x0014 + 0xc0aa + 0x3f55 + 0xf731 + 0x8000 + 0x1010 + 0xffff + 0x08ce);
	return sum;
	
	
}

void CheckFrameAK(BYTE* str, int index)
{
	UINT i,it,id=0,j;
	long sum=0;
	CString st="", sz;

	
	i=0;
		j=0;
		while(j< 48) 	{
			pTempDataAK[i] = str[j] + str[j+1]*256;
			j = j+2;
			i++;
		}
	
		
		
		
		for(i=0;i<3;i++){

	
		
			
			
			pBufData[index].Ain[116+i]= pTempDataAK[i];


		}
		
		

		
	
	
	
	
}


void CheckFrameDAQ(BYTE header,BYTE tailer,BYTE* str, int index)
{
	UINT i,it,id=0,j;
	
	if((str[0] == header) && (str[52] == tailer))
	{
		for(i=0;i<53;i++) 
		{
			BufData[i]=str[i];	
			
		}
		
	} 
	else
	{
		for(i=0;i<53;i++)
		{
			if((str[i+1] == header) && (str[i] == tailer)) 
				break;
		}
		if(i==53) return;

		for(j=0;j<53;j++)
		{
			if(j<52-i)
				BufData[j] = str[i+j+1];
			else
				BufData[j] = str[j+i-52];
		}

	}
	
	for (i=0;i<24;i++)
	{
		tempBuf[i] = BufData[2*i+2] ;
		tempBuf[i] <<= 4;
		tempBuf[i] |= (BufData[i*2+1]>>4);
		pBufData[index].Ain[i]=tempBuf[i];
	}
	for(i=51;i>48;i--)
		{
			it = BufData[i];
			it <<= 8*(i-49);
			id |= it;
		}
	iDepth = id;	
	
}

double polyFunc(double *k, double x, int n)
{
	double v = 0;
	int		i;
	for(i = n-1; i >= 0; i--)
		v = v*x + k[i];
	return v;
}

int Inclinometer()
{
	double bg[3], sg[3], bh[3], sh[3],g[3],h[3],az[3];

	int		i;
	double *k;
	k = inc_102;
	double tempe = RealData.fData[6];
	g[0] = RealData.fData[0];
	g[1] = RealData.fData[1];
	g[2] = RealData.fData[2];

	h[0] = RealData.fData[3];
	h[1] = RealData.fData[4];
	h[2] = RealData.fData[5];

	for(i =0; i < 3; i++)	{
			bg[i] = polyFunc(k+i*4, tempe, 4);
			sg[i] = polyFunc(k+i*4+12, tempe, 4);
			bh[i] = polyFunc(k+i*4+24, tempe, 4);
			sh[i] = polyFunc(k+i*4+36, tempe, 4);
			if(sg[i] > 4)
				g[i] = (g[i] - bg[i]) / sg[i];
			if(sh[i] > 4)
				h[i] = (h[i] - bh[i]) / sh[i];
		}
	RealData.fData[0]=g[0];
	RealData.fData[1]=g[1];
	RealData.fData[2]=g[2];
	RealData.fData[3]=h[0];
	RealData.fData[4]=h[1];
	RealData.fData[5]=h[2];


	return 1;
}

float fold[NUMCHANNEL];

void GetAllDataDMT()
{
	
	gData.fDepth=iDepth;
	gData.fDepth /= fEncoderConst;
	
	static float fmarked = 0;
	if(gData.fData[1]>= 1){ 
		
		fmarked = gData.fDepth;
	}
	
	info.fTemp1= fmarked;

	float ft;
	int i,j;
	
	float favr,fg,fl,flmax=0,flmin=1023;
	float fl1,fl2;
	int ng,nl;
	
		for(i=0;i<NUMCHANNEL;i++)
		{
			if((ReadyFlag==1)&&(filter>2)){
				ft = 0;
				flmax=0;
				flmin=65535;

				for(j=0;j<filter;j++){
					ft += pBufData[j].Ain[i];
					if(pBufData[j].Ain[i] > flmax) flmax = pBufData[j].Ain[i];
					if(pBufData[j].Ain[i] < flmin) flmin = pBufData[j].Ain[i];

				}
				
				favr = ft- flmax - flmin;
				favr /= (filter-2);

				
				RealData.fData[i] = favr;
			}

			else RealData.fData[i]=pBufData[Index].Ain[i];
			/*if(ReadyFlag){
				ft = 0;
				fg = 0;
				fl = 0;
				ng = 0;
				nl = 0;
				flmax=0;
				flmin=1023;
				for(j=0;j<filter;j++){
					ft += pBufData[j].Ain[i];
					if(pBufData[j].Ain[i] > flmax) flmax = pBufData[j].Ain[i];
					if(pBufData[j].Ain[i] < flmin) flmin = pBufData[j].Ain[i];

				}
				favr = ft/filter;
				//favr = ft - flmax - flmin;
				//favr /= (filter-2);
				

				for(j=0;j<filter;j++){
					if (pBufData[j].Ain[i] >= favr){
						ng++;
						fg += pBufData[j].Ain[i];
					}
					else {
						nl++;
						fl += pBufData[j].Ain[i];
					}

					
				}

				

				if(ng >= nl) 
					RealData.fData[i]= fg/ng;
				else 
					RealData.fData[i]= fl/nl;
				//RealData.fData[i] = favr;
				RealData.fData[i]=pBufData[Index].Ain[i];

				//if (fabs(pBufData[Index].Ain[i]-favr)/favr > 0.25){
					//if ((i==2)||(i==3)||(i==4)||(i==19)) {
					//	RealData.fData[i] = favr;

					//}
				//}
				//fold[i] = RealData.fData[i];
	
			}
				
			
			else */
				
				//RealData.fData[i]=pBufData[Index].Ain[i];

		//	RealData.fData[i]=pBufData[Index].Ain[i];		new

			//if (pBufData[Index].Ain[i] < 50){
			//	if ((i==2)||(i==3)||(i==4)) {

			//		RealData.fData[i] = 
					//RealData.fData[i] = fold[i];

			//	}
		//	}
			//fold[i] = RealData.fData[i];
			//RealData.fData[i]=pBufData[Index].Ain[i];

		}	
		
			

}




void GetAllData()
{
	//MA1             (0)
	//Hydraulic Pres
	//--------- Temp
	//Sample Pres
	//------ Temp
	//Motor Voltage
	//Pretest POT
	//Signal REF
	//GR
	//Gauge REF-
	//Gauge REF+
	//MA12
	//MA13
	//MA14
	//MA15             (14)

	//Position
	//Quarzt Temp
	//------ Pres
	//if(DepthFlag == 1 ) 
	
	if(iDepth >= 0x800000) {
		iDepth = 0xffffff-iDepth;
		iDepth++;
		gData.fDepth=-1.0*iDepth/fEncoderConst;
	}
	else	gData.fDepth=iDepth/fEncoderConst;
	
	float fmarked = 0;
	if(gData.fData[1] >= 0.1){ 
		
		fmarked = gData.fDepth;
	}
	
	info.fTemp1= fmarked;
	float ft;
	int i,j;
	
	float favr,fg,fl,flmax=0,flmin=4095;
	float fl1,fl2;
	int ng,nl;
		for(i=0;i<NUMCHANNEL;i++)
		{
			if(ReadyFlag){
				ft = 0;
				fg = 0;
				fl = 0;
				ng = 0;
				nl = 0;
				flmax=0;
				flmin=4095;
				for(j=0;j<filter;j++){
					ft += pBufData[j].Ain[i];
					//if(pBufData[j].Ain[i] > flmax) flmax = pBufData[j].Ain[i];
					//if(pBufData[j].Ain[i] < flmin) flmin = pBufData[j].Ain[i];

				}
				favr = ft/filter;
			//	favr = ft - flmax - flmin;
				//favr /= (filter);

				/*for(j=0;j<filter;j++){
					if (pBufData[j].Ain[i] >= favr){
						ng++;
						fg += pBufData[j].Ain[i];
					}
					else {
						nl++;
						fl += pBufData[j].Ain[i];
					}

					
				}

				

				if(ng >= nl) 
					RealData.fData[i]= fg/ng;
				else 
					RealData.fData[i]= fl/nl;*/

			//RealData.fData[i] = (RealData.fData[i]-2048)*5.0/2048.0;
				RealData.fData[i] = favr;
			}
			else RealData.fData[i]=pBufData[Index].Ain[i];
			RealData.fData[i] = (RealData.fData[i]-2048)*5.0/2048.0;
		}
		// correct temperature
		Inclinometer();
		float coef_inc = 0,coef_45=0;
		//Calculate Inclinometre
		fl = RealData.fData[0]*RealData.fData[0];
		fl += RealData.fData[1]*RealData.fData[1];
		fl += RealData.fData[2]*RealData.fData[2];
		fl = sqrt(fl);
		RealData.fData[0] /= fl;
		RealData.fData[1] /= fl;
		RealData.fData[2] /= fl;
		fl = RealData.fData[3]*RealData.fData[3];
		fl += RealData.fData[4]*RealData.fData[4];
		fl += RealData.fData[5]*RealData.fData[5];
		fl = sqrt(fl);
		RealData.fData[3] /= -1*fl;
		RealData.fData[4] /= -1*fl;
		RealData.fData[5] /= -1*fl;

		fg = RealData.fData[0]*RealData.fData[0];
		fg += RealData.fData[1]*RealData.fData[1];
		fg = sqrt(fg);
		// inc = atan(sqrt(Ax^2 + Ay^2) / Az)
		if (RealData.fData[2] != 0) {
			fg /= RealData.fData[2];
			fg = atan(fg);
			fg = fg*180/3.14159265;
			if(fg < 0) fg += 180;
			

			RealData.fData[9] = fg;	//Inc channel
			//correct inc for <= 45
			/*if(fg <46) {
				coef_45 = 2.075*tan((45-fg)*3.14159265/180)-0.1;
				//RealData.fData[9]
				fg += coef_45;
			}*/
			
		}
		else { 
		RealData.fData[9] = 90;
		}
		coef_inc = 0.0*tan((90-fg)*3.14159265/180);
		//Calculate Azimuth
		float h1, h2;
		h1 = RealData.fData[0]*RealData.fData[4] - RealData.fData[1]*RealData.fData[3];
		h2 = RealData.fData[5]-RealData.fData[2]*(RealData.fData[0]*RealData.fData[3]+RealData.fData[1]*RealData.fData[4]+
			 RealData.fData[2]*RealData.fData[5]);
		fg = h1/h2;
		fg = atan(fg);
		fg = fg*180/3.14159265;
		
		if((h1>0)&&(h2<0)) fg = 180 + fg;
		else if((h1<=0)&&(h2<0)) fg = 180 + fg;
		else if((h1<0)&&(h2>0)) fg = 360 + fg;

		RealData.fData[10] = 360- fg;// - coef_inc;
		if(RealData.fData[10]<0) RealData.fData[10] += 360;

		//Bering 

		h1 = RealData.fData[1];
		h2 = RealData.fData[0];
		fg = h1/h2;
		fg = atan(fg);
		fg = fg*180/3.14159265;
		
		if((h1>0)&&(h2<0)) fg = 180 + fg;
		else if((h1<=0)&&(h2<0)) fg = 180 + fg;
		else if((h1<0)&&(h2>0)) fg = 360 + fg;

		RealData.fData[11] = 360-fg ;//- coef_inc;
		if(RealData.fData[11]>360) RealData.fData[11] -=360;

		//Calculate AZ-I
		h1 = RealData.fData[5]*RealData.fData[1] - RealData.fData[4]*RealData.fData[2];
		h2 = RealData.fData[3] - RealData.fData[0]*(RealData.fData[0]*RealData.fData[3]+RealData.fData[1]*RealData.fData[4]+
			 RealData.fData[2]*RealData.fData[5]);
		fg = h1/h2;
		fg = atan(fg);
		fg = fg*180/3.14159265;

		if((h1>0)&&(h2<0)) fg = 180 + fg;
		else if((h1<=0)&&(h2<0)) fg = 180 + fg;
		else if((h1<0)&&(h2>0)) fg = 360 + fg;

		//RealData.fData[12] = 360-fg;
		RealData.fData[12] = 180-fg;
		RealData.fData[13] = RealData.fData[12]+RealData.fData[11];
		if(RealData.fData[13] >360) RealData.fData[13] -= 360;
		if(RealData.fData[13] <0) RealData.fData[13] += 360;
}

void mwait(long it)
{	static DWORD	dPrev;
	
	dPrev=GetTickCount();
	while((GetTickCount()-dPrev) < it);
}

float fspeed[10]={0,0,0,0,0,0,0,0,0,0},ftempsp=0;
int nsp=0;
void DisplayDataInPanel(CWnd* pWnd)
{
	//
	SYSTEMTIME		SystemTime;

	static	float	fPrevDepth=0;
	static	DWORD	dPrevTime;
	static	DWORD	dCurTime;
	float ft,fbuf[NUMCHANNEL],fave=0,fmin=30000,fmax=-30000;
	UINT irot=0,j;

	dCurTime=GetTickCount();

	GetLocalTime(&SystemTime);
	sprintf(info.szCurTime,"%02d:%02d:%02d",SystemTime.wHour,
			SystemTime.wMinute,SystemTime.wSecond);
		
	sprintf(info.szCurDepth,"Depth(m): %.2f",gData.fDepth);
	info.fDepth=gData.fDepth;
	
	//info.fTemp1=gData.fData[2];//Hydraulic
	/*info.fPres1=gData.fData[1];
	info.fTemp2=gData.fData[4];//Sample
	info.fPres2=gData.fData[3];
	info.fTemp3=gData.fData[19];//Quarzt
	info.fPres3=gData.fData[18];
	info.fPistonPos=gData.fData[6];
	info.fMotorVolt = gData.fData[MOTOR_VOLTAGE];
	switch ((int)gData.fData[POSITION]) {
	case 0:
		info.bValve1Open=true;
		info.bValve2Open=false;
		break;
	case 1:
		info.bValve1Open=false;
		info.bValve2Open=false;
		break;
	case 2:
		info.bValve1Open=false;
		info.bValve2Open=true;
		break;
	case 3:
		info.bValve1Open=false;
		info.bValve2Open=false;
		break;
	default:
		info.bValve1Open=false;
		info.bValve2Open=false;
		break;
	}*/

	if(dCurTime-dPrevTime >= 1000)
	{
		
		
		fspeed[nsp] = fabs(fPrevDepth-gData.fDepth);
		ftempsp = 0;

		for(j=0;j<10;j++){
			ftempsp +=fspeed[j];

		}

		nsp++;

		if(nsp > 9) nsp = 0;
		
		info.fSpeed= ftempsp *1000;
		info.fSpeed /= (dCurTime-dPrevTime);
		info.fSpeed *=6;

		//info.fSpeed=fabs(fPrevDepth-gData.fDepth)*1000.0;//60000.0;

		//info.fSpeed /= (dCurTime-dPrevTime);

		//info.fSpeed *=60;
		 
		fPrevDepth=gData.fDepth;
		dPrevTime=dCurTime;
	}
	info.fSpeed = RealData.fData[47];
	//info.fSpeed *=6;

	//if(DepthFlag == 1 ) 
	{
		for(int i=0;i<NUMCHANNEL;i++)
		{
			
			//gData.fData[20+i] = CalibChannel(RealData.fData[i],
			//								fCalib[i].RawVal[0],fCalib[i].RealVal[0],
			//								fCalib[i].RawVal[1],fCalib[i].RealVal[1], 
			// 								fCalib[i].RawVal[2], fCalib[i].RealVal[2]);
			gData.fData[i] = MergeChannel(RealData.fData[i],i);	
			
			
		}

		
	if(mbMSC)	{
	//	ft = gData.fData[95];

	//	irot = (int)ft;

	//	irot /= 9;

		

		if(msc60_flag){
			fave=0;
			for(i=48;i<108;i++){
		
			
				fave += gData.fData[i];
				if(fmax < gData.fData[i]) fmax = gData.fData[i];
				if(fmin > gData.fData[i]) fmin = gData.fData[i];
			
	

			}
			gData.fData[116]=fave/60;
			gData.fData[118]=fmax;
		
			gData.fData[119]=fmin;
		}
		else {
		fave=0;
		for(i=48;i<88;i++){
		//	if(i+irot < 88)	{
		//		fbuf[i-48] = gData.fData[irot+i];
		//	}
		//	else {
		//		fbuf[i-48]= gData.fData[irot+i-40];

		//	}

			
			fave += gData.fData[i];
			if(fmax < gData.fData[i]) fmax = gData.fData[i];
			if(fmin > gData.fData[i]) fmin = gData.fData[i];
			

		

		}
			gData.fData[116]=fave/40;
			gData.fData[118]=fmax;
		
			gData.fData[119]=fmin;
		}


		 

		
	}

	}
	
	

	/*float fmarked = 0;
	if(iMarked >= 0x800000) {
		iMarked = 0xffffff-iMarked;
		iMarked++;
		fmarked =-1.0*iMarked;
		fmarked /= fEncoderConst;
	}
	else	{
		fmarked=iMarked;
		fmarked /= fEncoderConst;
	}
	info.fTemp1= fmarked;*/

	if(pWnd!=NULL && !bStopFirmware){

		//((CPanel*)pWnd)->m_sMarked.Format("%.2f",fmarked);	
		

		

		((CPanel*)pWnd)->DisplayDataInPanel(info);	
		
		
	}
	
	//(CPosition*)pWnd->m_sDL60Test = m_sDL60_msg;
}

int TimeStringToInt(VARIANT last_varV, int* time)
{
	int hour,minute,second,percs,pos1,pos2;
	char *sz,*zz,*zs,*aa;
	_bstr_t ss;
	
	ss = last_varV.bstrVal;
	zz = ss;
	sz = strrchr(zz,'.');
	zs = sz + 1;
	percs = atoi(zs);
	time[3]=percs;
	sz = strrchr(zz,':');
	sz++;
	zz = ss;
	sz = strtok(zz,":");
	pos1 = strcspn(zz,":");
	hour = atoi(sz);
	time[0] = hour;
	ss = last_varV.bstrVal;
	zz = ss;
	zz += pos1 +1;
	sz = strtok(zz,":");
	pos2 = strcspn(zz,":");
	minute = atoi(sz);
	time[1] = minute;
	ss = last_varV.bstrVal;
	zz = ss;
	zz += pos1 + pos2 +2;
	sz = strtok(zz,".");
	second = atoi(sz);
	time[2] = second;
	return (percs + second*1000 + minute*1000*60 + hour*1000*3600);
}