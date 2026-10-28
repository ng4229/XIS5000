// DlgPage2.cpp : 구현 파dzień입니다.
//

#include "stdafx.h"
#include "DlgPage2.h"
#include "afxdialogex.h"

#include <iostream>
#include <string>
#include <fstream>

// CDlgPage2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgPage2, CDialogEx)

CDlgPage2::CDlgPage2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPage2::IDD, pParent)
{

	m_Page2_1 = _T("");
	m_Page2_2 = _T("");
	m_Page2_3 = _T("");
	m_Page2_4 = _T("");
	m_Date2 = COleDateTime::GetCurrentTime();
	m_SearchDate = "";
}

CDlgPage2::~CDlgPage2()
{
}

void CDlgPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_C, m_List_C);
	DDX_Text(pDX, IDC_PAGE2_1, m_Page2_1);
	DDX_Text(pDX, IDC_PAGE2_2, m_Page2_2);
	DDX_Text(pDX, IDC_PAGE2_3, m_Page2_3);
	DDX_Text(pDX, IDC_PAGE2_4, m_Page2_4);
	DDX_DateTimeCtrl(pDX, IDC_RDATETIMEPICKER2, m_Date2);
	//	DDX_Control(pDX, IDC_MSCHART2, m_Chart2);
	DDX_Control(pDX, IDC_INPUTDATA, m_BtInput);
	DDX_Control(pDX, IDC_CHART2, m_ChartViewer2);
}


BEGIN_MESSAGE_MAP(CDlgPage2, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_RDATETIMEPICKER2, &CDlgPage2::OnDatetimechangeRdatetimepicker2)
	ON_BN_CLICKED(IDC_INPUTDATA, &CDlgPage2::OnBnClickedInputdata)
END_MESSAGE_MAP()


// CDlgPage2 메시지 처리기입니다.
BOOL CDlgPage2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadBackground();
	((CDateTimeCtrl*)GetDlgItem(IDC_RDATETIMEPICKER2))->SetFormat(_T("         yyyy.   MM"));
	InitList();
	m_SearchDate = m_Date2.Format(_T("%Y-%m"));
	WriteReport();
	Refresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDlgPage2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		return false;	
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CDlgPage2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CDlgPage2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpMainBG);
	dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &cdc, 0, 0, SRCCOPY);
	cdc.DeleteDC(); 
}


void CDlgPage2::OnDatetimechangeRdatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	UpdateData(true);
	if(m_SearchDate!=m_Date2.Format(_T("%Y-%m")))
	{
		m_SearchDate = m_Date2.Format(_T("%Y-%m"));
		*pResult = 0;
		WriteReport();	
		Refresh();
	}
	*pResult = 0;
}


void CDlgPage2::LoadBackground()
{
	BITMAP bmpInfo; // 비트맵 정보
	if(m_RNationalCode==0)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_2);
	else if(m_RNationalCode==1)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_2_ENG);
	else if(m_RNationalCode==2)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_2_CHI);
	else if (m_RNationalCode == 3)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_2_RUS);
	else if (m_RNationalCode == 4)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_2_UKR);
	else if (m_RNationalCode == 5)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_2_SPN);
	else if (m_RNationalCode == 6)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_2_POL);
	else if (m_RNationalCode == 7)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_2_TIW);
    m_bmpMainBG.GetBitmap(&bmpInfo); // Main 배경 비트맵 정보 읽기
    m_nMainWidth = bmpInfo.bmWidth; // Main 윈도우 폭
    m_nMainHeight = bmpInfo.bmHeight; // Main 윈도우 높이
    SetWindowPos(NULL, 0, 0, m_nMainWidth, m_nMainHeight, SWP_NOZORDER | SWP_NOMOVE); 
	m_BtInput.LoadBitmaps(IDB_HIDEBT); 
	m_BtInput.SizeToContent();
}

