// ViewerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ViewerDlg.h"
#include "afxdialogex.h"

#include <iostream>

#include <ctime>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//#include "../CXImage/ximage.h"
#include "Message.h"
#include "TypeConversion.h"
#include "Media.h"

//static CString DirPath = _T("D:\\nse-temp\\image\\"); 
// CViewerDlg 대화 상자입니다.
namespace fs = std::filesystem;

IMPLEMENT_DYNAMIC(CViewerDlg, CDialogEx)

CViewerDlg::CViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CViewerDlg::IDD, pParent)
	, m_NameCount(0)
	, m_Fullname(_T(""))
	, m_bVideo(false)
{
	m_ConditionInfo = _T("");
	b_AllChecked = FALSE;
	m_SelectedTabNum = 0;	
	g_Height=640;
	g_Width=640;
	m_ZoomRate=10;
	b_IsMakeArray=false;
	b_AllCheck=false;
	b_IsDragging=false;
	m_NationalCode=0;
	m_Date = COleDateTime::GetCurrentTime();
	m_ShowPosition = 0;
	m_ProductName = _T("");

	//20190819
	b_IsStart=false;
}

CViewerDlg::~CViewerDlg()
{
}

void CViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ALLCHECK, m_BtAllCheck);
	DDX_Control(pDX, IDC_BT_ZOOMIN, m_BtZoomIn);
	DDX_Control(pDX, IDC_BT_ZOOMOUT, m_BtZoomOut);
	DDX_Control(pDX, IDC_COPYUSB, m_BtCopy);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER, m_Date);
	DDX_Control(pDX, IDC_FILELIST, m_CtrlList);
	DDX_Control(pDX, IDC_PIC, m_Pic);
	DDX_Control(pDX, IDC_RADIOTAP, m_BtTabNg);
	DDX_Control(pDX, IDC_RADIOTAP1, m_BtTabOmit);
	DDX_Control(pDX, IDCANCEL, m_BtExit);
	DDX_Radio(pDX, IDC_SHOWPOSITION, m_ShowPosition);
	DDX_Text(pDX, IDC_PRODUCTNAME, m_ProductName);	
	DDX_Control(pDX, IDC_CONDITIONINFO6, m_EditCondInfo6);
	DDX_Control(pDX, IDC_CONDITIONINFO5, m_EditCondInfo5);
	DDX_Control(pDX, IDC_CONDITIONINFO4, m_EditCondInfo4);
	DDX_Control(pDX, IDC_CONDITIONINFO3, m_EditCondInfo3);
	DDX_Control(pDX, IDC_CONDITIONINFO2, m_EditCondInfo2);
	DDX_Control(pDX, IDC_CONDITIONINFO, m_EditCondInfo);
	DDX_Control(pDX, IDC_COPYREJECT, m_Reject_Save);

	DDX_Control(pDX, IDC_UP, m_ListUp); //20190611 Viewer 개선
	DDX_Control(pDX, IDC_DOWN, m_ListDown); //20190611 Viewer 개선

	//20200411
	DDX_Control(pDX, IDC_RADIOTAP2, m_BtTabTest);
	DDX_Control(pDX, IDC_PLAY, m_PlayReject);
}


BEGIN_MESSAGE_MAP(CViewerDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER, &CViewerDlg::OnDatetimechangeDatetimepicker)
	ON_BN_CLICKED(IDC_SHOWPOSITION, &CViewerDlg::OnClickedShowposition)
	ON_COMMAND(IDC_SHOWPOSITION1, &CViewerDlg::OnShowposition1)
	ON_BN_CLICKED(IDC_COPYUSB, &CViewerDlg::OnBnClickedCopyusb)
	ON_BN_CLICKED(IDC_ALLCHECK, &CViewerDlg::OnClickedAllcheck)
	ON_BN_CLICKED(IDC_BT_ZOOMIN, &CViewerDlg::OnBnClickedBtZoomin)
	ON_BN_CLICKED(IDC_BT_ZOOMOUT, &CViewerDlg::OnBnClickedBtZoomout)
	ON_BN_CLICKED(IDC_RADIOTAP, &CViewerDlg::OnClickedRadiotap)
	ON_COMMAND(IDC_RADIOTAP1, &CViewerDlg::OnRadiotap1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILELIST, &CViewerDlg::OnItemchangedFilelist)
	ON_BN_CLICKED(IDC_COPYREJECT, &CViewerDlg::OnBnClickedCopyreject)
	ON_BN_CLICKED(IDC_UP, &CViewerDlg::OnBnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, &CViewerDlg::OnBnClickedDown)
	//20200411
	ON_COMMAND(IDC_RADIOTAP2, &CViewerDlg::OnRadiotap2)
	ON_BN_CLICKED(IDC_PLAY, &CViewerDlg::OnBnClickedPlay)
END_MESSAGE_MAP()


