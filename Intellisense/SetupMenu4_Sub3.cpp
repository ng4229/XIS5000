// SetupMenu4_Sub3.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "SetupMenu4_Sub3.h"
#include "afxdialogex.h"
#include "KeyboardDlg.h"


// CSetupMenu4_Sub3 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMenu4_Sub3, CDialogEx)

CSetupMenu4_Sub3::CSetupMenu4_Sub3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMenu4_Sub3::IDD, pParent)
	, m_TempID(_T(""))
	, m_NameID(_T(""))
	, m_NamePW(_T(""))
	, m_Level(0)
	, Message1(_T(""))
	, Message2(_T(""))
	, Message3(_T(""))
{

}

CSetupMenu4_Sub3::~CSetupMenu4_Sub3()
{
}

void CSetupMenu4_Sub3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLOSE, m_List_Close);
	DDX_Control(pDX, IDC_SAVE, m_List_Save);
}


BEGIN_MESSAGE_MAP(CSetupMenu4_Sub3, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SAVE, &CSetupMenu4_Sub3::OnBnClickedSave)
	ON_BN_CLICKED(IDC_CLOSE, &CSetupMenu4_Sub3::OnBnClickedClose)
	ON_BN_CLICKED(IDC_RUSEMASKBOX_0, &CSetupMenu4_Sub3::OnBnClickedRusemaskbox0)
	ON_BN_CLICKED(IDC_RUSEMASKBOX_1, &CSetupMenu4_Sub3::OnBnClickedRusemaskbox1)
//	ON_WM_XBUTTONDOWN()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CSetupMenu4_Sub3 메시지 처리기입니다.