void CDlgPage2::InitList()
{
	CRect rect;
	m_List_C.GetClientRect(&rect);
	m_List_C.InsertColumn(0, _T(""), LVCFMT_CENTER, 0);
	m_List_C.InsertColumn(1, _T(""), LVCFMT_CENTER, 173);
	m_List_C.InsertColumn(2, _T(""), LVCFMT_CENTER, rect.Width()-190);
	m_List_C.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
 }

void CDlgPage2::WriteReport()
{
	CFileFind   finder;
	CString m_strFilter;
	int c=0;
	
	if(!g_Connecting)		
	{
		//AfxMessageBox(_T("실패"));
	}
	else
	{
		
		MYSQL_RES* m_Res;
		MYSQL_ROW row;
		mysql_query(&m_mysql,"set names euckr");
		SqlQuery.Format(_T("SELECT * FROM intellisense_db3"));
		
		if(mysql_query(&m_mysql,CStringA(SqlQuery)))
		{
			CString error(mysql_error(&m_mysql));
			WriteErrorLog(3, error);
		}
		else
		{
			m_Res=mysql_store_result(&m_mysql);
				
			if((row = mysql_fetch_row(m_Res)) == NULL)
			{
				m_Page2_1="";
				SetDlgItemText(IDC_PAGE2_1, m_Page2_1);
				m_Page2_2="";
				SetDlgItemText(IDC_PAGE2_2, m_Page2_2);
				m_Page2_3="";
				SetDlgItemText(IDC_PAGE2_3, m_Page2_3);
				m_Page2_4="";
				SetDlgItemText(IDC_PAGE2_4, m_Page2_4);
			}
			else
			{
				CString strModel (row[0]);
				CString strSN (row[1]);
				CString strDate (row[2]);
				CString strRunTime (row[3]);

				SetDlgItemText(IDC_PAGE2_1, strModel);
				SetDlgItemText(IDC_PAGE2_2, strSN);
				SetDlgItemText(IDC_PAGE2_3, strDate);
				SetDlgItemText(IDC_PAGE2_4, strRunTime);	
			
			}
			mysql_free_result(m_Res);
		}
	}
	/*
    if(finder.FindFile(g_StrDirPath[3]))
	{
		if(!m_pSet_3.Open_DB(g_StrDirPath[3]))		
		{
			
		}
		else
		{
			CString strSQL;
			strSQL.Format(_T("SELECT * FROM Monoblock_Info"));
			if(m_pSet_3.Select_DB(strSQL))
			{
				if(m_pSet_3.IsEOF())
				{
					m_Page2_1="";
					SetDlgItemText(IDC_PAGE2_1, m_Page2_1);
					m_Page2_2="";
					SetDlgItemText(IDC_PAGE2_2, m_Page2_2);
					m_Page2_3="";
					SetDlgItemText(IDC_PAGE2_3, m_Page2_3);
					m_Page2_4="";
					SetDlgItemText(IDC_PAGE2_4, m_Page2_4);
				}
				else
				{
					m_pSet_3.ChangeRsChar(_T("strModel"), m_Page2_1);
					SetDlgItemText(IDC_PAGE2_1, m_Page2_1);
					m_pSet_3.ChangeRsChar(_T("strSN"), m_Page2_2);
					SetDlgItemText(IDC_PAGE2_2, m_Page2_2);
					m_pSet_3.ChangeRsChar(_T("StrDate"), m_Page2_3);
					SetDlgItemText(IDC_PAGE2_3, m_Page2_3);
					m_pSet_3.ChangeRsChar(_T("strRunTime"), m_Page2_4);
					SetDlgItemText(IDC_PAGE2_4, m_Page2_4);		
				}
	
			}
			if(m_pSet_3.b_IsOpenedDB)
				m_pSet_3.Close_DB();
		}
	}
	*/

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
		SqlQuery.Format(_T("SELECT * FROM intellisense_db4 WHERE strDate LIKE '%s-%%%%' ORDER BY strErrorTime DESC"), m_SearchDate);
		
		if(mysql_query(&m_mysql,CStringA(SqlQuery)))
		{
			CString error(mysql_error(&m_mysql));
			WriteErrorLog(4, error);
		}
		else
		{
			m_Res=mysql_store_result(&m_mysql);
			
			CString varValue;
			varValue = "";
			m_List_C.DeleteAllItems();
			while((row = mysql_fetch_row(m_Res)) != NULL)
			{

				CString strDate (row[0]);
				CString strErrorTime (row[1]);
				CString strErrorText (row[2]);
				CString strErrorCode (row[3]);

				
				m_List_C.InsertItem(index, _T("")); 
				m_List_C.SetItemText(index, 1, strErrorTime);	
				CString date= strErrorTime.Left(10).Right(2);
				m_List_C.SetItemText(index, 2, strErrorText);

				if(m_RNationalCode==0)
				{
					if (strErrorText.Find(_T("변경")) == -1)
						ErrorCount[_wtoi(date) - 1]++;
					else
						ChangeCount[_wtoi(date) - 1]++;
				}
				if(m_RNationalCode==1)
				{
					if(strErrorText.Find(_T("change"))==-1)
						ErrorCount[_wtoi(date)-1]++;
					else
						ChangeCount[_wtoi(date)-1]++;
				}
				else if(m_RNationalCode==2)
				{
					if(strErrorText.Find(_T("变更"))==-1)
						ErrorCount[_wtoi(date)-1]++;
					else
						ChangeCount[_wtoi(date)-1]++;
				}
				else if (m_RNationalCode == 3)
				{
					if (varValue.Find(_T("менять")) == -1)
						ErrorCount[_wtoi(date) - 1]++;
					else
						ChangeCount[_wtoi(date) - 1]++;
				}
				else if (m_RNationalCode == 4)
				{
					if (varValue.Find(_T("змінити")) == -1)
						ErrorCount[_wtoi(date) - 1]++;
					else
						ChangeCount[_wtoi(date) - 1]++;
				}
				else if (m_RNationalCode == 5)
				{
					if (varValue.Find(_T("cambio")) == -1)
						ErrorCount[_wtoi(date) - 1]++;
					else
						ChangeCount[_wtoi(date) - 1]++;
				}
				else if (m_RNationalCode == 6)
				{
					if (varValue.Find(_T("zmiana")) == -1)
						ErrorCount[_wtoi(date) - 1]++;
					else
						ChangeCount[_wtoi(date) - 1]++;
				}
				else if (m_RNationalCode == 7)
				{
					if (varValue.Find(_T("改變")) == -1)
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
    if(finder.FindFile(g_StrDirPath[4]))
	{
		m_List_C.DeleteAllItems();
		if(!m_pSet_4.Open_DB(g_StrDirPath[4]))		
		{
			
		}
		else
		{
			CString strSQL;
			strSQL.Format(_T("SELECT * FROM Monoblock_Log WHERE strDate LIKE '%s-%%%%' ORDER BY strErrorTime DESC"), m_SearchDate);
			if(m_pSet_4.Select_DB(strSQL))
			{
				CString varValue;
				varValue = "";
				c=0;
				while(!m_pSet_4.IsEOF())
				{
					m_List_C.InsertItem(c, _T("")); 
					m_pSet_4.ChangeRsChar(_T("strErrorTime"), varValue);
					m_List_C.SetItemText(c, 1, varValue);	
					CString date=varValue.Left(10).Right(2);
					m_pSet_4.ChangeRsChar(_T("strErrorText"), varValue);
					m_List_C.SetItemText(c, 2, varValue);
					if(m_RNationalCode==1)
					{
						if(varValue.Find(_T("change"))==-1)
							ErrorCount[_wtoi(date)-1]++;
						else
							ChangeCount[_wtoi(date)-1]++;
					}
					else if(m_RNationalCode==2)
					{
						if(varValue.Find(_T("变更"))==-1)
							ErrorCount[_wtoi(date)-1]++;
						else
							ChangeCount[_wtoi(date)-1]++;
					}
					else
					{
						if(varValue.Find(_T("변경"))==-1)
							ErrorCount[_wtoi(date)-1]++;
						else
							ChangeCount[_wtoi(date)-1]++;
					}
					m_pSet_4.MoveNext();	
					c++;
				}
			}
			if(m_pSet_4.b_IsOpenedDB)
				m_pSet_4.Close_DB();
		}		
	}*/
}

void CDlgPage2::Refresh()
{

	
	//m_Chart2.GetBackdrop().GetFill().SetStyle(1);

	//const VARIANT Index; 
	//m_Chart2.GetPlot().GetAxis(VtChAxisIdY,Index).GetLabels().GetItem(1).SetFormat(_T("0"));
	//m_Chart2.GetPlot().GetAxis(VtChAxisIdY2,Index).GetLabels().GetItem(1).SetFormat(_T("0"));
	DrawChart1();

	GetDlgItem(IDC_STATIC)->ShowWindow(false);
	GetDlgItem(IDC_STATIC)->ShowWindow(true);
}

void CDlgPage2::DrawChart1()
{
	/*
	m_Chart2.GetBackdrop().GetFill().SetStyle(1);
	m_Chart2.SetChartType(1); 
	m_Chart2.SetAllowSelections(FALSE);
	m_Chart2.SetShowLegend(TRUE); 
	m_Chart2.SetMousePointer(2);

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

	
	m_Chart2.SetColumnCount(m_Colnum1);
    m_Chart2.SetRowCount(m_Rownum1); 
	
	const VARIANT Index; 
    m_Chart2.GetPlot().GetAxis(VtChAxisIdY,Index).GetLabels().GetItem(1).SetFormat(_T("0"));
	m_Chart2.GetPlot().GetAxis(VtChAxisIdY2,Index).GetLabels().GetItem(1).SetFormat(_T("0"));

	CString str;
    m_Chart2.SetColumn(1);
	if(m_RNationalCode==1)
	{
		m_Chart2.SetColumnLabel(_T("Error"));
		m_Chart2.SetColumn(2);
		m_Chart2.SetColumnLabel(_T("Setting"));
	}
	else if(m_RNationalCode==2)
	{
		m_Chart2.SetColumnLabel(_T("Error"));//错误计数"));
		m_Chart2.SetColumn(2);
		m_Chart2.SetColumnLabel(_T("Setting"));//系统设置变更"));
	}
	else
	{
		m_Chart2.SetColumnLabel(_T("이상발생횟수"));
		m_Chart2.SetColumn(2);
		m_Chart2.SetColumnLabel(_T("설정변경횟수"));
	}
    
	for(i=1; i<=m_Rownum1; i++)
	{
		m_Chart2.SetRow(i);
		if(m_RNationalCode==1)
			str.Format(_T("%dday"), i);
		else if(m_RNationalCode==2)
			str.Format(_T("%d日"), i);
		else
			str.Format(_T("%d일"), i);
		m_Chart2.SetRowLabel(str);
        
	
		m_Chart2.SetColumn(1);
		str.Format(_T("%d"), ErrorCount[i-1]);
		m_Chart2.SetData(str);
		m_Chart2.SetColumn(2);
		str.Format(_T("%d"), ChangeCount[i-1]);
		m_Chart2.SetData(str);

	}
	m_Chart2.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1);
	m_Chart2.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetComponent(1);
	m_Chart2.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetValueFormat(_T("0"));
	
	m_Chart2.GetPlot().GetSeriesCollection().GetItem(2).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1);
	m_Chart2.GetPlot().GetSeriesCollection().GetItem(2).GetDataPoints().GetItem(-1).GetDataPointLabel().SetComponent(1);
	m_Chart2.GetPlot().GetSeriesCollection().GetItem(2).GetDataPoints().GetItem(-1).GetDataPointLabel().SetValueFormat(_T("0"));
	
    m_Chart2.Refresh();    */

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
							u8"21일", u8"22일", u8"23일", u8"24일", u8"25일", u8"26일", u8"27일", u8"28일", u8"29일", u8"30일", u8"31일"};
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
		cName1 = u8"이상발생횟수";
		cName2 = u8"설정변경횟수";
	}
	else if (m_RNationalCode == 1)
	{
		cName1 = "Error";
		cName2 = "Setting";
	}
	else if (m_RNationalCode == 2)
	{
		cName1 = "Error";
		cName2 = "Setting";
	}
	else if (m_RNationalCode == 3)
	{
		cName1 = u8"Ошибка";
		cName2 = u8"Параметр";
	}
	else if (m_RNationalCode == 4)
	{
		cName1 = u8"Помилка";
		cName2 = u8"Налаштування";
	}
	else if (m_RNationalCode == 5)
	{
		cName1 = "Error";
		cName2 = u8"Configuración";
	}
	else if (m_RNationalCode == 6)
	{
		cName1 = u8"Błąd";
		cName2 = u8"Konfiguracja";
	}
	else if (m_RNationalCode == 7)
	{
		cName1 = u8"錯誤";
		cName2 = u8"設定";
	}

	layer->addDataSet(DoubleArray(&vecDataAlram[0], (int)vecDataAlram.size()), 0xff0000, cName1);
	layer->addDataSet(DoubleArray(&vecDataChange[0], (int)vecDataChange.size()), 0x00ff00, cName2);

	layer->setAggregateLabelStyle("Arial", 10);


	// Set the labels on the x axis.	
	m_ChartViewer2.setChart(&c);
}

#include "InputDlg.h"

void CDlgPage2::OnBnClickedInputdata()
{
	CInputDlg dlg;

	//20180323 발생장치 수정
	if(m_Page2_1=="X3542" || m_Page2_1=="X4765(X3542)")
		dlg.m_Combo_1_Index=0;
	else if(m_Page2_1=="X4269")
		dlg.m_Combo_1_Index=1;	
	else if(m_Page2_1=="X4392" || m_Page2_1=="X5339(X4392)")
		dlg.m_Combo_1_Index=2;	
	else if(m_Page2_1=="VJ Tech")
		dlg.m_Combo_1_Index=3;	
	else if(m_Page2_1=="RXG-3071")
		dlg.m_Combo_1_Index=4;
	else if(m_Page2_1=="NXG-1001")
		dlg.m_Combo_1_Index=5;
	else //if(m_Page2_1=="NXG-1002")
		dlg.m_Combo_1_Index=6;

	dlg.m_Input1_Sn=m_Page2_2;
	if(m_Page2_4!="")
		dlg.m_Input1_Time=m_Page2_4;
	else
		dlg.m_Input1_Time="0";	
	dlg.m_Input1_Date.SetDate(_wtoi(m_Page2_3.Left(4)), _wtoi(m_Page2_3.Mid(5).Left(2)), _wtoi(m_Page2_3.Mid(8).Left(2)));
	
	int result = dlg.DoModal();
	if(result == IDOK)
	{
		CFileFind   finder;
		CString m_strFilter;
		int c=0;

		MYSQL_RES* m_Res;
		MYSQL_ROW row;

		SqlQuery.Format(_T("SELECT * FROM intellisense_db3"));
		if(mysql_query(&m_mysql,CStringA(SqlQuery)))
		{
			CString error(mysql_error(&m_mysql));
			WriteErrorLog(5, error);
		}
		else
		{
			m_Res=mysql_store_result(&m_mysql);

			CString strQuery;
			CString strModel, strRun=_T("0");
			//20180323
			if(dlg.m_Combo_1_Index==0)
				strModel="X4765(X3542)";
			else if(dlg.m_Combo_1_Index==1)
				strModel="X4269";
			else if(dlg.m_Combo_1_Index==2)
				strModel="X5339(X4392)";
			else if(dlg.m_Combo_1_Index==3)
				strModel="VJ Tech";
			else if(dlg.m_Combo_1_Index==4)
				strModel="RXG-3071";
			else if(dlg.m_Combo_1_Index==5)
				strModel="NXG-1001";
			else 
				strModel="NXG-1002";

			if(dlg.m_Input1_Time!="")
				strRun=dlg.m_Input1_Time;

		
	
			if((row = mysql_fetch_row(m_Res)) == NULL)
			{
				strQuery.Format(_T("Insert INTO intellisense_db3(strModel, strSN, StrDate, strRunTime)") _T("VALUES('%s', '%s', '%s', '%s')"),
						strModel, dlg.m_Input1_Sn, dlg.m_Input1_Date.Format(_T("%Y-%m-%d")), strRun);
			}
			else
			{
				strQuery.Format(_T("Update intellisense_db3 SET strModel = '%s', strSN = '%s', StrDate = '%s', strRunTime = '%s'"), 
					strModel, dlg.m_Input1_Sn, dlg.m_Input1_Date.Format(_T("%Y-%m-%d")),strRun);
			}
			
		
			if(mysql_query(&m_mysql,CStringA(strQuery)))
			{
				CString error(mysql_error(&m_mysql));
				WriteErrorLog(6, error);
			}
		
			mysql_free_result(m_Res);
					
			/*
			if(m_pSet_3.IsEOF())
			{
				strQuery.Format(_T("Insert INTO Monoblock_Info(strModel, strSN, StrDate, strRunTime)") _T("VALUES('%s', '%s', '%s', '%s')"),
					strModel, dlg.m_Input1_Sn, dlg.m_Input1_Date.Format(_T("%Y-%m-%d")), strRun);
				m_pSet_3.Insert_DB(strQuery);
			}
			else
			{
				strQuery.Format(_T("Update Monoblock_Info SET strModel = '%s', strSN = '%s', StrDate = '%s', strRunTime = '%s'"),
					strModel, dlg.m_Input1_Sn, dlg.m_Input1_Date.Format(_T("%Y-%m-%d")), strRun);
				m_pSet_3.Update_DB(strQuery);
			}
			*/
		}

		/*
		if(finder.FindFile(g_StrDirPath[3]))
		{
			if(!m_pSet_3.Open_DB(g_StrDirPath[3]))		
			{
				
			}
			else
			{
				CString strSQL;
				strSQL.Format(_T("SELECT * FROM Monoblock_Info"));
				if(m_pSet_3.Select_DB(strSQL))
				{
					CString strQuery;
					CString strModel, strRun=_T("0");
						//20180323
						if(dlg.m_Combo_1_Index==0)
							strModel="X4765(X3542)";
						else if(dlg.m_Combo_1_Index==1)
							strModel="X4269";
						else if(dlg.m_Combo_1_Index==2)
							strModel="X5339(X4392)";
						else if(dlg.m_Combo_1_Index==3)
							strModel="VJ Tech";
						else if(dlg.m_Combo_1_Index==4)
							strModel="RXG-3071";
						else if(dlg.m_Combo_1_Index==5)
							strModel="NXG-1001";
						else 
							strModel="NXG-1002";

						if(dlg.m_Input1_Time!="")
							strRun=dlg.m_Input1_Time;
					if(m_pSet_3.IsEOF())
					{
						strQuery.Format(_T("Insert INTO Monoblock_Info(strModel, strSN, StrDate, strRunTime)") _T("VALUES('%s', '%s', '%s', '%s')"),
							strModel, dlg.m_Input1_Sn, dlg.m_Input1_Date.Format(_T("%Y-%m-%d")), strRun);
						m_pSet_3.Insert_DB(strQuery);
					}
					else
					{
						strQuery.Format(_T("Update Monoblock_Info SET strModel = '%s', strSN = '%s', StrDate = '%s', strRunTime = '%s'"),
							strModel, dlg.m_Input1_Sn, dlg.m_Input1_Date.Format(_T("%Y-%m-%d")), strRun);
						m_pSet_3.Update_DB(strQuery);
					}
				}
				if(m_pSet_3.b_IsOpenedDB)
					m_pSet_3.Close_DB();
			}
		}
		*/

		WriteReport();
		Refresh();
	}
}