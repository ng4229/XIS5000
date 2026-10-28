// ConditionMaskSetup_0.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "ConditionMaskSetup_0.h"
#include "afxdialogex.h"

#include "NumPadBack.h"


//20160520
#include "MainFrm.h"
#include "ChildFrm.h"
#include "IntellisenseView.h"
// CConditionMaskSetup_0 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConditionMaskSetup_0, CDialogEx)

CConditionMaskSetup_0::CConditionMaskSetup_0(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConditionMaskSetup_0::IDD, pParent)
	, m_ShowMasking(0)
{
	m_SetupMaskBoxUse = 0;
	m_SetupMaskBoxCond1 = 0;
	m_SetupMaskBoxCond2 = 0;
	
}

CConditionMaskSetup_0::~CConditionMaskSetup_0()
{
	
}

void CConditionMaskSetup_0::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MASKBOX1_DN, m_Btcond1_Dn);
	DDX_Control(pDX, IDC_MASKBOX1_UP, m_Btcond1_Up);
	DDX_Control(pDX, IDC_MASKBOX2_UP, m_Btcond2_Up);
	DDX_Control(pDX, IDC_MASKBOX2_DN, m_Btcond2_Dn);
	DDX_Control(pDX, IDC_MASKBOX1_UP2, m_Btcond3_Up);
	DDX_Control(pDX, IDC_MASKBOX1_DN2, m_Btcond3_Dn);

	DDX_Control(pDX, IDC_MASKBOX1_DN3, m_Btcond4_Dn);
	DDX_Control(pDX, IDC_MASKBOX1_UP3, m_Btcond4_Up);
	DDX_Control(pDX, IDC_MASKBOX1_UP4, m_Btcond5_Up);
	DDX_Control(pDX, IDC_MASKBOX1_DN4, m_Btcond5_Dn);
	DDX_Control(pDX, IDC_MASKBOX1_UP5, m_Btcond6_Up);
	DDX_Control(pDX, IDC_MASKBOX1_DN5, m_Btcond6_Dn);


	DDX_Radio(pDX, IDC_RUSEMASKBOX_0, m_SetupMaskBoxUse);

	DDX_Text(pDX, IDC_SETUPMASKBOX1, m_SetupMaskBoxCond1);
	DDX_Text(pDX, IDC_SETUPMASKBOX2, m_SetupMaskBoxCond2);
	DDX_Text(pDX, IDC_SETUPMASKBOX3, m_SetupMaskBoxCond3);
	DDX_Text(pDX, IDC_SETUPMASKBOX4, m_SetupMaskBoxCond4);
	DDX_Text(pDX, IDC_SETUPMASKBOX5, m_SetupMaskBoxCond5);
	DDX_Text(pDX, IDC_SETUPMASKBOX6, m_SetupMaskBoxCond6);
	DDX_Radio(pDX, IDC_RUSEMASKBOX_2, m_ShowMasking);
	DDX_Control(pDX, IDC_SETUPMASKBOX4, m_SetMasking4);
	DDX_Control(pDX, IDC_SETUPMASKBOX5, m_SetMasking5);
	DDX_Control(pDX, IDC_SETUPMASKBOX6, m_SetMasking6);
}


BEGIN_MESSAGE_MAP(CConditionMaskSetup_0, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RUSEMASKBOX_0, &CConditionMaskSetup_0::OnClickedRusemaskbox0)
	ON_COMMAND(IDC_RUSEMASKBOX_1, &CConditionMaskSetup_0::OnRusemaskbox1)
	ON_BN_CLICKED(IDC_RUSEMASKBOX_2, &CConditionMaskSetup_0::OnBnClickedRusemaskbox2)
	ON_BN_CLICKED(IDC_RUSEMASKBOX_3, &CConditionMaskSetup_0::OnBnClickedRusemaskbox3)
END_MESSAGE_MAP()