void CSetupMenu4_Sub3::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CSetupMenu4_Sub3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255,255,255)); 

	LoadBackground();


	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CSetupMenu4_Sub3::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_EDIT_ID)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CKeyboardDlg dlg;
			dlg.m_NationalCode = g_NationalCode;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				GetDlgItem(IDC_EDIT_ID)->SetWindowText(dlg.m_TestName);
				m_NameID = dlg.m_TestName;
			}
			else
				GetDlgItem(IDC_EDIT_ID)->SetWindowText(_T(""));
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_EDIT_PASSWORD)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CKeyboardDlg dlg;
			dlg.m_NationalCode = g_NationalCode;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText(dlg.m_TestName);
				m_NamePW = dlg.m_TestName;
			}
			else
				GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText(_T(""));
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSetupMenu4_Sub3::LoadBackground(void)
{
	if(g_NationalCode == 0)
	{
		m_List_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE,IDB_BTSETUPMENU_SAVE_1); 
		m_List_Save.SizeToContent();	
		m_List_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT,IDB_SELPRODUCT_EXIT_0); 
		m_List_Close.SizeToContent();	
		SetDlgItemText(IDC_STATIC_1, _T("계정추가"));
		SetDlgItemText(IDC_STATIC_16, _T("아이디"));
		SetDlgItemText(IDC_STATIC_17, _T("비밀번호"));
		SetDlgItemText(IDC_STATIC_18, _T("등급"));
		SetDlgItemText(IDC_STATIC_20, _T("관리자"));
		SetDlgItemText(IDC_STATIC_21, _T("작업자"));
		Message1 = _T("같은 ID가 존재합니다.");
		Message2 = _T("가 추가 되었습니다.");
		Message3 = _T("없음");
	}
	else if(g_NationalCode == 1)
	{
		m_List_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_ENG,IDB_BTSETUPMENU_SAVE_1_ENG); 
		m_List_Save.SizeToContent();	
		m_List_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_ENG,IDB_SELPRODUCT_EXIT_0_ENG); 
		m_List_Close.SizeToContent();	
		
		SetDlgItemText(IDC_STATIC_1, _T("Add account"));
		SetDlgItemText(IDC_STATIC_16, _T("ID"));
		SetDlgItemText(IDC_STATIC_17, _T("PW"));
		SetDlgItemText(IDC_STATIC_18, _T("Level"));
		SetDlgItemText(IDC_STATIC_20, _T("Manager"));
		SetDlgItemText(IDC_STATIC_21, _T("Worker"));
		
		Message1 = _T("The same ID exists.");
		Message2 = _T("has been added.");
		Message3 = _T("does not exist");

	}
	else if(g_NationalCode == 2)
	{
		m_List_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_CHI,IDB_BTSETUPMENU_SAVE_1_CHI); 
		m_List_Save.SizeToContent();	
		m_List_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_CHI,IDB_SELPRODUCT_EXIT_0_CHI); 
		m_List_Close.SizeToContent();	

		SetDlgItemText(IDC_STATIC_1, _T("新增帐户"));
		SetDlgItemText(IDC_STATIC_16, _T("姓名"));
		SetDlgItemText(IDC_STATIC_17, _T("密码"));
		SetDlgItemText(IDC_STATIC_18, _T("等级"));
		SetDlgItemText(IDC_STATIC_20, _T("经理"));
		SetDlgItemText(IDC_STATIC_21, _T("工人"));

		Message1 = _T("存在相同的ID.");
		Message2 = _T("已添加. ");
		Message3 = _T("不存在");
	}
	else if(g_NationalCode == 3)
	{
		m_List_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_RUS,IDB_BTSETUPMENU_SAVE_1_RUS); 
		m_List_Save.SizeToContent();	
		m_List_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_RUS,IDB_SELPRODUCT_EXIT_0_RUS); 
		m_List_Close.SizeToContent();

		SetDlgItemText(IDC_STATIC_1, _T("добавить аккаунт"));
		SetDlgItemText(IDC_STATIC_16, _T("Имя"));
		SetDlgItemText(IDC_STATIC_17, _T("Пароль"));
		SetDlgItemText(IDC_STATIC_18, _T("уровень"));
		SetDlgItemText(IDC_STATIC_20, _T("управляющий делами"));
		SetDlgItemText(IDC_STATIC_21, _T("рабочий"));

		Message1 = _T("Такой же идентификатор существует.");
		Message2 = _T("Добавлен.");
		Message3 = _T("не существует");
	}
	else if(g_NationalCode == 4)
	{
		m_List_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_UKR,IDB_BTSETUPMENU_SAVE_1_UKR); 
		m_List_Save.SizeToContent();	
		m_List_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_UKR,IDB_SELPRODUCT_EXIT_0_UKR); 
		m_List_Close.SizeToContent();	

		SetDlgItemText(IDC_STATIC_1, _T("Додати обліковий запис"));
		SetDlgItemText(IDC_STATIC_16, _T("Ім'я"));
		SetDlgItemText(IDC_STATIC_17, _T("Пароль"));
		SetDlgItemText(IDC_STATIC_18, _T("Рівень"));
		SetDlgItemText(IDC_STATIC_20, _T("менеджер"));
		SetDlgItemText(IDC_STATIC_21, _T("Робітник"));

		Message1 = _T("Існує той самий ідентифікатор.");
		Message2 = _T("додано.");
		Message3 = _T("не існує");
	}
	else if(g_NationalCode == 5)
	{
		m_List_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_SPN,IDB_BTSETUPMENU_SAVE_1_SPN); 
		m_List_Save.SizeToContent();	
		m_List_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_SPN,IDB_SELPRODUCT_EXIT_0_SPN); 
		m_List_Close.SizeToContent();	

		SetDlgItemText(IDC_STATIC_1, _T("Añadir cuenta"));
		SetDlgItemText(IDC_STATIC_16, _T("Nombre"));
		SetDlgItemText(IDC_STATIC_17, _T("Clave"));
		SetDlgItemText(IDC_STATIC_18, _T("Nivel"));
		SetDlgItemText(IDC_STATIC_20, _T("gerente"));
		SetDlgItemText(IDC_STATIC_21, _T("Trabajador"));

		Message1 = _T("El mismo ID existe.");
		Message2 = _T("ha sido añadido.");
		Message3 = _T("no existe");
	}
	else if(g_NationalCode == 6)
	{
		m_List_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_POL,IDB_BTSETUPMENU_SAVE_1_POL); 
		m_List_Save.SizeToContent();	
		m_List_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_POL,IDB_SELPRODUCT_EXIT_0_POL); 
		m_List_Close.SizeToContent();	

		SetDlgItemText(IDC_STATIC_1, _T("Dodaj konto"));
		SetDlgItemText(IDC_STATIC_16, _T("imię"));
		SetDlgItemText(IDC_STATIC_17, _T("Hasło"));
		SetDlgItemText(IDC_STATIC_18, _T("Poziom"));
		SetDlgItemText(IDC_STATIC_20, _T("Menedżer"));
		SetDlgItemText(IDC_STATIC_21, _T("Pracownik"));

		Message1 = _T("Ten sam identyfikator istnieje.");
		Message2 = _T("Dodano");
		Message3 = _T("nie istnieje");
	}
	else if(g_NationalCode == 7)
	{
		m_List_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_TIW,IDB_BTSETUPMENU_SAVE_1_TIW); 
		m_List_Save.SizeToContent();	
		m_List_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_TIW,IDB_SELPRODUCT_EXIT_0_TIW); 
		m_List_Close.SizeToContent();

		SetDlgItemText(IDC_STATIC_1, _T("新增帳戶"));
		SetDlgItemText(IDC_STATIC_16, _T("ID"));
		SetDlgItemText(IDC_STATIC_17, _T("密碼"));
		SetDlgItemText(IDC_STATIC_18, _T("層級"));
		SetDlgItemText(IDC_STATIC_20, _T("經理"));
		SetDlgItemText(IDC_STATIC_21, _T("工人"));

		Message1 = _T("存在相同的ID.");
		Message2 = _T("已添加.");
		Message3 = _T("不存在.");
	}
}