// CViewerDlg 메시지 처리기입니다.
BOOL CViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowPos(NULL, 0, 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);	
	m_bkbrush2.CreateSolidBrush(RGB(215,223,233)); 
	pen1.CreatePen(PS_SOLID, 2, RGB(200, 0, 0));
	pen2.CreatePen(PS_SOLID, 2, RGB(255, 200, 0));
	pen3.CreatePen(PS_SOLID, 2, RGB(0, 200, 0));
	pen4.CreatePen(PS_SOLID, 2, RGB(0, 0, 200));
	pen5.CreatePen(PS_SOLID, 2, RGB(9, 232, 238));
	pen6.CreatePen(PS_SOLID, 2, RGB(200, 0, 200));
	pen10.CreatePen(PS_SOLID, 2, RGB(255, 192, 203));
	Brush.CreateStockObject(NULL_BRUSH);

	CFont timefont;
	timefont.CreatePointFont(110, _T("맑은 고딕")); 
	GetDlgItem(IDC_CONDITIONINFO)->SetFont(&timefont); 
	GetDlgItem(IDC_CONDITIONINFO2)->SetFont(&timefont); 
	GetDlgItem(IDC_CONDITIONINFO3)->SetFont(&timefont); 
	GetDlgItem(IDC_CONDITIONINFO4)->SetFont(&timefont); 
	GetDlgItem(IDC_CONDITIONINFO5)->SetFont(&timefont); 
	GetDlgItem(IDC_CONDITIONINFO6)->SetFont(&timefont); 

	m_Pic.GetWindowRect(&m_Picrect);
	ScreenToClient(m_Picrect);

	LoadBackground();
	InitList();
	SearchListData();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CViewerDlg::LoadBackground()
{
	BITMAP bmpInfo;
	
	if(m_NationalCode==1)
	{		
		SetDlgItemText(IDC_SHOWPOSITION, _T("Yes"));
		SetDlgItemText(IDC_SHOWPOSITION1, _T("No"));
		m_bmpTabNg.LoadBitmap(IDB_BTTABNG_ENG);
		m_bmpTabNg_1.LoadBitmap(IDB_BTTABNG_1_ENG);
		m_bmpTabOmit.LoadBitmap(IDB_BTTABOMIT_ENG);
		m_bmpTabOmit_1.LoadBitmap(IDB_BTTABOMIT_1_ENG);
		m_bmpMainBG.LoadBitmap(IDB_VIEWERBACKGROUND_ENG);
		m_BtExit.LoadBitmaps(IDB_BTCLOSE_ENG,IDB_BTCLOSE_1_ENG); 
		m_BtCopy.LoadBitmaps(IDB_BTCOPY_ENG, IDB_BTCOPY_1_ENG); 
		m_BtZoomIn.LoadBitmaps(IDB_BTZOOMINON_ENG, IDB_BTZOOMINOFF_ENG); 
		m_BtZoomOut.LoadBitmaps(IDB_BTZOOMOUTON_ENG, IDB_BTZOOMOUTOFF_ENG); 
		m_BmpAllCheck.LoadBitmap(IDB_BTALLCHECK_ENG);
		m_BmpAllCheck_1.LoadBitmap(IDB_BTALLCHECK_1_ENG);
		m_Reject_Save.LoadBitmaps(IDB_BT_REJECT_SAVE_ENG,IDB_BT_REJECT_SAVE_ENG_0); 
		//20200411
		m_bmpTabTest.LoadBitmap(IDB_BTTABTEST_ENG);
		m_bmpTabTest_1.LoadBitmap(IDB_BTTABTEST_1_ENG);

	}
	else if(m_NationalCode==2)
	{		
		SetDlgItemText(IDC_SHOWPOSITION, _T("表示"));
		SetDlgItemText(IDC_SHOWPOSITION1, _T("不表示"));
		m_bmpTabNg.LoadBitmap(IDB_BTTABNG_CHI);
		m_bmpTabNg_1.LoadBitmap(IDB_BTTABNG_1_CHI);
		m_bmpTabOmit.LoadBitmap(IDB_BTTABOMIT_CHI);
		m_bmpTabOmit_1.LoadBitmap(IDB_BTTABOMIT_1_CHI);
		m_bmpMainBG.LoadBitmap(IDB_VIEWERBACKGROUND_CHI);
		m_BtExit.LoadBitmaps(IDB_BTCLOSE_CHI,IDB_BTCLOSE_1_CHI); 
		m_BtCopy.LoadBitmaps(IDB_BTCOPY_CHI, IDB_BTCOPY_1_CHI); 
		m_BtZoomIn.LoadBitmaps(IDB_BTZOOMINON_CHI, IDB_BTZOOMINOFF_CHI); 
		m_BtZoomOut.LoadBitmaps(IDB_BTZOOMOUTON_CHI, IDB_BTZOOMOUTOFF_CHI); 
		m_BmpAllCheck.LoadBitmap(IDB_BTALLCHECK_CHI);
		m_BmpAllCheck_1.LoadBitmap(IDB_BTALLCHECK_1_CHI);
		m_Reject_Save.LoadBitmaps(IDB_BT_REJECT_SAVE_CHI,IDB_BT_REJECT_SAVE_CHI_0);
		//20200411
		m_bmpTabTest.LoadBitmap(IDB_BTTABTEST_CHI);
		m_bmpTabTest_1.LoadBitmap(IDB_BTTABTEST_1_CHI);
	}
	else if(m_NationalCode==0)
	{
		SetDlgItemText(IDC_SHOWPOSITION, _T("표시함"));
		SetDlgItemText(IDC_SHOWPOSITION1, _T("표시안함"));
		m_bmpTabNg.LoadBitmap(IDB_BTTABNG);
		m_bmpTabNg_1.LoadBitmap(IDB_BTTABNG_1);
		m_bmpTabOmit.LoadBitmap(IDB_BTTABOMIT);
		m_bmpTabOmit_1.LoadBitmap(IDB_BTTABOMIT_1);
		m_bmpMainBG.LoadBitmap(IDB_VIEWERBACKGROUND);
		m_BtExit.LoadBitmaps(IDB_BTCLOSE,IDB_BTCLOSE_1); 
		m_BtCopy.LoadBitmaps(IDB_BTCOPY, IDB_BTCOPY_1); 
		m_BtZoomIn.LoadBitmaps(IDB_BTZOOMINON, IDB_BTZOOMINOFF); 
		m_BtZoomOut.LoadBitmaps(IDB_BTZOOMOUTON, IDB_BTZOOMOUTOFF); 
		m_BmpAllCheck.LoadBitmap(IDB_BTALLCHECK);
		m_BmpAllCheck_1.LoadBitmap(IDB_BTALLCHECK_1);
		m_Reject_Save.LoadBitmaps(IDB_BT_REJECT_SAVE,IDB_BT_REJECT_SAVE_0);

		//20200411
		m_bmpTabTest.LoadBitmap(IDB_BTTABTEST);
		m_bmpTabTest_1.LoadBitmap(IDB_BTTABTEST_1);
	}
	else if(m_NationalCode==3)
	{
		SetDlgItemText(IDC_SHOWPOSITION, _T("да"));
		SetDlgItemText(IDC_SHOWPOSITION1, _T("нет"));
		m_bmpTabNg.LoadBitmap(IDB_BTTABNG_RUS);
		m_bmpTabNg_1.LoadBitmap(IDB_BTTABNG_1_RUS);
		m_bmpTabOmit.LoadBitmap(IDB_BTTABOMIT_RUS);
		m_bmpTabOmit_1.LoadBitmap(IDB_BTTABOMIT_1_RUS);
		m_bmpMainBG.LoadBitmap(IDB_VIEWERBACKGROUND_RUS);
		m_BtExit.LoadBitmaps(IDB_BTCLOSE_RUS,IDB_BTCLOSE_1_RUS); 
		m_BtCopy.LoadBitmaps(IDB_BTCOPY_RUS, IDB_BTCOPY_1_RUS); 
		m_BtZoomIn.LoadBitmaps(IDB_BTZOOMINON_RUS, IDB_BTZOOMINOFF_RUS); 
		m_BtZoomOut.LoadBitmaps(IDB_BTZOOMOUTON_RUS, IDB_BTZOOMOUTOFF_RUS); 
		m_BmpAllCheck.LoadBitmap(IDB_BTALLCHECK_RUS);
		m_BmpAllCheck_1.LoadBitmap(IDB_BTALLCHECK_1_RUS);
		m_Reject_Save.LoadBitmaps(IDB_BT_REJECT_SAVE_RUS,IDB_BT_REJECT_SAVE_RUS_0);

		//20200411
		m_bmpTabTest.LoadBitmap(IDB_BTTABTEST_RUS);
		m_bmpTabTest_1.LoadBitmap(IDB_BTTABTEST_1_RUS);
	}
	else if(m_NationalCode==4)
	{
		SetDlgItemText(IDC_SHOWPOSITION, _T("так"));
		SetDlgItemText(IDC_SHOWPOSITION1, _T("немає"));
		m_bmpTabNg.LoadBitmap(IDB_BTTABNG_UKR);
		m_bmpTabNg_1.LoadBitmap(IDB_BTTABNG_1_UKR);
		m_bmpTabOmit.LoadBitmap(IDB_BTTABOMIT_UKR);
		m_bmpTabOmit_1.LoadBitmap(IDB_BTTABOMIT_1_UKR);
		m_bmpMainBG.LoadBitmap(IDB_VIEWERBACKGROUND_UKR);
		m_BtExit.LoadBitmaps(IDB_BTCLOSE_UKR,IDB_BTCLOSE_1_UKR); 
		m_BtCopy.LoadBitmaps(IDB_BTCOPY_UKR, IDB_BTCOPY_1_UKR); 
		m_BtZoomIn.LoadBitmaps(IDB_BTZOOMINON_UKR, IDB_BTZOOMINOFF_UKR); 
		m_BtZoomOut.LoadBitmaps(IDB_BTZOOMOUTON_UKR, IDB_BTZOOMOUTOFF_UKR); 
		m_BmpAllCheck.LoadBitmap(IDB_BTALLCHECK_UKR);
		m_BmpAllCheck_1.LoadBitmap(IDB_BTALLCHECK_1_UKR);
		m_Reject_Save.LoadBitmaps(IDB_BT_REJECT_SAVE_UKR,IDB_BT_REJECT_SAVE_UKR_0);

		//20200411
		m_bmpTabTest.LoadBitmap(IDB_BTTABTEST_UKR);
		m_bmpTabTest_1.LoadBitmap(IDB_BTTABTEST_1_UKR);
	}
	else if(m_NationalCode==5)
	{		
		SetDlgItemText(IDC_SHOWPOSITION, _T("sí"));
		SetDlgItemText(IDC_SHOWPOSITION1, _T("No"));
		m_bmpTabNg.LoadBitmap(IDB_BTTABNG_SPN);
		m_bmpTabNg_1.LoadBitmap(IDB_BTTABNG_1_SPN);
		m_bmpTabOmit.LoadBitmap(IDB_BTTABOMIT_SPN);
		m_bmpTabOmit_1.LoadBitmap(IDB_BTTABOMIT_1_SPN);
		m_bmpMainBG.LoadBitmap(IDB_VIEWERBACKGROUND_SPN);
		m_BtExit.LoadBitmaps(IDB_BTCLOSE_SPN,IDB_BTCLOSE_1_SPN); 
		m_BtCopy.LoadBitmaps(IDB_BTCOPY_SPN, IDB_BTCOPY_1_SPN); 
		m_BtZoomIn.LoadBitmaps(IDB_BTZOOMINON_SPN, IDB_BTZOOMINOFF_SPN); 
		m_BtZoomOut.LoadBitmaps(IDB_BTZOOMOUTON_SPN, IDB_BTZOOMOUTOFF_SPN); 
		m_BmpAllCheck.LoadBitmap(IDB_BTALLCHECK_SPN);
		m_BmpAllCheck_1.LoadBitmap(IDB_BTALLCHECK_1_SPN);
		m_Reject_Save.LoadBitmaps(IDB_BT_REJECT_SAVE_SPN,IDB_BT_REJECT_SAVE_SPN_0); 
		//20200411
		m_bmpTabTest.LoadBitmap(IDB_BTTABTEST_SPN);
		m_bmpTabTest_1.LoadBitmap(IDB_BTTABTEST_1_SPN);
	}
	
	else if(m_NationalCode==6)
	{		
		SetDlgItemText(IDC_SHOWPOSITION, _T("tak"));
		SetDlgItemText(IDC_SHOWPOSITION1, _T("Nie"));
		m_bmpTabNg.LoadBitmap(IDB_BTTABNG_POL);
		m_bmpTabNg_1.LoadBitmap(IDB_BTTABNG_1_POL);
		m_bmpTabOmit.LoadBitmap(IDB_BTTABOMIT_POL);
		m_bmpTabOmit_1.LoadBitmap(IDB_BTTABOMIT_1_POL);
		m_bmpMainBG.LoadBitmap(IDB_VIEWERBACKGROUND_POL);
		m_BtExit.LoadBitmaps(IDB_BTCLOSE_POL,IDB_BTCLOSE_1_POL); 
		m_BtCopy.LoadBitmaps(IDB_BTCOPY_POL, IDB_BTCOPY_1_POL); 
		m_BtZoomIn.LoadBitmaps(IDB_BTZOOMINON_POL, IDB_BTZOOMINOFF_POL); 
		m_BtZoomOut.LoadBitmaps(IDB_BTZOOMOUTON_POL, IDB_BTZOOMOUTOFF_POL); 
		m_BmpAllCheck.LoadBitmap(IDB_BTALLCHECK_POL);
		m_BmpAllCheck_1.LoadBitmap(IDB_BTALLCHECK_1_POL);
		m_Reject_Save.LoadBitmaps(IDB_BT_REJECT_SAVE_POL,IDB_BT_REJECT_SAVE_POL_0); 
		//20200411
		m_bmpTabTest.LoadBitmap(IDB_BTTABTEST_POL);
		m_bmpTabTest_1.LoadBitmap(IDB_BTTABTEST_1_POL);
	}
	else if(m_NationalCode==7)
	{		
		SetDlgItemText(IDC_SHOWPOSITION, _T("開啟"));
		SetDlgItemText(IDC_SHOWPOSITION1, _T("關閉"));
		m_bmpTabNg.LoadBitmap(IDB_BTTABNG_TIW);
		m_bmpTabNg_1.LoadBitmap(IDB_BTTABNG_1_TIW);
		m_bmpTabOmit.LoadBitmap(IDB_BTTABOMIT_TIW);
		m_bmpTabOmit_1.LoadBitmap(IDB_BTTABOMIT_1_TIW);
		m_bmpMainBG.LoadBitmap(IDB_VIEWERBACKGROUND_TIW);
		m_BtExit.LoadBitmaps(IDB_BTCLOSE_TIW,IDB_BTCLOSE_1_TIW); 
		m_BtCopy.LoadBitmaps(IDB_BTCOPY_TIW, IDB_BTCOPY_1_TIW); 
		m_BtZoomIn.LoadBitmaps(IDB_BTZOOMINON_TIW, IDB_BTZOOMINOFF_TIW); 
		m_BtZoomOut.LoadBitmaps(IDB_BTZOOMOUTON_TIW, IDB_BTZOOMOUTOFF_TIW); 
		m_BmpAllCheck.LoadBitmap(IDB_BTALLCHECK_TIW);
		m_BmpAllCheck_1.LoadBitmap(IDB_BTALLCHECK_1_TIW);
		m_Reject_Save.LoadBitmaps(IDB_BT_REJECT_SAVE_TIW,IDB_BT_REJECT_SAVE_TIW_0); 
		//20200411
		m_bmpTabTest.LoadBitmap(IDB_BTTABTEST_TIW);
		m_bmpTabTest_1.LoadBitmap(IDB_BTTABTEST_1_TIW);
	}
	
	m_ListUp.LoadBitmaps(IDB_BTLISTUP,IDB_BT_LISTUP); //20190611 viewer 개선
	m_ListDown.LoadBitmaps(IDB_BTLISTDOWN,IDB_BTLISTDOWN_0); //20190611 viewer 개선

    m_bmpMainBG.GetBitmap(&bmpInfo); 
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 
    SetWindowPos(NULL, 0, 0, m_nMainWidth, m_nMainHeight, SWP_NOZORDER | SWP_NOMOVE); 

	m_BtExit.SizeToContent();
	m_BtCopy.SizeToContent();
	m_BtZoomIn.SizeToContent();
	m_BtZoomOut.SizeToContent();
	m_BtTabNg.SetBitmap(m_bmpTabNg);
	m_BtTabOmit.SetBitmap(m_bmpTabOmit_1);
	m_BtAllCheck.SetBitmap(m_BmpAllCheck);
	m_Reject_Save.SizeToContent();

	//20200411
	m_BtTabTest.SetBitmap(m_bmpTabTest_1);
	//20200601
	if(!m_bUseTestMode)
		GetDlgItem(IDC_RADIOTAP2)->ShowWindow(false);
	//20200326
	if(!m_bVideo)
		GetDlgItem(IDC_PLAY)->ShowWindow(false);
		
	
}

