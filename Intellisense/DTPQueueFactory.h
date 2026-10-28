#pragma once

class CDTPQueueIF;
class CDTPQueueFactory
{
private:
	CDTPQueueIF* m_pDTPQueue;
public:
	CDTPQueueFactory(void);
	~CDTPQueueFactory(void);
	CDTPQueueIF* GetDTPQueue(int netType);

};
