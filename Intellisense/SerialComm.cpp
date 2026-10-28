// SerialComm.cpp: implementation of the CSerialComm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerialComm.h"
#include "NSEException.h"
#include "assert.h"
#include "NSEHeader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ASCII_XON	0x11
#define ASCII_XOFF	0x13

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSerialComm::CSerialComm(const char *port)
{	
	// Initialize the OVERLAPPED structure of the CCOMMINFO data member
    if(m_pCommInfo=(PCOMMINFO)LocalAlloc(LPTR, sizeof(CCOMMINFO)))
	{
		COMDEV(m_pCommInfo) = 0;

		WRITE_OS( m_pCommInfo ).Offset = 0 ;
		WRITE_OS( m_pCommInfo ).OffsetHigh = 0 ;
		READ_OS( m_pCommInfo ).Offset = 0 ;
		READ_OS( m_pCommInfo ).OffsetHigh = 0 ;

		// create I/O event used for overlapped reads / writes
		READ_OS( m_pCommInfo ).hEvent = CreateEvent( NULL,    // no security
			    									 TRUE,    // explicit reset req
													 FALSE,   // initial event reset
													 NULL ) ; // no name
		if(READ_OS( m_pCommInfo ).hEvent == NULL)
		{
		    LocalFree( m_pCommInfo ) ;
		    CNSEException e("Event Create error!");
		    throw e;
		 }
	
		 WRITE_OS( m_pCommInfo ).hEvent = CreateEvent( NULL,    // no security
													   TRUE,    // explicit reset req
													   FALSE,   // initial event reset
													   NULL ) ; // no name
		 	
		 if (NULL == WRITE_OS( m_pCommInfo ).hEvent)
		 {
		     CloseHandle( READ_OS( m_pCommInfo ).hEvent ) ;
			 LocalFree( m_pCommInfo ) ;

			 CNSEException e("Event Create error!");
			 throw e;
		  }
				
		// Initialize the serial port
		initport(port);	
		
	}
	else
	{
		CNSEException e("Serial Communication initial error!");
		throw e;
	}

	
}

CSerialComm::~CSerialComm()
{
	stopOp();
	CloseHandle( READ_OS( m_pCommInfo ).hEvent ) ;
	CloseHandle( WRITE_OS( m_pCommInfo ).hEvent ) ;
	CloseHandle( COMDEV(m_pCommInfo) );
	LocalFree( m_pCommInfo ) ;
}

BOOL CSerialComm::initport(const char *port)
{
	// Pre-condition
	COMMTIMEOUTS commTimeouts = {0,0,0,0,0};
	commTimeouts.ReadIntervalTimeout = MAXDWORD;
    commTimeouts.WriteTotalTimeoutConstant = 500 ;

	// "COMx:" should work, but if you need > 9 you need to use this format "\\\\.\\COMx".
	COMDEV(m_pCommInfo) =:: CreateFile(_T("COM2"), GENERIC_READ | GENERIC_WRITE,
                  0,                      // exclusive access
                  NULL,                   // no security attrs
                  OPEN_EXISTING,		  // Must be
                  FILE_ATTRIBUTE_NORMAL  |
                  FILE_FLAG_OVERLAPPED,    // overlapped I/O
                  NULL );				  // handle to template file

	// 장비연결시에는 에러 안남 
	if(COMDEV(m_pCommInfo) == INVALID_HANDLE_VALUE)
	{	
		g_bGenerator = false;
		return false;
		CNSEException e("Serial Communication Open error!");
		throw e	;
		
	}

	
	// The following do: /1/ Get any early notifications
	//                /2/ Setup serial port buffers sizes
	//			  /3/ Set up for overlapped I/O	
	//		 /4/ empty serial port buffer, purge any information in the buffer
	if(!SetCommMask(COMDEV(m_pCommInfo), EV_RXCHAR) ||
		!SetupComm(COMDEV(m_pCommInfo),4096,4096) ||
		!PurgeComm(COMDEV(m_pCommInfo),PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR) ||
		!SetCommTimeouts( COMDEV(m_pCommInfo), &commTimeouts ))	
	{
		
		CNSEException e("Serial Communication Config error!");
		throw e	;
	}
	// ***** Setup DCB based connection
	if(!setDCB())
	{
		CNSEException e("Serial Communication Setup DCB error!");
		throw e	;
	}
 
	return true;
}

