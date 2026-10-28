// KeyboardDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "KeyboardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyboardDlg dialog
#define TAB 0x02
#define CAPSLOCK 0x03
#define SHIFT 0x04
#define CTRL 0x05
#define BSPACE 0x06
#define ENTER 0x07
#define ALT 0x08
#define WINDOW 0x9
#define LEFT 0x0A
#define RIGHT 0x0B
#define UP 0x0C
#define DOWN 0x0D
#define ESC 0x0E
#define PUP 0x0F
#define PDN 0x10
#define HOME 0x11
#define END 0x12
#define INS 0x13
#define DEL 0x14
#define WWW 0x15
 
CKeyboardDlg::CKeyboardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyboardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyboardDlg)
	m_TestName = _T("");
	//}}AFX_DATA_INIT
	m_NationalCode=0;
	m_Numchar=0;
	b_IsType=false;
	b_Hanglepress=false;
	b_Shiftpress=false;
	b_Capitalpress=false;
	b_Ctrlpress=false;
	b_Altpress=false;
	m_Skinflag = false;
	m_pTargetWnd = NULL;
	for(INT_PTR i=0; i<1000; i++)
		m_Char[i]=0;
}


void CKeyboardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyboardDlg)
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Text(pDX, IDC_TESTNAME, m_TestName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyboardDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyboardDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+10, OnHookMsg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
//	ON_EN_CHANGE(IDC_TESTNAME, &CKeyboardDlg::OnChangeTestname)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyboardDlg message handlers

BOOL CKeyboardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	testtest = 1234;

	int m_currenthan = false;
	int state = 0;
	DWORD ks;

	if (GetKeyState(VK_HANGUL))
	{
		m_Skinflag = true;
	}
	else
	{
		m_Skinflag = false;
	}


	LoadSkin();
	LoadButton();

	// TODO: Add extra initialization here
	HookInit(this->m_hWnd);
	HookOn();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CKeyboardDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDialog::OnPaint();
}

BOOL CKeyboardDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//	HWND wnd = ::GetForegroundWindow(); 

	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 

//	if(!(pMsg->hwnd == m_btOk.m_hWnd || pMsg->hwnd == m_btCancel.m_hWnd))
//		GetDlgItem(IDC_TESTNAME)->SetFocus();

	return CDialog::PreTranslateMessage(pMsg);
}


void CKeyboardDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	if(GetKeyState(VK_SHIFT) & 0xF000) 
	{
		keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0);;
	}
	if(GetKeyState(VK_CONTROL) & 0xF000) 
	{
		keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
	}
	if(GetKeyState(VK_MENU) & 0xF000) 
	{
		keybd_event(VK_MENU,0,KEYEVENTF_KEYUP,0);
	}
	if (GetKeyState(VK_HANGUL) & 0xF000)
	{
		keybd_event(VK_HANGUL, 0, KEYEVENTF_KEYUP, 0);
	}
	HookOff();
}

void CKeyboardDlg::AddKey(
	char pNormal,
	char pShifted,
	INT_PTR pWidth)
{
	KEYDEF * key = new KEYDEF;
	key->cNormal = pNormal;
	key->cShifted = pShifted;
	key->cWidth = pWidth;
	cKeys.AddTail(key);
}

void CKeyboardDlg::DrawKey(CDC * dc, CRect & rc, KEYDEF * key, BOOL cHilight)
{

}

void CKeyboardDlg::DrawKeyboard()
{

}

INT_PTR CKeyboardDlg::DescribeKeyState() 
{
	INT_PTR state = 0;
	short ks;
	ks = GetKeyState(VK_CAPITAL);
	if(ks & 0x000F) state += 0x01;
	ks = GetKeyState(VK_SHIFT);
	if(ks & 0xF000) state += 0x02;
	ks = GetKeyState(VK_CONTROL);
	if(ks & 0xF000) state += 0x04;
	return state;
}

void CKeyboardDlg::SendKey(KEYDEF * keydef)
{

}

