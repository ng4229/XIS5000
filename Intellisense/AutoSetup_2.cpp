// AutoSetup_2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "AutoSetup_2.h"
#include "afxdialogex.h"


// CAutoSetup_2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoSetup_2, CDialogEx)

CAutoSetup_2::CAutoSetup_2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoSetup_2::IDD, pParent)
{

	m_PIsNow = 0;
	m_PIsNowXray = 0;
}

CAutoSetup_2::~CAutoSetup_2()
{
}

void CAutoSetup_2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Radio(pDX, IDC_PISNOW, m_PIsNow);
	DDX_Radio(pDX, IDC_PISNOW, m_PIsNow);
	DDX_Radio(pDX, IDC_PISNOWXRAY, m_PIsNowXray);
}


BEGIN_MESSAGE_MAP(CAutoSetup_2, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CAutoSetup_2 메시지 처리기입니다.


BOOL CAutoSetup_2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Brush.CreateStockObject(NULL_BRUSH);
	LoadBackground();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAutoSetup_2::LoadBackground()
{
	BITMAP bmpInfo;
	m_bmpMainBG.LoadBitmap(IDB_BG_AUTOSETUPCHILD); 
    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 

	SetDlgItemText(IDC_STATIC_1, g_MessageArr[15]);
	SetDlgItemText(IDC_STATIC_2, g_MessageArr[16]);
	SetDlgItemText(IDC_PISNOW, g_MessageArr[17]);
	SetDlgItemText(IDC_PISNOW1, g_MessageArr[18]);
	SetDlgItemText(IDC_PISNOWXRAY, g_MessageArr[17]);
	SetDlgItemText(IDC_PISNOWXRAY1, g_MessageArr[18]);
	
}

BOOL CAutoSetup_2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CAutoSetup_2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_STATIC) 
	{
        pOldBrush = (CBrush*)pDC->SelectObject(&Brush);
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH) Brush;
    }
	return hbr;
}


void CAutoSetup_2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}
