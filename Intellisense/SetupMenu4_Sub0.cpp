// SetupMenu4_Sub0.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "SetupMenu4_Sub0.h"
#include "afxdialogex.h"
#include "KeyboardDlg.h"

//20200331
// CSetupMenu4_Sub0 대화 상자입니다.
#include "AdminDlg.h"
#include "MessageBoxDlg.h"

IMPLEMENT_DYNAMIC(CSetupMenu4_Sub0, CDialogEx)

CSetupMenu4_Sub0::CSetupMenu4_Sub0(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMenu4_Sub0::IDD, pParent)
{

	m_NewPassword = _T("");
	m_TempNewPassword= _T("");
	m_OldPassword = _T("");
	b_IsAdminChanged[0]=false;
}

CSetupMenu4_Sub0::~CSetupMenu4_Sub0()
{
}

void CSetupMenu4_Sub0::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NEWNAME, m_NewPassword);
	DDX_Text(pDX, IDC_EDIT_OLDNAME, m_OldPassword);
	DDX_Check(pDX, IDC_CHECK_AUTOLOGOUT, b_CheckAutoLogout);
	DDX_Control(pDX, IDC_LANGCOMBO, m_National);
}


BEGIN_MESSAGE_MAP(CSetupMenu4_Sub0, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_AUTOLOGOUT, &CSetupMenu4_Sub0::OnClickedCheckAutologout)
	ON_CBN_SELCHANGE(IDC_LANGCOMBO, &CSetupMenu4_Sub0::OnCbnSelchangeLangcombo)
END_MESSAGE_MAP()


// CSetupMenu4_Sub0 메시지 처리기입니다.


