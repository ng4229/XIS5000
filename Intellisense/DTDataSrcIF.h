#pragma once
class CDTPQueueIF;
class CDTPPacketIF;

class NetParam
{
public:
	char IP[20];
	WORD Port;
	NetParam(const char* strIP,WORD port)
	{
		strcpy(IP,strIP);
		Port = port;
	}
};

class CDTDataSrcIF
{
public:
	CDTDataSrcIF(void);
	virtual ~CDTDataSrcIF(void);
	virtual BOOL Open(LPVOID pParam)=0;//DO initial work
	virtual BOOL Start()=0;//Start Grab Thread called by DTImage FALSE Failed to start
	virtual BOOL Stop()=0;//StopGrab called by dtimage FALSE TIme Out
	virtual void AddDTPPacket(CDTPPacketIF* pPacket)=0;//Inser the single packet to the wait queue called by DTImage to create a wait queue
	virtual void AddDTPPacketList(CDTPPacketIF* pHead)=0;//Inser the packet list to the wait queue called by DTImage to create a wait queue

	//and DTPQueue to return the used packet to the wait queue
	//Create Need the DTPQueuePointer,to accept the DTP Packet
	virtual void SetDTPQueue(CDTPQueueIF* pQueue){};
//	virtual void SetDTFrameBuf(CDTFrameBufIF* pBufIF){};
	virtual BOOL IsRunning()=0;
	virtual void SetStopFlag(BOOL bTRue)=0;//becalled by thread to stop itself
	virtual BOOL GetStopFlag()=0;
	virtual void ResetQueue()=0;
	virtual void Close(void) = 0;
	virtual void SetTimeOut(LONG timeout)=0;
	virtual void GetTimeOut(LONG* ptimeout)=0;
	virtual void EnableDebug(BOOL bEnable)=0;
	
};
