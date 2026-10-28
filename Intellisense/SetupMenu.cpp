// SetupMenu.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "SetupMenu.h"
#include "afxdialogex.h"

#include "MessageBoxDlg.h"
#include "SetupMenu_0.h"
#include "SetupMenu_1.h"
#include "SetupMenu_2.h"
#include "SetupMenu_3.h"
#include "SetupMenu_4.h"

// CSetupMenu 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMenu, CDialogEx)

CSetupMenu::CSetupMenu(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMenu::IDD, pParent)
{

}

CSetupMenu::~CSetupMenu()
{
}

void CSetupMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_ADMIN, m_Bt_Admin);
	DDX_Control(pDX, IDC_BT_AUTOSETUP, m_Bt_Autosetup);
	DDX_Control(pDX, IDC_BT_CONDITION, m_Bt_Condition);
	DDX_Control(pDX, IDC_BT_DETECTOR, m_Bt_Detector);
	DDX_Control(pDX, IDC_BT_LOGOUT, m_Bt_Login);
	DDX_Control(pDX, IDC_BT_MAINPARTS, m_Bt_Mainparts);
	DDX_Control(pDX, IDC_BT_PRODUCT, m_Bt_Product);
	DDX_Control(pDX, IDC_BT_REJECT, m_Bt_Reject);
	DDX_Control(pDX, IDC_BT_REPORT, m_Bt_Report);
	DDX_Control(pDX, IDC_BT_SAVE, m_Bt_Save);
	DDX_Control(pDX, IDC_BT_XRAY, m_Bt_Xray);
	DDX_Control(pDX, IDCANCEL, m_Bt_Close);
}