void CKeyboardDlg::LoadSkin()
{
	UINT unDlgUp;
	if (m_Skinflag == false)
		unDlgUp = IDB_BGKEYBOARD_ENG;
	else
		unDlgUp = IDB_BGKEYBOARD;

	UINT unDlgDN = IDB_BGKEYBOARD_1;
	UINT unDlgFS = IDB_BGKEYBOARD_1;
	UINT unDlgMASK = IDB_BGKEYBOARDMASK;

	m_bmpDlgUp.DeleteObject();
	m_bmpDlgDn.DeleteObject();
	m_bmpDlgFs.DeleteObject();	

	m_bmpDlgUp.LoadBitmap(unDlgUp);
	m_bmpDlgDn.LoadBitmap(unDlgDN);
	m_bmpDlgFs.LoadBitmap(unDlgFS);

	BITMAP bmp;
	m_bmpDlgUp.GetBitmap(&bmp);

	SetWindowPos( NULL, 0, 32, bmp.bmWidth, bmp.bmHeight, SWP_NOZORDER );

	Invalidate();
}

void CKeyboardDlg::LoadButton()
{
	if(m_NationalCode==1)
	{
		m_btOk.LoadBitmaps(IDB_BTOKENG,IDB_BTOKENG_1); 
		m_btOk.SizeToContent();
		m_btCancel.LoadBitmaps(IDB_BTCANCELENG,IDB_BTCANCELENG_1); 
		m_btCancel.SizeToContent();
	}
	else if(m_NationalCode==2)
	{
		m_btOk.LoadBitmaps(IDB_BTOKCHI,IDB_BTOKCHI_1); 
		m_btOk.SizeToContent();
		m_btCancel.LoadBitmaps(IDB_BTCANCELCHI,IDB_BTCANCELCHI_1); 
		m_btCancel.SizeToContent();
	}
	else
	{
		m_btOk.LoadBitmaps(IDB_BTOK,IDB_BTOK_1); 
		m_btOk.SizeToContent();
		m_btCancel.LoadBitmaps(IDB_BTCANCEL,IDB_BTCANCEL_1); 
		m_btCancel.SizeToContent();
	}

	if(!IsWindow(m_bt11.m_hWnd))
		m_bt11.SubclassDlgItem(IDC_BT11, this);
	if(!IsWindow(m_bt1.m_hWnd))
		m_bt1.SubclassDlgItem(IDC_BT1, this);
	if(!IsWindow(m_bt2.m_hWnd))
		m_bt2.SubclassDlgItem(IDC_BT2, this);
	if(!IsWindow(m_bt3.m_hWnd))
		m_bt3.SubclassDlgItem(IDC_BT3, this);
	if(!IsWindow(m_bt4.m_hWnd))
		m_bt4.SubclassDlgItem(IDC_BT4, this);
	if(!IsWindow(m_bt5.m_hWnd))
		m_bt5.SubclassDlgItem(IDC_BT5, this);
	if(!IsWindow(m_bt6.m_hWnd))
		m_bt6.SubclassDlgItem(IDC_BT6, this);
	if(!IsWindow(m_bt7.m_hWnd))
		m_bt7.SubclassDlgItem(IDC_BT7, this);
	if(!IsWindow(m_bt8.m_hWnd))
		m_bt8.SubclassDlgItem(IDC_BT8, this);
	if(!IsWindow(m_bt9.m_hWnd))
		m_bt9.SubclassDlgItem(IDC_BT9, this);
	if(!IsWindow(m_bt0.m_hWnd))
		m_bt0.SubclassDlgItem(IDC_BT0, this);
	if(!IsWindow(m_bt12.m_hWnd))
		m_bt12.SubclassDlgItem(IDC_BT12, this);
	if(!IsWindow(m_bt13.m_hWnd))
		m_bt13.SubclassDlgItem(IDC_BT13, this);
	if(!IsWindow(m_bt14.m_hWnd))
		m_bt14.SubclassDlgItem(IDC_BT14, this);


	m_bt11.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt11.SizeToContent(13, 112, 71, 71, TRUE );
	m_bt1.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt1.SizeToContent(84, 112, 71, 71, TRUE );
	m_bt2.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt2.SizeToContent(155, 112, 71, 71, TRUE );
	m_bt3.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt3.SizeToContent(226, 112, 71, 71, TRUE );
	m_bt4.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt4.SizeToContent(297, 112, 71, 71, TRUE );
	m_bt5.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt5.SizeToContent(368, 112, 71, 71, TRUE );
	m_bt6.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt6.SizeToContent(439, 112, 71, 71, TRUE );
	m_bt7.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt7.SizeToContent(510, 112, 71, 71, TRUE );
	m_bt8.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt8.SizeToContent(581, 112, 71, 71, TRUE );
	m_bt9.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt9.SizeToContent(652, 112, 71, 71, TRUE );
	m_bt0.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt0.SizeToContent(723, 112, 71, 71, TRUE );
	m_bt12.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt12.SizeToContent(794, 112, 71, 71, TRUE );
	m_bt13.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt13.SizeToContent(865, 112, 71, 71, TRUE );
	m_bt14.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt14.SizeToContent(936, 112, 71, 71, TRUE );

	if(!IsWindow(m_bt21.m_hWnd))
		m_bt21.SubclassDlgItem(IDC_BT21, this);
	if(!IsWindow(m_btq.m_hWnd))
		m_btq.SubclassDlgItem(IDC_BTQ, this);
	if(!IsWindow(m_btw.m_hWnd))
		m_btw.SubclassDlgItem(IDC_BTW, this);
	if(!IsWindow(m_bte.m_hWnd))
		m_bte.SubclassDlgItem(IDC_BTE, this);
	if(!IsWindow(m_btr.m_hWnd))
		m_btr.SubclassDlgItem(IDC_BTR, this);
	if(!IsWindow(m_btt.m_hWnd))
		m_btt.SubclassDlgItem(IDC_BTT, this);
	if(!IsWindow(m_bty.m_hWnd))
		m_bty.SubclassDlgItem(IDC_BTY, this);
	if(!IsWindow(m_btu.m_hWnd))
		m_btu.SubclassDlgItem(IDC_BTU, this);
	if(!IsWindow(m_bti.m_hWnd))
		m_bti.SubclassDlgItem(IDC_BTI, this);
	if(!IsWindow(m_bto.m_hWnd))
		m_bto.SubclassDlgItem(IDC_BTO, this);
	if(!IsWindow(m_btp.m_hWnd))
		m_btp.SubclassDlgItem(IDC_BTP, this);
	if(!IsWindow(m_bt22.m_hWnd))
		m_bt22.SubclassDlgItem(IDC_BT22, this);
	if(!IsWindow(m_bt23.m_hWnd))
		m_bt23.SubclassDlgItem(IDC_BT23, this);
	if(!IsWindow(m_bt24.m_hWnd))
		m_bt24.SubclassDlgItem(IDC_BT24, this);

	m_bt21.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt21.SizeToContent(13, 198, 71, 71, TRUE );
	m_btq.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btq.SizeToContent(84, 198, 71, 71, TRUE );
	m_btw.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btw.SizeToContent(155, 198, 71, 71, TRUE );
	m_bte.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bte.SizeToContent(226, 198, 71, 71, TRUE );
	m_btr.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btr.SizeToContent(297, 198, 71, 71, TRUE );
	m_btt.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btt.SizeToContent(368, 198, 71, 71, TRUE );
	m_bty.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bty.SizeToContent(439, 198, 71, 71, TRUE );
	m_btu.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btu.SizeToContent(510, 198, 71, 71, TRUE );
	m_bti.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bti.SizeToContent(581, 198, 71, 71, TRUE );
	m_bto.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bto.SizeToContent(652, 198, 71, 71, TRUE );
	m_btp.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btp.SizeToContent(723, 198, 71, 71, TRUE );
	m_bt22.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt22.SizeToContent(794, 198, 71, 71, TRUE );
	m_bt23.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt23.SizeToContent(865, 198, 71, 71, TRUE );
	m_bt24.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt24.SizeToContent(936, 198, 71, 71, TRUE );

	if(!IsWindow(m_bt31.m_hWnd))
		m_bt31.SubclassDlgItem(IDC_BT31, this);
	if(!IsWindow(m_bta.m_hWnd))
		m_bta.SubclassDlgItem(IDC_BTA, this);
	if(!IsWindow(m_bts.m_hWnd))
		m_bts.SubclassDlgItem(IDC_BTS, this);
	if(!IsWindow(m_btd.m_hWnd))
		m_btd.SubclassDlgItem(IDC_BTD, this);
	if(!IsWindow(m_btf.m_hWnd))
		m_btf.SubclassDlgItem(IDC_BTF, this);
	if(!IsWindow(m_btg.m_hWnd))
		m_btg.SubclassDlgItem(IDC_BTG, this);
	if(!IsWindow(m_bth.m_hWnd))
		m_bth.SubclassDlgItem(IDC_BTH, this);
	if(!IsWindow(m_btj.m_hWnd))
		m_btj.SubclassDlgItem(IDC_BTJ, this);
	if(!IsWindow(m_btk.m_hWnd))
		m_btk.SubclassDlgItem(IDC_BTK, this);
	if(!IsWindow(m_btl.m_hWnd))
		m_btl.SubclassDlgItem(IDC_BTL, this);
	if(!IsWindow(m_bt32.m_hWnd))
		m_bt32.SubclassDlgItem(IDC_BT32, this);
	if(!IsWindow(m_bt33.m_hWnd))
		m_bt33.SubclassDlgItem(IDC_BT33, this);
	if(!IsWindow(m_bt34.m_hWnd))
		m_bt34.SubclassDlgItem(IDC_BT34, this);

	m_bt31.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt31.SizeToContent(13, 282, 71, 71, TRUE );
	m_bta.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bta.SizeToContent(84, 282, 71, 71, TRUE );
	m_bts.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bts.SizeToContent(155, 282, 71, 71, TRUE );
	m_btd.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btd.SizeToContent(226, 282, 71, 71, TRUE );
	m_btf.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btf.SizeToContent(297, 282, 71, 71, TRUE );
	m_btg.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btg.SizeToContent(368, 282, 71, 71, TRUE );
	m_bth.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bth.SizeToContent(439, 282, 71, 71, TRUE );
	m_btj.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btj.SizeToContent(510, 282, 71, 71, TRUE );
	m_btk.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btk.SizeToContent(581, 282, 71, 71, TRUE );
	m_btl.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btl.SizeToContent(652, 282, 71, 71, TRUE );
	m_bt32.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt32.SizeToContent(723, 282, 71, 71, TRUE );
	m_bt33.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt33.SizeToContent(794, 282, 71, 71, TRUE );
	m_bt34.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt34.SizeToContent(865, 282, 142, 71, TRUE );

	if(!IsWindow(m_bt41.m_hWnd))
		m_bt41.SubclassDlgItem(IDC_BT41, this);
	if(!IsWindow(m_btz.m_hWnd))
		m_btz.SubclassDlgItem(IDC_BTZ, this);
	if(!IsWindow(m_btx.m_hWnd))
		m_btx.SubclassDlgItem(IDC_BTX, this);
	if(!IsWindow(m_btc.m_hWnd))
		m_btc.SubclassDlgItem(IDC_BTC, this);
	if(!IsWindow(m_btv.m_hWnd))
		m_btv.SubclassDlgItem(IDC_BTV, this);
	if(!IsWindow(m_btb.m_hWnd))
		m_btb.SubclassDlgItem(IDC_BTB, this);
	if(!IsWindow(m_btn.m_hWnd))
		m_btn.SubclassDlgItem(IDC_BTN, this);
	if(!IsWindow(m_btm.m_hWnd))
		m_btm.SubclassDlgItem(IDC_BTM, this);
	if(!IsWindow(m_bt42.m_hWnd))
		m_bt42.SubclassDlgItem(IDC_BT42, this);
	if(!IsWindow(m_bt43.m_hWnd))
		m_bt43.SubclassDlgItem(IDC_BT43, this);
	if(!IsWindow(m_bt44.m_hWnd))
		m_bt44.SubclassDlgItem(IDC_BT44, this);
	if(!IsWindow(m_bt45.m_hWnd))
		m_bt45.SubclassDlgItem(IDC_BT45, this);


	m_bt41.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt41.SizeToContent(13, 369, 142, 71, TRUE );
	m_btz.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btz.SizeToContent(155, 369, 71, 71, TRUE );
	m_btx.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btx.SizeToContent(226, 369, 71, 71, TRUE );
	m_btc.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btc.SizeToContent(297, 369, 71, 71, TRUE );
	m_btv.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btv.SizeToContent(368, 369, 71, 71, TRUE );
	m_btb.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btb.SizeToContent(439, 369, 71, 71, TRUE );
	m_btn.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btn.SizeToContent(510, 369, 71, 71, TRUE );
	m_btm.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_btm.SizeToContent(581, 369, 71, 71, TRUE );
	m_bt42.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt42.SizeToContent(652, 369, 71, 71, TRUE );
	m_bt43.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt43.SizeToContent(723, 369, 71, 71, TRUE );
	m_bt44.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt44.SizeToContent(794, 369, 71, 71, TRUE );
	m_bt45.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt45.SizeToContent(936, 369, 71, 71, TRUE );

	if(!IsWindow(m_bt51.m_hWnd))
		m_bt51.SubclassDlgItem(IDC_BT51, this);
	if(!IsWindow(m_bt52.m_hWnd))
		m_bt52.SubclassDlgItem(IDC_BT52, this);
	if(!IsWindow(m_bt53.m_hWnd))
		m_bt53.SubclassDlgItem(IDC_BT53, this);
	if(!IsWindow(m_bt54.m_hWnd))
		m_bt54.SubclassDlgItem(IDC_BT54, this);
	if(!IsWindow(m_bt55.m_hWnd))
		m_bt55.SubclassDlgItem(IDC_BT55, this);
	if(!IsWindow(m_bt56.m_hWnd))
		m_bt56.SubclassDlgItem(IDC_BT56, this);

	m_bt51.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt51.SizeToContent(13, 452, 142, 71, TRUE );
	m_bt52.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt52.SizeToContent(155, 452, 142, 71, TRUE );
	m_bt53.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt53.SizeToContent(297, 452, 426, 71, TRUE );
	m_bt54.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt54.SizeToContent(723, 452, 142, 71, TRUE );
	m_bt55.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt55.SizeToContent(865, 452, 71, 71, TRUE );
	m_bt56.LoadBitmaps(&m_bmpDlgUp, &m_bmpDlgDn, &m_bmpDlgFs);
	m_bt56.SizeToContent(936, 452, 71, 71, TRUE );
}

