// SpaceMsg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "SpaceMsg.h"
#include "afxdialogex.h"


// CSpaceMsg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSpaceMsg, CDialogEx)

CSpaceMsg::CSpaceMsg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpaceMsg::IDD, pParent)
{
	m_pParent = pParent;
	m_nID = CSpaceMsg::IDD;
}

CSpaceMsg::~CSpaceMsg()
{
}

void CSpaceMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICSPACE, m_Pic);
}


BEGIN_MESSAGE_MAP(CSpaceMsg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSpaceMsg 메시지 처리기입니다.


BOOL CSpaceMsg::Create()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::Create(m_nID, m_pParent);
}


BOOL CSpaceMsg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 672, 119, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CSpaceMsg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSpaceMsg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DrawPic();
}


void CSpaceMsg::DrawPic(void)
{
	CRect rect;
	CBitmap bmp;
	BITMAP  bi;
	if(g_NationalCode==0)
	{
		bmp.LoadBitmapW(IDB_BG_SPACEDLG);
	}
	else if(g_NationalCode==1)
	{
		bmp.LoadBitmapW(IDB_BG_SPACEDLG_ENG);
	}
	else if(g_NationalCode==2)
	{
		bmp.LoadBitmapW(IDB_BG_SPACEDLG_CHI);
	}
	else if(g_NationalCode==3)
	{
		bmp.LoadBitmapW(IDB_BG_SPACEDLG_SPN);
	}
	
	CClientDC dc(this);
	CDC bmDC;
	bmDC.CreateCompatibleDC(&dc);
	bmDC.SelectObject(&bmp);
	m_Pic.GetWindowRect(&rect);
	ScreenToClient(&rect);
	bmp.GetBitmap(&bi);
	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),&bmDC,0,0,bi.bmWidth,bi.bmHeight, SRCCOPY);
	bmp.DeleteObject();
	bmDC.DeleteDC();	
}
