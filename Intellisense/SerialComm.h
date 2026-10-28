// SerialComm.h: interface for the CSerialComm class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SERIALCOMM_H
#define _SERIALCOMM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tagCOMMINFO
{
   HANDLE  hComDev ;
   OVERLAPPED  osWrite, osRead ;
} CCOMMINFO, NEAR *PCOMMINFO ;

#define COMDEV( x ) (x -> hComDev)
#define WRITE_OS( x ) (x -> osWrite)
#define READ_OS( x ) (x -> osRead)



class CSerialComm  
{
// Methods
public:
	// constuctor and destructor
	CSerialComm()
	{/*int port=2;*/}
	CSerialComm(const char *port);
	virtual ~CSerialComm();
	
	// Write data or command or query message into the serial line
	virtual BOOL write(const char *buffer, int nLen = 15) ;//const
	// Read response message from the serial port
	virtual int read(char *buffer,int nBytesToRead) ;
	//

	// Close the serial port communication
	void startOp();
	void stopOp();
	void flush();


protected:
	BOOL initport(const char *port);
	BOOL setDCB(void);


// Data Member 
protected:
	PCOMMINFO m_pCommInfo;
};

#endif // _SERIALCOMM_H
