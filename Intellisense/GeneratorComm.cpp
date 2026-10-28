// GeneratorComm.cpp: implementation of the CGeneratorComm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NSEHeader.h"
#include "GeneratorComm.h"
#include <string>
#include <stdlib.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGeneratorComm::CGeneratorComm(const char *port)
:CSerialComm(port)
{
	
}

CGeneratorComm::~CGeneratorComm()
{

}

// Is any alarm
int CGeneratorComm::status()    
{
	Sleepms(20);
	CString msg;
	if(NSE_XRAY_KIND==0)
	{
		char buf[4]="   ";
		char response[20]="                   ";
		msg.Format(_T("%cFLT%c"), 0x02, 0x3B);
		// checksum process
		int nSize = wcslen(msg);
		
		int x=0;
		int temp;
		for(int i=1;i<nSize;i++)
		{
			temp=msg.GetAt(i);
			x=x+temp;
		}
		int CSb1, CSb2, CSb3;
		CSb1 = 256 - x;
		CSb2 = CSb1 & 0x7f;
		CSb3 = CSb2 | 0x40;
		msg.Format(_T("%cFLT%c%c%c%c"), 0x02, 0x3B, CSb3, 0x0D, 0x0A);

		USES_CONVERSION;
		const char* cmsg = T2A(msg);  
		CSerialComm::write(cmsg);//const		
		Sleepms(200);

		readcomm(response,20);
	// Write the Query message into the X-Ray Generator
		
		buf[0]=response[1];
		buf[1]=response[2];
		buf[2]=response[3];

		int nCode;
		int status = 0;
		nCode=atoi(buf);//string->int
		switch(nCode) 
		{
			case 0:			//No alarm
				status = NONE_ERROR;
				break;
				
			case 1:
				status = TEMP_HIGH;
				break;

			case 2:
				status = ARC_FAULT;
				break;

			case 3:
				status = HIGH_MA;
				break;

			case 4:
				status = LOW_MA;
				break;

			case 5:
				status = LOW_KV;
				break;

			case 6:
				status = HIGH_KV;
				break;

			case 7:
				status = WATCHDOG_TIMER_EXPIRED;
				break;

			case 8:
				status = POWER_LIMIT;
				break;
		}
		return status;
	}
	else if(NSE_XRAY_KIND==1)
	{
		CString buf[9];
		char response[22]="                     ";

		msg.Format(_T("%cFLT%c"), 0x02, 0x0D);
		msg.Replace(_T(" "),_T(""));		//공백 없애기 
		msg.Replace(_T("\r\n"),_T(""));	    //엔터 없애기

		USES_CONVERSION;
		const char* cmsg = T2A(msg);  
		CSerialComm::write(cmsg);//const		
		Sleepms(200);
		readcomm(response,20);
	///////////////////////////////////////////////
		
	// Write the Query message into the X-Ray Generator
		buf[0]=response[17];   // X0
		buf[1]=response[15];   // X1
		buf[2]=response[13];   // X2
		buf[3]=response[11];   // X3
		buf[4]=response[9];    // X4
		buf[5]=response[7];    // X5
		buf[6]=response[5];    // X6
		buf[7]=response[3];    // X7
		buf[8]=response[1];    // X8

		int m_iCode[9];
		for(int a=0; a<9; a++)
		{
			m_iCode[a]=_wtoi(buf[a]);
			if(m_iCode[a]==1)
			{
				return a+1; //0은 정상처리 코드에서 +1해서 리턴
				break;
			}
		}
		return 0;
	}
	else if(NSE_XRAY_KIND==2)
	{
		char buf[8]="       ";
		char response[20]="                   ";

		msg.Format(_T("%cFLT%c"), 0x02, 0x3B);
		// checksum process
		int nSize = wcslen(msg);
		int x=0;
		int temp;
		for(int i=1;i<nSize;i++)
		{
			temp=msg.GetAt(i);
			x=x+temp;
		}
		int CSb1, CSb2, CSb3;
		CSb1 = 256 - x;
		CSb2 = CSb1 & 0x7f;
		CSb3 = CSb2 | 0x40;
		msg.Format(_T("%cFLT%c%c%c%c"), 0x02, 0x3B, CSb3, 0x0D, 0x0A);

		USES_CONVERSION;
		const char* cmsg = T2A(msg);  
		CSerialComm::write(cmsg);//const		
		Sleepms(200);
		readcomm(response,20);
	///////////////////////////////////////////////

	// Write the Query message into the X-Ray Generator
		buf[0]=response[8];   // X0
		buf[1]=response[7];   // X1
		buf[2]=response[6];   // X2
		buf[3]=response[5];   // X3
		buf[4]=response[4];    // X4
		buf[5]=response[3];    // X5
		buf[6]=response[2];    // X6
		buf[7]=response[1];    // X7
		int nCode;
		nCode=atoi(buf);
		int status = 0;
		switch(nCode) 
		{
			case 0:			//No alarm
				status = NONE_ERROR;
				break;		
			case 1:
				status = ARC_FAULT;
				break;
			case 10:
				status = TEMP_HIGH;
				break;
			case 100:
				status = HIGH_KV;
				break;
			case 1000:
				status = LOW_KV;
				break;
			case 10000:
				status = HIGH_MA;
				break;
			case 100000:
				status = LOW_MA;
				break;
			case 1000000:
				status = WATCHDOG_TIMER_EXPIRED;
				break;
			case 10000000:
				status = POWER_LIMIT;
				break;
		}

		return status;
	}

	return 0;
}

