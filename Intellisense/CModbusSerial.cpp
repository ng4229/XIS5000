// CModbusSerial.cpp: implementation of the CModbusSerial class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CModbusSerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModbusSerial::CModbusSerial()
{
	this->hComm = NULL;
		
}

CModbusSerial::~CModbusSerial()
{
	if( IsOpen() ) {
		Close();
	}
}

////////////////////////////////////////////////////////////////////
// Private method
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Protected method
////////////////////////////////////////////////////////////////////

/*******************************************************************************************************
// InputRegister 읽기 Format 생성자( Modbus는 40001번의 wAddress 0으로 설정 됩니다 )
// DeviceID : 상태 DeviceID
// wAddress : Modbus 주소
// wCount	: 읽을 WORD 개수
// pCommand : Modbus protocol Format retrun 됩니다
*******************************************************************************************************/
WORD CModbusSerial::FormatForReadInputRegister(BYTE DeviceID, WORD wAddress, WORD& wCount, BYTE* pCommand)
{
	// START	ADDRESS		FUNCTION	DATA	LRC-CHECK	END
	// 1 CHAR	2 CHARS		2 CHARS		2 CHARS	2 CHARS		2 CHARS
	//	:													CRLF

	CString Buff = _T("");
	BYTE LRC[2];
	
	// LRC Check Byte 생성
	BYTE LRCChecked[6];
	LRCChecked[0] = (BYTE)DeviceID;
	LRCChecked[1] = 0x04;			//Read Input Register
	memcpy( LRC, &wAddress, 2 );
	LRCChecked[2] = LRC[0];
	LRCChecked[3] = LRC[1];
	memcpy( LRC, &wCount, 2 );
	LRCChecked[4] = LRC[0];
	LRCChecked[5] = LRC[1];


	// Start
	pCommand[0] = (BYTE)':';
	// Address (Device ID)
	Buff.Format(_T("%02X"), DeviceID);
	pCommand[1] = (BYTE)Buff[0];
	pCommand[2] = (BYTE)Buff[1];
	//Function
	pCommand[3] = (BYTE)'0';
	pCommand[4] = (BYTE)'4';		//Read Input Register
	//Start Address Hi
	Buff.Format(_T("%04X"), wAddress);
	pCommand[5] = (BYTE)Buff[0];
	pCommand[6] = (BYTE)Buff[1];
	//Start Address Low
	pCommand[7] = (BYTE)Buff[2];
	pCommand[8] = (BYTE)Buff[3];
	Buff.Format(_T("%04X"), wCount);
	//Count Hi
	pCommand[9] = (BYTE)Buff[0];
	pCommand[10] = (BYTE)Buff[1];
	//Count Hi
	pCommand[11] = (BYTE)Buff[2];
	pCommand[12] = (BYTE)Buff[3];
	//LRC
	LRC[0] = MakeLRC( LRCChecked, 6);
	Buff.Format(_T("%02X"), LRC[0]);
	pCommand[13] = (BYTE)Buff[0];
	pCommand[14] = (BYTE)Buff[1];

	pCommand[15] = 0x0D;
	pCommand[16] = 0x0A;

	return 17;

}

/*******************************************************************************************************
// WriteSingleRegister 쓰기 Format ( Modbus Write Single Register쓰기는 하나의 WORD만 쓰기 합니다. 40001번 wAddress 0으로 설정됩니다)
// DeviceID : 상태 DeviceID
// wAddress : Modbus 주소
// wData	: 쓰기 아날로그 데이터값
// pCommand : Modbus protocol Format retrun 됩니다
*******************************************************************************************************/
WORD CModbusSerial::FormatForWriteSingleRegister(BYTE DeviceID, WORD wAddress, WORD& wData, BYTE* pCommand)
{
	//Station Number|Func Code|Register|Address Hi|Register Address Lo|Force Data Hi|Force Data Lo|Error Check
	//		11			06			00					01				00				03			9A 98

	CString Buff = _T("");
	BYTE LRC[2];
	
	// LRC Check Byte 생성
	BYTE LRCChecked[6];
	LRCChecked[0] = (BYTE)DeviceID;
	LRCChecked[1] = 0x06;			//Write HoldingRegister
	memcpy( LRC, &wAddress, 2 );
	LRCChecked[2] = LRC[0];
	LRCChecked[3] = LRC[1];
	memcpy( LRC, &wData, 2 );
	LRCChecked[4] = LRC[0];
	LRCChecked[5] = LRC[1];

	// Start
	pCommand[0] = (byte)':';
	// Address (Device ID)
	Buff.Format(_T("%02X"), DeviceID);
	pCommand[1] = (BYTE)Buff[0];
	pCommand[2] = (byte)Buff[1];
	//Function
	pCommand[3] = (BYTE)'0';
	pCommand[4] = (BYTE)'6';		//Write HoldingRegister
	//Start Address Hi
	Buff.Format(_T("%04X"), wAddress);
	pCommand[5] = (BYTE)Buff[0];
	pCommand[6] = (BYTE)Buff[1];
	//Start Address Low
	pCommand[7] = (BYTE)Buff[2];
	pCommand[8] = (BYTE)Buff[3];
	Buff.Format(_T("%04X"), wData);
	//Count Hi
	pCommand[9] = (BYTE)Buff[0];
	pCommand[10] = (BYTE)Buff[1];
	//Count Hi
	pCommand[11] = (BYTE)Buff[2];
	pCommand[12] = (BYTE)Buff[3];
	//LRC
	LRC[0] = MakeLRC( LRCChecked, 6);
	Buff.Format(_T("%02X"), LRC[0]);
	pCommand[13] = (BYTE)Buff[0];
	pCommand[14] = (BYTE)Buff[1];

	pCommand[15] = 0x0D;
	pCommand[16] = 0x0A;

	return 17;

}
/*******************************************************************************************************
// MODBUS ASCII CheckSum 생성
*******************************************************************************************************/
BYTE CModbusSerial::MakeLRC(BYTE* LRC, WORD wLength)
{
	BYTE lrc = 0;
    for (WORD ii = 0; ii < wLength; ii++)
        lrc += LRC[ii];
    return (BYTE)(-(byte)lrc);

}
////////////////////////////////////////////////////////////////////
// Public method
////////////////////////////////////////////////////////////////////

