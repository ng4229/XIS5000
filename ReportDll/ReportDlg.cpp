// ReportDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ReportDlg.h"
#include "afxdialogex.h"
#include "libharu/include/hpdf.h"

// CReportDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CReportDlg, CDialogEx)

CReportDlg::CReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReportDlg::IDD, pParent)
{
}

CReportDlg::~CReportDlg()
{
}

void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_EXCEL, m_BtExcel);
	DDX_Control(pDX, IDCANCEL, m_BtClose);
	DDX_Control(pDX, IDC_TAB, m_tabCtrl);
}

BEGIN_MESSAGE_MAP(CReportDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BT_EXCEL, &CReportDlg::OnBnClickedBtExcel)
END_MESSAGE_MAP()

// CReportDlg 메시지 처리기입니다.
BOOL CReportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	CStdioFile fileString;
	CString Language=_T("0");
	if(fileString.Open(_T("D:\\Database\\Language.ini"), 
		CStdioFile::modeRead | CStdioFile::shareDenyNone))
	{
		fileString.ReadString(Language);
		fileString.Close();
	}
	Init_Mysql();
	m_RNationalCode=_wtoi(Language);
	if (m_RNationalCode == 3) m_RNationalCode = 5;		// 20240528 스페인어 강제 변경
	LoadBackground();
	SetWindowPos(NULL, 0, 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);	
	if(m_RNationalCode==0)
	{
		m_tabCtrl.DeleteAllItems();
		m_tabCtrl.InsertItem(0, _T("  일일생산 정보  "));
		m_tabCtrl.InsertItem(1, _T("    X-ray 정보   "));
		m_tabCtrl.InsertItem(2, _T("   디텍터 정보   "));
		m_tabCtrl.InsertItem(3, _T("   컴퓨터 정보   "));
		m_tabCtrl.InsertItem(4, _T("   입출력 정보   "));
		m_tabCtrl.InsertItem(5, _T("  부품수명 정보 "));
		m_tabCtrl.InsertItem(6, _T(" 제품 정보 "));
	}
	else if(m_RNationalCode==1)
	{
		m_tabCtrl.DeleteAllItems();
		m_tabCtrl.InsertItem(0, _T("   Daily Report  "));
		m_tabCtrl.InsertItem(1, _T("   X-ray source  "));
		m_tabCtrl.InsertItem(2, _T("     Detector    "));
		m_tabCtrl.InsertItem(3, _T("     Computer    "));
		m_tabCtrl.InsertItem(4, _T("     I/O Card    "));
		m_tabCtrl.InsertItem(5, _T("    Main Parts  "));
		m_tabCtrl.InsertItem(6, _T(" Product Info "));
	}
	else if(m_RNationalCode==2)
	{
		m_tabCtrl.DeleteAllItems();
		m_tabCtrl.InsertItem(0, _T("     每日报告    "));
		m_tabCtrl.InsertItem(1, _T("     X线发生器   "));
		m_tabCtrl.InsertItem(2, _T("      探测器     "));
		m_tabCtrl.InsertItem(3, _T("       电脑      "));
		m_tabCtrl.InsertItem(4, _T("      I/O卡      "));
		m_tabCtrl.InsertItem(5, _T("     主要配件   "));
		m_tabCtrl.InsertItem(6, _T("    産品资料   "));
	}
	else if (m_RNationalCode == 3)
	{
		m_tabCtrl.DeleteAllItems();
		m_tabCtrl.InsertItem(0, _T("    ежедневный отчет   "));
		m_tabCtrl.InsertItem(1, _T("    XИсточник рентгеновского излучения  "));
		m_tabCtrl.InsertItem(2, _T("     детектор    "));
		m_tabCtrl.InsertItem(3, _T("      компьютер     "));
		m_tabCtrl.InsertItem(4, _T("     Карта ввода-вывода     "));
		m_tabCtrl.InsertItem(5, _T("    Основные части   "));
		m_tabCtrl.InsertItem(6, _T("    Информация о продукте   "));
	}
	else if (m_RNationalCode == 4)
	{
		m_tabCtrl.DeleteAllItems();
		m_tabCtrl.InsertItem(0, _T("Щоденний звіт"));
		m_tabCtrl.InsertItem(1, _T(" Джерело рентгенівського випромінювання "));
		m_tabCtrl.InsertItem(2, _T(" Детектор "));
		m_tabCtrl.InsertItem(3, _T(" Комп’ютер "));
		m_tabCtrl.InsertItem(4, _T(" Карта вводу-виводу "));
		m_tabCtrl.InsertItem(5, _T(" Основні частини "));
		m_tabCtrl.InsertItem(6, _T(" Інформація про продукт "));
	}
	else if (m_RNationalCode == 5)
	{
		m_tabCtrl.DeleteAllItems();
		m_tabCtrl.InsertItem(0, _T(" Reporte diario "));
		m_tabCtrl.InsertItem(1, _T(" X-ray Fuente "));
		m_tabCtrl.InsertItem(2, _T("  Detector  "));
		m_tabCtrl.InsertItem(3, _T("  Ordenador  "));
		m_tabCtrl.InsertItem(4, _T("  I/O Tarjeta  "));
		m_tabCtrl.InsertItem(5, _T("  Partes principales  "));
		m_tabCtrl.InsertItem(6, _T(" Producto Información "));
	}
	else if (m_RNationalCode == 6)
	{
		m_tabCtrl.DeleteAllItems();
		m_tabCtrl.InsertItem(0, _T(" Raport dzienny "));
		m_tabCtrl.InsertItem(1, _T(" Źródło RTG "));
		m_tabCtrl.InsertItem(2, _T("  Detektor  "));
		m_tabCtrl.InsertItem(3, _T("  Komputer  "));
		m_tabCtrl.InsertItem(4, _T("  Karta we/wy  "));
		m_tabCtrl.InsertItem(5, _T("  Części główne  "));
		m_tabCtrl.InsertItem(6, _T(" Info o produkcie "));
	}
	else if (m_RNationalCode == 7)
	{
		m_tabCtrl.DeleteAllItems();
		m_tabCtrl.InsertItem(0, _T(" 每日報告 "));
		m_tabCtrl.InsertItem(1, _T(" X-Ray光源 "));
		m_tabCtrl.InsertItem(2, _T("  探測器  "));
		m_tabCtrl.InsertItem(3, _T("  電腦  "));
		m_tabCtrl.InsertItem(4, _T("  I/O卡  "));
		m_tabCtrl.InsertItem(5, _T("  主要零件  "));
		m_tabCtrl.InsertItem(6, _T(" 產品資訊 "));
	}

	m_tabCtrl.Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CReportDlg::LoadBackground()
{
	if(m_RNationalCode==0)
	{
		m_bmpMainBG.LoadBitmap(IDB_REPORTBACKGROUND);
		m_BtClose.LoadBitmaps(IDB_BTRCLOSE, IDB_BTRCLOSE_1);
		m_BtExcel.LoadBitmaps(IDB_BTEXCEL, IDB_BTEXCEL_1);
	}
	else if(m_RNationalCode==1)
	{
		m_bmpMainBG.LoadBitmap(IDB_REPORTBACKGROUND_ENG);
		m_BtClose.LoadBitmaps(IDB_BTRCLOSE_ENG,IDB_BTRCLOSE_1_ENG); 
		m_BtExcel.LoadBitmaps(IDB_BTEXCEL_ENG,IDB_BTEXCEL_1_ENG); 
		
	}
	else if(m_RNationalCode==2)
	{
		m_bmpMainBG.LoadBitmap(IDB_REPORTBACKGROUND_CHI); 
		m_BtClose.LoadBitmaps(IDB_BTRCLOSE_CHI,IDB_BTRCLOSE_1_CHI); 
		m_BtExcel.LoadBitmaps(IDB_BTEXCEL_CHI,IDB_BTEXCEL_1_CHI); 
	}
	else if (m_RNationalCode == 3)
	{
		m_bmpMainBG.LoadBitmap(IDB_REPORTBACKGROUND_RUS);
		m_BtClose.LoadBitmaps(IDB_BTRCLOSE_RUS, IDB_BTRCLOSE_1_RUS);
		m_BtExcel.LoadBitmaps(IDB_BTEXCEL_RUS, IDB_BTEXCEL_1_RUS);
	}
	else if (m_RNationalCode == 4)
	{
		m_bmpMainBG.LoadBitmap(IDB_REPORTBACKGROUND_UKR);
		m_BtClose.LoadBitmaps(IDB_BTRCLOSE_UKR, IDB_BTRCLOSE_1_UKR);
		m_BtExcel.LoadBitmaps(IDB_BTEXCEL_UKR, IDB_BTEXCEL_1_UKR);
	}
	else if (m_RNationalCode == 5)
	{
		m_bmpMainBG.LoadBitmap(IDB_REPORTBACKGROUND_SPN);
		m_BtClose.LoadBitmaps(IDB_BTRCLOSE_SPN, IDB_BTRCLOSE_1_SPN);
		m_BtExcel.LoadBitmaps(IDB_BTEXCEL_SPN, IDB_BTEXCEL_1_SPN);
	}
	else if (m_RNationalCode == 6)
	{
		m_bmpMainBG.LoadBitmap(IDB_REPORTBACKGROUND_POL);
		m_BtClose.LoadBitmaps(IDB_BTRCLOSE_POL, IDB_BTRCLOSE_1_POL);
		m_BtExcel.LoadBitmaps(IDB_BTEXCEL_POL, IDB_BTEXCEL_1_POL);
	}
	else if (m_RNationalCode == 7)
	{
		m_bmpMainBG.LoadBitmap(IDB_REPORTBACKGROUND_TIW);
		m_BtClose.LoadBitmaps(IDB_BTRCLOSE_TIW, IDB_BTRCLOSE_1_TIW);
		m_BtExcel.LoadBitmaps(IDB_BTEXCEL_TIW, IDB_BTEXCEL_1_TIW);
	}

	BITMAP bmpInfo; 
	m_bmpMainBG.GetBitmap(&bmpInfo); 
	m_nMainWidth = bmpInfo.bmWidth; 
	m_nMainHeight = bmpInfo.bmHeight; 
	SetWindowPos(NULL, 0, 0, m_nMainWidth, m_nMainHeight, SWP_NOZORDER | SWP_NOMOVE); 
	m_BtClose.SizeToContent();
	m_BtExcel.SizeToContent();
}