// Is X-Ray ON
int CGeneratorComm::isXRayOn()
{
	// pre-condition
	if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)//LIC_20121217
	{
		Sleepms(200);
		CString msg;
		char buf[4]="   ";
		char response[20]="                   ";

		//////////////////////////////////////////
		msg.Format(_T("%cSTAT%c"), 0x02, 0x3B);
		// checksum process
		int nSize = wcslen(msg);
		int x=0;
		int temp;
		CString str;
		for(int i=1;i<nSize;i++){
			temp=msg.GetAt(i);
			x=x+temp;
		}

		int CSb1, CSb2, CSb3;
		CSb1 = 256 - x;
		CSb2 = CSb1 & 0x7f;
		CSb3 = CSb2 | 0x40;
		msg.Format(_T("%cSTAT%c%c%c%c"), 0x02, 0x3B, CSb3, 0x0D, 0x0A);
		USES_CONVERSION;
		const char* cmsg = T2A(msg);  
		CSerialComm::write(cmsg);//const		

		Sleepms(200);
		readcomm(response,20);
	///////////////////////////////////////////////
		
		// Write the Query message into the X-Ray Generator
		
		buf[0]=response[1];


		return atoi(buf); //string ->int
	}
	else if(NSE_XRAY_KIND==1)
	{
		Sleepms(200);
		CString msg;
		char buf[4]="   ";
		char response[20]="                   ";

		//////////////////////////////////////////
		msg.Format(_T("%cSTAT%c"), 0x02, 0x0D);
		USES_CONVERSION;
		const char* cmsg = T2A(msg);  
		CSerialComm::write(cmsg);//const		

		Sleepms(200);
		readcomm(response,5);
	///////////////////////////////////////////////
		
		// Write the Query message into the X-Ray Generator
		
		buf[0]=response[1];

		return atoi(buf);
	}
	return 0;
}