// CConditionMaskSetup_0 메시지 처리기입니다.
BOOL CConditionMaskSetup_0::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Brush.CreateStockObject(NULL_BRUSH);
	LoadBackground();
	LoadValue();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CConditionMaskSetup_0::LoadBackground(void)
{
	BITMAP bmpInfo;
	
	if(g_NationalCode==0)
	{
		
		if(g_TalMask_Bright == 2)
		{
			m_Btcond4_Up.ShowWindow(true);
			m_Btcond4_Dn.ShowWindow(true);
			m_SetMasking4.ShowWindow(true);
			m_Btcond5_Up.ShowWindow(true);
			m_Btcond5_Dn.ShowWindow(true);
			m_SetMasking5.ShowWindow(true);
			m_Btcond6_Up.ShowWindow(true);
			m_Btcond6_Dn.ShowWindow(true);
			m_SetMasking6.ShowWindow(true);
			m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_0); 
		}
		else
		{
			m_Btcond4_Up.ShowWindow(false);
			m_Btcond4_Dn.ShowWindow(false);
			m_SetMasking4.ShowWindow(false);
			m_Btcond5_Up.ShowWindow(false);
			m_Btcond5_Dn.ShowWindow(false);
			m_SetMasking5.ShowWindow(false);
			m_Btcond6_Up.ShowWindow(false);
			m_Btcond6_Dn.ShowWindow(false);
			m_SetMasking6.ShowWindow(false);
			m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP); 
		}
		
	}
	else if(g_NationalCode==1)
	{
		if(g_TalMask_Bright == 2)
		{
			m_Btcond4_Up.ShowWindow(true);
			m_Btcond4_Dn.ShowWindow(true);
			m_SetMasking4.ShowWindow(true);
			m_Btcond5_Up.ShowWindow(true);
			m_Btcond5_Dn.ShowWindow(true);
			m_SetMasking5.ShowWindow(true);
			m_Btcond6_Up.ShowWindow(true);
			m_Btcond6_Dn.ShowWindow(true);
			m_SetMasking6.ShowWindow(true);
			m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_ENG_0); 
		}
		else
		{
			m_Btcond4_Up.ShowWindow(false);
			m_Btcond4_Dn.ShowWindow(false);
			m_SetMasking4.ShowWindow(false);
			m_Btcond5_Up.ShowWindow(false);
			m_Btcond5_Dn.ShowWindow(false);
			m_SetMasking5.ShowWindow(false);
			m_Btcond6_Up.ShowWindow(false);
			m_Btcond6_Dn.ShowWindow(false);
			m_SetMasking6.ShowWindow(false);
			m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_ENG); 
		}

		
	}
	else if(g_NationalCode==2)
	{
		if(g_TalMask_Bright == 2)
		{
			m_Btcond4_Up.ShowWindow(true);
			m_Btcond4_Dn.ShowWindow(true);
			m_SetMasking4.ShowWindow(true);
			m_Btcond5_Up.ShowWindow(true);
			m_Btcond5_Dn.ShowWindow(true);
			m_SetMasking5.ShowWindow(true);
			m_Btcond6_Up.ShowWindow(true);
			m_Btcond6_Dn.ShowWindow(true);
			m_SetMasking6.ShowWindow(true);
			m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_CHI_0); 
		}
		else
		{
			m_Btcond4_Up.ShowWindow(false);
			m_Btcond4_Dn.ShowWindow(false);
			m_SetMasking4.ShowWindow(false);
			m_Btcond5_Up.ShowWindow(false);
			m_Btcond5_Dn.ShowWindow(false);
			m_SetMasking5.ShowWindow(false);
			m_Btcond6_Up.ShowWindow(false);
			m_Btcond6_Dn.ShowWindow(false);
			m_SetMasking6.ShowWindow(false);
			m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_CHI); 
		}
		
	}
	else if(g_NationalCode==3)
	{
		if(g_TalMask_Bright == 2)
		{
			m_Btcond4_Up.ShowWindow(true);
			m_Btcond4_Dn.ShowWindow(true);
			m_SetMasking4.ShowWindow(true);
			m_Btcond5_Up.ShowWindow(true);
			m_Btcond5_Dn.ShowWindow(true);
			m_SetMasking5.ShowWindow(true);
			m_Btcond6_Up.ShowWindow(true);
			m_Btcond6_Dn.ShowWindow(true);
			m_SetMasking6.ShowWindow(true);
			m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_SPN_0); 
		}
		else
		{
			m_Btcond4_Up.ShowWindow(false);
			m_Btcond4_Dn.ShowWindow(false);
			m_SetMasking4.ShowWindow(false);
			m_Btcond5_Up.ShowWindow(false);
			m_Btcond5_Dn.ShowWindow(false);
			m_SetMasking5.ShowWindow(false);
			m_Btcond6_Up.ShowWindow(false);
			m_Btcond6_Dn.ShowWindow(false);
			m_SetMasking6.ShowWindow(false);
			m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_SPN); 
		}
		
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
	m_Btcond3_Up.LoadBitmaps(IDB_CONDPLUSBT,IDB_CONDPLUSBT_1); 
	m_Btcond3_Up.SizeToContent();
	m_Btcond3_Dn.LoadBitmaps(IDB_CONDMINUSBT,IDB_CONDMINUSBT_1); 
	m_Btcond3_Dn.SizeToContent();
	
	m_Btcond4_Up.LoadBitmaps(IDB_CONDPLUSBT,IDB_CONDPLUSBT_1); 
	m_Btcond4_Up.SizeToContent();
	m_Btcond4_Dn.LoadBitmaps(IDB_CONDMINUSBT,IDB_CONDMINUSBT_1); 
	m_Btcond4_Dn.SizeToContent();
	m_Btcond5_Up.LoadBitmaps(IDB_CONDPLUSBT,IDB_CONDPLUSBT_1); 
	m_Btcond5_Up.SizeToContent();
	m_Btcond5_Dn.LoadBitmaps(IDB_CONDMINUSBT,IDB_CONDMINUSBT_1); 
	m_Btcond5_Dn.SizeToContent();
	m_Btcond6_Up.LoadBitmaps(IDB_CONDPLUSBT,IDB_CONDPLUSBT_1); 
	m_Btcond6_Up.SizeToContent();
	m_Btcond6_Dn.LoadBitmaps(IDB_CONDMINUSBT,IDB_CONDMINUSBT_1); 
	m_Btcond6_Dn.SizeToContent();



}