BOOL CReportDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		return false;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CReportDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpMainBG);
	dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &cdc, 0, 0, SRCCOPY);
	cdc.DeleteDC(); 
}

void CReportDlg::OnBnClickedBtExcel()
{
	ReportDlg1(g_StrDate);
}

void CReportDlg::ReportDlg1(CString strDate)
{
	g_Save = false;
	CEYH_ADO m_pSet_1;
	CEYH_ADO m_pSet_2;
	CEYH_ADO m_pSet_4;
	CEYH_ADO m_pSet_6;
	CEYH_ADO m_pSet_7;
	CEYH_ADO m_pSet_8;
	
	CString strSQL;
	CString sHtml;
	int iRead = 0;
	CFile file;

	CString m_Filename;
	
	// pdf 저장
	CString m_pdfpath;
	CString PDF_count[1000];
	CString PDF_count2[1000];
	CString PDF_count3[1000];
	CString PDF_count4[1000];
	CString PDF_name[1000];
	CString PDF_time[1000];
	CString PDF_log[1000];
	int con = 0;
	int con2 = 0;
	int con3 = 0;

	//20181011
	if (m_RNationalCode == 0)
		m_Filename = _T("D:\\Database\\daychart1.htm");
	else if(m_RNationalCode==1)
		m_Filename=_T("D:\\Database\\daychart_eng.htm");
	else if(m_RNationalCode==2)
		m_Filename=_T("D:\\Database\\daychart_chi.htm");
	else if(m_RNationalCode==3)
		m_Filename=_T("D:\\Database\\daychart_rus.htm");
	else if (m_RNationalCode == 4)
		m_Filename = _T("D:\\Database\\daychart_ukr.htm");
	else if (m_RNationalCode == 5)
		m_Filename= _T("D:\\Database\\daychart_spn.htm");
	else if (m_RNationalCode == 6)
		m_Filename = _T("D:\\Database\\daychart_pol.htm");
	else if (m_RNationalCode == 7)
		m_Filename = _T("D:\\Database\\daychart_tiw.htm");

	if(file.Open(m_Filename, CFile::modeRead))
	{
		DWORD sizeff=file.GetLength();
		char* szToken = new char[sizeff + 1];
		file.Read(szToken, sizeff);
		szToken[sizeff]='\0';
		sHtml = szToken;
		delete szToken;
		file.Close();

		CString pRS1 = _T("");
		CString pRS2 = _T("");
		CString pRS3 = _T("");
		CString tempRS = _T("");
		CString tempPDF = _T("");

		if (!g_Connecting)
		{
			//AfxMessageBox(_T("실패"));
		}
		else
		{
			MYSQL_RES* m_Res;
			MYSQL_ROW row;
			mysql_query(&m_mysql, "set names euckr");
			SqlQuery.Format(_T("SELECT * FROM intellisense_db1 WHERE strDate = '%s'"), strDate);
			int ok = 0, ng = 0, omit = 0, total = 0;

			if (mysql_query(&m_mysql, CStringA(SqlQuery)))
			{
				SqlQuery.Format(_T("SELECT * FROM intellisense_db1 WHERE strDate = '%s'"), strDate);
				if(mysql_query(&m_mysql, CStringA(SqlQuery)))
				{
					CString error(mysql_error(&m_mysql));
					WriteErrorLog(33, error);
				}

				m_Res = mysql_store_result(&m_mysql);
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
					tempRS.Format(_T("<tr height=33 style='mso-height-source:userset;height:24.95pt'><td colspan=4 height=33 class=xl30 style='height:24.95pt'>%s</td><td colspan=3 class=xl30 style='border-left:none'>%d</td><td colspan=3 class=xl30 style='border-left:none'>%d</td><td colspan=3 class=xl30 style='border-left:none'>%d</td><td colspan=3 class=xl30 style='border-left:none'>%d</td></tr>"), strName, total, ok, ng, omit);
					pRS1 += tempRS;
					// pdf
					PDF_name[con].Format(_T("%s"), strName);
					PDF_count[con].Format(_T("%d"), total);
					PDF_count2[con].Format(_T("%d"), ok);
					PDF_count3[con].Format(_T("%d"), ng);
					PDF_count4[con].Format(_T("%d"), omit);
					con++;
				}
				mysql_free_result(m_Res);
			}
		}

		if (!g_Connecting)
		{
			//AfxMessageBox(_T("실패"));
		}
		else
		{
			CString m_strRun;

			MYSQL_RES* m_Res;
			MYSQL_ROW row;
			mysql_query(&m_mysql, "set names euckr");
			SqlQuery.Format(_T("SELECT * FROM intellisense_db2  WHERE strDate = '%s'"), strDate);
			
			if (mysql_query(&m_mysql, CStringA(SqlQuery)))
			{
				CString error(mysql_error(&m_mysql));
				WriteErrorLog(34, error);
			}
			else
			{
				m_Res = mysql_store_result(&m_mysql);

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
						m_strRun.Format(_T("%02d:%02d:%02d"), (RunTime.GetDays() * 24) + RunTime.GetHours(), RunTime.GetMinutes(), RunTime.GetSeconds());
					}

					tempRS.Format(_T("<tr height=33 style='mso-height-source:userset;height:24.75pt'><td colspan=4 height=33 class=xl30 style='height:24.75pt'>%s</td><td colspan=4 class=xl30 style='border-left:none'>%s</td><td colspan=4 class=xl30 style='border-left:none'>%s</td><td colspan=4 class=xl26 style='border-right:.5pt solid black;border-left:none'>%s</td></tr>"), m_StartTime.Mid(11).Left(8), m_StopTime.Mid(11).Left(8), m_strRun, strName);
					pRS2 += tempRS;
					// pdf
					PDF_time[con2].Format(_T("%s              %s                 %s                     %s"), m_StartTime.Mid(11).Left(8), m_StopTime.Mid(11).Left(8), m_strRun, strName);
					con2++;
				}
				mysql_free_result(m_Res);
			}
		}

		if (!g_Connecting)
		{
			//AfxMessageBox(_T("실패"));
		}
		else
		{
			CString etime, etext;

			MYSQL_RES* m_Res;
			MYSQL_ROW row;
			mysql_query(&m_mysql, "set names euckr");
			SqlQuery.Format(_T("SELECT * FROM intellisense_db4 WHERE strDate = '%s'"), strDate);

			if (mysql_query(&m_mysql, CStringA(SqlQuery)))
			{
				CString error(mysql_error(&m_mysql));
				WriteErrorLog(35, error);
			}
			else
			{
				m_Res = mysql_store_result(&m_mysql);

				while ((row = mysql_fetch_row(m_Res)) != NULL)
				{
					CString etime(row[1]);
					CString etext(row[2]);
					tempRS.Format(_T("<tr height=33 style='mso-height-source:userset;height:24.75pt'><td colspan=5 height=33 class=xl30 style='height:24.75pt'>%s</td><td colspan=11 class=xl30 style='border-left:none'>%s</td></tr>"), etime, etext);
					pRS3 += tempRS;
					// pdf
					PDF_log[con3].Format(_T("%s          %s      "), etime, etext);
					con3++;
				}
				mysql_free_result(m_Res);
			}
		}

		if (!g_Connecting)
		{
			//AfxMessageBox(_T("실패"));
		}
		else
		{
			CString etime, etext;

			MYSQL_RES* m_Res;
			MYSQL_ROW row;
			mysql_query(&m_mysql, "set names euckr");
			SqlQuery.Format(_T("SELECT * FROM intellisense_db6 WHERE strDate = '%s'"), strDate);

			if (mysql_query(&m_mysql, CStringA(SqlQuery)))
			{
				CString error(mysql_error(&m_mysql));
				WriteErrorLog(36, error);
			}
			else
			{
				m_Res = mysql_store_result(&m_mysql);

				while ((row = mysql_fetch_row(m_Res)) != NULL)
				{
					CString etime(row[1]);
					CString etext(row[2]);
					tempRS.Format(_T("<tr height=33 style='mso-height-source:userset;height:24.75pt'><td colspan=5 height=33 class=xl30 style='height:24.75pt'>%s</td><td colspan=11 class=xl30 style='border-left:none'>%s</td></tr>"), etime, etext);
					pRS3 += tempRS;
					// pdf
					PDF_log[con3].Format(_T("%s          %s      "), etime, etext);
					con3++;
				}
				mysql_free_result(m_Res);
			}
		}

		if (!g_Connecting)
		{
			//AfxMessageBox(_T("실패"));
		}
		else
		{
			CString etime, etext;

			MYSQL_RES* m_Res;
			MYSQL_ROW row;
			mysql_query(&m_mysql, "set names euckr");
			SqlQuery.Format(_T("SELECT * FROM intellisense_db7 WHERE strDate = '%s'"), strDate);

			if (mysql_query(&m_mysql, CStringA(SqlQuery)))
			{
				CString error(mysql_error(&m_mysql));
				WriteErrorLog(37, error);
			}
			else
			{
				m_Res = mysql_store_result(&m_mysql);

				while ((row = mysql_fetch_row(m_Res)) != NULL)
				{
					CString etime(row[1]);
					CString etext(row[2]);
					tempRS.Format(_T("<tr height=33 style='mso-height-source:userset;height:24.75pt'><td colspan=5 height=33 class=xl30 style='height:24.75pt'>%s</td><td colspan=11 class=xl30 style='border-left:none'>%s</td></tr>"), etime, etext);
					pRS3 += tempRS;
					// pdf
					PDF_log[con3].Format(_T("%s          %s      "), etime, etext);
					con3++;
				}
				mysql_free_result(m_Res);
			}
		}

		if (!g_Connecting)
		{
			//AfxMessageBox(_T("실패"));
		}
		else
		{
			CString etime, etext;

			MYSQL_RES* m_Res;
			MYSQL_ROW row;
			mysql_query(&m_mysql, "set names euckr");
			SqlQuery.Format(_T("SELECT * FROM intellisense_db8 WHERE strDate = '%s'"), strDate);

			if (mysql_query(&m_mysql, CStringA(SqlQuery)))
			{
				CString error(mysql_error(&m_mysql));
				WriteErrorLog(38, error);
			}
			else
			{
				m_Res = mysql_store_result(&m_mysql);

				while ((row = mysql_fetch_row(m_Res)) != NULL)
				{
					CString etime(row[1]);
					CString etext(row[2]);
					tempRS.Format(_T("<tr height=33 style='mso-height-source:userset;height:24.75pt'><td colspan=5 height=33 class=xl30 style='height:24.75pt'>%s</td><td colspan=11 class=xl30 style='border-left:none'>%s</td></tr>"), etime, etext);
					pRS3 += tempRS;
					// pdf
					PDF_log[con3].Format(_T("%s          %s      "), etime, etext);
					con3++;
				}
				mysql_free_result(m_Res);
			}
		}

		sHtml.Replace(_T("<--today-->"), strDate);
		//20181010
		sHtml.Replace(_T("<--tablename-->"), strDate);
		sHtml.Replace(_T("<--table1-->"), pRS1);
		sHtml.Replace(_T("<--table2-->"), pRS2);
		sHtml.Replace(_T("<--table3-->"), pRS3);
		CString path=_T(""), ext=_T("");

		CFileDialog filedlg(false, _T("Excel file(*.xls)"), strDate, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Save(*.xls) |*.xls|"), NULL);
	
		int user_choice = filedlg.DoModal();
		if (user_choice == IDOK) 
		{
			g_Save = true;
			path = filedlg.GetPathName();
			m_pdfpath = filedlg.GetPathName();
			ext = filedlg.GetFileExt(); 
			if(pRS1=="" && pRS2=="" && pRS3=="")
			{
				if (m_RNationalCode == 0)
					AfxMessageBox(_T("생산기록이 없습니다."));
				else if(m_RNationalCode==1)
					AfxMessageBox(_T("There is no data."));
				else if(m_RNationalCode==2)
					AfxMessageBox(_T("没有生产数据."));
				else if (m_RNationalCode == 3)
					AfxMessageBox(_T("Нет данных."));
				else if (m_RNationalCode == 4)
					AfxMessageBox(_T("Немає даних."));
				else if (m_RNationalCode == 5)
					AfxMessageBox(_T("No hay datos."));
				else if (m_RNationalCode == 6)
					AfxMessageBox(_T("Brak danych."));
				else if (m_RNationalCode == 7)
					AfxMessageBox(_T("無資料."));
				return;
			}
	
			if(ext=="xls")
			{
				CFile sFile;
			    sFile.Open(path, CFile::modeCreate | CFile::modeWrite);
				DWORD dwSize = sHtml.GetLength();   // 파일의 크기를 바이트 단위로 알아냄 << 첫번째
				char* lpsz = new char[dwSize + 1];
				USES_CONVERSION;
				lpsz=T2A(sHtml);
				lpsz[dwSize] = '\0';
				sFile.Write(lpsz, dwSize);
				sHtml.ReleaseBuffer();
			    sFile.Close();
   			}
		}
	}
	else
	{
		if (m_RNationalCode == 0)
			AfxMessageBox(_T("리포트 원본 파일이 없어 리포트를 저장할 수 없습니다. 관리자에게 문의하세요"));
		else if(m_RNationalCode==1)
			AfxMessageBox(_T("Could not find original file. Please contact service"));
		else if(m_RNationalCode==2)
			AfxMessageBox(_T("没有原文件不可保存. 请向管理者询问."));
		else if (m_RNationalCode == 3)
			AfxMessageBox(_T("Не удалось найти оригинальный файл."));
		else if (m_RNationalCode == 4)
			AfxMessageBox(_T("Не вдалося знайти оригінальний файл."));
		else if (m_RNationalCode == 5)
			AfxMessageBox(_T("No se pudo encontrar el archivo original. Póngase en contacto con el servicio"));
		else if (m_RNationalCode == 6)
			AfxMessageBox(_T("Nie udało się znaleźć oryginalnego pliku. Skontaktuj się z serwisem"));
		else if (m_RNationalCode == 7)
			AfxMessageBox(_T("找不到原始文件。"));
	}

	// pdf 저장
	if (g_Save)
	{
		HPDF_Doc pdf;
		HPDF_Error_Handler error_handler;
		pdf = HPDF_New(error_handler, NULL);
		HPDF_Page page[10];

		HPDF_Font pdf_font;
		HPDF_REAL pdf_width, pdf_height, pdf_tw;

		int nHeight_Current;
		const char* Text1;
		const char* Text2;

		const char* Count1;
		const char* Count2;
		const char* Count3;
		const char* Count4;

		const char* Date1;
		const char* Date2;
		const char* Date3;

		int nPage_count = 0;

		if (!pdf)
		{
			CString csMessage;
			csMessage = _T("Error : connot create pdf object.");
			AfxMessageBox(csMessage);
			return;
		}

		CStringA temp1;
		CStringA temp2;
		CStringA temp3;
		CStringA temp4;

		CStringA temp = CStringA(strDate);
		Date1 = temp;

		page[nPage_count] = HPDF_AddPage(pdf);

		pdf_height = HPDF_Page_GetHeight(page[nPage_count]);
		pdf_width = HPDF_Page_GetWidth(page[nPage_count]);

		/* Print the lines of the page. */
		HPDF_Page_SetLineWidth(page[nPage_count], 1);
		HPDF_Page_Rectangle(page[nPage_count], 50, 25, pdf_width - 100, pdf_height - 70);
		HPDF_Page_Stroke(page[nPage_count]);

		HPDF_UseKRFonts(pdf);
		HPDF_UseKREncodings(pdf);
		const char* korean_font_name;

		korean_font_name = HPDF_LoadTTFontFromFile(pdf, "c:/NanumGothic.ttf", HPDF_TRUE);
		pdf_font = HPDF_GetFont(pdf, korean_font_name, "KSC-EUC-H");

		HPDF_Page_SetFontAndSize(page[nPage_count], pdf_font, 24);

		if (m_RNationalCode == 0)
			pdf_tw = HPDF_Page_TextWidth(page[nPage_count], "X-ray 일일생산 보고서");
		if (m_RNationalCode == 1)
			pdf_tw = HPDF_Page_TextWidth(page[nPage_count], "X-ray Report");
		else if (m_RNationalCode == 2)
			pdf_tw = HPDF_Page_TextWidth(page[nPage_count], "X-ray 報告");
		else if (m_RNationalCode == 3)
			pdf_tw = HPDF_Page_TextWidth(page[nPage_count], "отчет");
		else if (m_RNationalCode == 4)
			pdf_tw = HPDF_Page_TextWidth(page[nPage_count], "Звіт");
		else if (m_RNationalCode == 5)
			pdf_tw = HPDF_Page_TextWidth(page[nPage_count], "X-ray Informe");
		else if (m_RNationalCode == 6)
			pdf_tw = HPDF_Page_TextWidth(page[nPage_count], "RTG raport");
		else if (m_RNationalCode == 7)
			pdf_tw = HPDF_Page_TextWidth(page[nPage_count], "X-ray 報告");

		HPDF_Page_BeginText(page[nPage_count]);

		if (m_RNationalCode == 0)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 일일생산 보고서");
		if (m_RNationalCode == 1)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray Report");
		else if (m_RNationalCode == 2)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 報告");
		else if (m_RNationalCode == 3)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "отчет");
		else if (m_RNationalCode == 4)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "Звіт");
		else if (m_RNationalCode == 5)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray Informe");
		else if (m_RNationalCode == 6)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "RTG raport");
		else if (m_RNationalCode == 7)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 報告");

		HPDF_Page_SetFontAndSize(page[nPage_count], pdf_font, 15);
		HPDF_Page_TextOut(page[nPage_count], pdf_width - 150, pdf_height - 40, Date1);
		HPDF_Page_EndText(page[nPage_count]);


		// 제품생산정보
		HPDF_Page_BeginText(page[nPage_count]);

		if (m_RNationalCode == 0)
		{
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<제품생산정보>");
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "제품명      총생산수량          정상수량             이물수량            누락수량");
		}
		else if (m_RNationalCode == 1)
		{
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Product Info>");
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Name           Total                 OK                     NG                    Omit");
		}
		else if (m_RNationalCode == 2)
		{
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<産品資訊>");
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "名稱           全部的                 OK                     NG                    遺漏");
		}
		else if (m_RNationalCode == 3)
		{
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Информация о продукте>");
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Продукт      Общее          OK             He OK            Пропускать");
		}
		else if (m_RNationalCode == 4)
		{
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Інформація про товар>");
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Продукт      Усього          OK             He OK            Опустити");
		}
		else if (m_RNationalCode == 5)
		{
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Producto Información>");
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Nombre           Total                 Okay                     NG                    Omitir");
		}
		else if (m_RNationalCode == 6)
		{
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<informacje o produkcie>");
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Nazwa           całkowity                 OK                     NG                    Omit");
		}
		else if (m_RNationalCode == 7)
		{
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<産品資訊>");
			HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "名稱           全部的                 OK                     NG                    遺漏");
		}

		HPDF_Page_EndText(page[nPage_count]);
		HPDF_Page_MoveTo(page[nPage_count], 55, pdf_height - 95);
		HPDF_Page_LineTo(page[nPage_count], pdf_width - 52, pdf_height - 95);
		HPDF_Page_Stroke(page[nPage_count]);

		nHeight_Current = pdf_height - 95;
		int n_Height = 0;
		n_Height = nHeight_Current;
		int nCount = 0;

		for (int i = 0; i < con; i++)
		{
			temp1 = CStringA(PDF_count[i]);
			Count1 = temp1;
			temp2 = CStringA(PDF_count2[i]);
			Count2 = temp2;
			temp3 = CStringA(PDF_count3[i]);
			Count3 = temp3;
			temp4 = CStringA(PDF_count4[i]);
			Count4 = temp4;

			temp = CStringA(PDF_name[i]);
			Text2 = temp;

			if (nHeight_Current <= 40)
			{
				nPage_count++;
				page[nPage_count] = HPDF_AddPage(pdf);
				HPDF_Page_SetLineWidth(page[nPage_count], 1);
				HPDF_Page_Rectangle(page[nPage_count], 50, 25, pdf_width - 100, pdf_height - 70);
				HPDF_Page_Stroke(page[nPage_count]);

				HPDF_Page_BeginText(page[nPage_count]);
				HPDF_Page_SetFontAndSize(page[nPage_count], pdf_font, 24);

				if (m_RNationalCode == 0)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 일일생산 보고서");
				else if (m_RNationalCode == 1)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray Report");
				else if (m_RNationalCode == 2)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 報告");
				else if (m_RNationalCode == 3)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "Рентгеновский отчет");
				else if (m_RNationalCode == 4)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "Рентгенівський звіт");
				else if (m_RNationalCode == 5)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray Informe");
				else if (m_RNationalCode == 6)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "RTG raport");
				else if (m_RNationalCode == 7)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 報告");


				HPDF_Page_SetFontAndSize(page[nPage_count], pdf_font, 15);
				HPDF_Page_TextOut(page[nPage_count], pdf_width - 150, pdf_height - 40, Date1);
				HPDF_Page_EndText(page[nPage_count]);

				HPDF_Page_BeginText(page[nPage_count]);

				if (m_RNationalCode == 0)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<제품생산정보>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "제품명      총생산수량          정상수량            이물수량            누락수량");
				}
				if (m_RNationalCode == 1)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Product Info>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Name           Total                 OK                     NG                   Omit");
				}
				else if (m_RNationalCode == 2)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<産品資訊>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "名稱           全部的                 OK                     NG                   遺漏");
				}
				else if (m_RNationalCode == 3)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Информация о продукте>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Продукт      Общее          OK             He OK            Пропускать");
				}
				else if (m_RNationalCode == 4)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Інформація про товар>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Продукт      Усього          OK             He OK            Опустити");
				}
				else if (m_RNationalCode == 5)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Producto Información>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Nombre           Total                 Okay                     NG                    Omitir");
				}
				else if (m_RNationalCode == 6)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<informacje o produkcie>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Nazwa           całkowity                 OK                     NG                    Omit");
				}
				else if (m_RNationalCode == 7)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<産品資訊>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "名稱           全部的                 OK                     NG                   遺漏");
				}

				HPDF_Page_EndText(page[nPage_count]);

				HPDF_Page_MoveTo(page[nPage_count], 55, pdf_height - 95);
				HPDF_Page_LineTo(page[nPage_count], pdf_width - 52, pdf_height - 95);
				nHeight_Current = pdf_height - 95;
				n_Height = nHeight_Current;

				nCount = 0;

				HPDF_Page_BeginText(page[nPage_count]);
				HPDF_Page_TextOut(page[nPage_count], 55, n_Height - (20 + (nCount * 20)), Text2);
				HPDF_Page_TextOut(page[nPage_count], 150, n_Height - (20 + (nCount * 20)), Count1);
				HPDF_Page_TextOut(page[nPage_count], 260, n_Height - (20 + (nCount * 20)), Count2);
				HPDF_Page_TextOut(page[nPage_count], 370, n_Height - (20 + (nCount * 20)), Count3);
				HPDF_Page_TextOut(page[nPage_count], 480, n_Height - (20 + (nCount * 20)), Count4);
				HPDF_Page_EndText(page[nPage_count]);

				nCount++;
			}
			else
			{
				HPDF_Page_BeginText(page[nPage_count]);
				HPDF_Page_TextOut(page[nPage_count], 55, n_Height - (20 + (nCount * 20)), Text2);
				HPDF_Page_TextOut(page[nPage_count], 150, n_Height - (20 + (nCount * 20)), Count1);
				HPDF_Page_TextOut(page[nPage_count], 260, n_Height - (20 + (nCount * 20)), Count2);
				HPDF_Page_TextOut(page[nPage_count], 370, n_Height - (20 + (nCount * 20)), Count3);
				HPDF_Page_TextOut(page[nPage_count], 480, n_Height - (20 + (nCount * 20)), Count4);
				HPDF_Page_EndText(page[nPage_count]);
				nHeight_Current = n_Height - (20 + (nCount * 20));
				nCount++;
			}
		}

		// 기기동작정보

		CStringA csTemp = CStringA(strDate);
		Date2 = csTemp;

		nPage_count++;
		page[nPage_count] = HPDF_AddPage(pdf);
		HPDF_Page_SetLineWidth(page[nPage_count], 1);
		HPDF_Page_Rectangle(page[nPage_count], 50, 25, pdf_width - 100, pdf_height - 70);
		HPDF_Page_Stroke(page[nPage_count]);

		HPDF_Page_SetFontAndSize(page[nPage_count], pdf_font, 24);
		HPDF_Page_BeginText(page[nPage_count]);

		if(m_RNationalCode==0)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 일일생산 보고서");
		if (m_RNationalCode == 1)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray Report");
		else if (m_RNationalCode == 2)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "報告");
		else if (m_RNationalCode == 3)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "Рентгеновский отчет");
		else if (m_RNationalCode == 4)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "Рентгенівський звіт");
		else if (m_RNationalCode == 5)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray Informe");
		else if (m_RNationalCode == 6)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "RTG raport");
		else if (m_RNationalCode == 7)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "報告");

		HPDF_Page_SetFontAndSize(page[nPage_count], pdf_font, 15);
		HPDF_Page_TextOut(page[nPage_count], pdf_width - 150, pdf_height - 40, Date2);
		HPDF_Page_EndText(page[nPage_count]);
		nHeight_Current = pdf_height - 35;

		HPDF_Page_BeginText(page[nPage_count]);
		if (m_RNationalCode == 0)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<기기동작정보>");
		else if (m_RNationalCode == 1)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<Machine Info>");
		else if (m_RNationalCode == 2)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<機器運行資訊>");
		else if (m_RNationalCode == 3)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<Информация о машине>");
		else if (m_RNationalCode == 4)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<Інформація про машину>");
		else if (m_RNationalCode == 5)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<Información de la máquina>");
		else if (m_RNationalCode == 6)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<informacje o maszynie>");
		else if (m_RNationalCode == 7)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<機器運行資訊>");


		nHeight_Current = nHeight_Current - 30;

		if (m_RNationalCode == 0)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "시작시간                정지시간                  가동시간                  생산제품명");
		else if (m_RNationalCode == 1)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "Starttime              Stoptime                  Runtime                  Productname");
		else if (m_RNationalCode == 2)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "開始時間              停止時間                  運行時間                  産品名稱");
		else if (m_RNationalCode == 3)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "Время начала                Остановить время                  Время выполнения                  наименование товара");
		else if (m_RNationalCode == 4)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "Час початку                Час зупинки                  Час роботи                  Назва продукту");
		else if (m_RNationalCode == 5)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "Hora de inicio              Para el tiempo                  Tiempo de ejecución                  Nombre del producto");
		else if (m_RNationalCode == 6)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "czas rozpoczecia  czas przestoju      czas pracy       Nazwa produktu");
		else if (m_RNationalCode == 7)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "開始時間              停止時間                  運行時間                  産品名稱");
		nHeight_Current = nHeight_Current - 20;
		HPDF_Page_EndText(page[nPage_count]);

		HPDF_Page_MoveTo(page[nPage_count], 55, nHeight_Current - 10);
		HPDF_Page_LineTo(page[nPage_count], pdf_width - 52, nHeight_Current - 10);
		HPDF_Page_Stroke(page[nPage_count]);
		nHeight_Current = nHeight_Current - 10;

		int n_Height2 = 0;
		n_Height2 = nHeight_Current;
		int nCount2 = 0;
		for (int i = 0; i < con2; i++)
		{
			temp = CStringA(PDF_time[i]);
			Text1 = temp;

			if (nHeight_Current <= 40)
			{
				nPage_count++;
				page[nPage_count] = HPDF_AddPage(pdf);
				HPDF_Page_SetLineWidth(page[nPage_count], 1);
				HPDF_Page_Rectangle(page[nPage_count], 50, 25, pdf_width - 100, pdf_height - 70);
				HPDF_Page_Stroke(page[nPage_count]);

				HPDF_Page_BeginText(page[nPage_count]);
				HPDF_Page_SetFontAndSize(page[nPage_count], pdf_font, 24);

				if (m_RNationalCode == 0)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 일일생산 보고서");
				if (m_RNationalCode == 1)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray Report");
				else if (m_RNationalCode == 2)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "報告");
				else if (m_RNationalCode == 3)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "Отчет о рентгеновском снимке");
				else if (m_RNationalCode == 4)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "Звіт Xray");
				else if (m_RNationalCode == 5)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray Informe");
				else if (m_RNationalCode == 6)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "RTG raport");
				else if (m_RNationalCode == 7)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "報告");

				HPDF_Page_SetFontAndSize(page[nPage_count], pdf_font, 15);
				HPDF_Page_TextOut(page[nPage_count], pdf_width - 150, pdf_height - 40, Date2);

				if (m_RNationalCode == 0)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<기기동작정보>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "시작시간                정지시간                  가동시간                  생산제품명");
				}
				else if (m_RNationalCode == 1)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Machine Info>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Strattime              Stoptime                  Runtime                  Productname");
				}
				else if (m_RNationalCode == 2)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<機器運行資訊>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "開始時間              停止時間                  運行時間                  産品名稱");
				}
				else if (m_RNationalCode == 3)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Информация о машине>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Время начала                Остановить время                  Время выполнения                  наименование товара");
				}
				else if (m_RNationalCode == 4)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Інформація про машину>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Час початку                Час зупинки                  Час роботи                  Назва продукту");
				}
				else if (m_RNationalCode == 5)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Información de la máquina>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Hora de inicio              Para el tiempo                  Tiempo de ejecución                  Nombre del producto");
				}
				else if (m_RNationalCode == 6)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<informacje o maszynie>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "czas rozpoczecia  czas przestoju      czas pracy       Nazwa produktu");

				}
				else if (m_RNationalCode == 7)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<機器運行資訊>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "開始時間              停止時間                  運行時間                  産品名稱");
				}

				HPDF_Page_EndText(page[nPage_count]);

				HPDF_Page_MoveTo(page[nPage_count], 55, pdf_height - 95);
				HPDF_Page_LineTo(page[nPage_count], pdf_width - 52, pdf_height - 95);
				HPDF_Page_Stroke(page[nPage_count]);
				nHeight_Current = pdf_height - 95;
				n_Height2 = nHeight_Current;

				nCount2 = 0;

				HPDF_Page_BeginText(page[nPage_count]);
				HPDF_Page_TextOut(page[nPage_count], 55, n_Height2 - (20 + (nCount2 * 20)), Text1);
				HPDF_Page_EndText(page[nPage_count]);
				nCount2++;


			}
			else
			{
				HPDF_Page_BeginText(page[nPage_count]);
				HPDF_Page_TextOut(page[nPage_count], 55, n_Height2 - (20 + (nCount2 * 20)), Text1);
				HPDF_Page_EndText(page[nPage_count]);
				nHeight_Current = n_Height2 - (20 + (nCount2 * 20));
				nCount2++;
			}
		}

		// 로그정보
		CStringA csTemp2 = CStringA(strDate);
		Date3 = csTemp2;

		nPage_count++;
		page[nPage_count] = HPDF_AddPage(pdf);
		HPDF_Page_SetLineWidth(page[nPage_count], 1);
		HPDF_Page_Rectangle(page[nPage_count], 50, 25, pdf_width - 100, pdf_height - 70);
		HPDF_Page_Stroke(page[nPage_count]);

		HPDF_Page_SetFontAndSize(page[nPage_count], pdf_font, 24);
		HPDF_Page_BeginText(page[nPage_count]);

		if (m_RNationalCode == 0)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 일일생산 보고서");
		else if (m_RNationalCode == 1)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray Report");
		else if (m_RNationalCode == 2)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 報告");
		else if (m_RNationalCode == 3)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "Рентгеновский отчет");
		else if (m_RNationalCode == 4)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "Звіт Xray");
		else if (m_RNationalCode == 5)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray Informe");
		else if (m_RNationalCode == 6)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "RTG raport");
		else if (m_RNationalCode == 7)
			HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 報告");

		HPDF_Page_SetFontAndSize(page[nPage_count], pdf_font, 15);
		HPDF_Page_TextOut(page[nPage_count], pdf_width - 150, pdf_height - 40, Date3);
		HPDF_Page_EndText(page[nPage_count]);
		nHeight_Current = pdf_height - 35;
		HPDF_Page_BeginText(page[nPage_count]);

		if (m_RNationalCode == 0)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<로그정보>");
		else if (m_RNationalCode == 1)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<Log>");
		else if (m_RNationalCode == 2)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<歷史紀錄>");
		else if (m_RNationalCode == 3)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<Бревно>");
		else if (m_RNationalCode == 4)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<Журнал>");
		else if (m_RNationalCode == 5)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<Tronco>");
		else if (m_RNationalCode == 6)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<Dziennik>");
		else if (m_RNationalCode == 7)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 30, "<歷史紀錄>");
		nHeight_Current = nHeight_Current - 30;

		if (m_RNationalCode == 0)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "발생시간                                                     발생내용       ");
		else if (m_RNationalCode == 1)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "   Time                                                     Log       ");
		else if (m_RNationalCode == 2)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "   時間                                                     歷史紀錄       ");
		else if (m_RNationalCode == 3)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "время                                                     Бревно       ");
		else if (m_RNationalCode == 4)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "час                                                     Журнал       ");
		else if (m_RNationalCode == 5)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "   Hora                                                     Tronco       ");
		else if (m_RNationalCode == 6)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "   czas                                                     Dziennik       ");
		else if (m_RNationalCode == 7)
			HPDF_Page_TextOut(page[nPage_count], 55, nHeight_Current - 20, "   時間                                                     歷史紀錄       ");
		nHeight_Current = nHeight_Current - 20;
		HPDF_Page_EndText(page[nPage_count]);

		HPDF_Page_MoveTo(page[nPage_count], 55, nHeight_Current - 10);
		HPDF_Page_LineTo(page[nPage_count], pdf_width - 52, nHeight_Current - 10);
		HPDF_Page_Stroke(page[nPage_count]);
		nHeight_Current = nHeight_Current - 10;

		int n_Height3 = 0;
		n_Height3 = nHeight_Current;
		int nCount3 = 0;
		for (int i = 0; i < con3; i++)
		{
			temp = CStringA(PDF_log[i]);
			Text1 = temp;

			if (nHeight_Current <= 40)
			{
				nPage_count++;
				page[nPage_count] = HPDF_AddPage(pdf);
				HPDF_Page_SetLineWidth(page[nPage_count], 1);
				HPDF_Page_Rectangle(page[nPage_count], 50, 25, pdf_width - 100, pdf_height - 70);
				HPDF_Page_Stroke(page[nPage_count]);

				HPDF_Page_BeginText(page[nPage_count]);
				HPDF_Page_SetFontAndSize(page[nPage_count], pdf_font, 24);

				if (m_RNationalCode == 0)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 일일생산 보고서");
				else if (m_RNationalCode == 1)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray Report");
				else if (m_RNationalCode == 2)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 報告");
				else if (m_RNationalCode == 3)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "рентгеновский отчет");
				else if (m_RNationalCode == 4)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "звіт xray");
				else if (m_RNationalCode == 5)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray Informe");
				else if (m_RNationalCode == 6)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "RTG raport");
				else if (m_RNationalCode == 7)
					HPDF_Page_TextOut(page[nPage_count], (pdf_width - pdf_tw) / 2, pdf_height - 25, "X-ray 報告");

				HPDF_Page_SetFontAndSize(page[nPage_count], pdf_font, 15);
				HPDF_Page_TextOut(page[nPage_count], pdf_width - 150, pdf_height - 40, Date3);

				if (m_RNationalCode == 0)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<로그정보>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "발생시간                                                    발생내용       ");
				}
				else if (m_RNationalCode == 1)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Log>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "   Time                                                    Log       ");
				}
				else if (m_RNationalCode == 2)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<歷史紀錄>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "   時間                                                    歷史紀錄       ");
				}
				else if (m_RNationalCode == 3)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Бревно>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Время                                                    Бревно       ");
				}
				else if (m_RNationalCode == 4)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Журнал>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "Час                                                    Журнал       ");
				}
				else if (m_RNationalCode == 5)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Tronco>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "   Hora                                                    Tronco       ");
				}
				else if (m_RNationalCode == 6)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<Dziennik>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "   czas                                                    Dziennik       ");
				}
				else if (m_RNationalCode == 7)
				{
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 65, "<歷史紀錄>");
					HPDF_Page_TextOut(page[nPage_count], 55, pdf_height - 85, "   時間                                                    歷史紀錄       ");
				}

				HPDF_Page_EndText(page[nPage_count]);

				HPDF_Page_MoveTo(page[nPage_count], 55, pdf_height - 95);
				HPDF_Page_LineTo(page[nPage_count], pdf_width - 52, pdf_height - 95);
				HPDF_Page_Stroke(page[nPage_count]);
				nHeight_Current = pdf_height - 95;
				n_Height3 = nHeight_Current;

				nCount3 = 0;

				HPDF_Page_BeginText(page[nPage_count]);
				HPDF_Page_TextOut(page[nPage_count], 55, n_Height3 - (20 + (nCount3 * 20)), Text1);
				HPDF_Page_EndText(page[nPage_count]);
				nCount3++;
			}
			else
			{
				HPDF_Page_BeginText(page[nPage_count]);
				HPDF_Page_TextOut(page[nPage_count], 55, n_Height3 - (20 + (nCount3 * 20)), Text1);
				HPDF_Page_EndText(page[nPage_count]);
				nHeight_Current = n_Height3 - (20 + (nCount3 * 20));
				nCount3++;
			}
		}

		for (int i = nPage_count; i < 1; i--)
		{
			page[nPage_count - 1] = HPDF_InsertPage(pdf, page[nPage_count]);
		}

		const char* path;
		m_pdfpath.Replace(_T(".xls"), _T(".pdf"));
		strDate = m_pdfpath;
		temp = CStringA(strDate);
		path = temp;
		HPDF_SaveToFile(pdf, path);

		HPDF_Free(pdf);
	}
}



void CReportDlg::Init_Mysql(void)
{
	mysql_init(&m_mysql);

	if(mysql_real_connect( &m_mysql, "localhost", "root", "123456", CStringA("intellisense_db"), 3306, NULL, 0 ) == NULL )
	{ // 디비 접속
		g_Connecting = false;
		CString Error(mysql_error(&m_mysql));
		WriteErrorLog(0, Error);
	}
	else
	{
		g_Connecting = true;
	}

	mysql_query(&m_mysql,"set names euckr");
}
