// DlgPage4.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgPage4.h"
#include "afxdialogex.h"

#include <iostream>
#include <string>
#include <fstream>


// CDlgPage4 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgPage4, CDialogEx)

CDlgPage4::CDlgPage4(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPage4::IDD, pParent)
{

	m_Combo_Index = 0;
	m_Date4 = COleDateTime::GetCurrentTime();
	m_SearchDate = "";
}

CDlgPage4::~CDlgPage4()
{
}

void CDlgPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_E, m_List_E);
	DDX_Control(pDX, IDC_PAGE4_COMBO, m_Combo_Drive);
	DDX_CBIndex(pDX, IDC_PAGE4_COMBO, m_Combo_Index);
	DDX_DateTimeCtrl(pDX, IDC_RDATETIMEPICKER4, m_Date4);
	//	DDX_Control(pDX, IDC_MSCHART4, m_Chart4);
	DDX_Control(pDX, IDC_CHART4, m_ChartViewer4);
}


BEGIN_MESSAGE_MAP(CDlgPage4, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_RDATETIMEPICKER4, &CDlgPage4::OnDatetimechangeRdatetimepicker4)
	ON_CBN_SELCHANGE(IDC_PAGE4_COMBO, &CDlgPage4::OnSelchangePage4Combo)
END_MESSAGE_MAP()


// CDlgPage4 메시지 처리기입니다.


BOOL CDlgPage4::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LoadBackground();
	((CDateTimeCtrl*)GetDlgItem(IDC_RDATETIMEPICKER4))->SetFormat(_T("         yyyy.   MM"));
	// TODO: Add extra initialization here
	InitList();
	GetComputerInfo();
	GetDriveList();
	m_SearchDate = m_Date4.Format(_T("%Y-%m"));
	WriteReport();
	Refresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDlgPage4::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		return false;	
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CDlgPage4::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	//CBrush Brush, * pOldBrush;
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		//Brush.CreateStockObject(NULL_BRUSH);
		//pOldBrush = (CBrush*)pDC->SelectObject(&Brush);
		pDC->SetBkColor(RGB(255, 0, 0));
		//pDC->SetBkMode(TRANSPARENT);
		//return (HBRUSH)Brush;
	}

	UINT nID = pWnd->GetDlgCtrlID();

	if (nID == IDC_STATIC)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
		hbr = ::CreateSolidBrush(RGB(255, 255, 255));
	}
	return hbr;
}


void CDlgPage4::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpMainBG);
	dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &cdc, 0, 0, SRCCOPY);
	cdc.DeleteDC(); 
}


void CDlgPage4::OnDatetimechangeRdatetimepicker4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	UpdateData(true);
	if(m_SearchDate!=m_Date4.Format(_T("%Y-%m")))
	{
		m_SearchDate = m_Date4.Format(_T("%Y-%m"));
		*pResult = 0;
		WriteReport();	
		Refresh();
	}
	*pResult = 0;
}


void CDlgPage4::OnSelchangePage4Combo()
{
	UpdateData(true);
	GetDiskSpace();	
}


void CDlgPage4::LoadBackground()
{
	BITMAP bmpInfo; // 비트맵 정보
	if (m_RNationalCode == 0)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_4);
	else if(m_RNationalCode==1)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_4_ENG);
	else if(m_RNationalCode==2)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_4_CHI);
	else if (m_RNationalCode == 3)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_4_RUS);
	else if (m_RNationalCode == 4)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_4_UKR);
	else if (m_RNationalCode == 5)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_4_SPN);
	else if (m_RNationalCode == 6)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_4_POL);
	else if (m_RNationalCode == 7)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_4_TIW);
	
    m_bmpMainBG.GetBitmap(&bmpInfo); // Main 배경 비트맵 정보 읽기
    m_nMainWidth = bmpInfo.bmWidth; // Main 윈도우 폭
    m_nMainHeight = bmpInfo.bmHeight; // Main 윈도우 높이
    SetWindowPos(NULL, 0, 0, m_nMainWidth, m_nMainHeight, SWP_NOZORDER | SWP_NOMOVE); 
}

