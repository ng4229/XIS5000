// ReportTabCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ReportTabCtrl.h"


// CReportTabCtrl
#include "DlgPage1.h"
#include "DlgPage2.h"
#include "DlgPage3.h"
#include "DlgPage4.h"
#include "DlgPage5.h"
#include "DlgPage6.h"
#include "DlgPage7.h"

IMPLEMENT_DYNAMIC(CReportTabCtrl, CTabCtrl)

CReportTabCtrl::CReportTabCtrl()
{
	::ZeroMemory(&m_tabPages, sizeof(m_tabPages));

	m_tabPages[0]=new CDlgPage1;
	m_tabPages[1]=new CDlgPage2;
	m_tabPages[2]=new CDlgPage3;
	m_tabPages[3]=new CDlgPage4;
	m_tabPages[4]=new CDlgPage5;
	m_tabPages[5]=new CDlgPage6;
	m_tabPages[6] = new CDlgPage7;

	m_nNumberOfPages=7;
}

CReportTabCtrl::~CReportTabCtrl()
{
	for(int nCount=0; nCount < m_nNumberOfPages; nCount++){
		delete m_tabPages[nCount];
	}
}


void CReportTabCtrl::Init()
{
	m_tabCurrent=0;

	m_tabPages[0]->Create(IDD_PAGE_1, this);
	m_tabPages[1]->Create(IDD_PAGE_2, this);
	m_tabPages[2]->Create(IDD_PAGE_3, this);
	m_tabPages[3]->Create(IDD_PAGE_4, this);
	m_tabPages[4]->Create(IDD_PAGE_5, this);
	m_tabPages[5]->Create(IDD_PAGE_6, this);
	m_tabPages[6]->Create(IDD_PAGE_7, this);

	m_tabPages[0]->ShowWindow(SW_SHOW);
	m_tabPages[1]->ShowWindow(SW_HIDE);
	m_tabPages[2]->ShowWindow(SW_HIDE);
	m_tabPages[3]->ShowWindow(SW_HIDE);
	m_tabPages[4]->ShowWindow(SW_HIDE);
	m_tabPages[5]->ShowWindow(SW_HIDE);
	m_tabPages[6]->ShowWindow(SW_HIDE);

	SetRectangle();
}

void CReportTabCtrl::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount=1; nCount < m_nNumberOfPages; nCount++){
		m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}

}
BEGIN_MESSAGE_MAP(CReportTabCtrl, CTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CReportTabCtrl 메시지 처리기입니다.


void CReportTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CTabCtrl::OnLButtonDown(nFlags, point);

	if(m_tabCurrent != GetCurFocus())
	{
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent=GetCurFocus();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();
	}
}
