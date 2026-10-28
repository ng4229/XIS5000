// CtrlPanelView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "CtrlPanelView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "../Viewer/ViewerDlg.h"
#include "../ReportDll/ReportDlg.h"
#include "../ReportDll/ReportTabCtrl.h"

#include "InitailDlg.h"// CCtrlPanelView
#include "MessageBoxDlg.h"
#include "SelectProduct.h"
#include "ModifyProductName.h"
#include "SetupMenu.h"
#include "AdminDlg.h"
#include "AutoSetup.h"
#include "SetupAuto.h"
#include "SetupAutoXray.h"
#include "Maintenance.h"

IMPLEMENT_DYNCREATE(CCtrlPanelView, CFormView)

static bool b_ThreadGen = false;
static bool b_ThreadSC = false; //20191202
CCtrlPanelView::CCtrlPanelView()
	: CFormView(CCtrlPanelView::IDD)
	, m_Comparedate(_T(""))
{
	m_GComm=NULL;
	d_SpaceMsg=NULL;

	g_Xray_kv=70;
	g_Xray_ma=2000;

	g_XrayState=0;
	g_iXrayOn=0;

	g_bGenerator=true;
	g_bDetector=true;
	g_bIOcard=true;	
	m_Interock_State=false;

	g_bAdmin=false;
	g_Password=_T("0000");
	g_ProductName=_T("");
	g_ChangeNGCond=false;
	g_ChangeOmitCond[0]=false;
	g_ChangeOmitCond[1]=false;
	g_ChangeOmitCond[2]=false;
	g_ChangeMaskCond[0]=false;
	g_ChangeMaskCond[1]=false;
	g_ChangeMaskCond[2]=false;
	g_bUseNormal=false;
	g_bAutoSetup=false;
	g_bXrayAutoSetup=false;

	g_Margin=10;

	g_PreproUse=0;
	g_PreproDisp=0;
	g_PreproValue=0;
	g_AirconCycle=0; 

	g_MaskBoxCondition1=0;
	g_MaskBoxCondition2=0;
	g_MaskBoxCondition3=0;
	g_MaskClip_Direction=0;
	g_MaskClip_Length=0;
	g_MaskClip_Deep=0;

	g_Countnum=0;
	g_Countnoise=0;
	g_PageMaskLR=0;
	g_PageMaskUD=0;
	g_TalMask_Bright=0;
	g_TalMask_Bright2=0;

	m_PreName=_T("");
	b_HandDetect=false;

}

CCtrlPanelView::~CCtrlPanelView()
{
}

void CCtrlPanelView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MENU_VIEWER, m_Bt_Viewer);
	DDX_Control(pDX, IDC_MENU_STOP, m_Bt_Stop);
	DDX_Control(pDX, IDC_MENU_START, m_Bt_Start);
	DDX_Control(pDX, IDC_MENU_SETUP, m_Bt_Setup);
	DDX_Control(pDX, IDC_MINI, m_Bt_Window);
	DDX_Control(pDX, IDC_MENU_RESETCOUNT, m_Bt_ResetCount);
}

BEGIN_MESSAGE_MAP(CCtrlPanelView, CFormView)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_MENU_SETUP, &CCtrlPanelView::OnBnClickedMenuSetup)
	ON_BN_CLICKED(IDC_MENU_VIEWER, &CCtrlPanelView::OnBnClickedMenuViewer)
	ON_BN_CLICKED(IDC_MENU_START, &CCtrlPanelView::OnBnClickedMenuStart)
	ON_BN_CLICKED(IDC_MENU_STOP, &CCtrlPanelView::OnBnClickedMenuStop)
	ON_BN_CLICKED(IDC_MENU_RESETCOUNT, &CCtrlPanelView::OnBnClickedMenuResetcount)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_EYH_STOP, &CCtrlPanelView::OnEyhStop)
	ON_BN_CLICKED(IDC_MINI, &CCtrlPanelView::OnBnClickedMini)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CCtrlPanelView 진단입니다.

#ifdef _DEBUG
void CCtrlPanelView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCtrlPanelView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCtrlPanelView 메시지 처리기입니다.
void CCtrlPanelView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CFont m_Font_Name;
	m_Font_Name.CreatePointFont(140, _T("맑은고딕")); 
	GetDlgItem(IDC_COUNT_TOTAL)->SetFont(&m_Font_Name); 
	GetDlgItem(IDC_COUNT_OK)->SetFont(&m_Font_Name); 
	GetDlgItem(IDC_COUNT_NG)->SetFont(&m_Font_Name); 
	GetDlgItem(IDC_COUNT_SOUP)->SetFont(&m_Font_Name); 


	LoadBackground();
	SetTimer(0,10, NULL);
	m_bkbrush.CreateSolidBrush(RGB(215,223,233)); 

	d_SpaceMsg = new CSpaceMsg;
	d_SpaceMsg->Create();
	d_SpaceMsg->ShowWindow(SW_HIDE);

	if(g_NationalCode==1)
		m_Bt_ResetCount.EnableWindow(false);

	HideCaret();
	g_StopKind = 0; //20191202

}


void CCtrlPanelView::LoadBackground(void)
{
	BITMAP bmpInfo;
	if(g_NationalCode==0)
	{
		m_bmpMainBG.LoadBitmap(IDB_CTRL_SCREEN);
		m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT,IDB_MENU_STARTBT_0);//, 0, IDB_MENU_STARTBT_2); 
		m_Bt_Stop.LoadBitmaps(IDB_MENU_STOPBT,IDB_MENU_STOPBT_0);//, 0, IDB_MENU_STOPBT_2); 
		m_Bt_Setup.LoadBitmaps(IDB_MENU_SETUPBT,IDB_MENU_SETUPBT_0);//, 0, IDB_MENU_SETUPBT_2); 
		m_Bt_Viewer.LoadBitmaps(IDB_MENU_VIEWERBT,IDB_MENU_VIEWERBT_0);//, 0, IDB_MENU_VIEWERBT_2); 
		m_Bt_ResetCount.LoadBitmaps(IDB_COUNTRESETBT,IDB_COUNTRESETBT_1, 0, IDB_COUNTRESETBT_2); 
	}
	else if(g_NationalCode==1)
	{
		m_bmpMainBG.LoadBitmap(IDB_CTRL_SCREEN_ENG);
		m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT_ENG,IDB_MENU_STARTBT_0_ENG);//, 0, IDB_MENU_STARTBT_2_ENG);  
		m_Bt_Stop.LoadBitmaps(IDB_MENU_STOPBT_ENG,IDB_MENU_STOPBT_0_ENG);//, 0, IDB_MENU_STOPBT_2_ENG); 
		m_Bt_Setup.LoadBitmaps(IDB_MENU_SETUPBT_ENG,IDB_MENU_SETUPBT_0_ENG);//, 0, IDB_MENU_SETUPBT_2_ENG); 
		m_Bt_Viewer.LoadBitmaps(IDB_MENU_VIEWERBT_ENG,IDB_MENU_VIEWERBT_0_ENG);//, 0, IDB_MENU_VIEWERBT_2_ENG); 
		m_Bt_ResetCount.LoadBitmaps(IDB_COUNTRESETBT_ENG,IDB_COUNTRESETBT_1_ENG, 0, IDB_COUNTRESETBT_2_ENG); 
	}
	else if(g_NationalCode==2)
	{
		m_bmpMainBG.LoadBitmap(IDB_CTRL_SCREEN_CHI);
		m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT_CHI,IDB_MENU_STARTBT_0_CHI);//, 0, IDB_MENU_STARTBT_2_CHI);  
		m_Bt_Stop.LoadBitmaps(IDB_MENU_STOPBT_CHI,IDB_MENU_STOPBT_0_CHI);//, 0, IDB_MENU_STOPBT_2_CHI); 
		m_Bt_Setup.LoadBitmaps(IDB_MENU_SETUPBT_CHI,IDB_MENU_SETUPBT_0_CHI);//, 0, IDB_MENU_SETUPBT_2_CHI);  
		m_Bt_Viewer.LoadBitmaps(IDB_MENU_VIEWERBT_CHI,IDB_MENU_VIEWERBT_0_CHI);//, 0, IDB_MENU_VIEWERBT_2_CHI); 
		m_Bt_ResetCount.LoadBitmaps(IDB_COUNTRESETBT_CHI,IDB_COUNTRESETBT_1_CHI, 0, IDB_COUNTRESETBT_2_CHI);  
	}
	else if(g_NationalCode==3)
	{
		m_bmpMainBG.LoadBitmap(IDB_CTRL_SCREEN_SPN);
		m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT_SPN,IDB_MENU_STARTBT_0_SPN);//, 0, IDB_MENU_STARTBT_2_CHI);  
		m_Bt_Stop.LoadBitmaps(IDB_MENU_STOPBT_SPN,IDB_MENU_STOPBT_0_SPN);//, 0, IDB_MENU_STOPBT_2_CHI); 
		m_Bt_Setup.LoadBitmaps(IDB_MENU_SETUPBT_SPN,IDB_MENU_SETUPBT_0_SPN);//, 0, IDB_MENU_SETUPBT_2_CHI);  
		m_Bt_Viewer.LoadBitmaps(IDB_MENU_VIEWERBT_SPN,IDB_MENU_VIEWERBT_0_SPN);//, 0, IDB_MENU_VIEWERBT_2_CHI); 
		m_Bt_ResetCount.LoadBitmaps(IDB_COUNTRESETBT_SPN,IDB_COUNTRESETBT_1_SPN, 0, IDB_COUNTRESETBT_2_SPN);  
	}		
    m_bmpMainBG.GetBitmap(&bmpInfo); 
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 
    SetWindowPos(NULL, 0, 0, m_nMainWidth, m_nMainHeight, SWP_NOZORDER | SWP_NOMOVE); 
		
	m_Bt_Start.SizeToContent();
	m_Bt_Stop.SizeToContent();
	m_Bt_Setup.SizeToContent();
	m_Bt_Viewer.SizeToContent();
	m_Bt_ResetCount.SizeToContent();
	m_Bt_Window.LoadBitmaps(IDB_BTWINDOW,IDB_BTWINDOW); 
	m_Bt_Window.SizeToContent();
}


BOOL CCtrlPanelView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    CDC MemDC; 
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SelectObject(&m_bmpMainBG);
    pDC->BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
	return false; //CFormView::OnEraseBkgnd(pDC);
}


void CCtrlPanelView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.
}

void CCtrlPanelView::AutoCondition()
{
	g_bAutoSetup=true;
	if(g_XrayState==1 || g_XrayState==2)
	{
	
	}
	else
	{
		OnBnClickedMenuStart();
	}

	g_bAutoSetup=true;
	CSetupAuto dlg;
	int result1 = dlg.DoModal();
	if(result1==IDOK)
	{
		g_bAutoSetup=false;
		CSelectProduct load;
		load.LoadValue(g_ProductName);
		OnLoadValue();
	}
}

//Main Menu
void CCtrlPanelView::OnBnClickedMenuSetup()
{
	if(m_Interock_State)
		return;
	
	if(g_ProductName=="")
	{
		SelectProduct();
		return;
	}

	CSetupMenu dlg;
	int result = dlg.DoModal();

	

	if(result==11)
	{
		CAdminDlg login;
		login.m_Mode=1;
		int result1 = login.DoModal();
	}
	else if(result==12)
	{
		SelectProduct();
	}
	else if(result==13)
	{	
		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
		pView->OnConditionsetup();			
	}
	else if(result==32)
	{
		AutoCondition();
	}
	else if(result==41)
	{
		OnMenuReport();
	}
	else if(result==42)
	{
		KillTimer(1);
		BmInfo->bmiHeader.biSizeImage = NSE_VAR_SIZE*NSE_VAR_SIZE;
		BmInfo->bmiHeader.biWidth = NSE_VAR_SIZE;

		CMaintenance MaintenDlg;
		MaintenDlg.DoModal();

		g_XrayState = 0;
		DrawXrayState();

		BmInfo->bmiHeader.biSizeImage = NSE_VAR_SIZE*NSE_WIDTH_SIZE;
		BmInfo->bmiHeader.biWidth = NSE_WIDTH_SIZE;
		SetTimer(1, 1200, NULL);
	}
	else if(result==66)
	{
		CStdioFile fileString;
		CString language;
		language.Format(_T("%d"), g_NationalCode);
		if(fileString.Open(_T("D:\\Database\\Language.ini"),
			CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
		{
			fileString.WriteString(language);
			fileString.Close();
		}

		CString DllPath, MessageNo;

		CRegKey reg;
		reg.Open(HKEY_CURRENT_USER, _T("Control Panel\\International"));
		reg.SetValue(_T("AM"), _T("s1159"));
		reg.SetValue(_T("PM"), _T("s2359"));
		reg.SetValue(_T("₩"), _T("sCurrency"));
		reg.SetValue(_T("-"), _T("sDate"));
		reg.SetValue(_T("yyyy-MM-dd"), _T("sLongDate"));
		reg.SetValue(_T("yyyy-MM-dd"), _T("sShortDate"));
		reg.SetValue(_T("tt h:mm:ss"), _T("sTimeFormat"));
		reg.SetValue(_T("tt h:mm"), _T("sShortTime"));
		if(g_NationalCode==0) //1042
		{
			DllPath.Format(_T("D:\\Database\\Korean.ini"));
			m_Bt_ResetCount.EnableWindow(true);

			reg.SetValue(_T("00000412"), _T("Locale"));
			reg.SetValue(_T("ko-KR"), _T("LocaleName"));
			reg.SetValue(_T("대한민국"), _T("sCountry"));
			reg.SetValue(_T("KOR"), _T("sLanguage"));
			reg.SetValue(_T("yyyy'년' M'월'"), _T("sYearMonth"));
			reg.SetValue(_T("82"), _T("iCountry"));
			reg.SetValue(_T("0"), _T("iCurrDigits"));
			reg.SetValue(_T("2"), _T("iDate"));
			reg.SetValue(_T("1"), _T("iLZero"));
			reg.SetValue(_T("0"), _T("iMeasure"));
			reg.SetValue(_T("1"), _T("iNegCurr"));
			reg.SetValue(_T("9"), _T("iPaperSize"));
			reg.SetValue(_T("0"), _T("iTime"));
			reg.SetValue(_T("1"), _T("iTimePrefix"));
		}
		else if(g_NationalCode==1)
		{
			DllPath.Format(_T("D:\\Database\\English.ini"));
			if(!g_bAdmin)
				m_Bt_ResetCount.EnableWindow(false);

			reg.SetValue(_T("00000409"), _T("Locale"));
			reg.SetValue(_T("en-US"), _T("LocaleName"));
			reg.SetValue(_T("미국"), _T("sCountry"));
			reg.SetValue(_T("ENU"), _T("sLanguage"));
			reg.SetValue(_T("MMMM, yyyy"), _T("sYearMonth"));
			reg.SetValue(_T("1"), _T("iCountry"));
			reg.SetValue(_T("2"), _T("iCurrDigits"));
			reg.SetValue(_T("0"), _T("iDate"));
			reg.SetValue(_T("1"), _T("iLZero"));
			reg.SetValue(_T("1"), _T("iMeasure"));
			reg.SetValue(_T("0"), _T("iNegCurr"));
			reg.SetValue(_T("1"), _T("iPaperSize"));
			reg.SetValue(_T("0"), _T("iTime"));
			reg.SetValue(_T("0"), _T("iTimePrefix"));
		}
		else if(g_NationalCode==2)
		{
			DllPath.Format(_T("D:\\Database\\Chinese.ini")); 
			m_Bt_ResetCount.EnableWindow(true);

			reg.SetValue(_T("00000804"), _T("Locale"));
			reg.SetValue(_T("zh-CN"), _T("LocaleName"));
			reg.SetValue(_T("중국"), _T("sCountry"));
			reg.SetValue(_T("CHS"), _T("sLanguage"));
			reg.SetValue(_T("yyyy'年'M'月"), _T("sYearMonth"));
			reg.SetValue(_T("86"), _T("iCountry"));
			reg.SetValue(_T("2"), _T("iCurrDigits"));
			reg.SetValue(_T("2"), _T("iDate"));
			reg.SetValue(_T("0"), _T("iLZero"));
			reg.SetValue(_T("0"), _T("iMeasure"));
			reg.SetValue(_T("2"), _T("iNegCurr"));
			reg.SetValue(_T("9"), _T("iPaperSize"));
			reg.SetValue(_T("1"), _T("iTime"));
			reg.SetValue(_T("0"), _T("iTimePrefix"));
		}
		else if(g_NationalCode==3)
		{
			DllPath.Format(_T("D:\\Database\\Spanish.ini")); 
			m_Bt_ResetCount.EnableWindow(true);

			reg.SetValue(_T("0000080A"), _T("Locale"));
			reg.SetValue(_T("es-MX"), _T("LocaleName"));
			reg.SetValue(_T("멕시코"), _T("sCountry"));
			reg.SetValue(_T("ESM"), _T("sLanguage"));
			reg.SetValue(_T("MMMM, yyyy"), _T("sYearMonth"));
			reg.SetValue(_T("52"), _T("iCountry"));
			reg.SetValue(_T("2"), _T("iCurrDigits"));
			reg.SetValue(_T("1"), _T("iDate"));
			reg.SetValue(_T("1"), _T("iLZero"));
			reg.SetValue(_T("0"), _T("iMeasure"));
			reg.SetValue(_T("1"), _T("iNegCurr"));
			reg.SetValue(_T("1"), _T("iPaperSize"));
			reg.SetValue(_T("0"), _T("iTime"));
			reg.SetValue(_T("0"), _T("iTimePrefix"));
		}
		else
		{   
			g_NationalCode=0;
			DllPath.Format(_T("D:\\Database\\Korean.ini"));

			reg.SetValue(_T("00000412"), _T("Locale"));
			reg.SetValue(_T("ko-KR"), _T("LocaleName"));
			reg.SetValue(_T("대한민국"), _T("sCountry"));
			reg.SetValue(_T("KOR"), _T("sLanguage"));
			reg.SetValue(_T("yyyy'년' M'월'"), _T("sYearMonth"));
			reg.SetValue(_T("82"), _T("iCountry"));
			reg.SetValue(_T("0"), _T("iCurrDigits"));
			reg.SetValue(_T("2"), _T("iDate"));
			reg.SetValue(_T("1"), _T("iLZero"));
			reg.SetValue(_T("0"), _T("iMeasure"));
			reg.SetValue(_T("1"), _T("iNegCurr"));
			reg.SetValue(_T("9"), _T("iPaperSize"));
			reg.SetValue(_T("0"), _T("iTime"));
			reg.SetValue(_T("1"), _T("iTimePrefix"));
		}
		reg.Close();


		TCHAR buf[MAX_PATH] = {0,};
		for(int i=1; i<=240; i++)
		{
			MessageNo.Format(_T("msg%d"), i);
			GetPrivateProfileStringW(_T("Message"), MessageNo, _T(""), buf, MAX_PATH, DllPath);
			g_MessageArr[i]=buf;
		}			

		LoadBackground();
		Invalidate(true);
		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
		pView->LoadBackground();
		pView->Invalidate(true);
		pView->OnDispCondActive();
	}	
	else if(result==33)
	{
		AutoXraySetup();
	}
	else if(result==111)
	{
		if(g_NationalCode==1)
			m_Bt_ResetCount.EnableWindow(false);
	}

}


void CCtrlPanelView::OnBnClickedMenuViewer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_Interock_State)
		return;

	CViewerDlg dlg;
	dlg.m_NationalCode=g_NationalCode;
	if (dlg.m_NationalCode == 3)	dlg.m_NationalCode = 5;		// 20240528 스페인어 강제 변환
	dlg.DoModal();
}


