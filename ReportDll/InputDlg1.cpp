// InputDlg1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InputDlg1.h"
#include "afxdialogex.h"


// CInputDlg1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInputDlg1, CDialogEx)

CInputDlg1::CInputDlg1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputDlg1::IDD, pParent)
{

	m_Input2_Date = COleDateTime::GetCurrentTime();
	m_Input2_Time = _T("");
}

CInputDlg1::~CInputDlg1()
{
}

void CInputDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_INPUT2_DATE, m_Input2_Date);
	DDX_Text(pDX, IDC_INPUT2_TIME, m_Input2_Time);
}


BEGIN_MESSAGE_MAP(CInputDlg1, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInputDlg1::OnBnClickedOk)
END_MESSAGE_MAP()


// CInputDlg1 메시지 처리기입니다.


BOOL CInputDlg1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if(m_RNationalCode==1)
	{
		SetDlgItemText(IDC_STATIC_1, _T("X-ray Generator"));
		SetDlgItemText(IDC_STATIC_2, _T("Installed :"));
		SetDlgItemText(IDC_STATIC_3, _T("Used time :"));	
	}
	else if(m_RNationalCode==2)
	{
		SetDlgItemText(IDC_STATIC_1, _T("X-ray 信息"));
		SetDlgItemText(IDC_STATIC_2, _T("安装日期 :"));
		SetDlgItemText(IDC_STATIC_3, _T("使用时间 :"));	
	}
	else
	{
		SetDlgItemText(IDC_STATIC_1, _T("X-ray 발생장치"));
		SetDlgItemText(IDC_STATIC_2, _T("설치일자 :"));
		SetDlgItemText(IDC_STATIC_3, _T("사용시간 :"));	
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

#include "../Keyboard/KeyboardDlg.h"

BOOL CInputDlg1::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;
	
	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_INPUT2_TIME)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CKeyboardDlg dlg;
			dlg.m_NationalCode = m_RNationalCode;
			int result = dlg.DoModal();
			//ShellExecute(NULL, L"open", L"C:\\Windows\\system32\\osk.exe", NULL, NULL, SW_NORMAL);
			if(result == IDOK)
			{
				GetDlgItem(IDC_INPUT2_TIME)->SetWindowText(dlg.m_TestName);
			}
			else
				GetDlgItem(IDC_INPUT2_TIME)->SetWindowText(_T(""));
		}
	}
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CInputDlg1::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
