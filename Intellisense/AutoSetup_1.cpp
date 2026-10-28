// AutoSetup_1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "AutoSetup_1.h"
#include "afxdialogex.h"

#include "NumPadBack.h"

// CAutoSetup_1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoSetup_1, CDialogEx)

CAutoSetup_1::CAutoSetup_1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoSetup_1::IDD, pParent)
{

	m_Pkv = NSE_BASIC_KV;
	m_Pua = NSE_BASIC_MA;
	m_Pscanspeed = NSE_BASIC_SPEED;
}

CAutoSetup_1::~CAutoSetup_1()
{
}

void CAutoSetup_1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PKV, m_Pkv);
	DDX_Text(pDX, IDC_PUA, m_Pua);
	DDX_Text(pDX, IDC_PSCANSPEED, m_Pscanspeed);
	DDX_Control(pDX, IDC_BTKV_UP, m_Btkv_Up);
	DDX_Control(pDX, IDC_BTKV_DN, m_Btkv_Dn);
	DDX_Control(pDX, IDC_BTSCANSPEED_UP, m_Btscanspeed_Up);
	DDX_Control(pDX, IDC_BTSCANSPEED_DN, m_Btscanspeed_Dn);
	DDX_Control(pDX, IDC_BTUA_DN, m_Btua_Dn);
	DDX_Control(pDX, IDC_BTUA_UP, m_Btua_Up);
}


BEGIN_MESSAGE_MAP(CAutoSetup_1, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAutoSetup_1 메시지 처리기입니다.


BOOL CAutoSetup_1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Brush.CreateStockObject(NULL_BRUSH);
	LoadBackground();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAutoSetup_1::LoadBackground()
{
	BITMAP bmpInfo;
	m_bmpMainBG.LoadBitmap(IDB_BG_AUTOSETUPCHILD); 
    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 

	m_Btkv_Up.LoadBitmaps(IDB_CONDPLUSBT,IDB_CONDPLUSBT_1); 
	m_Btkv_Up.SizeToContent();
	m_Btkv_Dn.LoadBitmaps(IDB_CONDMINUSBT,IDB_CONDMINUSBT_1); 
	m_Btkv_Dn.SizeToContent();
	m_Btua_Up.LoadBitmaps(IDB_CONDPLUSBT,IDB_CONDPLUSBT_1); 
	m_Btua_Up.SizeToContent();
	m_Btua_Dn.LoadBitmaps(IDB_CONDMINUSBT,IDB_CONDMINUSBT_1); 
	m_Btua_Dn.SizeToContent();
	m_Btscanspeed_Up.LoadBitmaps(IDB_CONDPLUSBT,IDB_CONDPLUSBT_1); 
	m_Btscanspeed_Up.SizeToContent();
	m_Btscanspeed_Dn.LoadBitmaps(IDB_CONDMINUSBT,IDB_CONDMINUSBT_1); 
	m_Btscanspeed_Dn.SizeToContent();

	SetDlgItemText(IDC_STATIC_1, g_MessageArr[12]);
	SetDlgItemText(IDC_STATIC_2, g_MessageArr[13]);
	SetDlgItemText(IDC_STATIC_3, g_MessageArr[14]);
}


BOOL CAutoSetup_1::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	if(pMsg->hwnd == m_Btkv_Up.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(1, 80, NULL);
			SendMessage(WM_TIMER,1);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(1);
		}
	}
	else if(pMsg->hwnd == m_Btkv_Dn.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(2, 80, NULL);
			SendMessage(WM_TIMER,2);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(2);
		}
	}//
	else if(pMsg->hwnd == m_Btua_Up.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(3, 80, NULL);
			SendMessage(WM_TIMER,3);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(3);
		}
	}
	else if(pMsg->hwnd == m_Btua_Dn.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(4, 80, NULL);
			SendMessage(WM_TIMER,4);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(4);
		}
	}//
	if(pMsg->hwnd == m_Btscanspeed_Up.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(5, 80, NULL);
			SendMessage(WM_TIMER,5);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(5);
		}
	}
	else if(pMsg->hwnd == m_Btscanspeed_Dn.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(6, 80, NULL);
			SendMessage(WM_TIMER,6);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(6);
		}
	}
	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_PKV)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=NSE_XRAY_KV;
			dlg.m_Min=30;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_Pkv=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_PKV)->SetWindowText(dlg.m_Value);
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_PUA)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=NSE_XRAY_MA;

			if(NSE_XRAY_KIND==2)
				dlg.m_Min=500;
			else
				dlg.m_Min=1000;

			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_Pua=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_PUA)->SetWindowText(dlg.m_Value);
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_PSCANSPEED)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=166;
			dlg.m_Min=10;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_Pscanspeed=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_PSCANSPEED)->SetWindowText(dlg.m_Value);
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CAutoSetup_1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CAutoSetup_1::OnPaint()
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


void CAutoSetup_1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
		case 1:
			if(m_Pkv<NSE_XRAY_KV)
			{
				m_Pkv++;
				UpdateData(false);
			}	
			break;
		case 2:
			if(m_Pkv>30)
			{
				m_Pkv--;
				UpdateData(false);
			}	
			break;
		case 3:
			if(m_Pua<NSE_XRAY_MA)
			{
				m_Pua += 100;
				UpdateData(false);
			}	
			break;
		case 4:
			if(NSE_XRAY_KIND==2)
			{
				if(m_Pua>500)
				{
					m_Pua -= 100;;
					UpdateData(false);
				}	
			}
			else
			{
				if(m_Pua>1000)
				{
					m_Pua -= 100;;
					UpdateData(false);
				}	
			}
				break;
		case 5:
			if(m_Pscanspeed<166)
			{
				m_Pscanspeed++;
				UpdateData(false);
			}	
			break;
		case 6:
			if(m_Pscanspeed>10)
			{
				m_Pscanspeed--;
				UpdateData(false);
			}	
			break;
	}	
	CDialogEx::OnTimer(nIDEvent);
}