/*******************************************************************************************************
// 시리얼포트 열여있는지 확인
*******************************************************************************************************/
BOOL CModbusSerial::IsOpen()
{
	// 핸들이 NULL이 아니라면 시리얼 포트를 연 것이라고 판단
	return this->hComm != NULL ? TRUE : FALSE;
}

/*******************************************************************************************************
// 시리얼포트 열기 ( Byte Size : 8, Parity : NONPARITY , Stop Bit : ONESTOPBIT  변경은 소스에서 )
	// nPort	: 포트 번호
	// nSpeed   : Baud Rate
*******************************************************************************************************/
BOOL CModbusSerial::Open( int nPort, UINT nBaudRate /*= 9600*/)
{
	 if( IsOpen() ) {
		return FALSE;
	}

	memset( &readOverlap, 0x00, sizeof( readOverlap ) );
	memset( &writeOverlap, 0x00, sizeof( writeOverlap ) );
	// 읽기 오버랩
	readOverlap.hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	// 쓰기 오버랩
	writeOverlap.hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

	CString strPort = _T("");

	if( nPort < 10 ) { //시리얼 포트 파일명을 만든다.
		strPort.Format(_T("COM%d"), nPort );
	} else {
		// \\.\COM
		strPort.Format(_T("\\\\.\\COM%d"), nPort );
	}

	//시리얼 포트를 연다.
	/*hComm = CreateFile( (LPCTSTR)strPort, GENERIC_READ | GENERIC_WRITE, 0, NULL,
						OPEN_EXISTING, 0, NULL );*/

	hComm = CreateFile( (LPCTSTR)strPort, GENERIC_READ | GENERIC_WRITE, 0, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, NULL );

	if( hComm == INVALID_HANDLE_VALUE ) {
		// 다른 process에 의해 점유
		Close();
		return FALSE;
	}
	
	//communication eviroment을 얻어오고 고친다.
	DCB dcb;
	memset( &dcb, 0x00, sizeof( dcb ) );
	dcb.DCBlength = sizeof( dcb );
	//시스템에 설정된 DCB 값을 얻어옮
	if( !GetCommState( hComm, &dcb ) ) {
		Close();
		return FALSE;
	}

	dcb.BaudRate = nBaudRate;		//속도 패리티, 바이트 크기, 스탑비트수 세팅
	dcb.Parity = NOPARITY;			//패리티 지정 0 NOPARITY, 1 ODDPARITY
	dcb.fParity = FALSE;
	dcb.ByteSize = 8;				//1바이트를 표현하기위한 비트수 지정
	dcb.StopBits = ONESTOPBIT;		//스탑비트 지정
									// 0 ONESTOPBIT , 1 ONE5STOPBITS
	dcb.fNull = FALSE;				//만약 0x00 값을 송수신하지 않으려면 TRUE 값을 넣는다.

	//flow control setting(여기서는 흐름제어 사용안함으로 설정)
	/*dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = TRUE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fTXContinueOnXoff = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fErrorChar = FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	*/
	SetCommState( hComm, &dcb );
	
	// 시리얼 포트 환경 세팅 완료
	
	SetCommMask( hComm, EV_RXCHAR ); // RX에 데이터가 수신된 경우에만 이벤트를 받음
	SetupComm(hComm, 4096, 4096);

	//clear buffer
	PurgeComm( hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ); 
	DWORD dwCommError = 0;
	ClearCommError( hComm, &dwCommError, NULL ); //시리얼 포트에 수신된 모든 에러를 초기화함

	COMMTIMEOUTS CommTimeOut;
	GetCommTimeouts( hComm, &CommTimeOut);//타임아웃 정보 얻기
	CommTimeOut.ReadTotalTimeoutConstant = 0;//MSDN에는 MAXDOWRD 설정과 0 과 동일하게 동작한다고 하구서는 
	CommTimeOut.ReadIntervalTimeout = 0;//실질적으로는 0으로 세팅해야 밑의 함수가 성공한다.
	CommTimeOut.ReadTotalTimeoutMultiplier  = 0;
	//CommTimeOut.WriteTotalTimeoutConstant = 0;
	//CommTimeOut.WriteTotalTimeoutMultiplier= 0;
	//쓰기에 관련된 타임 아웃은 설정하실 필요가 없습니다.
	//왜냐 무조건 다 전송해야지만 상대방이 알아먹거든요

	SetCommTimeouts( hComm, &CommTimeOut );//타임아웃지정
	

	return TRUE;
}

