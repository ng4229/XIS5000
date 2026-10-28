#pragma once
#include "dtframebufif.h"
typedef BYTE* PBYTE;

//union DataConverter
//{
//	BYTE Data[4];
//	ULONG Value;
//};

class CDTFrameBuf :
	public CDTFrameBufIF
{
public:
	CDTFrameBuf(void);
	virtual ~CDTFrameBuf(void);
private:
	// The width of the image
	unsigned int m_FrameWidth;
	unsigned int m_FrameLength;
	unsigned int m_BytesPerPixel;
	unsigned int m_SubFrameLength;
	unsigned int m_FrameBufNums;
	unsigned int m_TotalRowsNum;
	unsigned int m_RowSize;
	unsigned int m_GrabFrameNumTotal;
	unsigned int m_CurGrabFrameNum;
	unsigned int m_StreamBytes;
	
//Image Data Bufer; Size = 
	//BYTE**  m_pRowAddr;	//store The Every Row address 
	PBYTE*   m_pRowAddr;
	BYTE*	m_pData;
	//The pointer to frame address be used in event manager
	unsigned int	m_CurSubFrameRowID;//Pointer to current accessing Frame address
	unsigned int	m_CurFrameRowID;//Pointer to Currrent accessing Subframe address 
	unsigned int	m_CurRowID;//Pinter to current line data addt
	unsigned int	m_CurLinePos;//Store the current line size;
	
	CDTEventManagerIF* m_pEventManager;
	CImageObject*	m_pImageObject;
	
public:
	void AddOneSubFrame(BYTE* pSrc,unsigned int Size);//For GigE
	void AddOneFrameLine(BYTE* pSrc,unsigned int Size,BOOL bLineEnd);
	bool Create(CDTEventManagerIF* pEventManager,unsigned int FrameWidth, unsigned int FrameLength, unsigned int BytesPerPixel, unsigned int StreamBytes, unsigned int SubFrameNum, unsigned int FrameBufNum);
	void SetGrabFrameNum(unsigned int Num);//Set want to grab how many frames
	unsigned int  GetFrameWidth();
	unsigned int  GetFrameLength();
	unsigned int  GetBytesPerPixel();
	void SetImageObject(CImageObject* pImageObject);
	void Reset();
	void Start(){};
	void Stop(){};
	
	
};
