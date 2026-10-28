#pragma once
//Maintain the packet list
//Create new packet
//releaes the packet resource
class CDTPPacketIF;

#define USB_PACKET	0
#define MAX_PACKET_NUM	256
class CDTPPacketFactory
{
	CDTPPacketIF* m_PacketArray[MAX_PACKET_NUM];//Every create packet will add one pointer in List
public:
	CDTPPacketFactory(void);
	CDTPPacketIF* GetNewDTPPacketList(BYTE PacketType,DWORD PacketBufSize, WORD PacketNum);//Return a list of packet with the size buffer
	void ReleaseAllPacket(void);
	void ClearAllPacket();
	~CDTPPacketFactory(void);
};
