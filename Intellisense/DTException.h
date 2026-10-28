#pragma once

class CDTException
{
private:
	//char m_Buf[100];
	public:
	int m_ERROR_ID;
	char* m_ERROR_INFO;
	CDTException(int ID);
	~CDTException(void);
	void PrintErrInfo();
	void ProcessError();
};
