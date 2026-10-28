// ConditionOmitSetup_0.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "ConditionOmitSetup_0.h"
#include "afxdialogex.h"

#include "IntellisenseView.h"
#include "MainFrm.h"
#include "ChildFrm.h"


#include "NumPadBack.h"
// CConditionOmitSetup_0 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConditionOmitSetup_0, CDialogEx)

CConditionOmitSetup_0::CConditionOmitSetup_0(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConditionOmitSetup_0::IDD, pParent)
{

	m_SetupOmitCond1 = 0;
	m_SetupOmitCond2 = 0;
	m_SetupOmitCond3 = 0;
	m_SetupOmitCond4 = 0;
	m_SetupOmitCond5 = 0;
	m_SetupOmitUse = 0;
	m_DispOmitCond3 = 0;
}

CConditionOmitSetup_0::~CConditionOmitSetup_0()
{
}

void CConditionOmitSetup_0::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SETUPOMITCONDITION1, m_SetupOmitCond1);
	DDX_Text(pDX, IDC_SETUPOMITCONDITION2, m_SetupOmitCond2);
	DDX_Text(pDX, IDC_SETUPOMITCONDITION3, m_SetupOmitCond3);
	DDX_Text(pDX, IDC_SETUPOMITCONDITION4, m_SetupOmitCond4);
	DDX_Text(pDX, IDC_SETUPOMITCONDITION5, m_SetupOmitCond5);
	DDX_Radio(pDX, IDC_RUSEOMIT_0, m_SetupOmitUse);
	DDX_Control(pDX, IDC_OMITCONDITION1_UP, m_Btcond1_Up);
	DDX_Control(pDX, IDC_OMITCONDITION2_UP, m_Btcond2_Up);
	DDX_Control(pDX, IDC_OMITCONDITION3_UP, m_Btcond3_Up);
	DDX_Control(pDX, IDC_OMITCONDITION4_UP, m_Btcond4_Up);
	DDX_Control(pDX, IDC_OMITCONDITION5_UP, m_Btcond5_Up);
	DDX_Control(pDX, IDC_OMITCONDITION1_DN, m_Btcond1_Dn);
	DDX_Control(pDX, IDC_OMITCONDITION2_DN, m_Btcond2_Dn);
	DDX_Control(pDX, IDC_OMITCONDITION3_DN, m_Btcond3_Dn);
	DDX_Control(pDX, IDC_OMITCONDITION4_DN, m_Btcond4_Dn);
	DDX_Control(pDX, IDC_OMITCONDITION5_DN, m_Btcond5_Dn);
	DDX_Text(pDX, IDC_DISPOMITCONDITION3, m_DispOmitCond3);
	DDX_Check(pDX, IDC_CHECKDISP, m_DispArea);
}


BEGIN_MESSAGE_MAP(CConditionOmitSetup_0, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RUSEOMIT_0, &CConditionOmitSetup_0::OnClickedRuseomit0)
	ON_COMMAND(IDC_RUSEOMIT_1, &CConditionOmitSetup_0::OnRuseomit1)
	ON_EN_SETFOCUS(IDC_DISPOMITCONDITION3, &CConditionOmitSetup_0::OnSetfocusDispomitcondition3)
END_MESSAGE_MAP()


// CConditionOmitSetup_0 메시지 처리기입니다.


BOOL CConditionOmitSetup_0::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Brush.CreateStockObject(NULL_BRUSH);
	LoadBackground();
	LoadValue();
	ComputNum(g_3D_Count);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CConditionOmitSetup_0::PreTranslateMessage(MSG* pMsg)
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
		}
	}//	

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPOMITCONDITION1)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=g_Height/2;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupOmitCond1=_wtoi(dlg.m_Value);
			//	if(m_SetupOmitCond1>m_SetupOmitCond2)
				//	m_SetupOmitCond2=m_SetupOmitCond1;
				UpdateData(false);
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPOMITCONDITION2)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=g_Height;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupOmitCond2=_wtoi(dlg.m_Value);
			//	if(m_SetupOmitCond1>m_SetupOmitCond2)
				//	m_SetupOmitCond1=m_SetupOmitCond2;
				UpdateData(false);
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPOMITCONDITION3)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=g_Height*g_Width-10;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupOmitCond3=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPOMITCONDITION3)->SetWindowText(dlg.m_Value);
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPOMITCONDITION4)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=100;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupOmitCond4=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPOMITCONDITION4)->SetWindowText(dlg.m_Value);
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPOMITCONDITION5)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=1000000;
			dlg.m_Min=100;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupOmitCond5=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPOMITCONDITION5)->SetWindowText(dlg.m_Value);
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CConditionOmitSetup_0::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


