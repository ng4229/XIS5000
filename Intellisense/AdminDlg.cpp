// AdminDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "AdminDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CtrlPanelView.h"

// CAdminDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAdminDlg, CDialogEx)

CAdminDlg::CAdminDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAdminDlg::IDD, pParent)
{
	m_Password = _T("");
	m_Mode=1;
}

CAdminDlg::~CAdminDlg()
{
}

void CAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_ADMIN0, m_Bt_Admin0);
	DDX_Control(pDX, IDC_BT_ADMIN1, m_Bt_Admin1);
	DDX_Control(pDX, IDC_BT_ADMIN2, m_Bt_Admin2);
	DDX_Control(pDX, IDC_BT_ADMIN3, m_Bt_Admin3);
	DDX_Control(pDX, IDC_BT_ADMIN4, m_Bt_Admin4);
	DDX_Control(pDX, IDC_BT_ADMIN5, m_Bt_Admin5);
	DDX_Control(pDX, IDC_BT_ADMIN6, m_Bt_Admin6);
	DDX_Control(pDX, IDC_BT_ADMIN7, m_Bt_Admin7);
	DDX_Control(pDX, IDC_BT_ADMIN8, m_Bt_Admin8);
	DDX_Control(pDX, IDC_BT_ADMIN9, m_Bt_Admin9);
	DDX_Control(pDX, IDC_BT_RESET, m_Bt_Reset);
	DDX_Control(pDX, IDCANCEL, m_Bt_Close);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_Password);
}


BEGIN_MESSAGE_MAP(CAdminDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BT_ADMIN1, &CAdminDlg::OnBnClickedBtAdmin1)
	ON_BN_CLICKED(IDC_BT_ADMIN2, &CAdminDlg::OnBnClickedBtAdmin2)
	ON_BN_CLICKED(IDC_BT_ADMIN3, &CAdminDlg::OnBnClickedBtAdmin3)
	ON_BN_CLICKED(IDC_BT_ADMIN4, &CAdminDlg::OnBnClickedBtAdmin4)
	ON_BN_CLICKED(IDC_BT_ADMIN5, &CAdminDlg::OnBnClickedBtAdmin5)
	ON_BN_CLICKED(IDC_BT_ADMIN6, &CAdminDlg::OnBnClickedBtAdmin6)
	ON_BN_CLICKED(IDC_BT_ADMIN7, &CAdminDlg::OnBnClickedBtAdmin7)
	ON_BN_CLICKED(IDC_BT_ADMIN8, &CAdminDlg::OnBnClickedBtAdmin8)
	ON_BN_CLICKED(IDC_BT_ADMIN9, &CAdminDlg::OnBnClickedBtAdmin9)
	ON_BN_CLICKED(IDC_BT_ADMIN0, &CAdminDlg::OnBnClickedBtAdmin0)
	ON_BN_CLICKED(IDC_BT_RESET, &CAdminDlg::OnBnClickedBtReset)
END_MESSAGE_MAP()


// CAdminDlg 메시지 처리기입니다.


BOOL CAdminDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(255,255,255)); 
	SetWindowPos(NULL, 672, 119, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	LoadBackground();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CAdminDlg::LoadBackground()
{
	int mainbgnum=0;
	if(g_NationalCode==0)
	{
		mainbgnum=IDB_BG_ADMIN;
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT,IDB_SELPRODUCT_EXIT_0); 
		m_Bt_Reset.LoadBitmaps(IDB_BTADMINREINPUT,IDB_BTADMINREINPUT_1); 
	}
	else if(g_NationalCode==1)
	{
		mainbgnum=IDB_BG_ADMIN_ENG;
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_ENG,IDB_SELPRODUCT_EXIT_0_ENG); 
		m_Bt_Reset.LoadBitmaps(IDB_BTADMINREINPUT_ENG,IDB_BTADMINREINPUT_1_ENG); 
	}
    else if(g_NationalCode==2)
	{
		mainbgnum=IDB_BG_ADMIN_CHI;
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_CHI,IDB_SELPRODUCT_EXIT_0_CHI); 
		m_Bt_Reset.LoadBitmaps(IDB_BTADMINREINPUT_CHI,IDB_BTADMINREINPUT_1_CHI); 
	}
	else if(g_NationalCode==3)
	{
		mainbgnum=IDB_BG_ADMIN_SPN;
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_SPN,IDB_SELPRODUCT_EXIT_0_SPN); 
		m_Bt_Reset.LoadBitmaps(IDB_BTADMINREINPUT_SPN,IDB_BTADMINREINPUT_1_SPN); 
	}
	SetBackgroundImage(mainbgnum, CDialogEx::BACKGR_TOPLEFT);

	m_Bt_Admin0.LoadBitmaps(IDB_ADMIN0,IDB_ADMIN0_1); 
	m_Bt_Admin0.SizeToContent();	
	m_Bt_Admin1.LoadBitmaps(IDB_ADMIN1,IDB_ADMIN1_1); 
	m_Bt_Admin1.SizeToContent();	
	m_Bt_Admin2.LoadBitmaps(IDB_ADMIN2,IDB_ADMIN2_1); 
	m_Bt_Admin2.SizeToContent();	
	m_Bt_Admin3.LoadBitmaps(IDB_ADMIN3,IDB_ADMIN3_1); 
	m_Bt_Admin3.SizeToContent();	
	m_Bt_Admin4.LoadBitmaps(IDB_ADMIN4,IDB_ADMIN4_1); 
	m_Bt_Admin4.SizeToContent();	
	m_Bt_Admin5.LoadBitmaps(IDB_ADMIN5,IDB_ADMIN5_1); 
	m_Bt_Admin5.SizeToContent();	
	m_Bt_Admin6.LoadBitmaps(IDB_ADMIN6,IDB_ADMIN6_1); 
	m_Bt_Admin6.SizeToContent();	
	m_Bt_Admin7.LoadBitmaps(IDB_ADMIN7,IDB_ADMIN7_1); 
	m_Bt_Admin7.SizeToContent();	
	m_Bt_Admin8.LoadBitmaps(IDB_ADMIN8,IDB_ADMIN8_1); 
	m_Bt_Admin8.SizeToContent();	
	m_Bt_Admin9.LoadBitmaps(IDB_ADMIN9,IDB_ADMIN9_1); 
	m_Bt_Admin9.SizeToContent();	
	m_Bt_Close.SizeToContent();		
	m_Bt_Reset.SizeToContent();	
}

void CAdminDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}

BOOL CAdminDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CAdminDlg::OnBnClickedBtAdmin1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Password += "1";
	UpdateData(false);
	if(m_Mode==1 && m_Password.GetLength()==4)
		OnCompare();
	else if(m_Mode==2 && m_Password.GetLength()==4)
		EndDialog(2);
}


void CAdminDlg::OnBnClickedBtAdmin2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Password += "2";
	UpdateData(false);
	if(m_Mode==1 && m_Password.GetLength()==4)
		OnCompare();
	else if(m_Mode==2 && m_Password.GetLength()==4)
		EndDialog(2);
}


void CAdminDlg::OnBnClickedBtAdmin3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Password += "3";
	UpdateData(false);
	if(m_Mode==1 && m_Password.GetLength()==4)
		OnCompare();
	else if(m_Mode==2 && m_Password.GetLength()==4)
		EndDialog(2);
}


void CAdminDlg::OnBnClickedBtAdmin4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Password += "4";
	UpdateData(false);
	if(m_Mode==1 && m_Password.GetLength()==4)
		OnCompare();
	else if(m_Mode==2 && m_Password.GetLength()==4)
		EndDialog(2);
}


void CAdminDlg::OnBnClickedBtAdmin5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Password += "5";
	UpdateData(false);
	if(m_Mode==1 && m_Password.GetLength()==4)
		OnCompare();
	else if(m_Mode==2 && m_Password.GetLength()==4)
		EndDialog(2);
}


void CAdminDlg::OnBnClickedBtAdmin6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Password += "6";
	UpdateData(false);
	if(m_Mode==1 && m_Password.GetLength()==4)
		OnCompare();
	else if(m_Mode==2 && m_Password.GetLength()==4)
		EndDialog(2);
}


void CAdminDlg::OnBnClickedBtAdmin7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Password += "7";
	UpdateData(false);
	if(m_Mode==1 && m_Password.GetLength()==4)
		OnCompare();
	else if(m_Mode==2 && m_Password.GetLength()==4)
		EndDialog(2);
}


void CAdminDlg::OnBnClickedBtAdmin8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Password += "8";
	UpdateData(false);
	if(m_Mode==1 && m_Password.GetLength()==4)
		OnCompare();
	else if(m_Mode==2 && m_Password.GetLength()==4)
		EndDialog(2);
}


void CAdminDlg::OnBnClickedBtAdmin9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Password += "9";
	UpdateData(false);
	if(m_Mode==1 && m_Password.GetLength()==4)
		OnCompare();
	else if(m_Mode==2 && m_Password.GetLength()==4)
		EndDialog(2);
}


void CAdminDlg::OnBnClickedBtAdmin0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Password += "0";
	UpdateData(false);
	if(m_Mode==1 && m_Password.GetLength()==4)
		OnCompare();
	else if(m_Mode==2 && m_Password.GetLength()==4)
		EndDialog(2);
}


void CAdminDlg::OnBnClickedBtReset()
{
	m_Password="";
	UpdateData(false);
}


void CAdminDlg::OnCompare(void)
{
	if(m_Password==g_Password)
	{
		g_bAdmin = true;
		m_Password="";
		if(g_NationalCode==1)
		{
			CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
			CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();
			pCtrlView->m_Bt_ResetCount.EnableWindow(true);
		}
		EndDialog(1);
	}
	else
	{
		g_bAdmin = false;
		m_Password="";
		UpdateData(false);
	}
}
