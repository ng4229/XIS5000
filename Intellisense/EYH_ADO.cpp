// EYH_ADO.cpp: implementation of the CEYH_ADO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EYH_ADO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEYH_ADO::CEYH_ADO()
{
	m_pConnect=NULL;
	m_pRecord=NULL;
	m_pCommand=NULL;

	::CoInitialize(NULL);
//	m_pRecord.CreateInstance(__uuidof(Recordset));
//	m_pCommand.CreateInstance(__uuidof(Command));
	b_IsOpenedDB=false;
}


CEYH_ADO::~CEYH_ADO()
{
	::CoUninitialize();
}

bool CEYH_ADO::Open_DB(CString path)
{

	CString strDBPath;
	//strDBPath = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + path;
	strDBPath = _T("Provider=Microsoft.ACE.OLEDB.16.0;Data Source=") + path;
    _bstr_t strConnect(strDBPath);

	try
	{	
		m_pConnect.CreateInstance(__uuidof(Connection));
    	m_pConnect->Open(strConnect, "", "", adConnectUnspecified);
	}
	catch(...)
	{
		b_IsOpenedDB = false;
		return false;
	}
	b_IsOpenedDB = true;
	return true;
}

void CEYH_ADO::Close_DB()
{
	if(m_pRecord!=NULL && m_pRecord->GetState()!=adStateClosed)
	{
		m_pRecord->Close();
		m_pRecord.Release();
		m_pRecord = NULL;
	}			
/*	if(m_pConnect!=NULL && m_pConnect->GetState()!=adStateClosed)
	{
		m_pConnect->Close();
		m_pConnect.Release();
		m_pConnect = NULL;
	}*/

}

