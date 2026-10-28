// Maintenance.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "Maintenance.h"
#include "afxdialogex.h"


// CMaintenance 대화 상자입니다.
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CtrlPanelView.h"
#include "InitailDlg.h"

#include "ImageProcessing.h"

static bool b_ThreadDrawImage = false;

IMPLEMENT_DYNAMIC(CMaintenance, CDialogEx)


CMaintenance::CMaintenance(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMaintenance::IDD, pParent)
	, m_Interlock(false)
	, b_XrayPower(false)
{
	m_ErrorName = _T("");
	b_XrayOnOff=false;
	m_SavePath = _T("");
	m_DispXraykV=0;
	m_DispXrayuA=0;
	b_ConveyorOnOff=false;
	b_GreenlampOnOff=false;
	b_RejectOnOff=false;
	b_YellowlampOnOff=false;
	m_ImageCount=0;
}

CMaintenance::~CMaintenance()
{
}

void CMaintenance::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_CLOSE, m_BtClose);
	DDX_Check(pDX, IDC_BT_XRAY_ONOFF, b_XrayOnOff);
	DDX_Check(pDX, IDC_BT_IO_CONVEYOR, b_ConveyorOnOff);
	DDX_Check(pDX, IDC_BT_IO_GREENLAMP, b_GreenlampOnOff);
	DDX_Check(pDX, IDC_BT_IO_REJECT, b_RejectOnOff);
	DDX_Check(pDX, IDC_BT_IO_YELLOWLAMP, b_YellowlampOnOff);
	DDX_Control(pDX, IDC_PICTURE, m_GrabImage);
	DDX_Control(pDX, IDC_PICGRAPH, m_PicGraph);
	DDX_Control(pDX, IDC_LISTINFO, m_ListInfo);
	DDX_Control(pDX, IDC_BT_XRAY_ONOFF, m_BtXray);
}


BEGIN_MESSAGE_MAP(CMaintenance, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BT_CLOSE, &CMaintenance::OnBnClickedBtClose)
	ON_BN_CLICKED(IDC_BT_XRAY_STATUS, &CMaintenance::OnBnClickedBtXrayStatus)
	ON_BN_CLICKED(IDC_BT_XRAY_RESET, &CMaintenance::OnBnClickedBtXrayReset)
	ON_BN_CLICKED(IDC_BT_XRAY_ONOFF, &CMaintenance::OnBnClickedBtXrayOnoff)
	ON_BN_CLICKED(IDC_BT_XRAY_WARMUP, &CMaintenance::OnBnClickedBtXrayWarmup)
	ON_BN_CLICKED(IDC_BT_DT_NORMAL, &CMaintenance::OnBnClickedBtDtNormal)
	ON_BN_CLICKED(IDC_BT_DT_RESET, &CMaintenance::OnBnClickedBtDtReset)
	ON_BN_CLICKED(IDC_BT_DT_SAVECHECKIMAGE, &CMaintenance::OnBnClickedBtDtSavecheckimage)
	ON_BN_CLICKED(IDC_BT_DT_SAVENOWIMAGE, &CMaintenance::OnBnClickedBtDtSavenowimage)
	ON_BN_CLICKED(IDC_BT_IO_CONVEYOR, &CMaintenance::OnBnClickedBtIoConveyor)
	ON_BN_CLICKED(IDC_BT_IO_REJECT, &CMaintenance::OnBnClickedBtIoReject)
	ON_BN_CLICKED(IDC_BT_IO_YELLOWLAMP, &CMaintenance::OnBnClickedBtIoYellowlamp)
	ON_BN_CLICKED(IDC_BT_IO_GREENLAMP, &CMaintenance::OnBnClickedBtIoGreenlamp)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BT_IO_ENCODER, &CMaintenance::OnBnClickedBtIoEncoder)
	ON_BN_CLICKED(IDC_BT_IO_REJECT2, &CMaintenance::OnBnClickedBtIoReject2)
	ON_BN_CLICKED(IDC_BT_IO_REJECT3, &CMaintenance::OnBnClickedBtIoReject3)
END_MESSAGE_MAP()


