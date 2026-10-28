// SetupMenu4_Sub2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "SetupMenu4_Sub2.h"
#include "afxdialogex.h"

//20200331
// CSetupMenu4_Sub2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMenu4_Sub2, CDialogEx)

CSetupMenu4_Sub2::CSetupMenu4_Sub2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMenu4_Sub2::IDD, pParent)
{
	
}

CSetupMenu4_Sub2::~CSetupMenu4_Sub2()
{
}

void CSetupMenu4_Sub2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetupMenu4_Sub2, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_SETUP2, &CSetupMenu4_Sub2::OnBnClickedCheckSetup2)
	ON_BN_CLICKED(IDC_CHECK_SETUP3, &CSetupMenu4_Sub2::OnBnClickedCheckSetup3)
	ON_BN_CLICKED(IDC_CHECK_SETUP4, &CSetupMenu4_Sub2::OnBnClickedCheckSetup4)
	ON_BN_CLICKED(IDC_CHECK_SETUP5, &CSetupMenu4_Sub2::OnBnClickedCheckSetup5)
	ON_BN_CLICKED(IDC_CHECK_SETUP6, &CSetupMenu4_Sub2::OnBnClickedCheckSetup6)
	ON_BN_CLICKED(IDC_CHECK_SETUP7, &CSetupMenu4_Sub2::OnBnClickedCheckSetup7)
	ON_BN_CLICKED(IDC_CHECK_SETUP9, &CSetupMenu4_Sub2::OnBnClickedCheckSetup9)
	ON_BN_CLICKED(IDC_CHECK_SETUP10, &CSetupMenu4_Sub2::OnBnClickedCheckSetup10)
END_MESSAGE_MAP()


// CSetupMenu4_Sub2 메시지 처리기입니다.


void CSetupMenu4_Sub2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}