void CConditionMaskSetup_0::LoadValue()
{
	m_SetupMaskBoxUse = g_MaskBoxUse;
	m_ShowMasking = g_ShowMaskUse;
	m_SetupMaskBoxCond1 = g_MaskBoxCondition1;
	m_SetupMaskBoxCond2 = g_MaskBoxCondition2;
	m_SetupMaskBoxCond3 = g_MaskBoxCondition3;

	m_SetupMaskBoxCond4 = g_MaskBoxCondition4;
	m_SetupMaskBoxCond5 = g_MaskBoxCondition5;
	m_SetupMaskBoxCond6 = g_MaskBoxCondition6;


	
	if(m_SetupMaskBoxUse==0)
	{
		((CButton*)GetDlgItem(IDC_RUSEMASKBOX_0))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RUSEMASKBOX_1))->SetCheck(false);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RUSEMASKBOX_0))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RUSEMASKBOX_1))->SetCheck(true);
	}

	if(m_ShowMasking ==0)
	{
		((CButton*)GetDlgItem(IDC_RUSEMASKBOX_2))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RUSEMASKBOX_3))->SetCheck(false);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RUSEMASKBOX_2))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RUSEMASKBOX_3))->SetCheck(true);
	}
	SetDlgItemInt(IDC_SETUPMASKBOX1, m_SetupMaskBoxCond1, true);
	SetDlgItemInt(IDC_SETUPMASKBOX2, m_SetupMaskBoxCond2, true);
	SetDlgItemInt(IDC_SETUPMASKBOX3, m_SetupMaskBoxCond3, true);

	SetDlgItemInt(IDC_SETUPMASKBOX4, m_SetupMaskBoxCond4, true);
	SetDlgItemInt(IDC_SETUPMASKBOX5, m_SetupMaskBoxCond5, true);
	SetDlgItemInt(IDC_SETUPMASKBOX6, m_SetupMaskBoxCond6, true);
}