// CMaintenance 메시지 처리기입니다.
BOOL CMaintenance::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(255,255,255));
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	LoadBackground();

	int i=0;
	for(i=m_ListInfo.GetCount()-1; i>=0;i--)
		m_ListInfo.DeleteString(i);

	if(g_bGenerator)
		m_ListInfo.AddString(g_MessageArr[204]);//
	else
	{
		GetDlgItem(IDC_BT_XRAY_RESET)->EnableWindow(false);
		GetDlgItem(IDC_BT_XRAY_ONOFF)->EnableWindow(false);
		GetDlgItem(IDC_BT_XRAY_WARMUP)->EnableWindow(false);
		m_ListInfo.AddString(g_MessageArr[205]);//_T("X-ray 발생장치 : 연결되지 않음"));
	}
	if(g_bDetector)
	{
		g_XrayState=2;		
		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
	
		if(pView->m_DTImage.GetIsOpened())
		{
			pView->b_IsNormalization=3;
			pView->m_FrameCount = 0;
			pView->m_DTImage.Grab(0);
		}

		b_ThreadDrawImage = true;
		AfxBeginThread(ThreadDrawImage, (LPVOID)this);
		m_ListInfo.AddString(g_MessageArr[206]);//_T("디텍터 : 연결됨"));

	}
	else
	{
		GetDlgItem(IDC_BT_DT_NORMAL)->EnableWindow(false);
		GetDlgItem(IDC_BT_DT_RESET)->EnableWindow(false);
		GetDlgItem(IDC_BT_DT_SAVENOWIMAGE)->EnableWindow(false);
		GetDlgItem(IDC_BT_DT_SAVECHECKIMAGE)->EnableWindow(false);
		m_ListInfo.AddString(g_MessageArr[207]);//_T("디텍터 : 연결되지 않음"));
	}

	if(g_bIOcard)
	{
		b_ThreadDrawImage = true;
		AfxBeginThread(ThreadDrawImage, (LPVOID)this);
		m_ListInfo.AddString(g_MessageArr[208]);//_T("I/O board : 연결됨"));
	}
	else
	{
		GetDlgItem(IDC_BT_IO_CONVEYOR)->EnableWindow(false);
		GetDlgItem(IDC_BT_IO_YELLOWLAMP)->EnableWindow(false);
		GetDlgItem(IDC_BT_IO_GREENLAMP)->EnableWindow(false);
		GetDlgItem(IDC_BT_IO_REJECT)->EnableWindow(false);
		GetDlgItem(IDC_BT_IO_REJECT2)->EnableWindow(false);
		GetDlgItem(IDC_BT_IO_REJECT3)->EnableWindow(false);
		m_ListInfo.AddString(g_MessageArr[209]);//_T("I/O board : 연결되지 않음"));
	}
	//부품 점검 엔코더 정상동작 상태 20181119
	if(g_RejectType == 0)
	{
		GetDlgItem(IDC_BT_IO_ENCODER)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_BT_IO_ENCODER)->EnableWindow(false);
	}

	SetDlgItemInt(IDC_DISPXRAYKV, 0);
	SetDlgItemInt(IDC_DISPXRAYUA, 0);
	SetDlgItemInt(IDC_DISPHEIGHT, g_Height);
	SetTimer(2, 3000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CMaintenance::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMaintenance::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

void CMaintenance::LoadBackground()
{
	int mainbgnum;
	if(g_NationalCode==0)
	{
		mainbgnum=IDB_SETTINGSCREEN;
		m_BtClose.LoadBitmaps(IDB_BTDTTESTCLOSE, IDB_BTDTTESTCLOSE_1); 
	}
	else if(g_NationalCode==1)
	{
		mainbgnum=IDB_SETTINGSCREEN_ENG;
		m_BtClose.LoadBitmaps(IDB_BTDTTESTCLOSE_ENG, IDB_BTDTTESTCLOSE_1_ENG); 
	}
	else if(g_NationalCode==2)
	{
		mainbgnum=IDB_SETTINGSCREEN_CHI;
		m_BtClose.LoadBitmaps(IDB_BTDTTESTCLOSE_CHI, IDB_BTDTTESTCLOSE_1_CHI); 
	}
	else if(g_NationalCode==3)
	{
		mainbgnum=IDB_SETTINGSCREEN_SPN;
		m_BtClose.LoadBitmaps(IDB_BTDTTESTCLOSE_SPN, IDB_BTDTTESTCLOSE_1_SPN); 
	}
	SetBackgroundImage(mainbgnum, CDialogEx::BACKGR_TOPLEFT);

	SetDlgItemText(IDC_BT_XRAY_STATUS, g_MessageArr[189]);
	SetDlgItemText(IDC_BT_XRAY_RESET, g_MessageArr[190]);
	SetDlgItemText(IDC_BT_XRAY_WARMUP, g_MessageArr[191]);
	SetDlgItemText(IDC_BT_DT_NORMAL, g_MessageArr[192]);
	SetDlgItemText(IDC_BT_DT_RESET, g_MessageArr[193]);
	SetDlgItemText(IDC_BT_DT_SAVECHECKIMAGE, g_MessageArr[194]);
	SetDlgItemText(IDC_BT_DT_SAVENOWIMAGE, g_MessageArr[195]);
	SetDlgItemText(IDC_BT_IO_CONVEYOR, g_MessageArr[196]);
	SetDlgItemText(IDC_STATIC, g_MessageArr[197]);
	SetDlgItemText(IDC_BT_IO_YELLOWLAMP, g_MessageArr[198]);
	SetDlgItemText(IDC_BT_IO_GREENLAMP, g_MessageArr[199]);

	m_BtClose.SizeToContent();
}

void CMaintenance::OnBnClickedBtClose()
{
	CDialog::OnCancel();
}


void CMaintenance::OnBnClickedBtXrayStatus()
{
	m_ErrorName = _T("");
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();
	if(g_bGenerator)
	{
		if(NSE_XRAY_KIND == 3)
		{
			OnDigitalInPut_All();
			bool result = CheckXrayError(0);
			if(result)
			{
				m_ListInfo.AddString(g_MessageArr[210]);//_T("X-ray 발생장치 상태 : 정상"));
				m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
				
			}
			else
			{
				m_ListInfo.AddString(g_MessageArr[211]+m_ErrorName);
				m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
			}
		}
		else
		{
			pCtrlView->m_GComm->setClear();
			int status=pCtrlView->m_GComm->status();
			bool result = CheckXrayError(status);
			if(result)
			{
				m_ListInfo.AddString(g_MessageArr[210]);//_T("X-ray 발생장치 상태 : 정상"));
				m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
			}
			else
			{
				m_ListInfo.AddString(g_MessageArr[211]+m_ErrorName);
				m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
			}
		}
	}
	else
	{
		m_ListInfo.AddString(g_MessageArr[205]);//_T("X-ray 발생장치 : 연결되지 않음"));
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
	}
}


void CMaintenance::OnBnClickedBtXrayReset()
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();

	if(g_bGenerator)
	{
		if(NSE_XRAY_KIND==3)
		{
			g_Bit_Out[14]=1;
			OnDigitalOutPut_All();
			Sleepms(500);
			g_Bit_Out[14]=0;
			OnDigitalOutPut_All();
			Sleepms(500);
		}
		else
		{
			pCtrlView->m_GComm->setClear();
		}
		m_ListInfo.AddString(g_MessageArr[212]);//_T("X-ray 발생장치 리셋 완료"));
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
	}	
}


void CMaintenance::OnBnClickedBtXrayOnoff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();

	UpdateData(true);
	if(b_XrayOnOff)
	{
		OnTurnonXray();
		m_ListInfo.AddString(g_MessageArr[213]);//_T("X-ray 발생장치 X-ray 켜짐"));
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
			
	}	
	else
	{	
		OnTurnoffXray();
		m_ListInfo.AddString(g_MessageArr[214]);//_T("X-ray 발생장치 X-ray 꺼짐"));
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
			
	}		
}

void CMaintenance::OnTurnoffXray(void)
{
	
	if(g_bGenerator)
	{
		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();

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
			if(pCtrlView->m_GComm->isXRayOn())
				pCtrlView->m_GComm->setXRay(false);
		}
		KillTimer(1);
		b_XrayPower=false;
		SetDlgItemInt(IDC_DISPXRAYKV, 0);
		SetDlgItemInt(IDC_DISPXRAYUA, 0);
		SetDlgItemText(IDC_BT_XRAY_ONOFF, _T("X-ray ON"));
		m_BtXray.SetCheck(false);
		b_XrayOnOff=false;
	
	}
}