// Get the current
int CGeneratorComm::getMA(void) 
{
	if(NSE_XRAY_KIND==0)
	{
		Sleepms(200);
		CString msg;
		char buf[4]="   ";
		char response[20]="                   ";

		//////////////////////////////////////////
		msg.Format(_T("%cIMON%c"), 0x02, 0x3B);
		// checksum process
		int nSize = wcslen(msg);
		int x=0;
		int temp=0;
		CString str;
		for(int i=1;i<nSize;i++)
		{
			temp=msg.GetAt(i);
			x=x+temp;
		}

		int CSb1, CSb2, CSb3;
		CSb1 = 256 - x;
		CSb2 = CSb1 & 0x7f;
		CSb3 = CSb2 | 0x40;
		msg.Format(_T("%cIMON%c%c%c%c"), 0x02, 0x3B, CSb3, 0x0D, 0x0A);
		USES_CONVERSION;
		const char* cmsg = T2A(msg);  
		CSerialComm::write(cmsg);//const		
		Sleepms(300);
		readcomm(response,20);


		// Write the Query message into the X-Ray Generator
		
		CString rekv;
		rekv=(CString)response;
		rekv=rekv.Left(5).Right(4);
		int a=_wtoi(rekv);

		return a;
	}
	else if(NSE_XRAY_KIND==2)//LIC_20131018
	{
		Sleepms(200);
		CString msg;
		char buf[4]="   ";
		char response[20]="                   ";

		//////////////////////////////////////////
		msg.Format(_T("%cIMON%c"), 0x02, 0x3B);
		// checksum process
		int nSize = wcslen(msg);
		int x=0;
		int temp=0;
		CString str;
		for(int i=1;i<nSize;i++)
		{
			temp=msg.GetAt(i);
			x=x+temp;
		}

		int CSb1, CSb2, CSb3;
		CSb1 = 256 - x;
		CSb2 = CSb1 & 0x7f;
		CSb3 = CSb2 | 0x40;
		msg.Format(_T("%cIMON%c%c%c%c"), 0x02, 0x3B, CSb3, 0x0D, 0x0A);
		USES_CONVERSION;
		const char* cmsg = T2A(msg);  
		CSerialComm::write(cmsg);//const		

		Sleepms(300);
		readcomm(response, 20);

		// Write the Query message into the X-Ray Generator
		CString rekv;
		rekv=(CString)response;
		rekv=rekv.Left(5).Right(4);

		int a=_wtoi(rekv);

		 a = a/3; // max값 4095를 맞추기 위한 상수값
		return a;
	}
	else if(NSE_XRAY_KIND==1)
	{
		Sleepms(200);
		CString msg;
		char buf[4]="   ";
		char response[21]="                    ";

		//////////////////////////////////////////
		msg.Format(_T("%cMON%c"), 0x02, 0x0D);
		USES_CONVERSION;
		const char* cmsg = T2A(msg);  
		CSerialComm::write(cmsg);//const		
		Sleepms(200);
		readcomm(response, 21);
		// Write the Query message into the X-Ray Generator
		
		buf[0]=response[6];
		buf[1]=response[7];
		buf[2]=response[8];
		buf[3]=response[9];

		return atoi(buf);
	}

	return 0;
}

// Get the voltage
int CGeneratorComm::getKV(void) 
{
	if(NSE_XRAY_KIND==0)
	{
		Sleepms(200);
		CString msg;
		char buf[4]="   ";
		char response[20]="                   ";

		//////////////////////////////////////////
		msg.Format(_T("%cVMON%c"), 0x02, 0x3B);
		// checksum process
		int nSize = wcslen(msg);
		int x=0;
		int temp;
		CString str;
		for(int i=1;i<nSize;i++){
			temp=msg.GetAt(i);
			x=x+temp;
		}

		int CSb1, CSb2, CSb3;
		CSb1 = 256 - x;
		CSb2 = CSb1 & 0x7f;
		CSb3 = CSb2 | 0x40;
		msg.Format(_T("%cVMON%c%c%c%c"), 0x02, 0x3B, CSb3, 0x0D, 0x0A);

		USES_CONVERSION;
		const char* cmsg = T2A(msg);  
		CSerialComm::write(cmsg);//const		

		Sleepms(300);
		readcomm(response,20);
	///////////////////////////////////////////////
	
		// Write the Query message into the X-Ray Generator
		CString rekv;
		rekv=(CString)response;
		rekv=rekv.Left(5).Right(4);
		int a=_wtoi(rekv);
	
		//a = a/ 46;

		if(a>300){         //kds20090320
			return (a/10); //string ->int
		}else{
			return	a;
		}
	}
	else if(NSE_XRAY_KIND==2)//LIC_20131018
	{
		Sleepms(200);
		CString msg;
		char buf[4]="   ";
		char response[20]="                   ";

		//////////////////////////////////////////
		msg.Format(_T("%cVMON%c"), 0x02, 0x3B);
		// checksum process
		int nSize = wcslen(msg);
		int x=0;
		int temp;
		CString str;
		for(int i=1;i<nSize;i++){
			temp=msg.GetAt(i);
			x=x+temp;
		}

		int CSb1, CSb2, CSb3;
		CSb1 = 256 - x;
		CSb2 = CSb1 & 0x7f;
		CSb3 = CSb2 | 0x40;
		msg.Format(_T("%cVMON%c%c%c%c"), 0x02, 0x3B, CSb3, 0x0D, 0x0A);

		USES_CONVERSION;
		const char* cmsg = T2A(msg);  
		CSerialComm::write(cmsg);//const		

		Sleepms(300);
		readcomm(response,20);
	///////////////////////////////////////////////
		CString rekv;
		rekv=(CString)response;
		rekv=rekv.Left(5).Right(4);
		// Write the Query message into the X-Ray Generator
		int a=_wtoi(rekv);
	
		a = a / 46;  // max값 4095를 맞추기위한 상수값

		if(a>300){         //kds20090320
			return (a/10); //string ->int
		}else{
			return	a;
		}
	}
	else if(NSE_XRAY_KIND==1)
	{
		Sleepms(200);
		CString msg;
		char buf[4]="   ";
		char response[21]="                    ";

		msg.Format(_T("%cMON%c"), 0x02, 0x3B);

		USES_CONVERSION;
		const char* cmsg = T2A(msg);  
		CSerialComm::write(cmsg);//const		
		Sleepms(200);
		readcomm(response,21);
		
		CString rekv;
		rekv=(CString)response;
		rekv=rekv.Left(5).Right(4);
		int a=_wtoi(rekv);

		return (a/10);
	}

	return 0;
}

