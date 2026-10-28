// AutoSetup_4.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "AutoSetup_4.h"
#include "afxdialogex.h"


// CAutoSetup_4 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoSetup_4, CDialogEx)

CAutoSetup_4::CAutoSetup_4(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoSetup_4::IDD, pParent)
{

}

CAutoSetup_4::~CAutoSetup_4()
{
}

void CAutoSetup_4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAutoSetup_4, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAutoSetup_4 메시지 처리기입니다.


BOOL CAutoSetup_4::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Brush.CreateStockObject(NULL_BRUSH);
	LoadBackground();
	SetTimer(1,1000,NULL);	
	GetDlgItem(IDC_PKV)->SetWindowText(_T("..."));
	GetDlgItem(IDC_PMA)->SetWindowText(_T("..."));	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAutoSetup_4::LoadBackground()
{
	BITMAP bmpInfo;
	m_bmpMainBG.LoadBitmap(IDB_BG_AUTOSETUPCHILD); 
    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 

	SetDlgItemText(IDC_STATIC_1, g_MessageArr[29]);
	SetDlgItemText(IDC_STATIC_2, g_MessageArr[30]);
	SetDlgItemText(IDC_STATIC_3, g_MessageArr[31]);

}

BOOL CAutoSetup_4::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;	

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAutoSetup_4::OnDestroy()
{
	KillTimer(1);

	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CAutoSetup_4::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


void CAutoSetup_4::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent==1)
	{
		SetDlgItemInt(IDC_PCOUNT, g_AutoXrayCount);
		if(g_AutoXrayCount>0)
			ShowResult();
	}
	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CAutoSetup_4::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CAutoSetup_4::ShowResult(void)
{
	if(NSE_XRAY_KIND==2 || (NSE_XRAY_KIND==0 && NSE_XRAY_KV==80))
	{
		if(g_AutoXrayValue<=10)
		{
			g_Xray_kv=80;
			g_Xray_ma=1000;
			g_Xray_kv2=30;
			g_Xray_ma2=200;
		}
		else if(g_AutoXrayValue>10 && g_AutoXrayValue<=20)
		{
			g_Xray_kv=80;
			g_Xray_ma=1000;
			g_Xray_kv2=20;
			g_Xray_ma2=200;
		}
		else if(g_AutoXrayValue>20 && g_AutoXrayValue<=30)
		{
			g_Xray_kv=80;
			g_Xray_ma=1000;
			g_Xray_kv2=10;
			g_Xray_ma2=200;
		}
		else if(g_AutoXrayValue>30 && g_AutoXrayValue<=40)
		{
			g_Xray_kv=70;
			g_Xray_ma=1000;
			g_Xray_kv2=10;
			g_Xray_ma2=200;
		}
		else if(g_AutoXrayValue>40 && g_AutoXrayValue<=200)
		{
			g_Xray_kv=(-25*g_AutoXrayValue)/100+90;
			if(NSE_XRAY_KIND==0)
			{
				if(g_Xray_kv<60)
					g_Xray_kv=60;
				g_Xray_ma=1000;
			}
			else
			{
				if(g_Xray_kv<50)
					g_Xray_kv=50;
				g_Xray_ma=800;
			}
			g_Xray_kv2=0;
			g_Xray_ma2=0;
		}
		else
		{
			
			g_Xray_kv=45;
			g_Xray_ma=800;
			if(NSE_XRAY_KIND==0)
			{
				g_Xray_kv=60;
				g_Xray_ma=1000;
			}
			g_Xray_kv2=0;
			g_Xray_ma2=0;
		}
	}
	else if(NSE_XRAY_KIND==3)
	{
		if(g_AutoXrayValue<=10)
		{
			g_Xray_kv=80;
			g_Xray_ma=2500;
			g_Xray_kv2=30;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>10 && g_AutoXrayValue<=20)
		{
			g_Xray_kv=80;
			g_Xray_ma=2500;
			g_Xray_kv2=20;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>20 && g_AutoXrayValue<=30)
		{
			g_Xray_kv=80;
			g_Xray_ma=2500;
			g_Xray_kv2=10;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>30 && g_AutoXrayValue<=40)
		{
			g_Xray_kv=70;
			g_Xray_ma=2500;
			g_Xray_kv2=10;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>40 && g_AutoXrayValue<=200)
		{
			g_Xray_kv=(-25*g_AutoXrayValue)/100+90;
			g_Xray_ma=2500;
			g_Xray_kv2=0;
			g_Xray_ma2=0;
		}
		else
		{
			g_Xray_kv=40;
			g_Xray_ma=3000;
			g_Xray_kv2=0;
			g_Xray_ma2=0;
		}
	}
	else
	{
		if(g_AutoXrayValue<=10)
		{
			g_Xray_kv=100;
			g_Xray_ma=3000;
			g_Xray_kv2=30;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>10 && g_AutoXrayValue<=20)
		{
			g_Xray_kv=100;
			g_Xray_ma=3000;
			g_Xray_kv2=20;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>20 && g_AutoXrayValue<=30)
		{
			g_Xray_kv=100;
			g_Xray_ma=3000;
			g_Xray_kv2=10;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>30 && g_AutoXrayValue<=40)
		{
			g_Xray_kv=90;
			g_Xray_ma=2500;
			g_Xray_kv2=10;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>40 && g_AutoXrayValue<=200)
		{
			g_Xray_kv=(-25*g_AutoXrayValue)/100+90;
			g_Xray_ma=2500;
			g_Xray_kv2=0;
			g_Xray_ma2=0;
		}
		else
		{
			g_Xray_kv=40;
			g_Xray_ma=3000;
			g_Xray_kv2=0;
			g_Xray_ma2=0;
		}
	}

	CString str;
	if(g_Xray_kv2==0)
		str.Format(_T("%d"), g_Xray_kv);
	else
		str.Format(_T("%d(%d)"), g_Xray_kv, g_Xray_kv2);
	GetDlgItem(IDC_PKV)->SetWindowText(str);
	if(g_Xray_ma2==0)
		str.Format(_T("%d"), g_Xray_ma);
	else
		str.Format(_T("%d(%d)"), g_Xray_ma, g_Xray_ma2);
	GetDlgItem(IDC_PMA)->SetWindowText(str);
}