LRESULT CCtrlPanelView::OnEyhStop(WPARAM wparam, LPARAM lpara)
{
	int param = wparam;
	if(param == 1)
	{
		OnBnClickedMenuStop(); 
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[38];//"제품인식 오류! X-ray 발생장치가 정상적으로 동작하는지 확인하시고 다시 시작하세요. (E2011)";
		dlg.DoModal();
	}
	else if(param == 2)
	{
		OnBnClickedMenuStop();  
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[39];//"제품인식 오류! 컨베이어 벨트에 이물질이 있는지 확인하세요. (E2012)";
		dlg.DoModal();
	}
	else if(param == 3)
	{
		OnBnClickedMenuStop();  
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[40];//"제품인식 오류! 디텍터 셀에 이상이 발생하였습니다. 관리자에게 문의하세요. (E2013)";
		dlg.DoModal();
		
	}
	else if(param == 10)  
	{
		/*
		OnBnClickedMenuStop();  
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[41];//"디텍터 오류! 검사를 다시 시작해 주세요! (E2014)";
		dlg.DoModal();
		*/

		Sleepms(100);
		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
		pView->OnMenuReStart();

	}
	else if(param == 20)  
	{
		Sleepms(100);
		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
		pView->OnMenuReStart();

	}

	return 1;
}

void CCtrlPanelView::OnBnClickedMenuStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
	if((g_XrayState==1 || g_XrayState==2) && pView->b_ConveyorOn)
	{
		if(pView->b_ConveyorOn)
		{
			if(g_NationalCode==0)
				m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT,IDB_MENU_STARTBT_0, 0, IDB_MENU_STARTBT_2); 
			else if(g_NationalCode==1)
				m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT_ENG,IDB_MENU_STARTBT_0_ENG, 0, IDB_MENU_STARTBT_2_ENG);  
			else if(g_NationalCode==2)
				m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT_CHI,IDB_MENU_STARTBT_0_CHI, 0, IDB_MENU_STARTBT_2_CHI);  
			else if(g_NationalCode==3)
				m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT_SPN,IDB_MENU_STARTBT_0_SPN, 0, IDB_MENU_STARTBT_2_SPN);  
			m_Bt_Start.Invalidate();
	//		OnStartConveyor();
			Sleepms(300);
			pView->b_ConveyorOn=false;
		}
		return;
	}

	if(g_ConReject == 1)
	{
		if(NSE_IO_OUPUT_PORT==2)
		{
			g_Continu_Ng = 0;
			g_Bit_Out[3]=1;
			g_Bit_Out[5]=0;//부저이게맞나 확인
			g_Bit_Out[12]=0; //연속불량
			OnDigitalOutPut_All22();
		}
		else
		{
			//20200525
			//g_portWriteData = 0x04;
			//g_portWriteData = 0x40;
			g_portWriteData = 0x1d;	//260810 값변경 및 NSE_IO_OUPUT_PORT로 변
			ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
			if(error)
				return;
			//20200525
			/*
			USHORT      gwMask = NSE_CONVERYOR_MASKING;                  // mask
			USHORT      gwData = NSE_CONVERYOR_OFF;                  // output data

			m_ptDioWritePortByte.port = NSE_IO_OUPUT_PORT;
			m_ptDioWritePortByte.mask  = 0xc4;
			m_ptDioWritePortByte.state = 0x04;

			if((m_ErrCde = DRV_DioWritePortByte(m_DriverHandle,
				 (LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
			{
				DRV_DioWritePortByte(m_DriverHandle,(LPT_DioWritePortByte)&m_ptDioWritePortByte);
			//	return;
			}*/
		}

		if(g_NationalCode==0)
				m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT,IDB_MENU_STARTBT_0, 0, IDB_MENU_STARTBT_2); 
			else if(g_NationalCode==1)
				m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT_ENG,IDB_MENU_STARTBT_0_ENG, 0, IDB_MENU_STARTBT_2_ENG);  
			else if(g_NationalCode==2)
				m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT_CHI,IDB_MENU_STARTBT_0_CHI, 0, IDB_MENU_STARTBT_2_CHI);  
			else if(g_NationalCode==3)
				m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT_SPN,IDB_MENU_STARTBT_0_SPN, 0, IDB_MENU_STARTBT_2_SPN);
			m_Bt_Start.Invalidate();
	}


	if(m_Interock_State || !g_bGenerator || !g_bDetector || !g_bIOcard)	
	{
		if(!g_bGenerator)
		{
			CMessageBoxDlg dlg;
			dlg.m_Message=g_MessageArr[42];//"X-ray 발생장치와 연결되지 않았습니다. (E1020)";
			dlg.DoModal();
		}
		if(!g_bDetector)
		{
			CMessageBoxDlg dlg;
			dlg.m_Message=g_MessageArr[43];//"디텍터와 연결되지 않았습니다. (E2001)";
			dlg.DoModal();
		}
		if(!g_bIOcard)
		{
			CMessageBoxDlg dlg;	
			dlg.m_Message=g_MessageArr[44];//"IO card와 통신할 수 없습니다. (E3001)";
			dlg.DoModal();
		}
		return;		//260810 주석해제
	}
 	

	if(g_TalMask_Bright == 3)
	{
		NSE_DETECTOR_WIDTH=768;
		NSE_VAR_SIZE=768;
		NSE_WIDTH_SIZE=768;
		NSE_TRIGER_HEIGHT=1;
		NSE_TRIGER_FRAME_SIZE=768;
	}
/*	else
	{
		NSE_DETECTOR_WIDTH=1024;
		NSE_VAR_SIZE=1024;
		NSE_WIDTH_SIZE=1024;
		NSE_TRIGER_HEIGHT=1;
		NSE_TRIGER_FRAME_SIZE=1024;
	}*/

	g_Height = NSE_VAR_SIZE;
	g_Width = NSE_WIDTH_SIZE;

	pView->m_Height = NSE_TRIGER_HEIGHT;
	BmInfo->bmiHeader.biHeight = NSE_VAR_SIZE;
	BmInfo->bmiHeader.biSizeImage = NSE_VAR_SIZE * NSE_WIDTH_SIZE;
	BmInfo->bmiHeader.biWidth = NSE_WIDTH_SIZE;
	pView->m_DTImage.PutImgWidth(NSE_DETECTOR_WIDTH);
	pView->m_DTImage.PutImgHeight(NSE_TRIGER_HEIGHT);
		

	if(g_ProductName=="")	
	{
		SelectProduct();
		return;
	}



	if(g_XrayState==0)
	{
		Checkhard();
		
		g_XrayState=1;
		DrawXrayState();
		
		if(g_bGenerator)
		{
			if(NSE_XRAY_KIND==3)
			{
			//	g_XrayState=0; // 추가 20161219
				OnDigitalInPut_All();

				if(!OnTurnonXrayState())
				{
					if(!CheckXrayError(1, 0))
					{
						CMessageBoxDlg dlg;
						dlg.m_Message=g_MessageArr[45];//"X-Ray 스위치가 On으로 되었는지 확인하시고 리셋 버튼을 누룬후 다시 시작해주세요. (E1031)";
						dlg.DoModal();
					}

					OnTurnoffXray();
					g_XrayState=0;
					DrawXrayState();
					return;
				}
			}
			else
			{
				m_GComm->setClear();
				int status = m_GComm->status();
				if(status!=0)
				{
					if(!OnTurnonXrayState())
					{
						int status = m_GComm->status();
						if(!CheckXrayError(1, status))
						{
							CMessageBoxDlg dlg;
							dlg.m_Message=g_MessageArr[45];//"X-Ray 스위치가 On으로 되었는지 확인하시고 리셋 버튼을 누룬후 다시 시작해주세요. (E1031)";
							dlg.DoModal();
						}
							
						OnTurnoffXray();
						g_XrayState=0;
						DrawXrayState();
						return;
					}
				}
			}			
		}
 
		CInitailDlg d_Progress;
	//	bool m_useNomal = false;

		if(!g_bUseNormal)
		{
			if(g_Model_Select == 1)  //20190327 
			{
				if (g_SelDetector == 0) //c5
				{
					char rev[REV_BUF_SIZE];
					pView->m_Detector.SendCommandA("[EL,W,0]", rev);
					Sleep(200);
					pView->m_Detector.SendCommandA("[SD,W,1]", rev);
					Sleep(200);

					pView->m_DTImage.PutTimeOut(60000 * 60 * 100);
					Sleep(200);
					pView->m_Detector.PutCmdTimeOut(60000 * 60 * 100);
					Sleep(200);
				}
				else //c01b
				{
					// lineTrigger mode에서 normalization을 해야됨

					//char s;
					//pView->_xcommand.SendAscCmd("[EL,W,0,0]", &s);		// external line trigger off
					pView->_xcommand.SetPara(XPARA_EN_LINE_TRIGGER, 0);

					Sleep(200);
					//pView->_xcommand.SendAscCmd("[OM,W,0,0]", &s);		// continuous mode
					pView->_xcommand.SetPara(XPARA_OPE_MODE, 0);
					Sleep(200);

					pView->_xcommand.ExecutePara(XPARA_SAVE_PARA);
			
					Sleep(200);
				}
			}
			else
			{
				if (g_SelDetector == 1) //c01b
				{
					pView->_xcommand.SetPara(XPARA_EN_LINE_TRIGGER, 0);
					Sleep(200);
					pView->_xcommand.SetPara(XPARA_OPE_MODE, 0);
					Sleep(200);

					pView->_xcommand.ExecutePara(XPARA_SAVE_PARA);
				}

			}
		
			 d_Progress.Create();
			 d_Progress.ShowWindow(SW_SHOW);
		
			 d_Progress.StepIt(0);

			 d_Progress.m_Text=g_MessageArr[46];//"감도보정 중...";

			 d_Progress.StepIt(20);
			//20251029 lim 개선 (컨베이어 탐지 모드 추가 예정)
			 if (g_PreproUse == 0) //0일때 Auto 모드 , 1일때 수동 모드
				 OnAutoTeaching();
			 else
				 g_Margin2 = g_PreproValue;

			 Sleepms(1000);
		
			 if (g_SelDetector == 0)
				 OnNormalization();
			 else
				 OnNormalizationC01B();

			 if(g_Model_Select == 1)  //20190327
			 {
				 if (g_SelDetector == 0)
				 {
					 char rev[REV_BUF_SIZE];
					 float fSpeed = (float)g_DTScanspeed;
					 int fIntTime = int((0.4 / fSpeed) * 100000);

					 std::ostringstream oss;
					 oss << "[SK,W," << std::hex << fIntTime << "]"; // 16진수 형식으로 변환
					 std::string s_speed = oss.str();               // 최종 문자열 생성
					 char* buffer = new char[s_speed.length() + 1]; // 동적 메모리 할당
					 std::strcpy(buffer, s_speed.c_str());          // 문자열 복사

					 pView->m_Detector.SendCommandA(buffer, rev);

					 Sleep(200);
					 pView->m_Detector.SendCommandA("[SK,S]", rev);
					 Sleep(200);
					 pView->m_Detector.SendCommandA("[SD,W,3]", rev);
					 Sleep(200);
					 pView->m_Detector.SendCommandA("[EL,W,1]", rev);
					 Sleep(200);
				 }
				 else
				 {
					 uint64_t llsize;
					 float fSpeed = (float)g_DTScanspeed;

					 pView->_xcommand.GetPara(XPARA_PIXEL_SIZE, llsize);
					 float fsize = (float)llsize / 10.0; 


					 float fIntTime = (fsize / fSpeed) * 100000;
					 //float fIntTime = 1000;

					 pView->_xcommand.SetPara(XPARA_INT_TIME, (long)fIntTime);
					 Sleep(200);

					 pView->_xcommand.SetPara(XPARA_OPE_MODE, 2);
					 Sleep(200);
					 pView->_xcommand.SetPara(XPARA_EN_LINE_TRIGGER, 1);
					 Sleep(200);
					 pView->_xcommand.SetPara(XPARA_LINE_TRIGGER_MODE, 0);
					 Sleep(200);

					 pView->_xcommand.ExecutePara(XPARA_SAVE_PARA);
				 }
			 }
			 d_Progress.StepIt(30);
		}
		else
		{
			g_bUseNormal=false;
		}


		

		Sleepms(100);
		g_AirconCycle = 0;
		OnStartConveyor();
		
		if(NSE_XRAY_KIND!=3)
		{
			b_ThreadGen = true;
			AfxBeginThread(ThreadGenerator, (LPVOID)this);
		}
		
		d_Progress.StepIt(60);
		pView->OnMenuStart();
		
		Sleepms(100);
		OnWriteLog(100);

		g_StopKind = 0; //20201120 통신적용

		g_XrayState=2;
		DrawXrayState();
		
		d_Progress.StepIt(100); 	
		d_Progress.m_Text=g_MessageArr[47];//"감도보정이 완료 되었습니다."; 	
		d_Progress.DestroyWindow();

	
	}

}




void CCtrlPanelView::OnBnClickedMenuStop()
{
	if(g_XrayState==1 || g_XrayState==2)
	{
		g_DispArea=false;
		g_XrayState=0;
		DrawXrayState();

		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
		pView->OnMenuStop();

		

		b_ThreadGen=false;

		if(g_bGenerator)
		{
			if(NSE_XRAY_KIND==3)
				OnTurnoffXray();
			else
			{
				m_GComm->setXRay(false);
				OnTurnoffXray();
			}
		}
	
		OnStopConveyor();
		

		OnWriteLog(200);
		if(pView->b_ConveyorOn)
		{
			if(g_NationalCode==0)
				m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT,IDB_MENU_STARTBT_0, 0, IDB_MENU_STARTBT_2); 
			else if(g_NationalCode==1)
				m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT_ENG,IDB_MENU_STARTBT_0_ENG, 0, IDB_MENU_STARTBT_2_ENG);  
			else if(g_NationalCode==2)
				m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT_CHI,IDB_MENU_STARTBT_0_CHI, 0, IDB_MENU_STARTBT_2_CHI);  
			else if(g_NationalCode==3)
				m_Bt_Start.LoadBitmaps(IDB_MENU_STARTBT_SPN,IDB_MENU_STARTBT_0_SPN, 0, IDB_MENU_STARTBT_2_SPN);  
			m_Bt_Start.Invalidate();
			pView->b_ConveyorOn=false;
		}
	}
	else
		return;
}

void CCtrlPanelView::OnBnClickedMenuResetcount()
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (g_bAdmin)
	{
		DisplayCount(0, 0, 0, 0);
		CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
		pView->OnCountresetbt();
	}

}

void CCtrlPanelView::Initial_Delay(void)
{
	CInitailDlg d_Progress;
	d_Progress.Create();
	d_Progress.ShowWindow(SW_SHOW);
	d_Progress.m_Text="";
	d_Progress.StepIt(0);
	Sleepms(1000);

	d_Progress.StepIt(1);
	d_Progress.m_Text=g_MessageArr[48];//"시스템 UI 초기화 진행중...";
	d_Progress.StepIt(10);
	Sleepms(1000);
		
	d_Progress.m_Text=g_MessageArr[49];//"발생장치 초기화 진행중...";
	d_Progress.StepIt(20);
	Sleepms(1000);
		
	d_Progress.m_Text=g_MessageArr[50];//"디텍터 초기화 진행중...";
	d_Progress.StepIt(40);
	Sleepms(1000);

	d_Progress.m_Text=g_MessageArr[51];//"I/O 시스템 초기화 진행중...";
	d_Progress.StepIt(50);
	Sleepms(1000);

	d_Progress.m_Text=g_MessageArr[52];//"사용자 설정 확인 진행중...";
	d_Progress.StepIt(80);
	Sleepms(1000);

	d_Progress.m_Text=g_MessageArr[53];//"시스템 가동 준비중...";
	d_Progress.StepIt(100);
	Sleepms(3000);
	d_Progress.DestroyWindow();
}