void CSetupMenu4_Sub3::OnBnClickedSave()
{
	if(!m_pSet_p.Open_DB(g_StrDirPath_Admin[0]))		
	{
				
	}
	else
	{
		CString strSQL;
		CString strQuery;
		strSQL.Format(_T("SELECT * FROM Product_Admin WHERE strID = '%s'"), m_NameID);

		if(m_pSet_p.Select_DB(strSQL))
		{
			if(m_pSet_p.IsEOF())
			{
				strQuery.Format(_T("Insert INTO Product_Admin(strID, strPassword, strLevel, strlog)") _T("VALUES('%s', '%s', '%d', '%s')"),
					m_NameID, m_NamePW, m_Level, Message3);
				m_pSet_p.Insert_DB(strQuery);	

				OnWrite_Log();

			}
			else
			{											
				AfxMessageBox(Message1);
				/*
				strQuery.Format(_T("Update PartChange SET strInstallDate = '%s', strExpectDate = '%s', strCycle = '%s' WHERE strName = '%s'"),
					dlg.m_Input1_Date.Format(_T("%Y-%m-%d")), _T("0"),  cycle, temp);
				m_pSet_10.Update_DB(strQuery);*/
			}
		}
		if(m_pSet_p.b_IsOpenedDB)
			m_pSet_p.Close_DB();
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSetupMenu4_Sub3::OnBnClickedClose()
{
	EndDialog(1);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSetupMenu4_Sub3::OnWrite_Log(void)
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

		strQuery.Format(_T("Insert INTO Computer_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),	strdate, _T("ID : ") + m_NameID + Message2,strtime);
		m_pSet_7.Insert_DB(strQuery);
		if(m_pSet_7.b_IsOpenedDB)
			m_pSet_7.Close_DB();
	}
}


void CSetupMenu4_Sub3::OnBnClickedRusemaskbox0()
{
	m_Level = 1;	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSetupMenu4_Sub3::OnBnClickedRusemaskbox1()
{
	m_Level = 0;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


//void CSetupMenu4_Sub3::OnXButtonDown(UINT nFlags, UINT nButton, CPoint point)
//{
//	// 이 기능을 사용하려면 Windows 2000 이상이 필요합니다.
//	// _WIN32_WINNT 및 WINVER 기호는 0x0500보다 크거나 같아야 합니다.
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//
//
//	CDialogEx::OnXButtonDown(nFlags, nButton, point);
//}


void CSetupMenu4_Sub3::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CDialogEx::OnLButtonDown(nFlags, point);
}