void CMaintenance::OnTurnonXray(void)
{
	if(g_bGenerator)
	{
		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();
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
			if(!pCtrlView->m_GComm->isXRayOn())
			{
				if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
				{
					pCtrlView->m_GComm->setKV(g_Xray_kv);
					Sleepms(200);
					if(NSE_XRAY_KIND==2)
						pCtrlView->m_GComm->setMA(g_Xray_ma);
					else
						pCtrlView->m_GComm->setMA(g_Xray_ma);
					Sleepms(200);
					pCtrlView->m_GComm->setXRay(true);
					Sleepms(2000);// 5초정도의 여유시간을 주자
				}
				else 
				{
					pCtrlView->m_GComm->setKV(g_Xray_kv);
					Sleepms(500);
					pCtrlView->m_GComm->setMA(g_Xray_ma);
					Sleepms(500);
					pCtrlView->m_GComm->setXRay(true);
					Sleepms(1000);
					pCtrlView->m_GComm->setXRay(true);
				}
			}
		}
		SetTimer(1, 6000, NULL);
		b_XrayPower=false;
		SetDlgItemText(IDC_BT_XRAY_ONOFF, _T("X-ray OFF"));
		m_BtXray.SetCheck(true);
		b_XrayOnOff=true;
		
	}
}


void CMaintenance::OnBnClickedBtXrayWarmup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다
	
	if(g_bGenerator)
	{
		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();

		if(NSE_XRAY_KIND == 3)
			pCtrlView->XrayWarmUp_Rtec();
		else
			pCtrlView->XrayWarmUp();
		m_ListInfo.AddString(g_MessageArr[215]);//_T("X-ray 발생장치 워밍업 완료"));
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
	}
}


void CMaintenance::OnBnClickedBtDtNormal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();

	if(m_Interock_State || !g_bGenerator || !g_bDetector)	
	{
		m_ListInfo.AddString(g_MessageArr[216]);//_T("디텍터 감도보정 실패"));
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
		return;
	}

	if(g_bGenerator)
	{
		if(NSE_XRAY_KIND==3)
		{
			OnDigitalInPut_All();
			if(!pCtrlView->CheckXrayError(1, 0))
			{
				OnTurnoffXray();				
			}
		}
		else
		{
			pCtrlView->m_GComm->setClear();
			int status = pCtrlView->m_GComm->status();
			if(status!=0)
			{
				if(!pCtrlView->OnTurnonXrayState())
				{
					int status = pCtrlView->m_GComm->status();
					if(!pCtrlView->CheckXrayError(1, status))
					{
							
					}
							
					OnTurnoffXray();
					return;
				}
			}
		}			
	}
 
	CInitailDlg d_Progress;	
	d_Progress.Create();
	d_Progress.b_IsUse=true;
	d_Progress.ShowWindow(SW_SHOW);
		

	d_Progress.StepIt(0);

	d_Progress.m_Text=g_MessageArr[46];//"감도보정 중...";

	d_Progress.StepIt(20);
	
//	pCtrlView->OnAutoTeaching(); //5000 모델 감도보정시 오토 티칭 진행 20181206

	Sleepms(10000);
		
    OnNormalization();

	d_Progress.StepIt(50);

	d_Progress.b_IsUse=false;
	d_Progress.StepIt(100); 	
	d_Progress.m_Text=g_MessageArr[47];//"감도보정이 완료 되었습니다."; 	
	d_Progress.DestroyWindow();
	
	m_ListInfo.AddString(g_MessageArr[217]);//_T("디텍터 감도보정 완료"));
	m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
	Sleepms(2000);

	CheckDetectorCell();
}


void CMaintenance::OnNormalization(void)
{
	if(!(g_bGenerator && g_bDetector))
		return; 
	
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
	CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();

	int m_nStart, m_nEnd;
	m_nStart=0;
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
	OnTurnonXray();
	
		Sleepms(1000);
		if(NSE_XRAY_KIND==1)
			pCtrlView->m_GComm->isXRayOn();
		pView->m_Commander.PutCorrectionOffset(FALSE);
		pView->m_Commander.PutCorrectionGain(FALSE);
		pView->m_Commander.PutCorrectionOffset(FALSE);
		pView->m_Commander.PutCorrectionGain(FALSE);
		Sleepms(200);
		if(NSE_XRAY_KIND==1)
			pCtrlView->m_GComm->isXRayOn();
		pView->m_Commander.PutStartPixel(m_nStart);
		pView->m_Commander.PutEndPixel(m_nEnd);
		pView->m_Commander.OnBoardGainCalibration(13180);
		Sleepms(500);
		pView->m_Commander.PutCorrectionGain (TRUE);
		pView->m_Commander.PutCorrectionGain (TRUE);
		pView->m_Commander.PutCorrectionOffset (TRUE);
		Sleepms(500);
	
		if(NSE_XRAY_KIND==1)
			pCtrlView->m_GComm->isXRayOn();
}

void CMaintenance::OnBnClickedBtDtReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
	CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();

	if(g_bDetector)
	{
		CInitailDlg d_Progress;
		d_Progress.Create();
		d_Progress.b_IsUse=true;
		d_Progress.ShowWindow(SW_SHOW);
		
		d_Progress.StepIt(0);
		d_Progress.m_Text=g_MessageArr[218];//"디텍터 초기화 진행중...");//"감도보정 중...";
		d_Progress.StepIt(10);
		OnTurnoffXray();
		Sleepms(500);
		d_Progress.StepIt(20);
		char Msg;
		pView->m_Detector.SendCommandA("[RO,C,1F4,A]",&Msg);
		Sleepms(3000);
		d_Progress.StepIt(50);
		//pView->m_Commander.ResetGain();
		//pView->m_Commander.ResetOffset();
		d_Progress.StepIt(60);
		pView->m_Commander.PutCorrectionOffset(FALSE);
		pView->m_Commander.PutCorrectionGain(FALSE);
		Sleepms(100);
		d_Progress.StepIt(80);
		pView->m_Commander.PutStartPixel(0);
		pView->m_Commander.PutEndPixel(g_Width-1);
		pView->m_Commander.OnBoardOffsetCalibration();
		pView->m_Commander.PutCorrectionOffset(TRUE);
		pView->m_Commander.PutCorrectionGain(FALSE);
		d_Progress.b_IsUse=false;
		d_Progress.StepIt(100);
		d_Progress.DestroyWindow();

		
		m_ListInfo.AddString(g_MessageArr[219]);//_T("디텍터 초기화 완료"));
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
	}
}