BOOL CConditionMaskSetup_0::PreTranslateMessage(MSG* pMsg)
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
			SetTimer(1, 120, NULL);
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
			SetTimer(2, 120, NULL);
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
			SetTimer(3, 120, NULL);
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
			SetTimer(4, 120, NULL);
			SendMessage(WM_TIMER,4);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(4);
			OnChangeSetupcondition();

		}
	}//	

	if(pMsg->hwnd == m_Btcond3_Up.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(5, 120, NULL);
			SendMessage(WM_TIMER,5);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(5);
			OnChangeSetupcondition();
		}
	}
	else if(pMsg->hwnd == m_Btcond3_Dn.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(6, 120, NULL);
			SendMessage(WM_TIMER,6);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(6);
			OnChangeSetupcondition();

		}
	}//	
	if(pMsg->hwnd == m_Btcond4_Up.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(7, 120, NULL);
			SendMessage(WM_TIMER,7);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(7);
			OnChangeSetupcondition();
		}
	}
	else if(pMsg->hwnd == m_Btcond4_Dn.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(8, 120, NULL);
			SendMessage(WM_TIMER,8);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(8);
			OnChangeSetupcondition();

		}
	}//	
	if(pMsg->hwnd == m_Btcond5_Up.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(9, 120, NULL);
			SendMessage(WM_TIMER,9);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(9);
			OnChangeSetupcondition();
		}
	}
	else if(pMsg->hwnd == m_Btcond5_Dn.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(10, 120, NULL);
			SendMessage(WM_TIMER,10);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(10);
			OnChangeSetupcondition();

		}
	}//	
	if(pMsg->hwnd == m_Btcond6_Up.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(11, 120, NULL);
			SendMessage(WM_TIMER,11);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(11);
			OnChangeSetupcondition();
		}
	}
	else if(pMsg->hwnd == m_Btcond6_Dn.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(12, 120, NULL);
			SendMessage(WM_TIMER,12);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(12);
			OnChangeSetupcondition();

		}
	}//	
	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPMASKBOX1)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=g_Height;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupMaskBoxCond1=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPMASKBOX1)->SetWindowText(dlg.m_Value);
				OnChangeSetupcondition();
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPMASKBOX2)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=g_Height;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupMaskBoxCond2=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPMASKBOX2)->SetWindowText(dlg.m_Value);
				OnChangeSetupcondition();
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPMASKBOX3)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=g_Height;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupMaskBoxCond3=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPMASKBOX3)->SetWindowText(dlg.m_Value);
				OnChangeSetupcondition();
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CConditionMaskSetup_0::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CConditionMaskSetup_0::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


void CConditionMaskSetup_0::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString s;
	switch(nIDEvent)
	{
		case 1:
			if(m_SetupMaskBoxCond1<g_Height)
			{
				m_SetupMaskBoxCond1++;
				g_EventPaint.PulseEvent();
				Draw_Data();
				UpdateData(false);
			}	
			break;
		case 2:
			if(m_SetupMaskBoxCond1>0)
			{
				m_SetupMaskBoxCond1--;
				g_EventPaint.PulseEvent();
				Draw_Data();
				UpdateData(false);
			}	
			break;
		case 3:
			if(m_SetupMaskBoxCond2<300)
			{
				m_SetupMaskBoxCond2++;
				g_EventPaint.PulseEvent();
				Draw_Data();
				UpdateData(false);
			}
			break;
		case 4:
			if(m_SetupMaskBoxCond2>0)
			{
				m_SetupMaskBoxCond2--;
				g_EventPaint.PulseEvent();
				Draw_Data();
				UpdateData(false);
			}	
			break;
		case 5:
			if(m_SetupMaskBoxCond3<g_Height)
			{
				m_SetupMaskBoxCond3++;
			//	g_EventPaint.PulseEvent();
			//	Draw_Data();
				UpdateData(false);
			}
			break;
		case 6:
			if(m_SetupMaskBoxCond3>0)
			{
				m_SetupMaskBoxCond3--;
			//	g_EventPaint.PulseEvent();
			//	Draw_Data();
				UpdateData(false);
			}	
			break;
		case 7:
			if(m_SetupMaskBoxCond4<g_Height)
			{
				m_SetupMaskBoxCond4++;
			//	g_EventPaint.PulseEvent();
			//	Draw_Data();
				UpdateData(false);
			}
			break;
		case 8:
			if(m_SetupMaskBoxCond4>0)
			{
				m_SetupMaskBoxCond4--;
			//	g_EventPaint.PulseEvent();
			//	Draw_Data();
				UpdateData(false);
			}	
			break;
		case 9:
			if(m_SetupMaskBoxCond5<g_Height)
			{
				m_SetupMaskBoxCond5++;
			//	g_EventPaint.PulseEvent();
			//	Draw_Data();
				UpdateData(false);
			}
			break;
		case 10:
			if(m_SetupMaskBoxCond5>0)
			{
				m_SetupMaskBoxCond5--;
			//	g_EventPaint.PulseEvent();
			//	Draw_Data();
				UpdateData(false);
			}	
			break;
		case 11:
			if(m_SetupMaskBoxCond6<g_Height)
			{
				m_SetupMaskBoxCond6++;
			//	g_EventPaint.PulseEvent();
			//	Draw_Data();
				UpdateData(false);
			}
			break;
		case 12:
			if(m_SetupMaskBoxCond6>0)
			{
				m_SetupMaskBoxCond6--;
			//	g_EventPaint.PulseEvent();
			//	Draw_Data();
				UpdateData(false);
			}	
			break;
	}		

	CDialogEx::OnTimer(nIDEvent);
}