BOOL CSerialComm::setDCB()
{
	// pre-condition
	DCB dcb;
	BOOL bDTRDSR,bRTSCTS,bXONXOFF ;
	int nParity,nStopBits;
	CWinApp *pApp = AfxGetApp();

	dcb.DCBlength = sizeof(DCB);
	if (!GetCommState(COMDEV(m_pCommInfo),&dcb)){
		return FALSE;
	}

	dcb.DCBlength = sizeof(DCB);
	dcb.XonChar = ASCII_XON;
	dcb.XoffChar = ASCII_XOFF;
	dcb.XonLim = 100;
	dcb.XoffLim = 100;
	dcb.fBinary = TRUE;
	dcb.fParity = TRUE;
	if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==1)
	{
		dcb.BaudRate = pApp->GetProfileInt(_T("Communication"),_T("Baud"),9600); 
	}
	else if(NSE_XRAY_KIND==2)
	{
		dcb.BaudRate = pApp->GetProfileInt(_T("Communication"),_T("Baud"),115200);	
	}

	dcb.ByteSize = pApp->GetProfileInt(_T("Communication"),_T("DataBits"),8);  

	bDTRDSR		= (BOOL)(LPCTSTR)pApp->GetProfileInt(_T("Communication"),_T("DTRDSR"),0);  
	dcb.fOutxDsrFlow = bDTRDSR;
	dcb.fDtrControl = bDTRDSR ? DTR_CONTROL_HANDSHAKE : DTR_CONTROL_DISABLE;
	
	bRTSCTS		= (BOOL)(LPCTSTR)pApp->GetProfileInt(_T("Communication"),_T("RTSCTS"),0);  
	dcb.fOutxCtsFlow = bRTSCTS;
	dcb.fRtsControl = bRTSCTS ? RTS_CONTROL_HANDSHAKE : RTS_CONTROL_DISABLE;

	bXONXOFF	= (BOOL)(LPCTSTR)pApp->GetProfileInt(_T("Communication"),_T("XONXOFF"),0);    
	dcb.fInX	= dcb.fOutX = bXONXOFF;

	
	if(NSE_XRAY_KIND==0) //Spellman
		nParity	= pApp->GetProfileInt(_T("Communication"),_T("Parity"),1);  
	else //VJ,Spellman_100W
		nParity	= pApp->GetProfileInt(_T("Communication"),_T("Parity"),0);  
	switch(nParity)
	{
		case 0: dcb.Parity = NOPARITY; break;
		case 1: dcb.Parity = EVENPARITY; break;
		case 2: dcb.Parity = ODDPARITY; break;
		case 3: dcb.Parity = MARKPARITY; break;
		case 4: dcb.Parity = SPACEPARITY; break;
		default: ASSERT(FALSE);
	}
	
	nStopBits	= pApp->GetProfileInt(_T("Communication"),_T("StopBits"),0);   
	switch (nStopBits)
	{
		case 0: dcb.StopBits = ONESTOPBIT; break;
		case 1: dcb.StopBits = ONE5STOPBITS; break;
		case 2: dcb.StopBits = TWOSTOPBITS; break;
		default: ASSERT(FALSE);
	}

	if (!SetCommState(COMDEV(m_pCommInfo),&dcb))
	{
		return FALSE;
	}

	
	return true;
}

int CSerialComm::read (char *buffer, int nBytesToRead)
{
	DWORD	dwRead, dwError, dwErrorFlags;
	COMSTAT	comstat;

	//--- system queue에 도착한 byte수만 미리 읽는다.
	ClearCommError( COMDEV(m_pCommInfo), &dwErrorFlags, &comstat);

	//--> 시스템 큐에서 읽을 거리가 있으면..
	dwRead = comstat.cbInQue;
	
	if(dwRead > 0)
	{
		//--> 버퍼에 일단 읽어들이는데.. 만일..읽어들인값이 없다면..
		if(!ReadFile( COMDEV(m_pCommInfo), buffer, nBytesToRead, &dwRead, &READ_OS(m_pCommInfo)) )//오버랩대신 NULL로 하면 통신 안됨 
		{
			//--> 읽을 거리가 남았으면..
			if(GetLastError() == ERROR_IO_PENDING)
			{
				//--------- timeouts에 정해준 시간만큼 기다려준다.
				while(!GetOverlappedResult( COMDEV(m_pCommInfo), &READ_OS(m_pCommInfo), &dwRead, TRUE))
				{
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError( COMDEV(m_pCommInfo), &dwErrorFlags, &comstat);
						break;
					}
				}
			}
			else
			{
				dwRead = 0;
				ClearCommError(COMDEV(m_pCommInfo), &dwErrorFlags, &comstat);
			}
		}
	}
	//--> 실제 읽어들인 갯수를 리턴.
	return dwRead;
}

BOOL CSerialComm::write(const char *buffer,int nLen)//const
{
	DWORD dwWritten;
	int nSize = wcslen((CString)buffer);
	nLen = (nSize<nLen)? nSize: nLen;

	if(!WriteFile(COMDEV(m_pCommInfo),buffer,(DWORD)nLen, &dwWritten, &WRITE_OS( m_pCommInfo )) &&
		ERROR_IO_PENDING == GetLastError()) // 
	{	
		// write is pending
		if(!WaitForSingleObject(WRITE_OS( m_pCommInfo ).hEvent,WRITE_TIMEOUT))
		{		
			// OVERLAPPED structure's event has been signaled.
			GetOverlappedResult(COMDEV(m_pCommInfo),&WRITE_OS( m_pCommInfo ),
								&dwWritten,FALSE);
			WRITE_OS( m_pCommInfo ).Offset += dwWritten ;
			int uiii=0;

		}
		else
		{
			dwWritten =  0;
		}
	}
	else
	{

	}
	
	return TRUE;
}

void CSerialComm::startOp()
{
	if(!SetCommMask(COMDEV(m_pCommInfo), EV_RXCHAR)) 
	{
		return;
	}

	PurgeComm(COMDEV(m_pCommInfo), PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	EscapeCommFunction(COMDEV(m_pCommInfo),SETDTR);
}

void CSerialComm::stopOp()
{

	SetCommMask(COMDEV(m_pCommInfo),0);
	PurgeComm(COMDEV(m_pCommInfo), PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	EscapeCommFunction(COMDEV(m_pCommInfo),CLRDTR);
}


void CSerialComm::flush()
{
	if(!FlushFileBuffers(COMDEV(m_pCommInfo)))
	{
//		CNSEException err("Flush error !");
//		throw err;		
	}
}
