// DlgPage1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgPage1.h"
#include "afxdialogex.h"

#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <atlbase.h>

// CDlgPage1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgPage1, CDialogEx)

CDlgPage1::CDlgPage1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPage1::IDD, pParent)
{

	m_Date1 = COleDateTime::GetCurrentTime();
	m_SearchDate = _T("");
}

CDlgPage1::~CDlgPage1()
{
}

void CDlgPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_A, m_List_A);
	DDX_Control(pDX, IDC_LIST_B, m_List_B);
	DDX_DateTimeCtrl(pDX, IDC_RDATETIMEPICKER, m_Date1);
	DDX_Control(pDX, IDC_CHART1, m_ChartViewer);
	DDX_Control(pDX, IDC_TABLE, m_Table);
}


BEGIN_MESSAGE_MAP(CDlgPage1, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_RDATETIMEPICKER, &CDlgPage1::OnDatetimechangeRdatetimepicker)
END_MESSAGE_MAP()


// CDlgPage1 메시지 처리기입니다.


BOOL CDlgPage1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadBackground();
	InitList();
	m_SearchDate = m_Date1.Format(_T("%Y-%m-%d"));
	g_StrDate = m_SearchDate;
	WriteReport();
	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgPage1::LoadBackground()
{
	BITMAP bmpInfo; // 비트맵 정보
	if(m_RNationalCode==0)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_1);
	else if (m_RNationalCode == 1)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_1_ENG);
	else if (m_RNationalCode == 2)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_1_CHI);
	else if (m_RNationalCode == 3)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_1_RUS);
	else if (m_RNationalCode == 4)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_1_UKR);
	else if (m_RNationalCode == 5)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_1_SPN);
	else if (m_RNationalCode == 6)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_1_POL);
	else if (m_RNationalCode == 7)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_1_TIW);

	m_bmpMainBG.GetBitmap(&bmpInfo); // Main 배경 비트맵 정보 읽기
	m_nMainWidth = bmpInfo.bmWidth; // Main 윈도우 폭
	m_nMainHeight = bmpInfo.bmHeight; // Main 윈도우 높이
	SetWindowPos(NULL, 0, 0, m_nMainWidth, m_nMainHeight, SWP_NOZORDER | SWP_NOMOVE);
}

BOOL CDlgPage1::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		return false;

	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CDlgPage1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	//CBrush Brush, *pOldBrush;
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		//Brush.CreateStockObject(NULL_BRUSH);
		//pOldBrush = (CBrush*)pDC->SelectObject(&Brush);
		pDC->SetBkColor(RGB(255, 255, 255));
		hbr = ::CreateSolidBrush(RGB(255, 255, 255));
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


void CDlgPage1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpMainBG);
	dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &cdc, 0, 0, SRCCOPY);
	cdc.DeleteDC();
}

void CDlgPage1::InitList()
{
	CRect rect;
	m_List_A.GetClientRect(&rect);
	m_List_A.InsertColumn(0, _T(""), LVCFMT_CENTER, 0);
	m_List_A.InsertColumn(1, _T(""), LVCFMT_CENTER, rect.Width() / 5 + 23);
	m_List_A.InsertColumn(2, _T(""), LVCFMT_CENTER, rect.Width() / 5 - 10);
	m_List_A.InsertColumn(3, _T(""), LVCFMT_CENTER, rect.Width() / 5 - 10);
	m_List_A.InsertColumn(4, _T(""), LVCFMT_CENTER, rect.Width() / 5 - 10);
	m_List_A.InsertColumn(5, _T(""), LVCFMT_CENTER, rect.Width() / 5 - 10);
	m_List_B.GetClientRect(&rect);
	m_List_B.InsertColumn(0, _T(""), LVCFMT_CENTER, 0);
	m_List_B.InsertColumn(1, _T(""), LVCFMT_CENTER, rect.Width() / 4 - 20);
	m_List_B.InsertColumn(2, _T(""), LVCFMT_CENTER, rect.Width() / 4 - 20);
	m_List_B.InsertColumn(3, _T(""), LVCFMT_CENTER, rect.Width() / 4 - 20);
	m_List_B.InsertColumn(4, _T(""), LVCFMT_CENTER, rect.Width() / 4 + 43);
	m_List_A.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List_B.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}