void CMaintenance::OnBnClickedBtDtSavecheckimage()
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();
	CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();

	if(g_bDetector)
	{
		TCHAR toPath[MAX_PATH+2];
		BROWSEINFO bi = { 0 };
		bi.lpszTitle = g_MessageArr[220];//;
		LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

		if(pidl!= 0)
		{
			// get the name of the folder and put it in path
			SHGetPathFromIDList(pidl, toPath);
			SetCurrentDirectory(toPath);
			m_SavePath=toPath;

			if(m_SavePath.Right(1)==_T("\\"))
				m_SavePath.Replace(_T("\\"), _T(""));
			CString	m_SaveFile;
			SYSTEMTIME cur_time;
			GetLocalTime(&cur_time);
			m_SaveFile.Format(_T("%s\\%04d%02d%02d_%02d%02d%02d"), m_SavePath, cur_time.wYear, cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond);
			
			CInitailDlg d_Progress;
			d_Progress.Create();
			d_Progress.b_IsUse=true;
			d_Progress.ShowWindow(SW_SHOW);
		
			d_Progress.StepIt(0);
			d_Progress.m_Text=g_MessageArr[221]+_T("(1/3)...");
			d_Progress.StepIt(10);
			OnTurnoffXray();
			Sleepms(500);
			char Msg;
			pView->m_Detector.SendCommandA("[RO,C,1F4,A]",&Msg);
			Sleepms(3000);
			d_Progress.StepIt(20);
	//		pView->m_Commander.ResetGain();
	//		pView->m_Commander.ResetOffset();
			pView->m_Commander.PutCorrectionOffset(FALSE);
			pView->m_Commander.PutCorrectionGain(FALSE);
			Sleepms(100);
			pView->m_Commander.PutStartPixel(0);
			pView->m_Commander.PutEndPixel(g_Width-1);
			pView->m_Commander.OnBoardOffsetCalibration();
			pView->m_Commander.PutCorrectionOffset(TRUE);
			pView->m_Commander.PutCorrectionGain(FALSE);

			Sleepms(3000);
			OnSaveDocument(m_SaveFile+_T("_1.jpg")); 
			OnSaveDocument(m_SaveFile+_T("_1.txt"));

			d_Progress.StepIt(30);

			d_Progress.m_Text=g_MessageArr[221]+_T("(2/3)...");
			OnTurnonXray();
			Sleepms(1000);
			d_Progress.StepIt(40);
			Sleepms(3000);
			OnSaveDocument(m_SaveFile+_T("_2.jpg")); 
			OnSaveDocument(m_SaveFile+_T("_2.txt"));

			d_Progress.StepIt(60);
			d_Progress.m_Text=g_MessageArr[221]+_T("(3/3)...");
			OnNormalization();
			d_Progress.StepIt(80);
			Sleepms(3000);
			OnSaveDocument(m_SaveFile+_T("_3.jpg")); 
			OnSaveDocument(m_SaveFile+_T("_3.txt"));
		    d_Progress.m_Text=g_MessageArr[222];//;_T("이미지 저장이 완료되었습니다.");
			d_Progress.b_IsUse=false;
			d_Progress.StepIt(100);
			d_Progress.DestroyWindow();
			
			m_ListInfo.AddString(g_MessageArr[223]);//;_T("점검영상 저장 완료"));
			m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
		}
		else
		{
			m_SavePath="";
		}
	}
}


void CMaintenance::OnBnClickedBtDtSavenowimage()
{
	TCHAR toPath[MAX_PATH+2];
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = g_MessageArr[220];//_T("저장할 경로를 선택하세요.");
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if(pidl!= 0)
	{
		// get the name of the folder and put it in path

		SHGetPathFromIDList(pidl, toPath);
		SetCurrentDirectory(toPath);
		m_SavePath=toPath;

		if(m_SavePath.Right(1)==_T("\\"))
			m_SavePath.Replace(_T("\\"), _T(""));
		
		CString	m_SaveFile;
		SYSTEMTIME cur_time;
		CInitailDlg d_Progress;
		d_Progress.Create();
		d_Progress.b_IsUse=true;
		d_Progress.ShowWindow(SW_SHOW);
		
		d_Progress.StepIt(0);
		d_Progress.m_Text=g_MessageArr[221]+_T("...");
		d_Progress.StepIt(10);
		
		GetLocalTime(&cur_time);
		m_SaveFile.Format(_T("%s\\%04d%02d%02d_%02d%02d%02d%03d.jpg"), m_SavePath, cur_time.wYear, cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);
		OnSaveDocument(m_SaveFile);
		d_Progress.StepIt(50);
		m_SaveFile.Format(_T("%s\\%04d%02d%02d_%02d%02d%02d%03d.txt"), m_SavePath, cur_time.wYear, cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);
		OnSaveDocument(m_SaveFile);
		d_Progress.b_IsUse=false;
		d_Progress.StepIt(100);
		d_Progress.DestroyWindow();
		
		m_ListInfo.AddString(g_MessageArr[224]);//_T("현재영상 저장 완료"));
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
	}
	else
	{
		m_SavePath=_T("");
	}
}


void CMaintenance::OnBnClickedBtIoConveyor()
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();

	if(g_bIOcard)
	{
		UpdateData(true);
		if(b_ConveyorOnOff)
		{
			pCtrlView->OnStartConveyor();
			SetDlgItemText(IDC_BT_IO_CONVEYOR, g_MessageArr[200]);
		}
		else
		{
			pCtrlView->OnStopConveyor();
			SetDlgItemText(IDC_BT_IO_CONVEYOR, g_MessageArr[196]);
		}
	}
	else
	{
		//SetWindowText("I/O card 연결이 실패 하였습니다");
	}

}


