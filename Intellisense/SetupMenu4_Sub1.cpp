// SetupMenu4_Sub1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "SetupMenu4_Sub1.h"
#include "afxdialogex.h"
#include "MessageBoxDlg.h"
#include "SetupMenu4_Sub3.h"
#include "SetupMenu_4_Admin.h"
#include "MainFrm.h"
#include "ChildFrm.h"
//20200331
// CSetupMenu4_Sub1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMenu4_Sub1, CDialogEx)

CSetupMenu4_Sub1::CSetupMenu4_Sub1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMenu4_Sub1::IDD, pParent)
	, L_ID(_T(""))
	, L_PW(_T(""))
	, L_Level(_T(""))
	, Message1(_T(""))
	, Level_1(_T(""))
	, Level_2(_T(""))
	, Level_3(_T(""))
{

	m_Language = g_NationalCode;
	m_AdminLanguage = g_NationalCode;
	b_IsAdminChanged[1]=false;
}

CSetupMenu4_Sub1::~CSetupMenu4_Sub1()
{
}

void CSetupMenu4_Sub1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RLANGUAGE, m_Language);
	DDX_Control(pDX, IDC_LANGCOMBO, m_National);
	DDX_Control(pDX, IDC_LIST_ID, m_ListID);

	DDX_Control(pDX, IDC_ADD_ID, m_List_Add);
	DDX_Control(pDX, IDC_DEL_ID, m_List_Del);
	DDX_Control(pDX, IDC_REVICE_ID, m_List_Revise);
}


BEGIN_MESSAGE_MAP(CSetupMenu4_Sub1, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RLANGUAGE, &CSetupMenu4_Sub1::OnClickedRlanguage)
	ON_COMMAND(IDC_RLANGUAGE1, &CSetupMenu4_Sub1::OnRlanguage1)
	ON_COMMAND(IDC_RLANGUAGE2, &CSetupMenu4_Sub1::OnRlanguage2)
	ON_CBN_SELCHANGE(IDC_LANGCOMBO, &CSetupMenu4_Sub1::OnCbnSelchangeLangcombo)
	ON_BN_CLICKED(IDC_ADD_ID, &CSetupMenu4_Sub1::OnBnClickedAddId)
	ON_BN_CLICKED(IDC_REVICE_ID, &CSetupMenu4_Sub1::OnBnClickedReviceId)
	ON_BN_CLICKED(IDC_DEL_ID, &CSetupMenu4_Sub1::OnBnClickedDelId)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ID, &CSetupMenu4_Sub1::OnLvnItemchangedListId)
END_MESSAGE_MAP()


// CSetupMenu4_Sub1 메시지 처리기입니다.


void CSetupMenu4_Sub1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}


