// CModbusSerial.h: interface for the CModbusSerial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CModbusSerial_H__B02ADD74_6AD1_4588_81ED_197F7228F1EA__INCLUDED_)
#define AFX_CModbusSerial_H__B02ADD74_6AD1_4588_81ED_197F7228F1EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CModbusSerial  
{
	// Modbus ASCII 읽기 쓰기용 Serial 포트 관리  Class
	// * 기본적인 Error처리는 추가 해야 합니다.
	// Windows 7 - Visual Studio 2010에서 개발 되었습니다
	// 다른 OS 및 개발 환경에서는 디버깅이 필요할 수 있습니다.
public:
	//기본 생성자
	CModbusSerial();
	//소멸자
	virtual ~CModbusSerial();


////////////////////////////////////////////////////////////////////
// Private properties
////////////////////////////////////////////////////////////////////
private:
	
////////////////////////////////////////////////////////////////////
// Protected properties
////////////////////////////////////////////////////////////////////
protected:
	OVERLAPPED readOverlap;		//수신 오버랩
	OVERLAPPED writeOverlap;	//송신 오버랩
	HANDLE hComm;				//시리얼포트 핸들

////////////////////////////////////////////////////////////////////
// Public properties
////////////////////////////////////////////////////////////////////
public:
	
////////////////////////////////////////////////////////////////////
// Private method
////////////////////////////////////////////////////////////////////
private:

////////////////////////////////////////////////////////////////////
// Protected method
////////////////////////////////////////////////////////////////////
protected:
	// MODBUS ASCII CheckSum 생성
	BYTE MakeLRC(BYTE* LRC, WORD wLength);
	// InputRegister 읽기 Format 생성자( Modbus는 40001번의 wAddress 0으로 설정 됩니다 )
	// DeviceID : 상태 DeviceID
	// wAddress : Modbus 주소
	// wCount	: 읽을 WORD 개수
	// pCommand : Modbus protocol Format retrun 됩니다
	WORD FormatForReadInputRegister(BYTE DeviceID, WORD wAddress, WORD &wCount, BYTE* pCommand);
	// WriteSingleRegister 쓰기 Format ( Modbus Write Single Register쓰기는 하나의 WORD만 쓰기 합니다. 40001번 wAddress 0으로 설정됩니다)
	// DeviceID : 상태 DeviceID
	// wAddress : Modbus 주소
	// wData	: 쓰기 아날로그 데이터값
	// pCommand : Modbus protocol Format retrun 됩니다
	WORD FormatForWriteSingleRegister(BYTE DeviceID, WORD wAddress, WORD &wData, BYTE* pCommand);

	// 수신된 데이터 읽기
	// 통신포트에서 데이터를 읽어 옮니다
	// pData : 읽을 데이터 버퍼
	// wSize : 데이터 버퍼 Size
	WORD ReadComm( BYTE* pData, WORD& wSize );
	// 데이터 송신
	// 통신 포트에 데이터를 씁니다
	// pData : 쓰기 데이터
	// wSize : 쓰기 데이터 Size
	WORD WriteComm( BYTE* pData, WORD& wSize );

////////////////////////////////////////////////////////////////////
// Public method
////////////////////////////////////////////////////////////////////
public:
	// 시리얼포트 열여있는지 확인
	BOOL IsOpen();
	// 시리얼포트 열기 ( Byte Size : 8, Parity : NONPARITY , Stop Bit : ONESTOPBIT  변경은 소스에서 )
	// nPort	: 포트 번호
	// nSpeed   : Baud Rate
	BOOL Open( int nPort, UINT nBaudRate = 38400);

	// 시리얼 포트 닫기
	void Close();

	// Input Register 값을 읽어 옯니다.( Modbus는 40001번의 wAddress 0으로 설정 됩니다 )
	// DeviceID : 상태 DeviceID
	// wAddress : Modbus 주소
	// wCount	: 쓰기 아날로그 데이터값
	// pData	: 아날로그 데이터가 리턴됩니다.( wCount개의 배열 )
	BOOL ReadInputRegister(BYTE DeviceID, WORD wAddress, WORD& wCount, WORD* pData);
	// WriteSingleRegister 쓰기 ( Modbus Write Single Register쓰기는 하나의 WORD만 쓰기 합니다. 40001번 wAddress 0으로 설정됩니다)
	// DeviceID : 상태 DeviceID
	// wAddress : Modbus 주소
	// wData	: 쓰기 아날로그 데이터값
	BOOL WriteSingleRegister(BYTE DeviceID, WORD wAddress, WORD& wData);
	
	// Word 값을 16개의 BOOL 배열로 변환 합니다
	// wData : 변환할 Word
	// pBit  : 변환된 16개의 Bit 배열
	void WordToBit( WORD& wData, BOOL* pBit);

	// 16개의 BOOL 값을 WORD로 변환 합니다
	// pBit : 변환할 16개의 BOOL
	WORD BitToWord( BOOL* pBit);
};

#endif // !defined(AFX_CModbusSerial_H__B02ADD74_6AD1_4588_81ED_197F7228F1EA__INCLUDED_)