void CMaintenance::OnBnClickedBtIoReject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(g_bIOcard)
	{
		UpdateData(true);
		if(b_RejectOnOff)
		{
			if(NSE_IO_OUPUT_PORT==2)
			{
				g_Bit_Out[1]=1;
				g_Bit_Out[2]=1;
				OnDigitalOutPut_All();
			}
			else
			{
				//20200525
				g_portWriteData = 0x02;
				ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
				if(error)
					return;
				//20200525

		/*
				USHORT      gwMask = NSE_REJECT_MASKING;                  
				USHORT      gwData = NSE_REJECT_ON;                  
				m_ptDioWritePortByte.port  = NSE_IO_OUPUT_PORT;
				m_ptDioWritePortByte.mask  = gwMask;
				m_ptDioWritePortByte.state = gwData;

				if((DRV_DioWritePortByte(m_DriverHandle,
						 (LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
				{
					DRV_DioWritePortByte(m_DriverHandle,(LPT_DioWritePortByte)&m_ptDioWritePortByte);
				}*/
			}
			SetDlgItemText(IDC_BT_IO_REJECT, g_MessageArr[201]);

		}
		else
		{
			if(NSE_IO_OUPUT_PORT==2)
			{
				g_Bit_Out[1]=0;
				g_Bit_Out[2]=0;
				OnDigitalOutPut_All();
			}
			else
			{//20200525
				g_portWriteData = 0x00;
				ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
				if(error)
					return;
				//20200525
			
				/*
				USHORT gwMask = NSE_REJECT_MASKING;                  
				USHORT gwData = NSE_REJECT_OFF;
				m_ptDioWritePortByte.port  = NSE_IO_OUPUT_PORT;
				m_ptDioWritePortByte.mask  = gwMask;
				m_ptDioWritePortByte.state = gwData;

				if((DRV_DioWritePortByte(m_DriverHandle,
						 (LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
				{
					DRV_DioWritePortByte(m_DriverHandle,(LPT_DioWritePortByte)&m_ptDioWritePortByte);
				}
				*/
			}
			SetDlgItemText(IDC_BT_IO_REJECT, g_MessageArr[197]);
		}
	}
}


void CMaintenance::OnBnClickedBtIoYellowlamp()
{
	if(g_bIOcard)
	{
		UpdateData(true);
		if(b_YellowlampOnOff)
		{
			if(NSE_IO_OUPUT_PORT==2)
			{
				g_Bit_Out[3]=0;
				OnDigitalOutPut_All();
			}
			else
			{//20200525
				g_portWriteData = 0x00;
				ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
				if(error)
					return;
				//20200525
				/*
				USHORT gwMask = 0x08;
				if(NSE_IO_OUPUT_PORT==1)
					gwMask=0x04;
				USHORT gwData = 0x00;
		
				m_ptDioWritePortByte.port = NSE_IO_OUPUT_PORT;
				m_ptDioWritePortByte.mask  = gwMask;
				m_ptDioWritePortByte.state = gwData;

				if((DRV_DioWritePortByte(m_DriverHandle,
						 (LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
				{
				
				}*/
			}
			SetDlgItemText(IDC_BT_IO_YELLOWLAMP, g_MessageArr[202]);
		}
		else
		{
			if(NSE_IO_OUPUT_PORT==2)
			{
				g_Bit_Out[3]=1;
				OnDigitalOutPut_All();
			}
			else
			{//20200525
				g_portWriteData = 0x04;
				ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
				if(error)
					return;
				//20200525
			
				/*
				USHORT gwMask = 0x08;                  
				if(NSE_IO_OUPUT_PORT==1)
					gwMask=0x04;
				USHORT gwData = NSE_YELLO_ON;
		
				m_ptDioWritePortByte.port = NSE_IO_OUPUT_PORT;
				m_ptDioWritePortByte.mask  = gwMask;
				m_ptDioWritePortByte.state = gwData;
				if((m_ErrCde = DRV_DioWritePortByte(m_DriverHandle,
						 (LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
				{
		
				}*/
			}
			SetDlgItemText(IDC_BT_IO_YELLOWLAMP, g_MessageArr[198]);
		}
	}

}


