// DlgPage6.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgPage6.h"
#include "afxdialogex.h"

#include "InputDlg2.h"

// CDlgPage6 대화 상자입니다.
CString m_strCombo2[] = {_T("X-ray"), _T("디텍터"), _T("컴퓨터"), _T("모니터"), _T("컨베이어벨트"), _T("경광등")};
CString m_strCombo2Eng[] = {_T("X-ray"), _T("Detector"), _T("Computer"), _T("Monitor"), _T("Conveyor belt"), _T("Alarm lamp")};
CString m_strCombo2Chi[] = {_T("X线发生器"), _T("探测器"), _T("电脑"), _T("显示器"), _T("输送带"), _T("警灯")};
CString m_strCombo2Rus[] = { _T("Рентген"), _T("детектор"), _T("компьютер"), _T("Монитор"), _T("Конвейерная лента"), _T("Сигнальная лампа") };
CString m_strCombo2Ukr[] = { _T("Рентген"), _T("детектор"), _T("Комп’ютер"), _T("Монітор"), _T("Стрічковий транспортер"), _T("Сигнальна лампа") };
CString m_strCombo2Spn[] = { _T("X-ray"), _T("Detector"), _T("Computer"), _T("Monitor"), _T("Conveyor belt"), _T("Alarm lamp") };
CString m_strCombo2Pol[] = { _T("Źródło RTG"), _T("Detektor"), _T("Komputer"), _T("monitora"), _T("przenośniku taśmowym"), _T("Alarm lampka") };
CString m_strCombo2Tiw[] = { _T("X-Ray"), _T("探測器"), _T("電腦"), _T("監視器"), _T("輸送帶"), _T("報警燈") };
IMPLEMENT_DYNAMIC(CDlgPage6, CDialogEx)

CDlgPage6::CDlgPage6(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPage6::IDD, pParent)
{

	m_Clean1 = 0;
	m_Clean2 = 0;
	m_Clean3 = 0;
}

CDlgPage6::~CDlgPage6()
{
}

void CDlgPage6::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_ADD, m_BtAdd);
	DDX_Control(pDX, IDC_BT_DEL, m_BtDel);
	DDX_Control(pDX, IDC_BT_SAVE, m_BtSave);
	DDX_Radio(pDX, IDC_CLEAN1_0, m_Clean1);
	DDX_Control(pDX, IDC_CLEAN1_DAY, m_Clean1_Day);
	DDX_Control(pDX, IDC_CLEAN1_WEEKLY, m_Clean1_Week);
	DDX_Radio(pDX, IDC_CLEAN2_0, m_Clean2);
	DDX_Control(pDX, IDC_CLEAN2_DAY, m_Clean2_Day);
	DDX_Control(pDX, IDC_CLEAN2_WEEKLY, m_Clean2_Week);
	DDX_Radio(pDX, IDC_CLEAN3_0, m_Clean3);
	DDX_Control(pDX, IDC_CLEAN3_DAY, m_Clean3_Day);
	DDX_Control(pDX, IDC_CLEAN3_WEEKLY, m_Clean3_Week);
	DDX_Control(pDX, IDC_LIST_G, m_List_G);
}


BEGIN_MESSAGE_MAP(CDlgPage6, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BT_ADD, &CDlgPage6::OnClickedBtAdd)
	ON_BN_CLICKED(IDC_BT_DEL, &CDlgPage6::OnClickedBtDel)
	ON_BN_CLICKED(IDC_BT_SAVE, &CDlgPage6::OnClickedBtSave)
	ON_BN_CLICKED(IDC_CLEAN1_0, &CDlgPage6::OnClickedClean10)
	ON_COMMAND(IDC_CLEAN1_1, &CDlgPage6::OnClean11)
	ON_COMMAND(IDC_CLEAN1_2, &CDlgPage6::OnClean12)
	ON_COMMAND(IDC_CLEAN1_3, &CDlgPage6::OnClean13)
	ON_BN_CLICKED(IDC_CLEAN2_0, &CDlgPage6::OnClickedClean20)
	ON_COMMAND(IDC_CLEAN2_1, &CDlgPage6::OnClean21)
	ON_COMMAND(IDC_CLEAN2_2, &CDlgPage6::OnClean22)
	ON_COMMAND(IDC_CLEAN2_3, &CDlgPage6::OnClean23)
	ON_BN_CLICKED(IDC_CLEAN3_0, &CDlgPage6::OnClickedClean30)
	ON_COMMAND(IDC_CLEAN3_1, &CDlgPage6::OnClean31)
	ON_COMMAND(IDC_CLEAN3_2, &CDlgPage6::OnClean32)
	ON_COMMAND(IDC_CLEAN3_3, &CDlgPage6::OnClean33)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_G, &CDlgPage6::OnItemchangedListG)
END_MESSAGE_MAP()


// CDlgPage6 메시지 처리기입니다.