/*******************************************************************************************************
// 수신된 데이터 읽기
// 통신포트에서 데이터를 읽어 옮니다
// pData : 읽을 데이터 버퍼
// wSize : 데이터 버퍼 Size
*******************************************************************************************************/
WORD CModbusSerial::ReadComm( BYTE* pData, WORD& wSize )
{
	register DWORD dwReadSize = 0; //수신버퍼에서 읽은 량을 저장할 것이다.
	
	if( IsOpen() == FALSE ) return (WORD)dwReadSize;

	if( pData == NULL || wSize == 0 ) return (WORD)dwReadSize;

	//수신버퍼에 있는 데이터량을 가져온다.
	COMSTAT State;
	ClearCommError( hComm, &dwReadSize, &State );
	// 비교해서 작은 쪽으로 선택한다.
	if( (WORD)State.cbInQue < wSize ) wSize = (WORD)State.cbInQue ;
	
	//오버랩을 초기화한다. 다른 값은 OS가 사용하는 것이므로 손대면 책임 못 진다.
	readOverlap.OffsetHigh = readOverlap.Offset = 0;

	if( !ReadFile( hComm, pData, wSize, &dwReadSize, &readOverlap ) ) {
		DWORD dwError = GetLastError();
		//Error가 발생했다면 Error 처리 하세요
		while( dwError == ERROR_IO_PENDING || dwError == ERROR_IO_INCOMPLETE ) {
			// 입출력 대기 또는 입출력이 종결되지 았다면, 오버랩에게 나머지 일을 떠넘긴다.
			if( GetOverlappedResult( hComm, &readOverlap, &dwReadSize, TRUE ) ) {
				// 수신 완료할때까지 loop
				return (WORD)dwReadSize; //읽은 데이터 크기를 리턴한다.
			}
			dwError = GetLastError();
		}
		// 수신된 에러 데이터 초기화
		ClearCommError( hComm, &dwError, &State );
	}

	return (WORD)dwReadSize;//읽은 데이터 크기를 리턴한다.
}

/*******************************************************************************************************
// 시리얼 포트 닫기
*******************************************************************************************************/
void CModbusSerial::Close()
{
	if( readOverlap.hEvent ) {
		//오버랩 핸들이 지정됬다면 해제한다.
		CloseHandle( readOverlap.hEvent );
		readOverlap.hEvent = NULL;
	}

	if( writeOverlap.hEvent ) {
		//오버랩 핸들이 지정됬다면 해제한다.
		CloseHandle( writeOverlap.hEvent );
		writeOverlap.hEvent = NULL;
	}

	if( this->hComm ) {
		//시리얼 포트에 대한 핸들을 닫는다.
		CloseHandle( this->hComm );
		this->hComm = NULL;
	}

}