BEGIN_MESSAGE_MAP(CSetupMenu, CDialogEx)
	ON_BN_CLICKED(IDC_BT_LOGOUT, &CSetupMenu::OnBnClickedBtLogout)
	ON_BN_CLICKED(IDC_BT_PRODUCT, &CSetupMenu::OnBnClickedBtProduct)
	ON_BN_CLICKED(IDC_BT_CONDITION, &CSetupMenu::OnBnClickedBtCondition)
	ON_BN_CLICKED(IDC_BT_XRAY, &CSetupMenu::OnBnClickedBtXray)
	ON_BN_CLICKED(IDC_BT_DETECTOR, &CSetupMenu::OnBnClickedBtDetector)
	ON_BN_CLICKED(IDC_BT_REJECT, &CSetupMenu::OnBnClickedBtReject)
	ON_BN_CLICKED(IDC_BT_SAVE, &CSetupMenu::OnBnClickedBtSave)
	ON_BN_CLICKED(IDC_BT_AUTOSETUP, &CSetupMenu::OnBnClickedBtAutosetup)
	ON_BN_CLICKED(IDC_BT_ADMIN, &CSetupMenu::OnBnClickedBtAdmin)
	ON_BN_CLICKED(IDC_BT_REPORT, &CSetupMenu::OnBnClickedBtReport)
	ON_BN_CLICKED(IDC_BT_MAINPARTS, &CSetupMenu::OnBnClickedBtMainparts)
	ON_BN_CLICKED(IDCANCEL, &CSetupMenu::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSetupMenu 메시지 처리기입니다.

BOOL CSetupMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	SetBackgroundColor(RGB(45,65,104)); 
	SetWindowPos(NULL, 672, 119, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	LoadBackground();
	LoadList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupMenu::LoadBackground()
{
	int mainbgnum=0;
	if(g_NationalCode==0)
	{
		mainbgnum=IDB_BG_SETUPMENU; 
		m_Bt_Close.LoadBitmaps(IDB_BT_SETUPMENU_43,IDB_BT_SETUPMENU_43_1); 
		if(!g_bAdmin)
			m_Bt_Login.LoadBitmaps(IDB_BT_SETUPMENU_11,IDB_BT_SETUPMENU_11); 
		else
			m_Bt_Login.LoadBitmaps(IDB_BT_SETUPMENU_111,IDB_BT_SETUPMENU_111);
			m_Bt_Product.LoadBitmaps(IDB_BT_SETUPMENU_12,IDB_BT_SETUPMENU_12); 
			m_Bt_Condition.LoadBitmaps(IDB_BT_SETUPMENU_13,IDB_BT_SETUPMENU_13);
			m_Bt_Xray.LoadBitmaps(IDB_BT_SETUPMENU_21,IDB_BT_SETUPMENU_21); 
			m_Bt_Detector.LoadBitmaps(IDB_BT_SETUPMENU_22,IDB_BT_SETUPMENU_22);
			m_Bt_Reject.LoadBitmaps(IDB_BT_SETUPMENU_23,IDB_BT_SETUPMENU_23); 
			m_Bt_Save.LoadBitmaps(IDB_BT_SETUPMENU_31,IDB_BT_SETUPMENU_31); 
			m_Bt_Autosetup.LoadBitmaps(IDB_BT_SETUPMENU_32,IDB_BT_SETUPMENU_32); 
			m_Bt_Admin.LoadBitmaps(IDB_BT_SETUPMENU_33,IDB_BT_SETUPMENU_33); 
			m_Bt_Report.LoadBitmaps(IDB_BT_SETUPMENU_41,IDB_BT_SETUPMENU_41); 
			m_Bt_Mainparts.LoadBitmaps(IDB_BT_SETUPMENU_51,IDB_BT_SETUPMENU_51);
	}
	else if(g_NationalCode==1)
	{
		mainbgnum=IDB_BG_SETUPMENU_ENG; 
		m_Bt_Close.LoadBitmaps(IDB_BT_SETUPMENU_43_ENG,IDB_BT_SETUPMENU_43_1_ENG); 
		if(!g_bAdmin)
			m_Bt_Login.LoadBitmaps(IDB_BT_SETUPMENU_11_ENG,IDB_BT_SETUPMENU_11_ENG);
		else
			m_Bt_Login.LoadBitmaps(IDB_BT_SETUPMENU_111_ENG,IDB_BT_SETUPMENU_111_ENG); 
			m_Bt_Product.LoadBitmaps(IDB_BT_SETUPMENU_12_ENG,IDB_BT_SETUPMENU_12_ENG); 
			m_Bt_Condition.LoadBitmaps(IDB_BT_SETUPMENU_13_ENG,IDB_BT_SETUPMENU_13_ENG);
			m_Bt_Xray.LoadBitmaps(IDB_BT_SETUPMENU_21_ENG,IDB_BT_SETUPMENU_21_ENG); 
			m_Bt_Detector.LoadBitmaps(IDB_BT_SETUPMENU_22_ENG,IDB_BT_SETUPMENU_22_ENG);
			m_Bt_Reject.LoadBitmaps(IDB_BT_SETUPMENU_23_ENG,IDB_BT_SETUPMENU_23_ENG); 
			m_Bt_Save.LoadBitmaps(IDB_BT_SETUPMENU_31_ENG,IDB_BT_SETUPMENU_31_ENG); 
			m_Bt_Autosetup.LoadBitmaps(IDB_BT_SETUPMENU_32_ENG,IDB_BT_SETUPMENU_32_ENG); 
			m_Bt_Admin.LoadBitmaps(IDB_BT_SETUPMENU_33_ENG,IDB_BT_SETUPMENU_33_ENG); 
			m_Bt_Report.LoadBitmaps(IDB_BT_SETUPMENU_41_ENG,IDB_BT_SETUPMENU_41_ENG); 
			m_Bt_Mainparts.LoadBitmaps(IDB_BT_SETUPMENU_51_ENG,IDB_BT_SETUPMENU_51_ENG);
	}
	else if(g_NationalCode==2)
	{
		mainbgnum=IDB_BG_SETUPMENU_CHI; 
		m_Bt_Close.LoadBitmaps(IDB_BT_SETUPMENU_43_CHI,IDB_BT_SETUPMENU_43_1_CHI); 
		if(!g_bAdmin)
			m_Bt_Login.LoadBitmaps(IDB_BT_SETUPMENU_11_CHI,IDB_BT_SETUPMENU_11_CHI);
		else
			m_Bt_Login.LoadBitmaps(IDB_BT_SETUPMENU_111_CHI,IDB_BT_SETUPMENU_111_CHI); 
			m_Bt_Product.LoadBitmaps(IDB_BT_SETUPMENU_12_CHI,IDB_BT_SETUPMENU_12_CHI); 
			m_Bt_Condition.LoadBitmaps(IDB_BT_SETUPMENU_13_CHI,IDB_BT_SETUPMENU_13_CHI);
			m_Bt_Xray.LoadBitmaps(IDB_BT_SETUPMENU_21_CHI,IDB_BT_SETUPMENU_21_CHI); 
			m_Bt_Detector.LoadBitmaps(IDB_BT_SETUPMENU_22_CHI,IDB_BT_SETUPMENU_22_CHI);
			m_Bt_Reject.LoadBitmaps(IDB_BT_SETUPMENU_23_CHI,IDB_BT_SETUPMENU_23_CHI); 
			m_Bt_Save.LoadBitmaps(IDB_BT_SETUPMENU_31_CHI,IDB_BT_SETUPMENU_31_CHI); 
			m_Bt_Autosetup.LoadBitmaps(IDB_BT_SETUPMENU_32_CHI,IDB_BT_SETUPMENU_32_CHI); 
			m_Bt_Admin.LoadBitmaps(IDB_BT_SETUPMENU_33_CHI,IDB_BT_SETUPMENU_33_CHI); 
			m_Bt_Report.LoadBitmaps(IDB_BT_SETUPMENU_41_CHI,IDB_BT_SETUPMENU_41_CHI); 
			m_Bt_Mainparts.LoadBitmaps(IDB_BT_SETUPMENU_51_CHI,IDB_BT_SETUPMENU_51_CHI);
	}
	else if(g_NationalCode==3)
	{
		mainbgnum=IDB_BG_SETUPMENU_SPN; 
		m_Bt_Close.LoadBitmaps(IDB_BT_SETUPMENU_43_SPN,IDB_BT_SETUPMENU_43_1_SPN); 
		if(!g_bAdmin)
			m_Bt_Login.LoadBitmaps(IDB_BT_SETUPMENU_11_SPN,IDB_BT_SETUPMENU_11_SPN);
		else
			m_Bt_Login.LoadBitmaps(IDB_BT_SETUPMENU_111_SPN,IDB_BT_SETUPMENU_111_SPN); 
			m_Bt_Product.LoadBitmaps(IDB_BT_SETUPMENU_12_SPN,IDB_BT_SETUPMENU_12_SPN); 
			m_Bt_Condition.LoadBitmaps(IDB_BT_SETUPMENU_13_SPN,IDB_BT_SETUPMENU_13_SPN);
			m_Bt_Xray.LoadBitmaps(IDB_BT_SETUPMENU_21_SPN,IDB_BT_SETUPMENU_21_SPN); 
			m_Bt_Detector.LoadBitmaps(IDB_BT_SETUPMENU_22_SPN,IDB_BT_SETUPMENU_22_SPN);
			m_Bt_Reject.LoadBitmaps(IDB_BT_SETUPMENU_23_SPN,IDB_BT_SETUPMENU_23_SPN); 
			m_Bt_Save.LoadBitmaps(IDB_BT_SETUPMENU_31_SPN,IDB_BT_SETUPMENU_31_SPN); 
			m_Bt_Autosetup.LoadBitmaps(IDB_BT_SETUPMENU_32_SPN,IDB_BT_SETUPMENU_32_SPN); 
			m_Bt_Admin.LoadBitmaps(IDB_BT_SETUPMENU_33_SPN,IDB_BT_SETUPMENU_33_SPN); 
			m_Bt_Report.LoadBitmaps(IDB_BT_SETUPMENU_41_SPN,IDB_BT_SETUPMENU_41_SPN); 
			m_Bt_Mainparts.LoadBitmaps(IDB_BT_SETUPMENU_51_SPN,IDB_BT_SETUPMENU_51_SPN);
	}
	SetBackgroundImage(mainbgnum, CDialogEx::BACKGR_TOPLEFT);

	m_Bt_Close.SizeToContent();	
	m_Bt_Login.SizeToContent();	
	m_Bt_Product.SizeToContent();	
	m_Bt_Condition.SizeToContent();	
	m_Bt_Xray.SizeToContent();	
	m_Bt_Detector.SizeToContent();	
	m_Bt_Reject.SizeToContent();	
	m_Bt_Save.SizeToContent();	
	m_Bt_Autosetup.SizeToContent();	
	m_Bt_Admin.SizeToContent();	
	m_Bt_Report.SizeToContent();	
	m_Bt_Mainparts.SizeToContent();
}

void CSetupMenu::LoadList(void)
{
	if(g_bAdmin)
	{
		if(g_XrayState==1 || g_XrayState==2)
		{	
			m_Bt_Report.ShowWindow(false);
			m_Bt_Mainparts.ShowWindow(false);
		}
	}
	else
	{
		m_Bt_Condition.ShowWindow(false);
		m_Bt_Xray.ShowWindow(false);
		m_Bt_Detector.ShowWindow(false);
		m_Bt_Reject.ShowWindow(false);
		m_Bt_Save.ShowWindow(false);
		m_Bt_Autosetup.ShowWindow(false);
		m_Bt_Admin.ShowWindow(false);
		m_Bt_Report.ShowWindow(false);
		m_Bt_Mainparts.ShowWindow(false);	
	}
}


BOOL CSetupMenu::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSetupMenu::OnBnClickedBtLogout()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(g_bAdmin)
	{
		g_bAdmin = false;
		AfxMessageBox(g_MessageArr[156]);//_T("로그아웃 되었습니다."));
		EndDialog(111);
	}
	else
	{
		EndDialog(11);
	}
}


void CSetupMenu::OnBnClickedBtProduct()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(g_XrayState==1 || g_XrayState==2)
	{
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[157];//"생산중에 변경할 수 없습니다.";
		dlg.DoModal();
		EndDialog(122);
	}
	else
		EndDialog(12);
}