// Setup the X-Ray generator
void CGeneratorComm::setKV(int kv)
{
	Sleepms(200);
	CString msg;
	char response[20]="                   ";

	if(NSE_XRAY_KIND==0)
	{
		if((kv<30) || (kv>140))
		{
			kv = DEFAULT_VOLTAGE;//35kv(350)
		}

		kv=(kv*10);
		msg.Format(_T("%cVREF%c%d%c"), 0x02, 0x20, kv, 0x3B);
		// checksum process
		int nSize = wcslen(msg);
		int x=0;
		int temp;
		CString str;
		for(int i=1;i<nSize;i++){
			temp=msg.GetAt(i);
			x=x+temp;
		}
		
		int CSb1, CSb2, CSb3;
		CSb1 = 256 - x;
		CSb2 = CSb1 & 0x7f;
		CSb3 = CSb2 | 0x40;
		msg.Format(_T("%cVREF%c%d%c%c%c%c"), 0x02, 0x20, kv, 0x3B, CSb3, 0x0D, 0x0A);
	}
	else if(NSE_XRAY_KIND==1)
	{
		kv=kv*10;
		
		//ltoa(kv,buf,10);// long int -> string으로 10진수
		msg.Format(_T("%cVP%04d%c"), 0x02, kv, 0x0D);
	}
	else if(NSE_XRAY_KIND==2)//LIC_20131018
	{
		kv = kv * 46; //max값 4095를 맞추기위한 상수값
		msg.Format(_T("%cVREF%c%04d%c"), 0x02, 0x20, kv, 0x3B);
		// checksum process
		int nSize = wcslen(msg);
		int x=0;
		int temp;
		CString str;
		for(int i=1;i<nSize;i++){
			temp=msg.GetAt(i);
			x=x+temp;
		}

		int CSb1, CSb2, CSb3;
		CSb1 = 256 - x;
		CSb2 = CSb1 & 0x7f;
		CSb3 = CSb2 | 0x40;
		msg.Format(_T("%cVREF%c%04d%c%c%c%c"), 0x02, 0x20, kv, 0x3B, CSb3, 0x0D, 0x0A);
	}
	
	USES_CONVERSION;
	const char* cmsg = T2A(msg);  
	CSerialComm::write(cmsg);//const		
	int index=0;
	index=readcomm(response,20);
}

void CGeneratorComm::setMA(int ma)
{
	Sleepms(200);
	CString msgg, ss_ma;
	char response[20]="                   ";	

	if(NSE_XRAY_KIND==0)
	{
		msgg.Format(_T("%cIREF%c%04d%c"), 0x02, 0x20, ma, 0x3B);
	//	ma = ma * 3;

		//checksum process
		int nSize = wcslen(msgg);

		int x=0;
		int temp=0;
		CString str;
		for(int i=1;i<nSize;i++)
		{
			temp=msgg.GetAt(i);
			x=x+temp;
		}


		int CSb1, CSb2, CSb3;
		CSb1 = 256 - x;
		CSb2 = CSb1 & 0x7f;
		CSb3 = CSb2 | 0x40;
		msgg.Format(_T("%cIREF%c%04d%c%c%c%c"), 0x02, 0x20, ma, 0x3B, CSb3, 0x0D, 0x0A);

	}
	else if(NSE_XRAY_KIND==1)
	{
		msgg.Format(_T("%cCP%d%c"), 0x02, ma, 0x0D);
	}
	else if(NSE_XRAY_KIND==2)//LIC 20131018
	{
		ma*=3;
		msgg.Format(_T("%cIREF%c%04d%c"), 0x02, 0x20, ma, 0x3B);
		//checksum process
		int nSize = wcslen(msgg);

		int x=0;
		int temp=0;
		CString str;
		for(int i=1;i<nSize;i++){
			temp=msgg.GetAt(i);
			x=x+temp;
		}


		int CSb1, CSb2, CSb3;
		CSb1 = 256 - x;
		CSb2 = CSb1 & 0x7f;
		CSb3 = CSb2 | 0x40;
		msgg.Format(_T("%cIREF%c%04d%c%c%c%c"), 0x02, 0x20, ma, 0x3B, CSb3, 0x0D, 0x0A);
	}

	USES_CONVERSION;
	const char* cmsg = T2A(msgg);  
	CSerialComm::write(cmsg);//const		
	int index=0;
	index=readcomm(response,20);
}