BOOL CDlgPage6::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadBackground();
	InitCombo();
	LoadClean();
	InitList();
	WriteReport();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgPage6::InitList()
{
	CRect rect;
	m_List_G.GetClientRect(&rect);
	m_List_G.InsertColumn(0, _T(""), LVCFMT_CENTER, 0);
	m_List_G.InsertColumn(1, _T(""), LVCFMT_CENTER, rect.Width()/4+90);
	m_List_G.InsertColumn(2, _T(""), LVCFMT_CENTER, rect.Width()/4-30);
	m_List_G.InsertColumn(3, _T(""), LVCFMT_CENTER, rect.Width()/4-30);
	m_List_G.InsertColumn(4, _T(""), LVCFMT_CENTER, rect.Width()/4-30);
	m_List_G.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void CDlgPage6::LoadBackground()
{	
	BITMAP bmpInfo; // 비트맵 정보
	if(m_RNationalCode==0)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_6);
	if(m_RNationalCode==1)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_6_ENG);
	else if(m_RNationalCode==2)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_6_CHI);
	else if (m_RNationalCode == 3)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_6_RUS);
	else if (m_RNationalCode == 4)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_6_UKR);
	else if (m_RNationalCode == 5)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_6_SPN);
	else if (m_RNationalCode == 6)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_6_POL);
	else if (m_RNationalCode == 7)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_6_TIW);
    m_bmpMainBG.GetBitmap(&bmpInfo); // Main 배경 비트맵 정보 읽기
    m_nMainWidth = bmpInfo.bmWidth; // Main 윈도우 폭
    m_nMainHeight = bmpInfo.bmHeight; // Main 윈도우 높이
	SetWindowPos(NULL, 0, 0, m_nMainWidth, m_nMainHeight, SWP_NOZORDER | SWP_NOMOVE); 

	if (m_RNationalCode == 0)
	{
		m_BtAdd.LoadBitmaps(IDB_BTADD, IDB_BTADD_1);
		m_BtAdd.SizeToContent();
		m_BtDel.LoadBitmaps(IDB_BTDEL, IDB_BTDEL_1);
		m_BtDel.SizeToContent();
		m_BtSave.LoadBitmaps(IDB_BTSAVE, IDB_BTSAVE_1);
		m_BtSave.SizeToContent();
	}
	if(m_RNationalCode==1)
	{
		m_BtAdd.LoadBitmaps(IDB_BTADD_ENG,IDB_BTADD_1_ENG); 
		m_BtAdd.SizeToContent();
		m_BtDel.LoadBitmaps(IDB_BTDEL_ENG,IDB_BTDEL_1_ENG); 
		m_BtDel.SizeToContent();
		m_BtSave.LoadBitmaps(IDB_BTSAVE_ENG,IDB_BTSAVE_1_ENG); 
		m_BtSave.SizeToContent();
	}
	else if(m_RNationalCode==2)
	{
		m_BtAdd.LoadBitmaps(IDB_BTADD_CHI,IDB_BTADD_1_CHI); 
		m_BtAdd.SizeToContent();
		m_BtDel.LoadBitmaps(IDB_BTDEL_CHI,IDB_BTDEL_1_CHI); 
		m_BtDel.SizeToContent();
		m_BtSave.LoadBitmaps(IDB_BTSAVE_CHI,IDB_BTSAVE_1_CHI); 
		m_BtSave.SizeToContent();
	}
	else if (m_RNationalCode == 3)
	{
		m_BtAdd.LoadBitmaps(IDB_BTADD_RUS, IDB_BTADD_1_RUS);
		m_BtAdd.SizeToContent();
		m_BtDel.LoadBitmaps(IDB_BTDEL_RUS, IDB_BTDEL_1_RUS);
		m_BtDel.SizeToContent();
		m_BtSave.LoadBitmaps(IDB_BTSAVE_RUS, IDB_BTSAVE_1_RUS);
		m_BtSave.SizeToContent();
	}
	else if (m_RNationalCode == 4)
	{
		m_BtAdd.LoadBitmaps(IDB_BTADD_UKR, IDB_BTADD_1_UKR);
		m_BtAdd.SizeToContent();
		m_BtDel.LoadBitmaps(IDB_BTDEL_UKR, IDB_BTDEL_1_UKR);
		m_BtDel.SizeToContent();
		m_BtSave.LoadBitmaps(IDB_BTSAVE_UKR, IDB_BTSAVE_1_UKR);
		m_BtSave.SizeToContent();
	}
	else if (m_RNationalCode == 5)
	{
		m_BtAdd.LoadBitmaps(IDB_BTADD_SPN, IDB_BTADD_1_SPN);
		m_BtAdd.SizeToContent();
		m_BtDel.LoadBitmaps(IDB_BTDEL_SPN, IDB_BTDEL_1_SPN);
		m_BtDel.SizeToContent();
		m_BtSave.LoadBitmaps(IDB_BTSAVE_SPN, IDB_BTSAVE_1_SPN);
		m_BtSave.SizeToContent();
	}
	else if (m_RNationalCode == 6)
	{
		m_BtAdd.LoadBitmaps(IDB_BTADD_POL, IDB_BTADD_1_POL);
		m_BtAdd.SizeToContent();
		m_BtDel.LoadBitmaps(IDB_BTDEL_POL, IDB_BTDEL_1_POL);
		m_BtDel.SizeToContent();
		m_BtSave.LoadBitmaps(IDB_BTSAVE_POL, IDB_BTSAVE_1_POL);
		m_BtSave.SizeToContent();
	}
	else if (m_RNationalCode == 7)
	{
		m_BtAdd.LoadBitmaps(IDB_BTADD_TIW, IDB_BTADD_1_TIW);
		m_BtAdd.SizeToContent();
		m_BtDel.LoadBitmaps(IDB_BTDEL_TIW, IDB_BTDEL_1_TIW);
		m_BtDel.SizeToContent();
		m_BtSave.LoadBitmaps(IDB_BTSAVE_TIW, IDB_BTSAVE_1_TIW);
		m_BtSave.SizeToContent();
	}
}

BOOL CDlgPage6::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		return false;
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CDlgPage6::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	CBrush Brush, *pOldBrush;	
    if (nCtlColor == CTLCOLOR_STATIC) 
	{
	    Brush.CreateStockObject(NULL_BRUSH);
        pOldBrush = (CBrush*)pDC->SelectObject(&Brush);
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH) Brush;
    }
	return hbr;
}