void CMaintenance::OnBnClickedBtIoGreenlamp()
{
	if(g_bIOcard)
	{
		UpdateData(true);
		if(b_GreenlampOnOff)
		{
			if(NSE_IO_OUPUT_PORT==2)
			{
				g_Bit_Out[4]=1;
				OnDigitalOutPut_All();
			}
			else
			{//20200525
				g_portWriteData = 0x08;
				ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
				if(error)
					return;
				//20200525
				/*
				USHORT gwMask = 0x16; 
				if(NSE_IO_OUPUT_PORT==1)
						gwMask = 0x08;
				USHORT gwData = NSE_GREEN_ON;
				m_ptDioWritePortByte.port = NSE_IO_OUPUT_PORT;

				m_ptDioWritePortByte.mask  = gwMask;
				m_ptDioWritePortByte.state = gwData;

				if((DRV_DioWritePortByte(m_DriverHandle,
						 (LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
				{
				}
				*/
			}
			SetDlgItemText(IDC_BT_IO_GREENLAMP, g_MessageArr[203]);
		}
		else
		{
			if(NSE_IO_OUPUT_PORT==2)
			{
				g_Bit_Out[4]=0;
				OnDigitalOutPut_All();
			}
			else
			{//20200525
				g_portWriteData = 0x00;
				ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
				if(error)
					return;
				//20200525
				/*
				USHORT gwMask = 0x16;                  
				if(NSE_IO_OUPUT_PORT==1)
						gwMask=0x08;
				USHORT gwData = 0x00;

				m_ptDioWritePortByte.port = NSE_IO_OUPUT_PORT;

				m_ptDioWritePortByte.mask  = gwMask;
				m_ptDioWritePortByte.state = gwData;

				if((DRV_DioWritePortByte(m_DriverHandle,
						 (LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
				{
				}
				*/
			}
			SetDlgItemText(IDC_BT_IO_GREENLAMP, g_MessageArr[199]);
		}
	}
}

 bool CMaintenance::CheckXrayError(int Result)
{
	int LogNum=0;
	m_ErrorName=_T("");
	if(NSE_XRAY_KIND==3)
	{
		if(g_Bit_In[12] == 1)
		{
			LogNum=1003;
			m_ErrorName=_T("High mA");
		}
		if(g_Bit_In[14] == 1)
		{
			LogNum=1009;
			m_ErrorName=_T("Interlock Open");
		}
		if(g_Bit_In[15] == 1)
		{	  
			LogNum=1001;
			m_ErrorName=_T("Temp High");
		}
		if(LogNum!=0)
		{
			g_iXrayOn=LogNum;
			return false;
		}
	}
	else if(NSE_XRAY_KIND==0 || NSE_XRAY_KIND==2)
	{
		if(Result == 1)
		{
			m_ErrorName=_T("Temp High");
			LogNum=1001;
		}
		else if(Result == 2)
		{
			m_ErrorName=_T("Arc Fault");
			LogNum=1002;
		}
		else if(Result == 3)
		{
			m_ErrorName=_T("High mA");
			LogNum=1003;
		}
		else if(Result == 4)
		{	
			m_ErrorName=_T("Low mA");
			LogNum=1004;
		}
		else if(Result == 5)
		{
			m_ErrorName=_T("Low kV");
			LogNum=1005;
		}
		else if(Result == 6)
		{
			m_ErrorName=_T("High kV");
			LogNum=1006;
		}
		else if(Result == 7)
		{
			m_ErrorName=_T("Watchdog Timer Expired");
			LogNum=1007;
		}
		else if(Result == 8)
		{
			m_ErrorName=_T("Power Limit");
			LogNum=1008;
		}

		if(LogNum!=0)
		{
			g_iXrayOn=LogNum;
			return false;
		}
	}	
	else if(NSE_XRAY_KIND==1)
	{
		if(Result == 1) //X0
		{
			m_ErrorName=_T("X0 Regulation");
			LogNum=1001;
		}
		else if(Result == 2) //X1
		{
			m_ErrorName=_T("X1 Interlock Open");
			LogNum=1002;
		}
		else if(Result == 3) //X2
		{
			m_ErrorName=_T("X2 Cathode Over kV");
			LogNum=1003;
		}
		else if(Result == 4)//X3
		{	
			m_ErrorName=_T("X3 Anode Over kV");
			LogNum=1004;
		}
		else if(Result == 5)//X4
		{
			m_ErrorName=_T("X4 Temp High");
			LogNum=1005;
		}
		else if(Result == 6)//X5
		{
			m_ErrorName=_T("X5 Arc Fault");
			LogNum=1006;
		}
		else if(Result == 7)//X6
		{
			m_ErrorName=_T("X6 Over Current");
			LogNum=1007;
		}
		else if(Result == 8)//X7
		{
			m_ErrorName=_T("X7 Power Limit");
			LogNum=1008;
		}
		else if(Result == 9)//X8
		{
			m_ErrorName=_T("X8 Over Voltage");
			LogNum=1009;
		}	
		if(LogNum!=0)
		{
			g_iXrayOn=LogNum;
			return false;
		}
	}
	return true;
}

