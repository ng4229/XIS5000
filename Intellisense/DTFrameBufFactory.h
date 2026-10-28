#pragma once
#include "DTConst.h"
class CDTFrameBufIF;
class CDTEventManagerIF;
class CDTFrameBufFactory
{
	CDTFrameBufIF* m_pFrameBuf;
public:
	CDTFrameBufFactory(void);
	~CDTFrameBufFactory(void);
	CDTFrameBufIF* GetDTFrameBuf(CDTEventManagerIF* pEventManager,unsigned int FrameWidth, unsigned int FrameLength, unsigned int BytesPerPixel, unsigned int StreamBytes, unsigned int SubFrameNum=1, unsigned int FrameBufNum=FRAME_BUF_NUM);
};