BOOL CSetupMenu4_Sub0::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255,255,255)); 

	LoadBackground();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	b_CheckAutoLogout=g_bUseAutoLogout;
	b_TempCheckAutoLogout=g_bUseAutoLogout;
	if(b_CheckAutoLogout==0)
		((CButton*)GetDlgItem(IDC_CHECK_AUTOLOGOUT))->SetCheck(false);
	else
		((CButton*)GetDlgItem(IDC_CHECK_AUTOLOGOUT))->SetCheck(true);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupMenu4_Sub0::LoadBackground()
{
	if(g_NationalCode==0)
	{
		if(g_AdminLevel==2)
		{
			SetDlgItemText(IDC_STATIC_TITLE, _T("최고 관리자 비밀번호 변경"));
		}
		else
			SetDlgItemText(IDC_STATIC_TITLE, _T("비밀번호 변경"));

		SetDlgItemText(IDC_CHECK_AUTOLOGOUT, _T("자동 로그아웃 기능 사용여부"));
		SetDlgItemText(IDC_STATIC_OLD, _T("이전 비밀번호"));
		SetDlgItemText(IDC_STATIC_NEW, _T("신규 비밀번호"));
		SetDlgItemText(IDC_STATIC_NEW2, _T("프로그램 적용 언어 선택"));
	}
	else if(g_NationalCode==1)
	{
		if(g_AdminLevel==2)
		{
			SetDlgItemText(IDC_STATIC_TITLE, _T("Super Administrator Password Change"));
		}
		else
			SetDlgItemText(IDC_STATIC_TITLE, _T("Password Change"));

		SetDlgItemText(IDC_CHECK_AUTOLOGOUT, _T("Use Automatic Logout Fuction"));
		SetDlgItemText(IDC_STATIC_OLD, _T("Enter Old Password"));
		SetDlgItemText(IDC_STATIC_NEW, _T("Enter New Password"));
		SetDlgItemText(IDC_STATIC_NEW2, _T("Select language for program application"));
	}
	else if(g_NationalCode==2)
	{
		if(g_AdminLevel==2)
		{
			SetDlgItemText(IDC_STATIC_TITLE, _T("顶级管理员密码更改"));
		}
		else
			SetDlgItemText(IDC_STATIC_TITLE, _T("密码更改"));

		SetDlgItemText(IDC_CHECK_AUTOLOGOUT, _T("是否使用自动注销功能"));
		SetDlgItemText(IDC_STATIC_OLD, _T("旧密码"));
		SetDlgItemText(IDC_STATIC_NEW, _T("新密码"));
		SetDlgItemText(IDC_STATIC_NEW2, _T("选择程序应用程序的语言"));
	}
	else if(g_NationalCode==3)
	{
		if(g_AdminLevel==2)
		{
			SetDlgItemText(IDC_STATIC_TITLE, _T("Изменение пароля суперадминистратора"));
		}
		else
			SetDlgItemText(IDC_STATIC_TITLE, _T("Изменение пароля"));

		SetDlgItemText(IDC_CHECK_AUTOLOGOUT, _T("Использовать функцию автоматического выхода из системы"));
		SetDlgItemText(IDC_STATIC_OLD, _T("Введите старый пароль"));
		SetDlgItemText(IDC_STATIC_NEW, _T("Введите новый пароль"));
		SetDlgItemText(IDC_STATIC_NEW2, _T("Выберите язык приложения программы"));
	}
	else if(g_NationalCode==4)
	{
		if(g_AdminLevel==2)
		{
			SetDlgItemText(IDC_STATIC_TITLE, _T("Зміна пароля супер адміністратора"));
		}
		else
			SetDlgItemText(IDC_STATIC_TITLE, _T("Зміна пароля"));

		SetDlgItemText(IDC_CHECK_AUTOLOGOUT, _T("Використовуйте функцію автоматичного виходу з системи"));
		SetDlgItemText(IDC_STATIC_OLD, _T("Введіть старий пароль"));
		SetDlgItemText(IDC_STATIC_NEW, _T("Введіть новий пароль"));
		SetDlgItemText(IDC_STATIC_NEW2, _T("Виберіть мову для програми"));
	}
	else if(g_NationalCode==5)
	{
		if(g_AdminLevel==2)
		{
			SetDlgItemText(IDC_STATIC_TITLE, _T("Cambio de contraseña de superadministrador"));
		}
		else
			SetDlgItemText(IDC_STATIC_TITLE, _T("Cambio de contraseña"));

		SetDlgItemText(IDC_CHECK_AUTOLOGOUT, _T("Usar la función de cierre de sesión automático"));
		SetDlgItemText(IDC_STATIC_OLD, _T("Ingrese la contraseña anterior"));
		SetDlgItemText(IDC_STATIC_NEW, _T("Introduzca una nueva contraseña"));
		SetDlgItemText(IDC_STATIC_NEW2, _T("Seleccione el idioma para la aplicación del programa"));
	}
	else if(g_NationalCode==6)
	{
		if(g_AdminLevel==2)
		{
			SetDlgItemText(IDC_STATIC_TITLE, _T("Zmiana hasła superadministratora"));
		}
		else
			SetDlgItemText(IDC_STATIC_TITLE, _T("Zmiana hasła"));

		SetDlgItemText(IDC_CHECK_AUTOLOGOUT, _T("Użyj funkcji automatycznego wylogowania"));
		SetDlgItemText(IDC_STATIC_OLD, _T("Wprowadź stare hasło"));
		SetDlgItemText(IDC_STATIC_NEW, _T("Wprowadź nowe hasło"));
		SetDlgItemText(IDC_STATIC_NEW2, _T("Wybierz język aplikacji programu"));
	}
	else if(g_NationalCode==7)
	{
		if(g_AdminLevel==2)
		{
			SetDlgItemText(IDC_STATIC_TITLE, _T("最高管理員密碼"));
		}
		else
			SetDlgItemText(IDC_STATIC_TITLE, _T("更改密碼"));

		SetDlgItemText(IDC_CHECK_AUTOLOGOUT, _T("使用自動登出工程"));
		SetDlgItemText(IDC_STATIC_OLD, _T("輸入舊密碼"));
		SetDlgItemText(IDC_STATIC_NEW, _T("輸入新密碼"));
		SetDlgItemText(IDC_STATIC_NEW2, _T("選擇語言"));
	}

	m_National.AddString(g_MessageArr[181]); //_T("국문(Korean)"));
	m_National.AddString(g_MessageArr[182]); //_T("영문(English)"));
	m_National.AddString(g_MessageArr[183]); //_T("중문(Chinese)"));
	m_National.AddString(g_MessageArr[239]); //_T("러시아어(Russian)"));
	m_National.AddString(g_MessageArr[240]);//_T("우크라이나어(Ukraine)"));
	m_National.AddString(g_MessageArr[238]);//_T("스페인어(Spanish)"));
	m_National.AddString(g_MessageArr[241]); //폴란드어
	m_National.AddString(g_MessageArr[243]); //대만어

	m_National.SetCurSel(g_NationalCode);

}

