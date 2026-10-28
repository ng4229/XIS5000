// InitailDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "InitailDlg.h"
#include "afxdialogex.h"


// CInitailDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInitailDlg, CDialogEx)

CInitailDlg::CInitailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInitailDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_nID = CInitailDlg::IDD;
	b_IsUse=true;
}

CInitailDlg::~CInitailDlg()
{
	b_IsUse=false;
}

void CInitailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
}

BEGIN_MESSAGE_MAP(CInitailDlg, CDialogEx)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

// CInitailDlg 메시지 처리기입니다.

BOOL CInitailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255,255,255)); 
	if(g_NationalCode==0)
		SetBackgroundImage(IDB_BG_PROGRESS, CDialogEx::BACKGR_TOPLEFT);
	else if(g_NationalCode==1)
		SetBackgroundImage(IDB_BG_PROGRESS_ENG, CDialogEx::BACKGR_TOPLEFT);
	else if(g_NationalCode==2)
		SetBackgroundImage(IDB_BG_PROGRESS_CHI, CDialogEx::BACKGR_TOPLEFT);
	else if(g_NationalCode==3)
		SetBackgroundImage(IDB_BG_PROGRESS_SPN, CDialogEx::BACKGR_TOPLEFT);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 272, 271, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	m_Progress.SetRange(0, 100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CInitailDlg::Create()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::Create(m_nID, m_pParent);
}

BOOL CInitailDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE;
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CInitailDlg::StepIt(int value)
{

	DisplayText();
	m_Progress.SetPos(value);
}


void CInitailDlg::DisplayText(void)
{
	GetDlgItem(IDC_EDIT_TEXT)->SetWindowText(m_Text);
}


void CInitailDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialogEx::OnKillFocus(pNewWnd);

	if(b_IsUse)
	{
		this->ShowWindow(SW_SHOW);
		this->SetFocus();
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