void CDlgPage4::InitList()
{
/*	CRect rect;
	m_List_E.GetClientRect(&rect);
	m_List_E.InsertColumn(0, _T(""), LVCFMT_CENTER, 0);
	m_List_E.InsertColumn(1, _T("발생시간"), LVCFMT_CENTER, 173);
	m_List_E.InsertColumn(2, _T("발생내용"), LVCFMT_CENTER, rect.Width()-190);
	m_List_E.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
*/
	CRect rect;
	m_List_E.GetClientRect(&rect);
	m_List_E.InsertColumn(0, _T(""), LVCFMT_CENTER, 0);
	m_List_E.InsertColumn(1, _T(""), LVCFMT_CENTER, 173);
	m_List_E.InsertColumn(2, _T(""), LVCFMT_CENTER, rect.Width()-190);
	m_List_E.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
 }

void CDlgPage4::GetComputerInfo()
{
	CString strBuf;

	//운영체제
	//DWORD dwVersion = ::GetVersion();
//	OSVERSIONINFO osvi;
/*	osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	::GetVersionEx(&osvi);
	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		strBuf.Format(_T("Windows 9x %d.%d (Build %d)"), LOBYTE(LOWORD(dwVersion)),HIBYTE(LOWORD(dwVersion)), LOWORD(osvi.dwBuildNumber));
	else if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)  //20170828 윈도우 버전 수정
	{
		if(osvi.dwMajorVersion == 4)
			strBuf.Format(_T("Windows NT %d.%d (Build %d)"), LOBYTE(LOWORD(dwVersion)),HIBYTE(LOWORD(dwVersion)), LOWORD(osvi.dwBuildNumber));
		else if(osvi.dwMajorVersion == 5 && osvi.dwMajorVersion == 0)
			strBuf.Format(_T("Windows 2000 %d.%d (Build %d)"), LOBYTE(LOWORD(dwVersion)),HIBYTE(LOWORD(dwVersion)), LOWORD(osvi.dwBuildNumber));
		else if(osvi.dwMajorVersion == 5 && osvi.dwMajorVersion == 1)
			strBuf.Format(_T("Windows xp %d.%d (Build %d)"), LOBYTE(LOWORD(dwVersion)),HIBYTE(LOWORD(dwVersion)), LOWORD(osvi.dwBuildNumber));
		else if(osvi.dwMajorVersion == 5 && osvi.dwMajorVersion == 2)
			strBuf.Format(_T("Windows 2003 %d.%d (Build %d)"), LOBYTE(LOWORD(dwVersion)),HIBYTE(LOWORD(dwVersion)), LOWORD(osvi.dwBuildNumber));
		else if(osvi.dwMajorVersion == 6)
			strBuf.Format(_T("Windows 7 %d.%d (Build %d)"), LOBYTE(LOWORD(dwVersion)),HIBYTE(LOWORD(dwVersion)), LOWORD(osvi.dwBuildNumber));
		else if(osvi.dwMajorVersion == 10)
			strBuf.Format(_T("Windows 10 %d.%d (Build %d)"), LOBYTE(LOWORD(dwVersion)),HIBYTE(LOWORD(dwVersion)), LOWORD(osvi.dwBuildNumber));
	}
	*/
	OSVERSIONINFOEX osVersionInfoEx;
	ZeroMemory(&osVersionInfoEx, sizeof(OSVERSIONINFOEX));
	osVersionInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((LPOSVERSIONINFOW)&osVersionInfoEx);
	switch (osVersionInfoEx.dwMajorVersion)
	{
	case 5:
		{
			switch (osVersionInfoEx.dwMinorVersion)
			{
			case 1:
				GetDlgItem(IDC_PAGE4_1)->SetWindowTextW(_T("Windows XP"));
				break;
			case 2:
				if (osVersionInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					GetDlgItem(IDC_PAGE4_1)->SetWindowTextW(_T("Windows XP Professional x64 Edition"));
					break;
				}
				else if (GetSystemMetrics(SM_SERVERR2) == 0)
				{
					GetDlgItem(IDC_PAGE4_1)->SetWindowTextW(_T("Windows Server 2003"));
					break;
				}
				else if (osVersionInfoEx.wSuiteMask & VER_SUITE_WH_SERVER)
				{
					GetDlgItem(IDC_PAGE4_1)->SetWindowTextW(_T("Windows Home Server"));
					break;
				}
				else if (GetSystemMetrics(SM_SERVERR2) != 0)
				{
					GetDlgItem(IDC_PAGE4_1)->SetWindowTextW(_T("Windows Server 2003 R2"));
					break;
				}
				break;
			default:
				CString err;
				err.Format(_T("Can't find version name of dwMiniorVersion: %d"), osVersionInfoEx.dwMinorVersion);
				AfxMessageBox(err);
				break;
			}
			break;
		}
	case 6:
		{
			switch (osVersionInfoEx.dwMinorVersion)
			{
			case 0:
				if (osVersionInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					GetDlgItem(IDC_PAGE4_1)->SetWindowTextW(_T("Windows Vista"));
					break;
				}
				else if (osVersionInfoEx.wProductType != VER_NT_WORKSTATION)
				{
					GetDlgItem(IDC_PAGE4_1)->SetWindowTextW(_T("Windows Server 2008"));
					break;
				}
				break;
			case 1:
				if (osVersionInfoEx.wProductType != VER_NT_WORKSTATION)
				{
					GetDlgItem(IDC_PAGE4_1)->SetWindowTextW(_T("Windows Server 2008 R2"));
					break;
				}
				else if (osVersionInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					GetDlgItem(IDC_PAGE4_1)->SetWindowTextW(_T("Windows 7"));
					break;
				}
				break;
			case 2:
				if (osVersionInfoEx.wProductType != VER_NT_WORKSTATION)
				{
					GetDlgItem(IDC_PAGE4_1)->SetWindowTextW(_T("Windows Server 2012"));
					break;
				}
				else if (osVersionInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					GetDlgItem(IDC_PAGE4_1)->SetWindowTextW(_T("Windows 10"));
					break;
				}
				break;
			case 3:
				if (osVersionInfoEx.wProductType != VER_NT_WORKSTATION)
				{
					GetDlgItem(IDC_PAGE4_1)->SetWindowTextW(_T("Windows Server 2012 R2"));
					break;
				}
				else if (osVersionInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					GetDlgItem(IDC_PAGE4_1)->SetWindowTextW(_T("Windows 8.1"));
					break;
				}
				break;
			default:
				CString err;
				err.Format(_T("Can't find version name of dwMiniorVersion: %d"), osVersionInfoEx.dwMinorVersion);
				break;
			}
			break;
		}
	default:
		break;
	}

//	SetDlgItemText(IDC_PAGE4_1, strBuf);

	//메모리
	/*MEMORYSTATUS ms;
	ms.dwLength=sizeof(MEMORYSTATUS);
	::GlobalMemoryStatus(&ms);
	DWORD a;
	a=ms.dwTotalPhys;	*/
	/*if(a/1000000 > 1000)		
		strBuf.Format(_T("%d GB RAM"), a/1000000000);
	else
		strBuf.Format(_T("%d MB RAM"), a/10000000);*/

	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalPhysMem = memInfo.ullTotalPhys;

	if (totalPhysMem / 1000000 > 1000)
		strBuf.Format(_T("%d GB RAM"), totalPhysMem / 1024 / 1024 / 1000);
	else
		strBuf.Format(_T("%d MB RAM"), totalPhysMem / 1048576);

	SetDlgItemText(IDC_PAGE4_2, strBuf);

	//해상도
	HDC hDC=::GetDC((HWND)NULL); 
	int nWidth = ::GetDeviceCaps(hDC,HORZRES);
	int nHeight = ::GetDeviceCaps(hDC,VERTRES);
	strBuf.Format(_T("%d x %d Pixel"), nWidth, nHeight);
	SetDlgItemText(IDC_PAGE4_4, strBuf);
}

