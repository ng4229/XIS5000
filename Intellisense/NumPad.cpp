// NumPad.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "NumPad.h"
#include "afxdialogex.h"


// CNumPad 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNumPad, CDialogEx)

CNumPad::CNumPad(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNumPad::IDD, pParent)
{

}

CNumPad::~CNumPad()
{
}

void CNumPad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_NUM0, m_Bt_Num0);
	DDX_Control(pDX, IDC_BT_NUM1, m_Bt_Num1);
	DDX_Control(pDX, IDC_BT_NUM2, m_Bt_Num2);
	DDX_Control(pDX, IDC_BT_NUM3, m_Bt_Num3);
	DDX_Control(pDX, IDC_BT_NUM4, m_Bt_Num4);
	DDX_Control(pDX, IDC_BT_NUM5, m_Bt_Num5);
	DDX_Control(pDX, IDC_BT_NUM6, m_Bt_Num6);
	DDX_Control(pDX, IDC_BT_NUM7, m_Bt_Num7);
	DDX_Control(pDX, IDC_BT_NUM8, m_Bt_Num8);
	DDX_Control(pDX, IDC_BT_NUM9, m_Bt_Num9);
	DDX_Control(pDX, IDC_BT_RESET, m_Bt_Reset);
	DDX_Control(pDX, IDCANCEL, m_Bt_Close);
	DDX_Control(pDX, IDOK, m_Bt_Ok);
}


BEGIN_MESSAGE_MAP(CNumPad, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BT_NUM1, &CNumPad::OnBnClickedBtNum1)
	ON_BN_CLICKED(IDC_BT_NUM2, &CNumPad::OnBnClickedBtNum2)
	ON_BN_CLICKED(IDC_BT_NUM3, &CNumPad::OnBnClickedBtNum3)
	ON_BN_CLICKED(IDC_BT_NUM4, &CNumPad::OnBnClickedBtNum4)
	ON_BN_CLICKED(IDC_BT_NUM5, &CNumPad::OnBnClickedBtNum5)
	ON_BN_CLICKED(IDC_BT_NUM6, &CNumPad::OnBnClickedBtNum6)
	ON_BN_CLICKED(IDC_BT_NUM7, &CNumPad::OnBnClickedBtNum7)
	ON_BN_CLICKED(IDC_BT_NUM8, &CNumPad::OnBnClickedBtNum8)
	ON_BN_CLICKED(IDC_BT_NUM9, &CNumPad::OnBnClickedBtNum9)
	ON_BN_CLICKED(IDC_BT_NUM0, &CNumPad::OnBnClickedBtNum0)
	ON_BN_CLICKED(IDC_BT_RESET, &CNumPad::OnBnClickedBtReset)
END_MESSAGE_MAP()


// CNumPad 메시지 처리기입니다.


BOOL CNumPad::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadBackgound();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CNumPad::LoadBackgound()
{
	BITMAP bmpInfo;
	if(g_NationalCode==0)
	{
		m_Bt_Close.LoadBitmaps(IDB_BT_NUMPAD_CANCEL,IDB_BT_NUMPAD_CANCEL_0); 
		m_Bt_Reset.LoadBitmaps(IDB_BT_NUMPAD_RESET,IDB_BT_NUMPAD_RESET_0); 
		m_Bt_Ok.LoadBitmaps(IDB_BT_NUMPAD_OK,IDB_BT_NUMPAD_OK_0); 
	}
	else if(g_NationalCode==1)
	{
		m_Bt_Close.LoadBitmaps(IDB_BT_NUMPAD_CANCEL_ENG,IDB_BT_NUMPAD_CANCEL_0_ENG); 
		m_Bt_Reset.LoadBitmaps(IDB_BT_NUMPAD_RESET_ENG,IDB_BT_NUMPAD_RESET_0_ENG); 
		m_Bt_Ok.LoadBitmaps(IDB_BT_NUMPAD_OK_ENG,IDB_BT_NUMPAD_OK_0_ENG); 
	}
	else if(g_NationalCode==2)
	{
		m_Bt_Close.LoadBitmaps(IDB_BT_NUMPAD_CANCEL_CHI,IDB_BT_NUMPAD_CANCEL_0_CHI); 
		m_Bt_Reset.LoadBitmaps(IDB_BT_NUMPAD_RESET_CHI,IDB_BT_NUMPAD_RESET_0_CHI); 
		m_Bt_Ok.LoadBitmaps(IDB_BT_NUMPAD_OK_CHI,IDB_BT_NUMPAD_OK_0_CHI); 
	}
	else if(g_NationalCode==3)
	{
		m_Bt_Close.LoadBitmaps(IDB_BT_NUMPAD_CANCEL_SPN,IDB_BT_NUMPAD_CANCEL_0_SPN); 
		m_Bt_Reset.LoadBitmaps(IDB_BT_NUMPAD_RESET_SPN,IDB_BT_NUMPAD_RESET_0_SPN); 
		m_Bt_Ok.LoadBitmaps(IDB_BT_NUMPAD_OK_SPN,IDB_BT_NUMPAD_OK_0_SPN); 
	}
	m_bmpMainBG.LoadBitmap(IDB_BG_NUMPAD); 
    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 

	CenterWindow();
	
	m_Bt_Num0.LoadBitmaps(IDB_BT_NUMPAD0,IDB_BT_NUMPAD0_0); 
	m_Bt_Num0.SizeToContent();	
	m_Bt_Num1.LoadBitmaps(IDB_BT_NUMPAD1,IDB_BT_NUMPAD1_0); 
	m_Bt_Num1.SizeToContent();	
	m_Bt_Num2.LoadBitmaps(IDB_BT_NUMPAD2,IDB_BT_NUMPAD2_0); 
	m_Bt_Num2.SizeToContent();	
	m_Bt_Num3.LoadBitmaps(IDB_BT_NUMPAD3,IDB_BT_NUMPAD3_0); 
	m_Bt_Num3.SizeToContent();	
	m_Bt_Num4.LoadBitmaps(IDB_BT_NUMPAD4,IDB_BT_NUMPAD4_0); 
	m_Bt_Num4.SizeToContent();	
	m_Bt_Num5.LoadBitmaps(IDB_BT_NUMPAD5,IDB_BT_NUMPAD5_0); 
	m_Bt_Num5.SizeToContent();	
	m_Bt_Num6.LoadBitmaps(IDB_BT_NUMPAD6,IDB_BT_NUMPAD6_0); 
	m_Bt_Num6.SizeToContent();	
	m_Bt_Num7.LoadBitmaps(IDB_BT_NUMPAD7,IDB_BT_NUMPAD7_0); 
	m_Bt_Num7.SizeToContent();	
	m_Bt_Num8.LoadBitmaps(IDB_BT_NUMPAD8,IDB_BT_NUMPAD8_0); 
	m_Bt_Num8.SizeToContent();	
	m_Bt_Num9.LoadBitmaps(IDB_BT_NUMPAD9,IDB_BT_NUMPAD9_0); 
	m_Bt_Num9.SizeToContent();	
	m_Bt_Close.SizeToContent();	
	m_Bt_Reset.SizeToContent();	
	m_Bt_Ok.SizeToContent();	
}