void CDlgPage6::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpMainBG);
	dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &cdc, 0, 0, SRCCOPY);
	cdc.DeleteDC(); 
}

void CDlgPage6::OnClickedBtAdd()
{
	CInputDlg2 dlg;

	int result = dlg.DoModal();
	if(result == IDOK)
	{
		CFileFind   finder;
		CString m_strFilter;
		int c=0;

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
			CString temp, cycle=_T("0");
			CString strSQL, strQuery;
			if(m_RNationalCode==0)
				temp = m_strCombo2[dlg.m_Combo_1_Index];
			if(m_RNationalCode==1)
				temp=m_strCombo2Eng[dlg.m_Combo_1_Index];
			else if(m_RNationalCode==2)
				temp=m_strCombo2Chi[dlg.m_Combo_1_Index];
			else if (m_RNationalCode == 3)
				temp = m_strCombo2Rus[dlg.m_Combo_1_Index];
			else if (m_RNationalCode == 4)
				temp = m_strCombo2Ukr[dlg.m_Combo_1_Index];
			else if (m_RNationalCode == 5)
				temp = m_strCombo2Spn[dlg.m_Combo_1_Index];
			else if (m_RNationalCode == 6)
				temp = m_strCombo2Pol[dlg.m_Combo_1_Index];
			else if (m_RNationalCode == 7)
				temp = m_strCombo2Tiw[dlg.m_Combo_1_Index];
			strSQL.Format(_T("SELECT * FROM intellisense_db10 WHERE strName = '%s'"), temp);
				
			if(mysql_query(&m_mysql,CStringA(strSQL)))
			{
				CString error(mysql_error(&m_mysql));
				WriteErrorLog(13, error);
			}
			else
			{
				m_Res=mysql_store_result(&m_mysql);
				if(dlg.m_Input1_Time!="")
						cycle=dlg.m_Input1_Time;
				if((row = mysql_fetch_row(m_Res)) == NULL)
				{
					strQuery.Format(_T("Insert INTO intellisense_db10(strName, strInstallDate, strExpectDate, strCycle)") _T("VALUES('%s', '%s', '%s', '%s')"),
							temp, dlg.m_Input1_Date.Format(_T("%Y-%m-%d")), _T("0"),  cycle);
				}
				else
				{
					strQuery.Format(_T("Update intellisense_db10 SET strInstallDate = '%s', strExpectDate = '%s', strCycle = '%s' WHERE strName = '%s'"),
							dlg.m_Input1_Date.Format(_T("%Y-%m-%d")), _T("0"),  cycle, temp);
				}

				if(mysql_query(&m_mysql,CStringA(strQuery)))
				{
					CString error(mysql_error(&m_mysql));
					WriteErrorLog(14, error);
				}
			}

			mysql_free_result(m_Res);
		}
			

		/*
		if(finder.FindFile(g_StrDirPath[10]))
		{
			if(!m_pSet_10.Open_DB(g_StrDirPath[10]))		
			{
				
			}
			else
			{
				CString temp, cycle=_T("0");
				CString strSQL, strQuery;
				if(m_RNationalCode==1)
					temp=m_strCombo2Eng[dlg.m_Combo_1_Index];
				else if(m_RNationalCode==2)
					temp=m_strCombo2Chi[dlg.m_Combo_1_Index];
				else 
					temp=m_strCombo2[dlg.m_Combo_1_Index];
				strSQL.Format(_T("SELECT * FROM PartChange WHERE strName = '%s'"), temp);
				if(m_pSet_10.Select_DB(strSQL))
				{
					if(dlg.m_Input1_Time!="")
						cycle=dlg.m_Input1_Time;
					if(m_pSet_10.IsEOF())
					{
						strQuery.Format(_T("Insert INTO PartChange(strName, strInstallDate, strExpectDate, strCycle)") _T("VALUES('%s', '%s', '%s', '%s')"),
							temp, dlg.m_Input1_Date.Format(_T("%Y-%m-%d")), _T("0"),  cycle);
						m_pSet_10.Insert_DB(strQuery);		

					}
					else
					{
						strQuery.Format(_T("Update PartChange SET strInstallDate = '%s', strExpectDate = '%s', strCycle = '%s' WHERE strName = '%s'"),
							dlg.m_Input1_Date.Format(_T("%Y-%m-%d")), _T("0"),  cycle, temp);
						m_pSet_10.Update_DB(strQuery);
					}
				}
				if(m_pSet_10.b_IsOpenedDB)
					m_pSet_10.Close_DB();
			}
		}*/
		WriteReport();
	}
}


void CDlgPage6::OnClickedBtDel()
{
	CFileFind   finder;

	if(g_Connecting == false)		
	{
			
	}
	else
	{		
			
		CString strQuery;
		strQuery.Format(_T("Delete From intellisense_db10 WHERE strName = '%s'"), temp_name);
		if(mysql_query(&m_mysql,CStringA(strQuery)))
		{
			CString error(mysql_error(&m_mysql));
			WriteErrorLog(15, error);		
		}
	}	
	
	/*
	if(finder.FindFile(g_StrDirPath[10]))
	{
		if(!m_pSet_10.Open_DB(g_StrDirPath[10]))		
		{
			
		}
		else
		{
			CString strQuery;
			strQuery.Format(_T("Delete From PartChange WHERE strName = '%s'"), temp_name);
			m_pSet_10.Delete_DB(strQuery);
				
			if(m_pSet_10.b_IsOpenedDB)
				m_pSet_10.Close_DB();
		}	
	}*/
	WriteReport();	
}


