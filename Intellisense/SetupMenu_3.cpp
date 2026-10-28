// SetupMenu_3.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "SetupMenu_3.h"
#include "afxdialogex.h"

#include "MessageBoxDlg.h"
// CSetupMenu_3 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMenu_3, CDialogEx)

CSetupMenu_3::CSetupMenu_3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMenu_3::IDD, pParent)
{

	m_SetupSaveFile = 0;
	m_SetupSaveHard = 0;
	m_DelOptionD_1 = 0;
	m_DelOptionD_2 = 0;
	m_DelOptionE_2 = 0;
	m_DelOptionE_1 = 0;
}

CSetupMenu_3::~CSetupMenu_3()
{
}

void CSetupMenu_3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_SAVE, m_Bt_Save);
	DDX_Control(pDX, IDCANCEL, m_Bt_Close);
	DDX_Radio(pDX, IDC_SETUPSAVEFILE, m_SetupSaveFile);
	DDX_Radio(pDX, IDC_SETUPSAVEHARD, m_SetupSaveHard);
	DDX_Check(pDX, IDC_APPLYALL, b_ApplyAll);
	DDX_Control(pDX, IDC_AUTODEL_PER, m_ComboD_1);
	DDX_Control(pDX, IDC_AUTODEL_PER2, m_ComboE_1);
	DDX_Control(pDX, IDC_AUTODEL_MONTH, m_ComboD_2);
	DDX_Control(pDX, IDC_AUTODEL_MONTH2, m_ComboE_2);
	DDX_Check(pDX, IDC_CHECK_AUTODEL, m_DelOptionD);
	DDX_Check(pDX, IDC_CHECK_AUTODEL2, m_DelOptionE);
	DDX_CBIndex(pDX, IDC_AUTODEL_PER, m_DelOptionD_1);
	DDX_CBIndex(pDX, IDC_AUTODEL_MONTH, m_DelOptionD_2);
	DDX_CBIndex(pDX, IDC_AUTODEL_MONTH2, m_DelOptionE_2);
	DDX_CBIndex(pDX, IDC_AUTODEL_PER2, m_DelOptionE_1);
	DDX_Control(pDX, IDC_COMBO1, m_ViewControl); //20171115 영상 강제 보정
	DDX_CBIndex(pDX, IDC_COMBO1, m_ViewControl2); //20171115 영상 강제 보정

	DDX_Check(pDX, IDC_ACCEPT, m_accet);
}


BEGIN_MESSAGE_MAP(CSetupMenu_3, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BT_SAVE, &CSetupMenu_3::OnBnClickedBtSave)

END_MESSAGE_MAP()


// CSetupMenu_3 메시지 처리기입니다.


