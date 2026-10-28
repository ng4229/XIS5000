// InterockDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "InterockDlg.h"
#include "afxdialogex.h"


// CInterockDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInterockDlg, CDialogEx)

CInterockDlg::CInterockDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInterockDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_nID = CInterockDlg::IDD;
	What_Interock=0;
}

CInterockDlg::~CInterockDlg()
{
}

void CInterockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICINTEROCK, m_PicInterock);
}


BEGIN_MESSAGE_MAP(CInterockDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CInterockDlg 메시지 처리기입니다.


BOOL CInterockDlg::Create()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::Create(m_nID, m_pParent);
}


BOOL CInterockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 672, 119, 0, 0, SWP_NOSIZE | SWP_NOZORDER);	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CInterockDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CInterockDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	DrawInterock();
}

void CInterockDlg::DrawInterock()
{
	CRect rect;
	CBitmap bmp;
	BITMAP  bi;
	
	if(g_NationalCode==0)
	{
		switch(What_Interock)
		{
			case 0:
				bmp.LoadBitmapW(IDB_INTEROCK_FRONT);
				break;
			case 1:
				bmp.LoadBitmapW(IDB_INTEROCK_EMER);
				break;
			case 3:
				bmp.LoadBitmapW(IDB_INTEROCK_EMER);
				break;
			case 7:
				bmp.LoadBitmapW(IDB_INTEROCK_LEFT_EX);
				break;
			case 15:
				bmp.LoadBitmapW(IDB_INTEROCK_RIGHT_EX);
				break;
			case 31:
				bmp.LoadBitmapW(IDB_INTEROCK_MONITOR);
				break;
			case 63:
				bmp.LoadBitmapW(IDB_INTEROCK_BACK);
				break;
			case 80:
				bmp.LoadBitmapW(IDB_INTEROCK_RIGHT_HAND);
				break;
			case 81:
				bmp.LoadBitmapW(IDB_INTEROCK_LEFT_HAND);
				break;
		}
	}
	else if(g_NationalCode==1)
	{
		switch(What_Interock)
		{
			case 0:
				bmp.LoadBitmapW(IDB_INTEROCK_FRONT_ENG);
				break;
			case 1:
				bmp.LoadBitmapW(IDB_INTEROCK_EMER_ENG);
				break;
			case 3:
				bmp.LoadBitmapW(IDB_INTEROCK_EMER_ENG);
				break;
			case 7:
				bmp.LoadBitmapW(IDB_INTEROCK_LEFT_EX_ENG);
				break;
			case 15:
				bmp.LoadBitmapW(IDB_INTEROCK_RIGHT_EX_ENG);
				break;
			case 31:
				bmp.LoadBitmapW(IDB_INTEROCK_MONITOR_ENG);
				break;
			case 63:
				bmp.LoadBitmapW(IDB_INTEROCK_BACK_ENG);
				break;
			case 80:
				bmp.LoadBitmapW(IDB_INTEROCK_RIGHT_HAND_ENG);
				break;
			case 81:
				bmp.LoadBitmapW(IDB_INTEROCK_LEFT_HAND_ENG);
				break;
		}		
	}
	else if(g_NationalCode==2)
	{
		switch(What_Interock)
		{
			case 0:
				bmp.LoadBitmapW(IDB_INTEROCK_FRONT_CHI);
				break;
			case 1:
				bmp.LoadBitmapW(IDB_INTEROCK_EMER_CHI);
				break;
			case 3:
				bmp.LoadBitmapW(IDB_INTEROCK_EMER_CHI);
				break;
			case 7:
				bmp.LoadBitmapW(IDB_INTEROCK_LEFT_EX_CHI);
				break;
			case 15:
				bmp.LoadBitmapW(IDB_INTEROCK_RIGHT_EX_CHI);
				break;
			case 31:
				bmp.LoadBitmapW(IDB_INTEROCK_MONITOR_CHI);
				break;
			case 63:
				bmp.LoadBitmapW(IDB_INTEROCK_BACK_CHI);
				break;
			case 80:
				bmp.LoadBitmapW(IDB_INTEROCK_RIGHT_HAND_CHI);
				break;
			case 81:
				bmp.LoadBitmapW(IDB_INTEROCK_LEFT_HAND_CHI);
				break;
		}		
	}
	else if(g_NationalCode==3)
	{
		switch(What_Interock)
		{
			case 0:
				bmp.LoadBitmapW(IDB_INTEROCK_FRONT_SPN);
				break;
			case 1:
				bmp.LoadBitmapW(IDB_INTEROCK_EMER_SPN);
				break;
			case 3:
				bmp.LoadBitmapW(IDB_INTEROCK_EMER_SPN);
				break;
			case 7:
				bmp.LoadBitmapW(IDB_INTEROCK_LEFT_EX_SPN);
				break;
			case 15:
				bmp.LoadBitmapW(IDB_INTEROCK_RIGHT_EX_SPN);
				break;
			case 31:
				bmp.LoadBitmapW(IDB_INTEROCK_MONITOR_SPN);
				break;
			case 63:
				bmp.LoadBitmapW(IDB_INTEROCK_BACK_SPN);
				break;
			case 80:
				bmp.LoadBitmapW(IDB_INTEROCK_RIGHT_HAND_SPN);
				break;
			case 81:
				bmp.LoadBitmapW(IDB_INTEROCK_LEFT_HAND_SPN);
				break;
		}		
	}

	CClientDC dc(this);
	CDC bmDC;
	bmDC.CreateCompatibleDC(&dc);
	bmDC.SelectObject(&bmp);
	m_PicInterock.GetWindowRect(&rect);
	ScreenToClient(&rect);
	bmp.GetBitmap(&bi);
	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),&bmDC,0,0,bi.bmWidth,bi.bmHeight, SRCCOPY);
	bmp.DeleteObject();
	bmDC.DeleteDC();	
}