bool CEYH_ADO::Select_DB(LPCTSTR Query)
{
	CursorTypeEnum emCurType = adOpenStatic;
	LockTypeEnum emLockType = adLockReadOnly; 
	long lop = adCmdUnknown;
	
	if(m_pConnect==NULL || m_pConnect->GetState()!=adStateOpen)
		return false;


	if(m_pRecord!=NULL && m_pRecord->GetState()!=adStateClosed)
	{
		m_pRecord->Close();
		m_pRecord.Release();
		m_pRecord=NULL;
	}

	m_pRecord.CreateInstance(__uuidof(Recordset));
	m_pRecord->CursorLocation = adUseClient;

	try
	{
		_bstr_t btQry=(_bstr_t)Query;
        m_pRecord->Open(btQry, _variant_t((IDispatch *) m_pConnect), emCurType, emLockType, lop);
		
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CEYH_ADO::IsEOF()
{
	if(m_pRecord==NULL)
	{
		return true;
	}
	return m_pRecord->adoEOF;
}

bool CEYH_ADO::Delete_DB(LPCTSTR Query)
{	
	if(m_pConnect==NULL || m_pConnect->GetState()!=adStateOpen)
		return false;

	try
	{
		m_pCommand.CreateInstance(__uuidof(Command)); 
		m_pCommand->ActiveConnection = m_pConnect;
		m_pCommand->CommandType = adCmdStoredProc; 
		m_pCommand->CommandText = (_bstr_t)Query;
        m_pCommand->Execute(NULL, NULL, adCmdText);
		
	}
	catch(...)
	{
		return false;
	}

	return true;


}

bool CEYH_ADO::Insert_DB(LPCTSTR Query)
{
	if(m_pConnect==NULL || m_pConnect->GetState()!=adStateOpen)
		return false;

	try
	{
		m_pCommand.CreateInstance(__uuidof(Command)); 
		m_pCommand->ActiveConnection = m_pConnect;
		m_pCommand->CommandType = adCmdStoredProc; 
		m_pCommand->CommandText = (_bstr_t)Query;
        m_pCommand->Execute(NULL, NULL, adCmdText);
	}
	catch(...)
	{
		return false;
	}

	return true;

}

bool CEYH_ADO::Update_DB(LPCTSTR Query)
{
	if(m_pConnect==NULL || m_pConnect->GetState()!=adStateOpen)
		return false;

	try
	{
		m_pCommand.CreateInstance(__uuidof(Command)); 
		m_pCommand->ActiveConnection = m_pConnect;
		m_pCommand->CommandType = adCmdStoredProc; 
		m_pCommand->CommandText = (_bstr_t)Query;
        m_pCommand->Execute(NULL, NULL, adCmdText);
		
	}
	catch(...)
	{
		return false;
	}

	return true;
}

void CEYH_ADO::MoveLast()
{
	if(m_pRecord!=NULL)
	{
		m_pRecord->MoveLast();
	}
}

void CEYH_ADO::MoveNext()
{
	if(m_pRecord!=NULL)
	{
		m_pRecord->MoveNext();
	}
}

void CEYH_ADO::MoveFirst()
{
	if(m_pRecord!=NULL)
	{
		m_pRecord->MoveFirst();
	}
}


void CEYH_ADO::MovePrevious()
{
	if(m_pRecord!=NULL)
	{
		m_pRecord->MovePrevious();
	}
}

long CEYH_ADO::RecordCount()
{
	return m_pRecord->RecordCount;
}


bool CEYH_ADO::ChangeRsChar(LPCTSTR inputStr, CString &returnv)
{
	_variant_t var;
	try
	{
		if(m_pRecord!=NULL)
		{

			var = m_pRecord->GetFields()->GetItem(inputStr)->GetValue();
			if(var.vt==VT_NULL)
				returnv="";
			else
				returnv=var.bstrVal;

		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CEYH_ADO::ChangeRsInt(LPCTSTR inputStr, int &returnv)
{
	_variant_t var;
	try
	{
		if(m_pRecord!=NULL)
		{

			var = m_pRecord->GetFields()->GetItem(inputStr)->GetValue();
			if(var.vt==VT_NULL)
				returnv=0;
			else
				returnv=var.intVal;
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}


bool CEYH_ADO::ChangeRsChar(LPCTSTR inputStr, char *returnv)
{
	_variant_t var;
	USES_CONVERSION;
	try
	{
		if(m_pRecord!=NULL)
		{

			var = m_pRecord->GetFields()->GetItem(inputStr)->GetValue();
			if(var.vt==VT_NULL)
				returnv[0]=1;
			else
			{
				int lenght = ::WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, NULL, 0, NULL, NULL);
				::WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, returnv, lenght, NULL, NULL);
			}
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CEYH_ADO::ChangeRsLong(LPCTSTR inputStr, long &returnv)
{
	_variant_t var;
	try
	{
		if(m_pRecord!=NULL)
		{

			var = m_pRecord->GetFields()->GetItem(inputStr)->GetValue();
			if(var.vt==VT_NULL)
				returnv=0L;
			else
				returnv=var.intVal;
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CEYH_ADO::ChangeRsTime(LPCTSTR inputStr, CTime &returnv)
{
	_variant_t var;
	SYSTEMTIME	SystemTime ;
	DATE		date;

	try
	{
		if(m_pRecord!=NULL)
		{

			var = m_pRecord->GetFields()->GetItem(inputStr)->GetValue();
			if(var.vt==VT_NULL)
			{
				returnv = 0;
			}
			else
			{
				date = var.date;				
				if(VariantTimeToSystemTime(date, &SystemTime)==1)
				{
					if ((1959>SystemTime.wYear) | (3000<SystemTime.wYear))
					{
						returnv = 0;
					}
					else
					{
						CTime cDTime(SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
							SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond );

						returnv = (CTime)cDTime;
					}
				}
			}

		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CEYH_ADO::ChangeRsBool(LPCTSTR inputStr, bool &returnv)
{
	_variant_t var;
	try
	{
		if(m_pRecord!=NULL)
		{

			var = m_pRecord->GetFields()->GetItem(inputStr)->GetValue();
			if(var.vt==VT_NULL)
				returnv=false;
			else if(var.boolVal==-1)
				returnv=true;
			else
				returnv=false;
		}	
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CEYH_ADO::ChangeRsFloat(LPCTSTR inputStr, float &returnv)
{
	_variant_t var;
	try
	{
		if(m_pRecord!=NULL)
		{

			var = m_pRecord->GetFields()->GetItem(inputStr)->GetValue();
			if(var.vt==VT_NULL)
				returnv=0.0;
			else
				returnv=var.fltVal;
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

