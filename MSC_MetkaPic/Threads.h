#if !defined _THREADS_H
#define _THREADS_H

#include "StdAfx.h"
#include "Comm.h"



#define		TIMESLICE		300
#define		MAX_CURVE		120

#define		DIRECTION_UP	0
#define		DIRECTION_DOWN	1

#define		SCALE_30SEC		30
#define		SCALE_1MIN		60
#define		SCALE_5MIN		300
#define		SCALE_10MIN		600
#define     SCALE_20MIN		1200
#define		SCALE_30MIN		1800

#define		SCALE_5M		5
#define		SCALE_10M		10
#define		SCALE_20M		20
#define		SCALE_30M		30
#define		SCALE_50M		50
#define		SCALE_100M		100

#define			MAXTRACK		10
#define			LINEAR			0
#define			LOGARIT			1

//#define NUMCHANNEL			48
#define NUMCHANNEL			120


class CTrackList:public CObject
{
public:
	char		szTitle1[20];
	char		szTitle2[20];
	char		szMnemonic[15];
	float		fLeftEdge;//use inch
	float		fRightEdge;
	BOOL		bDepthTrack;
	BOOL		bGridFront;

	BOOL		bHorzOn;
	float		fHFreq;

	BOOL		bVertOn;
	int			nVType;
	int			nDivisionNum;
	int			nDecadesNum;
	int			nSCycle;
public:
	CTrackList(){}
	~CTrackList(){}

	CTrackList(const CTrackList* trackList)
	{
	strcpy(this->szTitle1,trackList->szTitle1);
	strcpy(this->szTitle2,trackList->szTitle2);
	strcpy(this->szMnemonic,trackList->szMnemonic);
	this->fLeftEdge=trackList->fLeftEdge;
	this->fRightEdge=trackList->fRightEdge;
	this->bDepthTrack=trackList->bDepthTrack;
	this->bHorzOn=trackList->bHorzOn;
	this->bVertOn=trackList->bVertOn;
	this->nVType=trackList->nVType;
	this->nDivisionNum=trackList->nDivisionNum;
	this->nDecadesNum=trackList->nDecadesNum;
	this->nSCycle=trackList->nSCycle;

	}
};

/*class CCurveList:public CObject
{
public:
	char		szMnemonic[15];
	char		szName[20];
	char		szUnit[15];
	char		szTrack[15];//track name
	BOOL		bActive;//visible or not
	int			nScale;//logarit or linear
	int			nClass;//don't use
	int			nIdx;//curve index
	int			nTrackIdx;//track index
	BOOL		bPlotTitle;//don't use
	BOOL		bPlotSample;//don't use
	BOOL		bPlotPerfor;//don't use

	float		fLeftScale;
	float		fRightScale;
	int 		nLeftRebase;//don't use
	int 		nRightRebase;//don't use

	int			nStyle;//don't use
	int			nColor;
	int			nWidth;//don't use

	float		fGap;//don't use
public:
	CCurveList(){}
	~CCurveList(){}
	CCurveList(const CCurveList* curveList)
	{		
		strcpy(this->szMnemonic,curveList->szMnemonic);
		this->bActive=curveList->bActive;
		this->nScale=curveList->nScale;
		this->nTrackIdx=curveList->nTrackIdx;
		this->nIdx=curveList->nIdx;
		this->fLeftScale=curveList->fLeftScale;
		this->fRightScale=curveList->fRightScale;
		this->nColor=curveList->nColor;
	}
};*/


class CCurveList:public CObject
{
public:
	char		szMnemonic[15];
	char		szName[20];
	char		szUnit[15];
	char		szTrack[15];//track name
	BOOL		bActive;//visible or not
	int			nScale;//logarit or linear
	int			nClass;//don't use
	int			nIdx;//curve index
	int			nTrackIdx;//track index
	BOOL		bPlotTitle;//don't use
	BOOL		bPlotSample;//don't use
	BOOL		bPlotPerfor;//don't use

	float		fLeftScale;
	float		fRightScale;
	int 		nLeftRebase;//don't use
	int 		nRightRebase;//don't use

	int			nStyle;//don't use
	int			nColor;
	int			nWidth;//don't use

