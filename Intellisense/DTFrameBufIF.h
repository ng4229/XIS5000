#pragma once
class CImageObject;
class CDTEventManagerIF;

class CDTFrameBufIF
{
public:
	CDTFrameBufIF(void);
	virtual ~CDTFrameBufIF(void);
	virtual bool Create(CDTEventManagerIF* pEventManager,unsigned int FrameWidth, unsigned int FrameLength, unsigned int BytesPerPixel, unsigned int StreamBytes, unsigned int SubFrameNum, unsigned int FrameBufNum) = 0;
	virtual void AddOneFrameLine(BYTE* pSrc, unsigned int Size,BOOL bLineEnd) = 0;
	virtual void AddOneSubFrame(BYTE* pSrc,unsigned int Size)=0;
	virtual void SetGrabFrameNum(unsigned int Num)=0;//Set want to grab how many frames
	virtual unsigned int GetFrameWidth()=0;
	virtual unsigned int GetFrameLength()=0;
	virtual unsigned int GetBytesPerPixel()=0;
	virtual void SetImageObject(CImageObject* pImageObject)=0;
	virtual void Reset()=0;
	virtual void Start()=0;
	virtual void Stop()=0;
	virtual void SetRegShift(float shift)=0;
	virtual float GetRegShift()=0;

	unsigned int m_DataOffset;//this mean the offset to data, if data exclude lineinfo ,this should be 0
};
