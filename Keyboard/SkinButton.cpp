// SkinButton.cpp : implementation file
//

#include "stdafx.h"
//#include "MonsterSkin.h"
#include "SkinButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinButton

CSkinButton::CSkinButton()
{
	m_bmpDlgUp = NULL;
	m_bmpDlgDn = NULL;
	m_bmpDlgFs = NULL;
	m_bIsPointed = FALSE;
}

CSkinButton::~CSkinButton()
{
}


BEGIN_MESSAGE_MAP(CSkinButton, CButton)
	//{{AFX_MSG_MAP(CSkinButton)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinButton message handlers
void CSkinButton::LoadBitmaps( CBitmap *dlgUp, CBitmap *dlgDn, CBitmap *dlgFs)
{
	m_bmpDlgUp = dlgUp;
	m_bmpDlgDn = dlgDn;
	m_bmpDlgFs = dlgFs;
}

void CSkinButton::SizeToContent(INT_PTR x, INT_PTR y, INT_PTR nWidth, INT_PTR nHeight, BOOL bRedraw)
{
	m_nXPos = x;
	m_nYPos = y;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	
	MoveWindow(x, y, nWidth, nHeight, bRedraw);
}

void CSkinButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	ASSERT(lpDrawItemStruct != NULL);
	UINT nState = lpDrawItemStruct->itemState;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rc;
	rc.CopyRect(&lpDrawItemStruct->rcItem);

	CDC MemDC;	
	MemDC.CreateCompatibleDC(pDC);	

	CBitmap* bmp = m_bmpDlgUp;
	if( (nState & ODS_SELECTED) && (m_bmpDlgDn != NULL))
	{
		bmp = m_bmpDlgDn;
	}
	
	CBitmap* pbmpOld = MemDC.SelectObject( bmp );		
	pDC->BitBlt(0, 0, m_nWidth, m_nHeight, &MemDC, m_nXPos, m_nYPos, SRCCOPY);
	MemDC.SelectObject( pbmpOld );
	
	ReleaseDC(pDC);
}

void CSkinButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( (GetForegroundWindow() == GetParent() || 
		GetForegroundWindow() == GetParent()->GetOwner()) 
		&& m_bmpDlgFs != NULL )
	{
		CDC* pDC = GetDC();
		
		CDC MemDC;	
		MemDC.CreateCompatibleDC(pDC);			
		
		CBitmap* pbmpOld = MemDC.SelectObject( m_bmpDlgFs );		
		pDC->BitBlt(0, 0, m_nWidth, m_nHeight, &MemDC, m_nXPos, m_nYPos, SRCCOPY);
		MemDC.SelectObject( pbmpOld );
		
		ReleaseDC(pDC);
		
		m_bIsPointed = TRUE;
		SetTimer(1100, 50, NULL);
	}
	CButton::OnMouseMove(nFlags, point);
}

void CSkinButton::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == 1100 )
	{
		KillTimer(1100);
		
		CPoint point;
		GetCursorPos(&point);

		m_bIsPointed = (CWnd::WindowFromPoint(point) == this);

		if (m_bIsPointed)
			SetTimer(1100, 50, NULL);
		else
			Invalidate();
	}

	CButton::OnTimer(nIDEvent);
}