void CCtrlPanelView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent==0)
	{
		KillTimer(0);
		
		Initial_Delay();

		OnReadPassword();
	
		Initial_IOCard();
		
		Initial_Monoblock();
	
		SetTimer(10, 1000, NULL);
		
		if(NSE_XRAY_KIND == 3)
			XrayWarmUp_Rtec();
		else
			XrayWarmUp();
		
		KillTimer(10);

		CheckPartsUseTime();
		
		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
		pView->SetTimer(0, 10, NULL);
		
		SelectProduct();

		d_InterockDlg.Create();
		d_InterockDlg.ShowWindow(SW_HIDE);

		//20201120 통신모듈 추가
		COleDateTime stime=COleDateTime::GetCurrentTime();
		m_StrDate.Format(_T("%04d-%02d-%02d"), stime.GetYear(),stime.GetMonth(),stime.GetDay());


		SetTimer(1, 1200, NULL);


		pView->OnGetcount(); //20191202
		
		/*
		//20191202
		b_ThreadSC = true;
		AfxBeginThread(ThreadClient, (LPVOID)this);
		*/

		OnWriteLog(200);
	
		

	}

	if(nIDEvent==1)
	{

		if(g_iXrayOn>0)
		{ //kds20090709   X-ray가 가동중 off 될경우
			OnWriteLog(g_iXrayOn);
			g_iXrayOn=0;
			OnBnClickedMenuStop();
		}
		if(g_bIOcard)
		{
			bool temp_state = m_Interock_State;

			USHORT	gwValue = 2;

			if(NSE_IO_OUPUT_PORT == 2)
			{//신규 i/o
				if(g_bReject)
				{
					OnDigitalInPut_All();
					gwValue = (g_Word_In % 128);

					if((NSE_XRAY_KIND==3) && (g_XrayState>0))
					{//r-tec일 경우 발생장치 에러 확인용
						if(g_Bit_In[12] == 1)
						{//알람
							g_iXrayOn = 1003; //kds20090709
							CMessageBoxDlg dlg;
							dlg.m_Message=g_MessageArr[54];//"X-ray 발생장치 이상발생(High mA)! 관리자에게 문의하세요. (E1004)";
							dlg.DoModal();
							WriteXrayLog(_T("-Alarm\r\n"));
							g_StopKind = 1; //20191202
						}
						if(g_Bit_In[15] == 1)
						{//과열
							g_iXrayOn = 1001; //kds20090709
							CMessageBoxDlg dlg;
							dlg.m_Message=g_MessageArr[55];//"X-ray 발생장치 이상발생(과열)! 메인 전원을 내린 후 기기를 식힌 후 사용하세요. (E1002)";
							dlg.DoModal();
							WriteXrayLog(_T("-Temp High\r\n"));
							g_StopKind = 1; //20191202
						}
						if(g_Bit_In[13] == 0 && g_XrayState==2)
						{
							g_iXrayOn = 1009; //kds20090709
							CMessageBoxDlg dlg;
							dlg.m_Message=g_MessageArr[131];
							dlg.DoModal();
							WriteXrayLog(_T("-X-ray Off\r\n"));
							g_StopKind = 1; //20191202
						}
					}

					if(g_Model_Kind == "XIS-100") 
					{
						if (g_Bit_In[1] == 0) {//비상스위치
							m_Interock_State=true;
							d_InterockDlg.What_Interock=1;
							g_StopKind = 3; //20191202
						}
						else if (g_Bit_In[4] == 0) {//후문
							m_Interock_State=true;
							d_InterockDlg.What_Interock=63;
							g_StopKind = 3; //20191202

						}
						else if(g_Bit_In[7]==1 && g_HandsensorLU==1){
							gwValue = 96;
							m_Interock_State=true;
							d_InterockDlg.What_Interock=81;
							b_HandDetect=true;
							g_Bit_Out[5]=1;
							OnDigitalOutPut_All();
							g_StopKind = 3; //20191202
						}
						else if(g_Bit_In[9]==1 && g_HandsensorRU==1)
						{
							gwValue = 98;
							m_Interock_State=true;
							d_InterockDlg.What_Interock=80;
							b_HandDetect=true;
							g_Bit_Out[5]=1;
							OnDigitalOutPut_All();
							g_StopKind = 3; //20191202
						}
						else 
						{
							m_Interock_State=false;
							d_InterockDlg.ShowWindow(SW_HIDE);
							if(b_HandDetect)
							{
								b_HandDetect=false;
								CMessageBoxDlg dlg;
								dlg.m_Mode=1;
								dlg.m_Message=g_MessageArr[118];//_T("손감지 센서가 감지되었습니다. 손을 기기안으로 투입할 경우 매우 위험합니다.");//"컨베이어 이상발생!! 컨베이어 확인해 주세요.";
								dlg.DoModal();
								g_Bit_Out[5]=0;
								OnDigitalOutPut_All();
							}
						//	g_StopKind = 0;
						}
						if(g_Bit_In[11]==1 )
						{
							OnBnClickedMenuStop();
							CMessageBoxDlg dlg;
							dlg.m_Mode=1;
							dlg.m_Message=g_MessageArr[56];//"컨베이어 이상발생!! 컨베이어 확인해 주세요.";
							dlg.DoModal();
						}
					}
					else
					{
						if(g_Bit_In[0] == 0)
						{//앞문
							m_Interock_State=true;
							d_InterockDlg.What_Interock=0;
							if(g_AlamUse == 0)
							{
								g_Bit_Out[5]=1;
							}
							g_Bit_Out[11]=1;
							OnDigitalOutPut_All();
							g_StopKind = 3; //20191202
						}
						else if (g_Bit_In[1] == 0) 
						{
							m_Interock_State=true;
							d_InterockDlg.What_Interock=1;
							if(g_AlamUse == 0)
							{
								g_Bit_Out[5]=1;
							}
							g_Bit_Out[11]=1;
							OnDigitalOutPut_All();
							g_StopKind = 3; //20191202
						}
						else if (g_Bit_In[2] == 0) {//비상스위치
							m_Interock_State=true;
							d_InterockDlg.What_Interock=3;
							if(g_AlamUse == 0)
							{
								g_Bit_Out[5]=1;
							}
							g_Bit_Out[11]=1;
							OnDigitalOutPut_All();
							g_StopKind = 3; //20191202
						}
						else if (g_Bit_In[3] == 0) {//모니터커버
							m_Interock_State=true;
							d_InterockDlg.What_Interock=31;
							if(g_AlamUse == 0)
							{
								g_Bit_Out[5]=1;
							}
							g_Bit_Out[11]=1;
							OnDigitalOutPut_All();
							g_StopKind = 3; //20191202
						}
						else if (g_Bit_In[4] == 0) {//후문
							m_Interock_State=true;
							d_InterockDlg.What_Interock=63;
							if(g_AlamUse == 0)
							{
								g_Bit_Out[5]=1;
							}
							g_Bit_Out[11]=1;
							OnDigitalOutPut_All();
							g_StopKind = 3; //20191202
						}
						else if (g_Bit_In[5] == 0) {//확장터널좌
							m_Interock_State=true;
							d_InterockDlg.What_Interock=7;
							if(g_AlamUse == 0)
							{
								g_Bit_Out[5]=1;
							}
							g_Bit_Out[11]=1;
							OnDigitalOutPut_All();
							g_StopKind = 3; //20191202
							gwValue = NSE_INTEROCK_EX_LEFT;
						}
						else if (g_Bit_In[6] == 0) {//확장터널우
							m_Interock_State=true;
							d_InterockDlg.What_Interock=15;
							if(g_AlamUse == 0)
							{
								g_Bit_Out[5]=1;
							}
							g_Bit_Out[11]=1;
							OnDigitalOutPut_All();
							g_StopKind = 3; //20191202
							gwValue = NSE_INTEROCK_EX_RIGHT;
						}
						else if(g_Bit_In[7]==1 && g_HandsensorLU==1){
							gwValue = 96;
							m_Interock_State=true;
							d_InterockDlg.What_Interock=81;
							b_HandDetect=true;
							if(g_AlamUse == 0)
							{
								g_Bit_Out[5]=1;
							}
							g_Bit_Out[11]=1;
							OnDigitalOutPut_All();
							g_StopKind = 3; //20191202
						}
						else if(g_Bit_In[8]==1 && g_HandsensorLD==1)
						{
							gwValue = 97;
							m_Interock_State=true;
							d_InterockDlg.What_Interock = 81;
							b_HandDetect=true;
							if(g_AlamUse == 0)
							{
								g_Bit_Out[5]=1;
							}
							g_Bit_Out[11]=1;
							OnDigitalOutPut_All();
							g_StopKind = 3; //20191202
						}
						else if(g_Bit_In[9]==1 && g_HandsensorRU==1)
						{
							gwValue = 98;
							m_Interock_State=true;
							d_InterockDlg.What_Interock=80;
							b_HandDetect=true;
							if(g_AlamUse == 0)
							{
								g_Bit_Out[5]=1;
							}
							g_Bit_Out[11]=1;
							OnDigitalOutPut_All();
							g_StopKind = 3; //20191202
						}
						else if(g_Bit_In[10]==1 && g_HandsensorRD==1)
						{
							gwValue = 99;
							m_Interock_State=true;
							d_InterockDlg.What_Interock=80;
							b_HandDetect=true;
							if(g_AlamUse == 0)
							{
								g_Bit_Out[5]=1;
							}
							g_Bit_Out[11]=1;
							OnDigitalOutPut_All();
							g_StopKind = 3; //20191202
						}	
						else
						{
							d_InterockDlg.ShowWindow(SW_HIDE);
							if(b_HandDetect)
							{
								b_HandDetect=false;
								CMessageBoxDlg dlg;
								dlg.m_Mode=1;
								dlg.m_Message=g_MessageArr[118];_T("손감지 센서가 감지되었습니다. 손을 기기 안으로 투입할 경우 매우 위험합니다.");
								dlg.DoModal();
								g_Bit_Out[5]=0;
								OnDigitalOutPut_All();
							}
							if (g_AlamUse == 0 && g_Continu_Ng == 0 && m_Interock_State == true)
							{
								g_Bit_Out[5] = 0;
								g_Bit_Out[11] = 0;
								OnDigitalOutPut_All();
								//	g_StopKind = 0;
							}

							m_Interock_State = false;
						}
						if(g_Bit_In[11]==1 && g_AirconCycle==0)
						{
							g_AirconCycle=1;
							CMessageBoxDlg dlg;
							dlg.m_Mode=1;
							dlg.m_Message=g_MessageArr[57];//"에어컨 필터에 이물질이 많습니다. 필터 청소 및 교체 해주세요.";
							dlg.DoModal();
						}
					}

				}
			}
			else
			{ //신규 어드밴텍 IO Version
				//20200525
				//IO 변경
				//후문 : 63 -> 27
				//확장터널좌 : 7 -> 3
				//화장터널우 : 15 -> 11


				ErrorCode error = m_instantDi->Read(0,g_portCount,g_portReadData);
				if(error)
					return;

				gwValue = (int)g_portReadData[0];

				if(gwValue==0){//앞문
					m_Interock_State=true;
					d_InterockDlg.What_Interock=0;
					g_StopKind = 3; //20191202
				}else if (gwValue == 1) {//비상좌
					m_Interock_State=true;
					d_InterockDlg.What_Interock=1;
					g_StopKind = 3; //20191202
				}
			/*	else if (gwValue == 3) {//비상우
					m_Interock_State=true;
					d_InterockDlg.What_Interock=3;
				}*/
				else if (gwValue == NSE_INTEROCK_BACK) {//후문
					m_Interock_State=true;
					d_InterockDlg.What_Interock=63;
					g_StopKind = 3; //20191202
				}
				else if (gwValue == NSE_INTEROCK_EX_LEFT) {//확장터널좌
					m_Interock_State=true;
					d_InterockDlg.What_Interock=7;
					g_StopKind = 3; //20191202
				}
				else if (gwValue == NSE_INTEROCK_EX_RIGHT) {//확장터널우
					m_Interock_State=true;
					d_InterockDlg.What_Interock=15;
					g_StopKind = 3; //20191202

				}
			/*	else if (gwValue == NSE_INTEROCK_MONITOR) {//모니터커버
					m_Interock_State=true;
					d_InterockDlg.What_Interock=31;
				}*/
				else {
					m_Interock_State=false;
					d_InterockDlg.ShowWindow(SW_HIDE);
				//	g_StopKind = 0; //20191202
				}	

				if(NSE_XRAY_KIND==3)  //20190403 IO->어드벤택 + X-ray->Spellman이 아닐때 X-ray 체크
				{
					OnDigitalInPut_All();
					//gwValue = (g_Word_In % 128);	//260810 gwValue 덮어씌워져 인터락 값이 이상해짐

					if((NSE_XRAY_KIND==3) && (g_XrayState>0))
					{//r-tec일 경우 발생장치 에러 확인용
						if(g_Bit_In[12] == 1)
						{//알람
							g_iXrayOn = 1003; //kds20090709
							CMessageBoxDlg dlg;
							dlg.m_Message=g_MessageArr[54];//"X-ray 발생장치 이상발생(High mA)! 관리자에게 문의하세요. (E1004)";
							dlg.DoModal();
							WriteXrayLog(_T("-Alarm\r\n"));
						}
						if(g_Bit_In[15] == 1)
						{//과열
							g_iXrayOn = 1001; //kds20090709
							CMessageBoxDlg dlg;
							dlg.m_Message=g_MessageArr[55];//"X-ray 발생장치 이상발생(과열)! 메인 전원을 내린 후 기기를 식힌 후 사용하세요. (E1002)";
							dlg.DoModal();
							WriteXrayLog(_T("-Temp High\r\n"));
						}
						if(g_Bit_In[13] == 0 && g_XrayState==2)
						{
							g_iXrayOn = 1009; //kds20090709
							CMessageBoxDlg dlg;
							dlg.m_Message=g_MessageArr[131];
							dlg.DoModal();
							WriteXrayLog(_T("-X-ray Off\r\n"));
						}
					}
				}
			}

			if(m_Interock_State)
			{
				OnBnClickedMenuStop();
				d_InterockDlg.DrawInterock();
				d_InterockDlg.ShowWindow(SW_SHOW);

				if(!temp_state)
					OnWriteLog(gwValue);
			}
		}
	}

	if(nIDEvent==10)
	{
		if(g_bIOcard)
		{
			USHORT	gwValue = 2;
			if(NSE_IO_OUPUT_PORT == 2)
			{
				OnDigitalInPut_All();
			}
			else
			{  
				//20200525
				ErrorCode error = m_instantDi->Read(0,g_portCount,g_portReadData);
				if(error)
					return;

				gwValue = (int)g_portReadData[0];
				//20200525
	/*			m_ptDioReadPortByte.port = 0;
				m_ptDioReadPortByte.value = (USHORT far *)&gwValue;
				if(DRV_DioReadPortByte(m_DriverHandle,	(LPT_DioReadPortByte)&m_ptDioReadPortByte)!=0)
				{
					return;
				} */
			}
			
			if(NSE_IO_OUPUT_PORT == 2)//신규 i/o
			{
				if(g_Model_Kind == "XIS-100") 
				{
					if(g_Bit_In[1]==0) 
						m_Interock_State=true;
					else if (g_Bit_In[4]==0)
						m_Interock_State=true;
					else if(g_Bit_In[7]==1 && g_HandsensorLU==1)
						m_Interock_State=true;
					else if(g_Bit_In[9]==1 && g_HandsensorRU==1)
						m_Interock_State=true;
					else 
						m_Interock_State=false;

				}
				else
				{
					if(g_Bit_In[0] == 0) //앞문
						m_Interock_State=true;
					else if(g_Bit_In[1]==0)//비상좌
						m_Interock_State=true;
					else if(g_Bit_In[2]==0)//비상우
						m_Interock_State=true;
					else if(g_Bit_In[3]==0)//모니터커버
						m_Interock_State=true;
					else if(g_Bit_In[4]==0)//후문
						m_Interock_State=true;
					else if(g_Bit_In[5]==0 && g_KindModel==4)//확장터널좌
						m_Interock_State=true;
					else if(g_Bit_In[6]==0 && g_KindModel==4)//확장터널우
						m_Interock_State=true;
					else if(g_Bit_In[7]==1 && g_HandsensorLU==1)
						m_Interock_State=true;
					else if(g_Bit_In[8]==1 && g_HandsensorLD==1)
						m_Interock_State=true;
					else if(g_Bit_In[9]==1 && g_HandsensorRU==1)
						m_Interock_State=true;
					else if(g_Bit_In[10]==1 && g_HandsensorRD==1)
						m_Interock_State=true;
					else 
						m_Interock_State=false;
				}
			}
			else //기존보드
			{
				if(gwValue==0)//앞문
					m_Interock_State=true;
				else if (gwValue == 1)//비상좌
					m_Interock_State=true;
				else if (gwValue == 3)//비상우
					m_Interock_State=true;
				else if (gwValue == NSE_INTEROCK_BACK)//후문
					m_Interock_State=true;
				else if (gwValue == NSE_INTEROCK_EX_LEFT)//확장터널좌
					m_Interock_State=true;
				else if (gwValue == NSE_INTEROCK_EX_RIGHT)//확장터널우
					m_Interock_State=true;
				else if (gwValue == NSE_INTEROCK_MONITOR)//모니터커버
					m_Interock_State=true;
				else 
					m_Interock_State=false;
			}
			
		}
	}

	CFormView::OnTimer(nIDEvent);
}

void CCtrlPanelView::Initial_IOCard(void)
{




	//시뮬레이션 용

	//g_bIOcard = false;
	//return;

	if(NSE_XRAY_KIND==3 || NSE_IO_OUPUT_PORT==2)
	{
		for(int a=0; a<16; a++)
		{
			g_Bit_In[a]=0;
			g_Bit_Out[a]=0;
		}

		g_bReject = true;
		if(!ModbusComm.IsOpen())
		{
			if(!ModbusComm.Open(3, 38400))
			{
				if(!ModbusComm.Open(3, 38400))
				{
				//AfxMessageBox("입출력보드 통신연결 실패!");
				CMessageBoxDlg dlg;
				dlg.m_Message=g_MessageArr[58];//"입출력보드와 통신할 수 없습니다. (E3001-1)";
				dlg.DoModal();
				g_bIOcard=false;
				OnWriteLog(400);
				}
			}
			
			if(g_bIOcard)
			{
				if(!OnDigitalOutPut_All_Init())
				{
					//AfxMessageBox("입출력보드 통신테스트 불량!");
					CMessageBoxDlg dlg;
					dlg.m_Message=g_MessageArr[59];//"입출력보드와 통신할 수 없습니다. (E3001-2)";
					dlg.DoModal();
					g_bIOcard=false;
					OnWriteLog(400);
				}
				else
				{
					BYTE DeviceID = (BYTE)1;
					WORD StartAddr = (WORD)0;
					WORD WriteData = 0;
					ModbusComm.WriteSingleRegister(DeviceID, StartAddr, WriteData);
					StartAddr = (WORD)1;
					ModbusComm.WriteSingleRegister(DeviceID, StartAddr, WriteData);
					StartAddr = (WORD)2;
					ModbusComm.WriteSingleRegister(DeviceID, StartAddr, WriteData);
					StartAddr = (WORD)3;
					ModbusComm.WriteSingleRegister(DeviceID, StartAddr, WriteData);
					StartAddr = (WORD)10;
					WriteData = (WORD)(g_HandCycle);
					ModbusComm.WriteSingleRegister(DeviceID,StartAddr, WriteData);
					StartAddr = (WORD)11;
					WriteData = (WORD)(g_HandCycle);
					ModbusComm.WriteSingleRegister(DeviceID, StartAddr, WriteData);
					g_bIOcard=true;
				}
			}
		}
		else
		{
			//"입출력보드가 통신중 입니다. (E3001-3)"
			CMessageBoxDlg dlg;
			dlg.m_Message=g_MessageArr[60];//"입출력보드가 통신중 입니다.! (E3001-3)";
			dlg.DoModal();
			g_bIOcard=false;
			OnWriteLog(400);
		}
	}
	if(NSE_IO_OUPUT_PORT!=2)
	{
		//20200525	
		m_instantDi = InstantDiCtrl::Create();
		m_instantDo = InstantDoCtrl::Create();
		Array<DeviceTreeNode>* sptedDevices = m_instantDi->getSupportedDevices();
		Array<DeviceTreeNode>* sptedDevices2 = m_instantDo->getSupportedDevices();
		if(sptedDevices->getCount() == 0 || sptedDevices2->getCount() == 0)
		{
			CMessageBoxDlg dlg;
			dlg.m_Message=g_MessageArr[61];//"IO card와 통신할 수 없습니다. (E3001)";
			dlg.DoModal();
		
			g_bIOcard=false;
			OnWriteLog(400);
			return;
		}

		DeviceInformation devInfo(1);
//		DeviceInformation devInfo2(1);

		m_instantDi->setSelectedDevice(devInfo);
		m_instantDo->setSelectedDevice(devInfo);
		g_portCount = m_instantDi->getFeatures()->getPortCount();
		g_portReadData = new byte[g_portCount];
		
	
		g_portWriteData = 0;
		m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);	//260810 NSE_IO_OUPUT_PORT로 변경
		//20200525


		
		/*
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		if (m_DriverHandle!=NULL)
			DRV_DeviceClose( &m_DriverHandle);
		if ((m_ErrCde = DRV_DeviceOpen(0, &m_DriverHandle))!=0) 
		{
			//"IO card와 통신할 수 없습니다. (E3001)"
			CMessageBoxDlg dlg;
			dlg.m_Message=g_MessageArr[61];//"IO card와 통신할 수 없습니다. (E3001)";
			dlg.DoModal();
		
			g_bIOcard=false;
			OnWriteLog(400);
			return;
		}

		USHORT      gwMask = 0xff;                  // mask
		USHORT      gwData = 0x0;                  // output data


		m_ptDioWritePortByte.port  = NSE_IO_OUPUT_PORT;
		m_ptDioWritePortByte.mask  = gwMask;
		m_ptDioWritePortByte.state = gwData;
		if((m_ErrCde = DRV_DioWritePortByte(m_DriverHandle,
			(LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
		{
		//	DRV_GetErrorMessage(m_ErrCde,(LPSTR)m_szErrMsg);
			return;
		}
		*/

		/*
		if(g_KindModel==1 || g_KindModel==3 || g_KindModel==4)
		{
			m_ptDioWritePortByte.port  = 0;
		}
		else if(g_KindModel==2)
		{
			m_ptDioWritePortByte.port  = 1;
		}

		m_ptDioWritePortByte.mask  = gwMask;
		m_ptDioWritePortByte.state = gwData;

		if((DRV_DioWritePortByte(m_DriverHandle,(LPT_DioWritePortByte)&m_ptDioWritePortByte))!=0)
		{
		//	DRV_GetErrorMessage(m_ErrCde,(LPSTR)m_szErrMsg);
		}*/
	}
}

void CCtrlPanelView::Initial_Monoblock(void)
{
	//R-tec
	g_bGenerator=true;
	if(NSE_XRAY_KIND==3)
	{
		if(g_bIOcard)
		{	
			g_Bit_Out[14]=1;
			OnDigitalOutPut_All();
			Sleepms(500);
			g_Bit_Out[14]=0;
			OnDigitalOutPut_All();
			OnDigitalInPut_All();
			CheckXrayError(1, 0);
		}	
		else
		{
			g_bGenerator=false;
		}
	}
	else
	{
		m_GComm=new CGeneratorComm("COM2");
		if(g_bGenerator)
		{
			m_GComm->setClear();
			int result=m_GComm->status();

			if(result!=0)
			{
				CheckXrayError(1, result);
				g_bGenerator=true;
			}
		}
		else
		{
			g_bGenerator=false;
			delete m_GComm;
			m_GComm = NULL;
		}
	}

	if(!g_bGenerator)
	{
		OnWriteLog(300);
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[62];//"X-ray 발생장치와 연결되지 않았습니다. (E1020)";
		dlg.DoModal();
	}

}

