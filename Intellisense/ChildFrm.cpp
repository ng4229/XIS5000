
// ChildFrm.cpp : CChildFrame 클래스의 구현
//

#include "stdafx.h"
#include "Intellisense.h"

#include "ChildFrm.h"

#include "CtrlPanelView.h"
#include "IntellisenseDoc.h"
#include "IntellisenseView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
END_MESSAGE_MAP()

// CChildFrame 생성/소멸

CChildFrame::CChildFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_pControlView = NULL;
	m_pDisplayView = NULL;	
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU 
		| WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;
	cs.style &= ~FWS_ADDTOTITLE;
	cs.style &= ~WS_SYSMENU;
	cs.style &= ~WS_CAPTION; 
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrm->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	pFrm->m_pFirstChildFrm = this;

	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

CView* CChildFrame::GetControlView()
{
	return m_pControlView;
}

CView* CChildFrame::GetDisplayView()
{
	return m_pDisplayView;
}

// CChildFrame 진단

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 메시지 처리기
BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)		
{
	// TODO: Add your specialized code here and/or call the base class
	m_wndSplitter.CreateStatic(this, 2, 1);
	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CIntellisenseView), /*csDisplay*/CSize(1024,663), pContext);
	m_wndSplitter.CreateView(1,0,RUNTIME_CLASS(CCtrlPanelView), /*csControl*/CSize(1024,105), pContext);

	m_pDisplayView = (CView*)m_wndSplitter.GetPane(0,0);
    m_pControlView = (CView*)m_wndSplitter.GetPane(1,0);
	SetActiveView(m_pDisplayView);
	SetActiveView(m_pControlView);
	//	m_wndSplitter.RecalcLayout();
	return true;
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}
