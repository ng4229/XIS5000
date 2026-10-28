
// ChildFrm.h : CChildFrame 클래스의 인터페이스
//
#include "FlatSplitterWnd.h"

#pragma once

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// 특성입니다.
public:
	CView* GetDisplayView();
	CView* GetControlView();
	CFlatSplitterWnd m_wndSplitter;

// 작업입니다.
public:

// 재정의입니다.
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// 구현입니다.
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 생성된 메시지 맵 함수
protected:
	CView* m_pDisplayView;
	CView* m_pControlView;
	DECLARE_MESSAGE_MAP()
};