void CViewerDlg::InitList()
{
	CRect rect;
    m_CtrlList.GetClientRect(&rect);
	m_CtrlList.InsertColumn(0, _T("File Name"), LVCFMT_LEFT, rect.Width()-18);
	m_CtrlList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES );

	CTime time;
	time = CTime::GetCurrentTime();
	m_CurrentDate.Format(_T("%4d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());
}

BOOL CViewerDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		return false;

	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CViewerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	 if (nCtlColor == CTLCOLOR_STATIC) 
	{	
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH) Brush;
    }
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		if(pWnd->m_hWnd == m_EditCondInfo.m_hWnd || pWnd->m_hWnd == m_EditCondInfo2.m_hWnd || pWnd->m_hWnd == m_EditCondInfo3.m_hWnd 
			|| pWnd->m_hWnd == m_EditCondInfo4.m_hWnd || pWnd->m_hWnd == m_EditCondInfo5.m_hWnd || pWnd->m_hWnd == m_EditCondInfo6.m_hWnd)
		{
			pDC->SetBkColor(RGB(215,223,233));
			pDC->SetTextColor(RGB(0,0,0));
			return (HBRUSH) m_bkbrush2;
		}
		else
		{
			pDC->SetBkColor(RGB(255,255, 255));
			return CreateSolidBrush(RGB(255,255,255)); 
		}
	}
	return hbr;
}

BOOL CViewerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    CDC MemDC; 
    MemDC.CreateCompatibleDC(pDC);
    MemDC.SelectObject(&m_bmpMainBG); 
    pDC->BitBlt(0, 0,  m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
    MemDC.DeleteDC();
    ReleaseDC(pDC);

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(b_IsDragging && m_ZoomRate!=10)
	{
		if(m_Picrect.PtInRect(point)!=0)
		{	
			m_Mouse.x+=(m_FirstPoint.x-point.x);	
			//m_Mouse.y+=(m_FirstPoint.y-point.y);
			m_Mouse.y += (point.y - m_FirstPoint.y);

			if(m_Mouse.x<0)
				m_Mouse.x=0;
			else if(m_Mouse.x>g_Width)
				m_Mouse.x=g_Width;
			if(m_Mouse.y<0)
				m_Mouse.y=0;
			else if(m_Mouse.y>g_Height)
				m_Mouse.y=g_Height;
			
			DrawPicture();
			m_FirstPoint=point;
		}
		else
			b_IsDragging=false;
	}
	else
		b_IsDragging=false;

	//20190819
	//20190910
	if(m_Picrect.PtInRect(point)!=0)
	{
		if(b_IsStart && m_ZoomRate==10)
		{
			CDC *pDC1 = m_Pic.GetDC();
			CString text=_T("");
		
			int w=m_Picrect.right-m_Picrect.left;
			int h=m_Picrect.bottom-m_Picrect.top;
			int x, y;
			x = (point.y-m_Picrect.top)*g_Height/h;
			y = (point.x-m_Picrect.left)*g_Width/w;
			CString temp;
			if(y>=0 && y<g_Width && x>=0 && x<g_Height)
			{
				text.Format(_T("(%4d, %4d) %3d   "), g_Height-1-x, y, m_ImgBuffer[g_Height-1-x][y]);	
				pDC1->TextOutW(m_Picrect.left-25, h-20, text);
			}
			ReleaseDC(pDC1);
		}
		else
		{

		}
		
	}	
	CDialogEx::OnMouseMove(nFlags, point);
}


void CViewerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DrawPicture();
}

void CViewerDlg::SearchListData()
{
	int i = 0;
	m_CtrlList.DeleteAllItems();
	switch (m_SelectedTabNum)
	{
	case 1:
		m_StrDirectory.Format(_T("%s%s\\OMIT"), csDirPath, m_CurrentDate);
		break;
	case 2:
		m_StrDirectory.Format(_T("%s%s\\TEST"), csDirPath, m_CurrentDate);
		break;
	case 0:
	default:
		m_StrDirectory.Format(_T("%s%s\\NG"), csDirPath, m_CurrentDate);
		break;
	}

	m_NameCount = 0;

	std::string strPath = cstos(m_StrDirectory);
	fs::path p(strPath);

	if (!fs::exists(p))
	{

	}
	else
	{
		for (const fs::directory_entry& entry : fs::directory_iterator(p))
		{
			std::string strFilename = entry.path().filename().string();
			std::string strExt = strFilename.substr(strFilename.size() - 3, 3);
			if (strExt == "jpg" || strExt == "JPG")
			{
				CString csFilename = stocs(strFilename);
				//AfxMessageBox(csFilename);
				m_CtrlList.InsertItem(i, csFilename);
				m_strName.push_back(csFilename);
				i++;
				m_NameCount++;
			}
		}
	}

	b_AllCheck = false;
	b_AllChecked = false;
	m_BtAllCheck.SetBitmap(m_BmpAllCheck);

	m_ListNum = m_CtrlList.GetTopIndex();
	m_CtrlList.SetFocus();
	m_CtrlList.SetItemState(m_ListNum, LVIS_SELECTED, LVIS_SELECTED);

	if (m_bUseTestMode)
		SetDlgItemInt(IDC_EDIT_COUNTFILE, m_CtrlList.GetItemCount(), true);
	else
		GetDlgItem(IDC_EDIT_COUNTFILE)->ShowWindow(false);

	if (m_CtrlList.GetItemCount() > 0)
	{
		ChangeFile();
		DrawPicture();

		b_IsStart = true;
	}
}