void CDlgPage6::OnClickedBtSave()
{
	CFileFind   finder;

	MYSQL_RES* m_Res;
	MYSQL_ROW row;
	mysql_query(&m_mysql,"set names euckr");
	SqlQuery.Format(_T("SELECT * FROM intellisense_db10 WHERE strName LIKE 'Clean%%'"));
		
	if(mysql_query(&m_mysql,CStringA(SqlQuery)))
	{
		CString error(mysql_error(&m_mysql));
		WriteErrorLog(16, error);
	}
	else
	{
		m_Res=mysql_store_result(&m_mysql);
		while((row = mysql_fetch_row(m_Res)) != NULL)
		{
			CString strInstallDate (row[0]);
			CString strExpectDate (row[1]);
			CString strCycle (row[2]);
			CString strName (row[3]);


		//	CString tmp=_T("");
		//	m_pSet_10.ChangeRsChar(_T("strName"), tmp);
			CString tmpd=_T("");
			CString m_Cycle=_T("");
			if(strName==_T("Clean1"))
			{
				tmpd.Format(_T("%d"), m_Clean1);
				if(m_Clean1==2)
					m_Cycle.Format(_T("%d"), m_Clean1_Week.GetCurSel());
				else if(m_Clean1==3)
					m_Cycle.Format(_T("%d"), m_Clean1_Day.GetCurSel());
				else
					m_Cycle=_T("0");
					
			}
			else if(strName==_T("Clean2"))
			{
				tmpd.Format(_T("%d"), m_Clean2);
				if(m_Clean2==2)
					m_Cycle.Format(_T("%d"), m_Clean2_Week.GetCurSel());
				else if(m_Clean2==3)
					m_Cycle.Format(_T("%d"), m_Clean2_Day.GetCurSel());
				else
					m_Cycle=_T("0");
			}
			else if(strName==_T("Clean3"))
			{
				tmpd.Format(_T("%d"), m_Clean3);
				int m_Clean3=_wtoi(tmpd);
				if(m_Clean3==2)
					m_Cycle.Format(_T("%d"), m_Clean3_Week.GetCurSel());
				else if(m_Clean3==3)
					m_Cycle.Format(_T("%d"), m_Clean3_Day.GetCurSel());
				else
					m_Cycle=_T("0");
			}		
			CString strQuery;
			strQuery.Format(_T("Update intellisense_db10 SET strInstallDate = '%s', strExpectDate = '%s', strCycle = '%s' WHERE strName = '%s'"),
				tmpd, _T("0"),  m_Cycle, strName);
			
			if(mysql_query(&m_mysql,CStringA(strQuery)))
			{
				CString error(mysql_error(&m_mysql));
				WriteErrorLog(17, error);
			}
		//	m_pSet_10.Update_DB(strQuery);
		//	m_pSet_10.MoveNext();
		}

		mysql_free_result(m_Res);
	}




	/*
	if(finder.FindFile(g_StrDirPath[10]))
	{
		if(!m_pSet_10.Open_DB(g_StrDirPath[10]))		
		{
			
		}
		else
		{
			CString strSQL;
			strSQL.Format(_T("SELECT * FROM PartChange WHERE strName LIKE 'Clean%%'"));
			if(m_pSet_10.Select_DB(strSQL))
			{
				while(m_pSet_10.IsEOF()!=1)
				{
					CString tmp=_T("");
					m_pSet_10.ChangeRsChar(_T("strName"), tmp);
					CString tmpd=_T("");
					CString m_Cycle=_T("");
					if(tmp==_T("Clean1"))
					{
						tmpd.Format(_T("%d"), m_Clean1);
						if(m_Clean1==2)
							m_Cycle.Format(_T("%d"), m_Clean1_Week.GetCurSel());
						else if(m_Clean1==3)
							m_Cycle.Format(_T("%d"), m_Clean1_Day.GetCurSel());
						else
							m_Cycle=_T("0");
					
					}
					else if(tmp==_T("Clean2"))
					{
						tmpd.Format(_T("%d"), m_Clean2);
						if(m_Clean2==2)
							m_Cycle.Format(_T("%d"), m_Clean2_Week.GetCurSel());
						else if(m_Clean2==3)
							m_Cycle.Format(_T("%d"), m_Clean2_Day.GetCurSel());
						else
							m_Cycle=_T("0");
					}
					else if(tmp==_T("Clean3"))
					{
						tmpd.Format(_T("%d"), m_Clean3);
						int m_Clean3=_wtoi(tmpd);
						if(m_Clean3==2)
							m_Cycle.Format(_T("%d"), m_Clean3_Week.GetCurSel());
						else if(m_Clean3==3)
							m_Cycle.Format(_T("%d"), m_Clean3_Day.GetCurSel());
						else
							m_Cycle=_T("0");
					}		
					CString strQuery;
					strQuery.Format(_T("Update PartChange SET strInstallDate = '%s', strExpectDate = '%s', strCycle = '%s' WHERE strName = '%s'"),
						tmpd, _T("0"),  m_Cycle, tmp);
					m_pSet_10.Update_DB(strQuery);
					m_pSet_10.MoveNext();
				}
			}
			if(m_pSet_10.b_IsOpenedDB)
				m_pSet_10.Close_DB();
		}
	}*/
}


void CDlgPage6::OnClickedClean10()
{
	m_Clean1=0;
	m_Clean1_Day.EnableWindow(false);
	m_Clean1_Week.EnableWindow(false);
}


