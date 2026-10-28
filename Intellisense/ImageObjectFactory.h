#pragma once
class CImageObject;
class CImageObjectFactory
{
public:
	CImageObjectFactory(void);
	~CImageObjectFactory(void);
	CImageObject* GetImageObject();
private:
	CImageObject* m_pImageObject;
};
