#pragma once
#include <stdio.h>
class CDTPPacketIF
{
protected:
	int	m_PacketID;
public:

	CDTPPacketIF()
	{
		m_PacketID = -1;
	};
	virtual BYTE* GetBuf() = 0;//called by datasrc to copy data from driver
	virtual DWORD GetFirstIndex() = 0;
	virtual void  MoveFirstToWithOffset(DWORD pos) = 0;//Called by dtpqueue copy to Frame buffer
	virtual void  Reset() = 0;
	virtual DWORD GetDTPLen() = 0;//offset is start from First. Becalled to get the Len
	virtual void  SetNext(CDTPPacketIF* pNext)=0;
	virtual CDTPPacketIF* GetNext()=0;
	virtual DWORD  GetBufSize()=0;
	virtual ~CDTPPacketIF(void){};
	virtual void SetDataSize(DWORD len)=0;//be called by the data input function to set the avalible length
	virtual void Clear()=0;
	virtual void Trace(FILE* pFile)=0;
	virtual void SetID(int ID)
	{
		m_PacketID = ID;
	}
	

};
class CDTPUSBPacket : public CDTPPacketIF
{

	BYTE* m_pBuf;
	DWORD m_FirstIndex;
	DWORD m_DataLen;
	DWORD m_BufSize;
	CDTPPacketIF* m_pNext;
public:
	PUCHAR m_pContext;//get the return value from the begin transfer and be used in endtransfer
	OVERLAPPED m_InOvLap;//the event to wait the end of transfer;

public:
	CDTPUSBPacket(DWORD BufSize);
	BYTE* GetBuf();//called by datasrc to copy data from driver
	DWORD GetFirstIndex();
	void  MoveFirstToWithOffset(DWORD pos);//Called by dtpqueue copy to Frame buffer
	void  Reset();
	DWORD GetDTPLen();//offset is start from First. Becalled to get the Len
	DWORD  GetBufSize();
	void  SetNext(CDTPPacketIF* pNext);
	void SetDataSize(DWORD len);	//be called by the data input function to set the avalible length
	void Clear();
	void Trace(FILE* pFile);
	CDTPPacketIF* GetNext();
	~CDTPUSBPacket(void);
};