void CSetupMenu::OnBnClickedBtCondition()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(13);
}


void CSetupMenu::OnBnClickedBtXray()
{
	CSetupMenu_0 dlg;
	int result = dlg.DoModal();
	if(result==33)
		EndDialog(33);
}


void CSetupMenu::OnBnClickedBtDetector()
{
	CSetupMenu_1 dlg;
	dlg.DoModal();
}


void CSetupMenu::OnBnClickedBtReject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSetupMenu_2 dlg;
	dlg.DoModal();
}


void CSetupMenu::OnBnClickedBtSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSetupMenu_3 dlg;
	dlg.DoModal();
}

void CSetupMenu::OnBnClickedBtAutosetup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMessageBoxDlg dlg;
	dlg.m_Mode=2;
	dlg.m_Message=g_MessageArr[158];//"검사값 자동설정을 계속하시려면 '확인' 버튼을 그렇지 않으면 '닫기' 버튼을 눌러주세요.\n검사값 자동설정을 하시면 이전의 검사값 기록은 삭제되니 주의하십시요";
	int result=dlg.DoModal();
	if(result==IDOK)
		EndDialog(32);
	else if(result==IDCANCEL)
		EndDialog(333);
}

void CSetupMenu::OnBnClickedBtAdmin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSetupMenu_4 dlg;
	int result = dlg.DoModal();
	if(result==IDOK)
	{
		EndDialog(66);
	}
}

void CSetupMenu::OnBnClickedBtReport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(41);
}


void CSetupMenu::OnBnClickedBtMainparts()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(42);
}


void CSetupMenu::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}



HBRUSH CSetupMenu::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if(nCtlColor == CTLCOLOR_STATIC) 
	{
		pDC->SetTextColor(RGB(255,255,255));
	}

	return hbr;
}