BOOL CNumPad::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CNumPad::OnPaint()
{
	CPaintDC dc(this); // device context for painting

    CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


void CNumPad::OnBnClickedBtNum1()
{
	UpdateData(true);
	if(m_Value!="0")
		m_Value+="1";
	else
		m_Value="1";
	if(_wtoi(m_Value)>m_Max)
		m_Value.Format(_T("%d"), m_Max);
	GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(m_Value);
}


void CNumPad::OnBnClickedBtNum2()
{
	UpdateData(true);
	if(m_Value!="0")
		m_Value+="2";
	else
		m_Value="2";
	if(_wtoi(m_Value)>m_Max)
		m_Value.Format(_T("%d"), m_Max);
	GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(m_Value);	
}


void CNumPad::OnBnClickedBtNum3()
{
	UpdateData(true);
	if(m_Value!="0")
		m_Value+="3";
	else
		m_Value="3";
	if(_wtoi(m_Value)>m_Max)
		m_Value.Format(_T("%d"), m_Max);
	GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(m_Value);	
}


void CNumPad::OnBnClickedBtNum4()
{
	UpdateData(true);
	if(m_Value!="0")
		m_Value+="4";
	else
		m_Value="4";
	if(_wtoi(m_Value)>m_Max)
		m_Value.Format(_T("%d"), m_Max);
	GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(m_Value);	
}


void CNumPad::OnBnClickedBtNum5()
{
	UpdateData(true);
	if(m_Value!="0")
		m_Value+="5";
	else
		m_Value="5";
	if(_wtoi(m_Value)>m_Max)
		m_Value.Format(_T("%d"), m_Max);
	GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(m_Value);	
}


void CNumPad::OnBnClickedBtNum6()
{
	UpdateData(true);
	if(m_Value!="0")
		m_Value+="6";
	else
		m_Value="6";
	if(_wtoi(m_Value)>m_Max)
		m_Value.Format(_T("%d"), m_Max);
	GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(m_Value);	
}


void CNumPad::OnBnClickedBtNum7()
{
	UpdateData(true);
	if(m_Value!="0")
		m_Value+="7";
	else
		m_Value="7";
	if(_wtoi(m_Value)>m_Max)
		m_Value.Format(_T("%d"), m_Max);
	GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(m_Value);	
}


void CNumPad::OnBnClickedBtNum8()
{
	UpdateData(true);
	if(m_Value!="0")
		m_Value+="8";
	else
		m_Value="8";
	if(_wtoi(m_Value)>m_Max)
		m_Value.Format(_T("%d"), m_Max);
	GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(m_Value);	
}


void CNumPad::OnBnClickedBtNum9()
{
	UpdateData(true);
	if(m_Value!="0")
		m_Value+="9";
	else
		m_Value="9";
	if(_wtoi(m_Value)>m_Max)
		m_Value.Format(_T("%d"), m_Max);
	GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(m_Value);	
}


void CNumPad::OnBnClickedBtNum0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	if(m_Value!="0")
		m_Value+="0";
	else
		m_Value="0";
	if(_wtoi(m_Value)>m_Max)
		m_Value.Format(_T("%d"), m_Max);
	GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(m_Value);	
}


void CNumPad::OnBnClickedBtReset()
{
	m_Value="";
	GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(m_Value);
}