BOOL CSetupMenu4_Sub2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(255,255,255)); 

	LoadBackground(); 

	((CButton*)GetDlgItem(IDC_CHECK_SETUP0))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_CHECK_SETUP1))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_CHECK_SETUP8))->SetCheck(true);

	b_IsAdminChanged[2]=false;

	if(m_AdminAuth_Right[2]==1)
		((CButton*)GetDlgItem(IDC_CHECK_SETUP2))->SetCheck(true);

	if(m_AdminAuth_Right[3]==1)
		((CButton*)GetDlgItem(IDC_CHECK_SETUP3))->SetCheck(true);

	if(m_AdminAuth_Right[4]==1)
		((CButton*)GetDlgItem(IDC_CHECK_SETUP4))->SetCheck(true);

	if(m_AdminAuth_Right[5]==1)
		((CButton*)GetDlgItem(IDC_CHECK_SETUP5))->SetCheck(true);

	if(m_AdminAuth_Right[6]==1)
		((CButton*)GetDlgItem(IDC_CHECK_SETUP6))->SetCheck(true);

	if(m_AdminAuth_Right[7]==1)
		((CButton*)GetDlgItem(IDC_CHECK_SETUP7))->SetCheck(true);

	if(m_AdminAuth_Right[9]==1)
		((CButton*)GetDlgItem(IDC_CHECK_SETUP9))->SetCheck(true);

	if(m_AdminAuth_Right[10]==1)
		((CButton*)GetDlgItem(IDC_CHECK_SETUP10))->SetCheck(true);

	for(int i=0; i<11; i++)
	{
		m_AdminAuth[i]=m_AdminAuth_Right[i];
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSetupMenu4_Sub2::LoadBackground(void)
{
	if(g_NationalCode==0)
	{
		SetDlgItemText(IDC_STATIC_TITLE2, _T("일반 관리자 권한 부여"));
		SetDlgItemText(IDC_CHECK_SETUP0, _T("로그인"));
		SetDlgItemText(IDC_CHECK_SETUP1, _T("제품선택"));
		SetDlgItemText(IDC_CHECK_SETUP2, _T("검사값설정"));
		SetDlgItemText(IDC_CHECK_SETUP4, _T("디텍터"));
		SetDlgItemText(IDC_CHECK_SETUP5, _T("리젝트"));
		SetDlgItemText(IDC_CHECK_SETUP6, _T("영상저장"));
		SetDlgItemText(IDC_CHECK_SETUP7, _T("자동설정"));
		SetDlgItemText(IDC_CHECK_SETUP8, _T("관리자"));
		SetDlgItemText(IDC_CHECK_SETUP9, _T("리포트"));
		SetDlgItemText(IDC_CHECK_SETUP10, _T("부품점검"));
	}
	else if(g_NationalCode==1)
	{
		SetDlgItemText(IDC_STATIC_TITLE2, _T("Nomal administrator empowerment"));
		SetDlgItemText(IDC_CHECK_SETUP0, _T("Login"));
		SetDlgItemText(IDC_CHECK_SETUP1, _T("Product"));
		SetDlgItemText(IDC_CHECK_SETUP2, _T("Condition"));
		SetDlgItemText(IDC_CHECK_SETUP4, _T("Detector"));
		SetDlgItemText(IDC_CHECK_SETUP5, _T("Reject"));
		SetDlgItemText(IDC_CHECK_SETUP6, _T("Image"));
		SetDlgItemText(IDC_CHECK_SETUP7, _T("AutoSetup"));
		SetDlgItemText(IDC_CHECK_SETUP8, _T("Administrator"));
		SetDlgItemText(IDC_CHECK_SETUP9, _T("Report"));
		SetDlgItemText(IDC_CHECK_SETUP10, _T("Maintenance"));
	}
	else if(g_NationalCode==2)
	{
		SetDlgItemText(IDC_STATIC_TITLE2, _T("通用管理员授权"));
		SetDlgItemText(IDC_CHECK_SETUP0, _T("登录"));
		SetDlgItemText(IDC_CHECK_SETUP1, _T("产品设置"));
		SetDlgItemText(IDC_CHECK_SETUP2, _T("条件已设定"));
		SetDlgItemText(IDC_CHECK_SETUP4, _T("探测器"));
		SetDlgItemText(IDC_CHECK_SETUP5, _T("排除器"));
		SetDlgItemText(IDC_CHECK_SETUP6, _T("图像"));
		SetDlgItemText(IDC_CHECK_SETUP7, _T("自动设置"));
		SetDlgItemText(IDC_CHECK_SETUP8, _T("管理员"));
		SetDlgItemText(IDC_CHECK_SETUP9, _T("报告"));
		SetDlgItemText(IDC_CHECK_SETUP10, _T("零件检查"));
	}
	else if(g_NationalCode==3)
	{
		SetDlgItemText(IDC_STATIC_TITLE2, _T("Обычная авторизация администратора"));
		SetDlgItemText(IDC_CHECK_SETUP0, _T("Авторизоваться"));
		SetDlgItemText(IDC_CHECK_SETUP1, _T("Продукт"));
		SetDlgItemText(IDC_CHECK_SETUP2, _T("Условие"));
		SetDlgItemText(IDC_CHECK_SETUP4, _T("Детектор"));
		SetDlgItemText(IDC_CHECK_SETUP5, _T("Отклонять"));
		SetDlgItemText(IDC_CHECK_SETUP6, _T("Изображение"));
		SetDlgItemText(IDC_CHECK_SETUP7, _T("AutoSetup"));
		SetDlgItemText(IDC_CHECK_SETUP8, _T("Администратор"));
		SetDlgItemText(IDC_CHECK_SETUP9, _T("Отчет"));
		SetDlgItemText(IDC_CHECK_SETUP10, _T("Обслуживание"));
	}
	else if(g_NationalCode==4)
	{
		SetDlgItemText(IDC_STATIC_TITLE2, _T("Ідентифікація номінального адміністратора"));
		SetDlgItemText(IDC_CHECK_SETUP0, _T("Увійти"));
		SetDlgItemText(IDC_CHECK_SETUP1, _T("Продукт"));
		SetDlgItemText(IDC_CHECK_SETUP2, _T("Хвороба"));
		SetDlgItemText(IDC_CHECK_SETUP4, _T("Детектор"));
		SetDlgItemText(IDC_CHECK_SETUP5, _T("Відхилити"));
		SetDlgItemText(IDC_CHECK_SETUP6, _T("Зображення"));
		SetDlgItemText(IDC_CHECK_SETUP7, _T("Автонастроювання"));
		SetDlgItemText(IDC_CHECK_SETUP8, _T("Адміністратор"));
		SetDlgItemText(IDC_CHECK_SETUP9, _T("Звіт"));
		SetDlgItemText(IDC_CHECK_SETUP10, _T("Технічне обслуговування"));
	}
	else if(g_NationalCode==5)
	{
		SetDlgItemText(IDC_STATIC_TITLE2, _T("Autorización de administrador normal"));
		SetDlgItemText(IDC_CHECK_SETUP0, _T("Acceso"));
		SetDlgItemText(IDC_CHECK_SETUP1, _T("Producto"));
		SetDlgItemText(IDC_CHECK_SETUP2, _T("Condition"));
		SetDlgItemText(IDC_CHECK_SETUP4, _T("Detector"));
		SetDlgItemText(IDC_CHECK_SETUP5, _T("Rechazar"));
		SetDlgItemText(IDC_CHECK_SETUP6, _T("Imagen"));
		SetDlgItemText(IDC_CHECK_SETUP7, _T("AutoSetup"));
		SetDlgItemText(IDC_CHECK_SETUP8, _T("Administradora"));
		SetDlgItemText(IDC_CHECK_SETUP9, _T("Informe"));
		SetDlgItemText(IDC_CHECK_SETUP10, _T("Mantenimiento "));
	}
	else if(g_NationalCode==6)
	{
		SetDlgItemText(IDC_STATIC_TITLE2, _T("Normalna autoryzacja administratora"));
		SetDlgItemText(IDC_CHECK_SETUP0, _T("Zaloguj sie"));
		SetDlgItemText(IDC_CHECK_SETUP1, _T("Produkt"));
		SetDlgItemText(IDC_CHECK_SETUP2, _T("Warunek"));
		SetDlgItemText(IDC_CHECK_SETUP4, _T("Detektor"));
		SetDlgItemText(IDC_CHECK_SETUP5, _T("Odrzuć"));
		SetDlgItemText(IDC_CHECK_SETUP6, _T("Obraz"));
		SetDlgItemText(IDC_CHECK_SETUP7, _T("Automatyczna konfiguracja"));
		SetDlgItemText(IDC_CHECK_SETUP8, _T("Administrator"));
		SetDlgItemText(IDC_CHECK_SETUP9, _T("Raport"));
		SetDlgItemText(IDC_CHECK_SETUP10, _T("Konserwacja"));
	}
	else if(g_NationalCode==7)
	{
		SetDlgItemText(IDC_STATIC_TITLE2, _T("管理者權限"));
		SetDlgItemText(IDC_CHECK_SETUP0, _T("登入"));
		SetDlgItemText(IDC_CHECK_SETUP1, _T("產品"));
		SetDlgItemText(IDC_CHECK_SETUP2, _T("檢測值條件"));
		SetDlgItemText(IDC_CHECK_SETUP4, _T("探測器"));
		SetDlgItemText(IDC_CHECK_SETUP5, _T("剔除器"));
		SetDlgItemText(IDC_CHECK_SETUP6, _T("圖像"));
		SetDlgItemText(IDC_CHECK_SETUP7, _T("自動設定"));
		SetDlgItemText(IDC_CHECK_SETUP8, _T("管理員"));
		SetDlgItemText(IDC_CHECK_SETUP9, _T("報告"));
		SetDlgItemText(IDC_CHECK_SETUP10, _T("維護"));
	}
}


BOOL CSetupMenu4_Sub2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;	

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupMenu4_Sub2::OnBnClickedCheckSetup2()
{
	b_IsAdminChanged[2]=true;
	if(m_AdminAuth[2]==1)
		m_AdminAuth[2]=0;
	else
		m_AdminAuth[2]=1;
}

void CSetupMenu4_Sub2::OnBnClickedCheckSetup3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	b_IsAdminChanged[2]=true;
	if(m_AdminAuth[3]==1)
		m_AdminAuth[3]=0;
	else
		m_AdminAuth[3]=1;
}