BOOL CSetupMenu4_Sub1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255,255,255)); 
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadBackground();
	Init_List();
	Load_List();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CSetupMenu4_Sub1::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;	

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSetupMenu4_Sub1::LoadBackground()
{
	SetDlgItemText(IDC_STATIC_1, g_MessageArr[180]);//_T("프로그램 적용 언어 선택"));
	SetDlgItemText(IDC_RLANGUAGE, g_MessageArr[181]);//_T("국문(Korean)"));
	SetDlgItemText(IDC_RLANGUAGE1, g_MessageArr[182]);//_T("영문(English)"));
	SetDlgItemText(IDC_RLANGUAGE2, g_MessageArr[183]);//_T("중문(Chinese)"));
	SetDlgItemText(IDC_RLANGUAGE3, g_MessageArr[239]);//_T("러시아어(Russian)"));
	SetDlgItemText(IDC_RLANGUAGE4, g_MessageArr[240]);//_T("우크라이나어(Ukraine)"));

	m_National.AddString(g_MessageArr[181]); //_T("국문(Korean)"));
	m_National.AddString(g_MessageArr[182]); //_T("영문(English)"));
	m_National.AddString(g_MessageArr[183]); //_T("중문(Chinese)"));
	m_National.AddString(g_MessageArr[239]); //_T("러시아어(Russian)"));
	m_National.AddString(g_MessageArr[240]);//_T("우크라이나어(Ukraine)"));
	m_National.AddString(g_MessageArr[238]);//_T("스페인어(Spanish)"));
	m_National.AddString(g_MessageArr[241]); //폴란드어
	m_National.AddString(g_MessageArr[243]); //대만어


	//SetDlgItemText(IDC_STATIC_1, g_MessageArr[180]);//_T("프로그램 적용 언어 선택"));
	if(g_NationalCode == 0)
	{
		m_List_Add.LoadBitmaps(IDB_SELPRODUCT_ADD,IDB_SELPRODUCT_ADD_0); 
		m_List_Add.SizeToContent();	
		m_List_Del.LoadBitmaps(IDB_SELPRODUCT_DEL,IDB_SELPRODUCT_DEL_0); 
		m_List_Del.SizeToContent();	
		m_List_Revise.LoadBitmaps(IDB_SELECTPRODUCT_REVISE,IDB_SELECTPRODUCT_REVISE_1); 
		SetDlgItemText(IDC_STATIC_16, _T("계정관리"));
		L_ID = _T("아이디");
		L_PW = _T("비밀번호");
		L_Level = _T("등급");
		Message1 = _T("가 삭제 되었습니다.");
		Level_1 = _T("최고관리자");
		Level_2 = _T("관리자");
		Level_3 = _T("작업자");
	}
	else if(g_NationalCode == 1)
	{
		m_List_Add.LoadBitmaps(IDB_SELPRODUCT_ADD_ENG,IDB_SELPRODUCT_ADD_0_ENG); 
		m_List_Add.SizeToContent();	
		m_List_Del.LoadBitmaps(IDB_SELPRODUCT_DEL_ENG,IDB_SELPRODUCT_DEL_0_ENG); 
		m_List_Del.SizeToContent();	
		m_List_Revise.LoadBitmaps(IDB_SELECTPRODUCT_REVISE_ENG,IDB_SELECTPRODUCT_REVISE_1_ENG); 
		SetDlgItemText(IDC_STATIC_16, _T("Account management"));
		L_ID = _T("ID");
		L_PW = _T("PassWord");
		L_Level = _T("Level");
		Message1 = _T("has been deleted.");
		Level_1 = _T("admin");
		Level_2 = _T("manager");
		Level_3 = _T("worker");
	}
	else if(g_NationalCode == 2)
	{
		m_List_Add.LoadBitmaps(IDB_SELPRODUCT_ADD_CHI,IDB_SELPRODUCT_ADD_0_CHI); 
		m_List_Add.SizeToContent();	
		m_List_Del.LoadBitmaps(IDB_SELPRODUCT_DEL_CHI,IDB_SELPRODUCT_DEL_0_CHI); 
		m_List_Del.SizeToContent();	
		m_List_Revise.LoadBitmaps(IDB_SELECTPRODUCT_REVISE_CHI,IDB_SELECTPRODUCT_REVISE_1_CHI); 
		SetDlgItemText(IDC_STATIC_16, _T("帐户管理"));
		L_ID = _T("姓名");
		L_PW = _T("密码");
		L_Level = _T("等级");
		Message1 = _T("已被删除");
		Level_1 = _T("超级管理员");
		Level_2 = _T("经理");
		Level_3 = _T("工人");
	}
	else if(g_NationalCode == 3)
	{
		m_List_Add.LoadBitmaps(IDB_SELPRODUCT_ADD_RUS,IDB_SELPRODUCT_ADD_0_RUS); 
		m_List_Add.SizeToContent();	
		m_List_Del.LoadBitmaps(IDB_SELPRODUCT_DEL_RUS,IDB_SELPRODUCT_DEL_0_RUS); 
		m_List_Del.SizeToContent();	
		m_List_Revise.LoadBitmaps(IDB_SELECTPRODUCT_REVISE_RUS,IDB_SELECTPRODUCT_REVISE_1_RUS); 
		SetDlgItemText(IDC_STATIC_16, _T("управление аккаунтом"));
		L_ID = _T("Я БЫ");
		L_PW = _T("Пароль");
		L_Level = _T("Уровень");
		Message1 = _T("был удален.");
		Level_1 = _T("администратор");
		Level_2 = _T("управляющий делами");
		Level_3 = _T("рабочий");
	}
	else if(g_NationalCode == 4)
	{
		m_List_Add.LoadBitmaps(IDB_SELPRODUCT_ADD_UKR,IDB_SELPRODUCT_ADD_0_UKR); 
		m_List_Add.SizeToContent();	
		m_List_Del.LoadBitmaps(IDB_SELPRODUCT_DEL_UKR,IDB_SELPRODUCT_DEL_0_UKR); 
		m_List_Del.SizeToContent();	
		m_List_Revise.LoadBitmaps(IDB_SELECTPRODUCT_REVISE_UKR,IDB_SELECTPRODUCT_REVISE_1_UKR); 
		SetDlgItemText(IDC_STATIC_16, _T("управління рахунками"));
		L_ID = _T("Ім'я");
		L_PW = _T("Пароль");
		L_Level = _T("Рівень");
		Message1 = _T("було видалено.");
		Level_1 = _T("адмін");
		Level_2 = _T("менеджер");
		Level_3 = _T("працівник");
	}
	else if(g_NationalCode == 5)
	{
		m_List_Add.LoadBitmaps(IDB_SELPRODUCT_ADD_SPN,IDB_SELPRODUCT_ADD_0_SPN); 
		m_List_Add.SizeToContent();	
		m_List_Del.LoadBitmaps(IDB_SELPRODUCT_DEL_SPN,IDB_SELPRODUCT_DEL_0_SPN); 
		m_List_Del.SizeToContent();	
		m_List_Revise.LoadBitmaps(IDB_SELECTPRODUCT_REVISE_SPN,IDB_SELECTPRODUCT_REVISE_1_SPN); 
		SetDlgItemText(IDC_STATIC_16, _T("administración de cuentas"));
		L_ID = _T("nombre");
		L_PW = _T("Clave");
		L_Level = _T("Nivel");
		Message1 = _T("ha sido eliminado.");
		Level_1 = _T("administración");
		Level_2 = _T("gerente");
		Level_3 = _T("trabajador");
	}
	else if(g_NationalCode == 6)
	{
		m_List_Add.LoadBitmaps(IDB_SELPRODUCT_ADD_POL,IDB_SELPRODUCT_ADD_0_POL); 
		m_List_Add.SizeToContent();	
		m_List_Del.LoadBitmaps(IDB_SELPRODUCT_DEL_POL,IDB_SELPRODUCT_DEL_0_POL); 
		m_List_Del.SizeToContent();	
		m_List_Revise.LoadBitmaps(IDB_SELECTPRODUCT_REVISE_POL,IDB_SELECTPRODUCT_REVISE_1_POL); 
		SetDlgItemText(IDC_STATIC_16, _T("Zarządzanie kontem"));
		L_ID = _T("imię");
		L_PW = _T("Hasło");
		L_Level = _T("Poziom");
		Message1 = _T("zostało usunięte.");
		Level_1 = _T("Admin");
		Level_2 = _T("menedżer");
		Level_3 = _T("pracownik");
	}
	else if(g_NationalCode == 7)
	{
		m_List_Add.LoadBitmaps(IDB_SELPRODUCT_ADD_TIW,IDB_SELPRODUCT_ADD_0_TIW); 
		m_List_Add.SizeToContent();	
		m_List_Del.LoadBitmaps(IDB_SELPRODUCT_DEL_TIW,IDB_SELPRODUCT_DEL_0_TIW); 
		m_List_Del.SizeToContent();	
		m_List_Revise.LoadBitmaps(IDB_SELECTPRODUCT_REVISE_TIW,IDB_SELECTPRODUCT_REVISE_1_TIW); 
		SetDlgItemText(IDC_STATIC_16, _T("帳戶管理"));
		L_ID = _T("ID");
		L_PW = _T("密碼");
		L_Level = _T("層級");
		Message1 = _T("已被刪除。");
		Level_1 = _T("管理者");
		Level_2 = _T("經理");
		Level_3 = _T("操作員");
	}

	m_List_Revise.SizeToContent();	
	m_National.SetCurSel(g_NationalCode);
}