void CDlgPage4::GetDriveList()
{
	CString strBuf, strDrive, strType;
	int realDrive = 0;

	for (int nDrive = 0 ; nDrive < 26 ; nDrive++)
	{
		if (::GetLogicalDrives() & (1 << nDrive))
		{
			UINT nType;
			strDrive.Format(_T("%c:\\"), nDrive + 'A');
			nType = ::GetDriveType(strDrive);
			if(nType == DRIVE_REMOVABLE)
				strType=strDrive+_T("(Floppy)");
			else if(nType == DRIVE_FIXED)
				strType=strDrive+_T("(Hard)");
			else if(nType == DRIVE_REMOTE)
				strType=strDrive+_T("(Network)");
			else if(nType == DRIVE_CDROM)
				strType=strDrive+_T("(CD-ROM)");
			else if(nType == DRIVE_RAMDISK)
				strType=strDrive+_T("(RAM)");
			else
				strType=strDrive;
			
			if(nType != DRIVE_REMOVABLE)
			{
				DriverArray[realDrive].Format(_T("%s"), strType);
				strBuf.Format(_T("%s"),strDrive.Left(3));
				realDrive++;
			}
		}
	}

	for(int i=0; i<realDrive; i++)
		m_Combo_Drive.AddString(DriverArray[i]);
	m_Combo_Drive.SetCurSel(0);	
	GetDiskSpace();
}