void CConditionMaskSetup_0::OnClickedRusemaskbox0()
{
	m_SetupMaskBoxUse=0;
}


void CConditionMaskSetup_0::OnRusemaskbox1()
{
	m_SetupMaskBoxUse=1;
}


//20160520
void CConditionMaskSetup_0::OnChangeSetupcondition(void)
{
	if(g_OffInspection && !m_SetupMaskBoxUse && NSE_DT_ISBULK==0)
	{
		int Temp_cond1 = g_MaskBoxCondition1;
		int Temp_cond2 = g_MaskBoxCondition2;
		int Temp_cond3 = g_MaskBoxCondition3;
		int Temp_cond4 = g_MaskBoxCondition4;
		int Temp_cond5 = g_MaskBoxCondition5;
		int Temp_cond6 = g_MaskBoxCondition6;

		g_MaskBoxCondition1 = m_SetupMaskBoxCond1;
		g_MaskBoxCondition2 = m_SetupMaskBoxCond2;
		g_MaskBoxCondition3 = m_SetupMaskBoxCond3;
		
		g_MaskBoxCondition4 = m_SetupMaskBoxCond4;
		g_MaskBoxCondition5 = m_SetupMaskBoxCond5;
		g_MaskBoxCondition6 = m_SetupMaskBoxCond6;


		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
		pView->OnRecognition_Test(g_3D_Count);
		g_MaskBoxCondition1 = Temp_cond1;	
		g_MaskBoxCondition2 = Temp_cond2;	
		g_MaskBoxCondition3 = Temp_cond3;
		g_MaskBoxCondition4 = Temp_cond4;	
		g_MaskBoxCondition5 = Temp_cond5;	
		g_MaskBoxCondition6 = Temp_cond6;
	}
}


void CConditionMaskSetup_0::Draw_Data(void)
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
	CDC *pDC = pView->m_GrabImage.GetDC();			
	CRect rect;
	pView->m_GrabImage.GetWindowRect(&rect);
	pDC->SetStretchBltMode(COLORONCOLOR);


	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	CPen pen2;
	pen2.CreatePen(PS_SOLID, 2, RGB(255, 100, 0));

	pDC->SelectObject(&pen2);

	//	UpdateData(false);
	int Dent_Up=GetDlgItemInt(IDC_SETUPMASKBOX1); //Up
	int real_val2 = (h*Dent_Up)/g_Height;  //Up

	pDC->MoveTo(1, real_val2);
	pDC->LineTo((rect.Width()-5), real_val2);



	int Dent_Down=GetDlgItemInt(IDC_SETUPMASKBOX2); //Down
//	Dent_Down = Dent_Down + (g_Margin2 *2);
	Dent_Down = Dent_Down + (g_Margin2);

	int real_val = (h*Dent_Down)/g_Height; //DOWN
	pDC->MoveTo(1, h-real_val);
	pDC->LineTo((rect.Width()-5), h-real_val);





	ReleaseDC(pDC);
}


void CConditionMaskSetup_0::OnBnClickedRusemaskbox2()
{
	m_ShowMasking = 0;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CConditionMaskSetup_0::OnBnClickedRusemaskbox3()
{
	m_ShowMasking = 1;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