	float		fGap;//don't use
public:
	int nShape;
	CPen Pen;
	BOOL bIsOldWrap;
	BOOL bIsWrap;
	CCurveList(){}
	~CCurveList(){}
	CCurveList(const CCurveList* curveList)
	{		
		strcpy(this->szMnemonic,curveList->szMnemonic);
		this->bActive=curveList->bActive;
		this->nScale=curveList->nScale;
		this->nTrackIdx=curveList->nTrackIdx;
		this->nIdx=curveList->nIdx;
		this->fLeftScale=curveList->fLeftScale;
		this->fRightScale=curveList->fRightScale;
		this->nColor=curveList->nColor;
		this->nWidth=curveList->nWidth;
		this->nStyle=curveList->nStyle;
	}
};

typedef CTypedPtrArray<CObArray,CTrackList*> CTrackListArray;
typedef CTypedPtrArray<CObArray,CCurveList*> CCurveListArray;

const int WM_READDATATHREAD_UPDATE=WM_USER+101;

struct ThreadInfo_t
{
	CWnd*				pWnd;
	CRect*				pRect;
	CRect*				pRectPart;
	int*				pCurY;
	CTrackListArray*	pTrackList;	
	CCurveListArray*	pCurveList;
};
struct	ReadDataThreadInfo_t
{
	CWnd*		pWnd;
};
struct	WriteDataThreadInfo_t
{
	CWnd*		pWnd;
};
struct  UpdateInfo_t
{
	char		szCurDepth[80];
	char		szCurTime[80];
	float		fDepth;
	float		fTemp1,fPres1;
	float		fTemp2,fPres2;
	float		fTemp3,fPres3;
	float		fPistonPos;
	bool		bValve1Open;
	bool		bValve2Open;
	float		fMotorVolt;
	float		fSpeed;
};
struct	RawData_t
{
	float		fDepth;
	float		fData[MAX_CURVE];
};

struct MFTData_t
{ 
	int MA[15];
	int MD[3];
	float Ain[NUMCHANNEL];
	double dTempeFreq;
	double dPressFreq;	
	
};
struct Calib_t
{ 
	float RawVal[3];
	float RealVal[3];
};
struct	LastInfoCalib_t
{
	char		szNameChannel[20];
	Calib_t		pCh;		
};


extern BOOL		bIsLogging;
extern BOOL		bIsSaving;
extern BOOL		bStopFirmware;
//extern int		nDirection;
//extern int		nLeftViewScale;
extern int		nRightUpperViewScale;
extern int		nRightLowerViewScale;
extern RawData_t		gData;
extern RawData_t		RealData;
extern LastInfoCalib_t  pInfoCalib[NUMCHANNEL];
extern LastInfoCalib_t  lInfoCalib[NUMCHANNEL];

extern int		nGaugeHydraulicMin1;
extern int		nGaugeHydraulicMax1;
extern int		nGaugeHydraulicMin2;
extern int		nGaugeHydraulicMax2;
extern BOOL	bGaugeHydraulicLogarit;

extern int		nGaugeSampleMin1;
extern int		nGaugeSampleMax1;
extern int		nGaugeSampleMin2;
extern int		nGaugeSampleMax2;
extern BOOL	bGaugeSampleLogarit;

extern int		nQuartzPretestMin1;
extern int		nQuartzPretestMax1;
extern int		nQuartzPretestMin2;
extern int		nQuartzPretestMax2;
extern BOOL	bQuartzPretestLogarit;

extern int		nPositionMin;
extern int		nPositionMax;

extern int cmd_flag;
extern int load_flag,msc60_flag,copy_flag,lcur,lreco;
extern BYTE tool[8];
extern char szTime[80];

extern double	fEncoderConst;
extern int		nEncoder,nNumChannel;
extern int		nATension;
extern int		nBTension;

extern int mbALM, mbALMTest,mbDL60,mbDL60Test, mbBK3,mbBK3Test, mbGK76, mbGK76Test, mbGK60, mbGK60Test,mbSKP,
		   mbSKPTest,mbOpenAll,mbCloseAll,mbOpen,mbClose,runingArm,mbMSC,
		   mbMSCTest,mbMSCOpenAll,mbMSCCloseAll,mbMSCOpen,mbMSCClose; 
extern int mbCopyMem,mbRTCMem,mbViewMem,mbCreateMem,mbFormatMem,mbOnTimerMem,mbOffTimerMem,mbOnPowerMem,mbOffPowerMem;

extern int mbBK79,mbBK79Test,mbNNK,mbNNKTest,mbGGK,mbGGKTest,mbAK1_1,mbAK1_2,mbAK2_1,mbAK2_2,mbAK_T,mbT_P,
			mbT_PTest,mbSGK,mbSGKTest;