void CSetupMenu4_Sub2::OnBnClickedCheckSetup4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	b_IsAdminChanged[2]=true;
	if(m_AdminAuth[4]==1)
		m_AdminAuth[4]=0;
	else
		m_AdminAuth[4]=1;
}


void CSetupMenu4_Sub2::OnBnClickedCheckSetup5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	b_IsAdminChanged[2]=true;
	if(m_AdminAuth[5]==1)
		m_AdminAuth[5]=0;
	else
		m_AdminAuth[5]=1;
}


void CSetupMenu4_Sub2::OnBnClickedCheckSetup6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	b_IsAdminChanged[2]=true;
	if(m_AdminAuth[6]==1)
		m_AdminAuth[6]=0;
	else
		m_AdminAuth[6]=1;
}


void CSetupMenu4_Sub2::OnBnClickedCheckSetup7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	b_IsAdminChanged[2]=true;
	if(m_AdminAuth[7]==1)
		m_AdminAuth[7]=0;
	else
		m_AdminAuth[7]=1;
}


void CSetupMenu4_Sub2::OnBnClickedCheckSetup9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	b_IsAdminChanged[2]=true;
	if(m_AdminAuth[9]==1)
		m_AdminAuth[9]=0;
	else
		m_AdminAuth[9]=1;
}


void CSetupMenu4_Sub2::OnBnClickedCheckSetup10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	b_IsAdminChanged[2]=true;
	if(m_AdminAuth[10]==1)
		m_AdminAuth[10]=0;
	else
		m_AdminAuth[10]=1;
}

