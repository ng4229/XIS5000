// SetupMenu_4_Admin.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "SetupMenu_4_Admin.h"
#include "afxdialogex.h"

//20200331
#include "MainFrm.h"
#include "ChildFrm.h"
#include "IntellisenseView.h"
#include "AdminDlg.h"
#include "MessageBoxDlg.h"


// CSetupMenu_4_Admin 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMenu_4_Admin, CDialogEx)

CSetupMenu_4_Admin::CSetupMenu_4_Admin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMenu_4_Admin::IDD, pParent)
{
	
}

CSetupMenu_4_Admin::~CSetupMenu_4_Admin()
{
}

void CSetupMenu_4_Admin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_Bt_Close);
	DDX_Control(pDX, IDOK, m_Bt_Save);
	DDX_Control(pDX, IDC_TAB_SETUP4, m_tabCtrl_Setup4);
}


BEGIN_MESSAGE_MAP(CSetupMenu_4_Admin, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetupMenu_4_Admin::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetupMenu_4_Admin::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CSetupMenu_4_Admin 메시지 처리기입니다.


BOOL CSetupMenu_4_Admin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	/*
	if(g_NationalCode==0)
	{
		m_tabCtrl_Setup4.InsertItem(0, _T("비밀번호"));
		m_tabCtrl_Setup4.InsertItem(1, _T("언어설정"));
		m_tabCtrl_Setup4.InsertItem(2, _T("권한부여"));
	}
	else if(g_NationalCode==1)
	{
		m_tabCtrl_Setup4.InsertItem(0, _T("Password"));
		m_tabCtrl_Setup4.InsertItem(1, _T("Language"));
		m_tabCtrl_Setup4.InsertItem(2, _T("Authority"));
	}
	else if(g_NationalCode==2)
	{
		m_tabCtrl_Setup4.InsertItem(0, _T("修改密码"));
		m_tabCtrl_Setup4.InsertItem(1, _T("改变语言"));
		m_tabCtrl_Setup4.InsertItem(2, _T("授予权限"));
	}
	else if(g_NationalCode==3)
	{
		m_tabCtrl_Setup4.InsertItem(0, _T("Пароль"));
		m_tabCtrl_Setup4.InsertItem(1, _T("Язык"));
		m_tabCtrl_Setup4.InsertItem(2, _T("Власть"));
	}
	else if(g_NationalCode==4)
	{
		m_tabCtrl_Setup4.InsertItem(0, _T("Пароль"));
		m_tabCtrl_Setup4.InsertItem(1, _T("Мову"));
		m_tabCtrl_Setup4.InsertItem(2, _T("Авторитет"));
	}
	else if(g_NationalCode==5)
	{
		m_tabCtrl_Setup4.InsertItem(0, _T("Contraseña"));
		m_tabCtrl_Setup4.InsertItem(1, _T("Idioma"));
		m_tabCtrl_Setup4.InsertItem(2, _T("Autoridad"));
	}
	else if(g_NationalCode==6)
	{
		m_tabCtrl_Setup4.InsertItem(0, _T("hasło"));
		m_tabCtrl_Setup4.InsertItem(1, _T("Język"));
		m_tabCtrl_Setup4.InsertItem(2, _T("Autorytet"));
	}
	else if(g_NationalCode==7)
	{
		m_tabCtrl_Setup4.InsertItem(0, _T("密碼"));
		m_tabCtrl_Setup4.InsertItem(1, _T("語言"));
		m_tabCtrl_Setup4.InsertItem(2, _T("權限"));
	}
	*/
	if(g_NationalCode==0)
	{
		if(g_AdminLevel==1)
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("비밀번호"));
			
		}
		else
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("비밀번호"));
			m_tabCtrl_Setup4.InsertItem(1, _T("계정관리"));
			m_tabCtrl_Setup4.InsertItem(2, _T("권한부여"));
			
		}
	}
	else if(g_NationalCode==1)
	{
		if(g_AdminLevel==1)
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("Password"));

		}
		else
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("Password"));
			m_tabCtrl_Setup4.InsertItem(1, _T("Account"));
			m_tabCtrl_Setup4.InsertItem(2, _T("Authority"));
		
		}
	}
	else if(g_NationalCode==2)
	{
		if(g_AdminLevel==1)
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("修改密码"));
			
		}
		else
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("修改密码"));
			m_tabCtrl_Setup4.InsertItem(1, _T("帐户"));
			m_tabCtrl_Setup4.InsertItem(2, _T("权威"));
		
		}
	}
	else if(g_NationalCode==3)
	{
		if(g_AdminLevel==1)
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("Пароль"));
			
		}
		else
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("Пароль"));
			m_tabCtrl_Setup4.InsertItem(1, _T("Счет"));
			m_tabCtrl_Setup4.InsertItem(2, _T("Власть"));
			
		}
	}
	else if(g_NationalCode==4)
	{
		if(g_AdminLevel==1)
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("Пароль"));
		
		}
		else
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("Пароль"));
			m_tabCtrl_Setup4.InsertItem(1, _T("Рахунок"));
			m_tabCtrl_Setup4.InsertItem(2, _T("Авторитет"));
			
		}
	}
	else if(g_NationalCode==5)
	{
		if(g_AdminLevel==1)
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("Clave"));
			
		}
		else
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("Clave"));
			m_tabCtrl_Setup4.InsertItem(1, _T("Cuenta"));
			m_tabCtrl_Setup4.InsertItem(2, _T("Autoridad"));
			
		}
	}
	else if(g_NationalCode==6)
	{
		if(g_AdminLevel==1)
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("Hasło"));
			
		}
		else
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("Hasło"));
			m_tabCtrl_Setup4.InsertItem(1, _T("Rachunek"));
			m_tabCtrl_Setup4.InsertItem(2, _T("Autorytet"));
		
		}
	}
	else if(g_NationalCode==7)
	{
		if(g_AdminLevel==1)
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("密碼"));
		
		}
		else
		{
			m_tabCtrl_Setup4.InsertItem(0, _T("密碼"));
			m_tabCtrl_Setup4.InsertItem(1, _T("帳戶"));
			m_tabCtrl_Setup4.InsertItem(2, _T("權限"));
			
		}
	}

	m_tabCtrl_Setup4.Init();

	LoadBackground();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupMenu_4_Admin::LoadBackground()
{
	BITMAP bmpInfo;
	if(g_NationalCode==0)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_4); 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE,IDB_BTSETUPMENU_SAVE_1);
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT,IDB_SELPRODUCT_EXIT_0); 
	}
	else if(g_NationalCode==1)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_4_ENG); 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_ENG,IDB_BTSETUPMENU_SAVE_1_ENG); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_ENG,IDB_SELPRODUCT_EXIT_0_ENG); 
	}
	else if(g_NationalCode==2)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_4_CHI); 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_CHI,IDB_BTSETUPMENU_SAVE_1_CHI); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_CHI,IDB_SELPRODUCT_EXIT_0_CHI); 
	}
	else if(g_NationalCode==3)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_4_RUS); 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_RUS,IDB_BTSETUPMENU_SAVE_1_RUS); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_RUS,IDB_SELPRODUCT_EXIT_0_RUS); 
	}
	else if(g_NationalCode==4)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_4_UKR); 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_UKR,IDB_BTSETUPMENU_SAVE_1_UKR); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_UKR,IDB_SELPRODUCT_EXIT_0_UKR); 
	}
	else if(g_NationalCode==5)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_4_SPN); 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_SPN,IDB_BTSETUPMENU_SAVE_1_SPN); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_SPN,IDB_SELPRODUCT_EXIT_0_SPN); 
	}
	else if(g_NationalCode==6)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_4_POL); 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_POL,IDB_BTSETUPMENU_SAVE_1_POL); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_POL,IDB_SELPRODUCT_EXIT_0_POL); 
	}
	else if(g_NationalCode==7)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_4_TIW); 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_TIW,IDB_BTSETUPMENU_SAVE_1_TIW); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_TIW,IDB_SELPRODUCT_EXIT_0_TIW); 
	}

	m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 	

	m_Bt_Save.SizeToContent();
	m_Bt_Close.SizeToContent();	



}