extern int		PauseFlag;
extern bool		bSendOK;
extern int		nSetDepth;
extern int		iMarked ;
extern int		iSetMarked;
extern int		IgniteFlag;
extern int		CloseFlag;
extern int		dmt_flag;
extern	UpdateInfo_t	info;
extern CString m_sDL60_msg,m_sALM_msg, m_sBK3_msg,m_sGK76_msg, m_sGK60_msg, m_sMSCCalib;

//extern CDaoDatabase		m_pDB;
//extern CDaoDatabase		m_pDB1;
//extern CDaoRecordset	m_rs;
//extern CDaoRecordset	m_rRs1;
//extern CDaoRecordset	m_Lrs;
extern _RecordsetPtr m_rs;
extern ThreadInfo_t*	m_pthreadInfo2;
extern CFile			*pFile;
extern CFile			*pcFile;
extern int				nStartNum;
extern CCurveList*		m_pCurve;			
extern CCurveListArray*		m_pCurveList;
extern ThreadInfo_t*		m_pThreadInfo;
extern char CurName[150];

UINT	View1Thread(LPVOID pParam);
UINT	View2Thread(LPVOID pParam);
UINT	ReadDataThread(LPVOID pParam);
UINT	WriteDataThread(LPVOID pParam);
void DrawGrid(CDC *pDC,int nDivNum,CTrackListArray *trackList,CRect *rViewport);
float DataToScr(float dat, CCurveList *curve, CTrackList *track, int nScrWidth);
float WrapConvert(float dat, CCurveList *curve, CTrackList *track,int nScrWidth);
//int TimeStringToInt(VARIANT last_varV);
int TimeStringToInt(VARIANT last_varV, int* time);
int IsFileExist(char szFileName[]);

//-----------------------------------------
//
//			COMM SECTION
//
//-----------------------------------------

extern SerialComm m_cComm;
extern int Index,ReadyFlag,filter ;
extern int DepthFlag;
extern int Readflag;
extern int nOldStartNum;
extern MFTData_t *pBufData;

extern float pTempData[NUMCHANNEL];

extern int		pTempDataALM[NUMCHANNEL];
extern int		pTempDataALM1[NUMCHANNEL];
extern int		pTempDataBK3[NUMCHANNEL];
extern int		pTempDataBK31[260];
extern int		pTempDataDL60[NUMCHANNEL];
extern int		pTempDataDL601[NUMCHANNEL];
extern int		pTempDataGK76[NUMCHANNEL];
extern int		pTempDataGK761[NUMCHANNEL];
extern int		pTempDataGK60[NUMCHANNEL];
extern int		pTempDataGK601[NUMCHANNEL];
extern int		pTempDataSKP[NUMCHANNEL];
extern int		pTempDataSKP1[NUMCHANNEL];
extern int		pTempDataDMT[NUMCHANNEL];


extern int		pTempDataBK79[NUMCHANNEL];
extern int		pTempDataSGK90[NUMCHANNEL];

extern int		iSGK[260];
extern int		iBK[260];
extern int 		buft[12];

extern int		pTempDataMSC[NUMCHANNEL];
extern int		pTempDataMSC1[NUMCHANNEL];
extern float	fTempDataMSC[NUMCHANNEL];
extern int		pTempDataAK[NUMCHANNEL];

extern float	fTempDataSKP[NUMCHANNEL];


extern RawData_t		gData1;
extern RawData_t		gData2;
extern RawData_t		gData_D;
BYTE iByte(BYTE b);
UINT i12Bits(int b1,int b2);
UINT i24Bits(int b1,int b2,int b3);
double Press_Cal(int fp,int ft);
double Press_Cal1(int fp,int ft);
double Temperature_Cal(int ft);
double Temperature_Cal1(int ft);
double Motor_Volt(double count);
double CalibADC(double a, double refm, double refg, double refp);
void GetMAChannels(BYTE *a);
UINT CheckFrameDepth(BYTE header,BYTE tailer,BYTE* str);
void CheckFrameMft(BYTE header,BYTE tailer,BYTE* str, int index);
void GetDepth();
void GetMftData();
void SetDepth(UINT idepth);
void mwait(long it);
UINT	ReadCommDataThread(LPVOID pParam);
void GetAllData();
void DisplayDataInPanel(CWnd* pWnd);
int IsFileExist(char szFileName[]);
#endif