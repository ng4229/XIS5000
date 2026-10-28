#include "stdafx.h"
#include "Comidas.h"
#include "ComiEncoderMng.h"

HANDLE hDevice;
char cDeviceName[30];	//보드명
USHORT usDeviceID;	//Device load를 하기 위한 보드ID값 저장
ComiEncoderMng::ComiEncoderMng()
{
	m_Mode = 0;
	m_Channel = 0;
	m_nEncoderValue = 0;
}


ComiEncoderMng::~ComiEncoderMng(void)
{
}

BOOL ComiEncoderMng::Initialize()
{
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	TComiDevList pDevList;

	COMI_LoadDll();	//라이브러리 로드
	COMI_GetAvailDevList(&pDevList);	//현재 연결되어 있는 보드 정보 가져오기	

	strcpy(cDeviceName, pDevList.DevInfo->szDevName);

	//DeviceLoad를 보드에 맞는 값으로 해주기 위한 조건문

	if (strcmp(cDeviceName, "COMI-SD501") == 0)
	{
		usDeviceID = COMI_SD501;
	}
	else
	{
		AfxMessageBox(_T("지원하지 않는 보드 이거나 보드를 찾지 못하였습니다."));

		return FALSE;
	}

	hDevice = COMI_LoadDevice(usDeviceID, 0);	//Device Load

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("지원하지 않는 보드 이거나 보드를 찾지 못하였습니다."));
		
		return FALSE;
	}

	COMI_ENC_Config(hDevice, m_Channel, m_Mode, 0);

	return TRUE;
}

void ComiEncoderMng::Reset()
{
	//m_Channel의 엔코더 A/B상 카운트 값 Reset 후
	COMI_ENC_Reset(hDevice, m_Channel);
	//m_Channel의 A/B상 카운트 값 Read
	m_nEncoderValue = COMI_ENC_Read(hDevice, m_Channel);

	return;
}

int ComiEncoderMng::Read()
{
	//CString strValue = _T("");	//도스창 트래킹
	m_nEncoderValue = COMI_ENC_Read(hDevice, m_Channel);
	//strValue.Format(_T("%d \n"),m_nEncoderValue);
	//printf(strValue);

	return m_nEncoderValue;
}