BOOL CSetupMenu_3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Brush.CreateStockObject(NULL_BRUSH);
	SetWindowPos(NULL, 672, 119, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	LoadBackground();
	LoadValue();

	//전체영상 저장기능 개선 20171226
	if(g_ExHardUse == false)
	{
		GetDlgItem(IDC_SETUPSAVEHARD)->ShowWindow(false);
		GetDlgItem(IDC_SETUPSAVEHARD1)->ShowWindow(false);
		GetDlgItem(IDC_CHECK_AUTODEL2)->ShowWindow(false);
		GetDlgItem(IDC_AUTODEL_PER2)->ShowWindow(false);
		GetDlgItem(IDC_AUTODEL_MONTH2)->ShowWindow(false);
	}
	else
	{
		GetDlgItem(IDC_SETUPSAVEHARD)->ShowWindow(true);
		GetDlgItem(IDC_SETUPSAVEHARD1)->ShowWindow(true);
		GetDlgItem(IDC_CHECK_AUTODEL2)->ShowWindow(true);
		GetDlgItem(IDC_AUTODEL_PER2)->ShowWindow(true);
		GetDlgItem(IDC_AUTODEL_MONTH2)->ShowWindow(true);
	}
	//전체영상 저장기능 개선 20171226


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSetupMenu_3::LoadBackground()
{
	BITMAP bmpInfo;
	if(g_NationalCode==0)
	{
			//전체영상 저장 개선 20171226
		if(g_ExHardUse == true)
			m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_3); 
		else if(g_ExHardUse == false)
			m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_31);
		//전체영상 저장 개선 20171226

		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE,IDB_BTSETUPMENU_SAVE_1);
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT,IDB_SELPRODUCT_EXIT_0); 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE,IDB_BTSETUPMENU_SAVE_1);
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT,IDB_SELPRODUCT_EXIT_0); 
	}
	else if(g_NationalCode==1)
	{
		//전체영상 저장 개선 20171226
		if(g_ExHardUse == true)
			m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_3_ENG); 
		else if(g_ExHardUse == false)
			m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_31_ENG);
		//전체영상 저장 개선 20171226
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_ENG,IDB_BTSETUPMENU_SAVE_1_ENG); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_ENG,IDB_SELPRODUCT_EXIT_0_ENG); 
	}
	else if(g_NationalCode==2)
	{
		//전체영상 저장 개선 20171226
			if(g_ExHardUse == true)
			m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_3_CHI); 
		else if(g_ExHardUse == false)
			m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_31_CHI);
			//전체영상 저장 개선 20171226
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_CHI,IDB_BTSETUPMENU_SAVE_1_CHI); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_CHI,IDB_SELPRODUCT_EXIT_0_CHI); 
	}
	else if(g_NationalCode==3)
	{
		//전체영상 저장 개선 20171226
			if(g_ExHardUse == true)
			m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_3_SPN); 
		else if(g_ExHardUse == false)
			m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_31_SPN);
			//전체영상 저장 개선 20171226
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_SPN,IDB_BTSETUPMENU_SAVE_1_SPN); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_SPN,IDB_SELPRODUCT_EXIT_0_SPN); 
	}
    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 	

	m_Bt_Save.SizeToContent();
	m_Bt_Close.SizeToContent();
}

BOOL CSetupMenu_3::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSetupMenu_3::OnPaint()
{
	CPaintDC dc(this); // device context for painting
    CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


void CSetupMenu_3::LoadValue(void)
{
	int i=0;
	CString temp;
	for(i=0; i<=100; i++)
	{
		temp.Format(_T("%d"), i);
		m_ComboD_1.AddString(temp);
		m_ComboE_1.AddString(temp);
		if(i<=36)
		{
			m_ComboD_2.AddString(temp);
			m_ComboE_2.AddString(temp);
		}
	}

	//영상강제 보정 20171115
	if(g_NationalCode == 0)
	{
		m_ViewControl.AddString(_T("원본"));
		m_ViewControl.AddString(_T("1단계"));
		m_ViewControl.AddString(_T("2단계"));
		m_ViewControl.AddString(_T("3단계"));
	}
	else if(g_NationalCode == 1)
	{
		m_ViewControl.AddString(_T("Original"));
		m_ViewControl.AddString(_T("1Step"));
		m_ViewControl.AddString(_T("2Step"));
		m_ViewControl.AddString(_T("3Step"));
	}
	else if(g_NationalCode == 2)
	{
		m_ViewControl.AddString(_T("原来"));
		m_ViewControl.AddString(_T("步骤1"));
		m_ViewControl.AddString(_T("步骤2"));
		m_ViewControl.AddString(_T("步骤3"));
	}
	else if(g_NationalCode == 3)
	{
		m_ViewControl.AddString(_T("Original"));
		m_ViewControl.AddString(_T("1Paso"));
		m_ViewControl.AddString(_T("2Paso"));
		m_ViewControl.AddString(_T("3Paso"));
	}
	//영상강제 보정 20171115




	m_SetupSaveFile = g_SaveFile;
	m_SetupSaveHard = g_SaveHard;

	m_DelOptionD = g_bOptionD;
	m_DelOptionD_1 = g_bOptionD_1;
	m_DelOptionD_2 = g_bOptionD_2;

	m_DelOptionE = g_bOptionE;
	m_DelOptionE_1 = g_bOptionE_1;
	m_DelOptionE_2 = g_bOptionE_2;

	m_ViewControl2 = g_ViewControl; //영상 강제 보정 20171115

	m_accet = g_Accept; //영상 보정 검사 적용 20171130

	m_ComboD_1.SetCurSel(m_DelOptionD_1);
	m_ComboD_2.SetCurSel(m_DelOptionD_2);
	m_ComboD_1.SetCurSel(m_DelOptionE_1);
	m_ComboD_2.SetCurSel(m_DelOptionE_2);

	m_ViewControl.SetCurSel(m_ViewControl2); //영상강제 보정 20171115

	UpdateData(false);
}


HBRUSH CSetupMenu_3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

   if(nCtlColor == CTLCOLOR_STATIC) 
	{
        pOldBrush = (CBrush*)pDC->SelectObject(&Brush);
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH) Brush;
    }

	return hbr;
}


