// ConditionMaskSetup_2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "ConditionMaskSetup_2.h"
#include "afxdialogex.h"

#include "NumPadBack.h"

//20160520
#include "MainFrm.h"
#include "ChildFrm.h"
#include "IntellisenseView.h"
// CConditionMaskSetup_2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConditionMaskSetup_2, CDialogEx)

CConditionMaskSetup_2::CConditionMaskSetup_2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConditionMaskSetup_2::IDD, pParent)
{

	m_SetupMaskTalUse = 0;
	m_SetupMaskTalCond1 = 0;
	m_SetupMaskTalCond2 = 0;
}

CConditionMaskSetup_2::~CConditionMaskSetup_2()
{
}

void CConditionMaskSetup_2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MASKTAL1_DN, m_Btcond1_Dn);
	DDX_Control(pDX, IDC_MASKTAL1_UP, m_Btcond1_Up);
	DDX_Control(pDX, IDC_MASKTAL2_DN, m_Btcond2_Dn);
	DDX_Control(pDX, IDC_MASKTAL2_UP, m_Btcond2_Up);
	DDX_Radio(pDX, IDC_RUSEMASKTAL_0, m_SetupMaskTalUse);
	DDX_Text(pDX, IDC_SETUPMASKTAL1, m_SetupMaskTalCond1);
	DDX_Text(pDX, IDC_SETUPMASKTAL2, m_SetupMaskTalCond2);
}


BEGIN_MESSAGE_MAP(CConditionMaskSetup_2, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RUSEMASKTAL_0, &CConditionMaskSetup_2::OnClickedRusemasktal0)
	ON_COMMAND(IDC_RUSEMASKTAL_1, &CConditionMaskSetup_2::OnRusemasktal1)
END_MESSAGE_MAP()


// CConditionMaskSetup_2 메시지 처리기입니다.


BOOL CConditionMaskSetup_2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Brush.CreateStockObject(NULL_BRUSH);
	LoadBackground();
	LoadValue();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CConditionMaskSetup_2::LoadBackground(void)
{
	BITMAP bmpInfo;

	if(g_NationalCode==0)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_2); 
	}
	else if(g_NationalCode==1)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_2_ENG); 
	}
	else if(g_NationalCode==2)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_2_CHI); 
	}
	else if(g_NationalCode==3)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONMASKSETUP_2_SPN); 
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

void CConditionMaskSetup_2::LoadValue()
{
	m_SetupMaskTalUse = g_MaskTalUse;
	m_SetupMaskTalCond1 = g_TalMask_Bright;
	m_SetupMaskTalCond2 = g_TalMask_Bright2;
	if(m_SetupMaskTalUse==0)
	{
		((CButton*)GetDlgItem(IDC_RUSEMASKTAL_0))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RUSEMASKTAL_1))->SetCheck(false);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RUSEMASKTAL_0))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RUSEMASKTAL_1))->SetCheck(true);
	}
	SetDlgItemInt(IDC_SETUPMASKTAL1, m_SetupMaskTalCond1, true);
	SetDlgItemInt(IDC_SETUPMASKTAL2, m_SetupMaskTalCond2, true);
}

BOOL CConditionMaskSetup_2::PreTranslateMessage(MSG* pMsg)
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

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPMASKTAL1)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=5;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupMaskTalCond1=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPMASKTAL1)->SetWindowText(dlg.m_Value);
				OnChangeSetupcondition();

			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPMASKTAL2)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=255;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupMaskTalCond2=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPMASKTAL2)->SetWindowText(dlg.m_Value);
				OnChangeSetupcondition();

			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CConditionMaskSetup_2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


void CConditionMaskSetup_2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString s;
	switch(nIDEvent)
	{
		case 1:
			if(m_SetupMaskTalCond1<3)
			{
				m_SetupMaskTalCond1++;
				UpdateData(false);
			}	
			break;
		case 2:
			if(m_SetupMaskTalCond1>0)
			{
				m_SetupMaskTalCond1--;
				UpdateData(false);
			}	
			break;
		case 3:
			if(m_SetupMaskTalCond2<255)
			{
				m_SetupMaskTalCond2+=5;
				UpdateData(false);
			}
			break;
		case 4:
			if(m_SetupMaskTalCond2>0)
			{
				m_SetupMaskTalCond2-=5;
				UpdateData(false);
			}	
			break;
	}		
	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CConditionMaskSetup_2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
  

void CConditionMaskSetup_2::OnClickedRusemasktal0()
{
	m_SetupMaskTalUse=0;
}

void CConditionMaskSetup_2::OnRusemasktal1()
{	
	m_SetupMaskTalUse=1;
}
 


void CConditionMaskSetup_2::OnChangeSetupcondition(void)
{
	if(g_OffInspection && !m_SetupMaskTalUse && NSE_DT_ISBULK==0)
	{
		int Temp_cond1 = g_TalMask_Bright;
		int Temp_cond2 = g_TalMask_Bright2;
		g_TalMask_Bright = m_SetupMaskTalCond1;
		g_TalMask_Bright2 = m_SetupMaskTalCond2;
		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
		pView->OnRecognition_Test(g_3D_Count);
		g_TalMask_Bright = Temp_cond1;	
		g_TalMask_Bright2 = Temp_cond2;	
	}
}
