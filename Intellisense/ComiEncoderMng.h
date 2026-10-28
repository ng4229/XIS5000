#pragma once
class ComiEncoderMng
{
public:
	ComiEncoderMng();
	~ComiEncoderMng(void);

public:
	BOOL Initialize();
	void Reset();
	int  Read();

	int m_Channel;
	int m_Mode;

	int m_nEncoderValue;
};

