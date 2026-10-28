#include "StdAfx.h"
#include "SetupMenu4_TabCtrl.h"

//20200331
#include "SetupMenu4_Sub0.h"
#include "SetupMenu4_Sub1.h"
#include "SetupMenu4_Sub2.h"

CSetupMenu4_TabCtrl::CSetupMenu4_TabCtrl(void)
{
	::ZeroMemory(&m_tabPagesSub, sizeof(m_tabPagesSub));
	/*
	m_tabPagesSub[0]=new CSetupMenu4_Sub0;
	m_tabPagesSub[1]=new CSetupMenu4_Sub1;
	m_tabPagesSub[2]=new CSetupMenu4_Sub2;

	m_nNumberOfPages=3;
	*/
	if(g_AdminLevel==1)
	{
		m_tabPagesSub[0]=new CSetupMenu4_Sub0;
	//	m_tabPagesSub[1]=new CSetupMenu4_Sub4;
	//	m_tabPagesSub[2]=new CSetupMenu4_Sub2;
	//	m_tabPagesSub[3]=new CSetupMenu4_Sub4;

		m_nNumberOfPages=1;
	}
	else
	{
		m_tabPagesSub[0]=new CSetupMenu4_Sub0;
		m_tabPagesSub[1]=new CSetupMenu4_Sub1;
		m_tabPagesSub[2]=new CSetupMenu4_Sub2;
	//	m_tabPagesSub[3]=new CSetupMenu4_Sub4;

		m_nNumberOfPages=3;
	}
}

CSetupMenu4_TabCtrl::~CSetupMenu4_TabCtrl(void)
{
	int nCount=0;
	for(nCount=0; nCount < m_nNumberOfPages; nCount++)
	{
		delete m_tabPagesSub[nCount];
	}
}


BEGIN_MESSAGE_MAP(CSetupMenu4_TabCtrl, CTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



void CSetupMenu4_TabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CTabCtrl::OnLButtonDown(nFlags, point);
	if(m_tabCurrent != GetCurFocus())
	{
		m_tabPagesSub[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent=GetCurFocus();
		m_tabPagesSub[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPagesSub[m_tabCurrent]->SetFocus();
	}
}


void CSetupMenu4_TabCtrl::Init()
{
	m_tabCurrent=0;
/*	m_tabPagesSub[0]->Create(IDD_SETUP_MENU_4_SUB0, this);
	m_tabPagesSub[1]->Create(IDD_SETUP_MENU_4_SUB1, this);
	m_tabPagesSub[2]->Create(IDD_SETUP_MENU_4_SUB2, this);
	*/
	if(g_AdminLevel==1)
	{
		m_tabPagesSub[0]->Create(IDD_SETUP_MENU_4_SUB0, this);
	//	m_tabPagesSub[1]->Create(IDD_SETUP_MENU_4_SUB4, this);
	//	m_tabPagesSub[2]->Create(IDD_SETUP_MENU_4_SUB2, this);
	//	m_tabPagesSub[3]->Create(IDD_SETUP_MENU_4_SUB4, this);
	}
	else
	{
		m_tabPagesSub[0]->Create(IDD_SETUP_MENU_4_SUB0, this);
		m_tabPagesSub[1]->Create(IDD_SETUP_MENU_4_SUB1, this);
		m_tabPagesSub[2]->Create(IDD_SETUP_MENU_4_SUB2, this);
	//	m_tabPagesSub[3]->Create(IDD_SETUP_MENU_4_SUB4, this);
	}

	SetRectangle();
}

void CSetupMenu4_TabCtrl::SetRectangle()
{
	/*
	m_tabCurrent=0;
	m_tabPagesSub[0]->ShowWindow(SW_SHOW);
	m_tabPagesSub[1]->ShowWindow(SW_HIDE);
	m_tabPagesSub[2]->ShowWindow(SW_HIDE);
	*/
	if(g_AdminLevel==1)
	{
		m_tabCurrent=0;
		m_tabPagesSub[0]->ShowWindow(SW_SHOW);
	//	m_tabPagesSub[1]->ShowWindow(SW_HIDE);
	}
	else
	{
		m_tabCurrent=0;
		m_tabPagesSub[0]->ShowWindow(SW_SHOW);
		m_tabPagesSub[1]->ShowWindow(SW_HIDE);
		m_tabPagesSub[2]->ShowWindow(SW_HIDE);
	//	m_tabPagesSub[3]->ShowWindow(SW_HIDE);
	}
	
	CRect tabRect;
	CRect itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(tabRect);
	GetItemRect(0, itemRect);

	nX=itemRect.left+690;
	nY=itemRect.bottom+70+113;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-33;

	//itemrect top 2, bottom 30 left 2 right 86
	//tabrect top 0 bottom 382, left 0 right 290
	int i=0;
	m_tabPagesSub[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_NOZORDER |SWP_SHOWWINDOW);
	for(i=1; i<m_nNumberOfPages; i++)
	{
		m_tabPagesSub[i]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_NOZORDER |SWP_HIDEWINDOW);
	}
}
