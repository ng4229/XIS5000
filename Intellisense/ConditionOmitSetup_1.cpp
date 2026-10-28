// ConditionOmitSetup_1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "ConditionOmitSetup_1.h"
#include "afxdialogex.h"

#include "NumPadBack.h"
#include "IntellisenseView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
// CConditionOmitSetup_1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConditionOmitSetup_1, CDialogEx)

CConditionOmitSetup_1::CConditionOmitSetup_1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConditionOmitSetup_1::IDD, pParent)
{
	m_SetupMassUse = 0;
	m_SetupMassCond1 = 0;
	m_SetupMassCond2 = 0;
	m_SetupMassCond3 = 0;
	m_DispOmitCond1 = 0;
	m_DispOmitCond2 = 0;
}

CConditionOmitSetup_1::~CConditionOmitSetup_1()
{
}

void CConditionOmitSetup_1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MASSCONDITION1_DN, m_Btcond1_Dn);
	DDX_Control(pDX, IDC_MASSCONDITION1_UP, m_Btcond1_Up);
	DDX_Control(pDX, IDC_MASSCONDITION2_DN, m_Btcond2_Dn);
	DDX_Control(pDX, IDC_MASSCONDITION2_UP, m_Btcond2_Up);
	DDX_Control(pDX, IDC_MASSCONDITION3_DN, m_Btcond3_Dn);
	DDX_Control(pDX, IDC_MASSCONDITION3_UP, m_Btcond3_Up);
	DDX_Radio(pDX, IDC_RUSEMASS_0, m_SetupMassUse);
	DDX_Text(pDX, IDC_SETUPMASSCONDITION1, m_SetupMassCond1);
	DDX_Text(pDX, IDC_SETUPMASSCONDITION2, m_SetupMassCond2);
	DDX_Text(pDX, IDC_SETUPMASSCONDITION3, m_SetupMassCond3);
	DDX_Text(pDX, IDC_DISPMASSCONDITION1, m_DispOmitCond1);
	DDX_Text(pDX, IDC_DISPMASSCONDITION2, m_DispOmitCond2);
}


BEGIN_MESSAGE_MAP(CConditionOmitSetup_1, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RUSEMASS_0, &CConditionOmitSetup_1::OnClickedRusemass0)
	ON_COMMAND(IDC_RUSEMASS_1, &CConditionOmitSetup_1::OnRusemass1)
	ON_EN_SETFOCUS(IDC_DISPMASSCONDITION1, &CConditionOmitSetup_1::OnSetfocusDispmasscondition1)
//	ON_EN_SETFOCUS(IDC_DISPLEFT, &CConditionOmitSetup_1::OnSetfocusDispleft)
//	ON_EN_SETFOCUS(IDC_DISPRIGHT, &CConditionOmitSetup_1::OnSetfocusDispright)
END_MESSAGE_MAP()


// CConditionOmitSetup_1 메시지 처리기입니다.


BOOL CConditionOmitSetup_1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Brush.CreateStockObject(NULL_BRUSH);
	LoadBackground();
	LoadValue();
	ComputNum();

	SetTimer(100, 200, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CConditionOmitSetup_1::PreTranslateMessage(MSG* pMsg)
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
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPMASSCONDITION1)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=40;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupMassCond1=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPMASSCONDITION1)->SetWindowText(dlg.m_Value);
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPMASSCONDITION2)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=g_Height/2;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupMassCond2=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPMASSCONDITION2)->SetWindowText(dlg.m_Value);
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPMASSCONDITION3)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=g_Height/2;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupMassCond3=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPMASSCONDITION3)->SetWindowText(dlg.m_Value);
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CConditionOmitSetup_1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