void CSetupMenu4_Sub1::OnClickedRlanguage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_AdminLanguage = 0;
}


void CSetupMenu4_Sub1::OnRlanguage1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_AdminLanguage = 1;
}


void CSetupMenu4_Sub1::OnRlanguage2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_AdminLanguage = 2;
}


void CSetupMenu4_Sub1::OnCbnSelchangeLangcombo()
{
	m_AdminLanguage = m_National.GetCurSel();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSetupMenu4_Sub1::OnBnClickedAddId()
{
	
	CSetupMenu4_Sub3 dlg;
	dlg.DoModal();
	Load_List();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSetupMenu4_Sub1::OnBnClickedReviceId()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSetupMenu4_Sub1::OnBnClickedDelId()
{
	if(m_TempListID=="")
		return;

	CMessageBoxDlg dlg;
	dlg.m_Message=g_MessageArr[153];//"삭제를 원하시면 '확인' 버튼을 눌러주세요. ";
	dlg.m_Mode=2;
	int result=dlg.DoModal();
	if(result==IDOK)
	{
		if(m_pSet_p.Open_DB(g_StrDirPath_Admin[0]))		
		{
			CString strQuery;
			strQuery.Format(_T("Delete From Product_Admin WHERE strID = '%s'"), m_TempListID);
			m_pSet_p.Delete_DB(strQuery);

			if(m_pSet_p.b_IsOpenedDB)
				m_pSet_p.Close_DB();

			OnWrite_Log(1);
		}	
		
		m_TempListID="";
		Load_List();
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSetupMenu4_Sub1::OnWrite_Log(int m_what)
{
	if(m_what == 1)
	{
		if(!m_pSet_7.Open_DB(g_StrDirPath[7]))		
		{
			
		}
		else
		{		
			CString strSQL;
			CString strQuery;
		
			CTime time;
			time = CTime::GetCurrentTime();
			CString strdate, strtime;
			strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());	
			strtime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), time.GetYear(), time.GetMonth(),time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

			strQuery.Format(_T("Insert INTO Computer_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),	strdate, _T("ID : ") + m_TempListID + Message1,strtime);
			m_pSet_7.Insert_DB(strQuery);
			if(m_pSet_7.b_IsOpenedDB)
				m_pSet_7.Close_DB();
		}
	}
}





void CSetupMenu4_Sub1::Load_List(void)
{
	CString oneColumn;
	
	int index=0;
	if(!m_pSet_p.Open_DB(g_StrDirPath_Admin[0]))		
	{
	}
	else
	{
		CString strSQL;
		strSQL.Format(_T("SELECT * FROM Product_Admin ORDER BY strID ASC"));
		if(m_pSet_p.Select_DB(strSQL))
		{
			m_ListID.DeleteAllItems();
			while(m_pSet_p.IsEOF()!=1)
			{
				// 새로운 행 등록
				CString tmp=+_T("");
				CString tmp2=+_T("");
				CString tmp3=+_T("");
				int m_tmp2=0;
				m_pSet_p.ChangeRsChar(_T("strID"), tmp);
				m_pSet_p.ChangeRsInt(_T("strLevel"),m_tmp2);
				
				if(m_tmp2 == 2)
					tmp2 = Level_1;
				else if(m_tmp2 == 1)
					tmp2 = Level_2;
				else
					tmp2 = Level_3;

				m_pSet_p.ChangeRsChar(_T("strPassword"),tmp3);
				if(tmp!=_T(""))
				{
					oneColumn.Format(_T("%03d"), (index+1));
					m_ListID.InsertItem(index, _T("")); 
			//		m_ListID.SetItemText(index, 1, oneColumn);
					//CString tmp=_T("");
					//m_pSet_p.ChangeRsChar(_T("ProductName"), tmp);
					m_ListID.SetItemText(index, 1, tmp);
					m_ListID.SetItemText(index, 3, tmp2);
					m_ListID.SetItemText(index, 2, tmp3);
/*					m_pSet_p.ChangeRsChar(_T("ProductRegisterDate"), tmp);
					m_Product_List.SetItemText(index, 3, tmp);
					m_pSet_p.ChangeRsChar(_T("ProductLastDate"), tmp);
					m_Product_List.SetItemText(index, 4, tmp);
*/					index++;
				}
				m_pSet_p.MoveNext();
			}
		}
		if(m_pSet_p.b_IsOpenedDB)
		{
			m_pSet_p.Close_DB();
		}
	}
}


void CSetupMenu4_Sub1::Init_List(void)
{
	CRect rect;
    m_ListID.GetClientRect(&rect);
	m_ListID.InsertColumn(0, _T(""), LVCFMT_CENTER, 0);
//	m_ListID.InsertColumn(1, _T("No")/*g_MessageArr[151]*/, LVCFMT_CENTER, rect.Width()/4-30);
	m_ListID.InsertColumn(1, L_ID/*g_MessageArr[152]*/, LVCFMT_CENTER, rect.Width()/4*3-70);
	m_ListID.InsertColumn(2, L_PW/*g_MessageArr[152]*/, LVCFMT_CENTER, rect.Width()/4+5);
	m_ListID.InsertColumn(3, L_Level/*g_MessageArr[152]*/, LVCFMT_CENTER, rect.Width()/4+5);
	m_ListID.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}


void CSetupMenu4_Sub1::OnLvnItemchangedListId(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	int num=pNMLV->iItem;
	m_TempListID=m_ListID.GetItemText(num, 1);
	*pResult = 0;
}