/*******************************************************************************************************
// 데이터 송신
// 통신 포트에 데이터를 씁니다
// pData : 쓰기 데이터
// wSize : 쓰기 데이터 Size
*******************************************************************************************************/
WORD CModbusSerial::WriteComm( BYTE* pData, WORD& wSize )
{
	if( IsOpen() == FALSE ) return 0;
	if( pData == NULL || wSize == 0 ) return 0;

	//오버랩을 초기화한다. 다른 값은 OS가 사용하는 것이므로 손대면 책임 못 진다.
	writeOverlap.Offset = 0;
	writeOverlap.OffsetHigh = 0;

	DWORD dwWriteSize = 0;// 전송한 데이터의 크기를 저장할 것이다.

	if( !WriteFile( hComm, pData, wSize, &dwWriteSize, &writeOverlap ) ) {
		DWORD dwError = GetLastError();
		//Error가 발생했다면 Error 처리 하세요
		while( dwError == ERROR_IO_PENDING || dwError == ERROR_IO_INCOMPLETE ) {
			//입출력 대기 또는 입출력이 종결되지 았다면, 오버랩에게 나머지 일을 떠넘긴다.
			if( GetOverlappedResult( hComm, &writeOverlap, &dwWriteSize, TRUE ) ) {
				// 송신 완료할때까지 loop
				dwError = GetLastError();
				return (WORD)dwWriteSize;//전송한 데이터 크기 리턴
			}
			dwError = GetLastError();
		}
		COMSTAT State;
		ClearCommError( hComm, &dwError, &State );//진짜 에러네, 에러 초기화!
	}
	
	return (WORD)dwWriteSize;
}

/*******************************************************************************************************
// Input Register 값을 읽어 옯니다.( Modbus는 40001번의 wAddress 0으로 설정 됩니다 )
// DeviceID : 상태 DeviceID
// wAddress : Modbus 주소
// wCount	: 쓰기 아날로그 데이터값
// pData	: 아날로그 데이터가 리턴됩니다.( wCount개의 배열 )
*******************************************************************************************************/
BOOL CModbusSerial::ReadInputRegister(BYTE DeviceID, WORD wAddress, WORD& wCount, WORD* pData)
{
	if( !IsOpen() ) return FALSE;

	BYTE Command[17];
	BYTE ReadBuff[2048];
	// 전송 Protocol을 생성합니다
	WORD nSend = FormatForReadInputRegister(DeviceID, wAddress, wCount, Command);

	// 전송
	WORD nWrite = WriteComm( Command, nSend );
	if( nWrite < nSend ) {
		return FALSE;
	}
	Sleepms( 20 );
	WORD nSize = 2048;
	int nRead = ReadComm( ReadBuff, nSize );
	// 4개의 Byte가 하나의 Word가 됩니다
	if( nRead == (11 + (wCount * 4)) ) {
		for( WORD nIndex = 0; nIndex < wCount; nIndex++ ) {
			char cBuf[4];
			memcpy(cBuf, &ReadBuff[7 + (nIndex*4)], 4);
			// Byte값을 Word(long)값으로 변환 합니다.
			pData[nIndex] = (WORD)strtoul(cBuf, NULL, 16);
		}
	} else {
		return FALSE;
	}
	
	return TRUE;
}
/*******************************************************************************************************
// WriteSingleRegister 쓰기 ( Modbus Write Single Register쓰기는 하나의 WORD만 쓰기 합니다. 40001번 wAddress 0으로 설정됩니다)
// DeviceID : 상태 DeviceID
// wAddress : Modbus 주소
// wData	: 쓰기 아날로그 데이터값
*******************************************************************************************************/
BOOL CModbusSerial::WriteSingleRegister(BYTE DeviceID, WORD wAddress, WORD& wData)
{
	BYTE Command[17];
	BYTE ReadBuff[2048];
	// 쓰기 Protocol 생성
	WORD nSend = FormatForWriteSingleRegister(DeviceID, wAddress, wData, Command);
	WORD nRead = 0;
	WORD nSize = 2048;
	if( WriteComm( Command, nSend ) >= nSend ) {
		Sleepms( 30 );
		nRead = ReadComm( ReadBuff, nSize ); 
	} else {
		// 쓰기 오류가 발생했다면 Error 처리 하세요
		return FALSE;
	}
	
	return nRead > 0 ?TRUE:FALSE;
}
/*******************************************************************************************************
// Word 값을 16개의 BOOL 배열로 변환 합니다
// wData : 변환할 Word
// pBit  : 변환된 16개의 Bit 배열
*******************************************************************************************************/
void CModbusSerial::WordToBit( WORD& wData, BOOL* pBit)
{
	for( int nIndex = 15; nIndex >= 0; nIndex-- ) {
		WORD w = wData;
		int n = nIndex;
		//pBit[15-nIndex] = (w & (1 << n)) >> n;
		pBit[nIndex] = (w & (1 << n)) >> n;//kds20140225
	}
}
/*******************************************************************************************************
// 16개의 BOOL 값을 WORD로 변환 합니다
// pBit : 변환할 16개의 BOOL
*******************************************************************************************************/
WORD CModbusSerial::BitToWord(BOOL* pBit)
{
	WORD dwRet = 0;
	for(int nIndex = 15; nIndex >= 0; nIndex-- ) {
		WORD w = 0;
		int n = nIndex;

		if(pBit[nIndex] == TRUE ) {//kds20140225
		//if(pBit[15 - nIndex] == TRUE ) {
			w = w | (1 << n);
		}
		
		dwRet = dwRet | w;
	}

	return dwRet;
}

