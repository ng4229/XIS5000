#pragma once
#include "dtpqueueif.h"
#include "DTConst.h"
#include <stdio.h>
typedef unsigned * PBEGINTHREADEX_THREADID;
typedef unsigned (WINAPI *PBEGINTHREADEX_THREADFUNC) (LPVOID lpThreadParameter);
class CDTPQueueUDP :
	public CDTPQueueIF
{
private:
	BYTE				m_ImageCounter;
	BYTE				m_LostNumber;
	BYTE				m_Header[DTP_HEAD_LEN_UDP];
	BYTE*				m_pBuffer;
	BYTE*				m_pBuffer0;
	BYTE*				m_pBuffer1;
	ULONG				m_BufferSize;
	CDTDataSrcIF*		m_pDataSrc;
	CDTEventManagerIF*	m_pEventManager;
	CDTFrameBufIF*		m_pFrameBUF;

	CDTPPacketIF*		m_pHead;
	CDTPPacketIF*		m_pTail;
	LONG				m_QueueSize;
	FILE*				m_hTraceFile;
	BOOL				m_bDebugMode;
	BOOL                m_bFirstLine;//first line flag for dual energy mode
	BYTE                m_bEnergyFlag;// the high/low energy flag for dual energy mode
	ULONG               m_DualEnergy; // the energy mode flag  normal/dual energy
	ULONG				m_DirectFlag;
	CRITICAL_SECTION	m_TailCR;
    BOOL                m_bIsExFrameTrigger;//RR0083,20140928,Peter
	BOOL				m_bEnableCopy;//Use for startgrabengine funtion
	BOOL				m_bGrabEngine;//Use for startgrabengine funtion. The ENgine function do not copy data to framebuf untill trigger grab call
	int					DataLen;
	enum{DT_IMG = 0x00,DT_CONTROL = 0x01,DT_DUMY = 0x02};

	volatile BOOL   m_bThreadStop;
	HANDLE			m_hThread;
	HANDLE			m_hThreadEndEvent;
	HANDLE			m_hPacketReady;
	DWORD			m_ThreadID;
private:
	void CheckDTP();
	ULONG GetCurDTPLen();
	ULONG GetQueueSize();
	BYTE GetCurType();
	BYTE GetCurContent();	
	void MoveHeader();
	void ClearQueue();
	void OnDataLostEvent(BYTE LostNum);
	void CheckAndAllocateDataBuffer(ULONG BufferSize);
	void ReleaseDataBuffer();
	void CopyDataToBuffer();
	void CopyOneRow(int CopyLen);
	void CopyTwoRow(int CopyLen);
	BOOL FindLineHeader(); 
	void DropCurPacket(CDTPPacketIF* pCurPacket);
	static DWORD WINAPI ThreadFunc(LPVOID param);
	virtual DWORD ThreadMemberFunc();
public:
	CDTPQueueUDP(void);
	virtual ~CDTPQueueUDP(void);
		// Called when a Datasrc packet ready, add the packet to the end of queue and check is there any finnished packet
	void AddTail(CDTPPacketIF* pDTPacket);
	// Set the data source to input data top returnt the used DTPacket
	void SetDataSrc(CDTDataSrcIF* pDataSrc);
	//Set evenmanager to accept DTEvent
	void SetEventManager(CDTEventManagerIF *pEventManager);
	//Set Frame buf to output the Frame data
	void SetFrameBuf(CDTFrameBufIF* pFrameBuf);
	void Start();
	void Stop();
	BOOL IsRunning();
	void ResetImageCounter();
	void ReturnAllPacket();
	void ResetHeader();
	void EngineEnable(BOOL bEnable);
	void EngineCopyEnable(BOOL bEnable);
	BOOL IsEngineCopyEnable();
	BOOL IsEngineEnable();
	LONG GetLostNumber();
	LONG GetLostStartID();
	void SetDualEnergyMode(ULONG DualEnergyMode);
	void EnableDebug(BOOL bEnable);
	void SetDirectionFlag(ULONG Flag);
	void SetExFrameTrigger(BOOL bIsExFrameTrigger);//RR0083,20140928,Peter
};