BOOL CKeyboardDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
    GetClientRect(&rc);

    CDC MemDC; 
    MemDC.CreateCompatibleDC(pDC);

    CBitmap* pbmpOld = MemDC.SelectObject(&m_bmpDlgUp); 
    pDC->BitBlt(0, 0, rc.right, rc.bottom, &MemDC, 0, 0, SRCCOPY);

    MemDC.SelectObject( pbmpOld );
    ReleaseDC(pDC);

	return CDialog::OnEraseBkgnd(pDC);
}

LONG_PTR CKeyboardDlg::OnHookMsg(WPARAM wParam, LPARAM lParam)
{
	LPMOUSEHOOKSTRUCT MouseParam;
	MouseParam = (MOUSEHOOKSTRUCT *)lParam;
	CPoint point = MouseParam->pt;
	ScreenToClient(&point);

    if(point.y>=112 && point.y<=183)
	{
		if(point.x>=13 && point.x<=83)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('`')));
			m_Numchar++;
		}
		else if(point.x>=84 && point.x<=154)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('1')));
			m_Numchar++;
		}
		else if(point.x>=155 && point.x<=225)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('2')));
			m_Numchar++;
		}
		else if(point.x>=226 && point.x<=296)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('3')));
			m_Numchar++;
		}
		else if(point.x>=297 && point.x<=367)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('4')));
			m_Numchar++;
		}
		else if(point.x>=368 && point.x<=438)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('5')));
			m_Numchar++;
		}
		else if(point.x>=439 && point.x<=509)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('6')));
			m_Numchar++;
		}
		else if(point.x>=510 && point.x<=580)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('7')));
			m_Numchar++;
		}
		else if(point.x>=581 && point.x<=651)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('8')));
			m_Numchar++;
		}
		else if(point.x>=652 && point.x<=722)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('9')));
			m_Numchar++;
		}
		else if(point.x>=723 && point.x<=793)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('0')));
			m_Numchar++;
		}
		else if(point.x>=794 && point.x<=864)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('-')));
			m_Numchar++;
		}
		else if(point.x>=865 && point.x<=935)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('=')));
			m_Numchar++;
		}
		else if(point.x>=936 && point.x<=1006)
		{
			bool b_flag=true;
			INT_PTR last=m_Numchar;
			while(b_flag)
			{
				m_Numchar--;
				if(m_Char[m_Numchar]==VK_HANGUL || m_Char[m_Numchar]==VK_SHIFT || m_Char[m_Numchar]==VK_CAPITAL)
				{	
					
				}
				else
				{
					b_flag=false;
				}
				if(m_Numchar<0)
				{
					m_Numchar=0;
					b_flag=false;
				}
			}
			if(last-m_Numchar>1)
			{
				for(int a=m_Numchar; a<last; a++)
				{
					m_Char[a]=m_Char[a+1];
				}
				m_Numchar=last-1;
			}
			
		}
	}

	else if(point.y>=198 && point.y<=269)
	{
		if(point.x>=84 && point.x<=154)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('q')));
			m_Numchar++;
	//		vk = LOBYTE(VkKeyScan(_T('q')));
	//		keybd_event(vk,0,0,0);
	//		keybd_event(vk,0,KEYEVENTF_KEYUP,0);
		}
		else if(point.x>=155 && point.x<=225)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('w')));
			m_Numchar++;
		}
		else if(point.x>=226 && point.x<=296)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('e')));
			m_Numchar++;
		}
		else if(point.x>=297 && point.x<=367)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('r')));
			m_Numchar++;
		}
		else if(point.x>=368 && point.x<=438)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('t')));
			m_Numchar++;
		}
		else if(point.x>=439 && point.x<=509)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('y')));
			m_Numchar++;
		}
		else if(point.x>=510 && point.x<=580)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('u')));
			m_Numchar++;
		}
		else if(point.x>=581 && point.x<=651)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('i')));
			m_Numchar++;
		}
		else if(point.x>=652 && point.x<=722)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('o')));
			m_Numchar++;
		}
		else if(point.x>=723 && point.x<=793)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('p')));
			m_Numchar++;
		}
		else if(point.x>=794 && point.x<=864)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('[')));
			m_Numchar++;
		}
		else if(point.x>=865 && point.x<=935)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T(']')));
			m_Numchar++;
		}
		else if(point.x>=936 && point.x<=1006)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('\\')));
			m_Numchar++;
		}
		
	}

	else if(point.y>=282 && point.y<=353)
	{
		if(point.x>=84 && point.x<=154)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('a')));
			m_Numchar++;
		}
		else if(point.x>=155 && point.x<=225)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('s')));
			m_Numchar++;
		}
		else if(point.x>=226 && point.x<=296)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('d')));
			m_Numchar++;
		}
		else if(point.x>=297 && point.x<=367)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('f')));
			m_Numchar++;
		}
		else if(point.x>=368 && point.x<=438)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('g')));
			m_Numchar++;
		}
		else if(point.x>=439 && point.x<=509)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('h')));
			m_Numchar++;
		}
		else if(point.x>=510 && point.x<=580)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('j')));
			m_Numchar++;
		}
		else if(point.x>=581 && point.x<=651)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('k')));
			m_Numchar++;
		}
		else if(point.x>=652 && point.x<=722)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('l')));
			m_Numchar++;
		}
		else if(point.x>=723 && point.x<=793)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T(';')));
			m_Numchar++;
		}
		else if(point.x>=794 && point.x<=864)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('\'')));
			m_Numchar++;
		}
	}

	else if(point.y>=369 && point.y<=440)
	{
		if(point.x>=155 && point.x<=225)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('z')));
			m_Numchar++;
		}
		else if(point.x>=226 && point.x<=296)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('x')));
			m_Numchar++;
		}
		else if(point.x>=297 && point.x<=367)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('c')));
			m_Numchar++;
		}
		else if(point.x>=368 && point.x<=438)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('v')));
			m_Numchar++;
		}
		else if(point.x>=439 && point.x<=509)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('b')));
			m_Numchar++;
		}
		else if(point.x>=510 && point.x<=580)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('n')));
			m_Numchar++;
		}
		else if(point.x>=581 && point.x<=651)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('m')));
			m_Numchar++;
		}
		else if(point.x>=652 && point.x<=722)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T(',')));
			m_Numchar++;
		}
		else if(point.x>=723 && point.x<=793)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('.')));
			m_Numchar++;
		}
		else if(point.x>=794 && point.x<=864)
		{
			m_Char[m_Numchar]=LOBYTE(VkKeyScan(_T('/')));
			m_Numchar++;
		}
		
	}

	if(point.y>=198 && point.y<=269)
	{
		if(point.x>=13 && point.x<=83)
		{
	/*		vk = VkKeyScan(0x09) & 0xFF;
			keybd_event(vk,0,0,0);
	*/	}
	}

	else if(point.y>=282 && point.y<=353)
	{
		if(point.x>=13 && point.x<=83)
		{
			m_Char[m_Numchar]= VK_CAPITAL;
			m_Numchar++;
			

		}
		else if(point.x>=865 && point.x<=1005)
		{
			m_Char[m_Numchar]= VkKeyScan(0x0D) & 0xFF;
			m_Numchar++;
			//keybd_event(vk,0,0,0);
		}
	}	

	else if(point.y>=369 && point.y<=440)
	{
		if((point.x>=13 && point.x<=154) || (point.x>=865 && point.x<=1005))
		{
			m_Char[m_Numchar]= VK_SHIFT;
			m_Numchar++;
		/*	vk = VK_SHIFT;
			if(GetKeyState(vk) & 0xF000) 
			{
				AfxMessageBox(_T("쉬프트 해제"));
				keybd_event(vk,0,KEYEVENTF_KEYUP,0);;
			}
			else
			{
				AfxMessageBox(_T("쉬프트 누름"));
				keybd_event(vk,0,0,0);
			}
		*/}
	}	

	else if(point.y>=452 && point.y<=523)
	{
		if(point.x>=13 && point.x<=154)
		{
		/*	vk = VK_CONTROL;
			if(GetKeyState(vk) & 0xF000) 
			{
			
				keybd_event(vk,0,KEYEVENTF_KEYUP,0);
			}
			else
			{
				keybd_event(vk,0,0,0);
			}
		*/}
		else if(point.x>=155 && point.x<=296)
		{
		/*	vk = VK_MENU;
			if(GetKeyState(vk) & 0xF000) 
			{
				keybd_event(vk,0,KEYEVENTF_KEYUP,0);
			}
			else
			{
				keybd_event(vk,0,0,0);
			}
		*/}
		else if(point.x>=297 && point.x<=722)
		{
			m_Char[m_Numchar]= LOBYTE(VkKeyScan(' '));
			m_Numchar++;
		}
		else if(point.x>=723 && point.x<=864)
		{
			m_Char[m_Numchar]= VK_HANGUL;
			m_Numchar++;
			if (m_Skinflag)
				m_Skinflag = false;
			else
				m_Skinflag = true;
			LoadSkin();
		//	b_Hangle[m_Numchar] = true;// VK_HANGUL;

		//	m_Numchar++;
		/*	if(GetKeyState(vk) & 0xF000) 
			{
				keybd_event(vk,0,KEYEVENTF_KEYUP,0);
			}
			else
			{
				keybd_event(vk,0,0,0);
			}
			keybd_event(vk,0,0,0);
		*/}
		else if(point.x>=865 && point.x<=935)
		{
			m_Char[m_Numchar]= VK_LEFT;
			m_Numchar++;
		}
		else if(point.x>=936 && point.x<=1006)
		{
			m_Char[m_Numchar]= VK_RIGHT;
			m_Numchar++;
		}
	}	
	OnChangeTestname();
	
	return 0;
}