void CDlgPage4::GetDiskSpace()
{
	if(m_Combo_Index<0)
		return;

	CString strBuf, strBuf_1;
	strBuf.Format(_T("%s"),DriverArray[m_Combo_Index].Left(3));
	ULARGE_INTEGER Fbatc;
	ULARGE_INTEGER lnTotBytes;
	ULARGE_INTEGER lnAvailable;
	double FreeBytes = 0, TotalBytes = 0, fUsed = 0 ;
	if (::GetDiskFreeSpaceEx(LPCTSTR(strBuf), &Fbatc, &lnTotBytes,&lnAvailable))
	{
		TotalBytes =lnTotBytes.LowPart+(double)lnTotBytes.HighPart*(double)4294967296;
		strBuf.Format(_T("%4.1fGB"), TotalBytes/1024/1024/1024 );
		FreeBytes = lnAvailable.LowPart+(double)lnAvailable.HighPart*(double)4294967296;
		if (m_RNationalCode == 0)
			strBuf_1.Format(_T("%4.1fGB/%s(여유/전체)"), FreeBytes / 1024 / 1024 / 1024, strBuf);
		else if(m_RNationalCode==1)
			strBuf_1.Format(_T("%4.1fGB/%s(free/total)"), FreeBytes/1024/1024/1024, strBuf);
		else if(m_RNationalCode==2)
			strBuf_1.Format(_T("%4.1fGB/%s(free/total)"), FreeBytes/1024/1024/1024, strBuf);
		else if (m_RNationalCode == 3)
			strBuf_1.Format(_T("%4.1fGB/%s(бесплатно/общее)"), FreeBytes / 1024 / 1024 / 1024, strBuf);
		else if (m_RNationalCode == 4)
			strBuf_1.Format(_T("%4.1fGB/%s(безкоштовно/усього)"), FreeBytes / 1024 / 1024 / 1024, strBuf);
		else if (m_RNationalCode == 5)
			strBuf_1.Format(_T("%4.1fGB/%s(libre/total)"), FreeBytes / 1024 / 1024 / 1024, strBuf);
		else if (m_RNationalCode == 6)
			strBuf_1.Format(_T("%4.1fGB/%s(wolny/całkowity)"), FreeBytes / 1024 / 1024 / 1024, strBuf);
		else if (m_RNationalCode == 7)
			strBuf_1.Format(_T("%4.1fGB/%s(剩餘容量/全部的)"), FreeBytes / 1024 / 1024 / 1024, strBuf);
		SetDlgItemText(IDC_PAGE4_3, strBuf_1);
	}
	else
	{
		if (m_RNationalCode == 0)
			SetDlgItemText(IDC_PAGE4_3, _T("0.0GB/0.0GB(여유/전체)"));
		if(m_RNationalCode==1)
			SetDlgItemText(IDC_PAGE4_3, _T("0.0GB/0.0GB(free/total)"));
		else if(m_RNationalCode==2)
			SetDlgItemText(IDC_PAGE4_3, _T("0.0GB/0.0GB(free/total)"));
		else if (m_RNationalCode == 3)
			SetDlgItemText(IDC_PAGE4_3, _T("0.0GB/0.0GB(бесплатно/общее)"));
		else if (m_RNationalCode == 4)
			SetDlgItemText(IDC_PAGE4_3, _T("0.0GB/0.0GB(безкоштовно/усього)"));
		else if (m_RNationalCode == 5)
			SetDlgItemText(IDC_PAGE4_3, _T("0.0GB/0.0GB(libre/total)"));
		else if (m_RNationalCode == 6)
			SetDlgItemText(IDC_PAGE4_3, _T("0.0GB/0.0GB(wolny/całkowity)"));
		else if (m_RNationalCode == 7)
			SetDlgItemText(IDC_PAGE4_3, _T("0.0GB/0.0GB(剩餘容量/全部的)"));
	}
}

