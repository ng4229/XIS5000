// ExitDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Intellisense.h"
#include "ExitDlg.h"
#include "afxdialogex.h"
// CExitDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CExitDlg, CDialogEx)

CExitDlg::CExitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExitDlg::IDD, pParent)
{

}

CExitDlg::~CExitDlg()
{
}

void CExitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_EXITPROGRAM, m_BtMenu2);
	DDX_Control(pDX, IDC_BT_TURNOFF, m_BtMenu1);
	DDX_Control(pDX, IDCANCEL, m_BtMenu3);
}


BEGIN_MESSAGE_MAP(CExitDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BT_TURNOFF, &CExitDlg::OnBnClickedBtTurnoff)
	ON_BN_CLICKED(IDC_BT_EXITPROGRAM, &CExitDlg::OnBnClickedBtExitprogram)
	ON_BN_CLICKED(IDCANCEL, &CExitDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CExitDlg 메시지 처리기입니다.
BOOL CExitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 272, 271, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	LoadBackground();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CExitDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;	

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CExitDlg::OnBnClickedBtTurnoff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(11);
}

void CExitDlg::OnBnClickedBtExitprogram()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(22);
}

void CExitDlg::LoadBackground()
{
	SetBackgroundColor(RGB(255,255,255)); 
	SetBackgroundImage(IDB_BG_EXIT, CDialogEx::BACKGR_TOPLEFT);

	m_BtMenu1.LoadBitmaps(IDB_BTEXITMENU1,IDB_BTEXITMENU1_1); 
	m_BtMenu1.SizeToContent();
	m_BtMenu2.LoadBitmaps(IDB_BTEXITMENU2,IDB_BTEXITMENU2_1); 
	m_BtMenu2.SizeToContent();
	m_BtMenu3.LoadBitmaps(IDB_BTEXITMENU3,IDB_BTEXITMENU3_1); 
	m_BtMenu3.SizeToContent();

	SetDlgItemText(IDC_STATIC_1, g_MessageArr[186]);
	SetDlgItemText(IDC_STATIC_2, g_MessageArr[187]);	
	SetDlgItemText(IDC_STATIC_3, g_MessageArr[188]);	
}

void CExitDlg::OnBnClickedCancel()
{

	CDialogEx::OnCancel();
}