void CViewerDlg::DrawPicture()
{
	/*
	CRect rect;
	HBITMAP hBmp;
	BITMAP bi;

	bool b_twice=false;

	CString csTempPath = m_StrDirectory + _T("\\") + m_StrFileName;
	m_Fullname = csTempPath;

	m_Fullname = m_StrDirectory + "\\" + m_StrFileName; //리젝트 동영상 추가 20170404 LIM
	CMedia dlg;
	dlg.m_Path = m_Fullname;
	g_path = m_Fullname;
	g_path.Replace(_T("jpg"),_T("avi"));

	if(g_path.Find(_T("NG")))
		g_path.Replace(_T("NG"), _T("VIDEO"));
		
	if(g_path.Find(_T("OMIT")))
		g_path.Replace(_T("OMIT"), _T("VIDEO"));


	std::string strPath = cstos(csTempPath);
	if (fs::exists(strPath))
	{
		std::string strExt = strPath.substr(strPath.size() - 3, 3);

		cv::Mat cvOrigin = cv::imread(strPath, 0);

		if (cvOrigin.empty())	return;

		cv::Mat cvCopyImgT(cvOrigin.rows, cvOrigin.cols / 2, cvOrigin.type());
		cv::Mat cvCopyImg;
		if (cvOrigin.cols >= cvOrigin.rows * 2)
		{
			cvCopyImgT = cvOrigin(cv::Rect(cvOrigin.cols / 2, 0, cvOrigin.cols / 2, cvOrigin.rows));
			cvCopyImg = cvCopyImgT.clone();
		}
		else
			cvCopyImg = cvOrigin.clone();

		int nTemp_h = cvCopyImg.rows;
		int nTemp_w = cvCopyImg.cols;

		if (!b_IsMakeArray)
		{
			g_Height = nTemp_h;
			g_Width = nTemp_w;
			NewArray();
			b_IsMakeArray = true;
		}
		else if (nTemp_h != g_Height || nTemp_w != g_Width)
		{
			ReleaseArray();
			g_Height = nTemp_h;
			g_Width = nTemp_w;
			NewArray();
		}

		// rectangle box draw
		cv::Mat cvCopyImgColor;
		cv::cvtColor(cvCopyImg, cvCopyImgColor, cv::COLOR_GRAY2BGR);

		//else
		//{
		CreateBitmapInfo(cvCopyImgColor.cols, cvCopyImgColor.rows, cvCopyImgColor.channels() * 8);
		//}

		CClientDC dc(GetDlgItem(IDC_PIC));
		GetDlgItem(IDC_PIC)->GetClientRect(&rect);

		CDC memDC;
		CBitmap* pOldBitmap, bitmap;
		memDC.CreateCompatibleDC(&dc);
		memDC.SetStretchBltMode(COLORONCOLOR);
		memDC.SelectStockObject(NULL_BRUSH);
		bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
		pOldBitmap = memDC.SelectObject(&bitmap);

		m_Pic.GetWindowRect(&rect);


		int xx = 0, yy = 0;

		if (m_ZoomRate == 10)
		{
			StretchDIBits(memDC.m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0,
				cvCopyImgColor.cols, cvCopyImgColor.rows, cvCopyImgColor.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		else if (m_ZoomRate != 10)
		{
			xx = cvCopyImgColor.cols * (10 - m_ZoomRate) / 10;
			yy = cvCopyImgColor.rows * (10 - m_ZoomRate) / 10;

			int x1, x2, y1, y2;
			x2 = cvCopyImgColor.cols - xx * 2;
			y2 = cvCopyImgColor.rows - yy * 2;

			if (b_IsDragging)
			{
				if (m_Mouse.x < 0)
					m_Mouse.x = 0;
				if (m_Mouse.x > cvCopyImgColor.cols - x2)
					m_Mouse.x = cvCopyImgColor.cols - x2;

				x1 = m_Mouse.x;

				if (m_Mouse.y < 0)
					m_Mouse.y = 0;
				if (m_Mouse.y > cvCopyImgColor.rows - y2)
					m_Mouse.y = cvCopyImgColor.rows - y2;

				y1 = m_Mouse.y;
			}
			else
			{
				x1 = cvCopyImgColor.cols / 2 - x2 / 2;
				y1 = cvCopyImgColor.rows / 2 - y2 / 2;
				m_Mouse.x = cvCopyImgColor.cols / 2 - x2 / 2;
				m_Mouse.y = cvCopyImgColor.rows / 2 - y2 / 2;
			}
			StretchDIBits(memDC.m_hDC, 0, 0, rect.Width(), rect.Height(), x1, y1, x2, y2,
				cvCopyImgColor.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		}

		//StretchDIBits(memDC.m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0,
		//	cvCopyImgColor.cols, cvCopyImgColor.rows, cvCopyImgColor.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		//GetXY();

		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);

		memDC.DeleteDC();
		bitmap.DeleteObject();

		if (!m_ShowPosition && m_ZoomRate == 10)
		{
			CDC* pDC = m_Pic.GetDC();
			bool bResult = false;
			bResult = GetXY();

			if (bResult)
			{
				int w = rect.right - rect.left;
				int h = rect.bottom - rect.top;
				// cvtColor, convertto 비교
				//cv::cvtColor(cvCopyImg, cvCopyImgColor, cv::COLOR_GRAY2BGR);
				for (auto x = 0; x < g_Width; x++)
				{
					for (auto y = 0; y < g_Height; y++)
					{
						switch (m_BadImg[y][x])
						{
						case 1:
							//cv::rectangle(cvCopyImgColor, cv::Rect(x - 8, g_Height - y - 8, 16, 16), cv::Scalar(255, 200, 0), 3);
							pDC->SelectObject(pen1);
							break;
						case 2:
							//cv::rectangle(cvCopyImgColor, cv::Rect(x - 8, g_Height - y - 8, 16, 16), cv::Scalar(0, 200, 0), 3);
							pDC->SelectObject(pen2);
							break;
						case 3:
							//cv::rectangle(cvCopyImgColor, cv::Rect(x - 8, g_Height - y - 8, 16, 16), cv::Scalar(0, 0, 200), 3);
							pDC->SelectObject(pen3);
							break;
						case 4:
							//cv::rectangle(cvCopyImgColor, cv::Rect(x - 8, g_Height - y - 8, 16, 16), cv::Scalar(255, 100, 0), 3);
							pDC->SelectObject(pen4);
							break;
						case 5:
							//cv::rectangle(cvCopyImgColor, cv::Rect(x - 8, g_Height - y - 8, 16, 16), cv::Scalar(9, 232, 238), 3);
							pDC->SelectObject(pen5);
							break;
						case 6:
							//cv::rectangle(cvCopyImgColor, cv::Rect(x - 8, g_Height - y - 8, 16, 16), cv::Scalar(200, 0, 200), 3);
							pDC->SelectObject(pen6);
							break;
						}

						pDC->MoveTo((w * y / g_Width) - 8, (h * (g_Height - x) / g_Height) - 8);
						pDC->LineTo((w * y / g_Width) - 8, (h * (g_Height - x) / g_Height) + 8);
						pDC->LineTo((w * y / g_Width) + 8, (h * (g_Height - x) / g_Height) + 8);
						pDC->LineTo((w * y / g_Width) + 8, (h * (g_Height - x) / g_Height) - 8);
						pDC->LineTo((w * y / g_Width) - 8, (h * (g_Height - x) / g_Height) - 8);
					}
				}
			}
			//CreateBitmapInfo(cvCopyImgColor.cols, cvCopyImgColor.rows, cvCopyImgColor.channels() * 8);
			ReleaseDC(pDC);
		}
	}

	std:; string strGpath = cstos(g_path);
	if(fs::exists(strGpath))
	{
		if(m_NationalCode==1)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_ENG, IDB_BT_REJECTPLAY_0_ENG);
		else if(m_NationalCode==2)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_CHI, IDB_BT_REJECTPLAY_0_CHI);
		else if(m_NationalCode==0)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY, IDB_BT_REJECTPLAY_0);
		else if(m_NationalCode==3)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_RUS, IDB_BT_REJECTPLAY_0_RUS);
		else if(m_NationalCode==4)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_UKR, IDB_BT_REJECTPLAY_0_UKR);
		else if(m_NationalCode==5)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_SPN, IDB_BT_REJECTPLAY_0_SPN);
		else if(m_NationalCode==6)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_POL, IDB_BT_REJECTPLAY_0_POL);
		else if(m_NationalCode==7)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_TIW, IDB_BT_REJECTPLAY_0_TIW);
		GetDlgItem(IDC_PLAY)->EnableWindow(TRUE);
	}
	else
	{
		if(m_NationalCode==1)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_1_ENG, IDB_BT_REJECTPLAY_1_ENG);
		else if(m_NationalCode==2)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_1_CHI, IDB_BT_REJECTPLAY_1_CHI);
		else if(m_NationalCode==0)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_1, IDB_BT_REJECTPLAY_1);
		else if(m_NationalCode==3)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_1_RUS, IDB_BT_REJECTPLAY_1_RUS);
		else if(m_NationalCode==4)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_1_UKR, IDB_BT_REJECTPLAY_1_UKR);
		else if(m_NationalCode==5)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_1_SPN, IDB_BT_REJECTPLAY_1_SPN);
		else if(m_NationalCode==6)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_1_POL, IDB_BT_REJECTPLAY_1_POL);
		else if(m_NationalCode==7)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_1_TIW, IDB_BT_REJECTPLAY_1_TIW);
		
		GetDlgItem(IDC_PLAY)->EnableWindow(FALSE);
	}
	*/


	CRect rect;
	//HBITMAP hBmp;
	//BITMAP  bi;
	CFileFind finder;

	//20180322
	bool b_twice = false;

	CString TempPass;
	TempPass.Format(_T("%s\\%s"), m_StrDirectory, m_StrFileName);

	m_Fullname = m_StrDirectory + "\\" + m_StrFileName; //리젝트 동영상 추가 20170404 LIM
	CMedia dlg;
	dlg.m_Path = m_Fullname;
	g_path = m_Fullname;
	g_path.Replace(_T("jpg"), _T("avi"));

	if (g_path.Find(_T("NG")))
	{
		g_path.Replace(_T("NG"), _T("VIDEO"));
	}

	if (g_path.Find(_T("OMIT")))
	{
		g_path.Replace(_T("OMIT"), _T("VIDEO"));
	}

	//	g_path.Replace(_T("D:"),_T("H:"));




	CString FileType = m_StrFileName.Right(3);

	if (finder.FindFile(TempPass))
	{
		// 20231031 grayscale load -> color load 변경
		cv::Mat cvOrigin = cv::imread(cstos(TempPass), 1);

		if (cvOrigin.empty())	return;

		cv::Mat cvCopyImgT(cvOrigin.rows, cvOrigin.cols / 2, cvOrigin.type(), cv::Scalar(255));
		cv::Mat cvCopyImg;
		if (cvOrigin.cols >= cvOrigin.rows * 2)
		{
			//cvCopyImgT = cvOrigin(cv::Rect(cvOrigin.cols / 2, 0, cvOrigin.cols / 2, cvOrigin.rows));

			if(m_ShowPosition == 0)
				cvCopyImgT = cvOrigin(cv::Rect(0, 0, cvOrigin.cols / 2, cvOrigin.rows));	// 20231031 왼쪽 Crop 변경
			else
				cvCopyImgT = cvOrigin(cv::Rect(cvOrigin.cols / 2, 0, cvOrigin.cols / 2, cvOrigin.rows));
			cvCopyImg = cvCopyImgT.clone();
		}
		else
			cvCopyImg = cvOrigin.clone();

		int nTemp_h = cvCopyImg.rows;
		int nTemp_w = cvCopyImg.cols;

		if (!b_IsMakeArray)
		{
			g_Width = nTemp_w;
			g_Height = nTemp_h;
			NewArray();
			b_IsMakeArray = true;
		}

		cv::Mat gray;
		cv::cvtColor(cvCopyImg, gray, cv::COLOR_BGR2GRAY);
		cv::flip(gray, gray, 0);
		for (int y = 0; y < g_Height; y++)
		{
			const unsigned char* pRow = gray.ptr<unsigned char>(y);
			for (int x = 0; x < g_Width; x++)
			{
				m_ImgBuffer[y][x] = pRow[x];
			}
		}

		// 20231031 주석처리 - Color 이미지 load
		cv::Mat cvCopyImgColor = cvCopyImg.clone();
		//cv::cvtColor(cvCopyImg, cvCopyImgColor, cv::COLOR_GRAY2BGR);

		CreateBitmapInfo(cvCopyImgColor.cols, cvCopyImgColor.rows, cvCopyImgColor.channels() * 8);

		CClientDC dc(GetDlgItem(IDC_PIC));
		GetDlgItem(IDC_PIC)->GetClientRect(&rect);

		CDC memDC;
		CBitmap* pOldBitmap, bitmap;
		memDC.CreateCompatibleDC(&dc);
		memDC.SetStretchBltMode(COLORONCOLOR);
		memDC.SelectStockObject(NULL_BRUSH);
		bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
		pOldBitmap = memDC.SelectObject(&bitmap);

		m_Pic.GetWindowRect(&rect);

		int xx = 0, yy = 0;

		if (m_ZoomRate == 10)
		{
			StretchDIBits(memDC.m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0,
				cvCopyImgColor.cols, cvCopyImgColor.rows, cvCopyImgColor.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		else if (m_ZoomRate != 10)
		{
			xx = cvCopyImgColor.cols * (10 - m_ZoomRate) / 10;
			yy = cvCopyImgColor.rows * (10 - m_ZoomRate) / 10;

			int x1, x2, y1, y2;
			x2 = cvCopyImgColor.cols - xx * 2;
			y2 = cvCopyImgColor.rows - yy * 2;

			if (b_IsDragging)
			{
				if (m_Mouse.x < 0)
					m_Mouse.x = 0;
				if (m_Mouse.x > cvCopyImgColor.cols - x2)
					m_Mouse.x = cvCopyImgColor.cols - x2;

				x1 = m_Mouse.x;

				if (m_Mouse.y < 0)
					m_Mouse.y = 0;
				// y 움직임 수정 필요
				if (m_Mouse.y > cvCopyImgColor.rows - y2)
					m_Mouse.y = cvCopyImgColor.rows - y2;

				y1 = m_Mouse.y;
			}
			else
			{
				x1 = cvCopyImgColor.cols / 2 - x2 / 2;
				y1 = cvCopyImgColor.rows / 2 - y2 / 2;
				m_Mouse.x = cvCopyImgColor.cols / 2 - x2 / 2;
				m_Mouse.y = cvCopyImgColor.rows / 2 - y2 / 2;
			}
			StretchDIBits(memDC.m_hDC, 0, 0, rect.Width(), rect.Height(), x1, y1, x2, y2,
				cvCopyImgColor.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		}

		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);

		memDC.DeleteDC();
		bitmap.DeleteObject();

		if (m_ShowPosition == 0)
			bool result = GetXY();
		else
			GetInfo();


		// 불량 위치 Draw - 20231031 주석처리 (왼쪽에 Draw된 이미지 crop하여 보여줌)
		if (!m_ShowPosition && m_ZoomRate == 10)
		{
			CDC* pDC = m_Pic.GetDC();
			bool result = false;

			if (m_SelectedTabNum == 1)
				GetInfo();
			else
				result = GetXY();

			
			if (result)
			{
				int w = rect.right - rect.left;
				int h = rect.bottom - rect.top;

				// All 이미지 관련 처리
				// topdown 1280*1280, Side 640*640 -> 1280으로 resize
				for (int x = 0; x < g_Height; x++)
				{
					for (int y = 0; y < g_Width; y++)
					{
						if((m_BadImg[x][y] >= 1 && m_BadImg[x][y] <=6)|| m_BadImg[x][y] == 10)
						{
							if (x <= 20 || x >= g_Height - 20 || y <= 20 || y >= g_Width - 20)
							{
								switch (m_BadImg[x][y])
								{
								case 6:
									pDC->SetPixel((w * y / g_Width), (h * (g_Height - x) / g_Height), RGB(200, 0, 200));
									break;
								case 5:
									pDC->SetPixel((w * y / g_Width), (h * (g_Height - x) / g_Height), RGB(9, 232, 238));
									break;
								case 4:
									pDC->SetPixel((w * y / g_Width), (h * (g_Height - x) / g_Height), RGB(255, 100, 0));
									break;
								case 3:
									pDC->SetPixel((w * y / g_Width), (h * (g_Height - x) / g_Height), RGB(0, 0, 200));
									break;
								case 2:
									pDC->SetPixel((w * y / g_Width), (h * (g_Height - x) / g_Height), RGB(0, 200, 0));
									break;
								case 1:
									pDC->SetPixel((w * y / g_Width), (h * (g_Height - x) / g_Height), RGB(255, 200, 0));
									break;
								}
							}
							else
							{
								switch (m_BadImg[x][y])
								{
								case 10:
									pDC->SelectObject(pen10);
									break;
								case 6:
									pDC->SelectObject(pen6);
									break;
								case 5:
									pDC->SelectObject(pen5);
									break;
								case 4:
									pDC->SelectObject(pen4);
									break;
								case 3:
									pDC->SelectObject(pen3);
									break;
								case 2:
									pDC->SelectObject(pen2);
									break;
								case 1:
									pDC->SelectObject(pen1);
									break;
								}
								if (m_BadImg[x][y] == 10)
								{
									pDC->MoveTo((w* y / g_Width) - 15, (h* (g_Height - x) / g_Height) - 15);
									pDC->LineTo((w* y / g_Width) - 15, (h* (g_Height - x) / g_Height) + 15);
									pDC->LineTo((w* y / g_Width) + 15, (h* (g_Height - x) / g_Height) + 15);
									pDC->LineTo((w* y / g_Width) + 15, (h* (g_Height - x) / g_Height) - 15);
									pDC->LineTo((w* y / g_Width) - 15, (h* (g_Height - x) / g_Height) - 15);
								}
								else 
								{
									pDC->MoveTo((w * y / g_Width) - 8, (h * (g_Height - x) / g_Height) - 8);
									pDC->LineTo((w * y / g_Width) - 8, (h * (g_Height - x) / g_Height) + 8);
									pDC->LineTo((w * y / g_Width) + 8, (h * (g_Height - x) / g_Height) + 8);
									pDC->LineTo((w * y / g_Width) + 8, (h * (g_Height - x) / g_Height) - 8);
									pDC->LineTo((w * y / g_Width) - 8, (h * (g_Height - x) / g_Height) - 8);
								}

								for (int a = x - 4; a < x + 4; a++)
								{
									for (int b = y - 4; b < y + 4; b++)
									{
										m_BadImg[a][b] = 0;
									}
								}
							}							
						}
					}
				}
			}
			
			ReleaseDC(pDC);
		}
	}
	



	bool bResult = finder.FindFile(g_path);
	if (bResult == true)
	{
		if (m_NationalCode == 1)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_ENG, IDB_BT_REJECTPLAY_0_ENG);
		else if (m_NationalCode == 2)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_CHI, IDB_BT_REJECTPLAY_0_CHI);
		else
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY, IDB_BT_REJECTPLAY_0);
		GetDlgItem(IDC_PLAY)->EnableWindow(TRUE);
	}
	else
	{
		if (m_NationalCode == 1)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_1_ENG, IDB_BT_REJECTPLAY_1_ENG);
		else if (m_NationalCode == 2)
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_1_CHI, IDB_BT_REJECTPLAY_1_CHI);
		else
			m_PlayReject.LoadBitmaps(IDB_BT_REJECTPLAY_1, IDB_BT_REJECTPLAY_1);

		GetDlgItem(IDC_PLAY)->EnableWindow(FALSE);
	}
}