void CConditionOmitSetup_1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
		case 1:
			if(m_SetupMassCond1<=40)
			{
				m_SetupMassCond1++;
				UpdateData(false);
			}
			break;
		case 2:
			if(m_SetupMassCond1>0)
			{
				m_SetupMassCond1--;
				UpdateData(false);
			}
			break;
		case 3:
			if(m_SetupMassCond2<g_Height)
			{
				m_SetupMassCond2++;
				g_EventPaint.PulseEvent();
				DrawData();
				UpdateData(false);
			}
			break;
		case 4:
			if(m_SetupMassCond2>0)
			{
				m_SetupMassCond2--;
				g_EventPaint.PulseEvent();
				DrawData();
				UpdateData(false);
			}
			break;
		case 5:
			if(m_SetupMassCond3<g_Height/2)
			{
				m_SetupMassCond3++;
				g_EventPaint.PulseEvent();
				DrawData();
				UpdateData(false);
			}	
			break;
		case 6:
			if(m_SetupMassCond3>5)
			{
				m_SetupMassCond3--;
				g_EventPaint.PulseEvent();
				DrawData();
				UpdateData(false);
			}
			break;
	}	

	if(nIDEvent == 100)
	{
		CString T_R, T_L;
		T_R.Format(_T("%d"), g_Dent_R);
		GetDlgItem(IDC_DISPMASSCONDITION2)->SetWindowText(T_R);

		T_L.Format(_T("%d"), g_Dent_L);
		GetDlgItem(IDC_DISPMASSCONDITION1)->SetWindowText(T_L);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CConditionOmitSetup_1::LoadBackground(void)
{
	BITMAP bmpInfo;
	if(g_NationalCode==0)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONOMITSETUP_1); 
	}
	else if(g_NationalCode==1)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONOMITSETUP_1_ENG); 
	}
	else if(g_NationalCode==2)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONOMITSETUP_1_CHI); 
	}
	else if(g_NationalCode==3)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONOMITSETUP_1_SPN); 
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

}


void CConditionOmitSetup_1::LoadValue()
{
	m_SetupMassUse = g_MassCondUse;//사용여부
	m_SetupMassCond1 = g_MassCondition1; //설정
	m_SetupMassCond2 = g_MassCondition2; //상
	m_SetupMassCond3 = g_MassCondition3; //하


	if(m_SetupMassUse==0)
	{
		((CButton*)GetDlgItem(IDC_RUSEMASS_0))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RUSEMASS_1))->SetCheck(false);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RUSEMASS_0))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RUSEMASS_1))->SetCheck(true);
	}
	SetDlgItemInt(IDC_SETUPMASSCONDITION1, m_SetupMassCond1, true);
	SetDlgItemInt(IDC_SETUPMASSCONDITION2, m_SetupMassCond2, true);
	SetDlgItemInt(IDC_SETUPMASSCONDITION3, m_SetupMassCond3, true);
}


HBRUSH CConditionOmitSetup_1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CConditionOmitSetup_1::OnClickedRusemass0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_SetupMassUse=0;
}


void CConditionOmitSetup_1::OnRusemass1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_SetupMassUse=1;
}


void CConditionOmitSetup_1::OnSetfocusDispmasscondition1()
{
	m_SetupMassCond1 = m_DispOmitCond1;

	SetDlgItemInt(IDC_SETUPMASSCONDITION1, m_SetupMassCond1, true);
}


void CConditionOmitSetup_1::ComputNum(void)
{	
	SetDlgItemInt(IDC_DISPLEFT, g_DispOpenBox1);
	SetDlgItemInt(IDC_DISPRIGHT, g_DispOpenBox2);
	m_DispOmitCond1=(g_DispOpenBox1+g_DispOpenBox2)/2;
	SetDlgItemInt(IDC_DISPMASSCONDITION1, m_DispOmitCond1);
	SetDlgItemInt(IDC_DISPMASSCONDITION2, m_DispOmitCond2);
}



void CConditionOmitSetup_1::DrawData(void)
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
	CDC *pDC = pView->m_GrabImage.GetDC();			
	CRect rect;
	pView->m_GrabImage.GetWindowRect(&rect);
	pDC->SetStretchBltMode(COLORONCOLOR);


	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(55, 192, 250));

	pDC->SelectObject(&pen);
	
//	UpdateData(false);
	int Dent_Up=GetDlgItemInt(IDC_SETUPMASSCONDITION2);
	int Dent_Down=GetDlgItemInt(IDC_SETUPMASSCONDITION3);
	int real_val2 = (h*Dent_Up)/g_Height;
	int real_val = (h*Dent_Down)/g_Height;

	pDC->MoveTo(1, h-real_val);
	pDC->LineTo((rect.Width()-5), h-real_val);

	pDC->MoveTo(1, real_val2);
	pDC->LineTo((rect.Width()-5), real_val2);





	ReleaseDC(pDC);
}
