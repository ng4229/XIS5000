// ConditionOmitSetup_2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "ConditionOmitSetup_2.h"
#include "afxdialogex.h"

#include "NumPadBack.h"
// CConditionOmitSetup_2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConditionOmitSetup_2, CDialogEx)

CConditionOmitSetup_2::CConditionOmitSetup_2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConditionOmitSetup_2::IDD, pParent)
{
	m_SetupCountUse = 0;
	m_SetupNum = 0;
	m_SetupNoise = 0;

}

CConditionOmitSetup_2::~CConditionOmitSetup_2()
{
}

void CConditionOmitSetup_2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COUNTCONDITION1_DN, m_Btcond1_Dn);
	DDX_Control(pDX, IDC_COUNTCONDITION1_UP, m_Btcond1_Up);
	DDX_Control(pDX, IDC_COUNTCONDITION2_DN, m_Btcond2_Dn);
	DDX_Control(pDX, IDC_COUNTCONDITION2_UP, m_Btcond2_Up);
	DDX_Text(pDX, IDC_SETUPCOUNTCONDITION1, m_SetupNum);
	DDX_Text(pDX, IDC_SETUPCOUNTCONDITION2, m_SetupNoise);
	DDX_Radio(pDX, IDC_RUSECOUNT_0, m_SetupCountUse);
}


BEGIN_MESSAGE_MAP(CConditionOmitSetup_2, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RUSECOUNT_0, &CConditionOmitSetup_2::OnClickedRusecount0)
	ON_COMMAND(IDC_RUSECOUNT_1, &CConditionOmitSetup_2::OnRusecount1)
END_MESSAGE_MAP()


// CConditionOmitSetup_2 메시지 처리기입니다.


BOOL CConditionOmitSetup_2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Brush.CreateStockObject(NULL_BRUSH);
	LoadBackground();
	LoadValue();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CConditionOmitSetup_2::LoadBackground()
{
	BITMAP bmpInfo;

	if(g_NationalCode==0)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONOMITSETUP_2); 
	}
	else if(g_NationalCode==1)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONOMITSETUP_2_ENG); 
	}
	else if(g_NationalCode==2)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONOMITSETUP_2_CHI); 
	}
	else if(g_NationalCode==3)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONOMITSETUP_2_SPN); 
	}

    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 

	m_Btcond1_Up.LoadBitmaps(IDB_CONDPLUSBT,IDB_CONDPLUSBT_1); 
	m_Btcond1_Up.SizeToContent();
	m_Btcond1_Dn.LoadBitmaps(IDB_CONDMINUSBT,IDB_CONDMINUSBT_1); 
	m_Btcond1_Dn.SizeToContent();
	m_Btcond2_Up.LoadBitmaps(IDB_CONDPLUSBT,IDB_CONDPLUSBT_1); 
	m_Btcond2_Up.SizeToContent();
	m_Btcond2_Dn.LoadBitmaps(IDB_CONDMINUSBT,IDB_CONDMINUSBT_1); 
	m_Btcond2_Dn.SizeToContent();
}

void CConditionOmitSetup_2::LoadValue()
{
	m_SetupCountUse=g_CountCondUse;
	m_SetupNum=g_Countnum;
	m_SetupNoise=g_Countnoise;
	if(m_SetupCountUse==0)
	{
		((CButton*)GetDlgItem(IDC_RUSECOUNT_0))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RUSECOUNT_1))->SetCheck(false);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RUSECOUNT_0))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RUSECOUNT_1))->SetCheck(true);
	}
	SetDlgItemInt(IDC_SETUPCOUNTCONDITION1, m_SetupNum, true);
	SetDlgItemInt(IDC_SETUPCOUNTCONDITION2, m_SetupNoise, true);
}

BOOL CConditionOmitSetup_2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	if(pMsg->hwnd == m_Btcond1_Up.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(1, 150, NULL);
			SendMessage(WM_TIMER,1);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(1);
		}
	}
	else if(pMsg->hwnd == m_Btcond1_Dn.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(2, 150, NULL);
			SendMessage(WM_TIMER,2);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(2);
		}
	}//			
	if(pMsg->hwnd == m_Btcond2_Up.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(3, 150, NULL);
			SendMessage(WM_TIMER,3);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(3);
		}
	}
	else if(pMsg->hwnd == m_Btcond2_Dn.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(4, 150, NULL);
			SendMessage(WM_TIMER,4);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(4);
		}
	}//	
	
	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPCOUNTCONDITION1)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=500;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupNum=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPCOUNTCONDITION1)->SetWindowText(dlg.m_Value);
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPCOUNTCONDITION2)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=200000;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupNoise=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPCOUNTCONDITION2)->SetWindowText(dlg.m_Value);
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CConditionOmitSetup_2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}

void CConditionOmitSetup_2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
		case 1:
			if(m_SetupNum<500)
			{
				m_SetupNum++;
				UpdateData(false);
			}	
			break;
		case 2:
			if(m_SetupNum>0)
			{
				m_SetupNum--;
				UpdateData(false);
			}	
			break;
		case 3:
			if(m_SetupNoise<200000)
			{
				m_SetupNoise+=10;
				UpdateData(false);
			}
			break;
		case 4:
			if(m_SetupNoise>=10)
			{
				m_SetupNoise-=10;
				UpdateData(false);
			}	
			else
			{
				m_SetupNoise=0;
				UpdateData(false);
			}
			break;
	}		

	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CConditionOmitSetup_2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CConditionOmitSetup_2::OnClickedRusecount0()
{
	m_SetupCountUse=0;
}


void CConditionOmitSetup_2::OnRusecount1()
{
	m_SetupCountUse=1;
}
