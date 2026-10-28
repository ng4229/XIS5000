#pragma once
#include <string>
#include <stdio.h>
#include <atlstr.h>
#include <vector>
#include <sstream>
#include <fstream>
#include "opencv2/opencv.hpp"

using namespace std;

namespace Helper {
	bool CheckFileExists(CString path);
	void CreateFolder(CString str);
	void CheckNCreateFolder(CString str);

	std::vector<string> TokenizeStr(const std::string data, const char chSep = ',');
	std::string TCHARToString(const TCHAR* ptsz);
	TCHAR* StringToTCHAR(string& s);
	std::vector<CString> TokenizeTCHAR(const CString data, const char chSep = ',');
}