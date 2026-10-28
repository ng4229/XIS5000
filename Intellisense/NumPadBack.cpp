// NumPadBack.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "NumPadBack.h"
#include "afxdialogex.h"

#include "NumPad.h"
// CNumPadBack 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNumPadBack, CDialogEx)

CNumPadBack::CNumPadBack(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNumPadBack::IDD, pParent)
{
	m_Min=0;
	m_Max=255;
}

CNumPadBack::~CNumPadBack()
{
}

void CNumPadBack::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BG_PICTURE, m_Bg_Picture);
}

BEGIN_MESSAGE_MAP(CNumPadBack, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNumPadBack 메시지 처리기입니다.


BOOL CNumPadBack::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadBackgound();
	SetTimer(1,100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

typedef BOOL (WINAPI *SLWA)(HWND , COLORREF , BYTE , DWORD);
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002
#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED			0x00080000
#endif

void CNumPadBack::LoadBackgound(void)
{
	CRect rect;
	m_Bg_Picture.GetWindowRect(&rect);
	ScreenToClient(&rect);
	SetWindowPos(NULL, 672, 119, rect.Width(), rect.Height(), SWP_NOSIZE); 

	HMODULE hmod = GetModuleHandle(_T("USER32.dll"));
	SLWA m_ProcessAttributes;
	m_ProcessAttributes = (SLWA)GetProcAddress(hmod, "SetLayeredWindowAttributes");
	::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	m_ProcessAttributes(GetSafeHwnd(), RGB(152, 159, 178), 150, LWA_ALPHA);
}

BOOL CNumPadBack::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CNumPadBack::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent==1)
	{
		KillTimer(1);
		CNumPad dlg;
		dlg.m_Min=m_Min;
		dlg.m_Max=m_Max;
		int result=dlg.DoModal();
		if(result==IDOK)
		{
			m_Value=dlg.m_Value;
			if(_wtoi(m_Value)<m_Min)
				m_Value.Format(_T("%d"), m_Min);
			else if(_wtoi(m_Value)>m_Max)
				m_Value.Format(_T("%d"), m_Max);

			EndDialog(IDOK);
		}
		else
		{
			EndDialog(IDCANCEL);
		}
	}
	
	CDialogEx::OnTimer(nIDEvent);
}