void CViewerDlg::OnDatetimechangeDatetimepicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp=m_CurrentDate;
	UpdateData(true);
    m_CurrentDate.Format(_T("%4d-%02d-%02d"), m_Date.GetYear(), m_Date.GetMonth(), m_Date.GetDay());  
	if(m_CurrentDate!=temp)
	{
		*pResult = 0;
		m_ZoomRate=10;
		SearchListData();
	}
	*pResult = 0;
}

bool CViewerDlg::GetXY()
{
	if (!b_IsMakeArray)
		return false;

	for (auto i = 0; i < g_Height; i++)
		for (auto j = 0; j < g_Width; j++)
			m_BadImg[i][j] = 0;

	CString csTempPath;
	CString csProduct;
	csTempPath.Format(_T("%s\\Log\\%s"), m_StrDirectory, m_StrLogName);

	ifstream file(csTempPath);
	std::string line;
	// Log file open
	if (file.is_open())
	{
		while (getline(file, line)) {
		}
		file.close();
	}
	else
	{
		m_ProductName = "";
		GetDlgItem(IDC_PRODUCTNAME)->SetWindowText(m_ProductName);
		m_ConditionInfo = "";
		GetDlgItem(IDC_CONDITIONINFO)->SetWindowText(m_ConditionInfo);
		GetDlgItem(IDC_CONDITIONINFO2)->SetWindowText(m_ConditionInfo);
		GetDlgItem(IDC_CONDITIONINFO3)->SetWindowText(m_ConditionInfo);
		GetDlgItem(IDC_CONDITIONINFO4)->SetWindowText(m_ConditionInfo);
		GetDlgItem(IDC_CONDITIONINFO5)->SetWindowText(m_ConditionInfo);
		GetDlgItem(IDC_CONDITIONINFO6)->SetWindowText(m_ConditionInfo);
		return false;
	}
	// 이물 좌표 .dat 파일 규칙
	// 제품명@검사값/검사값!#검사조건.X좌표.Y좌표+#
	// 제품명
	std::vector<std::string> vTokProduct;
	vTokProduct = tokenize_getline(line, '@');
	csProduct = stocs(vTokProduct[0]);
	GetDlgItem(IDC_PRODUCTNAME)->SetWindowText(csProduct);

	// 검사값
	std::vector<std::string> vTokCondition;
	vTokCondition = tokenize_getline(vTokProduct[1], '!');
	std::vector<std::string> vTokConditionSep;
	vTokConditionSep = tokenize_getline(vTokCondition[0], '/');
	GetDlgItem(IDC_CONDITIONINFO)->SetWindowTextW(stocs(vTokConditionSep[0]));
	GetDlgItem(IDC_CONDITIONINFO2)->SetWindowTextW(stocs(vTokConditionSep[1]));
	GetDlgItem(IDC_CONDITIONINFO3)->SetWindowTextW(stocs(vTokConditionSep[2]));
	GetDlgItem(IDC_CONDITIONINFO4)->SetWindowTextW(stocs(vTokConditionSep[3]));
	GetDlgItem(IDC_CONDITIONINFO5)->SetWindowTextW(stocs(vTokConditionSep[4]));
	GetDlgItem(IDC_CONDITIONINFO6)->SetWindowTextW(stocs(vTokConditionSep[5]));

	if (m_SelectedTabNum == 0) //0226
	{
		// 검사조건 및 좌표
		std::string strResult;
		strResult = vTokCondition[1].substr(0, vTokCondition[1].size() - 2);
		std::vector<std::string> vTokResultData;
		vTokResultData = tokenize_getline(strResult, '+');

		for (auto iter = vTokResultData.begin(); iter != vTokResultData.end(); iter++)
		{
			std::vector<std::string> vTokSub = tokenize_getline(*iter, '.');
			m_BadImg[stoi(vTokSub[1])][stoi(vTokSub[2])] = stoi(vTokSub[0]);
			vTokSub.clear();
		}
		vTokResultData.clear(); //0226
	}
	vTokProduct.clear();
	vTokCondition.clear();
	vTokConditionSep.clear();
	//vTokResultData.clear();//0226

	return true;
}