void CSetupMenu_3::OnWriteReport(void)
{
	CString strdate, strtime;
	CTime time;
	time = CTime::GetCurrentTime();
	strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());		
	strtime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), time.GetYear(),time.GetMonth(),time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	
	CString strtext;
	if(g_SaveFile==1 && g_SaveHard==1)
		strtext.Format(_T("%s%s"), g_ProductName, g_MessageArr[171]);
	else if(g_SaveFile==0 && g_SaveHard==1)
		strtext.Format(_T("%s%s"), g_ProductName, g_MessageArr[172]);
	else if(g_SaveFile==1 && g_SaveHard==0)
		strtext.Format(_T("%s%s"), g_ProductName, g_MessageArr[173]);
	else 
		strtext.Format(_T("%s%s"), g_ProductName, g_MessageArr[174]);

	
	bool g_bOpenDB=true;

	Init_Mysql();

	if (!g_Connecting)
		AfxMessageBox(_T("실패"));
	else
	{
		CString strQuery;
		strQuery.Format(_T("Insert INTO intellisense_db7(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
			strdate, strtext, strtime);
		if (mysql_query(&m_mysql, CStringA(strQuery))) {
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}
	}

	Close_Mysql();

	//if(!m_pSet_7.Open_DB(g_StrDirPath[7]))		
	//{	
	//	g_bOpenDB=false;
	//	//RecoveryDatabase(g_StrDirPath[7], g_StrDirPath_copy[7]);
	//	if(m_pSet_7.Open_DB(g_StrDirPath[7]))		
	//		g_bOpenDB=true;
	//}
	//if(g_bOpenDB)
	//{
	//	CString strQuery;
	//	strQuery.Format(_T("Insert INTO Computer_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
	//		strdate, strtext, strtime);
	//	m_pSet_7.Insert_DB(strQuery);
	//	if(m_pSet_7.b_IsOpenedDB)
	//		m_pSet_7.Close_DB();
	//}
}



void CSetupMenu_3::OnWriteReportAll(void)
{
	CString strdate, strtime;
	CTime time;
	time = CTime::GetCurrentTime();
	strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());		
	strtime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), time.GetYear(),time.GetMonth(),time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	
	CString strtext;
	if(g_SaveFile==1 && g_SaveHard==1)
		strtext.Format(g_MessageArr[175]);//_T("전제품(NG저장안함, 전체저장안함)변경"));
	else if(g_SaveFile==0 && g_SaveHard==1)
		strtext.Format(g_MessageArr[176]);//_T("전제품(NG저장, 전체저장안함)변경"));
	else if(g_SaveFile==1 && g_SaveHard==0)
		strtext.Format(g_MessageArr[177]);//_T("전제품(NG저장안함, 전체저장)변경"));
	else 
		strtext.Format(g_MessageArr[178]);//_T("전제품(NG저장, 전체저장)변경"));

	bool g_bOpenDB=true;

	Init_Mysql();

	if (!g_Connecting)
		AfxMessageBox(_T("실패"));
	else
	{
		CString strQuery;
		strQuery.Format(_T("Insert INTO intellisense_db7(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
			strdate, strtext, strtime);
		if (mysql_query(&m_mysql, CStringA(strQuery))) {
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}
	}

	Close_Mysql();

	//if(!m_pSet_7.Open_DB(g_StrDirPath[7]))		
	//{	
	//	g_bOpenDB=false;
	//	//RecoveryDatabase(g_StrDirPath[7], g_StrDirPath_copy[7]);
	//	if(m_pSet_7.Open_DB(g_StrDirPath[7]))		
	//		g_bOpenDB=true;
	//}
	//if(g_bOpenDB)
	//{
	//	CString strQuery;
	//	strQuery.Format(_T("Insert INTO Computer_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
	//		strdate, strtext, strtime);
	//	m_pSet_7.Insert_DB(strQuery);
	//	if(m_pSet_7.b_IsOpenedDB)
	//		m_pSet_7.Close_DB();
	//}
}


