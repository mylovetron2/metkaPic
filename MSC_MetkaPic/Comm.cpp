
#include "stdafx.h"
#include "io.h"
#include "dos.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "comm.h"

SerialComm::SerialComm()
{
	m_hComm = (HCOMM)-1;
	char	str[120];
	IdPort = 1;
	Connected =Status = 0;
	m_hWnd = 0; m_InSize = m_OutSize = 80;
	m_ComMask = EV_ERR|EV_BREAK;
#ifdef	WIN32
	memset(&dcb, 0, sizeof(dcb));
	dcb.DCBlength = sizeof(dcb);
	strcpy(str, "COM1: baud=9600 parity=N data=8 stop=1");
	BuildCommDCB(str,&dcb);
#else
	BuildCommDCB("COM1:9600,n,8,1", &dcb);
#endif
}

SerialComm::~SerialComm()
{
	Close();
}

BOOL  SerialComm::ReOpen()
{
	char sz[40];
	COMMTIMEOUTS	timeout;
	Close();
	wsprintf(sz,"COM%d",IdPort);
#ifdef	WIN32
	m_hComm = CreateFile(sz,
                     GENERIC_READ | GENERIC_WRITE,
                     0,    // comm devices must be opened w/exclusive-access
                     NULL, // no security attributes
                     OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                     0,    // not overlapped I/O
                     NULL  // hTemplate must be NULL for comm devices
                     );
#else
	m_hComm = OpenComm(sz,m_InSize,m_OutSize);
#endif
	if(m_hComm == (HCOMM)-1)	{
		return FALSE;
	}
#ifdef	WIN32
	Status = SetCommState(m_hComm, &dcb);
	SetCommMask(m_hComm,EV_ERR|EV_BREAK|EV_RXCHAR);
	timeout.ReadIntervalTimeout = 12;
	timeout.ReadTotalTimeoutConstant = 10;
	timeout.ReadTotalTimeoutMultiplier = 10;
	timeout.WriteTotalTimeoutConstant = 10;
	timeout.WriteTotalTimeoutMultiplier = 10;
	SetCommTimeouts(m_hComm, &timeout);
#else
	Status = SetCommState(&dcb);
	SetCommEventMask(m_hComm,EV_ERR|EV_BREAK|EV_RXCHAR);
	EnableCommNotification(m_hComm,m_hWnd,-1,-1);
#endif
	return !Status;
}

BOOL  SerialComm::Close()
{
	Status = 0;
	if(m_hComm != (HCOMM)-1)	{
#ifdef	WIN32
		CloseHandle(m_hComm);
#else
		EnableCommNotification(m_hComm,NULL,-1,-1);
		Status = CloseComm(m_hComm);
#endif
		m_hComm = (HCOMM)-1;
		Connected=0;
	}
	return !Status;
}

BOOL  SerialComm::Open(int idport, LPSTR OpenStr)
{
	int	err;
	char	str[120];
	strcpy(str, OpenStr);
	err = BuildCommDCB(str, &dcb);
	if (err < 0)	return FALSE;
	IdPort = idport;
	return ReOpen();
}

BOOL  SerialComm::Open(int iPort, long BaudRate, 
					  int ByteSize,int Parity, int StopBits,bool bOverlapped)
{
	char sz[40];
	COMMTIMEOUTS	timeout;
	Close();
	wsprintf(sz,"COM%d",iPort);
#ifdef	WIN32
	if(bOverlapped==false)
		m_hComm = CreateFile(sz,
                     GENERIC_READ | GENERIC_WRITE,
                     0,    // comm devices must be opened w/exclusive-access
                     NULL, // no security attributes
                     OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                     0,    // not overlapped I/O
                     NULL  // hTemplate must be NULL for comm devices
                     );
	else
		m_hComm = CreateFile(sz,
                     GENERIC_READ | GENERIC_WRITE,
                     0,    // comm devices must be opened w/exclusive-access
                     NULL, // no security attributes
                     OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                     FILE_FLAG_OVERLAPPED,    // not overlapped I/O
                     NULL  // hTemplate must be NULL for comm devices
                     );
#else
	m_hComm = OpenComm(sz,m_InSize,m_OutSize);
#endif
	if(m_hComm == (HCOMM)-1)	{
		return FALSE;
	}
	IdPort = iPort;
	GetCommState(m_hComm, &dcb);
	if((BaudRate & 0xff00) == 0xff00)
		dcb.BaudRate = BaudRate;
	else if(BaudRate <= 19200)
		dcb.BaudRate = BaudRate;
	else	{
		switch(BaudRate)	{
			case 38400l:dcb.BaudRate = CBR_38400; break;
			case 56000l:dcb.BaudRate = CBR_56000; break;
			case 128000l:dcb.BaudRate = CBR_128000; break;
			case 256000l:dcb.BaudRate = CBR_256000; break;
			
			default:return FALSE;
		}
	}
	dcb.ByteSize = ByteSize;
	dcb.Parity = Parity;
	if(StopBits == 1)	StopBits = 0;
	dcb.StopBits = StopBits;
#ifdef	WIN32
	Status = SetCommState(m_hComm, &dcb);
	SetCommMask(m_hComm,EV_ERR|EV_BREAK|EV_RXCHAR);
	timeout.ReadIntervalTimeout = 12;
	timeout.ReadTotalTimeoutConstant = 10;
	timeout.ReadTotalTimeoutMultiplier = 10;
	timeout.WriteTotalTimeoutConstant = 10;
	timeout.WriteTotalTimeoutMultiplier = 10;
	SetCommTimeouts(m_hComm, &timeout);
	
#else
	Status = SetCommState(&dcb);
	SetCommEventMask(m_hComm,EV_ERR|EV_BREAK|EV_RXCHAR);
	EnableCommNotification(m_hComm,m_hWnd,-1,-1);
#endif
	return !Status;
}

BOOL SerialComm::Init(LPSTR OpenStr)
{
	char	str[120];
	strcpy(str, OpenStr);
	Status = BuildCommDCB(str, &dcb);
	if(m_hComm != (HCOMM)-1)
#ifdef	WIN32
		Status = SetCommState(m_hComm,&dcb);
#else
		Status = !SetCommState(&dcb);
#endif
	return Status;
}


void	SerialComm::Init(HWND hWnd, UINT InSize, UINT OutSize)
{
	m_hWnd = hWnd;
	if(InSize)	m_InSize = InSize;
	if(OutSize) m_OutSize = OutSize;
	ReOpen();
	return;
}


int SerialComm::TransmitCommChar(char c)
{
	return ::TransmitCommChar(m_hComm, c);
}

int SerialComm::WriteComm(void FAR* pbyte, int len,OVERLAPPED* pOS)
{
	DWORD	cb =0;
#ifdef	WIN32
	WriteFile(m_hComm, pbyte, len, &cb,pOS);
	return cb;
#else
	return ::WriteComm(m_hComm, pbyte, len);
#endif
}

int SerialComm::ReadComm(void FAR* pbyte, int len,OVERLAPPED* pOS)
{
	DWORD	cb =0;
#ifdef	WIN32
	ReadFile(m_hComm, pbyte, len, &cb,pOS);
	return cb;
#else
	return ::ReadComm(m_hComm, pbyte, len);
#endif
}


HCOMM SerialComm::GetHandle()
{
	return m_hComm;
}