void CViewerDlg::GetInfo(void)
{
	if(!b_IsMakeArray)
		return;

	for(int i=0;i<g_Height;i++)
		for(int j=0;j<g_Width;j++)
			m_BadImg[i][j]=0;
	
	CString csTempPath;
	CString csProduct;
	csTempPath.Format(_T("%s\\Log\\%s"), m_StrDirectory, m_StrLogName);

	ifstream file(csTempPath);
	std::string line;
	if (file.is_open())
	{
		while (getline(file, line)) {
		}
		file.close();
	}
	else
	{
		m_ProductName = "";
		GetDlgItem(IDC_PRODUCTNAME)->SetWindowText(m_ProductName);
		m_ConditionInfo = "";
		GetDlgItem(IDC_CONDITIONINFO)->SetWindowTextW(m_ConditionInfo);
		GetDlgItem(IDC_CONDITIONINFO2)->SetWindowTextW(m_ConditionInfo);
		GetDlgItem(IDC_CONDITIONINFO3)->SetWindowTextW(m_ConditionInfo);
		GetDlgItem(IDC_CONDITIONINFO4)->SetWindowTextW(m_ConditionInfo);
		GetDlgItem(IDC_CONDITIONINFO5)->SetWindowTextW(m_ConditionInfo);
		GetDlgItem(IDC_CONDITIONINFO6)->SetWindowTextW(m_ConditionInfo);
	}

	// 이물 좌표 .dat 파일 규칙
	// 제품명@검사값/검사값!#검사조건.X좌표.Y좌표+#
	// 제품명
	std::vector<std::string> vTokProduct;
	vTokProduct = tokenize_getline(line, '@');
	csProduct = stocs(vTokProduct[0]);
	GetDlgItem(IDC_PRODUCTNAME)->SetWindowText(csProduct);

	// 검사값
	std::vector<std::string> vTokCondition;
	vTokCondition = tokenize_getline(vTokProduct[1], '!');
	std::vector<std::string> vTokConditionSep;
	vTokConditionSep = tokenize_getline(vTokCondition[0], '/');
	GetDlgItem(IDC_CONDITIONINFO)->SetWindowTextW(stocs(vTokConditionSep[0]));
	GetDlgItem(IDC_CONDITIONINFO2)->SetWindowTextW(stocs(vTokConditionSep[1]));
	GetDlgItem(IDC_CONDITIONINFO3)->SetWindowTextW(stocs(vTokConditionSep[2]));
	GetDlgItem(IDC_CONDITIONINFO4)->SetWindowTextW(stocs(vTokConditionSep[3]));
	GetDlgItem(IDC_CONDITIONINFO5)->SetWindowTextW(stocs(vTokConditionSep[4]));
	GetDlgItem(IDC_CONDITIONINFO6)->SetWindowTextW(stocs(vTokConditionSep[5]));

	vTokProduct.clear();
	vTokCondition.clear();
	vTokConditionSep.clear();
}