void CDlgPage1::OnDatetimechangeRdatetimepicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	UpdateData(true);
	if (m_SearchDate != m_Date1.Format(_T("%Y-%m-%d")))
	{
		m_SearchDate = m_Date1.Format(_T("%Y-%m-%d"));
		g_StrDate = m_SearchDate;
		WriteReport();
		Refresh();
	}
	*pResult = 0;
}


void CDlgPage1::WriteReport()
{
	CFileFind   finder;
	CString m_strFilter;
	CString temp;
	int ok = 0, ng = 0, omit = 0, total = 0;
	int a = 0, b = 0;
	//20150601 생산수량 전체 합계 기능 추가 
	int t_ok = 0, t_ng = 0, t_omit = 0, t_total = 0;
	int index = 0;


	if (!g_Connecting)
	{
		//AfxMessageBox(_T("실패"));
	}
	else
	{

		MYSQL_RES* m_Res;
		MYSQL_ROW row;
		mysql_query(&m_mysql, "set names euckr");
		SqlQuery.Format(_T("SELECT * FROM intellisense_db1 WHERE strDate = '%s'"), m_SearchDate);

		if (mysql_query(&m_mysql, CStringA(SqlQuery)))
		{
			CString error(mysql_error(&m_mysql));
			WriteErrorLog(1, error);
		}
		else
		{
			m_Res = mysql_store_result(&m_mysql);
			m_List_A.DeleteAllItems();
			while ((row = mysql_fetch_row(m_Res)) != NULL)
			{
				CString strName(row[0]);
				CString strDate(row[1]);
				CString strTotal(row[2]);
				total = _wtoi(strTotal);
				CString strOk(row[3]);
				ok = _wtoi(strOk);
				CString strNg(row[4]);
				ng = _wtoi(strNg);
				CString strOmit(row[5]);
				omit = _wtoi(strOmit);

				if ((ok + ng + omit) == 0)
				{
					continue;
				}

				m_List_A.InsertItem(index, _T(""));
				m_List_A.SetItemText(index, 1, strName);

				t_total += total;
				m_List_A.SetItemText(index, 2, strTotal);

				t_ok += ok;
				m_List_A.SetItemText(index, 3, strOk);

				t_ng += ng;
				m_List_A.SetItemText(index, 4, strNg);

				t_omit += omit;
				m_List_A.SetItemText(index, 5, strOmit);
				index++;

			}
			mysql_free_result(m_Res);

			if (t_total != 0)
			{
				m_List_A.InsertItem(a, _T(""));
				if (m_RNationalCode == 0)
					m_List_A.SetItemText(a, 1, _T("합계"));
				else if (m_RNationalCode == 1)
					m_List_A.SetItemText(a, 1, _T("Total"));
				else if (m_RNationalCode == 2)
					m_List_A.SetItemText(a, 1, _T("总计"));
				else if (m_RNationalCode == 3)
					m_List_A.SetItemText(a, 1, _T("Общее"));
				else if (m_RNationalCode == 4)
					m_List_A.SetItemText(a, 1, _T("Усього"));
				else if (m_RNationalCode == 5)
					m_List_A.SetItemText(a, 1, _T("Total"));
				else if (m_RNationalCode == 6)
					m_List_A.SetItemText(a, 1, _T("całkowita"));
				else if (m_RNationalCode == 7)
					m_List_A.SetItemText(a, 1, _T("總計"));
				temp.Format(_T("%d"), t_total);
				m_List_A.SetItemText(a, 2, temp);
				temp.Format(_T("%d"), t_ok);
				m_List_A.SetItemText(a, 3, temp);
				temp.Format(_T("%d"), t_ng);
				m_List_A.SetItemText(a, 4, temp);
				temp.Format(_T("%d"), t_omit);
				m_List_A.SetItemText(a, 5, temp);
			}
		}
	}


	/*
	if(finder.FindFile(g_StrDirPath[1]))
	{
	//20131226
	m_List_A.DeleteAllItems();
	if(!m_pSet_1.Open_DB(g_StrDirPath[1]))
	{

	}
	else
	{
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM Product_Info WHERE strDate = '%s'"), m_SearchDate);
	if(m_pSet_1.Select_DB(strSQL))
	{
	a=0;
	while(m_pSet_1.IsEOF()!=1)
	{

	CString name=_T("");
	m_pSet_1.ChangeRsChar(_T("strName"), name);
	m_pSet_1.ChangeRsInt(_T("strTotal"), total);
	m_pSet_1.ChangeRsInt(_T("strOk"), ok);
	m_pSet_1.ChangeRsInt(_T("strNg"), ng);
	m_pSet_1.ChangeRsInt(_T("strOmit"), omit);
	if((ok+ng+omit)==0)
	{
	m_pSet_1.MoveNext();
	continue;
	}

	m_List_A.InsertItem(a, _T(""));
	m_List_A.SetItemText(a, 1, name);
	temp.Format(_T("%d"), total);
	t_total+=total;
	m_List_A.SetItemText(a, 2, temp);
	temp.Format(_T("%d"), ok);
	t_ok+=ok;
	m_List_A.SetItemText(a, 3, temp);
	temp.Format(_T("%d"), ng);
	t_ng+=ng;
	m_List_A.SetItemText(a, 4, temp);
	temp.Format(_T("%d"), omit);
	t_omit+=omit;
	m_List_A.SetItemText(a, 5, temp);
	a++;
	m_pSet_1.MoveNext();
	}
	}
	if(m_pSet_1.b_IsOpenedDB)
	m_pSet_1.Close_DB();

	if(t_total!=0)
	{
	m_List_A.InsertItem(a, _T(""));
	if(m_RNationalCode==1)
	m_List_A.SetItemText(a, 1, _T("Total"));
	else if(m_RNationalCode==2)
	m_List_A.SetItemText(a, 1, _T("总计"));
	else
	m_List_A.SetItemText(a, 1, _T("합계"));
	temp.Format(_T("%d"), t_total);
	m_List_A.SetItemText(a, 2, temp);
	temp.Format(_T("%d"), t_ok);
	m_List_A.SetItemText(a, 3, temp);
	temp.Format(_T("%d"), t_ng);
	m_List_A.SetItemText(a, 4, temp);
	temp.Format(_T("%d"), t_omit);
	m_List_A.SetItemText(a, 5, temp);
	}
	}
	}*/
	index = 0;
	if (!g_Connecting)
	{
		//AfxMessageBox(_T("실패"));
	}
	else
	{

		MYSQL_RES* m_Res;
		MYSQL_ROW row;
		mysql_query(&m_mysql, "set names euckr");
		SqlQuery.Format(_T("SELECT * FROM intellisense_db2  WHERE strDate = '%s'"), m_SearchDate);
		//	SqlQuery.Format(_T("SELECT * FROM intellisense_db2 ORDER BY strDate DESC"));
		if (mysql_query(&m_mysql, CStringA(SqlQuery)))
		{
			CString error(mysql_error(&m_mysql));
			WriteErrorLog(2, error);
		}
		else
		{
			m_Res = mysql_store_result(&m_mysql);
			m_List_B.DeleteAllItems();
			while ((row = mysql_fetch_row(m_Res)) != NULL)
			{
				CString strDate(row[0]);
				CString m_StartTime(row[1]);
				CString m_StopTime(row[2]);
				CString strName(row[3]);

				if (m_StopTime == "")
				{
					continue;
				}
				else
				{
					m_List_B.InsertItem(index, _T(""));
					m_List_B.SetItemText(index, 1, m_StartTime.Mid(11).Left(8));
					m_List_B.SetItemText(index, 2, m_StopTime.Mid(11).Left(8));
					COleDateTime StopTime, StartTime;
					StopTime = COleDateTime(_wtoi((LPCTSTR)m_StopTime.Left(4)),
						_wtoi((LPCTSTR)m_StopTime.Mid(5).Left(2)),
						_wtoi((LPCTSTR)m_StopTime.Mid(8).Left(2)),
						_wtoi((LPCTSTR)m_StopTime.Mid(11).Left(2)),
						_wtoi((LPCTSTR)m_StopTime.Mid(14).Left(2)),
						_wtoi((LPCTSTR)m_StopTime.Mid(17).Left(2)));
					StartTime = COleDateTime(_wtoi((LPCTSTR)m_StartTime.Left(4)),
						_wtoi((LPCTSTR)m_StartTime.Mid(5).Left(2)),
						_wtoi((LPCTSTR)m_StartTime.Mid(8).Left(2)),
						_wtoi((LPCTSTR)m_StartTime.Mid(11).Left(2)),
						_wtoi((LPCTSTR)m_StartTime.Mid(14).Left(2)),
						_wtoi((LPCTSTR)m_StartTime.Mid(17).Left(2)));

					COleDateTimeSpan RunTime = StopTime - StartTime;
					CString m_strRun;
					m_strRun.Format(_T("%02d:%02d:%02d"), (RunTime.GetDays() * 24) + RunTime.GetHours(), RunTime.GetMinutes(), RunTime.GetSeconds());
					m_List_B.SetItemText(index, 3, m_strRun);
				}

				m_List_B.SetItemText(index, 4, strName);
				index++;

			}
			mysql_free_result(m_Res);
		}


	}

	/*
	if(finder.FindFile(g_StrDirPath[2]))
	{
	m_List_B.DeleteAllItems();
	if(!m_pSet_2.Open_DB(g_StrDirPath[2]))
	{

	}
	else
	{
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM Running_Info WHERE strDate = '%s'"), m_SearchDate);
	if(m_pSet_2.Select_DB(strSQL))
	{
	b=0;
	CString name, m_StopTime, m_StartTime;
	while(m_pSet_2.IsEOF()!=1)
	{
	m_pSet_2.ChangeRsChar(_T("strStopTime"), m_StopTime);
	m_pSet_2.ChangeRsChar(_T("strStartTime"), m_StartTime);
	if(m_StopTime=="")
	{
	m_pSet_2.MoveNext();
	continue;
	}
	else
	{
	m_List_B.InsertItem(b, _T(""));
	m_List_B.SetItemText(b, 1, m_StartTime.Mid(11).Left(8));
	m_List_B.SetItemText(b, 2, m_StopTime.Mid(11).Left(8));
	COleDateTime StopTime, StartTime;
	StopTime = COleDateTime(_wtoi((LPCTSTR)m_StopTime.Left(4)),
	_wtoi((LPCTSTR)m_StopTime.Mid(5).Left(2)),
	_wtoi((LPCTSTR)m_StopTime.Mid(8).Left(2)),
	_wtoi((LPCTSTR)m_StopTime.Mid(11).Left(2)),
	_wtoi((LPCTSTR)m_StopTime.Mid(14).Left(2)),
	_wtoi((LPCTSTR)m_StopTime.Mid(17).Left(2)));
	StartTime = COleDateTime(_wtoi((LPCTSTR)m_StartTime.Left(4)),
	_wtoi((LPCTSTR)m_StartTime.Mid(5).Left(2)),
	_wtoi((LPCTSTR)m_StartTime.Mid(8).Left(2)),
	_wtoi((LPCTSTR)m_StartTime.Mid(11).Left(2)),
	_wtoi((LPCTSTR)m_StartTime.Mid(14).Left(2)),
	_wtoi((LPCTSTR)m_StartTime.Mid(17).Left(2)));

	COleDateTimeSpan RunTime = StopTime - StartTime;
	CString m_strRun;
	m_strRun.Format(_T("%02d:%02d:%02d"),(RunTime.GetDays()*24)+RunTime.GetHours(), RunTime.GetMinutes(), RunTime.GetSeconds());
	m_List_B.SetItemText(b, 3, m_strRun);
	}
	m_pSet_2.ChangeRsChar(_T("strName"), name);
	m_List_B.SetItemText(b, 4, name);
	m_pSet_2.MoveNext();
	b++;
	}
	if(m_pSet_2.b_IsOpenedDB)
	m_pSet_2.Close_DB();
	}
	}
	}*/
}


