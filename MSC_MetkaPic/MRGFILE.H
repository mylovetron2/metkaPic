// MrgFile.h: interface for the CMrgFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MRGFILE_H__F948F062_2F25_11D5_B26B_C9CDA806EC82__INCLUDED_)
#define AFX_MRGFILE_H__F948F062_2F25_11D5_B26B_C9CDA806EC82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define UP		1
#define DOWN	0

#pragma pack(1)



struct MrgData_t
{
	long	Deep;
	float	Pulse20M;
	float	Data[120];
};
class CMrgFile  
{
public:
	FILE	*hFile;
	char	szFileName[80];
	char	szCurName[32][5];
	char	szCurUnit[32][5];
	float	fMaxVal[32],fMinVal[32];
	int		nCurNum;
	long	lBeginDepth,lEndDepth;
	long	lRecNum,lCurRec;
	int		nDirection;
	int		nLenOfData;
	float	fSampleDis;
public:
	CMrgFile();
	virtual ~CMrgFile();
	void	OpenMrgFile(char FN[]);
	void	CloseMrgFile();
	void	ReadStack();
	void	MoveTo(long index);
	void	MoveFirst();
	void	MoveEnd();
	void	MoveNext();
	void	MovePrev();
	MrgData_t ReadData();
	void	WriteData(MrgData_t Data);
	void	GoToDepth(long Depth);

};

#endif // !defined(AFX_MRGFILE_H__F948F062_2F25_11D5_B26B_C9CDA806EC82__INCLUDED_)