void CViewerDlg::OnClickedShowposition()
{
	m_ShowPosition = 0;
	UpdateData(false);
	DrawPicture();
}


void CViewerDlg::OnShowposition1()
{
	m_ShowPosition = 1;
	UpdateData(false);	
	DrawPicture();
}

void CViewerDlg::NewArray()
{
	m_BadImg = new unsigned char *[g_Height];
	m_ImgBuffer = new unsigned char *[g_Height];

	for(auto i=0;i<g_Height;i++)
	{
		m_BadImg[i] = new unsigned char [g_Width];
		m_ImgBuffer[i] = new unsigned char [g_Width];
	}
}

void CViewerDlg::ReleaseArray()
{
	for(auto i=0;i<g_Height;i++)
	{
		delete [] m_BadImg[i];
		delete [] m_ImgBuffer[i];
	}
	delete [] m_BadImg;
	delete [] m_ImgBuffer;
}

void CViewerDlg::ChangeFile()
{
	m_StrFileName = m_CtrlList.GetItemText(m_ListNum,0);
	int nNum = m_StrFileName.GetLength();
	m_StrLogName = m_StrFileName.Left(nNum - 4)+_T(".dat");
	CString FileType = m_StrFileName.Right(3);

	//if(FileType=="JPG" || FileType=="jpg")
	//	return; 

	//int temp_h=0, temp_w=0;
	//CFile infile;
	//BITMAPFILEHEADER hf;
	//BITMAPINFOHEADER hInfo;

	//if(!infile.Open(m_StrDirectory+"\\"+m_StrFileName, CFile::modeRead))
	//{
	//}
	//else
	//{
	//	infile.Read(&hf,sizeof(BITMAPFILEHEADER));
	//	if(!(hf.bfType == 0x4D42))
	//		return;
	//	infile.Read(&hInfo, sizeof(BITMAPINFOHEADER));
	//	if(hInfo.biBitCount=8) 
	//	{
	//		temp_h = hInfo.biHeight;
	//		temp_w = hInfo.biWidth;
	//	}
	//}

	//if(!b_IsMakeArray)
	//{
	//	g_Height=temp_h;
	//	g_Width=temp_w;
	//	NewArray();
	//	b_IsMakeArray=true;
	//}
	//else if(temp_h!=g_Height || temp_w!=g_Width)
	//{
	//	ReleaseArray();
	//	g_Height=temp_h;
	//	g_Width=temp_w;
	//	NewArray();
	//}
}

void CViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(!b_IsDragging)
	{
		if(m_Picrect.PtInRect(point)!=0)
		{	
			b_IsDragging=true;
			m_FirstPoint=point;
		}
		else
			b_IsDragging=false;
	}	

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CViewerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	b_IsDragging=false;
	

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CViewerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if(b_IsMakeArray)
		ReleaseArray();
}

void CViewerDlg::OnBnClickedCopyusb()
{	
	bool bRe = false;
	for (int i = 0; i < m_CtrlList.GetItemCount(); i++)
	{
		if (m_CtrlList.GetCheck(i) == 1)
		{
			bRe = true;
		}
	}

	if (!bRe)
		return;

	int suc = 0;
	int tcount = 0;
	int fcount = 0;

	TCHAR toPath[MAX_PATH + 2];
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = vMsgSavePath[m_NationalCode];
	//if (m_NationalCode == 1)
	//	bi.lpszTitle = (_T("Select the folder to save."));
	//else if (m_NationalCode == 2)
	//	bi.lpszTitle = (_T("选择存储路径."));
	//else if (m_NationalCode == 0)
	//	bi.lpszTitle = (_T("저장할 경로를 선택하세요."));
	//else if (m_NationalCode == 3)
	//	bi.lpszTitle = (_T("Выберите папку для сохранения."));
	//else if (m_NationalCode == 4)
	//	bi.lpszTitle = (_T("Виберіть папку для збереження."));
	//else if (m_NationalCode == 5)
	//	bi.lpszTitle = (_T("Seleccione la carpeta para guardar."));
	//else if (m_NationalCode == 6)
	//	bi.lpszTitle = (_T("Wybierz folder do zapisania."));
	//else if (m_NationalCode == 7)
	//	bi.lpszTitle = (_T("選取要儲存的資料夾."));
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != 0)
	{
		SHGetPathFromIDList(pidl, toPath);
		SetCurrentDirectory(toPath);
	}
	else
	{
		return;
	}

	for (int i = 0; i < m_CtrlList.GetItemCount(); i++)
	{
		if (m_CtrlList.GetCheck(i) == 1)
		{
			TCHAR fromPath[MAX_PATH + 2];
			memset(fromPath, 0, sizeof(fromPath));
			wcscpy(fromPath, m_StrDirectory + "\\" + m_CtrlList.GetItemText(i, 0));

			SHFILEOPSTRUCT shfo = { 0 };
			ZeroMemory(&shfo, sizeof shfo);
			shfo.hwnd = AfxGetMainWnd()->m_hWnd;
			shfo.wFunc = FO_COPY;
			shfo.fFlags = NULL;
			shfo.lpszProgressTitle = _T("File copy");
			shfo.fAnyOperationsAborted = false;
			shfo.pTo = toPath;
			shfo.pFrom = fromPath;

			tcount++;
			if (SHFileOperation(&shfo) == 0)
			{
				suc = 1;
			}
			else
			{
				suc = 2;
				fcount++;
			}

			if (suc == 1)
			{
				CString temp;
				temp.Format(vMsgSaveTotal[m_NationalCode], tcount);
				//if (m_NationalCode == 1)
				//	temp.Format(_T("Completed to copy %d files"), tcount);
				//else if (m_NationalCode == 2)
				//	temp.Format(_T("%d个文件复制完成"), tcount);
				//else if (m_NationalCode == 0)
				//	temp.Format(_T("총 %d개 파일 복사가 완료되었습니다"), tcount);
				//else if (m_NationalCode == 3)
				//	temp.Format(_T("Завершено копирование%d файлов"), tcount);
				//else if (m_NationalCode == 4)
				//	temp.Format(_T("Завершено для копіювання %d файлів"), tcount);
				//else if (m_NationalCode == 5)
				//	temp.Format(_T("Completado para copiar %d archivos"), tcount);
				//else if (m_NationalCode == 6)
				//	temp.Format(_T("Ukończono, aby skopiować %d plików"), tcount);
				//else if (m_NationalCode == 7)
				//	temp.Format(_T("已完成以複製 %d 個檔案"), tcount);
				AfxMessageBox(temp);
			}
			else
			{
				CString temp;
				temp.Format(vMsgSaveFail[m_NationalCode], fcount, tcount);
				/*if (m_NationalCode == 1)
					temp.Format(_T("Failed to copy %d files / %d total"), fcount, tcount);
				else if (m_NationalCode == 2)
					temp.Format(_T("%d/%d个文件复制失败"), fcount, tcount);
				else if (m_NationalCode == 0)
					temp.Format(_T("총 %d개의 파일 중 %d개의 파일 복사가 실패하였습니다."), tcount, fcount);
				else if (m_NationalCode == 3)
					temp.Format(_T("Не удалось скопировать %d файлов / %d всего"), fcount, tcount);
				else if (m_NationalCode == 4)
					temp.Format(_T("Не вдалося скопіювати %d файлів / %d всього."), tcount, fcount);
				else if (m_NationalCode == 5)
					temp.Format(_T("Completado para copiar %d archivos / %d total"), fcount, tcount);
				else if (m_NationalCode == 6)
					temp.Format(_T("Nie udało się skopiować %d plików / łącznie %d"), fcount, tcount);
				else if (m_NationalCode == 7)
					temp.Format(_T("無法複製 %d 個檔案 / %d 個總計"), fcount, tcount);*/
				AfxMessageBox(temp);
			}
		}
	}
}