void CDlgPage6::OnClean11()
{
	m_Clean1=1;
	m_Clean1_Day.EnableWindow(false);
	m_Clean1_Week.EnableWindow(false);
}


void CDlgPage6::OnClean12()
{
	m_Clean1=2;
	m_Clean1_Day.EnableWindow(false);
	m_Clean1_Week.EnableWindow(true);
}


void CDlgPage6::OnClean13()
{
	m_Clean1=3;
	m_Clean1_Day.EnableWindow(true);
	m_Clean1_Week.EnableWindow(false);
}


static CString m_StringWeek[] = {_T("일요일"), _T("월요일"), _T("화요일"), _T("수요일"), _T("목요일"), _T("금요일"), _T("토요일")};
static CString m_StringWeekEng[] = {_T("Sunday"), _T("Monday"), _T("Tuesday"), _T("Wednesday"), _T("Thursday"), _T("Friday"), _T("Saturday")};
static CString m_StringWeekChi[] = {_T("星期天"), _T("星期一"), _T("星期二"), _T("星期三"), _T("星期四"), _T("星期五"), _T("星期六")};
static CString m_StringWeekRus[] = { _T("Воскресенье"), _T("понедельник"), _T("вторник"), _T("среда"), _T("Четверг"), _T("Пятница"), _T("Суббота") };
static CString m_StringWeekUkr[] = { _T("Неділя"), _T("Понеділок"), _T("Вівторок"), _T("Середа"), _T("Четвер"), _T("П’ятниця"), _T("Субота") };
static CString m_StringWeekSpn[] = { _T("Domingo"), _T("Lunes"), _T("Martes"), _T("miércoles"), _T("Jueves"), _T("Viernes"), _T("Sábado") };
static CString m_StringWeekPol[] = { _T("niedziela"), _T("Poniedziałek"), _T("wtorek"), _T("Środa"), _T("Czwartek"), _T("piątek"), _T("sobota") };
static CString m_StringWeekTiw[] = { _T("星期日"), _T("星期一"), _T("星期二"), _T("星期三"), _T("星期四"), _T("星期五"), _T("星期六") };

void CDlgPage6::InitCombo()
{
	m_Clean1_Week.ResetContent();
	m_Clean2_Week.ResetContent();
	m_Clean3_Week.ResetContent();
	for(int i=0; i<7; i++)
	{
		if(m_RNationalCode==0)
		{
			m_Clean1_Week.AddString(m_StringWeek[i]);
			m_Clean2_Week.AddString(m_StringWeek[i]);
			m_Clean3_Week.AddString(m_StringWeek[i]);
		}
		if(m_RNationalCode==1)
		{
			m_Clean1_Week.AddString(m_StringWeekEng[i]);
			m_Clean2_Week.AddString(m_StringWeekEng[i]);
			m_Clean3_Week.AddString(m_StringWeekEng[i]);
		}
		else if(m_RNationalCode==2)
		{
			m_Clean1_Week.AddString(m_StringWeekChi[i]);
			m_Clean2_Week.AddString(m_StringWeekChi[i]);
			m_Clean3_Week.AddString(m_StringWeekChi[i]);
		}
		else if (m_RNationalCode == 3)
		{
			m_Clean1_Week.AddString(m_StringWeekRus[i]);
			m_Clean2_Week.AddString(m_StringWeekRus[i]);
			m_Clean3_Week.AddString(m_StringWeekRus[i]);
		}
		else if (m_RNationalCode == 4)
		{
			m_Clean1_Week.AddString(m_StringWeekUkr[i]);
			m_Clean2_Week.AddString(m_StringWeekUkr[i]);
			m_Clean3_Week.AddString(m_StringWeekUkr[i]);
		}
		else if (m_RNationalCode == 5)
		{
			m_Clean1_Week.AddString(m_StringWeekSpn[i]);
			m_Clean2_Week.AddString(m_StringWeekSpn[i]);
			m_Clean3_Week.AddString(m_StringWeekSpn[i]);
		}
		else if (m_RNationalCode == 6)
		{
			m_Clean1_Week.AddString(m_StringWeekPol[i]);
			m_Clean2_Week.AddString(m_StringWeekPol[i]);
			m_Clean3_Week.AddString(m_StringWeekPol[i]);
		}
		else if (m_RNationalCode == 7)
		{
			m_Clean1_Week.AddString(m_StringWeekTiw[i]);
			m_Clean2_Week.AddString(m_StringWeekTiw[i]);
			m_Clean3_Week.AddString(m_StringWeekTiw[i]);
		}
		
	}
	m_Clean1_Week.SetCurSel(0);
	m_Clean2_Week.SetCurSel(0);
	m_Clean3_Week.SetCurSel(0);


	CString day;
	for(int i=1; i<=31; i++)
	{
		if(m_RNationalCode==0)
			day.Format(_T("%d일"), i);
		if(m_RNationalCode==1)
			day.Format(_T("%dDay"), i);
		else if(m_RNationalCode==2)
			day.Format(_T("%d日"), i);
		else if (m_RNationalCode == 3)
			day.Format(_T("%dдень"), i);
		else if (m_RNationalCode == 4)
			day.Format(_T("%dдень"), i);
		else if (m_RNationalCode == 5)
			day.Format(_T("%dDía"), i);
		else if (m_RNationalCode == 6)
			day.Format(_T("%dDzień"), i);
		else if (m_RNationalCode == 7)
			day.Format(_T("%d日"), i);
		m_Clean1_Day.AddString(day);
		m_Clean2_Day.AddString(day);
		m_Clean3_Day.AddString(day);
	}

	m_Clean1_Day.SetCurSel(0);
	m_Clean2_Day.SetCurSel(0);
	m_Clean3_Day.SetCurSel(0);

}