void CConditionOmitSetup_0::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
		case 1:
			if(m_SetupOmitCond1<g_Height)
			{
				m_SetupOmitCond1++;
				ComputNum(g_3D_Count);
				UpdateData(false);

				//g_EventPaint.PulseEvent();
				//Draw_Data();
			
			}
			break;
		case 2:
			if(m_SetupOmitCond1>0)
			{
				m_SetupOmitCond1--;
				ComputNum(g_3D_Count);
				UpdateData(false);

				//g_EventPaint.PulseEvent();
				//Draw_Data();
			
			}	
			break;

		case 3:
			if(m_SetupOmitCond2<g_Height)
			{
				m_SetupOmitCond2++;
				ComputNum(g_3D_Count);
				UpdateData(false);

				//g_EventPaint.PulseEvent();
				//Draw_Data();    
				
			}
			break;
		case 4:
			if(m_SetupOmitCond2>0)
			{
				m_SetupOmitCond2--;
				ComputNum(g_3D_Count);
				UpdateData(false);

				//g_EventPaint.PulseEvent();
				//Draw_Data();
				
			}	
			break;

		case 5:
			if(m_SetupOmitCond3>=0)
			{
				m_SetupOmitCond3+=100;
				UpdateData(false);
			}
			break;
		case 6:
			if(m_SetupOmitCond3>=100)
			{
				m_SetupOmitCond3-=100;
				UpdateData(false);
			}
			if(m_SetupOmitCond3>=10 && m_SetupOmitCond3<100)  
			{
				m_SetupOmitCond3-=10;
				UpdateData(false);
			}
			if(m_SetupOmitCond3>0 && m_SetupOmitCond3<10)  
			{
				m_SetupOmitCond3-=1;
				UpdateData(false);
			}			
			break;

		case 7:
			if(m_SetupOmitCond4<100)
			{
				m_SetupOmitCond4++;
				UpdateData(false);
			}
			break;
		case 8:
			if(m_SetupOmitCond4>0)
			{
				m_SetupOmitCond4--;
				UpdateData(false);
			}	
			break;

		case 9:
			if(m_SetupOmitCond5>=100)
			{
				m_SetupOmitCond5++;
				UpdateData(false);
			}
			break;
		case 10:
			if(m_SetupOmitCond5>100)
			{
				m_SetupOmitCond5--;
				UpdateData(false);
			}	
			break;
	}	
	CDialogEx::OnTimer(nIDEvent);
}


void CConditionOmitSetup_0::LoadBackground(void)
{
	BITMAP bmpInfo;

	if(g_NationalCode==0)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONOMITSETUP); 
	}
	else if(g_NationalCode==1)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONOMITSETUP_ENG); 
	}
	else if(g_NationalCode==2)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONOMITSETUP_CHI); 
	}
	else if(g_NationalCode==3)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONOMITSETUP_SPN); 
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

	
	SetDlgItemText(IDC_CHECKDISP, g_MessageArr[155]);
}


void CConditionOmitSetup_0::LoadValue()
{
	m_SetupOmitUse = g_OmitCondUse;
	m_SetupOmitCond1 = g_OmitCondition1;
	m_SetupOmitCond2 = g_OmitCondition2;
	m_SetupOmitCond3 = g_OmitCondition3;
	m_SetupOmitCond4 = g_OmitCondition4;
	if(g_OmitCondition5>=100)
		m_SetupOmitCond5 = g_OmitCondition5;
	else
		m_SetupOmitCond5 = 100;
	m_DispArea=g_DispArea;
	if(m_SetupOmitUse==0)
	{
		((CButton*)GetDlgItem(IDC_RUSEOMIT_0))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RUSEOMIT_1))->SetCheck(false);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RUSEOMIT_0))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RUSEOMIT_1))->SetCheck(true);
	}
	SetDlgItemInt(IDC_SETUPOMITCONDITION1, m_SetupOmitCond1, true);
	SetDlgItemInt(IDC_SETUPOMITCONDITION2, m_SetupOmitCond2, true);
	SetDlgItemInt(IDC_SETUPOMITCONDITION3, m_SetupOmitCond3, true);
	SetDlgItemInt(IDC_SETUPOMITCONDITION4, m_SetupOmitCond4, true);
	SetDlgItemInt(IDC_SETUPOMITCONDITION5, m_SetupOmitCond5, true);

	if(m_DispArea==0)
		((CButton*)GetDlgItem(IDC_CHECKDISP))->SetCheck(false);
	else
		((CButton*)GetDlgItem(IDC_CHECKDISP))->SetCheck(true);
}


