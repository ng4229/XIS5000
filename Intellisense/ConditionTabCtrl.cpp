#include "StdAfx.h"
#include "ConditionTabCtrl.h"

#include "ConditionSetup_0.h"
#include "ConditionSetup_1.h"
#include "ConditionSetup_2.h"
#include "ConditionSetup_3.h"
#include "ConditionSetup_4.h"
#include "ConditionSetup_5.h"

#include "ConditionOmitSetup_0.h"
#include "ConditionOmitSetup_1.h"
//#include "ConditionOmitSetup_2.h"

#include "ConditionMaskSetup_0.h"
#include "ConditionMaskSetup_1.h"
#include "ConditionMaskSetup_2.h"

CConditionTabCtrl::CConditionTabCtrl(void)
{
	::ZeroMemory(&m_tabPagesNg, sizeof(m_tabPagesNg));
	::ZeroMemory(&m_tabPagesOmit, sizeof(m_tabPagesOmit));
	::ZeroMemory(&m_tabPagesMask, sizeof(m_tabPagesMask));

	m_tabPagesNg[0]=new CConditionSetup_0;
	m_tabPagesNg[1]=new CConditionSetup_1;
	m_tabPagesNg[2]=new CConditionSetup_2;
	m_tabPagesNg[3]=new CConditionSetup_3;
	m_tabPagesNg[4]=new CConditionSetup_4;
	m_tabPagesNg[5]=new CConditionSetup_5;
	
	m_tabPagesOmit[0]=new CConditionOmitSetup_0;
	m_tabPagesOmit[1]=new CConditionOmitSetup_1;

	m_tabPagesMask[0]=new CConditionMaskSetup_0;
	m_tabPagesMask[1]=new CConditionMaskSetup_1;
	m_tabPagesMask[2]=new CConditionMaskSetup_2;

	m_nNumberOfPagesNg=6;
	//20160428
	//m_nNumberOfPagesOmit=2;
	m_nNumberOfPagesOmit=2;
	m_nNumberOfPagesMask=3;
}


CConditionTabCtrl::~CConditionTabCtrl(void)
{
	int nCount=0;
	for(nCount=0; nCount < m_nNumberOfPagesNg; nCount++){
		delete m_tabPagesNg[nCount];
	}
	for(nCount=0; nCount < m_nNumberOfPagesOmit; nCount++){
		delete m_tabPagesOmit[nCount];
	}
	for(nCount=0; nCount < m_nNumberOfPagesMask; nCount++){
		delete m_tabPagesMask[nCount];
	}
}

BEGIN_MESSAGE_MAP(CConditionTabCtrl, CTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


void CConditionTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CTabCtrl::OnLButtonDown(nFlags, point);
	if(m_Kindtab==0)
	{
		if(m_tabCurrent != GetCurFocus())
		{
			m_tabPagesNg[m_tabCurrent]->ShowWindow(SW_HIDE);
			m_tabCurrent=GetCurFocus();
			m_tabPagesNg[m_tabCurrent]->ShowWindow(SW_SHOW);
			m_tabPagesNg[m_tabCurrent]->SetFocus();
		}
	}
	else if(m_Kindtab==1)
	{
		if(m_tabCurrent != GetCurFocus())
		{
			m_tabPagesOmit[m_tabCurrent]->ShowWindow(SW_HIDE);
			m_tabCurrent=GetCurFocus();
			m_tabPagesOmit[m_tabCurrent]->ShowWindow(SW_SHOW);
			m_tabPagesOmit[m_tabCurrent]->SetFocus();
		}
	}
	else if(m_Kindtab==2)
	{
		if(m_tabCurrent != GetCurFocus())
		{
			m_tabPagesMask[m_tabCurrent]->ShowWindow(SW_HIDE);
			m_tabCurrent=GetCurFocus();
			m_tabPagesMask[m_tabCurrent]->ShowWindow(SW_SHOW);
			m_tabPagesMask[m_tabCurrent]->SetFocus();
		}
	}
}