void CDlgPage1::Refresh()
{
	//	m_Chart1.GetBackdrop().GetFill().SetStyle(1);

	//const VARIANT Index;
	//   m_Chart1.GetPlot().GetAxis(VtChAxisIdY,Index).GetLabels().GetItem(1).SetFormat(_T("0"));
	//	m_Chart1.GetPlot().GetAxis(VtChAxisIdY2,Index).GetLabels().GetItem(1).SetFormat(_T("0"));
	DrawChart1();

	GetDlgItem(IDC_STATIC)->ShowWindow(false);
	GetDlgItem(IDC_STATIC)->ShowWindow(true);
}

DWORD convert_ansi_to_unicode_string(__out std::wstring& unicode, __in const char* ansi, __in const size_t ansi_size)
{
	DWORD error = 0;

	do {
		if ((nullptr == ansi) || (0 == ansi_size)) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		unicode.clear();

		int required_cch = ::MultiByteToWideChar(
			CP_ACP,
			0,
			ansi, static_cast<int>(ansi_size),
			nullptr, 0
		);

		if (0 == required_cch) {
			error = ::GetLastError();
			break;
		}

		unicode.resize(required_cch);

		if (0 == ::MultiByteToWideChar(
			CP_ACP,
			0,
			ansi, static_cast<int>(ansi_size),
			const_cast<wchar_t*>(unicode.c_str()), static_cast<int>(unicode.size())
		)) {
			error = ::GetLastError();
			break;
		}

	} while (false);

	return error;
}