void CDlgPage6::OnClickedClean20()
{
	m_Clean2=0;
	m_Clean2_Day.EnableWindow(false);
	m_Clean2_Week.EnableWindow(false);	
}


void CDlgPage6::OnClean21()
{
	m_Clean2=1;
	m_Clean2_Day.EnableWindow(false);
	m_Clean2_Week.EnableWindow(false);		
}


void CDlgPage6::OnClean22()
{
	m_Clean2=2;
	m_Clean2_Day.EnableWindow(false);
	m_Clean2_Week.EnableWindow(true);		
}


void CDlgPage6::OnClean23()
{
	m_Clean2=3;
	m_Clean2_Day.EnableWindow(true);
	m_Clean2_Week.EnableWindow(false);		
}


void CDlgPage6::OnClickedClean30()
{
	m_Clean3=0;
	m_Clean3_Day.EnableWindow(false);
	m_Clean3_Week.EnableWindow(false);		
}


void CDlgPage6::OnClean31()
{
	m_Clean3=1;
	m_Clean3_Day.EnableWindow(false);
	m_Clean3_Week.EnableWindow(false);		
}


void CDlgPage6::OnClean32()
{
	m_Clean3=2;
	m_Clean3_Day.EnableWindow(false);
	m_Clean3_Week.EnableWindow(true);		
}


void CDlgPage6::OnClean33()
{
	m_Clean3=3;
	m_Clean3_Day.EnableWindow(true);
	m_Clean3_Week.EnableWindow(false);		
}


void CDlgPage6::OnItemchangedListG(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	UpdateData(true);
	int num=pNMLV->iItem;
	temp_name=m_List_G.GetItemText(num,1);
	*pResult = 0;
}


void CDlgPage6::WriteReport()
{
	CFileFind   finder;
	int g=0;

	if(!g_Connecting)		
	{
		//AfxMessageBox(_T("실패"));
	}
	else
	{
		
		MYSQL_RES* m_Res;
		MYSQL_ROW row;
		mysql_query(&m_mysql,"set names euckr");
		SqlQuery.Format(_T("SELECT * FROM intellisense_db10"));
		
		if(mysql_query(&m_mysql,CStringA(SqlQuery)))
		{
			CString error(mysql_error(&m_mysql));
			WriteErrorLog(18, error);
		}
		else
		{
			m_Res=mysql_store_result(&m_mysql);
			CString name, install, cycle, expect;
			g=0;
			m_List_G.DeleteAllItems();
			while((row = mysql_fetch_row(m_Res)) != NULL)
			{
				CString strInstallDate (row[0]);
				CString strExpectDate (row[1]);
				CString strCycle (row[2]);
				CString strName (row[3]);
						
				if(strName!="Clean1" && strName!="Clean2" && strName!="Clean3")
				{
				//	
					m_List_G.InsertItem(g, _T(""));
					m_List_G.SetItemText(g, 1, strName);
					m_List_G.SetItemText(g, 2, strInstallDate);
					int y=(_wtoi(strInstallDate.Mid(5).Left(2))+_wtoi(strCycle))/12;
					int m=(_wtoi(strInstallDate.Mid(5).Left(2))+_wtoi(strCycle))%12;
					if(m==0)
					{
						y-=1;
						m=12;
					}
					COleDateTime OriDay;
					OriDay = COleDateTime(_wtoi(strInstallDate.Left(4))+y, m, _wtoi(strInstallDate.Mid(8).Left(2)), 0 ,0 ,0);
					expect=OriDay.Format(_T("%Y-%m-%d"));
					
					m_List_G.SetItemText(g, 3, expect);
					if(m_RNationalCode==0)
						m_List_G.SetItemText(g, 4, strCycle + " 개월");
					if(m_RNationalCode==1)
						m_List_G.SetItemText(g, 4, strCycle+" Months");
					else if(m_RNationalCode==2)
						m_List_G.SetItemText(g, 4, strCycle+" 个月");
					else if (m_RNationalCode == 3)
						m_List_G.SetItemText(g, 4, cycle + " месяцы");
					else if (m_RNationalCode == 4)
						m_List_G.SetItemText(g, 4, cycle + " місяців");
					else if (m_RNationalCode == 5)
						m_List_G.SetItemText(g, 4, cycle + " Meses");
					else if (m_RNationalCode == 6)
						m_List_G.SetItemText(g, 4, cycle + " miesiące");
					else if (m_RNationalCode == 7)
						m_List_G.SetItemText(g, 4, cycle + " 月");
					g++;
				}
				
			}
			mysql_free_result(m_Res);
		}
	}
	
	/*
    if(finder.FindFile(g_StrDirPath[10]))
	{
		m_List_G.DeleteAllItems();
		if(!m_pSet_10.Open_DB(g_StrDirPath[10]))		
		{
			
		}
		else
		{
			CString strSQL, strQuery;
			strSQL.Format(_T("SELECT * FROM PartChange"));
			if(m_pSet_10.Select_DB(strSQL))
			{
				if(m_pSet_10.IsEOF())
				{
				}
				else
				{
					CString name, install, cycle, expect;
					g=0;
					while(m_pSet_10.IsEOF() != 1)
					{
						m_pSet_10.ChangeRsChar(_T("strName"), name);
						m_pSet_10.ChangeRsChar(_T("strInstallDate"), install);
						m_pSet_10.ChangeRsChar(_T("strCycle"), cycle);
						if(name!="Clean1" && name!="Clean2" && name!="Clean3")
						{
							m_List_G.InsertItem(g, _T(""));
							m_List_G.SetItemText(g, 1, name);
							m_List_G.SetItemText(g, 2, install);
							int y=(_wtoi(install.Mid(5).Left(2))+_wtoi(cycle))/12;
							int m=(_wtoi(install.Mid(5).Left(2))+_wtoi(cycle))%12;
							if(m==0)
							{
								y-=1;
								m=12;
							}
							COleDateTime OriDay;
							OriDay = COleDateTime(_wtoi(install.Left(4))+y, m, _wtoi(install.Mid(8).Left(2)), 0 ,0 ,0);
							expect=OriDay.Format(_T("%Y-%m-%d"));
					
							m_List_G.SetItemText(g, 3, expect);
							if(m_RNationalCode==1)
								m_List_G.SetItemText(g, 4, cycle+" Months");
							else if(m_RNationalCode==2)
								m_List_G.SetItemText(g, 4, cycle+" 个月");
							else
								m_List_G.SetItemText(g, 4, cycle+" 개월");
							g++;
						}
						m_pSet_10.MoveNext();	
					}
				}
			}
			if(m_pSet_10.b_IsOpenedDB)
				m_pSet_10.Close_DB();
		}
	}*/
}


