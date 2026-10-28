// ConditionMaskSetup_1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "ConditionMaskSetup_1.h"
#include "afxdialogex.h"

#include "NumPadBack.h"

//20160520
#include "MainFrm.h"
#include "ChildFrm.h"
#include "IntellisenseView.h"
// CConditionMaskSetup_1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConditionMaskSetup_1, CDialogEx)

CConditionMaskSetup_1::CConditionMaskSetup_1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConditionMaskSetup_1::IDD, pParent)
{
	m_SetupMaskClipUse = 0;
	m_SetupMaskClipDirection = 0;
	m_SetupMaskClipCond1 = 0;
	m_SetupMaskClipCond2 = 0;
}

CConditionMaskSetup_1::~CConditionMaskSetup_1()
{
}

void CConditionMaskSetup_1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MASKCLIP1_DN, m_Btcond1_Dn);
	DDX_Control(pDX, IDC_MASKCLIP1_UP, m_Btcond1_Up);
	DDX_Control(pDX, IDC_MASKCLIP2_DN, m_Btcond2_Dn);
	DDX_Control(pDX, IDC_MASKCLIP2_UP, m_Btcond2_Up);
	DDX_Radio(pDX, IDC_RUSEMASKCLIP_0, m_SetupMaskClipUse);
	DDX_Radio(pDX, IDC_SETUPMASKCLIP_DIRECTiON0, m_SetupMaskClipDirection);
	DDX_Text(pDX, IDC_SETUPMASKCLIP1, m_SetupMaskClipCond1);
	DDX_Text(pDX, IDC_SETUPMASKCLIP2, m_SetupMaskClipCond2);
}


BEGIN_MESSAGE_MAP(CConditionMaskSetup_1, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RUSEMASKCLIP_0, &CConditionMaskSetup_1::OnClickedRusemaskclip0)
	ON_COMMAND(IDC_RUSEMASKCLIP_1, &CConditionMaskSetup_1::OnRusemaskclip1)
	ON_BN_CLICKED(IDC_SETUPMASKCLIP_DIRECTiON0, &CConditionMaskSetup_1::OnClickedSetupmaskclipDirection0)
	ON_COMMAND(IDC_SETUPMASKCLIP_DIRECTiON1, &CConditionMaskSetup_1::OnSetupmaskclipDirection1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CConditionMaskSetup_1 메시지 처리기입니다.


BOOL CConditionMaskSetup_1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Brush.CreateStockObject(NULL_BRUSH);
	LoadBackground();
	LoadValue();

	SetTimer(99, 200, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CConditionMaskSetup_1::LoadBackground()
{
	BITMAP bmpInfo;

	if(g_NationalCode==0)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_1); 
	}
	else if(g_NationalCode==1)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_1_ENG); 
	}
	else if(g_NationalCode==2)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_1_CHI); 
	}
	else if(g_NationalCode==3)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_1_SPN); 
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


void CConditionMaskSetup_1::LoadValue()
{
	m_SetupMaskClipUse = g_MaskClipUse; //사용여부
//	m_SetupMaskClipDirection = g_MaskClip_Direction;
	m_SetupMaskClipCond1=g_MaskClip_Length;//가변설정값
//m_SetupMaskClipCond2=g_MaskClip_Deep;
	if(m_SetupMaskClipUse==0)
	{
		((CButton*)GetDlgItem(IDC_RUSEMASKCLIP_0))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RUSEMASKCLIP_1))->SetCheck(false);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RUSEMASKCLIP_0))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RUSEMASKCLIP_1))->SetCheck(true);
	}
/*
	if(m_SetupMaskClipDirection==0)
	{
		((CButton*)GetDlgItem(IDC_SETUPMASKCLIP_DIRECTiON0))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_SETUPMASKCLIP_DIRECTiON1))->SetCheck(false);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_SETUPMASKCLIP_DIRECTiON0))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_SETUPMASKCLIP_DIRECTiON1))->SetCheck(true);
	}

	*/
	SetDlgItemInt(IDC_SETUPMASKCLIP1, m_SetupMaskClipCond1, true);
	SetDlgItemInt(IDC_SETUPMASKCLIP2, m_SetupMaskClipCond2, true);
}

BOOL CConditionMaskSetup_1::PreTranslateMessage(MSG* pMsg)
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
			OnChangeSetupcondition();
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
			OnChangeSetupcondition();
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
			OnChangeSetupcondition();
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
			OnChangeSetupcondition();
		}
	}//		


	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPMASKCLIP1)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=1536;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupMaskClipCond1=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPMASKCLIP1)->SetWindowText(dlg.m_Value);
				OnChangeSetupcondition();
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPMASKCLIP2)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=100000;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupMaskClipCond2=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPMASKCLIP2)->SetWindowText(dlg.m_Value);
				OnChangeSetupcondition();
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CConditionMaskSetup_1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


void CConditionMaskSetup_1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
		case 1:
			if(m_SetupMaskClipCond1<1536)
			{
				m_SetupMaskClipCond1++;
				SetDlgItemInt(IDC_SETUPMASKCLIP1, m_SetupMaskClipCond1);
			}
			break;
		case 2:
			if(m_SetupMaskClipCond1>0)
			{
				m_SetupMaskClipCond1--;
				SetDlgItemInt(IDC_SETUPMASKCLIP1, m_SetupMaskClipCond1);
			}	
			break;
		case 3:
			if(m_SetupMaskClipCond2>=0)
			{
				m_SetupMaskClipCond2++;
				SetDlgItemInt(IDC_SETUPMASKCLIP2, m_SetupMaskClipCond2);
			}
			break;
		case 4:
			if(m_SetupMaskClipCond2>0)
			{
				m_SetupMaskClipCond2--;
				SetDlgItemInt(IDC_SETUPMASKCLIP2, m_SetupMaskClipCond2);
			}	
			break;
	}		

	if(nIDEvent == 99)
	{
		CString Size;
		Size.Format(_T("%d"),g_SizeDisp);
		GetDlgItem(IDC_READ1)->SetWindowText(Size);
	}



	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CConditionMaskSetup_1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CConditionMaskSetup_1::OnClickedRusemaskclip0()
{
	m_SetupMaskClipUse=0;
}


void CConditionMaskSetup_1::OnRusemaskclip1()
{
	m_SetupMaskClipUse=1;
}


void CConditionMaskSetup_1::OnClickedSetupmaskclipDirection0()
{
	m_SetupMaskClipDirection=0;
}


void CConditionMaskSetup_1::OnSetupmaskclipDirection1()
{
	m_SetupMaskClipDirection=1;
}


void CConditionMaskSetup_1::OnChangeSetupcondition(void)
{
	if(g_OffInspection && !m_SetupMaskClipUse && NSE_DT_ISBULK==0)
	{
		int Temp_cond0 = g_MaskClip_Direction;
		int Temp_cond1 = g_MaskClip_Length;
		int Temp_cond2 = g_MaskClip_Deep;
		g_MaskClip_Direction = m_SetupMaskClipDirection;
		g_MaskClip_Length = m_SetupMaskClipCond1;
		g_MaskClip_Deep = m_SetupMaskClipCond2;
		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
		pView->OnRecognition_Test(g_3D_Count);
		g_MaskClip_Direction = Temp_cond0;	
		g_MaskClip_Length = Temp_cond1;	
		g_MaskClip_Deep = Temp_cond2;	
	}
}

void CConditionMaskSetup_1::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(99);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
