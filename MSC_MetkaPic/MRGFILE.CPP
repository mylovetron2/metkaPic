// MrgFile.cpp: implementation of the CMrgFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Las2Mrg.h"
#include "MrgFile.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMrgFile::CMrgFile()
{
	hFile=NULL;
}

CMrgFile::~CMrgFile()
{
	if(hFile!=NULL) fclose(hFile);
}
void CMrgFile::OpenMrgFile(char FN[])
{
	 CloseMrgFile();

	 strcpy(szFileName,FN);
	 hFile=fopen(szFileName,"rb+");
	 ReadStack();
}
void CMrgFile::CloseMrgFile()
{
	 if(hFile!=NULL) fclose(hFile);

}
void CMrgFile::ReadStack()
{
	 char *Stack,Channel[5],Unit[5];
	 char Header[2048];
	 int  i,j,k;
	 int  Index=0;
	 long l;

	 Stack=new char[256];
	 MrgData_t Data1,Data2;
	 fseek(hFile,2048,SEEK_SET);
	 fread(Stack,256,1,hFile);
	 i=0;
	 while(Stack[i]!='*')
	 {
		 k=i;
		 for(j=0;j<4;j++)
		 {
			 if(Stack[k]==0x20)
				 break;
			 Channel[j]=Stack[k];k++;
		 }
		 Channel[j]=0;
		 k=i+4;
		 for(j=0;j<4;j++)
		 {
			 if(Stack[k]==0x20)
				 break;
			 Unit[j]=Stack[k];k++;
		 }
		 Unit[j]=0;
		 i+=8;
		 strcpy(szCurName[Index],Channel);
		 strcpy(szCurUnit[Index],Unit);
		 Index++;
	 }


	 nCurNum=Index-1;
	 nLenOfData=sizeof(long)+sizeof(float)+sizeof(float)*nCurNum;
	 fread(&Data1,nLenOfData,1,hFile);
	 lBeginDepth=Data1.Deep;
	 fread(&Data2,nLenOfData,1,hFile);
	 l=Data2.Deep-Data1.Deep;fSampleDis=(float)l;
	 fSampleDis=fabs(fSampleDis);
	 if(Data2.Deep>Data1.Deep)   nDirection=DOWN;
	 else                        nDirection=UP;
	 fseek(hFile,0,SEEK_END);
	 l=ftell(hFile);
	 lRecNum=l-2048l-256l;lRecNum /=nLenOfData;
	 MoveTo(lRecNum-1);
	 Data2=ReadData();
	 lEndDepth=Data2.Deep;

	 MrgData_t Data;
	 MoveFirst();
	 Data=ReadData();
	 MoveFirst();
	 delete [] Stack;
}
void CMrgFile::MoveTo(long index)
{
	 long offset;
	 offset=nLenOfData;offset*=index;
	 if(index<0 || index >=lRecNum) return;
	 fseek(hFile,2048+256,SEEK_SET);
	 fseek(hFile,offset,SEEK_CUR);
	 lCurRec=index;
}
void CMrgFile::MoveFirst()
{
	 MoveTo(0);
}
void CMrgFile::MoveEnd()
{
	MoveTo(lRecNum-1);
}
void CMrgFile::MoveNext()
{
  MoveTo(lCurRec+1);
}
void CMrgFile::MovePrev()
{
	MoveTo(lCurRec-1);
}
MrgData_t CMrgFile::ReadData()
{
	MrgData_t Data;
	fread(&Data,nLenOfData,1,hFile);
	fseek(hFile,-nLenOfData,SEEK_CUR);
	return Data;
}
void CMrgFile::WriteData(MrgData_t Data)
{
	fwrite(&Data,nLenOfData,1,hFile);
	fseek(hFile,-nLenOfData,SEEK_SET);
}
void CMrgFile::GoToDepth(long Depth)
{
	MrgData_t Data;
	long      l;
	float     temp;
	if(nDirection==UP)
	{
		if(Depth>lBeginDepth||Depth<lEndDepth)
			return;
		l=lBeginDepth-Depth;
		temp=(float)l/fSampleDis;
		l=(long)temp;
		MoveTo(l);
	}
	else
	{
		if(Depth<lBeginDepth||Depth>lEndDepth)
			return;
		l=Depth-lBeginDepth;
		temp=(float)l/fSampleDis;
		l=(long)temp;
		MoveTo(l);
	}
}
