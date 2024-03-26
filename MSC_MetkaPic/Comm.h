#ifndef __COMSETUP_H__
#define __COMSETUP_H__
#include <windows.h>

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */


#define COM_RXQUEUE    4096
#define COM_TXQUEUE    4096

#define COM_FCDTRDSR      1
#define COM_FCRTSCTS      2
#define COM_FCXONXOFF     4

#define ASCII_BEL      0x07//ascii definitions
#define ASCII_BS       0x08
#define ASCII_LF       0x0A
#define ASCII_CR       0x0D
#define ASCII_XON      0x11
#define ASCII_XOFF     0x13

#define	BeginSend	0xfe
#define	EndSend		0xfd
#define	ReceiveError	0xfc
#define	ReceiveOk		0xfb
#define	Question		0xfa
#define	Answer		0xf9
#define	EndBlock		0xf8
#define	WM_COMMDATA		WM_USER+10
#define	WM_COMMSEND		WM_USER+11

#ifdef	WIN32
	#define HCOMM	HANDLE
#else
	#define HCOMM	int
#endif

BOOL	CommNotify(WPARAM wParam,LPARAM lParam);

class	SerialComm	{
protected:
	
public:
	HCOMM GetHandle();
	SerialComm();
	~SerialComm();
	BOOL  ReOpen();
	BOOL  Close();
	BOOL  Open(int idport, LPSTR OpenStr);
	BOOL  Open(int Port, long BauRate, int ByteSize,int Parity, int StopBits,bool bOverlapped=false);
	BOOL	Init(LPSTR OpenStr);
	void	Init(HWND hWnd, UINT InSize, UINT OutSize);
	int	TransmitCommChar(char c);
	int	WriteComm(void FAR* pbyte, int len,OVERLAPPED* pOS=NULL);
	int	ReadComm(void FAR* pbyte, int len,OVERLAPPED* pOS=NULL);
	int   Connected,Status, IdPort;
	
	WORD	m_InSize, m_OutSize;
	HWND	m_hWnd;
	DCB	dcb;
	DWORD	m_ComMask;
	HCOMM	m_hComm;
};

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