BOOL CSetupMenu4_Sub0::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;	

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_EDIT_OLDNAME)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{		
			//CAdminDlg login;
			//login.m_Mode=2;
			//int result=login.DoModal();
		//	if(result==2)
		//	{
			CKeyboardDlg dlg;
			dlg.m_NationalCode = g_NationalCode;
			int result = dlg.DoModal();

			m_OldPassword = g_NamePW;
			//m_OldPassword = login.m_Password;
			if(result == IDOK)
			{
				if(m_OldPassword!=dlg.m_TestName)
				{
					if(m_OldPassword!="")
					{
						CMessageBoxDlg dlg;
						dlg.m_Message=g_MessageArr[184];//"이전 비밀번호와 일치하지 않습니다. (E5002)";
						dlg.DoModal();
						m_OldPassword="";
					}
				}
				else
				{
					SetDlgItemTextW(IDC_EDIT_OLDNAME, m_OldPassword);
				}
			}
			else
				GetDlgItem(IDC_EDIT_OLDNAME)->SetWindowText(_T(""));
		//	}
		//	else
		//	{
		//		m_OldPassword=_T("");
		//		SetDlgItemTextW(IDC_EDIT_OLDNAME, m_OldPassword);
		//	}
			
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_EDIT_NEWNAME)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{		
			if(m_OldPassword=="")
				return FALSE;
			/*
			CAdminDlg login;
			login.m_Mode=2;
			int result=login.DoModal();
			if(result==2)
			{*/
			CKeyboardDlg dlg;
			dlg.m_NationalCode = g_NationalCode;
			int result = dlg.DoModal();

			if(result==IDOK)
			{
				//20200331
				if(dlg.m_TestName==_T("0000") || dlg.m_TestName==g_NamePW)
				{
					if(g_NationalCode==0)
						AfxMessageBox(_T("사용할 수 없는 비밀번호입니다."));
					else if(g_NationalCode==1)
						AfxMessageBox(_T("This password cannot be used"));
					else if(g_NationalCode==2)
						AfxMessageBox(_T("密码无法使用。"));
					else if(g_NationalCode==3)
						AfxMessageBox(_T("Этот пароль нельзя использовать"));
					else if(g_NationalCode==4)
						AfxMessageBox(_T("Цей пароль використовувати не можна"));
					else if(g_NationalCode==5)
						AfxMessageBox(_T("Esta contraseña no se puede utilizar"));
					else if(g_NationalCode==6)
						AfxMessageBox(_T("Nie można użyć tego hasła"));
					else if(g_NationalCode==7)
						AfxMessageBox(_T("此密碼無法使用"));
				}
				else
				{
					m_NewPassword = dlg.m_TestName;
					m_TempNewPassword = m_NewPassword;
					g_NamePW = m_NewPassword;
					b_IsAdminChanged[0]=true;
					SetDlgItemTextW(IDC_EDIT_NEWNAME, m_NewPassword);
				}
			}
			else
			{
				GetDlgItem(IDC_EDIT_NEWNAME)->SetWindowText(_T(""));
				m_NewPassword=_T("");
				m_TempNewPassword=_T("");
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSetupMenu4_Sub0::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}



void CSetupMenu4_Sub0::OnClickedCheckAutologout()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	b_TempCheckAutoLogout=b_CheckAutoLogout;
}


void CSetupMenu4_Sub0::OnCbnSelchangeLangcombo()
{
	m_AdminLanguage = m_National.GetCurSel();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