void CKeyboardDlg::OnOK() 
{
	if (m_pTargetWnd && ::IsWindow(m_pTargetWnd->m_hWnd))
	{
		CString temp;
		m_pTargetWnd->GetWindowText(temp); // 부모의 실제 편집창에 최종 입력된 텍스트
		m_TestName = temp;
	}
	else
	{
		// fallback: 혹시라도 자기 에디트에 뭐가 들어있으면 거기서라도 읽기
		CString temp;
		GetDlgItemText(IDC_TESTNAME, temp);
		m_TestName = temp;
	}
	// TODO: Add extra validation here
	CDialog::OnOK();
}

void CKeyboardDlg::OnCancel() 
{
	
	// TODO: Add extra cleanup here	
	CDialog::OnCancel();
}


void CKeyboardDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonDown(nFlags, point);
}

void CKeyboardDlg::ClickButton(CPoint point)
{	
}

void CKeyboardDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonUp(nFlags, point);
}


void CKeyboardDlg::OnChangeTestname()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if(b_IsType)
		return;
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	SetDlgItemTextW(IDC_TESTNAME, _T(""));
	//BYTE vk;
	b_IsType=true;
/*
	if(GetKeyState(VK_HANGUL) & 0xF000) 
	{
//		keybd_event(VK_HANGUL,0,KEYEVENTF_KEYUP,0);
		keybd_event(VK_HANGUL,0,0,0);
	}
*/
//	keybd_event(VK_HANGUL,0,0,0);
	//keybd_event(VK_HANGUL,0,0,0);
	if(b_Hanglepress)
	{
		//AfxMessageBox(_T("현재 한글임 => 영문으로 변경"));
		keybd_event(VK_HANGUL,0,KEYEVENTF_KEYUP,0);
		keybd_event(VK_HANGUL,0,0,0);
		b_Hanglepress=false;
	}
	if(b_Shiftpress)
	{
		keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0);;
		b_Shiftpress=false;
	}
	if(b_Capitalpress)
	{
		keybd_event(VK_CAPITAL,0,KEYEVENTF_KEYUP,0);;
		keybd_event(VK_CAPITAL,0,0,0);
		b_Capitalpress=false;
	}

	b_Ctrlpress=false;
	b_Altpress=false;
	
	for(INT_PTR i=0; i<m_Numchar; i++)
	{
		//vk = LOBYTE(VkKeyScan(m_Char[i]));
		if(m_Char[i]==VK_HANGUL)
		{
			if(GetKeyState(VK_HANGUL) & 0xF000) 
			{
				keybd_event(VK_HANGUL,0,KEYEVENTF_KEYUP,0);
			}
			else
			{
				keybd_event(VK_HANGUL,0,0,0);
			}
			keybd_event(VK_HANGUL,0,0,0);
			if(!b_Hanglepress)
				b_Hanglepress=true;
			else
				b_Hanglepress=false;
		}
		else if(m_Char[i]==VK_SHIFT)
		{
			if(!b_Shiftpress)
			{
				keybd_event(VK_SHIFT,0,0,0);
				b_Shiftpress=true;
			}
			else
			{
				keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0);;
				b_Shiftpress=false;
			}
		}
		else if(m_Char[i]==VK_CAPITAL)
		{
			keybd_event(VK_CAPITAL,0,KEYEVENTF_KEYUP,0);;
			keybd_event(VK_CAPITAL,0,0,0);
			if(!b_Capitalpress)
			{
				b_Capitalpress=true;
			}
			else
			{						
				b_Capitalpress=false;
			}
			
		}
		else
		{
			keybd_event(m_Char[i],0,0,0);
			if(m_Char[i]!=VK_RIGHT || m_Char[i]==VK_LEFT)
				keybd_event(m_Char[i],0,KEYEVENTF_KEYUP,0);
		}
	}
	b_IsType=false;
}
