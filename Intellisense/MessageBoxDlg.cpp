// MessageBoxDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "MessageBoxDlg.h"
#include "afxdialogex.h"


// CMessageBoxDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMessageBoxDlg, CDialogEx)

CMessageBoxDlg::CMessageBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessageBoxDlg::IDD, pParent)
{
	m_Message=_T("");
	//mode 1 : 닫기 mode 2 : 예, 아니오
	m_Mode=1; 
}

CMessageBoxDlg::~CMessageBoxDlg()
{
}

void CMessageBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_Bt_Ok);
	DDX_Control(pDX, IDCANCEL, m_Bt_Cancel);
}


BEGIN_MESSAGE_MAP(CMessageBoxDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMessageBoxDlg 메시지 처리기입니다.


BOOL CMessageBoxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Brush.CreateStockObject(NULL_BRUSH);
	SetWindowPos(NULL, 672, 119, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	LoadBackgound();
	GetDlgItem(IDC_MESSAGE)->SetWindowText(m_Message);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMessageBoxDlg::OnPaint()
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


HBRUSH CMessageBoxDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
    if(nCtlColor == CTLCOLOR_STATIC) 
	{
		pDC->SetTextColor(RGB(255,255,255));  //글씨색 변경	
        pOldBrush = (CBrush*)pDC->SelectObject(&Brush);
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH) Brush;
    }
	return hbr;
}


void CMessageBoxDlg::LoadBackgound(void)
{
	BITMAP bmpInfo;
	if(m_Mode==1)
	{
		m_Bt_Ok.ShowWindow(false);
	}
	if(g_NationalCode==0)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_MESSAGEBOX); 
		
		if(m_Mode==2)
		{
			m_Bt_Ok.LoadBitmaps(IDB_BTSETUPMENU_OK,IDB_BTSETUPMENU_OK_0); 
			m_Bt_Ok.SizeToContent();	
		}
		m_Bt_Cancel.LoadBitmaps(IDB_SELPRODUCT_EXIT,IDB_SELPRODUCT_EXIT_0); 
	}
	else if(g_NationalCode==1)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_MESSAGEBOX_ENG); 
		if(m_Mode==2)
		{
			m_Bt_Ok.LoadBitmaps(IDB_BTSETUPMENU_OK_ENG,IDB_BTSETUPMENU_OK_0_ENG); 
			m_Bt_Ok.SizeToContent();	
		}
		m_Bt_Cancel.LoadBitmaps(IDB_SELPRODUCT_EXIT_ENG,IDB_SELPRODUCT_EXIT_0_ENG); 
	}
	else if(g_NationalCode==2)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_MESSAGEBOX_CHI); 
		if(m_Mode==2)
		{
			m_Bt_Ok.LoadBitmaps(IDB_BTSETUPMENU_OK_CHI,IDB_BTSETUPMENU_OK_0_CHI); 
			m_Bt_Ok.SizeToContent();	
		}
		m_Bt_Cancel.LoadBitmaps(IDB_SELPRODUCT_EXIT_CHI,IDB_SELPRODUCT_EXIT_0_CHI);
	}
	else if(g_NationalCode==3)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_MESSAGEBOX_SPN); 
		if(m_Mode==2)
		{
			m_Bt_Ok.LoadBitmaps(IDB_BTSETUPMENU_OK_SPN,IDB_BTSETUPMENU_OK_0_SPN); 
			m_Bt_Ok.SizeToContent();	
		}
		m_Bt_Cancel.LoadBitmaps(IDB_SELPRODUCT_EXIT_SPN,IDB_SELPRODUCT_EXIT_0_SPN);
	}

    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 

	m_Bt_Cancel.SizeToContent();	

}


BOOL CMessageBoxDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE;
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	return CDialogEx::PreTranslateMessage(pMsg);
}
