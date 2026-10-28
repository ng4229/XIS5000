// AutoSetup_0.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "AutoSetup_0.h"
#include "afxdialogex.h"

#include "../Keyboard/KeyboardDlg.h"

// CAutoSetup_0 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoSetup_0, CDialogEx)

CAutoSetup_0::CAutoSetup_0(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoSetup_0::IDD, pParent)
{
	m_Pcopy = 1;
	m_Pname = _T("");
}

CAutoSetup_0::~CAutoSetup_0()
{
}

void CAutoSetup_0::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_COPY, m_Pcopy);
	DDX_Text(pDX, IDC_PNAME, m_Pname);
}


BEGIN_MESSAGE_MAP(CAutoSetup_0, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAutoSetup_0 메시지 처리기입니다.


BOOL CAutoSetup_0::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	Brush.CreateStockObject(NULL_BRUSH);
	LoadBackground();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CAutoSetup_0::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_PNAME)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{		
			
			// keyboard
			CKeyboardDlg dlg;
			dlg.m_NationalCode = g_NationalCode;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_Pname = dlg.m_TestName;
				GetDlgItem(IDC_PNAME)->SetWindowText(m_Pname);
			}
			else
			{
				GetDlgItem(IDC_PNAME)->SetWindowText(_T(""));
			}
			return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAutoSetup_0::LoadBackground()
{
	BITMAP bmpInfo;
	m_bmpMainBG.LoadBitmap(IDB_BG_AUTOSETUPCHILD); 
    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight;
	
	SetDlgItemText(IDC_STATIC_1, g_MessageArr[8]);
	SetDlgItemText(IDC_STATIC_2, g_MessageArr[9]);
	SetDlgItemText(IDC_COPY, g_MessageArr[10]);
	SetDlgItemText(IDC_COPY1, g_MessageArr[11]);
}

void CAutoSetup_0::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
    CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


HBRUSH CAutoSetup_0::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
    if(nCtlColor == CTLCOLOR_STATIC) 
	{
        pOldBrush = (CBrush*)pDC->SelectObject(&Brush);
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH) Brush;
    }
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}
