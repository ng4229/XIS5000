// AutoSetup_3.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "AutoSetup_3.h"
#include "afxdialogex.h"


// CAutoSetup_3 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoSetup_3, CDialogEx)

CAutoSetup_3::CAutoSetup_3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoSetup_3::IDD, pParent)
{

}

CAutoSetup_3::~CAutoSetup_3()
{
}

void CAutoSetup_3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAutoSetup_3, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CAutoSetup_3 메시지 처리기입니다.


BOOL CAutoSetup_3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Brush.CreateStockObject(NULL_BRUSH);
	LoadBackground();	
	SetTimer(1,1000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAutoSetup_3::LoadBackground()
{
	BITMAP bmpInfo;
	m_bmpMainBG.LoadBitmap(IDB_BG_AUTOSETUPCHILD); 
    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 

	SetDlgItemText(IDC_STATIC_1, g_MessageArr[19]);
	SetDlgItemText(IDC_STATIC_2, g_MessageArr[20]);
	SetDlgItemText(IDC_STATIC_3, g_MessageArr[21]);
	SetDlgItemText(IDC_STATIC_4, g_MessageArr[22]);
	SetDlgItemText(IDC_STATIC_5, g_MessageArr[23]);
	SetDlgItemText(IDC_STATIC_6, g_MessageArr[24]);
	SetDlgItemText(IDC_STATIC_7, g_MessageArr[25]);
	SetDlgItemText(IDC_STATIC_8, g_MessageArr[26]);
	SetDlgItemText(IDC_STATIC_9, g_MessageArr[27]);
	SetDlgItemText(IDC_STATIC_10, g_MessageArr[28]);
}

BOOL CAutoSetup_3::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAutoSetup_3::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


HBRUSH CAutoSetup_3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CAutoSetup_3::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent==1)
	{
		ShowResult();
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CAutoSetup_3::ShowResult(void)
{
	SetDlgItemInt(IDC_PCOUNT, g_AutoCount, true);

	SetDlgItemInt(IDC_PCOND1, g_AutoCond1, true);
	SetDlgItemInt(IDC_PCOND2, g_AutoCond2, true);
	SetDlgItemInt(IDC_PCOND3, g_AutoCond3, true);
	SetDlgItemInt(IDC_PCOND4, g_AutoCond4, true);
	SetDlgItemInt(IDC_PCOND5, g_AutoCond5, true);
	SetDlgItemInt(IDC_PCOND6, g_AutoCond6, true);

	SetDlgItemInt(IDC_PCOND1_1, g_AutoCond1_1, true);
	SetDlgItemInt(IDC_PCOND2_1, g_AutoCond2_1, true);
	SetDlgItemInt(IDC_PCOND3_1, g_AutoCond3_1, true);
	SetDlgItemInt(IDC_PCOND4_1, g_AutoCond4_1, true);
	SetDlgItemInt(IDC_PCOND5_1, g_AutoCond5_1, true);
	SetDlgItemInt(IDC_PCOND6_1, g_AutoCond6_1, true);
}


void CAutoSetup_3::OnDestroy()
{

	KillTimer(1);

	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