void CMaintenance::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent==1)
	{
		if(g_bGenerator)
		{
			if(NSE_XRAY_KIND==3)
			{
				m_DispXraykV = OnXray_kv_Output();
				m_DispXrayuA = OnXray_ma_Output();
			}
			else
			{
				CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
				CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();
				m_DispXraykV = pCtrlView->m_GComm->getKV();
				Sleep(500);
				m_DispXrayuA = pCtrlView->m_GComm->getMA();
			}
			SetDlgItemInt(IDC_DISPXRAYKV, m_DispXraykV);
			SetDlgItemInt(IDC_DISPXRAYUA, m_DispXrayuA);
			if(m_DispXraykV<10 && m_DispXrayuA<100 && !b_XrayPower)
			{
				b_XrayPower=true;
				m_ListInfo.AddString(g_MessageArr[211]+_T(" Low Power"));//_T("X-ray 발생장치 에러발생 : "));
				m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
			}

		}
		else
		{
			SetDlgItemInt(IDC_DISPXRAYKV, 0);
			SetDlgItemInt(IDC_DISPXRAYUA, 0);
		}
	}
	
	if(nIDEvent==2)
	{
		if(g_bIOcard)
		{
			int temp_state=m_Interlock;
			CString m_ErrorMsg=_T("");
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
				/*
				m_ptDioReadPortByte.port = 0;
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
					if(g_Bit_In[1]==0)//비상좌
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[86];//_T("인터락 발생 : 비상스위치 눌림");
					}
					else if(g_Bit_In[4]==0)//후문
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[91];//_T("인터락 발생 : 뒷문 열림");
					}
					else if(g_Bit_In[7]==1 && g_HandsensorLU==1) 
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[92];//_T("인터락 발생 : 좌측 손감지");
					}
					else if(g_Bit_In[9]==1 && g_HandsensorRU==1)
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[94];//_T("인터락 발생 : 우측 손감지");
					}
					else 
						m_Interlock=false;
	
				}
				else
				{
					if(g_Bit_In[0] == 0) //앞문
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[85];//_T("인터락 발생 : 앞문열림");
					}
					else if(g_Bit_In[1]==0)//비상좌
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[86];//_T("인터락 발생 : 비상스위치 눌림");
					}
					else if(g_Bit_In[2]==0)//비상우
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[87];//_T("인터락 발생 : 비상스위치 눌림");
					}
					else if(g_Bit_In[3]==0)//모니터커버
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[90];//_T("인터락 발생 : 모니터문 열림");
					}
					else if(g_Bit_In[4]==0)//후문
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[91];//_T("인터락 발생 : 뒷문 열림");
					}
					else if(g_Bit_In[5]==0 && g_KindModel==4)//확장터널좌
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[88];//_T("인터락 발생 : 좌측 확장터널 열림");
					}
					else if(g_Bit_In[6]==0 && g_KindModel==4)//확장터널우
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[89];//_T("인터락 발생 : 우측 확장터널 열림");
					}
					else if(g_Bit_In[7]==1 && g_HandsensorLU==1) 
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[92];//_T("인터락 발생 : 좌측 손감지");
					}
					else if(g_Bit_In[8]==1 && g_HandsensorLD==1)
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[93];//_T("인터락 발생 : 좌측 손감지");
					}
					else if(g_Bit_In[9]==1 && g_HandsensorRU==1)
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[94];//_T("인터락 발생 : 우측 손감지");
					}
					else if(g_Bit_In[10]==1 && g_HandsensorRD==1)
					{
						m_Interlock=true;
						m_ErrorMsg=g_MessageArr[95];//_T("인터락 발생 : 우측 손감지");
					}
					else 
						m_Interlock=false;
				}
			}
			else //기존보드
			{
				if(gwValue==0)//앞문
				{
					m_Interlock=true;
					m_ErrorMsg=g_MessageArr[85];//_T("인터락 발생 : 앞문열림");
				}
				else if (gwValue == 1)//비상좌
				{
					m_Interlock=true;
					m_ErrorMsg=g_MessageArr[86];//_T("인터락 발생 : 비상스위치 눌림");
				}
				else if (gwValue == 3)//비상우
				{
					m_Interlock=true;
					m_ErrorMsg=g_MessageArr[87];//_T("인터락 발생 : 비상스위치 눌림");
				}
				else if (gwValue == NSE_INTEROCK_BACK)//후문
				{
					m_Interlock=true;
					m_ErrorMsg=g_MessageArr[91];//_T("인터락 발생 : 뒷문 열림");

				}
				else if (gwValue == NSE_INTEROCK_EX_LEFT)//확장터널좌
				{
					m_Interlock=true;
					m_ErrorMsg=g_MessageArr[88];//_T("인터락 발생 : 좌측 확장터널 열림");
				}
				else if (gwValue == NSE_INTEROCK_EX_RIGHT)//확장터널우
				{
					m_Interlock=true;
					m_ErrorMsg=g_MessageArr[89];//_T("인터락 발생 : 우측 확장터널 열림");
				}
				else if (gwValue == NSE_INTEROCK_MONITOR)//모니터커버
				{
					m_Interlock=true;
					m_ErrorMsg=g_MessageArr[90];//_T("인터락 발생 : 모니터문 열림");
				}
				else 
					m_Interlock=false;
			}

			if(m_Interlock)
			{
				if(!temp_state)
				{
					m_ListInfo.AddString(m_ErrorMsg);
					m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
				}
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CMaintenance::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


void CMaintenance::DrawImage(void)
{
	CDC *pDC = m_GrabImage.GetDC();			
	CRect arect;
	m_GrabImage.GetWindowRect(&arect);
	pDC->SetStretchBltMode(COLORONCOLOR);
	::StretchDIBits(pDC->m_hDC,0,0, arect.Width(), arect.Height(), 0, 0, g_Height, g_Height, m_Buffer_3, BmInfo, DIB_RGB_COLORS,SRCCOPY);  
	m_ImageCount++;
	CString ss;
	ss.Format(_T("%d"), m_ImageCount);
	GetDlgItem(IDC_DISPFRAMECOUNT)->SetWindowTextW(ss);


	CRect rect;
	m_PicGraph.GetClientRect(&rect);

	int i, j, count=1;
	//int m_GraphVal[1024];
	ClearPicGrid();
	pDC = m_PicGraph.GetDC();
	int sum=0;
	for(i=0;i<g_Height;i++)
	{
		sum=0;
		for(j=0; j<g_Height;j++)
		{
			sum+=g_Img_Buf[g_3D_Count][i][j];
		}
		if(g_Width!=0)
			sum/=g_Height;
		if(sum<1)
			sum=1;
		if(sum>254)
			sum=254;
		pDC->SetPixel(rect.Width()*i/g_Height, rect.Height()*(255-sum)/255, RGB(255, 100, 0));
	}
	ReleaseDC(pDC);
	ComputeNoise();	
}


void CMaintenance::ComputeNoise()
{
	int i=0, j=0;
	int sub=0;
	double phyo=0.0;
	int sum=0;
	int count=0;
	for(i=50;i<g_Height-10;i++)
	{
		for(j=10; j<g_Height-10;j++)
		{
			//sum+=g_Img_Buf[g_3D_Count][i][j];
			sum+=g_Img_temp[i][j];
			count++;
		}
	}
	if(count!=0)
		sum/=count;

	for(i=50;i<g_Height-10;i++)
	{
		for(j=10; j<g_Height-10;j++)
		{
			//sub=g_Img_Buf[g_3D_Count][i][j]-sum;
			sub=g_Img_temp[i][j]-sum;
			sub=sub*sub;
			phyo+=sub;
		}
	}

	phyo=phyo/((g_Height-20)*(g_Height-20));
	CString disp;
	disp.Format(_T("%0.1f"), sqrt(phyo)*10);
	GetDlgItem(IDC_DISPQAULITYVALUE)->SetWindowText(disp);
}

void CMaintenance::ClearPicGrid()
{
	CRect rect;
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_GRAPHGRID);
	CClientDC dc(this);
	CDC bmDC;
	bmDC.CreateCompatibleDC(&dc);
	bmDC.SelectObject(&bmp);
	m_PicGraph.GetWindowRect(&rect);
	ScreenToClient(&rect);
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),&bmDC,0,0, SRCCOPY);
	bmp.DeleteObject();
	bmDC.DeleteDC();
}

void CMaintenance::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	wchar_t *ext = (wchar_t *)wcsstr(lpszPathName,_T("."));
	ext=wcslwr(ext);


	if(!wcscmp(ext, _T(".jpg")))
	{
		//cximage
		//CxImage ximg(g_Height, g_Height, 8, CXIMAGE_FORMAT_JPG);
		//BYTE* pBaseTo;
		//pBaseTo = ximg.GetBits();
		//for(int i=0;i<g_Height*g_Height;i++)
		//{
		//	*(pBaseTo+i) = m_Buffer_3[i];  
		//}
		//ximg.SetGrayPalette();
		//ximg.Save(lpszPathName,ximg.GetType());

		cv::Mat mImg(g_Height, g_Height, CV_8UC1, m_Buffer_3);
		NSE::Save(mImg, lpszPathName);
	}
	else if(!wcscmp(ext, _T(".txt")))
	{
		CStdioFile fileString;
		CString Data=_T(""), lineData=_T("");
		if(fileString.Open(lpszPathName, CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
		{
			Data=_T("");
			lineData=_T("");
			for(int i=0;i<g_Height*g_Height;i++)
			{
				if(m_Buffer_3[i]==0)
					Data=_T("0\t");
				else
					Data.Format(_T("%d\t"), m_Buffer_3[i]);
				lineData+=Data;
				if((i+1)%g_Height==0)
				{
					lineData+=_T("\n");
					fileString.WriteString(lineData);
					lineData=_T("");
				}
			}
			fileString.Close();
		}
		
	}

}

UINT ThreadDrawImage(LPVOID lParam)
{
	CMaintenance *pDlg;
	pDlg=(CMaintenance *)lParam;
	
	while(b_ThreadDrawImage)
	{
		g_EventDrawImage.Lock();
		if(b_ThreadDrawImage)
			pDlg->DrawImage();
	}
	return 0;
}

void CMaintenance::CheckDetectorCell(void)
{
	int i=0, j=0;
	int sum=0;
/*	int BadCell[4096];
	for(i=0; i<4096; i++)
		BadCell[i]=0;*/
	int BadDt[32];
	for(i=0; i<32; i++)
		BadDt[i]=0;
	bool b_IsBadline=false;

	for(i=50;i<g_Height-10;i++)
	{
	//	if(g_Img_Buf[g_3D_Count][i][g_Height/2]<185)
		if(g_Img_temp[i][g_Height/2]<185)
		{
		//	BadCell[i]=1;
			BadDt[i/128]=1;
			b_IsBadline=true;
		}
	}
	CString m_Msg=_T(""), temp=_T("");
	if(b_IsBadline)
	{
		for(i=0; i<32; i++)
		{
			if(BadDt[i]==1)
			{
				temp.Format(_T("%d "), i+1);
				m_Msg+=temp;
			}
		}
		m_ListInfo.AddString(g_MessageArr[225] + m_Msg);
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
	}
	else
	{
		m_ListInfo.AddString(g_MessageArr[226]);//;_T("디텍터 셀 상태 : 정상"));
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
	}
}


BOOL CMaintenance::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(b_GreenlampOnOff)
	{
		b_GreenlampOnOff=false;
		OnBnClickedBtIoGreenlamp();
	}

	KillTimer(1);
	KillTimer(2);

	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();	

	if(b_XrayOnOff)
	{
		OnTurnoffXray();
	}

	Sleepms(100);

	g_XrayState=0;		

	b_ThreadDrawImage = false;
	g_EventDrawImage.PulseEvent();
	Sleepms(1000);


	if(g_bDetector)
	{
		pView->b_IsNormalization=0;
		if(pView->m_DTImage.GetIsOpened())
		{
			pView->m_DTImage.Grab(NSE_TRIGER_HEIGHT); 
		} 

		Sleepms(100);
	}

	return CDialogEx::DestroyWindow();
}