bool CCtrlPanelView::CheckXrayError(int Mode, int Result)
{
	int LogNum=0;
	CString m_ErrorName=_T("");
	CString m_ErrorMsg=_T("");
	if(NSE_XRAY_KIND==3)
	{
		if(g_Bit_In[12] == 1)
		{
			LogNum=1003;
			m_ErrorName=_T("Alarm");
			m_ErrorMsg=g_MessageArr[63];//_T("X-ray 발생장치 이상발생(Alam)! 관리자에게 문의하세요. (E1004)");
			g_StopKind = 1; //20191202
		}
		if(g_Bit_In[14] == 1)
		{
			LogNum=1009;
			m_ErrorName=_T("Interlock Open");
			m_ErrorMsg=g_MessageArr[64];//_T("X-ray 발생장치 이상발생(Interlock Open)!  관리자에게 문의하세요. (E1011)");
			g_StopKind = 1; //20191202
		}
		if(g_Bit_In[15] == 1)
		{	  
			LogNum=1001;
			m_ErrorName=_T("Temp High");
			m_ErrorMsg=g_MessageArr[65];//_T("X-ray 발생장치 이상발생(과열)! 메인 전원을 내린 후 기기를 식힌 후 사용하세요. (E1002)");
			g_StopKind = 1; //20191202
		}
		if(LogNum!=0)
		{
			CMessageBoxDlg dlg;  
			dlg.m_Message=m_ErrorMsg;
			dlg.DoModal();
			if(Mode==1)
				OnWriteLog(LogNum);
			WriteXrayLog(m_ErrorName);
			return false;
		}
	}
	else if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
	{
		if(Result == 1)
		{
			m_ErrorName=_T("Temp High");
			m_ErrorMsg=g_MessageArr[65];//_T("X-ray 발생장치 이상발생(과열)! 메인 전원을 내린 후 기기를 식힌 후 사용하세요. (E1002)");
			LogNum=1001;
			g_StopKind = 1; //20191202
		}
		else if(Result == 2)
		{
			//status = ARC_FAULT;
			m_ErrorName=_T("Arc Fault");
			m_ErrorMsg=g_MessageArr[66];//"X-ray 발생장치 이상발생(Arc Fault)! 관리자에게 문의하세요. (E1003)";
			LogNum=1002;
			g_StopKind = 1; //20191202
		}
		else if(Result == 3)
		{
			//status = HIGH_MA;
			m_ErrorName=_T("High mA");
			m_ErrorMsg=g_MessageArr[67];//"X-ray 발생장치 이상발생(High mA)! 관리자에게 문의하세요. (E1004)";
			LogNum=1003;
			g_StopKind = 1; //20191202
		}
		else if(Result == 4)
		{	
			//status = LOW_MA;
			m_ErrorName=_T("Low mA");
			m_ErrorMsg=g_MessageArr[68];//"X-ray 발생장치 이상발생(Low mA)! 관리자에게 문의하세요. (E1005)";	
			LogNum=1004;
			g_StopKind = 1; //20191202
		}
		else if(Result == 5)
		{
			//status = LOW_KV;
			m_ErrorName=_T("Low kV");
			m_ErrorMsg=g_MessageArr[69];//"X-ray 발생장치 이상발생(Low kV)! 관리자에게 문의하세요. (E1006)";
			LogNum=1005;
			g_StopKind = 1; //20191202
		}
		else if(Result == 6)
		{
			//status = HIGH_KV;
			m_ErrorName=_T("High kV");
			m_ErrorMsg=g_MessageArr[70];//"X-ray 발생장치 이상발생(High kV)! 관리자에게 문의하세요. (E1007)";
			LogNum=1006;
			g_StopKind = 1; //20191202
		}
		else if(Result == 7)
		{
			//status = WATCHDOG_TIMER_EXPIRED;
			m_ErrorName=_T("Watchdog Timer Expired");
			m_ErrorMsg=g_MessageArr[71];//"X-ray 발생장치 이상발생(Watchdog Timer Expired)! 관리자에게 문의하세요. (E1008)";
			LogNum=1007;
			g_StopKind = 1; //20191202
		}
		else if(Result == 8)
		{
			//status = POWER_LIMIT;
			m_ErrorName=_T("Power Limit");
			m_ErrorMsg=g_MessageArr[72];//"X-ray 발생장치 이상발생(Power Limit)! 관리자에게 문의하세요. (E1009)";
			LogNum=1008;
			g_StopKind = 1; //20191202
		}

		if(LogNum!=0)
		{
			CMessageBoxDlg dlg;  
			dlg.m_Message=m_ErrorMsg;
			dlg.DoModal();
			if(Mode==1)
				OnWriteLog(LogNum);
			g_iXrayOn=LogNum;
			WriteXrayLog(m_ErrorName);
			return false;
		}
	}	
	else if(NSE_XRAY_KIND==1)
	{
		if(Result == 1) //X0
		{
			m_ErrorName=_T("X0 Regulation");
			m_ErrorMsg=g_MessageArr[73];//"X-ray 발생장치 이상발생(X0) - Regulation Error (E1010)" ;
			LogNum=1001;
		}
		else if(Result == 2) //X1
		{
			m_ErrorName=_T("X1 Interlock Open");
			m_ErrorMsg=g_MessageArr[74];//"인터록 확인(X1) - Interlock Open (E1011)";
			LogNum=1002;
		}
		else if(Result == 3) //X2
		{
			m_ErrorName=_T("X2 Cathode Over kV");
			m_ErrorMsg=g_MessageArr[75];//"X-ray 발생장치 이상발생(X2) - Cathode Over kV Fault (E1012)";
			LogNum=1003;
		}
		else if(Result == 4)//X3
		{	
			m_ErrorName=_T("X3 Anode Over kV");
			m_ErrorMsg=g_MessageArr[76];//"X-ray 발생장치 이상발생(X3) - Anode Over kV Fault (E1014)";
			LogNum=1004;
		}
		else if(Result == 5)//X4
		{
			m_ErrorName=_T("X4 Temp High");
			m_ErrorMsg=g_MessageArr[77];//"X-ray 발생장치 이상발생(X4) - Over Temperature Fault (E1015)";
			LogNum=1005;
		}
		else if(Result == 6)//X5
		{
			m_ErrorName=_T("X5 Arc Fault");
			m_ErrorMsg=g_MessageArr[78];//"X-ray 발생장치 이상발생(X5) - Arc Fault (E1016)";
			LogNum=1006;
		}
		else if(Result == 7)//X6
		{
			m_ErrorName=_T("X6 Over Current");
			m_ErrorMsg=g_MessageArr[79];//"X-ray 발생장치 이상발생(X6) - Over Current Fault (E1017)";
			LogNum=1007;
		}
		else if(Result == 8)//X7
		{
			m_ErrorName=_T("X7 Power Limit");
			m_ErrorMsg=g_MessageArr[80];//"X-ray 발생장치 이상발생(X7) - Power Limit Fault (E1018)";
			LogNum=1008;
		}
		else if(Result == 9)//X8
		{
			m_ErrorName=_T("X8 Over Voltage");
			m_ErrorMsg=g_MessageArr[81];//"X-ray 발생장치 이상발생(X8) - Over Voltage Fault (E1019)";
			LogNum=1009;
		}
		
		if(LogNum!=0)
		{
			CMessageBoxDlg dlg;  
			dlg.m_Message=m_ErrorMsg;
			dlg.DoModal();
			if(Mode==1)
				OnWriteLog(LogNum);
			g_iXrayOn=LogNum;
			WriteXrayLog(m_ErrorName);
			return false;
		}
	}

	return true;
}


