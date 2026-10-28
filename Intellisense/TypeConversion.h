#pragma once
#include "stdafx.h"

using namespace std;

#define cstos(data) string(CT2CA(data))					//CString	->	string
#define stocs(data) CString(data.c_str())				//string		->	CString
#define cstoi(data) _ttoi(data)						//CString	->	int
#define cstof(data) _ttof(data)						//CString	->	float   
#define itocs(data) CString(to_string(data).c_str())		//int		->	CString
#define itos(data) to_string(data)						//int		->	string
#define tchartocs(data) (LPCTSTR)data					//TCHAR		->	CString
#define tchartos(data) string(CT2CA((LPCTSTR)data))		//TCHAR		->	string