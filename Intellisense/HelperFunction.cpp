#include "stdafx.h"
#include "HelperFunction.h"

using namespace std;
bool Helper::CheckFileExists(CString cstr)
{
	bool state = true;
    DWORD attrib = GetFileAttributes(cstr);
    if(attrib == INVALID_FILE_ATTRIBUTES)
    {
        state = false;      // not accessible
    }
    else if(attrib & FILE_ATTRIBUTE_DIRECTORY)
    {
        state = false;      // it is a directory
    }
    return state;
}

void Helper::CreateFolder(CString strPath)
{
	int pos, len;
	CString str = _T("");
	if (strPath.Right(1) != _T('\\')) {
		strPath += '\\';
	}
	len = strPath.GetLength();
	pos = strPath.Find('\\');

	do {
		pos = strPath.Find('\\', pos + 1);
		str = strPath.Left(pos);
		::CreateDirectory(str, NULL);

	} while (len != str.GetLength() + 1);
}

void Helper::CheckNCreateFolder(CString str)
{
	CFileFind finder;
	if(!finder.FindFile(str))
		CreateFolder(str);
}

std::vector<string> Helper::TokenizeStr(const std::string data, const char chSep)
{
	std::vector<string> result;
	string token;
	stringstream ss(data);
	while(std::getline(ss, token, chSep)) {
		result.push_back(token);
	}

	return result;
}

string Helper::TCHARToString(const TCHAR* ptsz)
{
	int len = wcslen((wchar_t*)ptsz);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)ptsz, 2 * len + 1);
	std::string s = psz;
	delete[] psz;
	return s;
}

TCHAR* Helper::StringToTCHAR(string& s)
{
	const char* all = s.c_str();
	int len = 1 + strlen(all);
	wchar_t* t = new wchar_t[len];
	if(NULL == t)throw std::bad_alloc();
	mbstowcs(t, all, len);
	return (TCHAR*)t;
}

std::vector<CString> Helper::TokenizeTCHAR(const CString data, const char chSep)
{
	std::vector<CString> result;
	CString szSub;
	int i = 0;
	
	while(FALSE != AfxExtractSubString(szSub, data, i++, chSep))
		result.push_back(szSub);

	return result;
}