void CGeneratorComm::setXRay(bool bON)
{
	Sleepms(200);
	CString msg; 
	char response[20]="                   ";
	if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
	{
		msg.Format(_T("%cENBL%c0%c"), 0x02, 0x20, 0x3B);
		int nSize = wcslen(msg);
		int x=0;
		int temp=0;
		CString str;
		for(int i=1;i<nSize;i++)
		{
			temp=msg.GetAt(i);
			x=x+temp;
		}

		int CSb1, CSb2, CSb3;
		CSb1 = 256 - x;
		CSb2 = CSb1 & 0x7f;
		CSb3 = CSb2 | 0x40;

		msg.Format(_T("%cENBL%c0%c%c%c%c"), 0x02, 0x20, 0x3B, CSb3, 0x0D, 0x0A);

		if(bON)
		{
			msg.Format(_T("%cENBL%c1%c"), 0x02, 0x20, 0x3B);

			CString str;
			int nSize = wcslen(msg);
			x=0;
			for(int i=1;i<nSize;i++)
			{
				temp=msg.GetAt(i);
				x=x+temp;
			}
			int aaa=0;

			int CSb1, CSb2, CSb3;
			CSb1 = 256 - x;
			CSb2 = CSb1 & 0x7f;
			CSb3 = CSb2 | 0x40;

			msg.Format(_T("%cENBL%c1%c%c%c%c"), 0x02, 0x20, 0x3B, CSb3, 0x0D, 0x0A);
		}
	}
	else if(NSE_XRAY_KIND==1)
	{
		msg.Format(_T("%cENBL0%c"), 0x02, 0x0D);
		if(bON)
		{
			msg.Format(_T("%cENBL1%c"), 0x02, 0x0D);
		}
	}

	USES_CONVERSION;
	const char* cmsg = T2A(msg);  
	CSerialComm::write(cmsg);//const		
	readcomm(response, 20);
}

///////////////Function desrciption ///////////////////////
///
///    If no response, return error
///
///////////////////////////////////////////////////////////
int CGeneratorComm::readcomm(char *buffer, int nLen) 
{
	int result = 0;
	int nTimes = 0;
	bool bCommOK = true;
	int nBytesRead = 0;

	while((nBytesRead<nLen) && bCommOK)
	{
		int nBytes = read(&buffer[nBytesRead], nLen-nBytesRead);

		if(nBytes != 0)
		{
			nBytesRead += nBytes;
		}
		else 
		{
			nTimes++;
			Sleepms(60);
		}
		
		if(nTimes == MAX_ITERATION_TIMES)
		{
			bCommOK = false;
			result = COMM_NO_RESPONSE;
		}
	}

	return result;
}
   

// 오류 발생시 클리어
void CGeneratorComm::setClear()
{
	// pre-condition
	Sleepms(200);
	CString msg;
	char buf[4]="   ";
	char response[20]="                   ";

	if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
	{
		msg.Format(_T("%cCLR%c"), 0x02, 0x3B);
		// checksum process
		int nSize = wcslen(msg);
		int x=0;
		int temp;
		CString str;
		for(int i=1;i<nSize;i++)
		{
			temp=msg.GetAt(i);
			x=x+temp;
		}

		int CSb1, CSb2, CSb3;
		CSb1 = 256 - x;
		CSb2 = CSb1 & 0x7f;
		CSb3 = CSb2 | 0x40;
	
		msg.Format(_T("%cCLR%c%c%c%c"), 0x02, 0x3B, CSb3, 0x0D, 0x0A);
	}
	else if(NSE_XRAY_KIND==1)
	{
		msg.Format(_T("%cCLR%c"), 0x02, 0x0D);
	}

	USES_CONVERSION;
	const char* cmsg = T2A(msg);  
	CSerialComm::write(cmsg);//const		
	Sleepms(200);
	readcomm(response,20);
}