void CMaintenance::OnBnClickedBtIoEncoder() //부품점검 엔코더 정상 동작상태 추가 20181119
{
	ShellExecute(this->m_hWnd, _T("open"), _T("C:\\Program Files\\COMIZOA\\AUTOMATION3\\COMMON\\Apps\\BasicApp\\Encoder_Counter.exe"), NULL, NULL, NULL);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMaintenance::OnBnClickedBtIoReject2()
{
	if(g_bIOcard)
	{
		if(NSE_IO_OUPUT_PORT==2)
		{
			g_Bit_Out[1]=1;
			g_Bit_Out[2]=1;
			OnDigitalOutPut_All();
		}
		else
		{//20200525
			g_portWriteData = 0x02;
			ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
			if(error)
				return;
			//20200525
			/*
			USHORT      gwMask = NSE_REJECT_MASKING;                  
			USHORT      gwData = NSE_REJECT_ON;                  
			m_ptDioWritePortByte.port  = NSE_IO_OUPUT_PORT;
			m_ptDioWritePortByte.mask  = gwMask;
			m_ptDioWritePortByte.state = gwData;

			if((DRV_DioWritePortByte(m_DriverHandle,
						(LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
			{
				DRV_DioWritePortByte(m_DriverHandle,(LPT_DioWritePortByte)&m_ptDioWritePortByte);
			}*/
		}
			
		Sleep(g_RejectKeep);
	
		if(NSE_IO_OUPUT_PORT==2)
		{
			g_Bit_Out[1]=0;
			g_Bit_Out[2]=0;
			OnDigitalOutPut_All();
		}
		else
		{//20200525
			g_portWriteData = 0x00;
			ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
			if(error)
				return;
			//20200525
	
			/*
			USHORT gwMask = NSE_REJECT_MASKING;                  
			USHORT gwData = NSE_REJECT_OFF;
			m_ptDioWritePortByte.port  = NSE_IO_OUPUT_PORT;
			m_ptDioWritePortByte.mask  = gwMask;
			m_ptDioWritePortByte.state = gwData;

			if((DRV_DioWritePortByte(m_DriverHandle,
						(LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
			{
				DRV_DioWritePortByte(m_DriverHandle,(LPT_DioWritePortByte)&m_ptDioWritePortByte);
			}*/
		}
		
			
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMaintenance::OnBnClickedBtIoReject3()
{
	if(g_bIOcard)
	{
		int count_r = 0;
		bool m_flag = true;

		while(m_flag)
		{
			count_r++;
			if(NSE_IO_OUPUT_PORT==2)
			{
				g_Bit_Out[1]=1;
				g_Bit_Out[2]=1;
				OnDigitalOutPut_All();
			}
			else
			{//20200525
				g_portWriteData = 0x02;
				ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
				if(error)
					return;
			}
			
			Sleep(g_RejectKeep);
	
			if(NSE_IO_OUPUT_PORT==2)
			{
				g_Bit_Out[1]=0;
				g_Bit_Out[2]=0;
				OnDigitalOutPut_All();
			}
			else
			{//20200525
				g_portWriteData = 0x00;
				ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
				if(error)
					return;
			}
			Sleep(g_RejectKeep);
			if(count_r > 10)
				m_flag = false;
		}
			
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
