// EYH_ADO.h: interface for the CEYH_ADO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EYH_ADO_H__8110AA43_627F_4569_B883_AC75E826D6C6__INCLUDED_)
#define AFX_EYH_ADO_H__8110AA43_627F_4569_B883_AC75E826D6C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ATLCONV.H>

class CEYH_ADO  
{
public:
	void MoveFirst();
	long RecordCount();
	void MovePrevious();
	void MoveLast();
	bool Update_DB(LPCTSTR Query);
	bool Insert_DB(LPCTSTR Query);
	bool Delete_DB(LPCTSTR Query);
	void MoveNext();
	bool ChangeRsChar(LPCTSTR inputStr, CString& returnv);
	bool ChangeRsInt(LPCTSTR inputStr, int &returnv);
	bool ChangeRsChar(LPCTSTR inputStr, char *returnv);
	bool ChangeRsLong(LPCTSTR inputStr, long &returnv);
	bool ChangeRsTime(LPCTSTR inputStr, CTime &returnv);
	bool ChangeRsBool(LPCTSTR inputStr, bool &returnv);
	bool ChangeRsFloat(LPCTSTR inputStr, float &returnv);

	bool IsEOF();
	bool Select_DB(LPCTSTR Query);
	void Close_DB();
	bool b_IsOpenedDB;
	bool Open_DB(CString path);
	CEYH_ADO();
	virtual ~CEYH_ADO();

	_ConnectionPtr   m_pConnect; // 연결 포인터
	_RecordsetPtr    m_pRecord;  // 레코드셋 포인터
	_CommandPtr      m_pCommand; // 명령 포인터
};

#endif // !defined(AFX_EYH_ADO_H__8110AA43_627F_4569_B883_AC75E826D6C6__INCLUDED_)