void CDlgPage4::WriteReport()
{
	CFileFind   finder;
	CString m_strFilter;
	int e=0;

	for(int i=0; i<31; i++)
	{
		ErrorCount[i]=0;
		ChangeCount[i]=0;
	}
	int index = 0;
	if(!g_Connecting)		
	{
		//AfxMessageBox(_T("실패"));
	}
	else
	{
		
		MYSQL_RES* m_Res;
		MYSQL_ROW row;
		mysql_query(&m_mysql,"set names euckr");
		SqlQuery.Format(_T("SELECT * FROM intellisense_db7 WHERE strDate LIKE '%s-%%%%' ORDER BY strErrorTime DESC"), m_SearchDate);
		
		if(mysql_query(&m_mysql,CStringA(SqlQuery)))
		{
			CString error(mysql_error(&m_mysql));
			WriteErrorLog(11, error);
		}
		else
		{
			m_Res=mysql_store_result(&m_mysql);
			
			CString varValue;
			varValue = "";
			m_List_E.DeleteAllItems();
			while((row = mysql_fetch_row(m_Res)) != NULL)
			{

				CString strDate (row[0]);
				CString strErrorTime (row[1]);
				CString strErrorText (row[2]);
				CString strErrorCode (row[3]);

				
				m_List_E.InsertItem(index, _T("")); 
				m_List_E.SetItemText(index, 1, strErrorTime);	
				CString date= strErrorTime.Left(10).Right(2);
				m_List_E.SetItemText(index, 2, strErrorText);
				
				if(m_RNationalCode==0)
				{
					if (strErrorText.Left(4) == "제품")
						ErrorCount[_wtoi(date) - 1]++;
					else
						ChangeCount[_wtoi(date) - 1]++;
				}
				if(m_RNationalCode==1)
				{
					if(strErrorText.Find(_T("product"))!=-1 || strErrorText.Find(_T("Product"))!=-1)
						ErrorCount[_wtoi(date)-1]++;
					else
						ChangeCount[_wtoi(date)-1]++;
				}
				else if(m_RNationalCode==2)
				{
					if(strErrorText.Find(_T("产品"))!=-1)
						ErrorCount[_wtoi(date)-1]++;
					else
						ChangeCount[_wtoi(date)-1]++;
				}
				else if (m_RNationalCode == 3)
				{
					if (varValue.Find(_T("продукт")) != -1)
						ErrorCount[_wtoi(date) - 1]++;
					else
						ChangeCount[_wtoi(date) - 1]++;
				}
				else if (m_RNationalCode == 4)
				{
					if (varValue.Left(4) == "продукту")
						ErrorCount[_wtoi(date) - 1]++;
					else
						ChangeCount[_wtoi(date) - 1]++;
				}
				else if (m_RNationalCode == 5)
				{
					if (varValue.Find(_T("producto")) != -1 || varValue.Find(_T("Producto")) != -1)
						ErrorCount[_wtoi(date) - 1]++;
					else
						ChangeCount[_wtoi(date) - 1]++;
				}
				else if (m_RNationalCode == 6)
				{
					if (varValue.Find(_T("produkt")) != -1 || varValue.Find(_T("produkt")) != -1)
						ErrorCount[_wtoi(date) - 1]++;
					else
						ChangeCount[_wtoi(date) - 1]++;
				}
				else if (m_RNationalCode == 7)
				{
					if (varValue.Find(_T("產品")) != -1 || varValue.Find(_T("產品")) != -1)
						ErrorCount[_wtoi(date) - 1]++;
					else
						ChangeCount[_wtoi(date) - 1]++;
				}
				
			
				index++;
			}
			mysql_free_result(m_Res);
		}
	}

	/*
    if(finder.FindFile(g_StrDirPath[7]))
	{
		m_List_E.DeleteAllItems();
		if(!m_pSet_7.Open_DB(g_StrDirPath[7]))		
		{
			
		}
		else
		{
			CString strSQL;
			strSQL.Format(_T("SELECT * FROM Computer_Log WHERE strDate LIKE '%s-%%%%' ORDER BY strErrorTime DESC"), m_SearchDate);
			if(m_pSet_7.Select_DB(strSQL))
			{
				CString varValue;
				varValue = "";
				e=0;
				while(!m_pSet_7.IsEOF())
				{
					m_List_E.InsertItem(e, _T("")); 
					m_pSet_7.ChangeRsChar(_T("strErrorTime"), varValue);
					CString date=varValue.Left(10).Right(2);
					m_List_E.SetItemText(e, 1, varValue);
					m_pSet_7.ChangeRsChar(_T("strErrorText"), varValue);
					m_List_E.SetItemText(e, 2, varValue);	
					if(m_RNationalCode==1)
					{
						if(varValue.Find(_T("product"))!=-1 || varValue.Find(_T("Product"))!=-1)
							ErrorCount[_wtoi(date)-1]++;
						else
							ChangeCount[_wtoi(date)-1]++;
					}
					else if(m_RNationalCode==2)
					{
						if(varValue.Find(_T("产品"))!=-1)
							ErrorCount[_wtoi(date)-1]++;
						else
							ChangeCount[_wtoi(date)-1]++;
					}
					else
					{
						if(varValue.Left(4)=="제품")
							ErrorCount[_wtoi(date)-1]++;
						else
							ChangeCount[_wtoi(date)-1]++;
					}
					m_pSet_7.MoveNext();	
					e++;
				}
			}
			if(m_pSet_7.b_IsOpenedDB)
				m_pSet_7.Close_DB();
		}		
	}*/
}


