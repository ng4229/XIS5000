// ConditionSetup_0.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "ConditionSetup_0.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "IntellisenseView.h"
#include "NumPadBack.h"

// CConditionSetup_0 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConditionSetup_0, CDialogEx)

CConditionSetup_0::CConditionSetup_0(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConditionSetup_0::IDD, pParent)
{

	m_SetupCondition1 = 0;
}

CConditionSetup_0::~CConditionSetup_0()
{
}

void CConditionSetup_0::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SETUPCONDITION1, m_SetupCondition1);
	DDX_Control(pDX, IDC_CONDITION1_UP, m_Btcond_Up);
	DDX_Control(pDX, IDC_CONDITION1_DN, m_Btcond_Dn);
	DDX_Control(pDX, IDC_PICCONDITION, m_PicCondition);
}


BEGIN_MESSAGE_MAP(CConditionSetup_0, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CConditionSetup_0 메시지 처리기입니다.


BOOL CConditionSetup_0::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	pen.CreatePen(PS_SOLID, 2, RGB(200, 0, 0));
	pen1.CreatePen(PS_SOLID, 7, RGB(100, 100, 100));
	LoadBackground();

	m_SetupCondition1=g_Condition1;

	SetDlgItemInt(IDC_SETUPCONDITION1, m_SetupCondition1, true);
	
	DrawData();
	HideCaret();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CConditionSetup_0::LoadBackground()
{
	BITMAP bmpInfo;
	m_bmpMainBG.LoadBitmap(IDB_BG_CONDITIONSETUP_0); 
    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 

	m_Btcond_Up.LoadBitmaps(IDB_CONDUPBT,IDB_CONDUPBT_1); 
	m_Btcond_Up.SizeToContent();
	m_Btcond_Dn.LoadBitmaps(IDB_CONDDNBT,IDB_CONDDNBT_1); 
	m_Btcond_Dn.SizeToContent();
}

BOOL CConditionSetup_0::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	if(pMsg->hwnd == m_Btcond_Up.m_hWnd)
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
	else if(pMsg->hwnd == m_Btcond_Dn.m_hWnd)
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
	}
	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPCONDITION1)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=255;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupCondition1=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPCONDITION1)->SetWindowText(dlg.m_Value);
				OnChangeSetupcondition();
				ClearPicGrid();
				DrawData();
				DrawConditionValue();
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CConditionSetup_0::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
		case 1:
			if(m_SetupCondition1>=0 && m_SetupCondition1<255)
			{
				m_SetupCondition1++;
				UpdateData(false);
				ClearPicGrid();
				DrawData();
				DrawConditionValue();
			}
			break;
		case 2:
			
			if(m_SetupCondition1>0 && m_SetupCondition1<=255)
			{
				m_SetupCondition1--;
				UpdateData(false);
				ClearPicGrid();
				DrawData();
				DrawConditionValue();
			}	
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CConditionSetup_0::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();

	DrawData();
	DrawConditionValue();
}

void CConditionSetup_0::DrawData()
{
	CRect rect;
	m_PicCondition.GetClientRect(&rect);

	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	int i, count=1;

	CDC *pDC = m_PicCondition.GetDC();
	pDC->SelectObject(&pen1);


	if(g_BackupRear<g_BackupFront)
	{
		for(i=g_BackupRear; i>=0; i--)
		{
			int real_val=0;
			if(g_BackupCond1[i]<=5)
			{
				real_val = 0; 
			}
			else
			{
				real_val = (h * (g_BackupCond1[i]-5))/255;
			}
			if((rect.right-rect.left-count)<=0)
			{
				break;
			}
			pDC->MoveTo(rect.right-count-5, rect.bottom);
			pDC->LineTo(rect.right-count-5, rect.bottom - real_val);
			count+=10;

		}

		for(i=9999; i>g_BackupFront; i--)
		{
			int real_val=0;
			if(g_BackupCond1[i]<0)
			{
				real_val = h; 
			}
			else
			{
				real_val = (h * (g_BackupCond1[i]-5))/255;
			}
			if((rect.right-rect.left-count)<=0)
			{
				break;
			}
			pDC->MoveTo(rect.right-count-5, rect.bottom);
			pDC->LineTo(rect.right-count-5, rect.bottom - real_val);
			count+=10;
		}
		
	}
	else
	{
		for(i=g_BackupRear; i>g_BackupFront; i--)
		{	
			int real_val=0;
			if(g_BackupCond1[i]<0)
			{
				real_val = h; 
			}
			else
			{
				real_val = (h * (g_BackupCond1[i]-5))/255;
			}
			if((rect.right-rect.left-count)<=0)
			{
				break;
			}
			pDC->MoveTo(rect.right-count-5, rect.bottom);
			pDC->LineTo(rect.right-count-5, rect.bottom - real_val);
			count+=10;
		}
	}
	ReleaseDC(pDC);
}

void CConditionSetup_0::DrawConditionValue()
{
	CRect rect;
	m_PicCondition.GetClientRect(&rect);

	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	CDC *pDC = m_PicCondition.GetDC();
	pDC->SelectObject(&pen);
	
	int real_val = (h * (m_SetupCondition1))/256;
	if(real_val<=1)
		real_val=2;
	if(m_SetupCondition1>=254)
		real_val=h-2;

	pDC->MoveTo(rect.right-1, rect.bottom-real_val);
	pDC->LineTo(rect.left+1, rect.bottom-real_val);
	ReleaseDC(pDC);
}

void CConditionSetup_0::ClearPicGrid()
{
	CRect rect;
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_CONDITIONGRID);
	CClientDC dc(this);
	CDC bmDC;
	bmDC.CreateCompatibleDC(&dc);
	bmDC.SelectObject(&bmp);
	m_PicCondition.GetWindowRect(&rect);
	ScreenToClient(&rect);
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),&bmDC,0,0, SRCCOPY);
	bmp.DeleteObject();
	bmDC.DeleteDC();
}

void CConditionSetup_0::OnChangeSetupcondition()
{
	if(g_OffInspection && NSE_DT_ISBULK==0){//카톤검사만
		int Temp_g_Condition1 = g_Condition1;
		g_Condition1 = m_SetupCondition1;
		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
		pView->OnRecognition_Test(g_3D_Count);
		g_Condition1 = Temp_g_Condition1;	
	}
}