void CViewerDlg::OnClickedAllcheck()
{
	b_AllCheck=true;

	if(!b_AllChecked)
	{
		b_AllChecked=true;
		m_BtAllCheck.SetBitmap(m_BmpAllCheck_1);
		for(int i=0;i<m_CtrlList.GetItemCount();i++)
		{
			m_CtrlList.SetCheck(i, true);	
		}
	}
	else
	{
		b_AllChecked=false;
		m_BtAllCheck.SetBitmap(m_BmpAllCheck);
		for(int i=0;i<m_CtrlList.GetItemCount();i++)
		{
			m_CtrlList.SetCheck(i, false);	
		}
	}
	b_AllCheck=false;
}


void CViewerDlg::OnBnClickedBtZoomin()
{
	m_ZoomRate-=1;	
	if(m_ZoomRate<6)
		m_ZoomRate=6;
	else
		DrawPicture();
}


void CViewerDlg::OnBnClickedBtZoomout()
{
	m_ZoomRate+=1;
	if(m_ZoomRate>10) 
		m_ZoomRate=10;
	else
		DrawPicture();
}


void CViewerDlg::OnClickedRadiotap()
{
	m_BtTabNg.SetBitmap(m_bmpTabNg);
	m_BtTabOmit.SetBitmap(m_bmpTabOmit_1);
	//20200601
	if(m_bUseTestMode)
		m_BtTabTest.SetBitmap(m_bmpTabTest_1);
	m_SelectedTabNum = 0;
	SearchListData();
}


void CViewerDlg::OnRadiotap1()
{
	m_BtTabNg.SetBitmap(m_bmpTabNg_1);
	m_BtTabOmit.SetBitmap(m_bmpTabOmit);
	//20200601
	if(m_bUseTestMode)
		m_BtTabTest.SetBitmap(m_bmpTabTest_1);
	m_SelectedTabNum = 1;
	SearchListData();
}


//20200411
void CViewerDlg::OnRadiotap2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_BtTabNg.SetBitmap(m_bmpTabNg_1);
	m_BtTabOmit.SetBitmap(m_bmpTabOmit_1);
	m_BtTabTest.SetBitmap(m_bmpTabTest);
	m_SelectedTabNum = 2;
	SearchListData();
}

void CViewerDlg::OnItemchangedFilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(b_AllCheck)
		return;	
	
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	int num = pNMLV->iItem;	
	if(m_ListNum!=num)
	{
		m_ListNum = num;
		ChangeFile();
		m_ZoomRate=10;
		DrawPicture();
	}
	else
	{
		return;
	}

	*pResult = 0;
}



void CViewerDlg::OnBnClickedCopyreject()  //20170322
{
	CString sHtml;
	CString m_Filename;
	CString csTempPath;
	csTempPath.Format(_T("%s\\Log\\%s"), m_StrDirectory, m_StrLogName);

	if (m_NameCount == 0)
	{
		AfxMessageBox(vMsgSaveImage[m_NationalCode]);

		//if (m_NationalCode == 0)
		//	AfxMessageBox(_T("저장된 영상이 없습니다."));
		//else if (m_NationalCode == 1)
		//	AfxMessageBox(_T("There is no saved image."));
		//else if (m_NationalCode == 2)
		//	AfxMessageBox(_T("没有保存的图像。"));
		//else if (m_NationalCode == 3)
		//	AfxMessageBox(_T("Нет сохраненного изображения."));
		//else if (m_NationalCode == 4)
		//	AfxMessageBox(_T("Збереженого зображення немає.。"));
		//else if (m_NationalCode == 5)
		//	AfxMessageBox(_T("No hay imagen guardada."));
		//else if (m_NationalCode == 6)
		//	AfxMessageBox(_T("Nie ma zapisanego obrazu."));
		//else if (m_NationalCode == 7)
		//	AfxMessageBox(_T("沒有儲存的映像檔."));
		return;
	}

	m_Filename = vFilename[m_NationalCode];

	ifstream fs(m_Filename);
	std::string line;
	std::string strHtml;
	// Log file open
	if (fs.is_open())
	{
		while (getline(fs, line)) {
			strHtml += line;
		}
		fs.close();

		CString pRS1 = _T("");
		CString tempRS;
		for (int i = 1; i < m_NameCount + 1; i++)
		{
			int nNum = m_strName[i - 1].GetLength();
			csTempPath.Format(_T("%s\\Log\\%s"), m_StrDirectory, (m_strName[i - 1].Left(nNum - 4) + _T(".dat")));
			AfxMessageBox(csTempPath);

			ifstream fsFile(csTempPath);
			std::string ss;
			while (getline(fsFile, ss)) {
			}
			fsFile.close();

			std::vector<std::string> vTokProduct;
			vTokProduct = tokenize_getline(ss, '@');
			m_ProductName = stocs(vTokProduct[0]);

			tempRS.Format(_T("<tr height=33 style='mso-height-source:userset;height:24.95pt'><td colspan=4 height=33 class=xl30 style='height:24.95pt'>%s</td><td colspan=3 class=xl30 style='border-left:none'>%d</td><td colspan=6 class=xl30 style='border-left:none'>%s</td></td><td colspan=2 class=xl30 style='border-left:none'></td> </tr>"), m_ProductName, i, m_strName[i - 1]);
			pRS1 += tempRS;
		}

		m_strName.clear();

		CFileDialog filedlg(false, _T("Excel file(*.xls)"), m_CurrentDate, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Save(*.xls) |*.xls|"), NULL);

		sHtml = stocs(strHtml);
		sHtml.Replace(_T("<--today-->"), m_CurrentDate);
		sHtml.Replace(_T("<--table1-->"), pRS1);
		CString path = _T(""), ext = _T("");

		int user_choice = filedlg.DoModal();
		if (user_choice == IDOK)
		{
			path = filedlg.GetPathName();
			ext = filedlg.GetFileExt();

			if (ext == "xls")
			{
				CFile sFile;
				sFile.Open(path, CFile::modeCreate | CFile::modeWrite);
				DWORD dwSize = sHtml.GetLength();   // 파일의 크기를 바이트 단위로 알아냄 << 첫번째
				char* lpsz = new char[dwSize + 1];
				USES_CONVERSION;
				//lpsz = T2A(sHtml);
				int len = WideCharToMultiByte(CP_ACP, 0, sHtml, -1, NULL, 0, NULL, NULL);
				WideCharToMultiByte(CP_ACP, 0, sHtml, -1, lpsz, len, NULL, NULL);
				len = strlen(lpsz);
				lpsz[len] = '\0';
				sFile.Write(lpsz, len);
				sHtml.ReleaseBuffer();
				sFile.Close();
			}
		}
	}
}


void CViewerDlg::OnBnClickedUp() //20190611 viewer 개선
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE vk;
	vk = VK_UP;
	GetDlgItem(IDC_FILELIST)->SetFocus();
	keybd_event(vk,0,0,0);
}


void CViewerDlg::OnBnClickedDown() //20190611 viewer 개선
{
	BYTE vk;
	vk = VK_DOWN;
	GetDlgItem(IDC_FILELIST)->SetFocus();
	keybd_event(vk,0,0,0);
}


//20200326
void CViewerDlg::OnBnClickedPlay()
{
	CMedia dlg;
	dlg.DoModal();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

std::vector<CString> CViewerDlg::tokenize_getlineCS(const CString data, const char chSep)
{
	std::vector<CString> result = {};
	std::string token = {};
	std::string strData = cstos(data);
	std::stringstream ss(strData);
	while (std::getline(ss, token, chSep)) {
		result.push_back(stocs(token));
	}
	return result;
}

std::vector<std::string> CViewerDlg::tokenize_getline(const std::string data, const char chSep)
{
	std::vector<std::string> result = {};
	std::string token = {};
	std::stringstream ss(data);
	while (std::getline(ss, token, chSep)) {
		result.push_back(token);
	}
	return result;
}

//HBITMAP CViewerDlg::Mat2DIB(const cv::Mat& img)
//{
//	return HBITMAP();
//}

void CViewerDlg::CreateBitmapInfo(int w, int h, int bpp)
{
	m_pBitmapInfo = NULL;

	if (bpp == 8)
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biPlanes = 1;
	m_pBitmapInfo->bmiHeader.biBitCount = bpp;
	m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_pBitmapInfo->bmiHeader.biSizeImage = 0;
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biClrUsed = 0;
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8) {
		for (int i = 0; i < 256; i++) {
			m_pBitmapInfo->bmiColors->rgbBlue = (BYTE)i;
			m_pBitmapInfo->bmiColors->rgbGreen = (BYTE)i;
			m_pBitmapInfo->bmiColors->rgbRed = (BYTE)i;
			m_pBitmapInfo->bmiColors->rgbReserved = 0;
		}
	}

	m_pBitmapInfo->bmiHeader.biWidth = w;
	m_pBitmapInfo->bmiHeader.biHeight = -h;
}