void CConditionTabCtrl::Init()
{
	m_tabCurrent=0;
	m_tabPagesNg[0]->Create(IDD_CONDITIONSETUP_0, this);
	m_tabPagesNg[1]->Create(IDD_CONDITIONSETUP_1, this);
	m_tabPagesNg[2]->Create(IDD_CONDITIONSETUP_2, this);
	m_tabPagesNg[3]->Create(IDD_CONDITIONSETUP_3, this);
	m_tabPagesNg[4]->Create(IDD_CONDITIONSETUP_4, this);
	m_tabPagesNg[5]->Create(IDD_CONDITIONSETUP_5, this);

	m_tabPagesOmit[0]->Create(IDD_CONDITIONOMITSETUP_0, this);
	//20160428
	m_tabPagesOmit[1]->Create(IDD_CONDITIONOMITSETUP_1, this);
//	m_tabPagesOmit[2]->Create(IDD_CONDITIONOMITSETUP_2, this);
//	m_tabPagesOmit[1]->Create(IDD_CONDITIONOMITSETUP_2, this);

	m_tabPagesMask[0]->Create(IDD_CONDITIONMASKSETUP_0, this);
	m_tabPagesMask[1]->Create(IDD_CONDITIONMASKSETUP_1, this);
	m_tabPagesMask[2]->Create(IDD_CONDITIONMASKSETUP_2, this);
	
	SetRectangle();
}

void CConditionTabCtrl::SetRectangle()
{
	m_tabCurrent=0;
	if(m_Kindtab==0)
	{
		m_tabPagesNg[0]->ShowWindow(SW_SHOW);
		m_tabPagesOmit[0]->ShowWindow(SW_HIDE);
		m_tabPagesMask[0]->ShowWindow(SW_HIDE);
	}
	else if(m_Kindtab==1)
	{
		m_tabPagesNg[0]->ShowWindow(SW_HIDE);
		m_tabPagesOmit[0]->ShowWindow(SW_SHOW);
		m_tabPagesMask[0]->ShowWindow(SW_HIDE);
	}
	else if(m_Kindtab==2)
	{
		m_tabPagesNg[0]->ShowWindow(SW_HIDE);
		m_tabPagesOmit[0]->ShowWindow(SW_HIDE);
		m_tabPagesMask[0]->ShowWindow(SW_SHOW);
	}

	m_tabPagesNg[1]->ShowWindow(SW_HIDE);
	m_tabPagesNg[2]->ShowWindow(SW_HIDE);
	m_tabPagesNg[3]->ShowWindow(SW_HIDE);
	m_tabPagesNg[4]->ShowWindow(SW_HIDE);
	m_tabPagesNg[5]->ShowWindow(SW_HIDE);
		
	m_tabPagesOmit[1]->ShowWindow(SW_HIDE);
	//20160428
//	m_tabPagesOmit[2]->ShowWindow(SW_HIDE);
	
	m_tabPagesMask[1]->ShowWindow(SW_HIDE);
	m_tabPagesMask[2]->ShowWindow(SW_HIDE);
	

	CRect tabRect;
	CRect itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(tabRect);
	GetItemRect(0, itemRect);

	nX=itemRect.left+672+16;
	nY=itemRect.bottom+1+119+108;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom;

	int i=0;
	if(m_Kindtab==0)
	{
		m_tabPagesNg[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_NOZORDER |SWP_SHOWWINDOW);
		for(i=1; i<m_nNumberOfPagesNg; i++)
		{
			m_tabPagesNg[i]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_NOZORDER |SWP_HIDEWINDOW);
		}
	}
	else if(m_Kindtab==1)
	{
		m_tabPagesOmit[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_NOZORDER |SWP_SHOWWINDOW);
		for(i=1; i<m_nNumberOfPagesOmit; i++)
		{
			m_tabPagesOmit[i]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_NOZORDER |SWP_HIDEWINDOW);
		}
	}
	else if(m_Kindtab==2)
	{
		m_tabPagesMask[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_NOZORDER |SWP_SHOWWINDOW);
		for(i=1; i<m_nNumberOfPagesMask; i++)
		{
			m_tabPagesMask[i]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_NOZORDER |SWP_HIDEWINDOW);
		}
	}
}
