////////////////////////////////////////////////////////////////////////
//
//This is optimized frame buffer. It adds a thread to call
//the frame ready call back. That could buffer more than one frame
//than the old code. It add a vector to record the frame ready event and 
//the additional thread checks the event list to call the callback function.
//In this way, the frame ready callback will not affect the DTQueue grabbing
//thread.
//
//By Zhang Xu, 2013.1.6
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "dtframebufif.h"
#include <deque>
using namespace std;

typedef unsigned * PBEGINTHREADEX_THREADID;
typedef unsigned (WINAPI *PBEGINTHREADEX_THREADFUNC) (LPVOID lpThreadParameter);

typedef BYTE* PBYTE;
//union DataConverter
//{
//	BYTE Data[4];
//	ULONG Value;
//};

class CDTFrameBufOpt :
	public CDTFrameBufIF
{
public:
	CDTFrameBufOpt(void);
	virtual ~CDTFrameBufOpt(void);
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

	 
	PBYTE*   m_pRowAddr;
	BYTE*	m_pData;
	//The pointer to frame address be used in event manager
	unsigned int	m_CurSubFrameRowID;//Pointer to current accessing Frame address
	unsigned int	m_CurFrameRowID;//Pointer to Currrent accessing Subframe address 
	unsigned int	m_CurRowID;//Pinter to current line data addt
	unsigned int	m_CurLinePos;//Store the current line size;
	
	CDTEventManagerIF* m_pEventManager;
	CImageObject*	m_pImageObject;

	//New added event list and thread control objects
	HANDLE m_hThread;
	DWORD m_ThreadID;
	CRITICAL_SECTION m_hMutex;			//The lock of the thread
	HANDLE m_hThreadEndEvent;
	HANDLE m_hFrameReadyEvent;
	volatile BOOL m_bThreadStop;
	deque<ULONG_PTR> m_ImgAddressList;	//The frame ready event list
	float m_RegShift;

public:
	void AddOneSubFrame(BYTE* pSrc,unsigned int Size);//For PC_Dig and I2S Card
	void AddOneFrameLine(BYTE* pSrc,unsigned int Size,BOOL bLineEnd);
	bool Create(CDTEventManagerIF* pEventManager,unsigned int FrameWidth, unsigned int FrameLength, unsigned int BytesPerPixel, unsigned int StreamBytes, unsigned int SubFrameNum, unsigned int FrameBufNum);
	void SetGrabFrameNum(unsigned int Num);//Set want to grab how many frames
	unsigned int  GetFrameWidth();
	unsigned int  GetFrameLength();
	unsigned int  GetBytesPerPixel();
	void SetImageObject(CImageObject* pImageObject);
	void Reset();
	void Start();
	void Stop();
	void SetRegShift(float shift);
	float GetRegShift();
	
private:
	static DWORD WINAPI ThreadFunc(LPVOID param);
	virtual DWORD ThreadMemberFunc();
	BOOL IsRunning();
		
};
