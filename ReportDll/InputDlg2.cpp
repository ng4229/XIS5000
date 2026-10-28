// InputDlg2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InputDlg2.h"
#include "afxdialogex.h"


// CInputDlg2 대화 상자입니다.
CString m_strCombo21[] = {_T("X-ray"), _T("디텍터"), _T("컴퓨터"), _T("모니터"), _T("컨베이어벨트"), _T("경광등")};
CString m_strCombo21Eng[] = {_T("X-ray"), _T("Detector"), _T("Computer"), _T("Monitor"), _T("Conveyor belt"), _T("Alarm lamp")};
CString m_strCombo21Chi[] = {_T("X线发生器"), _T("探测器"), _T("电脑"), _T("显示器"), _T("输送带"), _T("警灯")};

IMPLEMENT_DYNAMIC(CInputDlg2, CDialogEx)

CInputDlg2::CInputDlg2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputDlg2::IDD, pParent)
{

	m_Combo_1_Index = 0;
	m_Input1_Date = COleDateTime::GetCurrentTime();
	m_Input1_Time = _T("24");
}

CInputDlg2::~CInputDlg2()
{
}

void CInputDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Combo_1);
	DDX_CBIndex(pDX, IDC_COMBO1, m_Combo_1_Index);
	DDX_DateTimeCtrl(pDX, IDC_INPUT1_DATE, m_Input1_Date);
	DDX_Text(pDX, IDC_INPUT1_TIME, m_Input1_Time);
}


BEGIN_MESSAGE_MAP(CInputDlg2, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CInputDlg2::OnSelchangeCombo1)
END_MESSAGE_MAP()


// CInputDlg2 메시지 처리기입니다.


BOOL CInputDlg2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(m_RNationalCode==1)
	{
		SetDlgItemText(IDC_STATIC_5, _T("Main Parts"));
		SetDlgItemText(IDC_STATIC_1, _T("Model     :"));
		SetDlgItemText(IDC_STATIC_2, _T("Installed :"));
		SetDlgItemText(IDC_STATIC_3, _T("Cycle time:"));	
		SetDlgItemText(IDC_STATIC_4, _T("Months"));	
	}
	else if(m_RNationalCode==2)
	{
		SetDlgItemText(IDC_STATIC_5, _T("主要零件 "));
		SetDlgItemText(IDC_STATIC_1, _T("型    号 :"));
		SetDlgItemText(IDC_STATIC_2, _T("安装日期 :"));
		SetDlgItemText(IDC_STATIC_3, _T("更换周期 :"));	
		SetDlgItemText(IDC_STATIC_4, _T("个月"));	
	}
	else
	{
		SetDlgItemText(IDC_STATIC_5, _T("주요부품"));
		SetDlgItemText(IDC_STATIC_1, _T("부 품 명 :"));
		SetDlgItemText(IDC_STATIC_2, _T("설치일자 :"));
		SetDlgItemText(IDC_STATIC_3, _T("교체주기 :"));	
		SetDlgItemText(IDC_STATIC_4, _T("개월"));	
	}
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	for(int i=0 ; i<6 ; i++)
	{
		//20181011
		if(m_RNationalCode==1)
		{
			m_Combo_1.AddString(m_strCombo21Eng[i]);
			GetDlgItem(IDOK)->SetWindowTextW(_T("Save"));
			GetDlgItem(IDCANCEL)->SetWindowTextW(_T("Close"));
		}
		else if(m_RNationalCode==2)
		{
			m_Combo_1.AddString(m_strCombo21Chi[i]);
			GetDlgItem(IDOK)->SetWindowTextW(_T("保存"));
			GetDlgItem(IDCANCEL)->SetWindowTextW(_T("关闭"));
		}
		else
		{
			m_Combo_1.AddString(m_strCombo21[i]);
			GetDlgItem(IDOK)->SetWindowTextW(_T("저장"));
			GetDlgItem(IDCANCEL)->SetWindowTextW(_T("닫기"));
		}
	}
	m_Combo_1.SetCurSel(m_Combo_1_Index);	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

#include "../Keyboard/KeyboardDlg.h"

BOOL CInputDlg2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;
	
	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_INPUT1_TIME)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CKeyboardDlg dlg;
			dlg.m_NationalCode = m_RNationalCode;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				GetDlgItem(IDC_INPUT1_TIME)->SetWindowText(dlg.m_TestName);
			}
			else
				GetDlgItem(IDC_INPUT1_TIME)->SetWindowText(_T(""));
		}
	}	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CInputDlg2::OnSelchangeCombo1()
{
	UpdateData(true);
}

