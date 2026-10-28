#pragma once

class CDTDataSrcIF;
class CDTDetector;
class CDTEventManagerIF;

class CDTDataSrcFactory
{
private:
	CDTDataSrcIF* m_pDataSrc;
public:
	CDTDataSrcFactory(void);
	~CDTDataSrcFactory(void);
	CDTDataSrcIF* GetDataSrc(int CardType,CDTDetector* pDetector=NULL,CDTEventManagerIF* pEventManager=NULL,LPVOID pParam=NULL);
};