void CDlgPage4::Refresh()
{
	/*
	m_Chart4.GetBackdrop().GetFill().SetStyle(1);

	const VARIANT Index; 
    m_Chart4.GetPlot().GetAxis(VtChAxisIdY,Index).GetLabels().GetItem(1).SetFormat(_T("0"));
	m_Chart4.GetPlot().GetAxis(VtChAxisIdY2,Index).GetLabels().GetItem(1).SetFormat(_T("0"));
	DrawChart1();*/
	DrawChart1();

	GetDlgItem(IDC_STATIC)->ShowWindow(false);
	GetDlgItem(IDC_STATIC)->ShowWindow(true);
}

void CDlgPage4::DrawChart1()
{
	/*
	m_Chart4.GetBackdrop().GetFill().SetStyle(1);
	m_Chart4.SetChartType(1); 
	m_Chart4.SetAllowSelections(FALSE);
	m_Chart4.SetShowLegend(TRUE); 
	m_Chart4.SetMousePointer(2);

   	int m_Colnum1=2;
	int year=2013, month=1;
	year=_wtoi(m_SearchDate.Left(4));
	month=_wtoi(m_SearchDate.Right(2));
	int lday[12] = {31,0,31,30,31,30,31,31,30,31,30,31};
	if((year % 400) == 0 || (year % 100) != 0 && (year % 4)== 0)
	{
		lday[1] = 29;
	}
	else {
		lday[1] = 28;
	}

	int m_Rownum1=lday[month-1];
	int i;

	
	m_Chart4.SetColumnCount(m_Colnum1);
    m_Chart4.SetRowCount(m_Rownum1); 
	
	const VARIANT Index; 
    m_Chart4.GetPlot().GetAxis(VtChAxisIdY,Index).GetLabels().GetItem(1).SetFormat(_T("0"));
	m_Chart4.GetPlot().GetAxis(VtChAxisIdY2,Index).GetLabels().GetItem(1).SetFormat(_T("0"));

	CString str;
    m_Chart4.SetColumn(1);
	if(m_RNationalCode==1)
	{
		m_Chart4.SetColumnLabel(_T("Product"));
		m_Chart4.SetColumn(2);
		m_Chart4.SetColumnLabel(_T("Setting"));
	}
	else if(m_RNationalCode==2)
	{
		m_Chart4.SetColumnLabel(_T("Product"));//产品设置变更"));
		m_Chart4.SetColumn(2);
		m_Chart4.SetColumnLabel(_T("Setting"));//系统设置变更"));
	}
	else
	{
		m_Chart4.SetColumnLabel(_T("제품관련횟수"));
		m_Chart4.SetColumn(2);
		m_Chart4.SetColumnLabel(_T("설정변경횟수"));
	}


    
	for(i=1; i<=m_Rownum1; i++)
	{
		m_Chart4.SetRow(i);
		if(m_RNationalCode==1)
			str.Format(_T("%dday"), i);
		else if(m_RNationalCode==2)
			str.Format(_T("%d日"), i);
		else
			str.Format(_T("%d일"), i);
		m_Chart4.SetRowLabel(str);
        
	
		m_Chart4.SetColumn(1);
		str.Format(_T("%d"), ErrorCount[i-1]);
		m_Chart4.SetData(str);
		m_Chart4.SetColumn(2);
		str.Format(_T("%d"), ChangeCount[i-1]);
		m_Chart4.SetData(str);

	}
	m_Chart4.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1);
	m_Chart4.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetComponent(1);
	m_Chart4.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetValueFormat(_T("0"));
	
	m_Chart4.GetPlot().GetSeriesCollection().GetItem(2).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1);
	m_Chart4.GetPlot().GetSeriesCollection().GetItem(2).GetDataPoints().GetItem(-1).GetDataPointLabel().SetComponent(1);
	m_Chart4.GetPlot().GetSeriesCollection().GetItem(2).GetDataPoints().GetItem(-1).GetDataPointLabel().SetValueFormat(_T("0"));
	
    m_Chart4.Refresh();    
	*/

	std::vector<double> vecDataAlram;
	std::vector<double> vecDataChange;
	std::vector<char*> vecLabel;

	// Create a XYChart object of size 600 x 360 pixels
	XYChart c(1000, 250);

	// Set the plotarea at (70, 20) and of size 500 x 300 pixels, with transparent background and
	// border and light grey (0xcccccc) horizontal grid lines
	c.setPlotArea(60, 10, 700, 200, 0xffffff);
	c.addLegend(800, 100)->setBackground(0xffffff, 0);

	// Set the x and y axis stems to transparent and the label font to 12pt Arial
	c.yAxis()->setTopMargin(20);

	int year = 2013, month = 1;
	year = _wtoi(m_SearchDate.Left(4));
	month = _wtoi(m_SearchDate.Right(2));
	int day[12] = { 31, 0, 31,30,31,30,31,31,30,31,30,31 };
	if ((year % 400) == 0 || (year % 100) != 0 && (year % 4) == 0)
		day[1] = 29;
	else
		day[1] = 28;

	/////////// 개선 필요 //////////
	const char* label0[] = { u8"1일", u8"2일",u8"3일",u8"4일",u8"5일",u8"6일",u8"7일",u8"8일",u8"9일",u8"10일",
							u8"11일",u8"12일",u8"13일",u8"14일",u8"15일",u8"16일",u8"17일",u8"18일",u8"19일",u8"20일",
							u8"21일", u8"22일", u8"23일", u8"24일", u8"25일", u8"26일", u8"27일", u8"28일", u8"29일", u8"30일", u8"31일" };
	const char* label1[] = { u8"1Day", u8"2Day",u8"3Day",u8"4Day",u8"5Day",u8"6Day",u8"7Day",u8"8Day",u8"9Day",u8"10Day",
						u8"11Day",u8"12Day",u8"13Day",u8"14Day",u8"15Day",u8"16Day",u8"17Day",u8"18Day",u8"19Day",u8"20Day",
						u8"21Day", u8"22Day", u8"23Day", u8"24Day", u8"25Day", u8"26Day", u8"27Day", u8"28Day", u8"29Day", u8"30Day", u8"31Day" };
	const char* label2[] = { u8"1日", u8"2日",u8"3日",u8"4日",u8"5日",u8"6日",u8"7日",u8"8日",u8"9日",u8"10日",
						u8"11日",u8"12日",u8"13日",u8"14日",u8"15日",u8"16日",u8"17日",u8"18日",u8"19日",u8"20日",
						u8"21日", u8"22日", u8"23日", u8"24日", u8"25日", u8"26日", u8"27日", u8"28日", u8"29日", u8"30日", u8"31日" };
	const char* label3[] = { u8"1день", u8"2день",u8"3день",u8"4день",u8"5день",u8"6день",u8"7день",u8"8день",u8"9день",u8"10день",
						u8"11день",u8"12день",u8"13день",u8"14день",u8"15день",u8"16день",u8"17день",u8"18день",u8"19день",u8"20день",
						u8"21день", u8"22день", u8"23день", u8"24день", u8"25день", u8"26день", u8"27день", u8"28день", u8"29день", u8"30день", u8"31день" };
	const char* label4[] = { u8"1день", u8"2день",u8"3день",u8"4день",u8"5день",u8"6день",u8"7день",u8"8день",u8"9день",u8"10день",
						u8"11день",u8"12день",u8"13день",u8"14день",u8"15день",u8"16день",u8"17день",u8"18день",u8"19день",u8"20день",
						u8"21день", u8"22день", u8"23день", u8"24день", u8"25день", u8"26день", u8"27день", u8"28день", u8"29день", u8"30день", u8"31день" };
	const char* label5[] = { u8"1día", u8"2día",u8"3día",u8"4día",u8"5día",u8"6día",u8"7día",u8"8día",u8"9día",u8"10día",
						u8"11día",u8"12día",u8"13día",u8"14día",u8"15día",u8"16día",u8"17día",u8"18día",u8"19día",u8"20día",
						u8"21día", u8"22día", u8"23día", u8"24día", u8"25día", u8"26día", u8"27día", u8"28día", u8"29día", u8"30día", u8"31día" };
	const char* label6[] = { u8"1dzień", u8"2dzień",u8"3dzień",u8"4dzień",u8"5dzień",u8"6dzień",u8"7dzień",u8"8dzień",u8"9dzień",u8"10dzień",
						u8"11dzień",u8"12dzień",u8"13dzień",u8"14dzień",u8"15dzień",u8"16dzień",u8"17dzień",u8"18dzień",u8"19dzień",u8"20dzień",
						u8"21dzień", u8"22dzień", u8"23dzień", u8"24dzień", u8"25dzień", u8"26dzień", u8"27dzień", u8"28dzień", u8"29dzień", u8"30dzień", u8"31dzień" };
	const char* label7[] = { u8"1日", u8"2日",u8"3日",u8"4日",u8"5日",u8"6日",u8"7日",u8"8日",u8"9日",u8"10日",
						u8"11日",u8"12日",u8"13日",u8"14日",u8"15日",u8"16日",u8"17日",u8"18日",u8"19日",u8"20日",
						u8"21日", u8"22日", u8"23日", u8"24日", u8"25日", u8"26日", u8"27日", u8"28日", u8"29日", u8"30日", u8"31日" };
	////////////////////////////////////


	const int label_size = day[month - 1];

	for (auto n = 1; n <= day[month - 1]; n++) {
		//char temp[6];
		//sprintf_s(temp, "%d일", n);

		//char* chDay = "";
		//if (m_RNationalCode == 0)	chDay =  u8"일";
		//else if (m_RNationalCode == 1)	chDay = "Day";
		//else if (m_RNationalCode == 2)	chDay = u8"日";
		//else if (m_RNationalCode == 3)	chDay = u8"день";
		//else if (m_RNationalCode == 4)	chDay = u8"день";
		//else if (m_RNationalCode == 5)	chDay = u8"día";
		//else if (m_RNationalCode == 6)	chDay = u8"dzień";
		//else if (m_RNationalCode == 7)	chDay = u8"日";

		//vecLabel.push_back(temp);

		vecDataAlram.push_back((double)ErrorCount[n - 1]);
		vecDataChange.push_back((double)ChangeCount[n - 1]);
	}

	//c.xAxis()->setLabels(StringArray(&vecLabel[0], (int)vecLabel.size()));
	if (m_RNationalCode == 0)	c.xAxis()->setLabels(StringArray(label0, label_size));
	else if (m_RNationalCode == 1)	c.xAxis()->setLabels(StringArray(label1, label_size));
	else if (m_RNationalCode == 2)	c.xAxis()->setLabels(StringArray(label2, label_size));
	else if (m_RNationalCode == 3)	c.xAxis()->setLabels(StringArray(label3, label_size));
	else if (m_RNationalCode == 4)	c.xAxis()->setLabels(StringArray(label4, label_size));
	else if (m_RNationalCode == 5)	c.xAxis()->setLabels(StringArray(label5, label_size));
	else if (m_RNationalCode == 6)	c.xAxis()->setLabels(StringArray(label6, label_size));
	else if (m_RNationalCode == 7)	c.xAxis()->setLabels(StringArray(label7, label_size));

	char* cName1;
	char* cName2;
	BarLayer* layer;
	layer = c.addBarLayer(Chart::Side, 3);

	if (m_RNationalCode == 0)
	{
		cName1 = u8"제품관련횟수";
		cName2 = u8"설정변경횟수";
	}
	else if (m_RNationalCode == 1)
	{
		cName1 = "Product";
		cName2 = "Setting";
	}
	else if (m_RNationalCode == 2)
	{
		cName1 = "産品";
		cName2 = "設定";
	}
	else if (m_RNationalCode == 3)
	{
		cName1 = u8"продукт";
		cName2 = u8"Параметр";
	}
	else if (m_RNationalCode == 4)
	{
		cName1 = u8"продукту";
		cName2 = u8"Налаштування";
	}
	else if (m_RNationalCode == 5)
	{
		cName1 = "Producto";
		cName2 = u8"Configuración";
	}
	else if (m_RNationalCode == 6)
	{
		cName1 = u8"produkt";
		cName2 = u8"Konfiguracja";
	}
	else if (m_RNationalCode == 7)
	{
		cName1 = u8"産品";
		cName2 = u8"設定";
	}

	layer->addDataSet(DoubleArray(&vecDataAlram[0], (int)vecDataAlram.size()), 0xff0000, cName1);
	layer->addDataSet(DoubleArray(&vecDataChange[0], (int)vecDataChange.size()), 0x00ff00, cName2);

	layer->setAggregateLabelStyle("Arial", 10);


	// Set the labels on the x axis.	
	m_ChartViewer4.setChart(&c);
}