void CSetupMenu_3::OnBnClickedBtSave()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	
	
	if(g_SaveFile == m_SetupSaveFile && g_SaveHard == m_SetupSaveHard && !b_ApplyAll &&
		m_DelOptionD == g_bOptionD && m_DelOptionD_1 == g_bOptionD_1 && m_DelOptionD_2 == g_bOptionD_2 &&
	//	m_DelOptionE == g_bOptionE && m_DelOptionE_1 == g_bOptionE_1 && m_DelOptionE_2 == g_bOptionE_2)
		m_DelOptionE == g_bOptionE && m_DelOptionE_1 == g_bOptionE_1 && m_DelOptionE_2 == g_bOptionE_2 && m_ViewControl2 == g_ViewControl && m_accet == g_Accept) //영상 강제 보정 20171115
		return;
	else
	{
		g_SaveFile = m_SetupSaveFile;
		if(m_SetupSaveHard==0)
		{
			UINT Diskcheck;
			CString disktype = _T("E:\\");
			Diskcheck = GetDriveType(disktype);

			if(Diskcheck == DRIVE_FIXED)
				g_SaveHard = m_SetupSaveHard;
			else
			{
				CMessageBoxDlg dlg;
				dlg.m_Message = g_MessageArr[179];//_T("외장하드디스크가 없습니다. NSE로 문의 바랍니다.");
				dlg.DoModal();
				g_SaveHard = 1;
			}
		}
		else
			g_SaveHard = m_SetupSaveHard;

		g_bOptionD = m_DelOptionD;
		g_bOptionD_1 = m_DelOptionD_1;
		g_bOptionD_2 = m_DelOptionD_2;

		g_bOptionE = m_DelOptionE;
		g_bOptionE_1 = m_DelOptionE_1;
		g_bOptionE_2 = m_DelOptionE_2;

		g_ViewControl = m_ViewControl2; //영상 강제 보정 20171115

		g_Accept = m_accet;  //영상보정 검사 적용 20171130
		
		Init_Mysql();

		if (b_ApplyAll)
		{
			if (!g_Connecting)
				AfxMessageBox(_T("실패"));
			else
			{
				MYSQL_RES* m_Res;
				MYSQL_ROW row;

				CString strSQL;
				strSQL.Format(_T("SELECT * FROM intellisense_product"));
				if (mysql_query(&m_mysql, CStringA(strSQL)))
				{
					CString error(mysql_error(&m_mysql));
					AfxMessageBox(error);
				}

				m_Res = mysql_store_result(&m_mysql);

				if ((row = mysql_fetch_row(m_Res)) == NULL)
					return;
				else
				{
					int m_count = 0;
					mysql_data_seek(m_Res, m_count);

					while ((row = mysql_fetch_row(m_Res)) != NULL)
					{
						CString strQuery;
						strQuery.Format(_T("Update intellisense_product SET ProductSaveFile = %d, ProductSaveHard = %d, ProductDelD = %d, ProductDelD_1 = %d, ProductDelD_2 = %d, ProductDelE = %d, ProductDelE_1 = %d, ProductDelE_2 = %d, ProductEtc13 = %d, ProductEtc14 = %d"), g_SaveFile, g_SaveHard, g_bOptionD, g_bOptionD_1, g_bOptionD_2, g_bOptionE, g_bOptionE_1, g_bOptionE_2, g_ViewControl, g_Accept); //영상 강제 보정 20171115
						if (mysql_query(&m_mysql, CStringA(strQuery))) {
							CString error(mysql_error(&m_mysql));
							AfxMessageBox(error);
						}

						m_count++;
						mysql_data_seek(m_Res, m_count);
					}

					OnWriteReportAll();
				}
				mysql_free_result(m_Res);
			}
		}
		else
		{
			if (!g_Connecting)
				AfxMessageBox(_T("실패"));
			else
			{
				CString strQuery;
				strQuery.Format(_T("Update intellisense_product SET ProductSaveFile = %d, ProductSaveHard = %d, ProductDelD = %d, ProductDelD_1 = %d, ProductDelD_2 = %d, ProductDelE = %d, ProductDelE_1 = %d, ProductDelE_2 = %d, ProductEtc13 = %d, ProductEtc14 = %d WHERE ProductName = '%s'"),
					g_SaveFile, g_SaveHard, g_bOptionD, g_bOptionD_1, g_bOptionD_2, g_bOptionE, g_bOptionE_1, g_bOptionE_2, g_ViewControl, g_Accept, g_ProductName);  //영상강제보정 20171115
				if (mysql_query(&m_mysql, CStringA(strQuery))) {
					CString error(mysql_error(&m_mysql));
					AfxMessageBox(error);
				}

				OnWriteReport();
			}
		}

		Close_Mysql();


		//if(b_ApplyAll)
		//{
		//	if(!m_pSet_p.Open_DB(g_StrDirPath[0]))		
		//	{
		//		
		//	}
		//	else
		//	{
		//		CString strSQL;
		//		strSQL.Format(_T("SELECT * FROM Product"));
		//		if(m_pSet_p.Select_DB(strSQL))
		//		{
		//			if(m_pSet_p.IsEOF())
		//			{
		//				if(m_pSet_p.b_IsOpenedDB)
		//					m_pSet_p.Close_DB();
		//				return;
		//			}
		//			else
		//			{
		//				m_pSet_p.MoveFirst();
		//				while(m_pSet_p.IsEOF()!=1)
		//				{
		//					CString strQuery;
		//				//	strQuery.Format(_T("Update Product SET ProductSaveFile = %d, ProductSaveHard = %d, ProductDelD = %d, ProductDelD_1 = %d, ProductDelD_2 = %d, ProductDelE = %d, ProductDelE_1 = %d, ProductDelE_2 = %d, ProductEtc7 = %d, ProductEtc8 = %d"), g_SaveFile, g_SaveHard, g_bOptionD, g_bOptionD_1, g_bOptionD_2, g_bOptionE, g_bOptionE_1, g_bOptionE_2);
		//					strQuery.Format(_T("Update Product SET ProductSaveFile = %d, ProductSaveHard = %d, ProductDelD = %d, ProductDelD_1 = %d, ProductDelD_2 = %d, ProductDelE = %d, ProductDelE_1 = %d, ProductDelE_2 = %d, ProductEtc13 = %d, ProductEtc14 = %d"), g_SaveFile, g_SaveHard, g_bOptionD, g_bOptionD_1, g_bOptionD_2, g_bOptionE, g_bOptionE_1, g_bOptionE_2, g_ViewControl, g_Accept); //영상 강제 보정 20171115
		//					m_pSet_p.Update_DB(strQuery);
		//					m_pSet_p.MoveNext();
		//				}
		//			}
		//			if(m_pSet_p.b_IsOpenedDB)
		//				m_pSet_p.Close_DB();
		//			OnWriteReportAll();
		//		}
		//	}
		//}
		//else
		//{
		//	if(!m_pSet_p.Open_DB(g_StrDirPath[0]))		
		//	{
		//		
		//	}
		//	else
		//	{
		//		CString strQuery;
		//	//	strQuery.Format(_T("Update Product SET ProductSaveFile = %d, ProductSaveHard = %d, ProductDelD = %d, ProductDelD_1 = %d, ProductDelD_2 = %d, ProductDelE = %d, ProductDelE_1 = %d, ProductDelE_2 = %d  WHERE ProductName = '%s'"),
		//	//		g_SaveFile, g_SaveHard, g_bOptionD, g_bOptionD_1, g_bOptionD_2,	g_bOptionE, g_bOptionE_1, g_bOptionE_2, g_ProductName);
		//		strQuery.Format(_T("Update Product SET ProductSaveFile = %d, ProductSaveHard = %d, ProductDelD = %d, ProductDelD_1 = %d, ProductDelD_2 = %d, ProductDelE = %d, ProductDelE_1 = %d, ProductDelE_2 = %d,ProductEtc13 = %d, ProductEtc14 = %d  WHERE ProductName = '%s'"),
		//			g_SaveFile, g_SaveHard, g_bOptionD, g_bOptionD_1, g_bOptionD_2,	g_bOptionE, g_bOptionE_1, g_bOptionE_2, g_ViewControl, g_Accept, g_ProductName);  //영상강제보정 20171115
		//		m_pSet_p.Update_DB(strQuery);
		//	
		//	
		//		if(m_pSet_p.b_IsOpenedDB)
		//			m_pSet_p.Close_DB();
		//		OnWriteReport();
		//	}
		//}
	}		
}