BOOL CSetupMenu_4_Admin::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;	

	return CDialogEx::PreTranslateMessage(pMsg);
}

#include "SetupMenu4_Sub0.h"

void CSetupMenu_4_Admin::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//Sub0
	/*
	if(b_IsAdminChanged[0])
	{
		b_IsAdminChanged[0]=false;
		g_Password_Admin=m_TempNewPassword;

		SetFileAttributes(_T("D:\\Database\\password_Admin.dat"), FILE_ATTRIBUTE_NORMAL);
		
		CStdioFile m_File;
		m_File.Open(_T("D:\\Database\\password_Admin.dat"), CStdioFile::modeCreate | CStdioFile::modeWrite);
		m_File.WriteString(g_Password_Admin);
		m_File.Close();

		SetFileAttributes(_T("D:\\Database\\password_Admin.dat"), FILE_ATTRIBUTE_HIDDEN);
		OnWriteReport();
	}
	*/

	if(b_IsAdminChanged[0])
	{
		if(!m_pSet_p.Open_DB(g_StrDirPath_Admin[0]))		
		{
				
		}
		else
		{
			b_IsAdminChanged[0]=false;

			CString strSQL;
			CString strQuery;
			strSQL.Format(_T("SELECT * FROM Product_Admin WHERE strID = '%s'"), g_NameID);

			if(m_pSet_p.Select_DB(strSQL))
			{
				if(m_pSet_p.IsEOF())
				{
					if(g_NationalCode==0) //한국어
					{
						AfxMessageBox(_T("등록된 ID가 없습니다. 다시 입력해주세요."));
					}
					else if(g_NationalCode==1)//영어
					{
						AfxMessageBox(_T("There is no registered ID. Please re-enter."));
					}
					else if(g_NationalCode==2)//중국어
					{
						AfxMessageBox(_T("没有注册ID。请重新输入."));
					}
					else if(g_NationalCode==3)//러시아어
					{
						AfxMessageBox(_T("Нет зарегистрированного удостоверения личности. Пожалуйста, введите повторно."));
					}
					else if(g_NationalCode==4)//우크라이나어
					{
						AfxMessageBox(_T("Немає зареєстрованого ідентифікатора. Будь ласка, введіть повторно."));
					}
					else if(g_NationalCode==5)//스페인어
					{
						AfxMessageBox(_T("No hay identificación registrada. Vuelva a ingresar."));
					}
					else if(g_NationalCode==6)//폴란드어
					{
						AfxMessageBox(_T("Nie ma zarejestrowanego identyfikatora. Wprowadź ponownie."));
					}
					else if(g_NationalCode==7)//대만어
					{
						AfxMessageBox(_T("沒有註冊ID。請重新輸入."));
					}		
				
				}
				else
				{				
					strQuery.Format(_T("Update Product_Admin SET strPassword = '%s' WHERE strID = '%s'"), m_TempNewPassword, g_NameID);
					m_pSet_p.Update_DB(strQuery);

				//	AfxMessageBox(strQuery);
				}
			}
			if(m_pSet_p.b_IsOpenedDB)
				m_pSet_p.Close_DB();
		}
	}

	
	UpdateData(true);

	
	if(b_TempCheckAutoLogout!=g_bUseAutoLogout)
	{
		g_bUseAutoLogout=b_TempCheckAutoLogout;
		CStdioFile fileString;
		fileString.Open(_T("D:\\Database\\AutoLogout.dat"), CStdioFile::modeCreate | CStdioFile::modeWrite);
		if(g_bUseAutoLogout==1)
		{
			fileString.WriteString(_T("1"));
			m_AutoLogout=2;
		}
		else
		{
			fileString.WriteString(_T("0"));
			m_AutoLogout=0;
		}
		fileString.Close();	
	}
	

	//Sub1


	if(m_AdminLanguage!=g_NationalCode)
	{
		b_IsAdminChanged[1]=true;
	}
	else
	{
		b_IsAdminChanged[1]=false;
	}

	//Sub2
	if(b_IsAdminChanged[2])
	{
		bool b_IsSave=false;
		b_IsAdminChanged[2]=false;
		for(int i=0; i<11; i++)
		{
			if(m_AdminAuth_Right[i]!=m_AdminAuth[i])
			{
				b_IsSave=true;
				m_AdminAuth_Right[i]=m_AdminAuth[i];
			}
		}
		m_AdminAuth_Right[0]=1;
		m_AdminAuth_Right[1]=1;
		m_AdminAuth_Right[8]=1;

		if(b_IsSave)
		{
			CFileFind ff;
			CStdioFile fileString;
			if(fileString.Open(_T("D:\\Database\\AdminAuthority.dat"),
				CStdioFile::modeWrite | CStdioFile::modeNoTruncate | CStdioFile::shareDenyNone))
			{
				fileString.SeekToBegin();

				for(int i=0; i<11; i++)
				{
					if(m_AdminAuth_Right[i]==0)
						fileString.WriteString(_T("0\n"));
					else
						fileString.WriteString(_T("1\n"));
				}
				fileString.Close();
			}
		}
	}
}