HBRUSH CConditionOmitSetup_0::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CConditionOmitSetup_0::OnClickedRuseomit0()
{
	m_SetupOmitUse=0;
}


void CConditionOmitSetup_0::OnRuseomit1()
{
	m_SetupOmitUse=1;
}


void CConditionOmitSetup_0::ComputNum(int I_Count)
{
	int count=0;
	/*
	for(int i=g_Margin;i<g_Height-g_Margin;i++)
	{
		for(int j=g_Margin;j<g_Width-g_Margin;j++)
		{
			if(m_SetupOmitCond1==0 && m_SetupOmitCond2==0)
			{
				if(m_Buffer_1[I_Count][i][j]<220)
				{
					count += (220-m_Buffer_1[I_Count][i][j])/10;
				}
			}
			else
			{
				if((int)m_Buffer_1[I_Count][i][j]>=m_SetupOmitCond1 && (int)m_Buffer_1[I_Count][i][j]<=m_SetupOmitCond2)
				{
					if(m_Buffer_1[I_Count][i][j]<220)
					{
						count += (220-m_Buffer_1[I_Count][i][j])/10;
					}
				}
			}
		}
	}*/
	
	//for(int i=g_Margin2+m_SetupOmitCond1;i<=g_Margin2+m_SetupOmitCond2;i++)
	if(m_SetupOmitCond1 == 0 || m_SetupOmitCond2 == 0)
		return;
	/*
	for(int i=g_Margin2+m_SetupOmitCond1;i<=g_Height-m_SetupOmitCond2;i++)
	{
		for(int j=g_Margin; j<g_Width-g_Margin;j++)
		{	
			if(m_Buffer_1[I_Count][i][j]<g_MaskClip_Length)
			{
				count += (g_MaskClip_Length-m_Buffer_1[I_Count][i][j])/10;
			//	count += 1;
			}
		}
	}*/
	int i, j;
	for (i = g_Margin2 + g_OmitCondition1; i <= g_Height - g_OmitCondition2; i++)
	{
		for (j = g_Margin; j < g_Width - g_Margin; j++)
		{
			if (m_Buffer_1[I_Count][i][j] < g_TalMask_Bright2)
			{
				count += (g_TalMask_Bright2 - m_Buffer_1[I_Count][i][j]) / 10;		// 가변설정 값으로 임시조치 20240701
				//		soupcount += 1;
			}
		}
	}
	m_DispOmitCond3=count;
	SetDlgItemInt(IDC_DISPOMITCONDITION3, m_DispOmitCond3);

}


void CConditionOmitSetup_0::OnSetfocusDispomitcondition3()
{
	m_SetupOmitCond3 = m_DispOmitCond3;
	UpdateData(false);
}

//20201123 누락검사 시뮬레이션
void CConditionOmitSetup_0::OnChangeSetupcondition(void)
{
	int Temp = g_OmitCondition1;
	int Temp2 = g_OmitCondition2;


	g_OmitCondition1 = m_SetupOmitCond1;
	g_OmitCondition2 = m_SetupOmitCond2;


	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
	pView->OnRecognition_Test(g_3D_Count);

	g_OmitCondition1 = Temp;
	g_OmitCondition2 = Temp2;
}

void CConditionOmitSetup_0::Draw_Data(void)
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
	pen2.CreatePen(PS_SOLID, 2, RGB(238, 43, 233));

	pDC->SelectObject(&pen2);
	
//	UpdateData(false);
	int Dent_Up=GetDlgItemInt(IDC_SETUPOMITCONDITION2);
	int Dent_Down=GetDlgItemInt(IDC_SETUPOMITCONDITION1);
	int real_val2 = (h*Dent_Up)/g_Height;
	int real_val = (h*Dent_Down)/g_Height;

	pDC->MoveTo(1, h-real_val);
	pDC->LineTo((rect.Width()-5), h-real_val);

	pDC->MoveTo(1, real_val2);
	pDC->LineTo((rect.Width()-5), real_val2);





	ReleaseDC(pDC);
}