DWORD convert_unicode_to_ansi_string(__out std::string& ansi,__in const wchar_t* unicode,__in const size_t unicode_size) 
{
	DWORD error = 0;

	do {

		if ((nullptr == unicode) || (0 == unicode_size)) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		ansi.clear();

		int required_cch = ::WideCharToMultiByte(
			CP_ACP,
			0,
			unicode, static_cast<int>(unicode_size),
			nullptr, 0,
			nullptr, nullptr
		);

		if (0 == required_cch) {
			error = ::GetLastError();
			break;
		}

		ansi.resize(required_cch);

		if (0 == ::WideCharToMultiByte(
			CP_ACP,
			0,
			unicode, static_cast<int>(unicode_size),
			const_cast<char*>(ansi.c_str()), static_cast<int>(ansi.size()),
			nullptr, nullptr
		)) {
			error = ::GetLastError();
			break;
		}

	} while (false);

	return error;
}

DWORD convert_unicode_to_utf8_string(__out std::string& utf8,__in const wchar_t* unicode,__in const size_t unicode_size) 
{

	DWORD error = 0;

	do {

		if ((nullptr == unicode) || (0 == unicode_size)) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		utf8.clear();

		int required_cch = ::WideCharToMultiByte(
			CP_UTF8,
			WC_ERR_INVALID_CHARS,
			unicode, static_cast<int>(unicode_size),
			nullptr, 0,
			nullptr, nullptr
		);

		if (0 == required_cch) {
			error = ::GetLastError();
			break;
		}

		utf8.resize(required_cch);

		if (0 == ::WideCharToMultiByte(
			CP_UTF8,
			WC_ERR_INVALID_CHARS,
			unicode, static_cast<int>(unicode_size),
			const_cast<char*>(utf8.c_str()), static_cast<int>(utf8.size()),
			nullptr, nullptr
		)) {
			error = ::GetLastError();
			break;
		}

	} while (false);

	return error;
}