void CSetupMenu_4_Admin::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(b_IsAdminChanged[1])
	{
		g_NationalCode=m_AdminLanguage;
		CDialogEx::OnOK();
	}
	else
		CDialogEx::OnCancel();
}

void CSetupMenu_4_Admin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
    CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


void CSetupMenu_4_Admin::OnWriteReport(void)
{
	CString strdate, strtime;
	CTime time;
	time = CTime::GetCurrentTime();
	strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());		
	strtime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), time.GetYear(),time.GetMonth(),time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	
	CString strtext;
	if(g_NationalCode==0)
		strtext.Format(_T("최고 ")+g_MessageArr[185]);//_T("관리자 비밀번호 변경됨"));
	else if(g_NationalCode==1)
		strtext.Format(_T("Super ")+g_MessageArr[185]);//_T("관리자 비밀번호 변경됨"));
	else if(g_NationalCode==2)
		strtext.Format(_T("最上位 ")+g_MessageArr[185]);//_T("관리자 비밀번호 변경됨"));
	else if(g_NationalCode==3)
		strtext.Format(_T("супер ")+g_MessageArr[185]);//_T("관리자 비밀번호 변경됨"));
	else if(g_NationalCode==4)
		strtext.Format(_T("Супер ")+g_MessageArr[185]);//_T("관리자 비밀번호 변경됨"));
	else if(g_NationalCode==5)
		strtext.Format(_T("Súper ")+g_MessageArr[185]);//_T("관리자 비밀번호 변경됨"));
	else if(g_NationalCode==6)
		strtext.Format(_T("Super ")+g_MessageArr[185]);//_T("관리자 비밀번호 변경됨"));
	else if(g_NationalCode==7)
		strtext.Format(_T("超級 ")+g_MessageArr[185]);//_T("관리자 비밀번호 변경됨"));
	bool g_bOpenDB=true;
	if(!m_pSet_7.Open_DB(g_StrDirPath[7]))		
	{	
		g_bOpenDB=false;
		RecoveryDatabase(g_StrDirPath[7], g_StrDirPath_copy[7]);
		if(m_pSet_7.Open_DB(g_StrDirPath[7]))		
			g_bOpenDB=true;
	}
	if(g_bOpenDB)
	{
		CString strQuery;
		strQuery.Format(_T("Insert INTO Computer_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
			strdate, strtext, strtime);
		m_pSet_7.Insert_DB(strQuery);
		if(m_pSet_7.b_IsOpenedDB)
			m_pSet_7.Close_DB();
	}
}


void CSetupMenu_4_Admin::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	if(g_AddID == true)
	{
		
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}