void CDlgPage6::LoadClean()
{
	CFileFind   finder;
	CString strQuery;

	if(!g_Connecting)		
	{
		//AfxMessageBox(_T("실패"));
	}
	else
	{
		
		MYSQL_RES* m_Res;
		MYSQL_ROW row;
		mysql_query(&m_mysql,"set names euckr");
		SqlQuery.Format(_T("SELECT * FROM intellisense_db10 WHERE strName LIKE 'Clean%%'"));
		
		if(mysql_query(&m_mysql,CStringA(SqlQuery)))
		{
			CString error(mysql_error(&m_mysql));
			WriteErrorLog(19, error);
		}
		else
		{
			m_Res=mysql_store_result(&m_mysql);
			int rowNum = 0;
			while(row = mysql_fetch_row(m_Res))
			{
				rowNum++;
				CString name=_T(""), install=_T(""), cycle;
				CString strInstallDate (row[0]);
				CString strExperctDate (row[1]);
				CString strCycle (row[2]);
				CString strName (row[3]);

				CButton* pButton;

				if(strName==_T("Clean1"))
				{
					m_Clean1=_wtoi(strInstallDate);
					if(m_Clean1==0)
						OnClickedClean10();
					else if(m_Clean1==1)
					{
						OnClean11();
						pButton = (CButton*)GetDlgItem(IDC_CLEAN1_0);
						pButton->SetCheck(false);

						pButton = (CButton*)GetDlgItem(IDC_CLEAN1_1);
						pButton->SetCheck(true);
					}
					else if(m_Clean1==2)
					{
						m_Clean1_Week.SetCurSel(_wtoi(strCycle));
						OnClean12();
						pButton = (CButton*)GetDlgItem(IDC_CLEAN1_0);
						pButton->SetCheck(false);

						pButton = (CButton*)GetDlgItem(IDC_CLEAN1_2);
						pButton->SetCheck(true);
					}
					else if(m_Clean1==3)
					{
						m_Clean1_Day.SetCurSel(_wtoi(strCycle));
						OnClean13();
						pButton = (CButton*)GetDlgItem(IDC_CLEAN1_0);
						pButton->SetCheck(false);

						pButton = (CButton*)GetDlgItem(IDC_CLEAN1_3);
						pButton->SetCheck(true);
					}
				}
				else if(strName==_T("Clean2"))
				{
					m_Clean2=_wtoi(strInstallDate);
					if(m_Clean2==0)
						OnClickedClean20();
					else if(m_Clean2==1)
					{
						OnClean21();
						pButton = (CButton*)GetDlgItem(IDC_CLEAN2_0);
						pButton->SetCheck(false);

						pButton = (CButton*)GetDlgItem(IDC_CLEAN2_1);
						pButton->SetCheck(true);
					}
					else if(m_Clean2==2)
					{
						m_Clean2_Week.SetCurSel(_wtoi(strCycle));
						OnClean22();
						pButton = (CButton*)GetDlgItem(IDC_CLEAN2_0);
						pButton->SetCheck(false);

						pButton = (CButton*)GetDlgItem(IDC_CLEAN2_2);
						pButton->SetCheck(true);
					}
					else if(m_Clean2==3)
					{
						m_Clean2_Day.SetCurSel(_wtoi(strCycle));
						OnClean23();
						pButton = (CButton*)GetDlgItem(IDC_CLEAN2_0);
						pButton->SetCheck(false);

						pButton = (CButton*)GetDlgItem(IDC_CLEAN2_3);
						pButton->SetCheck(true);
					}

				}
				else if(strName==_T("Clean3"))
				{
					m_Clean3=_wtoi(strInstallDate);
					if(m_Clean3==0)
						OnClickedClean30();
					else if(m_Clean3==1)
					{
						OnClean31();
						pButton = (CButton*)GetDlgItem(IDC_CLEAN3_0);
						pButton->SetCheck(false);

						pButton = (CButton*)GetDlgItem(IDC_CLEAN3_1);
						pButton->SetCheck(true);
					}
					else if(m_Clean3==2)
					{
						m_Clean3_Week.SetCurSel(_wtoi(strCycle));
						OnClean32();
						pButton = (CButton*)GetDlgItem(IDC_CLEAN3_0);
						pButton->SetCheck(false);

						pButton = (CButton*)GetDlgItem(IDC_CLEAN3_2);
						pButton->SetCheck(true);
					}
					else if(m_Clean3==3)
					{
						m_Clean3_Day.SetCurSel(_wtoi(strCycle));
						OnClean33();
						pButton = (CButton*)GetDlgItem(IDC_CLEAN3_0);
						pButton->SetCheck(false);

						pButton = (CButton*)GetDlgItem(IDC_CLEAN3_3);
						pButton->SetCheck(true);
					}
				}
			}

			if(rowNum == 0)
			{
				strQuery.Format(_T("Insert INTO intellisense_db10(strName, strInstallDate, strExpectDate, strCycle)") _T("VALUES('%s', '%s', '%s', '%s')"), _T("Clean1"), _T("0"), _T("0"), _T("0"));
				//m_pSet_10.Insert_DB(strQuery);
				if(mysql_query(&m_mysql,CStringA(strQuery)))
				{
					CString error(mysql_error(&m_mysql));
					WriteErrorLog(20, error);
				}
				strQuery.Format(_T("Insert INTO intellisense_db10(strName, strInstallDate, strExpectDate, strCycle)") _T("VALUES('%s', '%s', '%s', '%s')"), _T("Clean2"), _T("0"), _T("0"), _T("0"));
				//m_pSet_10.Insert_DB(strQuery);
				if(mysql_query(&m_mysql,CStringA(strQuery)))
				{
					CString error(mysql_error(&m_mysql));
					WriteErrorLog(21, error);
				}
				strQuery.Format(_T("Insert INTO intellisense_db10(strName, strInstallDate, strExpectDate, strCycle)") _T("VALUES('%s', '%s', '%s', '%s')"), _T("Clean3"), _T("0"), _T("0"), _T("0"));
			//	m_pSet_10.Insert_DB(strQuery);
				if(mysql_query(&m_mysql,CStringA(strQuery)))
				{
					CString error(mysql_error(&m_mysql));
					WriteErrorLog(22, error);
				}
			}

			mysql_free_result(m_Res);
		}
	}
			



	/*
	if(finder.FindFile(g_StrDirPath[10]))
	{
		if(!m_pSet_10.Open_DB(g_StrDirPath[10]))		
		{
			
		}
		else
		{
			CString strSQL, strQuery;
			strSQL.Format(_T("SELECT * FROM PartChange WHERE strName LIKE 'Clean%%'"));
			if(m_pSet_10.Select_DB(strSQL))
			{
				if(m_pSet_10.IsEOF())
				{
					strQuery.Format(_T("Insert INTO PartChange(strName, strInstallDate, strExpectDate, strCycle)") _T("VALUES('%s', '%s', '%s', '%s')"),
						_T("Clean1"), _T("0"), _T("0"), _T("0"));
					m_pSet_10.Insert_DB(strQuery);
					strQuery.Format(_T("Insert INTO PartChange(strName, strInstallDate, strExpectDate, strCycle)") _T("VALUES('%s', '%s', '%s', '%s')"),
						_T("Clean2"), _T("0"), _T("0"), _T("0"));
					m_pSet_10.Insert_DB(strQuery);
					strQuery.Format(_T("Insert INTO PartChange(strName, strInstallDate, strExpectDate, strCycle)") _T("VALUES('%s', '%s', '%s', '%s')"),
						_T("Clean3"), _T("0"), _T("0"), _T("0"));
					m_pSet_10.Insert_DB(strQuery);
				}
				else
				{
					while(m_pSet_10.IsEOF() != 1)
					{
						CString name=_T(""), install=_T(""), cycle;
						m_pSet_10.ChangeRsChar(_T("strName"), name);
						m_pSet_10.ChangeRsChar(_T("strInstallDate"), install);
						m_pSet_10.ChangeRsChar(_T("strCycle"), cycle);
						if(name==_T("Clean1"))
						{
							m_Clean1=_wtoi(install);
							if(m_Clean1==0)
								OnClickedClean10();
							else if(m_Clean1==1)
								OnClean11();
							if(m_Clean1==2)
							{
								m_Clean1_Week.SetCurSel(_wtoi(cycle));
								OnClean12();
							}
							else if(m_Clean1==3)
							{
								m_Clean1_Day.SetCurSel(_wtoi(cycle));
								OnClean13();
							}
						}
						else if(name==_T("Clean2"))
						{
							m_Clean2=_wtoi(install);
							if(m_Clean2==0)
								OnClickedClean20();
							else if(m_Clean2==1)
								OnClean21();
							else if(m_Clean2==2)
							{
								m_Clean2_Week.SetCurSel(_wtoi(cycle));
								OnClean22();
							}
							else if(m_Clean2==3)
							{
								m_Clean2_Day.SetCurSel(_wtoi(cycle));
								OnClean23();
							}

						}
						else if(name==_T("Clean3"))
						{
							m_Clean3=_wtoi(install);
							if(m_Clean3==0)
								OnClickedClean30();
							else if(m_Clean3==1)
								OnClean31();
							else if(m_Clean3==2)
							{
								m_Clean3_Week.SetCurSel(_wtoi(cycle));
								OnClean32();
							}
							else if(m_Clean3==3)
							{
								m_Clean3_Day.SetCurSel(_wtoi(cycle));
								OnClean33();
							}
						}
						m_pSet_10.MoveNext();	
					}
				}
			}
			UpdateData(false);
			if(m_pSet_10.b_IsOpenedDB)
				m_pSet_10.Close_DB();
		}
	}*/

}