void CCtrlPanelView::OnWriteLog(int Num)
{
	CString strdate, strtime, strtime1;
	CTime time;
	time = CTime::GetCurrentTime();
	strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());		
	g_Today.Format(_T("%04d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());	

	strtime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), time.GetYear(),time.GetMonth(),time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	strtime1.Format(_T("%04d-%02d-%02d/%02d:%02d:%02d"), time.GetYear(),time.GetMonth(),time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	CString strQuery;
	bool g_bOpenDB=true;
	if(Num == 100)
	{	
		//20130103
		g_bOpenDB=true;

		Init_Mysql();

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			mysql_query(&m_mysql, "set names euckr");

			strQuery.Format(_T("Insert INTO intellisense_db2(strDate, strStartTime, strName)") _T("VALUES('%s', '%s', '%s')"),
				strdate, strtime, g_ProductName);

			if (mysql_query(&m_mysql, CStringA(strQuery))) {
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
		}

		Close_Mysql();

		//if(!m_pSet_2.Open_DB(g_StrDirPath[2]))		
		//{	
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[2], g_StrDirPath_copy[2]);
		//	if(m_pSet_2.Open_DB(g_StrDirPath[2]))		
		//		g_bOpenDB=true;
		//}
		//if(g_bOpenDB)
		//{
		//	g_PGStartTime = strtime; //20191202
		//	strQuery.Format(_T("Insert INTO Running_Info(strDate, strStartTime, strName)") _T("VALUES('%s', '%s', '%s')"),
		//		strdate, strtime, g_ProductName);
		//	m_pSet_2.Insert_DB(strQuery);
		//	if(m_pSet_2.b_IsOpenedDB)
		//		m_pSet_2.Close_DB();
		//}		
		CStdioFile fileString;
		if(fileString.Open(_T("D:\\Database\\Temp.dat"),
			CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
		{
			fileString.SeekToEnd();
			fileString.WriteString(strtime1);
			fileString.Close();
		}
	}
	else if(Num == 200)
	{
		CStdioFile fileString;
		CString tempstarttime;
		if(fileString.Open(_T("D:\\Database\\Temp.dat"), 
			CStdioFile::modeRead | CStdioFile::shareDenyNone))
		{
			fileString.ReadString(tempstarttime);
			fileString.Close();
			fileString.Remove(_T("D:\\Database\\Temp.dat"));
		}
		else
			return;

		//20170426
		tempstarttime.Replace(_T(" "), _T(""));
		
		//20170215
		if(tempstarttime=="")
			return;

		
		//20170426
		tempstarttime.Replace(_T("/"), _T(" "));


		CString strStarttime;
		strStarttime.Format(_T("%s"), tempstarttime); 

		g_bOpenDB=true;
		
				Init_Mysql();

		strQuery.Format(_T("Update intellisense_db2 SET strStopTime = '%s' WHERE strStartTime = '%s'"), strtime, tempstarttime);
		if (mysql_query(&m_mysql, CStringA(strQuery))) {
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			CString strQuery;
			strQuery.Format(_T("Update intellisense_db2 SET strStopTime = '%s' WHERE strStartTime = '%s'"), strtime, tempstarttime);

			if (mysql_query(&m_mysql, CStringA(strQuery))) {
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
		}

		Close_Mysql();
		
		//if(!m_pSet_2.Open_DB(g_StrDirPath[2]))		
		//{	
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[2], g_StrDirPath_copy[2]);
		//	if(m_pSet_2.Open_DB(g_StrDirPath[2]))		
		//		g_bOpenDB=true;
		//}
		//if(g_bOpenDB)
		//{
		//	g_PGEndTime = strtime; //20191202
		//	strQuery.Format(_T("Update Running_Info SET strStopTime = '%s' WHERE strStartTime = '%s'"), 
		//		strtime, tempstarttime);
		//	m_pSet_2.Update_DB(strQuery);			
		//	if(m_pSet_2.b_IsOpenedDB)
		//		m_pSet_2.Close_DB();
		//}
		
		COleDateTime StopTime, StartTime, UsedTime_D, UsedTime_M;
		StopTime = COleDateTime(_wtoi(strtime.Left(4)),
			_wtoi(strtime.Mid(5).Left(2)),
			_wtoi(strtime.Mid(8).Left(2)),
			_wtoi(strtime.Mid(11).Left(2)),
			_wtoi(strtime.Mid(14).Left(2)),
			_wtoi(strtime.Mid(17).Left(2)));
		StartTime = COleDateTime(_wtoi(strStarttime.Left(4)),
			_wtoi(strStarttime.Mid(5).Left(2)),
			_wtoi(strStarttime.Mid(8).Left(2)),
			_wtoi(strStarttime.Mid(11).Left(2)),
			_wtoi(strStarttime.Mid(14).Left(2)),
			_wtoi(strStarttime.Mid(17).Left(2)));

		COleDateTimeSpan RunTime = StopTime - StartTime;
		CString m_strRun;
		m_strRun.Format(_T("%02d:%02d:%02d"),(RunTime.GetDays()*24)+RunTime.GetHours(), RunTime.GetMinutes(), RunTime.GetSeconds());
	
		//20170426
		if((RunTime.GetDays()*24)+RunTime.GetHours()>=1000)
		{
			return;
		}

		g_bOpenDB=true;
		Init_Mysql();

		if (!g_Connecting)
		{
			AfxMessageBox(_T("실패"));
		}
		else
		{
			MYSQL_RES* m_Res;
			MYSQL_ROW row;

			mysql_query(&m_mysql, "set names euckr");

			CString strSQL;
			strSQL.Format(_T("SELECT * FROM intellisense_db3"));
			if (mysql_query(&m_mysql, CStringA(strSQL)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}

			m_Res = mysql_store_result(&m_mysql);

			if ((row = mysql_fetch_row(m_Res)) == NULL)
			{
				strQuery.Format(_T("Insert INTO intellisense_db3(strRunTime)") _T("VALUES('%s')"), m_strRun);
				if (mysql_query(&m_mysql, CStringA(strQuery)))
				{
					CString error(mysql_error(&m_mysql));
					AfxMessageBox(error);
				}
			}
			else
			{
				CString varValue(row[3]);
				if (varValue == "" || varValue == "0")
				{
					strQuery.Format(_T("Update intellisense_db3 SET strRunTime = '%s'"), m_strRun);
					if (mysql_query(&m_mysql, CStringA(strQuery)))
					{
						CString error(mysql_error(&m_mysql));
						AfxMessageBox(error);
					}
				}
				else
				{
					COleDateTimeSpan ts1(0, 0, _wtoi(varValue.Right(5).Left(2)), _wtoi(varValue.Right(2)));
					COleDateTimeSpan ts2(0, 0, _wtoi(m_strRun.Right(5).Left(2)), _wtoi(m_strRun.Right(2)));
					COleDateTimeSpan ComputeUsedTime = ts1 + ts2;
					CString Run;
					Run.Format(_T("%02d:%02d:%02d"), _wtoi(varValue.Right(varValue.GetLength()).Left(varValue.GetLength() - 5))
						+ _wtoi(m_strRun.Right(m_strRun.GetLength()).Left(m_strRun.GetLength() - 5)) + ComputeUsedTime.GetHours(),
						ComputeUsedTime.GetMinutes(), ComputeUsedTime.GetSeconds());
					strQuery.Format(_T("Update intellisense_db3 SET strRunTime = '%s'"), Run);
					if (mysql_query(&m_mysql, CStringA(strQuery)))
					{
						CString error(mysql_error(&m_mysql));
						AfxMessageBox(error);
					}
				}
			}
			mysql_free_result(m_Res);
		}

		Close_Mysql();
		//if(!m_pSet_3.Open_DB(g_StrDirPath[3]))		
		//{
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[3], g_StrDirPath_copy[3]);
		//	if(m_pSet_3.Open_DB(g_StrDirPath[3]))		
		//		g_bOpenDB=true;
		//}
		//if(g_bOpenDB)
		//{
		//	CString strSQL;
		//	strSQL.Format(_T("SELECT * FROM Monoblock_Info"));
		//	if(m_pSet_3.Select_DB(strSQL))
		//	{
		//		if(m_pSet_3.IsEOF())
		//		{
		//			strQuery.Format(_T("Insert INTO Monoblock_Info(strRunTime)") _T("VALUES('%s')"), m_strRun);
		//			m_pSet_3.Insert_DB(strQuery);
		//		}
		//		else
		//		{
		//			CString varValue = _T("");
		//			m_pSet_3.ChangeRsChar(_T("strRunTime"), varValue);	
		//			if(varValue == "" || varValue == "0")
		//			{
		//				strQuery.Format(_T("Update Monoblock_Info SET strRunTime = '%s'"), m_strRun);
		//				m_pSet_3.Update_DB(strQuery);	
		//			}
		//			else
		//			{
		//				COleDateTimeSpan ts1(0, 0, _wtoi(varValue.Right(5).Left(2)), _wtoi(varValue.Right(2)));
		//				COleDateTimeSpan ts2(0, 0, _wtoi(m_strRun.Right(5).Left(2)), _wtoi(m_strRun.Right(2)));
		//				COleDateTimeSpan ComputeUsedTime = ts1 + ts2;
		//				CString Run;
		//				Run.Format(_T("%02d:%02d:%02d"),_wtoi(varValue.Right(varValue.GetLength()).Left(varValue.GetLength()-5))
		//					+_wtoi(m_strRun.Right(m_strRun.GetLength()).Left(m_strRun.GetLength()-5))+ComputeUsedTime.GetHours(),
		//					ComputeUsedTime.GetMinutes(), ComputeUsedTime.GetSeconds());	
		//				strQuery.Format(_T("Update Monoblock_Info SET strRunTime = '%s'"), Run);
		//				m_pSet_3.Update_DB(strQuery);			
		//			}
		//		}
		//	}
		//	if(m_pSet_3.b_IsOpenedDB)
		//		m_pSet_3.Close_DB();
		//}

		g_bOpenDB=true;
		Init_Mysql();

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			MYSQL_RES* m_Res;
			MYSQL_ROW row;

			mysql_query(&m_mysql, "set names euckr");

			CString strSQL;
			strSQL.Format(_T("SELECT * FROM intellisense_db5"));
			if (mysql_query(&m_mysql, CStringA(strSQL)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}

			m_Res = mysql_store_result(&m_mysql);

			if ((row = mysql_fetch_row(m_Res)) == NULL)
			{
				strQuery.Format(_T("Insert INTO intellisense_db5(strRunTime)") _T("VALUES('%s')"), m_strRun);
				if (mysql_query(&m_mysql, CStringA(strQuery)))
				{
					CString error(mysql_error(&m_mysql));
					AfxMessageBox(error);
				}
			}
			else
			{
				CString varValue(row[3]);
				if (varValue == "" || varValue == "0")
				{
					strQuery.Format(_T("Update intellisense_db5 SET strRunTime = '%s'"), m_strRun);
					if (mysql_query(&m_mysql, CStringA(strQuery)))
					{
						CString error(mysql_error(&m_mysql));
						AfxMessageBox(error);
					}
				}
				else
				{
					COleDateTimeSpan ts1(0, 0, _wtoi(varValue.Right(5).Left(2)), _wtoi(varValue.Right(2)));
					COleDateTimeSpan ts2(0, 0, _wtoi(m_strRun.Right(5).Left(2)), _wtoi(m_strRun.Right(2)));
					COleDateTimeSpan ComputeUsedTime = ts1 + ts2;
					CString Run;
					Run.Format(_T("%02d:%02d:%02d"), _wtoi((LPCTSTR)varValue.Right(varValue.GetLength()).Left(varValue.GetLength() - 5))
						+ _wtoi((LPCTSTR)m_strRun.Right(m_strRun.GetLength()).Left(m_strRun.GetLength() - 5)) + ComputeUsedTime.GetHours(),
						ComputeUsedTime.GetMinutes(), ComputeUsedTime.GetSeconds());
					strQuery.Format(_T("Update intellisense_db5 SET strRunTime = '%s'"), Run);
					if (mysql_query(&m_mysql, CStringA(strQuery)))
					{
						CString error(mysql_error(&m_mysql));
						AfxMessageBox(error);
					}
				}
			}
			mysql_free_result(m_Res);
		}

		Close_Mysql();

		//if(!m_pSet_5.Open_DB(g_StrDirPath[5]))		
		//{
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[5], g_StrDirPath_copy[5]);
		//	if(m_pSet_5.Open_DB(g_StrDirPath[5]))		
		//		g_bOpenDB=true;
		//}
		//if(g_bOpenDB)
		//{
		//	CString strSQL;
		//	strSQL.Format(_T("SELECT * FROM Detector_Info"));
		//	if(m_pSet_5.Select_DB(strSQL))
		//	{
		//		if(m_pSet_5.IsEOF())
		//		{
		//			strQuery.Format(_T("Insert INTO Detector_Info(strRunTime)") _T("VALUES('%s')"), m_strRun);
		//			m_pSet_5.Insert_DB(strQuery);
		//		}
		//		else
		//		{
		//			CString varValue = _T("");
		//			m_pSet_5.ChangeRsChar(_T("strRunTime"), varValue);	
		//			if(varValue == "" || varValue == "0")
		//			{
		//				strQuery.Format(_T("Update Detector_Info SET strRunTime = '%s'"), m_strRun);
		//				m_pSet_5.Update_DB(strQuery);	
		//			}
		//			else
		//			{
		//				COleDateTimeSpan ts1(0, 0, _wtoi(varValue.Right(5).Left(2)), _wtoi(varValue.Right(2)));
		//				COleDateTimeSpan ts2(0, 0, _wtoi(m_strRun.Right(5).Left(2)), _wtoi(m_strRun.Right(2)));
		//				COleDateTimeSpan ComputeUsedTime = ts1 + ts2;
		//				CString Run;
		//				Run.Format(_T("%02d:%02d:%02d"),_wtoi((LPCTSTR)varValue.Right(varValue.GetLength()).Left(varValue.GetLength()-5))
		//					+_wtoi((LPCTSTR)m_strRun.Right(m_strRun.GetLength()).Left(m_strRun.GetLength()-5))+ComputeUsedTime.GetHours(),
		//					ComputeUsedTime.GetMinutes(), ComputeUsedTime.GetSeconds());	
		//				strQuery.Format(_T("Update Detector_Info SET strRunTime = '%s'"), Run);
		//				m_pSet_5.Update_DB(strQuery);			
		//			}
		//		}
		//	}
		//	if(m_pSet_5.b_IsOpenedDB)
		//		m_pSet_5.Close_DB();
		//}

	}
	else if(Num == 300)
	{
		CString strtext;
		strtext.Format(g_MessageArr[82]);//_T("모노블럭과 연결되지 않음"));
		g_bOpenDB=true;

		Init_Mysql();

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			mysql_query(&m_mysql, "set names euckr");

			strQuery.Format(_T("Insert INTO intellisense_db4(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
				strdate, strtext, strtime);
			if (mysql_query(&m_mysql, CStringA(strQuery)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
		}

		Close_Mysql();
		//if(!m_pSet_4.Open_DB(g_StrDirPath[4]))		
		//{
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[4], g_StrDirPath_copy[4]);
		//	if(m_pSet_4.Open_DB(g_StrDirPath[4]))		
		//		g_bOpenDB=true;
		//}
		//if(g_bOpenDB)
		//{
		//	strQuery.Format(_T("Insert INTO Monoblock_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
		//		strdate, strtext, strtime);
		//	m_pSet_4.Insert_DB(strQuery);
		//	if(m_pSet_4.b_IsOpenedDB)
		//		m_pSet_4.Close_DB();
		//}		
	}
	else if(Num == 400)
	{
		CString strtext;
		strtext.Format(g_MessageArr[83]);//_T("I/O Card와 연결되지 않음"));

		g_bOpenDB=true;

		Init_Mysql();

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			mysql_query(&m_mysql, "set names euckr");

			strQuery.Format(_T("Insert INTO intellisense_db8(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
				strdate, strtext, strtime);

			if (mysql_query(&m_mysql, CStringA(strQuery)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
		}

		Close_Mysql();
		//if(!m_pSet_8.Open_DB(g_StrDirPath[8]))		
		//{
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[8], g_StrDirPath_copy[8]);
		//	if(m_pSet_8.Open_DB(g_StrDirPath[8]))		
		//		g_bOpenDB=true;
		//}
		//if(g_bOpenDB)
		//{
		//	strQuery.Format(_T("Insert INTO IO_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
		//		strdate, strtext, strtime);
		//	m_pSet_8.Insert_DB(strQuery);
		//	if(m_pSet_8.b_IsOpenedDB)
		//		m_pSet_8.Close_DB();
		//}	
	}
	else if(Num == 500)
	{
		CString strtext;
		strtext.Format(_T("%s(%s)"), g_MessageArr[84]/*_T("제품등록(%s)")*/, g_ProductName);
		g_bOpenDB=true;
		Init_Mysql();

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			mysql_query(&m_mysql, "set names euckr");

			strQuery.Format(_T("Insert INTO intellisense_db7(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
				strdate, strtext, strtime);
			if (mysql_query(&m_mysql, CStringA(strQuery)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
		}

		Close_Mysql();
		//if(!m_pSet_7.Open_DB(g_StrDirPath[7]))		
		//{
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[7], g_StrDirPath_copy[7]);
		//	if(m_pSet_7.Open_DB(g_StrDirPath[7]))		
		//		g_bOpenDB=true;
		//}
		//if(g_bOpenDB)
		//{
		//	strQuery.Format(_T("Insert INTO Computer_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
		//		strdate, strtext, strtime);
		//	m_pSet_7.Insert_DB(strQuery);
		//	if(m_pSet_7.b_IsOpenedDB)
		//		m_pSet_7.Close_DB();
		//}	
	}
	else if(Num >= 0 && Num <= 99)
	{
		CString strtext;

		if(Num==0)
			strtext.Format(g_MessageArr[85]);//_T("인터락 발생(앞문 열림) (E4001)"));
		else if(Num==1)
			strtext.Format(g_MessageArr[86]);//_T("인터락 발생(좌측 비상스위치 눌림) (E4002)"));
		//else if(Num==3)	//260810 로그 번호 중복으로 우측 비상스위치 로그 남는 문제로 주석처리
		//	strtext.Format(g_MessageArr[87]);//_T("인터락 발생(우측 비상스위치 눌림) (E4003)"));
		else if(Num==NSE_INTEROCK_EX_LEFT)
			strtext.Format(g_MessageArr[88]);//_T("인터락 발생(확장 터널 좌측 열림) (E4004)"));
		else if(Num==NSE_INTEROCK_EX_RIGHT)
			strtext.Format(g_MessageArr[89]);//_T("인터락 발생(확장 터널 우측 열림) (E4005)"));
		else if(Num==NSE_INTEROCK_MONITOR) 
			strtext.Format(g_MessageArr[90]);//_T("인터락 발생(모니터문 열림) (E4006)"));
		else if(Num==NSE_INTEROCK_BACK)
			strtext.Format(g_MessageArr[91]);//_T("인터락 발생(후문 열림) (E4007)"));
		else if(Num==96)
			strtext.Format(g_MessageArr[92]);//_T("인터락 발생(좌측 상 손감지) (E4008)"));
		else if(Num==97)
			strtext.Format(g_MessageArr[93]);//_T("인터락 발생(좌측 하 손감지) (E4009)"));
		else if(Num==98)
			strtext.Format(g_MessageArr[94]);//_T("인터락 발생(우측 상 손감지) (E4010)"));
		else if(Num==99)
			strtext.Format(g_MessageArr[95]);//_T("인터락 발생(우측 하 손감지) (E4011)"));

		g_bOpenDB=true;

		Init_Mysql();

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			mysql_query(&m_mysql, "set names euckr");

			strQuery.Format(_T("Insert INTO intellisense_db8(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
				strdate, strtext, strtime);
			if (mysql_query(&m_mysql, CStringA(strQuery)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
		}

		Close_Mysql();

		//if(!m_pSet_8.Open_DB(g_StrDirPath[8]))		
		//{
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[8], g_StrDirPath_copy[8]);
		//	if(m_pSet_8.Open_DB(g_StrDirPath[8]))		
		//		g_bOpenDB=true;
		//}
		//if(g_bOpenDB)
		//{
		//	strQuery.Format(_T("Insert INTO IO_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
		//		strdate, strtext, strtime);
		//	m_pSet_8.Insert_DB(strQuery);
		//	if(m_pSet_8.b_IsOpenedDB)
		//		m_pSet_8.Close_DB();
		//}	
	}
	else if(Num >= 1001 && Num <= 1009) 
	{
		CString strtext;
		if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2 || NSE_XRAY_KIND==3)
		{
			switch(Num)
			{
				//과열
				case 1001: 
					strtext.Format(g_MessageArr[96]);//_T("모노블럭 이상발생(과열) (E1002)"));
					break;
				//Arc Fault
				case 1002: 
					strtext.Format(g_MessageArr[97]);//_T("모노블럭 이상발생(Arc Fault) (E1003)"));
					break;
				//High mA
				case 1003: 
					strtext.Format(g_MessageArr[98]);//_T("모노블럭 이상발생(High mA) (E1004)"));
					break;
				//Low mA
				case 1004: 
					strtext.Format(g_MessageArr[99]);//_T("모노블럭 이상발생(Low mA) (E1005)"));
					break;
				//Low kV
				case 1005: 
					strtext.Format(g_MessageArr[100]);//_T("모노블럭 이상발생(Low kV) (E1006)"));
					break;
				//High kV
				case 1006: 
					strtext.Format(g_MessageArr[101]);//_T("모노블럭 이상발생(High kV) (E1007)"));
					break;
				//Watchdog Timer Expired
				case 1007: 
					strtext.Format(g_MessageArr[102]);//_T("모노블럭 이상발생(Watchdog) (E1008)"));
					break;
				//Power Limit
				case 1008: 
					strtext.Format(g_MessageArr[103]);//_T("모노블럭 이상발생(Power Limit) (E1009)"));
					break;
				case 1009:   //kds20090709
					strtext.Format(g_MessageArr[104]);//_T("X-ray OFF (InterLock)"));
					break;
			}
		}
		else
		{
			switch(Num)
			{
				case 1001: 
					strtext.Format(g_MessageArr[105]);//_T("Regulation (E1010)"));
					break;
				case 1002: 
					strtext.Format(g_MessageArr[106]);//_T("Interlock Open (E1011)"));
					break;
				case 1003: 
					strtext.Format(g_MessageArr[107]);//_T("Cathode Over KV Fault (E1012)"));
					break;
				case 1004: 
					strtext.Format(g_MessageArr[108]);//_T("Anode Over KV Fault (E1014)"));
					break;
				case 1005: 
					strtext.Format(g_MessageArr[109]);//_T("Over Temperature Fault (E1015)"));
					break;
				case 1006: 
					strtext.Format(g_MessageArr[110]);//_T("Arc Fault (E1016)"));
					break;
				case 1007: 
					strtext.Format(g_MessageArr[111]);//_T("Over Current Fault (E1017)"));
					break;
				case 1008: 
					strtext.Format(g_MessageArr[112]);//_T("Power Limit Fault (E1018)"));
					break;
				case 1009:   
					strtext.Format(g_MessageArr[113]);//_T("Over Voltage Fault (E1019)"));
					break;
				case 1010:   
					strtext.Format(g_MessageArr[114]);//_T("X-ray OFF (InterLock)"));
					break;
			}
		}

		g_bOpenDB=true;
		Init_Mysql();

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			mysql_query(&m_mysql, "set names euckr");

			strQuery.Format(_T("Insert INTO intellisense_db4(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
				strdate, strtext, strtime);
			if (mysql_query(&m_mysql, CStringA(strQuery)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
		}

		Close_Mysql();
		//if(!m_pSet_4.Open_DB(g_StrDirPath[4]))		
		//{
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[4], g_StrDirPath_copy[4]);
		//	if(m_pSet_4.Open_DB(g_StrDirPath[4]))		
		//		g_bOpenDB=true;
		//}
		//if(g_bOpenDB)
		//{
		//	strQuery.Format(_T("Insert INTO Monoblock_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
		//		strdate, strtext, strtime);
		//	m_pSet_4.Insert_DB(strQuery);
		//	if(m_pSet_4.b_IsOpenedDB)
		//		m_pSet_4.Close_DB();
		//}
	}
	else if(Num == 600)
	{
			
		CString strtext;
		strtext.Format(g_MessageArr[115]/*_T("제품 추가 복사(%s)")*/, g_ProductName);
		g_bOpenDB=true;

		Init_Mysql();

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			mysql_query(&m_mysql, "set names euckr");

			strQuery.Format(_T("Insert INTO intellisense_db7(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
				strdate, strtext, strtime);
			if (mysql_query(&m_mysql, CStringA(strQuery)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
		}

		Close_Mysql();
		//if(!m_pSet_7.Open_DB(g_StrDirPath[7]))		
		//{	
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[7], g_StrDirPath_copy[7]);
		//	if(m_pSet_7.Open_DB(g_StrDirPath[7]))		
		//		g_bOpenDB=true;	
		//}
		//if(g_bOpenDB)
		//{
		//	strQuery.Format(_T("Insert INTO Computer_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
		//		strdate, strtext, strtime);
		//	m_pSet_7.Insert_DB(strQuery);
		//	if(m_pSet_7.b_IsOpenedDB)
		//		m_pSet_7.Close_DB();
		//}		
	}
	else if(Num == 700)
	{
		CString strtext;
		strtext.Format(g_MessageArr[116]/*_T("제품명 변경(%s -> %s)")*/, m_PreName, g_ProductName);
		
		g_bOpenDB=true;

		Init_Mysql();

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			mysql_query(&m_mysql, "set names euckr");

			strQuery.Format(_T("Insert INTO intellisense_db7(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
				strdate, strtext, strtime);
			if (mysql_query(&m_mysql, CStringA(strQuery)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
		}

		Close_Mysql();
		//if(!m_pSet_7.Open_DB(g_StrDirPath[7]))		
		//{	
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[7], g_StrDirPath_copy[7]);
		//	if(m_pSet_7.Open_DB(g_StrDirPath[7]))		
		//		g_bOpenDB=true;	
		//}
		//if(g_bOpenDB)
		//{
		//	strQuery.Format(_T("Insert INTO Computer_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
		//		strdate, strtext, strtime);
		//	m_pSet_7.Insert_DB(strQuery);
		//	if(m_pSet_7.b_IsOpenedDB)
		//		m_pSet_7.Close_DB();
		//}		
	}
}


void CCtrlPanelView::XrayWarmUp_Rtec(void)
{
	int SleepTime=3000;
	
	if(!m_Interock_State && g_bGenerator && g_bIOcard)
	{	
		g_Bit_Out[14]=1; 
		OnDigitalOutPut_All(); 
		Sleep(500);
		g_Bit_Out[14]=0; 
		//알림 리셋
		g_Bit_Out[15]=1;
		OnDigitalOutPut_All();
		Sleepms(500);
		g_Bit_Out[14]=0;
		OnDigitalOutPut_All();
		Sleepms(1000);

		OnDigitalInPut_All();
		if(!CheckXrayError(1, 0))
			return;

		MSG msg;		
		CString m_Date;
		COleDateTime time=COleDateTime::GetCurrentTime();
		COleDateTime last;

		CStdioFile fileString;
		if(fileString.Open(_T("D:\\Database\\Warmup.dat"), CStdioFile::modeRead | CStdioFile::shareDenyNone))
		{
			fileString.ReadString(m_Date);
			last.SetDate(_wtoi(m_Date.Left(4)), _wtoi(m_Date.Mid(4).Left(2)), _wtoi(m_Date.Right(2)));
			COleDateTimeSpan dday;
				dday = time-last;		
				
			if((int)dday.GetTotalDays()<=0)
			{
				return;
			}
			else if((int)dday.GetTotalDays()<30)
				SleepTime=3000;
			else if((int)dday.GetTotalDays()<90)
				SleepTime=5000;
			else
				SleepTime=10000;
		}
		else
		{
			SleepTime=10000;
		}
		fileString.Close();
		CInitailDlg d_Progress;
		d_Progress.Create();
		d_Progress.ShowWindow(SW_SHOW);
		d_Progress.StepIt(0);
		d_Progress.m_Text=g_MessageArr[117];//_T("시스템 날짜를 계산중...");
		d_Progress.StepIt(5);
		d_Progress.m_Text=g_MessageArr[119]+_T("...");//_T("시스템 가동 준비중...");
		d_Progress.StepIt(10);
		if(NSE_XRAY_POWER==300000 && NSE_XRAY_KV==90)
        {
            int kv[10] = {  40,  50,  60,  70,  80,  90,  80,  70,  60,  50}; 
            int ma[10] = {1500,1600,1700,1800,1900,2000,1900,1800,1700,1600}; 

			for(int i=0; i<10; i++)
			{	
				OnAnalogOutPut_3(kv[i]);
				Sleepms(200);
				OnAnalogOutPut_4(ma[i]);
				Sleepms(200);
				g_Bit_Out[13]=1;
				OnDigitalOutPut_All();
				while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				if(m_Interock_State)
					break;
				Sleepms(SleepTime);
				
				d_Progress.m_Text.Format(_T("%s(%02d/10)..."), g_MessageArr[119], i+1);
				d_Progress.StepIt(10+(i+1)*8);
			}
		}
		else if(NSE_XRAY_POWER==300000)
		{
			int kv[10] = {  30,  35,  40,  45,  50,  55,  55,  60,  55,  50};
			int ma[10] = {1000,1500,2000,2500,3000,3500,4000,45000,4000,3500};
			for(int i=0; i<10; i++)
			{	
				OnAnalogOutPut_3(kv[i]);
				Sleepms(200);
				OnAnalogOutPut_4(ma[i]);
				Sleepms(200);
				g_Bit_Out[13]=1;
				OnDigitalOutPut_All();
				while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				if(m_Interock_State)
					break;
				Sleepms(SleepTime);
				
				d_Progress.m_Text.Format(_T("%s(%02d/10)..."), g_MessageArr[119], i+1);
				d_Progress.StepIt(10+(i+1)*8);
			}
		}

		OnTurnoffXray();

		m_Date.Format(_T("%04d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay());

		if(!m_Interock_State)
		{	
			if(fileString.Open(_T("D:\\Database\\Warmup.dat"), CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
			{
				fileString.WriteString(m_Date);
			}
			fileString.Close();
		}

		d_Progress.StepIt(95);
		Sleepms(200);
		d_Progress.m_Text=g_MessageArr[120];//_T("시스템 가동 준비가 완료되었습니다.");
		d_Progress.StepIt(100);
		Sleepms(500);
		d_Progress.DestroyWindow();
	}
}

void CCtrlPanelView::XrayWarmUp(void)
{
	int SleepTime=3000;
	
	if(!m_Interock_State && g_bGenerator)
	{	
		m_GComm->setClear();
		if(m_GComm->status()!=0)
		{
			return;
		}
		
		MSG msg;		
		CString m_Date=_T("");
		COleDateTime time=COleDateTime::GetCurrentTime();
		COleDateTime last;

		CStdioFile fileString;
		if(fileString.Open(_T("D:\\Database\\Warmup.dat"), CStdioFile::modeRead | CStdioFile::shareDenyNone))
		{
			fileString.ReadString(m_Date);
			last.SetDate(_wtoi(m_Date.Left(4)), _wtoi(m_Date.Mid(4).Left(2)), _wtoi(m_Date.Right(2)));
			COleDateTimeSpan dday;
				dday = time-last;		
				
			if((int)dday.GetTotalDays()<=0)
			{
				return;
			}
			else if((int)dday.GetTotalDays()<30)
				SleepTime=3000;
			else if((int)dday.GetTotalDays()<90)
				SleepTime=5000;
			else
				SleepTime=10000;
		}
		else
		{
			SleepTime=10000;
		}
		fileString.Close();

		CInitailDlg d_Progress;
		d_Progress.Create();
		d_Progress.ShowWindow(SW_SHOW);
		d_Progress.StepIt(0);
		d_Progress.m_Text=g_MessageArr[117];//"시스템 날짜를 계산중...";
		d_Progress.StepIt(5);
		d_Progress.m_Text=g_MessageArr[119]+_T("...");//"시스템 가동 준비중...";
		d_Progress.StepIt(10);
		//여기서부터 진행안되고있음
		if(NSE_XRAY_POWER>400000)
		{
			int kv[15] = {  60,  65,  70,  75,  80,  85,  90,  95, 100,  95,  85,  75,  70,  65,  60};
			int ma[15] = {1000,1300,1600,1900,2200,2500,2800,3100,3500,4000,4500,5000,5500,6000,6500};
			for(int i=0; i<15; i++)
			{
				if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
				{
					m_GComm->setKV(kv[i]);
					Sleepms(300);
					m_GComm->setMA(ma[i]);
					Sleepms(300);
					m_GComm->setXRay(true);
					Sleepms(1000);
					m_GComm->setXRay(true);
				}
				else 
				{
					m_GComm->setKV(kv[i]);
					Sleepms(300);
					m_GComm->setMA(ma[i]);
					Sleepms(300);
					m_GComm->setXRay(true);
					Sleepms(300);
					m_GComm->setXRay(true);
				}

				while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if(m_Interock_State)
					break;
				Sleepms(SleepTime);			
				d_Progress.m_Text.Format(_T("%s(%02d/15)..."), g_MessageArr[119], i+1);
				d_Progress.StepIt(10+(i+1)*5);
			}
		}
		else if(NSE_XRAY_POWER>200000)
		{
			int kv[10] = { 50, 60, 65,  70,  75,  80,  75,  70,  60,  50};
			int ma[10] = {500,700,900,1500,1700,2100,2500,2800,3200,4000};
			for(int i=0; i<10; i++)
			{	
				if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
				{
					m_GComm->setKV(kv[i]);
					Sleepms(300);
					m_GComm->setMA(ma[i]);
					Sleepms(300);
					m_GComm->setXRay(true);
					Sleepms(100);
					m_GComm->setXRay(true);
				}
				else 
				{
					m_GComm->setKV(kv[i]);
					Sleepms(300);
					m_GComm->setMA(ma[i]);
					Sleepms(300);
					m_GComm->setXRay(true);
					Sleepms(300);
					m_GComm->setXRay(true);
				}

				while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if(m_Interock_State)
					break;
				Sleepms(SleepTime);

				d_Progress.m_Text.Format(_T("%s(%02d/10)..."), g_MessageArr[119], i+1);
				d_Progress.StepIt(10+(i+1)*8);				
			}
		}
		else
		{
			int kv[10] = { 40, 45, 50, 55, 60,  65,  70,  70,  75,  80};
			int ma[10] = {250,400,550,700,850,1000,1100,1200,1200,1200};
			for(int i=0; i<10; i++)
			{	
				if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
				{
					m_GComm->setKV(kv[i]);
					Sleepms(300);
					m_GComm->setMA(ma[i]);
					Sleepms(300);
					m_GComm->setXRay(true);
					Sleepms(100);
					m_GComm->setXRay(true);
				}
				else 
				{
					m_GComm->setKV(kv[i]);
					Sleepms(300);
					m_GComm->setMA(ma[i]);
					Sleepms(300);
					m_GComm->setXRay(true);
					Sleepms(300);
					m_GComm->setXRay(true);
				}
				while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				if(m_Interock_State)
				{
					break;
				}
				Sleepms(SleepTime);
				d_Progress.m_Text.Format(_T("%s(%02d/10)..."), g_MessageArr[119], i+1);
				d_Progress.StepIt(10+(i+1)*8);
			}
		}

		OnTurnoffXray();

		m_Date.Format(_T("%04d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay());

		if(!m_Interock_State)
		{	
			if(fileString.Open(_T("D:\\Database\\Warmup.dat"),
				CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
			{
				fileString.WriteString(m_Date);
			}
			fileString.Close();
		}

		d_Progress.StepIt(95);
		Sleepms(200);
		d_Progress.m_Text=g_MessageArr[120];//_T("시스템 가동 준비가 완료되었습니다.");
		d_Progress.StepIt(100);
		Sleepms(500);
		d_Progress.DestroyWindow();
	}
}

void CCtrlPanelView::OnTurnoffXray(void)
{
	if(g_bGenerator)
	{
		if(NSE_XRAY_KIND==3)
		{
			if(g_bIOcard)
			{
				OnAnalogOutPut_3(0);
				Sleepms(200);
				OnAnalogOutPut_4(0);
				Sleepms(200);
				g_Bit_Out[13]=0;
				g_Bit_Out[15]=0;
				OnDigitalOutPut_All();
			}
		}
		else
		{
			if(m_GComm->isXRayOn())
				m_GComm->setXRay(false);
		}
	}
}


void CCtrlPanelView::OnTurnonXray(void)
{
	if(g_bGenerator)
	{
		if(NSE_XRAY_KIND==3)
		{
			g_Bit_Out[14]=1; 
			OnDigitalOutPut_All(); 
			Sleep(500);
			g_Bit_Out[14]=0; 
			//알림 리셋
			g_Bit_Out[15]=1;
			OnDigitalOutPut_All();
			Sleepms(1000);
			OnAnalogOutPut_3(g_Xray_kv);
			Sleepms(200);
			OnAnalogOutPut_4(g_Xray_ma);
			Sleepms(200);
			g_Bit_Out[13]=1;
			OnDigitalOutPut_All();
			Sleepms(1000);
		}
		else
		{
			if(!m_GComm->isXRayOn())
			{
				if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
				{
					m_GComm->setKV(g_Xray_kv);
					Sleepms(200);
					m_GComm->setMA(g_Xray_ma);
					Sleepms(200);
					m_GComm->setXRay(true);
					Sleepms(2000);// 5초정도의 여유시간을 주자
				}
				else 
				{
					m_GComm->setKV(g_Xray_kv);
					Sleepms(500);
					m_GComm->setMA(g_Xray_ma);
					Sleepms(500);
					m_GComm->setXRay(true);
					Sleepms(1000);
					m_GComm->setXRay(true);
				}
			}
		}
	}
}

void CCtrlPanelView::OnMenuExit(void)
{
	KillTimer(1);

	d_SpaceMsg->DestroyWindow();
	delete d_SpaceMsg;
	d_SpaceMsg=NULL;

	if(NSE_IO_OUPUT_PORT==2)
	{
		if(g_bIOcard)
		{
			for(int a=0; a<16; a++)
			{
				g_Bit_Out[a]=0;
			}
			OnDigitalOutPut_All();
			Sleepms(100);
			if(ModbusComm.IsOpen())
				ModbusComm.Close();
		}
	}
	else
	{
		if(g_bIOcard)
		{
			//20200525
			g_portWriteData = 0x00;
			ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);	//260810 NSE_IO_OUPUT_PORT로 변경
			if(error)
				return;
			//20200525
			/*
			USHORT      gwMask = 0xff;   
			USHORT      gwData = 0x0;           

			if(g_KindModel==1 || g_KindModel==3 || g_KindModel==4)
			{
				m_ptDioWritePortByte.port = 0;
			}
			else if(g_KindModel==2)
			{
				m_ptDioWritePortByte.port = 1;
			}
			m_ptDioWritePortByte.mask  = gwMask;
			m_ptDioWritePortByte.state = gwData;

			DRV_DioWritePortByte(m_DriverHandle, (LPT_DioWritePortByte)&m_ptDioWritePortByte);

			if(m_DriverHandle!= NULL)
				DRV_DeviceClose(&m_DriverHandle);	
				*/
			m_instantDo->Dispose();//20200525
			m_instantDi->Dispose();//20200525
		}

		if(NSE_XRAY_KIND!=3)
		{
			if(g_bGenerator)
			{
				delete m_GComm;
				m_GComm=NULL;
			}
		}
	}
}

void CCtrlPanelView::CheckPartsUseTime(void)
{
	CString mono, dt;
	CString strSQL;
	bool g_bOpenDB=true;
	Init_Mysql();

	if (!g_Connecting)
		AfxMessageBox(_T("실패"));
	else
	{
		MYSQL_RES* m_Res;
		MYSQL_ROW row;

		mysql_query(&m_mysql, "set names euckr");

		CString strQuery;

		strSQL.Format(_T("SELECT * FROM intellisense_db3"));
		if (mysql_query(&m_mysql, CStringA(strSQL)))
		{
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}

		m_Res = mysql_store_result(&m_mysql);
		if ((row = mysql_fetch_row(m_Res)) == NULL)
		{
			mono = _T("0");
		}
		else
		{
			CString varValue(row[3]);
			mono = varValue;
		}
		mysql_free_result(m_Res);
	}

	Close_Mysql();
	//if(!m_pSet_3.Open_DB(g_StrDirPath[3]))		
	//{
	//	g_bOpenDB=false;
	//	//RecoveryDatabase(g_StrDirPath[3], g_StrDirPath_copy[3]);
	//	if(m_pSet_3.Open_DB(g_StrDirPath[3]))	
	//		g_bOpenDB=true;
	//}

	//if(g_bOpenDB)
	//{
	//	strSQL.Format(_T("SELECT * FROM Monoblock_Info"));
	//	if(m_pSet_3.Select_DB(strSQL))
	//	{
	//		if(m_pSet_3.IsEOF())
	//			mono="0";
	//		else
	//			m_pSet_3.ChangeRsChar(_T("strRunTime"), mono);
	//	}
	//	if(m_pSet_3.b_IsOpenedDB)
	//		m_pSet_3.Close_DB();
	//}

	g_bOpenDB=true;
	Init_Mysql();

	if (!g_Connecting)
		AfxMessageBox(_T("실패"));
	else
	{
		MYSQL_RES* m_Res;
		MYSQL_ROW row;

		mysql_query(&m_mysql, "set names euckr");

		CString strQuery;

		strSQL.Format(_T("SELECT * FROM intellisense_db5"));
		if (mysql_query(&m_mysql, CStringA(strSQL)))
		{
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}

		m_Res = mysql_store_result(&m_mysql);
		if ((row = mysql_fetch_row(m_Res)) == NULL)
		{
			dt = _T("0");
		}
		else
		{
			CString varValue(row[3]);
			dt = varValue;
		}
		mysql_free_result(m_Res);
	}

	Close_Mysql();
	//if(!m_pSet_5.Open_DB(g_StrDirPath[5]))		
	//{
	//	g_bOpenDB=false;
	//	//RecoveryDatabase(g_StrDirPath[5], g_StrDirPath_copy[5]);
	//	if(m_pSet_5.Open_DB(g_StrDirPath[5]))	
	//		g_bOpenDB=true;
	//}

	//if(g_bOpenDB)
	//{
	//	strSQL.Format(_T("SELECT * FROM Detector_Info"));
	//	if(m_pSet_5.Select_DB(strSQL))
	//	{
	//		if(m_pSet_5.IsEOF())
	//			dt="0";
	//		else
	//			m_pSet_5.ChangeRsChar(_T("strRunTime"), dt);

	//		if(m_pSet_5.b_IsOpenedDB)
	//			m_pSet_5.Close_DB();
	//	}
	//}	
	
	if(_wtof(mono) > 15000.0)
	{
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[121];//_T("X-ray 발생장치 사용시간이 15,000시간을 초과 하였습니다. (E5004)");
		dlg.DoModal();
	}
	if(_wtof(dt) >15000.0)
	{
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[122];//_T("디텍터 사용시간이 15,000시간을 초과 하였습니다. (E5005)");
		dlg.DoModal();
	}
}

void CCtrlPanelView::SelectProduct(void)
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();

	CString OldName = g_ProductName;
	CSelectProduct dlg;
	int result = dlg.DoModal();

	if(result==IDOK)
	{
		OnLoadValue();

		if(OldName==g_ProductName)
			return;
		pView->m_Count_Total = 0;
		pView->m_Count_Soup = 0;
		pView->m_Count_OK = 0;
		pView->m_Count_NG = 0;
	
		DisplayCount(pView->m_Count_Total, pView->m_Count_OK, pView->m_Count_NG, pView->m_Count_Soup);

		//20201120 통신적용
		pView->m_Count_Total2 = 0;
		pView->m_Count_Soup2 = 0;
		pView->m_Count_OK2 = 0;
		pView->m_Count_NG2 = 0;
		//20201120 통신적용
		
		//260810 AI 초기화 제거
		//CString str;
		//CTime time;
		//time = CTime::GetCurrentTime();
		//str.Format(_T("%04d-%02d-%02d-%02d%02d%02d-%d  AI init....\r\n"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

		//CStdioFile fileString;
		//if (fileString.Open(_T("D:\\Init_AI.Log"),
		//	CStdioFile::modeWrite | CStdioFile::shareDenyNone))
		//{
		//	fileString.SeekToEnd();
		//	fileString.WriteString(str);
		//	fileString.Close();
		//}
		//else
		//{
		//	if (fileString.Open(_T("D:\\Init_AI.Log"),
		//		CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
		//	{
		//		fileString.WriteString(str);
		//		fileString.Close();
		//	}
		//}
		//Sleepms(500);
		//pView->Init_AI();//20260102 AI 추가 0210
		//Sleepms(100);
		//time = CTime::GetCurrentTime();
		//str.Format(_T("%04d-%02d-%02d-%02d%02d%02d-%d  AI 0 ok\r\n"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

		//
		//if (fileString.Open(_T("D:\\Init_AI.Log"),
		//	CStdioFile::modeWrite | CStdioFile::shareDenyNone))
		//{
		//	fileString.SeekToEnd();
		//	fileString.WriteString(str);
		//	fileString.Close();
		//}
		//else
		//{
		//	if (fileString.Open(_T("D:\\Init_AI.Log"),
		//		CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
		//	{
		//		fileString.WriteString(str);
		//		fileString.Close();
		//	}
		//}


	}
	else if(result==IDCANCEL)
	{	
		if(g_ProductName==_T(""))
		{
			OnLoadValue();
		}
	}
	else if(result== IDC_ADD_PRODUCT)
	{
		AutoSetup();
	}
	else if(result==IDC_MOD_PRODUCT)
	{
		CModifyProductName Namedlg;
		int re=Namedlg.DoModal();
		if(re==1)
		{
			m_PreName = g_ProductName;
			g_ProductName = Namedlg.m_NewName;
			OnWriteLog(800);

			CString path, pathnew;
			path.Format(_T("D:\\Database\\image\\%s.bmp"), m_PreName);
			pathnew.Format(_T("D:\\Database\\image\\%s.bmp"), g_ProductName);
			::MoveFile(path, pathnew);
		}
		CSelectProduct Seldlg;
		Seldlg.LoadValue(g_ProductName);
		OnLoadValue();
	}
}


void CCtrlPanelView::OnLoadValue(void)
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
	pView->OnLoadValue();
}


void CCtrlPanelView::DisplayCount(int c1, int c2, int c3, int c4)
{
	SetDlgItemInt(IDC_COUNT_TOTAL, c1, true);
	SetDlgItemInt(IDC_COUNT_OK, c2, true);
	SetDlgItemInt(IDC_COUNT_NG, c3, true);
	SetDlgItemInt(IDC_COUNT_SOUP, c4, true);
}


void CCtrlPanelView::OnReadPassword(void)
{
	SetFileAttributes(_T("D:\\Database\\password.dat"), FILE_ATTRIBUTE_NORMAL);
	CString m_strPass;
	CFileFind finder;
	CStdioFile m_File;

	if(finder.FindFile(_T("D:\\Database\\password.dat")))
	{
		if(m_File.Open(_T("D:\\Database\\password.dat"), CStdioFile::modeRead))
		{
			m_File.ReadString(m_strPass);
			if(m_strPass!=_T("0000"))
			{
				g_Password = m_strPass;
			}
			else
			{
				g_Password = "0000";
				m_strPass = g_Password;
				m_File.WriteString(m_strPass);
			}
		}
	}
	else
	{
		g_Password = "0000";
		m_File.Open(_T("D:\\Database\\password.dat"), CStdioFile::modeCreate | CStdioFile::modeWrite);
		m_File.WriteString(g_Password);
	}
	m_File.Close();
	
	SetFileAttributes(_T("D:\\Database\\password.dat"), FILE_ATTRIBUTE_HIDDEN);
}


void CCtrlPanelView::AutoSetup(void)
{
	CAutoSetup dlg;
	dlg.m_Step=false;
	dlg.m_CurrentPage=0;
	int result = dlg.DoModal();

	if(result==IDOK)
	{	
		if(g_ProductName!="")
		{
			OnWriteLog(500);
			CSelectProduct load;
			load.LoadValue(g_ProductName);
			OnLoadValue();
		}
	}
	//검사값 자동설정만 
	else if(result==33)
	{
		//EYH20110609
		Invalidate(false);
		if(g_ProductName!="")
		{
			OnWriteLog(500);
			CSelectProduct load;
			load.LoadValue(g_ProductName);
			OnLoadValue();
		}

		if(m_Interock_State || !g_bGenerator || !g_bDetector || !g_bIOcard)
		{
			g_bAutoSetup=false;
			return;
		}
		else
			OnBnClickedMenuStart();

		dlg.m_Step=1;
		//20151012 3->4
		dlg.m_CurrentPage=4;
		int result1 = dlg.DoModal();
		if(result1==IDOK)
		{
			g_bAutoSetup=false;
			if(g_ProductName!="")
			{
				CSelectProduct load;
				load.LoadValue(g_ProductName);
				OnLoadValue();
			}
		}
	}

	else if(result==44)
	{
		Invalidate(false);
		if(g_ProductName!="")
		{
			OnWriteLog(500);
			CSelectProduct load;
			load.LoadValue(g_ProductName);
			OnLoadValue();
		}

		if(m_Interock_State || !g_bGenerator || !g_bDetector || !g_bIOcard)
		{
			g_bXrayAutoSetup=false;
			return;
		}
		else
			OnBnClickedMenuStart();

		dlg.m_Step=2;
		dlg.m_CurrentPage=3;
		int result1 = dlg.DoModal();
		if(result1==IDOK)
		{
			g_bXrayAutoSetup=false;
			if(g_ProductName!="")
			{
				CSelectProduct load;
				load.LoadValue(g_ProductName);
				OnLoadValue();
			}
		}
	}
	else if(result==55)
	{
		Invalidate(false);
		if(g_ProductName!="")
		{
			OnWriteLog(500);
			CSelectProduct load;
			load.LoadValue(g_ProductName);
			OnLoadValue();
		}

		if(m_Interock_State || !g_bGenerator || !g_bDetector || !g_bIOcard)
		{
			g_bAutoSetup=false;
			g_bXrayAutoSetup=false;
			return;
		}
		else
			OnBnClickedMenuStart();
		dlg.m_Step=2;
		dlg.m_CurrentPage=3;
		int result1 = dlg.DoModal();

		if(result1==IDOK)
		{
			g_bXrayAutoSetup=false;
			if(g_ProductName!="")
			{
				CSelectProduct load;
				load.LoadValue(g_ProductName);
				OnLoadValue();
			}
		}
		else 
		{
			OnBnClickedMenuStop();
			return;
		}

		OnBnClickedMenuStop();

		if(m_Interock_State || !g_bGenerator || !g_bDetector || !g_bIOcard)
		{
			g_bAutoSetup=false;
			g_bXrayAutoSetup=false;
			return;
		}
		else
			OnBnClickedMenuStart();

		dlg.m_Step=1;
		dlg.m_CurrentPage=4;
		result1 = dlg.DoModal();
		if(result1==IDOK)
		{
			g_bAutoSetup=false;
			if(g_ProductName!="")
			{
				CSelectProduct load;
				load.LoadValue(g_ProductName);
				OnLoadValue();
			}
		}
	}
	else if(result==IDD_AUTOSETUP_0)
	{
		if(g_ProductName!="")
		{
			CSelectProduct load;
			load.LoadValue(g_ProductName);
			OnLoadValue();
		}
		else
			OnLoadValue();
	}
	else if(result==IDCANCEL)
	{
		if(g_ProductName!="")
		{
			CSelectProduct load;
			load.LoadValue(g_ProductName);
			OnLoadValue();
		}
		else
			OnLoadValue();

	}
}

void CCtrlPanelView::AutoXraySetup()
{
	g_bXrayAutoSetup=true;
	g_AutoXrayValue=0;
	g_AutoXrayCount=0;
	if(g_XrayState==1 || g_XrayState==2)
	{
		CMessageBoxDlg dlg;
		dlg.m_Mode=2;
		dlg.m_Message=g_MessageArr[123];//"X-ray 조사량 자동설정을 실행하기 위해서 검사가 재시작 됩니다. 계속하시려면 '확인' 버튼을 누르세요.";
		if(dlg.DoModal()==IDOK)
		{
			OnBnClickedMenuStop();
			Sleepms(500);
		}
		else
		{
			g_bXrayAutoSetup=false;
			return;
		}
	}
	
	OnBnClickedMenuStart();

	if(g_XrayState==1 || g_XrayState==2)
	{
		CSetupAutoXray dlg;
		int result = dlg.DoModal();
		if(result==IDOK)
		{
			CMessageBoxDlg dlg;
			dlg.m_Message=g_MessageArr[124];//"X-ray 조사량 자동설정이 끝났습니다. '닫기'버튼을 누르시면 검사가 정지됩니다.";
			dlg.DoModal();
		}
		else
		{
			CMessageBoxDlg dlg;
			dlg.m_Message=g_MessageArr[125];//"X-ray 조사량 자동설정이 취소되었습니다. '닫기'버튼을 누르시면 검사가 정지됩니다.";
			dlg.DoModal();
		}

		g_bXrayAutoSetup=false;
		OnBnClickedMenuStop();
	}
}

void CCtrlPanelView::WriteXrayLog(CString Message)
{
	CString date;
	CTime time;
	time=CTime::GetCurrentTime();
	date.Format(_T("%04d-%02d-%02d-%02d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
					
	CStdioFile fileString;
	if(fileString.Open(_T("D:\\Generator.Log"),
		CStdioFile::modeWrite | CStdioFile::shareDenyNone))
	{
		fileString.SeekToEnd();
		fileString.WriteString(date+Message);
		fileString.Close();
	}
	else
	{
		if(fileString.Open(_T("D:\\Generator.Log"),
			CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))	
		{
			fileString.WriteString(date+Message);
			fileString.Close();
		}
	}
}

static CString DirPath = _T("D:\\nse-temp\\image\\"); 
static CString DirPath1 = _T("E:\\image\\"); 

void CCtrlPanelView::Checkhard(void)
{
	if(!g_bOptionD && !g_bOptionE)
		return;

	int m_space = 0;
	
	if(g_bOptionD && !g_bOptionE)
	{
		m_space=CheckHardSpace(0);
	}
	else if(!g_bOptionD && g_bOptionE)
	{
		m_space=CheckHardSpace(1);
	}
	else if(g_bOptionD && g_bOptionE)
	{
		m_space=CheckHardSpace(2);
	}

	//영상삭제 시작
	if(m_space!=0)
	{	
		COleDateTime Today;
		CString TwoDay;
		int oney=0, onem=0, oned=0;
		int twoy=0, twom=0, twod=0;
		Today = COleDateTime::GetCurrentTime();
		if((m_space==1 || m_space==3) && g_bOptionD_2!=0) 
		{	

			if(g_bOptionD_2/12>=1)
			{
				oney=int(Today.GetYear()-g_bOptionD_2/12);
			}
			else
				oney=int(Today.GetYear());
			
			if(int(Today.GetMonth())-g_bOptionD_2%12>=1)
				onem=int(Today.GetMonth())-g_bOptionD_2%12;
			else
			{
				oney-=1;
				onem=12+(int(Today.GetMonth())-g_bOptionD_2%12);
			}
			
			oned=int(Today.GetDay());

			CFileFind ff;
			int i=0;
			d_SpaceMsg->ShowWindow(SW_SHOW);
			d_SpaceMsg->DrawPic();

			if(ff.FindFile(DirPath+"\\*.*") == TRUE)
			{
				BOOL bFlag = ff.FindNextFile();

				while(bFlag)
				{
					d_SpaceMsg->DrawPic();

					if(ff.IsDirectory())
					{		
						if(!ff.IsDots())
						{
							TwoDay = ff.GetFileName();
							twoy = _wtoi(TwoDay.Left(4));
							twom = _wtoi(TwoDay.Mid(6).Left(2));
							twod = _wtoi(TwoDay.Mid(10).Left(2));

							if(oney>twoy || (oney>=twoy && onem>twom) || (oney>=twoy && onem>=twom && oned>twod))
							{
								SHFILEOPSTRUCT FileOp = {0};
								wchar_t szTemp[MAX_PATH];

								wcscpy(szTemp, ff.GetFilePath());
								szTemp[ff.GetFilePath().GetLength() + 1] = NULL;    // NULL문자가 두개 들어가야 한다.

								FileOp.hwnd = NULL;
								FileOp.wFunc = FO_DELETE;
								FileOp.pTo = NULL;
								FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;  // 확인메시지가 안뜨도록 설정
								FileOp.fAnyOperationsAborted = false;
								FileOp.hNameMappings = NULL;
								FileOp.lpszProgressTitle = NULL;
								FileOp.pFrom = szTemp;	
								SHFileOperation(&FileOp);
							}
						}
					}

					bFlag = ff.FindNextFile();
				}
			}

			d_SpaceMsg->ShowWindow(SW_HIDE);	
			ff.Close();
		}

		if((m_space==2 || m_space==3) && g_bOptionE_2!=0) 
		{		
			//삭제할 날짜 계산 
			if(g_bOptionE_2/12>=1)
			{
				oney=int(Today.GetYear()-g_bOptionE_2/12);
			}
			else
				oney=int(Today.GetYear());
			
			if(int(Today.GetMonth())-g_bOptionE_2%12>=1)
				onem=int(Today.GetMonth())-g_bOptionE_2%12;
			else
			{
				oney-=1;
				onem=12+(int(Today.GetMonth())-g_bOptionE_2%12);
			}
			
			oned=int(Today.GetDay());

			CFileFind ff;
			int i=0;
			d_SpaceMsg->ShowWindow(SW_SHOW);
			d_SpaceMsg->DrawPic();

			if(ff.FindFile(DirPath1+"\\*.*") == TRUE)
			{
				BOOL bFlag = ff.FindNextFile();

				while(bFlag)
				{
					d_SpaceMsg->DrawPic();

					if(ff.IsDirectory())
					{		
						if(!ff.IsDots())
						{
							TwoDay = ff.GetFileName();
							twoy = _wtoi(TwoDay.Left(4));
							twom = _wtoi(TwoDay.Mid(6).Left(2));
							twod = _wtoi(TwoDay.Mid(10).Left(2));

							if(oney>twoy || (oney>=twoy && onem>twom) || (oney>=twoy && onem>=twom && oned>twod))
							{
								SHFILEOPSTRUCT FileOp = {0};
								wchar_t szTemp[MAX_PATH];

								wcscpy(szTemp, ff.GetFilePath());
								szTemp[ff.GetFilePath().GetLength() + 1] = NULL;    // NULL문자가 두개 들어가야 한다.

								FileOp.hwnd = NULL;
								FileOp.wFunc = FO_DELETE;
								FileOp.pTo = NULL;
								FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;  // 확인메시지가 안뜨도록 설정
								FileOp.fAnyOperationsAborted = false;
								FileOp.hNameMappings = NULL;
								FileOp.lpszProgressTitle = NULL;
								FileOp.pFrom = szTemp;	
								SHFileOperation(&FileOp);
							}
						}
					}
					bFlag = ff.FindNextFile();
				}
			}
			d_SpaceMsg->ShowWindow(SW_HIDE);	
			ff.Close();
		}
	}
}


int CCtrlPanelView::CheckHardSpace(int num)
{
	int result=0;
	CString strBuf_1, strBuf_2, strBuf_3;
	
	if(num==0 || num==2)
	{		
		strBuf_1=CheckSpaceD();
		if(strBuf_1=="")
		{
			strBuf_3="";
		}
		else
		{
			strBuf_3=strBuf_1;
			result=1;
		}
	}

	if(num==1 || num==2)
	{		
		//E 드라이브 존재여부 확인
		CString strDrive, strType;
		UINT nType;
		strDrive.Format(_T("E:\\"));
		nType = ::GetDriveType(strDrive);
		if(nType!=1 && (nType==DRIVE_FIXED || nType==DRIVE_REMOVABLE))
		{
			strBuf_2=CheckSpaceE();
			if(strBuf_2!="")
			{	
				strBuf_3+=_T("\n")+strBuf_2;
				result+=2;
			}
		}
	}

	if(result>0)
	{
		strBuf_3.Format(_T("%s%s"),	strBuf_3, g_MessageArr[126]);

		CMessageBoxDlg dlg;
		dlg.m_Message=strBuf_3;
		dlg.m_Mode=2;
		int re=dlg.DoModal();
		if(re==IDOK)
			return result;
		else
			return 0;
	}

	return result;
}


CString CCtrlPanelView::CheckSpaceD(void)
{
	CString strBuf, strBuf_1;
	strBuf.Format(_T("D:\\"));
	ULARGE_INTEGER Fbatc, lnTotBytes, lnAvailable;
	double FreeBytes = 0, TotalBytes = 0, fUsed = 0 ;

	if(::GetDiskFreeSpaceEx(LPCTSTR(strBuf), &Fbatc, &lnTotBytes,&lnAvailable))
	{
		TotalBytes =lnTotBytes.LowPart+(double)lnTotBytes.HighPart*(double)4294967296;
		strBuf.Format(_T("%4.1fGB"), TotalBytes/1024/1024/1024 );
		FreeBytes = lnAvailable.LowPart+(double)lnAvailable.HighPart*(double)4294967296;

		if(FreeBytes*100/TotalBytes<g_bOptionD_1)
		{
			strBuf_1.Format(_T("D%s : %s\n%s : %4.1fGB\n%s : %4.1fGB(%0.0f%%) %s\n\n"),
					g_MessageArr[127], strBuf, g_MessageArr[128], (TotalBytes-FreeBytes)/1024/1024/1024, g_MessageArr[129], FreeBytes/1024/1024/1024, FreeBytes*100/TotalBytes, g_MessageArr[130]);
		}
		
	}

	return strBuf_1;
}


CString CCtrlPanelView::CheckSpaceE(void)
{
	CString strBuf, strBuf_1;
	strBuf.Format(_T("E:\\"));
	ULARGE_INTEGER Fbatc, lnTotBytes, lnAvailable;
	double FreeBytes = 0, TotalBytes = 0, fUsed = 0 ;

	if(::GetDiskFreeSpaceEx(LPCTSTR(strBuf), &Fbatc, &lnTotBytes,&lnAvailable))
	{
		TotalBytes =lnTotBytes.LowPart+(double)lnTotBytes.HighPart*(double)4294967296;
		strBuf.Format(_T("%4.1fGB"), TotalBytes/1024/1024/1024 );
		FreeBytes = lnAvailable.LowPart+(double)lnAvailable.HighPart*(double)4294967296;

		if(FreeBytes*100/TotalBytes<g_bOptionE_1)
		{
			strBuf_1.Format(_T("E%s : %s\n%s : %4.1fGB\n%s : %4.1fGB(%0.0f%%) %s\n\n"),
							g_MessageArr[127], strBuf, g_MessageArr[128], (TotalBytes-FreeBytes)/1024/1024/1024, g_MessageArr[129], FreeBytes/1024/1024/1024, FreeBytes*100/TotalBytes, g_MessageArr[130]);
		}
	}

	return strBuf_1;
}


void CCtrlPanelView::DrawXrayState(void)
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
	pView->DrawXrayState();
}

bool CCtrlPanelView::OnTurnonXrayState(void)
{
	if(g_bGenerator)
	{
		int kv,ma;
		if(NSE_XRAY_KIND==3)
		{
			g_Bit_Out[14]=1; 
			OnDigitalOutPut_All(); 
			Sleep(500);
			g_Bit_Out[14]=0; 
			//알림 리셋
			g_Bit_Out[15]=1;
			OnDigitalOutPut_All();
			Sleepms(1000);
			OnAnalogOutPut_3(g_Xray_kv);
			Sleepms(200);
			OnAnalogOutPut_4(g_Xray_ma);
			Sleepms(200);
			g_Bit_Out[13]=1;
			OnDigitalOutPut_All();
			Sleepms(1000);
			kv = OnXray_kv_Output();
			ma = OnXray_ma_Output();

		}
		else
		{
			if(!m_GComm->isXRayOn())
			{
				m_GComm->setKV(g_Xray_kv);
				m_GComm->setMA(g_Xray_ma);
				m_GComm->setXRay(true);
				if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
				{
					Sleepms(2000);
				}
				else
				{	
					Sleepms(500);
					m_GComm->setXRay(true);
				}
			}
			kv=m_GComm->getKV();
			ma=m_GComm->getMA();
		}

		if(kv<10 || ma<500)
			return false;
		else
			return true;
	}
	else
		return false;
}


HBRUSH CCtrlPanelView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_EDIT)
	{
        pDC->SetBkColor(RGB(215,223,233));
		pDC->SetTextColor(RGB(0,0,0));
        return (HBRUSH) m_bkbrush;
    }
	
	return hbr;
}


void CCtrlPanelView::OnNormalization(void)
{
	if(!(g_bGenerator && g_bDetector))
		return; 
	
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
	
	int m_nStart, m_nEnd;
	//g_Margin2 = 20;
	m_nStart=g_Margin2;

	m_nEnd=g_Height-1;
//		pView->m_Commander.ResetGain();
//		pView->m_Commander.ResetOffset();
	OnTurnoffXray();
		pView->m_Commander.PutCorrectionOffset(FALSE);
		pView->m_Commander.PutCorrectionGain(FALSE);
		Sleepms(100);
		pView->m_Commander.PutStartPixel(m_nStart);
		pView->m_Commander.PutEndPixel(m_nEnd);
		pView->m_Commander.OnBoardOffsetCalibration();
		pView->m_Commander.PutCorrectionOffset(TRUE);
		pView->m_Commander.PutCorrectionGain(FALSE);
		Sleepms(500);
	if(g_bXrayAutoSetup)
		OnTrunonXray_Auto();
	else
	{
		if(g_Xray_kv2>0||g_Xray_ma2>0)
			OnTurnonXray_Low(); 
		else
			OnTurnonXray();
	}
		Sleepms(1000);
		if(NSE_XRAY_KIND==1)
			m_GComm->isXRayOn();

		pView->m_Commander.PutCorrectionOffset(FALSE);
		pView->m_Commander.PutCorrectionGain(FALSE);
		pView->m_Commander.PutCorrectionOffset(FALSE);
		pView->m_Commander.PutCorrectionGain(FALSE);
		Sleepms(200);
		if(NSE_XRAY_KIND==1)
			m_GComm->isXRayOn();

		pView->m_Commander.PutStartPixel(m_nStart);
		pView->m_Commander.PutEndPixel(m_nEnd);
		pView->m_Commander.OnBoardGainCalibration(16384);
		//pView->m_Commander.OnBoardGainCalibration(13106);
		Sleepms(500);
		pView->m_Commander.PutCorrectionGain(TRUE);
		pView->m_Commander.PutCorrectionGain(TRUE);
		pView->m_Commander.PutCorrectionOffset(TRUE);
		Sleepms(500);

	if(!g_bXrayAutoSetup)
	{
		if(g_Xray_kv2>0||g_Xray_ma2>0)
		{
			OnTurnoffXray();
			OnTurnonXray();
		}
	}
	if(NSE_XRAY_KIND==1)
		m_GComm->isXRayOn();
}

void CCtrlPanelView::OnNormalizationC01B(void)
{
	if (!(g_bGenerator && g_bDetector))
		return;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();

	int m_nStart, m_nEnd;

//	m_nStart = g_Margin2;
//	m_nEnd = g_Height - 1;

	//20251029 lim 개선
	m_nStart = 0;
	m_nEnd = g_Height;


	// =========================================================
	// Normalization 전용 Frame Line Number
	// =========================================================

	const UINT NORMALIZATION_LINE_NUMBER = 32;
	if (!pView->SetC01BFrameLineNumber(NORMALIZATION_LINE_NUMBER))
	{
		//CString strMsg;
		//strMsg.Format(_T("C01B Detector %d failed to set normalization LineNum=%u."), nDetector, NORMALIZATION_LINE_NUMBER);
		//pView->WriteDetectorErrorLog(1, strMsg);
		//return false;
		return;
	}



	// 20240214 CB01 Detector Normalizationca
	pView->_xcommand.SetPara(XPARA_EN_GAIN_CORRECT, 0);
	pView->_xcommand.SetPara(XPARA_EN_OFFSET_CORRECT, 0);
	pView->_xcommand.SetPara(XPARA_EN_BASELINE_CORRECT, 0);

	OnTurnoffXray();
	//Sleepms(2000);
	//pView->_xoff_correct.Reset();
	pView->_xoff_correct.CalculatePara(1, &(pView->_xacquisition), &(pView->_xtransfer), m_nStart, m_nEnd, 0);
	Sleepms(2000);

	//OnTurnonXray();
	if (g_bXrayAutoSetup)
		OnTrunonXray_Auto();
	else
	{
		if (g_Xray_kv2 > 0 || g_Xray_ma2 > 0)
			OnTurnonXray_Low();
		else
			OnTurnonXray();
	}
	Sleepms(1000);
	pView->_xoff_correct.CalculatePara(0, &(pView->_xacquisition), &(pView->_xtransfer), m_nStart, m_nEnd, 16384);
	Sleepms(2000);

	pView->_xoff_correct.SaveFlash(&(pView->_xcommand), 0);

	pView->_xcommand.SetPara(XPARA_EN_GAIN_CORRECT, 1);
	pView->_xcommand.SetPara(XPARA_EN_OFFSET_CORRECT, 1);
	Sleepms(500);
	if (NSE_XRAY_KIND == 1)
		m_GComm->isXRayOn();

	pView->_xcommand.SetPara(XPARA_BASE_LINE, 100);
	pView->_xcommand.SetPara(XPARA_EN_GAIN_CORRECT, 1);
	Sleepms(500);
	pView->_xcommand.ExecutePara(XPARA_SAVE_PARA);
	Sleepms(500);
	pView->_xcommand.ExecutePara(XPARA_LOAD_GAIN, 0);
	pView->_xcommand.ExecutePara(XPARA_LOAD_OFFSET);
	Sleepms(500);

	if (!g_bXrayAutoSetup)
	{
		if (g_Xray_kv2 > 0 || g_Xray_ma2 > 0)
		{
			OnTurnoffXray();
			OnTurnonXray();
		}
	}
	if (NSE_XRAY_KIND == 1)
		m_GComm->isXRayOn();

	//20240602 PDC
	char s;
	pView->_xcommand.SendAscCmd("[EP,W,0,1]", &s);


	// =========================================================
	// 일반 검사 모드 복구
	// =========================================================

	const UINT INSPECTION_LINE_NUMBER = NSE_TRIGER_HEIGHT;
	if (!pView->SetC01BFrameLineNumber(INSPECTION_LINE_NUMBER))
	{
		//CString strMsg;
		//strMsg.Format(_T("C01B Detector %d failed to set normalization LineNum=%u."), nDetector, NORMALIZATION_LINE_NUMBER);
		//pView->WriteDetectorErrorLog(1, strMsg);
		//return false;
		return;
	}


}

void CCtrlPanelView::OnTrunonXray_Auto(void)
{
	if(g_bGenerator)
	{
		if(NSE_XRAY_KIND==3)
		{
			g_Bit_Out[14]=1; 
			OnDigitalOutPut_All(); 
			Sleep(500);
			g_Bit_Out[14]=0; 
			//알림 리셋
			g_Bit_Out[15]=1;
			OnDigitalOutPut_All();
			Sleepms(1000);
			OnAnalogOutPut_3(40);
			Sleepms(200);
			OnAnalogOutPut_4(2000);
			Sleepms(200);
			g_Bit_Out[13]=1;
			OnDigitalOutPut_All();
			Sleepms(1000);
		}
		else
		{
			if(!m_GComm->isXRayOn())
			{
				if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
				{
					if(NSE_XRAY_KIND==2)
					{
						m_GComm->setKV(45);
						Sleepms(500);
						m_GComm->setMA(800);
					}
					else
					{
						m_GComm->setKV(60);
						Sleepms(500);
						m_GComm->setMA(2000);
					}
					Sleepms(500);
					m_GComm->setXRay(true);
					Sleepms(2000);
				}
				else 
				{
					m_GComm->setKV(60);
					Sleepms(500);
					m_GComm->setMA(2000);
					Sleepms(500);
					m_GComm->setXRay(true);
					Sleepms(1000);
					m_GComm->setXRay(true);
				}
			}
		}
	}
}

void CCtrlPanelView::OnTurnonXray_Low(void)
{
	if(g_bGenerator)
	{
		if(NSE_XRAY_KIND==3)
		{
			g_Bit_Out[14]=1; 
			OnDigitalOutPut_All(); 
			Sleep(500);
			g_Bit_Out[14]=0; 
			//알림 리셋
			g_Bit_Out[15]=1;
			OnDigitalOutPut_All();
			Sleepms(1000);
			OnAnalogOutPut_3(g_Xray_kv-g_Xray_kv2);
			Sleepms(200);
			OnAnalogOutPut_4(g_Xray_ma-g_Xray_ma2);
			Sleepms(200);
			g_Bit_Out[13]=1;
			OnDigitalOutPut_All();
			Sleepms(1000);
		}
		else
		{
			if(!m_GComm->isXRayOn())
			{
				m_GComm->setKV(g_Xray_kv-g_Xray_kv2);
				m_GComm->setMA(g_Xray_ma-g_Xray_ma2);
				m_GComm->setXRay(true);
				if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
				{
					Sleepms(2000);
				}
				else
				{
					Sleepms(500);
					m_GComm->setXRay(true);
				}
			}
		}
	}
}

void CCtrlPanelView::OnStartConveyor(void)
{
	if(g_bIOcard)
	{
		if(NSE_IO_OUPUT_PORT==2)
		{
			if(g_bIOcard){
				g_Bit_Out[0]=1;
				g_Bit_Out[3]=1;
				g_Bit_Out[4]=1;
				g_Bit_Out[10]=1;
				OnDigitalOutPut_All();
				Sleep(100);
				OnAnalogOutPut_1(g_ConveyorSpeed);
				return;
			}
		
		}
		else
		{
			//20200525
			g_portWriteData = 0x1d;
			ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);	//260810 NSE_IO_OUPUT_PORT로 변경
			if(error)
				return;
			//20200525
			/*
			USHORT      gwMask = NSE_CONVERYOR_MASKING;                  // mask
			USHORT      gwData = NSE_CONVERYOR_ON;                  // output data

			m_ptDioWritePortByte.port = NSE_IO_OUPUT_PORT;
			m_ptDioWritePortByte.mask  = gwMask;
			m_ptDioWritePortByte.state = 0x1d;

			if(DRV_DioWritePortByte(m_DriverHandle,
					 (LPT_DioWritePortByte)&m_ptDioWritePortByte)!=0)
			{
			}*/
		}
	}
}

void CCtrlPanelView::OnStopConveyor(void)
{
	if(g_bIOcard)
	{
		if(NSE_IO_OUPUT_PORT==2)
		{
			g_Bit_Out[0]=0;
			g_Bit_Out[3]=0;
			g_Bit_Out[4]=0;
			g_Bit_Out[10]=0;
			OnDigitalOutPut_All();
			Sleep(100);
			OnAnalogOutPut_1(0);
		}
		else
		{
			//20200525
			g_portWriteData = 0x00;
			ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);	//260810 NSE_IO_OUPUT_PORT로 변경
			if(error)
				return;
			//20200525
			/*
			USHORT      gwMask = NSE_CONVERYOR_MASKING;                  // mask
			USHORT      gwData = NSE_CONVERYOR_OFF;                  // output data
			gwData = 0x20;
			m_ptDioWritePortByte.port = NSE_IO_OUPUT_PORT;
			m_ptDioWritePortByte.mask  = gwMask;
			m_ptDioWritePortByte.state = gwData;
			
			if((m_ErrCde = DRV_DioWritePortByte(m_DriverHandle,
					 (LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
			{
			}*/
		}
	}
}


UINT ThreadGenerator(LPVOID lParam) 
{
	CCtrlPanelView *pView;
	pView=(CCtrlPanelView *)lParam;

	while(b_ThreadGen)
	{
		int t_Status = 0;
		int x_Status = 0; //kds20090709
		
		if(g_bGenerator && !m_Interock_State)
		{
			t_Status = pView->m_GComm->status();	
			if(t_Status==0)
			{
				Sleepms(500);
				x_Status = pView->m_GComm->isXRayOn();
				Sleepms(2000);
				t_Status = pView->m_GComm->status();
				if(t_Status==0)
				{
					if(x_Status==0 && b_ThreadGen)
					{ 
						if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
							g_iXrayOn = 1009; 
						else 
							g_iXrayOn = 1010; 

							g_StopKind = 1; //20191202

						CMessageBoxDlg dlg;
						dlg.m_Message=g_MessageArr[131];//"X-ray OFF 인터록 확인후 재시작 하세요!! - Code : 0709";
						dlg.DoModal();
				
						pView->WriteXrayLog(_T("-X-ray Off\r\n"));
					}
			
					if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
						Sleepms(3000);	
					else
						Sleepms(1500);	
				}
			}
			else
			{
				pView->CheckXrayError(0, t_Status);
				b_ThreadGen=false;
			}
		}
		else 
		{
			b_ThreadGen=false;
		}
	}
	return 0;
}

void CCtrlPanelView::OnMenuReport(void)
{
	if(m_Interock_State)
		return;

	if(g_bDetector)
	{
		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
		CString temp_name, temp_sn;

		if (g_SelDetector == 0)
		{
			if (pView->m_Commander.GetIsOpened())
			{
				float PxielSize;
				pView->m_Commander.GetPixelSize(&PxielSize);
				ULONG PNum;
				pView->m_Commander.GetPixelNumber(&PNum);
				int Len = (int)(PNum * PxielSize);

				char* pName;
				CString cs;
				if (pView->m_Commander.GetModuleTypeName(&pName))
					cs = (CString)pName;
				temp_name.Format(_T("%s-%1.1f-%d"), cs, PxielSize, Len);

				char* pInfo;
				if (pView->m_Commander.GetSerialNumber(&pInfo))
					temp_sn = (CString)pInfo;
			}
			else
			{
				CStdioFile fileString;
				if (fileString.Open(_T("D:\\Database\\Detector.dat"),
					CStdioFile::modeRead | CStdioFile::modeNoTruncate | CStdioFile::shareDenyNone))
				{
					fileString.SeekToBegin();
					fileString.ReadString(temp_name);
					fileString.ReadString(temp_sn);
				}
			}
		}
		else
		{
			XDevice* dev_ = pView->_xsystem.GetDevice(0);

			if (pView->_xsystem.GetIsOpen())
			{
				uint64_t PixelSize;
				pView->_xcommand.GetPara(XPARA_PIXEL_SIZE, PixelSize);
				float len = (float)PixelSize * 0.1f;

				ULONG PNum = dev_->GetPixelNumber();
				CString cs;
				cs = _T("C01B");
				temp_name.Format(_T("%s-%1.1f-%d"), cs, len, (int)(PNum * len));

				char sn_str[64];
				if (pView->_xcommand.GetPara(XPARA_GCU_SERIAL, sn_str))
					temp_sn = (CString)sn_str;
			}
			else
			{
				CStdioFile fileString;
				if (fileString.Open(_T("D:\\Database\\Detector.dat"),
					CStdioFile::modeRead | CStdioFile::modeNoTruncate | CStdioFile::shareDenyNone))
				{
					fileString.SeekToBegin();
					fileString.ReadString(temp_name);
					fileString.ReadString(temp_sn);
				}
			}
		}
		

		bool g_bOpenDB=true;

		Init_Mysql();

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			MYSQL_RES* m_Res;
			MYSQL_ROW row;

			mysql_query(&m_mysql, "set names euckr");

			CString strSQL;
			CString strQuery;
			strSQL.Format(_T("SELECT * FROM intellisense_db5"));

			if (mysql_query(&m_mysql, CStringA(strSQL)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
			m_Res = mysql_store_result(&m_mysql);

			if ((row = mysql_fetch_row(m_Res)) = NULL)
			{
				strQuery.Format(_T("Insert INTO intellisense_db5(strModel, strSN)") _T("VALUES('%s', '%s')"),
					temp_name, temp_sn);
				if (mysql_query(&m_mysql, CStringA(strQuery)))
				{
					CString error(mysql_error(&m_mysql));
					AfxMessageBox(error);
				}
			}
			else
			{
				strQuery.Format(_T("Update intellisense_db5 SET strModel = '%s', strSN = '%s'"),
					temp_name, temp_sn);
				if (mysql_query(&m_mysql, CStringA(strQuery)))
				{
					CString error(mysql_error(&m_mysql));
					AfxMessageBox(error);
				}
			}
			mysql_free_result(m_Res);
		}

		Close_Mysql();

		//if(!m_pSet_5.Open_DB(g_StrDirPath[5]))		
		//{	
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[5], g_StrDirPath_copy[5]);
		//	if(m_pSet_5.Open_DB(g_StrDirPath[5]))		
		//		g_bOpenDB=true;
		//}
		//if(g_bOpenDB)
		//{
		//	CString strSQL;
		//	strSQL.Format(_T("SELECT * FROM Detector_Info"));
		//	if(m_pSet_5.Select_DB(strSQL))
		//	{
		//		CString strQuery;
		//		if(m_pSet_5.IsEOF())
		//		{
		//			strQuery.Format(_T("Insert INTO Detector_Info(strModel, strSN)") _T("VALUES('%s', '%s')"),
		//				temp_name, temp_sn);
		//			m_pSet_5.Insert_DB(strQuery);
		//		}
		//		else
		//		{
		//			strQuery.Format(_T("Update Detector_Info SET strModel = '%s', strSN = '%s'"),
		//				temp_name, temp_sn);
		//			m_pSet_5.Update_DB(strQuery);
		//		}
		//	}
		//	if(m_pSet_5.b_IsOpenedDB)
		//		m_pSet_5.Close_DB();
		//}
	}

	CReportDlg dlg;
	dlg.DoModal();
}


void CCtrlPanelView::OnBnClickedMini()
{
	keybd_event(VK_LWIN,0, 0, 0);     
	keybd_event(LOBYTE(VkKeyScan('d')),0, 0, 0);     
	keybd_event(VK_LWIN,0, KEYEVENTF_KEYUP, 0);
	keybd_event(LOBYTE(VkKeyScan('d')),0,KEYEVENTF_KEYUP,0);	
}


void CCtrlPanelView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//영문 카운터리셋버튼 비활성화시 활성화
/*	if(point.x>=876 && point.x<=1023 && point.y>=1 && point.y<=104)
	{
		if(!g_bAdmin)
		{
			CAdminDlg login;
			login.m_Mode=1;
			int result = login.DoModal();
			if(result==1)
			{
				m_Bt_ResetCount.EnableWindow(true);
			}
		}
	}
*/
	CFormView::OnLButtonUp(nFlags, point);
}

void CCtrlPanelView::OnAutoTeaching(void)
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();

	if (g_SelDetector == 0)
	{
		pView->m_Commander.ResetGain();
		pView->m_Commander.ResetOffset();
	}
	else if (g_SelDetector == 1) //20251029 lim 개선
	{
		pView->_xcommand.SetPara(XPARA_RESET_GAIN, 0);
		pView->_xcommand.SetPara(XPARA_RESET_OFFSET, 0);

	}//20251029 lim 개선

	OnTurnonXray();
	Sleepms(1000);

	g_iGrab_Count = 0; //kds20140701
	Sleepms(10);
	if (g_SelDetector == 0)
	{
		if (pView->m_DTImage.GetIsOpened())
		{
			for (int i = 0; i < 50; i++)
			{
				//pView->m_DTImage.Snap(1);//kds20140917
				pView->m_DTImage.Snap();//kds20140917
				Sleepms(10);
			}
		}
	}
	else//20251029 lim 개선
	{
		if (pView->_xacquisition.GetIsOpen())
		{
			for (int i = 0; i < 50; i++)
			{
				//pView->m_DTImage.Snap(1);//kds20140917
				pView->_xacquisition.Snap();//kds20140917
				Sleepms(10);
			}
		}

	}//20251029 lim 개선

	
	int a=0;
	int gv =0;
	int m_MaxValue1=0;
	int m_Histogram[120];

	
	for (a = 0; a < 120; a++)
	{
		m_Histogram[a] = 0;
	}

	for (a = 0; a < 50; a++)
	{
		gv = (int)g_Margin22[a];
		m_Histogram[gv]++;
	}


	int maxa = m_Histogram[1];
	for (a = 1; a < 120; a++)
	{
		if (maxa < m_Histogram[a])
		{
			maxa = m_Histogram[a];
			m_MaxValue1 = a;
		}
	}
	
	g_Margin2 = m_MaxValue1+2; 

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	

	Sleepms(20);
	OnTurnoffXray();

	//log 저장 >>>>>
	CString str,str2;
	
	CTime time;
	time=CTime::GetCurrentTime();
	str.Format(_T("%04d-%02d-%02d-%02d%02d%02d > %d   "),time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), g_Margin2);

	CStdioFile fileString;
	if(fileString.Open(_T("D:\\Database\\Conveyor.Log"),
		CStdioFile::modeWrite | CStdioFile::shareDenyNone))
	{
		fileString.SeekToEnd();
		fileString.WriteString(str);
		fileString.Close();
	}
	else
	{
		if(fileString.Open(_T("D:\\Database\\Conveyor.Log"),
			CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))	
		{
			fileString.WriteString(str);
			fileString.Close();
		}
	}
}


void CCtrlPanelView::OnMenuInterlock(void)
{
	if(NSE_IO_OUPUT_PORT==2)
	{
		g_Bit_Out[3]=1;
		//부저이게맞나 확인
		g_Bit_Out[7]=0;
		OnDigitalOutPut_All22();
	}
	else
	{
		//20200525
		g_portWriteData = 0x04;
		ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);	//260810 NSE_IO_OUPUT_PORT로 변경
			if(error)
				return;
			//20200525
/*
		USHORT      gwMask = NSE_CONVERYOR_MASKING;                  // mask
		USHORT      gwData = NSE_CONVERYOR_OFF;                  // output data

		m_ptDioWritePortByte.port = NSE_IO_OUPUT_PORT;
		m_ptDioWritePortByte.mask  = 0x44;
		m_ptDioWritePortByte.state = 0x04;

		if((m_ErrCde = DRV_DioWritePortByte(m_DriverHandle,
			(LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
		{
			DRV_DioWritePortByte(m_DriverHandle,(LPT_DioWritePortByte)&m_ptDioWritePortByte);
			//	return;
		}*/
	}
}

//20191202
UINT ThreadClient(LPVOID lParam)
{
	int i=0;
	while(b_ThreadSC)
	{  	
		if(b_ThreadSC)
		{
			CCtrlPanelView *pView;
			pView=(CCtrlPanelView *)lParam;
			pView->SendData();

		}
		
		Sleep(g_SignalTime);
	}
		return 0;
}

void CCtrlPanelView::SendData(void)
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();

	
	int retval;
	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
	{
		WSACleanup();
		return;
	}

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET)
	{
		//AfxMessageBox("소켓생성 실패!");
		closesocket(sock);
		WSACleanup();
	
		return;
	}

	char* Temp2;
	int nStrLength2 = WideCharToMultiByte(CP_ACP,0,g_Adress,-1,NULL,0,NULL,NULL);
	Temp2 = new char[nStrLength2+1];
	WideCharToMultiByte(CP_ACP,0,g_Adress,-1,Temp2,nStrLength2,NULL,NULL);

	
	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	
	serveraddr.sin_port = htons(502);
//	serveraddr.sin_addr.s_addr = inet_addr("192.168.30.171");  
	serveraddr.sin_addr.s_addr = inet_addr(Temp2);  
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

	if(retval == SOCKET_ERROR)
	{
			
		closesocket(sock);
		WSACleanup();
		return;
	}	

	//kds20100319
	
	CString m_sData;
	CString m_sDate,m_sTime;
	CTime time;
	time = time.GetCurrentTime();
	m_sDate.Format(_T("%04d-%02d-%02d"), time.GetYear(), time.GetMonth(), time.GetDay());
	m_sTime.Format(_T("%02d:%02d:%02d"),time.GetHour(),time.GetMinute(),time.GetSecond());
	
	// 데이터 입력
//[CS001],[제품코드],[제품명],[생산실적수량],[정상수],[불량수(이물)],[누락]
//	,[0],[업로드시간(yyyyMMdd HHmmdd)],[수동/자동상태(0/1)],[작업시작



//  [CS002],[제품코드],[제품명],[총테스트수량],[정상수량],[불량수량],[누락수량],[테스트시작시간],[테스트종료시간]
//	CString g_Code;
//	g_Code = "XB2";
	CTime m_Rtime;
	GetSystemTime(&m_Rstm);
	m_Rtime=CTime::GetCurrentTime();
	m_Comparedate.Format(_T("%04d-%02d-%02d"), m_Rtime.GetYear(),m_Rtime.GetMonth(),m_Rtime.GetDay());		
	m_Comparehour = m_Rtime.GetHour();
	m_Comparemin = m_Rtime.GetMinute();
	CString m_UploadTime;
	m_UploadTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),m_Rtime.GetYear(),m_Rtime.GetMonth(),m_Rtime.GetDay(),m_Rtime.GetHour(),m_Rtime.GetMinute(),m_Rtime.GetSecond());
	//20190510  시작날짜와 지금날짜가 같지 않을때
	if(m_Comparedate!=m_StrDate)
	{
		if(m_Comparehour == 0 && m_Comparemin < 5) //다음날 5분동안만 동작
		{
			CTimeSpan span(1,0,0,0);
			time -= span;
			m_sDate.Format(_T("%04d-%02d-%02d"), time.GetYear(), time.GetMonth(), time.GetDay());
			
			if(m_Comparemin < 1)  // 1분보다 작을때 마지막 수량 기억 및 신규카운트 초기화
			{
				temp_total = g_Sum_Total_Count;
				temp_ok = g_Sum_Ok_Count;
				temp_ng = g_Sum_Ng_Count;
				temp_omit = g_Sum_Omit_Count;

				pView->m_Count_Total2 = 0;
				pView->m_Count_OK2 = 0;
				pView->m_Count_NG2 = 0;
				pView->m_Count_Soup2 = 0;

			}
			
			if(g_SendKind == 0)
			{
				m_sData.Format(_T("%cCS001,%s,%s,%c"), 0x02, g_Line, m_UploadTime, 0x03);
			}
			else if(g_SendKind == 1)
			{
				m_sData.Format(_T("%cCS002,%s,%s,%s,%d,%d,%d,%d,%s,%s,%c"),
					0x02, g_Line, g_ProductName, m_sDate, temp_total, temp_ok, temp_ng, temp_omit, m_UploadTime, g_PGStartTime, 0x03);
			}
			else if(g_SendKind == 2)
			{
				m_sData.Format(_T("%cCS003,%s,%s,%d,%d,%d,%d,%s,%s,%d,%c"),
					0x02, g_Line, g_ProductName, temp_total, temp_ok, temp_ng, temp_omit, g_PGStartTime, g_PGEndTime, g_StopKind, 0x03);
			}
		}
		else  //날짜가 바뀌고 5분이 지나면 마지막 수량 넘겨 주고, 신규수량 업데이트
		{
			if(g_SendKind == 0)
			{
				m_sData.Format(_T("%cCS001,%s,%s, %c"), 0x02, g_Line, m_UploadTime, 0x03);
			}
			else if(g_SendKind == 1)
			{
				m_sData.Format(_T("%cCS002,%s,%s,%s,%d,%d,%d,%d,%s,%s,%c"),
					0x02, g_Line, g_ProductName, m_sDate, temp_total, temp_ok, temp_ng, temp_omit, m_UploadTime, g_PGStartTime, 0x03);
			}
			else if(g_SendKind == 2)
			{
				m_sData.Format(_T("%cCS003,%s,%s,%d,%d,%d,%d,%s,%s,%d,%c"),
					0x02, g_Line, g_ProductName, temp_total, temp_ok, temp_ng, temp_omit, g_PGStartTime, g_PGEndTime, g_StopKind, 0x03);
			}

			g_Sum_Total_Count = pView->m_Count_Total2;
			g_Sum_Ok_Count = pView->m_Count_OK2;
			g_Sum_Ng_Count = pView->m_Count_NG2;
			g_Sum_Omit_Count = pView->m_Count_Soup2; 
		
			m_StrDate = m_Comparedate;
		}
	}
	else
	{
		if(g_SendKind == 0)
		{
			m_sData.Format(_T("%cCS001,%s,%s,%c"), 0x02, g_Line, m_UploadTime, 0x03);
		}
		else if(g_SendKind == 1)
		{
			m_sData.Format(_T("%cCS002,%s,%s,%s,%d,%d,%d,%d,%s,%s,%c"),
				0x02, g_Line, g_ProductName, m_sDate, g_Sum_Total_Count, g_Sum_Ok_Count, g_Sum_Ng_Count, g_Sum_Omit_Count, m_UploadTime, g_PGStartTime, 0x03);
		}
		else if(g_SendKind == 2)
		{
			m_sData.Format(_T("%cCS003,%s,%s,%d,%d,%d,%d,%s,%s,%d,%c"),
				0x02, g_Line, g_ProductName, g_Sum_Total_Count, g_Sum_Ok_Count, g_Sum_Ng_Count, g_Sum_Omit_Count, g_PGStartTime, g_PGEndTime, g_StopKind, 0x03);
		}
	}


	//GetDlgItem(IDC_EDIT1)->SetWindowText(m_sData);

	char* Temp;
	int nStrLength = WideCharToMultiByte(CP_ACP,0,m_sData,-1,NULL,0,NULL,NULL);
	Temp = new char[nStrLength+1];
	WideCharToMultiByte(CP_ACP,0,m_sData,-1,Temp,nStrLength,NULL,NULL);

	retval = send(sock, Temp, nStrLength, 0);

	if(retval == SOCKET_ERROR)
	{
	//	AfxMessageBox(_T("데이터 전송 실패!"));
	}

	delete[] Temp;
	closesocket(sock);
	WSACleanup();
}