void CDlgPage1::DrawChart1()
{
	/*
	m_Chart1.SetChartType(1); //원형14 막대 1
	m_Chart1.SetAllowSelections(FALSE); //차트 클릭 허용안함
	m_Chart1.SetShowLegend(TRUE); // 컬럼 레이블항목
	m_Chart1.SetMousePointer(2);

	int i;
	CString str;
	if(m_List_A.GetItemCount()<=0)
	{
	//	m_Chart1.SetTitleText("생산 데이터가 없습니다.");
	m_Chart1.SetColumnCount(3);
	m_Chart1.SetRowCount(1);
	m_Chart1.SetTitleText(_T(""));
	}
	else
	{
	m_Chart1.SetColumnCount(3);
	m_Chart1.SetRowCount(m_List_A.GetItemCount());
	m_Chart1.SetTitleText(_T(""));
	}


	for(i=1; i<=3; i++)
	{
	m_Chart1.SetColumn(i);
	if(m_RNationalCode==1)
	{
	if(i==1)
	m_Chart1.SetColumnLabel(_T("OK"));
	if(i==2)
	m_Chart1.SetColumnLabel(_T("NG"));
	if(i==3)
	m_Chart1.SetColumnLabel(_T("Omit"));
	}
	else if(m_RNationalCode==2)
	{
	if(i==1)
	m_Chart1.SetColumnLabel(_T("OK"));
	if(i==2)
	m_Chart1.SetColumnLabel(_T("NG"));
	if(i==3)
	m_Chart1.SetColumnLabel(_T("Omit"));
	}
	else
	{
	if(i==1)
	m_Chart1.SetColumnLabel(_T("정상"));
	if(i==2)
	m_Chart1.SetColumnLabel(_T("이물"));
	if(i==3)
	m_Chart1.SetColumnLabel(_T("누락"));
	}
	}

	if(m_List_A.GetItemCount()<=0)
	{
	m_Chart1.SetRow(1);
	if(m_RNationalCode==1)
	m_Chart1.SetRowLabel(_T("There is no data."));
	else if(m_RNationalCode==2)
	m_Chart1.SetRowLabel(_T("There is no data."));
	else
	m_Chart1.SetRowLabel(_T("생산 데이터가 없습니다."));
	for(int j=1; j<=3; j++)
	{
	m_Chart1.SetColumn(j);
	str.Format(_T("%d"), 0);
	m_Chart1.SetData(str);
	}
	}
	else
	{
	for(i=1; i<=m_List_A.GetItemCount(); i++)
	{
	m_Chart1.SetRow(i);
	m_Chart1.SetRowLabel(m_List_A.GetItemText(i-1,1));
	}

	for(i=1; i<=m_List_A.GetItemCount(); i++)
	{
	m_Chart1.SetRow(i);
	for(int j=1; j<=3; j++)
	{
	m_Chart1.SetColumn(j);
	str.Format(_T("%d"), _wtoi(m_List_A.GetItemText(i-1,j+2)));
	m_Chart1.SetData(str);


	}
	}
	}

	m_Chart1.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1);
	m_Chart1.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetComponent(1);
	m_Chart1.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetValueFormat(_T("0"));
	m_Chart1.GetPlot().GetSeriesCollection().GetItem(2).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1);
	m_Chart1.GetPlot().GetSeriesCollection().GetItem(2).GetDataPoints().GetItem(-1).GetDataPointLabel().SetComponent(1);
	m_Chart1.GetPlot().GetSeriesCollection().GetItem(2).GetDataPoints().GetItem(-1).GetDataPointLabel().SetValueFormat(_T("0"));
	m_Chart1.GetPlot().GetSeriesCollection().GetItem(3).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1);
	m_Chart1.GetPlot().GetSeriesCollection().GetItem(3).GetDataPoints().GetItem(-1).GetDataPointLabel().SetComponent(1);
	m_Chart1.GetPlot().GetSeriesCollection().GetItem(3).GetDataPoints().GetItem(-1).GetDataPointLabel().SetValueFormat(_T("0"));

	m_Chart1.Refresh();
	*/
	USES_CONVERSION;

	std::vector<double> vecDataOK;
	std::vector<double> vecDataNG;
	std::vector<double> vecDataOmit;
	std::vector<char*> vecLabel;

	std::vector<CString> vecData;

	// Create a XYChart object of size 600 x 360 pixels
	XYChart c(1000, 250);

	// Set the plotarea at (70, 20) and of size 500 x 300 pixels, with transparent background and
	// border and light grey (0xcccccc) horizontal grid lines
	c.setPlotArea(60, 10, 700, 200, 0xffffff);
	c.addLegend(830, 80)->setBackground(0xffffff, 0);

	// Set the x and y axis stems to transparent and the label font to 12pt Arial
	c.yAxis()->setTopMargin(20);

	if (m_List_A.GetItemCount() <= 0)
	{
		m_Table.ShowWindow(false);

		if (m_RNationalCode == 0)
			vecLabel.push_back(u8"생산 데이터가 없습니다.");
		else if (m_RNationalCode == 1)
			vecLabel.push_back("There is no data.");
		else if (m_RNationalCode == 2)
			vecLabel.push_back("There is no data.");
		else if (m_RNationalCode == 3)
			vecLabel.push_back(u8"Нет данных");
		else if (m_RNationalCode == 4)
			vecLabel.push_back(u8"Даних немає.");
		else if (m_RNationalCode == 5)
			vecLabel.push_back(u8"No hay datos.");
		else if (m_RNationalCode == 6)
			vecLabel.push_back(u8"nie ma danych.");
		else if (m_RNationalCode == 7)
			vecLabel.push_back(u8"沒有數據");

		vecDataOK.push_back(0);
		vecDataNG.push_back(0);
		vecDataOmit.push_back(0);

		c.xAxis()->setLabels(StringArray(&vecLabel[0], (int)vecLabel.size()));
	}
	else
	{
		m_Table.ShowWindow(true);

		for (auto i = 1; i <= m_List_A.GetItemCount(); i++)
		{
			CString cstrA = CString(m_List_A.GetItemText(i - 1, 1));
			//std::string str = std::string(CT2CA(cstrA));
			//std::wstring struni = L"";
			//std::string stru8 = u8"";

			//convert_ansi_to_unicode_string(struni, str.c_str(), str.size());
			//convert_unicode_to_utf8_string(stru8, struni.c_str(), struni.size());

			//std::cout << stru8.size() << std::endl;

			//char* pChar = cstrA.GetBuffer();
			//char* pChar = const_cast<char*>(stru8.c_str());

			//std::cout << pChar << std::endl;

			//USES_CONVERSION;
			//char* cmsg = T2A(cstrA);

			//vecLabel.push_back(cmsg);
			
			vecDataOK.push_back((double)_wtoi(m_List_A.GetItemText(i - 1, 3)));	// 정상
			vecDataNG.push_back((double)_wtoi(m_List_A.GetItemText(i - 1, 4)));	// 이물
			vecDataOmit.push_back((double)_wtoi(m_List_A.GetItemText(i - 1, 5)));	// 누락
			
			vecData.push_back(cstrA);
		}

		CString cstr;

		if (vecData.size() == 2)
			cstr.Format(vecData[0] + _T("\t\t\t\t\t\t") + vecData[1]);
		else if (vecData.size() == 3)
			cstr.Format(vecData[0] + _T("\t\t\t\t\t") + vecData[1] + _T("\t\t\t\t\t") + vecData[2]);
		else if (vecData.size() == 4)
			cstr.Format(vecData[0] + _T("\t\t\t    ") + vecData[1] + _T("\t\t\t    ") + vecData[2] + _T("\t\t\t    ") + vecData[3]);
		else if (vecData.size() == 5)
			cstr.Format(vecData[0] + _T("\t\t   ") + vecData[1] + _T("\t\t   ") + vecData[2] + _T("\t\t   ") + vecData[3] + _T("\t\t   ") + vecData[4]);
		else if (vecData.size() == 6)
			cstr.Format(vecData[0] + _T("\t\t") + vecData[1] + _T("\t\t") + vecData[2] + _T("\t\t") + vecData[3] + _T("\t\t") + vecData[4] + _T("\t\t") + vecData[5]);

		m_Table.SetWindowTextW(cstr);
	}

	//c.xAxis()->setLabels(StringArray(&vecLabel[0], (int)vecLabel.size()));

	char* cName1;
	char* cName2;
	char* cName3;
	BarLayer* layer;
	layer = c.addBarLayer(Chart::Side, 3);

	if(m_RNationalCode == 0)
	{
		cName1 = u8"정상";
		cName2 = u8"이물";
		cName3 = u8"누락";
	}
	else if(m_RNationalCode == 1)
	{
		cName1 = "OK";
		cName2 = "NG";
		cName3 = "Omit";
	}
	else if (m_RNationalCode == 2)
	{
		cName1 = "OK";
		cName2 = "NG";
		cName3 = "Omit";
	}
	else if (m_RNationalCode == 3)
	{
		cName1 = "OK";
		cName2 = u8"Не «ОК»";
		cName3 = u8"ПРОПУСТИТЬ";
	}
	else if (m_RNationalCode == 4)
	{
		cName1 = "OK";
		cName2 = u8"Не ОК";
		cName3 = u8"ПРОПУСТИТИ";
	}
	else if (m_RNationalCode == 5)
	{
		cName1 = "Okay";
		cName2 = "NG";
		cName3 = "Omitir";
	}
	else if (m_RNationalCode == 6)
	{
		cName1 = "OK";
		cName2 = "NG";
		cName3 = "Omit";
	}
	else if (m_RNationalCode == 7)
	{
		cName1 = "OK";
		cName2 = "NG";
		cName3 = u8"遺漏";
	}

	layer->addDataSet(DoubleArray(&vecDataOK[0], vecDataOK.size()), 0x00ff00, cName1);
	layer->addDataSet(DoubleArray(&vecDataNG[0], vecDataNG.size()), 0xff0000, cName2);
	layer->addDataSet(DoubleArray(&vecDataOmit[0], vecDataOmit.size()), 0xffbb00, cName3);

	layer->setAggregateLabelStyle("Arial", 12);
	

	// Set the labels on the x axis.	
	m_ChartViewer.setChart(&c);
}