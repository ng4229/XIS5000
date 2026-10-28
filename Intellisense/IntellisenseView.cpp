
// IntellisenseView.cpp : CIntellisenseView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Intellisense.h"
#endif

#include "MainFrm.h"
#include "ChildFrm.h"
#include "IntellisenseDoc.h"
#include "IntellisenseView.h"
#include "MessageBoxDlg.h"
#include "AdminDlg.h"
#include "ConditionSetup.h"
#include "ExitDlg.h"
#include "../Viewer/ViewerDlg.h"

#include "ImageProcessing.h"
#include "Deep.h"

// C01B Detector
#include "ixcmd_sink.h"
#include "iximg_sink.h"




#include <queue> //0402
#include <mutex>







#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DT_SERIAL		0
#define DT_USB			1
#define DT_NET			2

// CIntellisenseView
static bool b_ThreadAlarm = false;
static bool b_ThreadPaint = false;
static bool b_ThreadReject = false;
static bool b_ThreadRecog = false;

CIntellisenseView* g_pIntellisenseView;
//Define the call back funcitons
void OnDTEvent(BYTE ID, BYTE Num)
{
	PostMessage(g_pIntellisenseView->m_hWnd, WM_EVENT, ID, Num);
}
void OnDTError(LONG ID, CHAR* Info)
{
	PostMessage(g_pIntellisenseView->m_hWnd, WM_ERROR, ID, (LPARAM)Info);
}
void OnFrameReady(ULONG_PTR imgDataAddress)
{
	PostMessage(g_pIntellisenseView->m_hWnd, WM_FRAME_READY, (WPARAM)imgDataAddress, 0);
}

IMPLEMENT_DYNCREATE(CIntellisenseView, CFormView)

BEGIN_MESSAGE_MAP(CIntellisenseView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MENU_EXIT, &CIntellisenseView::OnBnClickedMenuExit)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BTLOAD, &CIntellisenseView::OnBnClickedBtload)

	ON_MESSAGE(WM_EVENT, OnEventMsg)
	ON_MESSAGE(WM_ERROR, OnErrorMsg)
	ON_MESSAGE(WM_FRAME_READY, OnFrameReadyMsg)

	ON_MESSAGE(WMU_EVENT_C01B, OnEventMsgC01B)
	ON_MESSAGE(WMU_ERROR_C01B, OnErrorMsgC01B)
	ON_MESSAGE(WMU_FRAME_READY_C01B, OnFrameReadyMsgC01B)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CIntellisenseView 생성/소멸

// C01B
float temperature;
float humidity;
CString err_str;

class XCmdSink : public IXCmdSink
{
public:
	void OnXError(uint32_t err_id, const char* err_msg_) override
	{
		err_str = (CString)err_msg_;
		if (::IsWindow(g_pIntellisenseView->m_hWnd))
			PostMessage(g_pIntellisenseView->m_hWnd, WMU_ERROR_C01B, 0, 0);
	}
	void OnXEvent(uint32_t event_id, float data) override
	{
		//switch (event_id)
		//{
		//case 56:
		//	temperature = data;
		//	break;
		//case 57:
		//	humidity = data;
		//	break;
		//}
		if (::IsWindow(g_pIntellisenseView->m_hWnd))
			PostMessage(g_pIntellisenseView->m_hWnd, WMU_EVENT_C01B, event_id, 0);
	}
};

class XImgSink : public IXImgSink
{
public:
	void OnXError(uint32_t err_id, const char* err_msg_) override
	{
		err_str = (CString)err_msg_;
		
		if (::IsWindow(g_pIntellisenseView->m_hWnd))
			PostMessage(g_pIntellisenseView->m_hWnd, WMU_ERROR_C01B, 0, 0);
	}
	void OnXEvent(uint32_t event_id, uint32_t data) override
	{
		if (::IsWindow(g_pIntellisenseView->m_hWnd))
			PostMessage(g_pIntellisenseView->m_hWnd, WMU_EVENT_C01B, event_id, data);
	}
	void OnFrameReady(XImage* image_) override
	{
		
		const int W = image_->_width;
		const int H = image_->_height;
		const int lineHeader = g_pIntellisenseView->b_UseLineInfo ? 8 : 0;                // SDK에 맞춰 확인
		const int stride = W * 2 + lineHeader;

		//260810 CH 전용으로 예외처리 제거
		/*if (W != 1024 || H != 1)
		{
			return;
		}*/

		
		auto* copy = new uint16_t[W * H];        // 16-bit 깊은 복사 버퍼
		const uint8_t* src = (const uint8_t*)image_->_data_;

		for (int y = 0; y < H; ++y) {
			const uint8_t* line = src + y * stride + lineHeader;
			memcpy(copy + y * W, line, W * sizeof(uint16_t));
		}

		if (!::IsWindow(g_pIntellisenseView->m_hWnd)) {
			delete[] copy;
			return;
		}

		PostMessage(g_pIntellisenseView->m_hWnd, WMU_FRAME_READY_C01B, MAKELONG(W, H), reinterpret_cast<LPARAM>(copy));
	}
};

XCmdSink xcmd_sink;
XImgSink ximg_sink;

CIntellisenseView::CIntellisenseView()
	: CFormView(CIntellisenseView::IDD)
	//, m_Icount(0)
	, m_InternalCount(0) //20190327
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_pImageObject = NULL;

	BmInfo = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)];
	BmInfo->bmiHeader.biBitCount = 8;
	BmInfo->bmiHeader.biClrImportant = 0;
	BmInfo->bmiHeader.biCompression = 0;
	BmInfo->bmiHeader.biHeight = NSE_VAR_SIZE;
	BmInfo->bmiHeader.biClrUsed =256;
	BmInfo->bmiHeader.biPlanes = 1;
	BmInfo->bmiHeader.biSize = 40;
	BmInfo->bmiHeader.biSizeImage = NSE_VAR_SIZE*NSE_WIDTH_SIZE;
	BmInfo->bmiHeader.biWidth = NSE_WIDTH_SIZE;
	BmInfo->bmiHeader.biXPelsPerMeter = 2835;
	BmInfo->bmiHeader.biYPelsPerMeter = 2835;
	//here set the palette
	for(int i=0; i<256;i++)
	{
		BmInfo->bmiColors[i].rgbBlue =i;
		BmInfo->bmiColors[i].rgbGreen =i;
		BmInfo->bmiColors[i].rgbRed =i;
		BmInfo->bmiColors[i].rgbReserved = 0;
	}

	


	m_Count_Total=0;
	m_Count_Soup=0;
	m_Count_OK=0;
	m_Count_NG=0;
	m_ProductName=_T("");
	m_Result_mode=0;

	m_OmitAve=0;

	b_IsNormalization=0;
//	m_ConditionInfo="";
//	m_ConditionInfo1="";

	b_TestMode=true;
	m_CountCond1 = 0;
	m_CountCond2 = 0;
	m_CountCond3 = 0;
	m_CountCond4 = 0;
	m_CountCond5 = 0;
	m_CountCond6 = 0;
	b_ConveyorOn=false;
	b_StopPaint=false;

	for(int i=0; i<10000; i++)
	{
		g_BackupCond1[i]=0;
		g_BackupCond2[i]=0;
		g_BackupCond3[i]=0;
		g_BackupCond4[i]=0;
		g_BackupCond5[i]=0;
		g_BackupCond6[i]=0;
	}

	g_BackupFront = 0;
	g_BackupRear = 0;

	g_Height = NSE_VAR_SIZE;
	g_Width = NSE_WIDTH_SIZE;
	m_Height = NSE_TRIGER_HEIGHT;

	m_IsProduct=0;
	m_OmitAve=0;

	g_RejectFront=0;
	g_RejectRear=0;

	b_IsNormalization=0;

	m_ReportOmit=0;
	m_ReportNg=0;
	m_ReportOk=0;
	m_ReportTotal=0;

	g_AutoCount=0;
	g_AutoCond1=0;
	g_AutoCond2=0;
	g_AutoCond3=0;
	g_AutoCond4=0;
	g_AutoCond5=0;
	g_AutoCond6=0;

	g_AutoCond1_1=0;
	g_AutoCond2_1=0;
	g_AutoCond3_1=0;
	g_AutoCond4_1=0;
	g_AutoCond5_1=0;
	g_AutoCond6_1=0;
	g_OmitCondition3_Disp=0;
	//20160428
	g_MassCondition1_Disp=0;
	g_DispArea=false;

	for(int i=0; i<10; i++)
		m_BPMCount[i] = 0;
	m_BPMState = 0;

	//20160428
	g_DispOpenBox1=0;
	g_DispOpenBox2=0;

	g_bSavingCondition=false;

	for(int i=0; i<50; i++)
	{
		m_ConditionInfo[i]="";
		m_ConditionInfo1[i]="";
		m_ConditionInfo2[i]="";
	}

	g_bComiDaq1 = true;

	g_iRejectCounter = 0;
	g_ConReject = 0;

		//20200514
	b_CheckOverwrite=false;
	m_TempReportOk=0;
	m_TempReportNg=0;
	m_TempReportOmit=0;
	m_TempReportTotal=0;

	g_pIntellisenseView = this;

	Init_Mysql();

	// C01B
	//_xoff_correct_ = NULL;

	//_xsystem_ = &_xsystem;
	//_xcommand_ = &_xcommand;
	//_xacquisition_ = &_xacquisition;
	//_xtransfer_ = &_xtransfer;
	//_xfactory_ = &_xfactory;
	//_xpiecewise_correct_ = &_xpiecewise_correct;
	//_xoff_correct_ = _xoff_correct_;

	RegisterSink();

	g_SelDetector = NSE_DT_KIND;		// detector

	csDtErrorMsg = "";

	b_UseLineInfo = true;
}

CIntellisenseView::~CIntellisenseView()
{
}

void CIntellisenseView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TIME, m_Edit_Time);
	DDX_Control(pDX, IDC_EDIT_TIME3, m_Edit_Time3);
	DDX_Control(pDX, IDC_XRAY_STATE, m_PicXrayState);
	DDX_Control(pDX, IDC_MENU_EXIT, m_Menu_ExitBt);
	DDX_Control(pDX, IDC_GRAB_IMAGE, m_GrabImage);
	DDX_Control(pDX, IDC_PICPROGRESS, m_PicProgress);
	DDX_Control(pDX, IDC_OMITAVE, m_OmitaveEdit);
	DDX_Text(pDX, IDC_PRODUCT_NAME, m_ProductName);
	DDX_Text(pDX, IDC_CONDITION1_COUNT, m_CountCond1);
	DDX_Text(pDX, IDC_CONDITION2_COUNT, m_CountCond2);
	DDX_Text(pDX, IDC_CONDITION3_COUNT, m_CountCond3);
	DDX_Text(pDX, IDC_CONDITION4_COUNT, m_CountCond4);
	DDX_Text(pDX, IDC_CONDITION5_COUNT, m_CountCond5);
	DDX_Text(pDX, IDC_CONDITION6_COUNT, m_CountCond6);
	DDX_Control(pDX, IDC_XRAY_ANI, m_PicXrayAni);
	DDX_Control(pDX, IDC_RESULT, m_PicResult);
	DDX_Control(pDX, IDC_MAINCOND1, m_PicCond1);
	DDX_Control(pDX, IDC_MAINCOND2, m_PicCond2);
	DDX_Control(pDX, IDC_MAINCOND3, m_PicCond3);
	DDX_Control(pDX, IDC_MAINCOND4, m_PicCond4);
	DDX_Control(pDX, IDC_MAINCOND5, m_PicCond5);
	DDX_Control(pDX, IDC_MAINCOND6, m_PicCond6);
	DDX_Control(pDX, IDC_PICPROGRESS2, m_PicProgress2);

	DDX_Control(pDX, IDC_EDIT_EAPERMIN, m_Edit_Eapermin);

	DDX_Control(pDX, IDC_STATE_EN, m_Encoder_S);
}

BOOL CIntellisenseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}


void CIntellisenseView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();
	
	Brush.CreateStockObject(NULL_BRUSH);
	m_bkbrush.CreateSolidBrush(RGB(255,255,255)); 
	m_bkbrush1.CreateSolidBrush(RGB(3,15,57)); 
	m_bkbrush2.CreateSolidBrush(RGB(75,87,118)); 
	pen.CreatePen(PS_SOLID, 2, RGB(200, 0, 0));
	pen1.CreatePen(PS_SOLID, 2, RGB(255, 200, 0));
	pen2.CreatePen(PS_SOLID, 2, RGB(0, 200, 0));
	pen3.CreatePen(PS_SOLID, 2, RGB(0, 0, 200));
	pen4.CreatePen(PS_SOLID, 2, RGB(255, 100, 0));
	pen5.CreatePen(PS_SOLID, 2, RGB(9, 232, 238));
	pen6.CreatePen(PS_SOLID, 2, RGB(200, 0, 200));
	pen10.CreatePen(PS_SOLID, 2, RGB(255, 192, 203));
	LoadBackground();

	m_Font_Name.CreatePointFont(140, _T("맑은고딕")); 
	GetDlgItem(IDC_EDIT_TIME)->SetFont(&m_Font_Name);
	GetDlgItem(IDC_EDIT_TIME3)->SetFont(&m_Font_Name);
	GetDlgItem(IDC_EDIT_EAPERMIN)->SetFont(&m_Font_Name); 
	m_Font_Name.CreatePointFont(180, _T("맑은고딕")); 
	GetDlgItem(IDC_PRODUCT_NAME)->SetFont(&m_Font_Name); 

	HideCaret();

	//20191202
	g_SendKind = 0;

	if(NSE_DT_ISBULK==0)
	{
		iv_1 = g_Margin;
		iv_2 = g_Width-g_Margin;
	}
	else if(NSE_DT_ISBULK==1)
	{
		if(NSE_WIDTH_SIZE<600)
		{
			iv_1 = 64;
			iv_2 = 448;
		}
		else
		{
			iv_1 = 200;
			iv_2 = 584;
		}
	}
}

void CIntellisenseView::Initial_Detector(void)
{
	m_BytesPerPixel = 2;
	if (m_Detector.GetIsOpened())
		return;

	m_Detector.PutChannelType(DT_USB);
	m_Detector.PutCmdTimeOut(10000);
	m_Detector.SetCallback(&OnDTError);

	m_DTImage.PutDetectorObject(&m_Detector);
	m_DTImage.PutImgWidth(NSE_DETECTOR_WIDTH);
	m_DTImage.PutImgHeight(NSE_TRIGER_HEIGHT);

	m_DTImage.PutChannelType(DT_USB);
	m_DTImage.PutExFrameTrigger(0);
	m_DTImage.PutBytesPerPixel(m_BytesPerPixel);
	m_DTImage.PutStreamPixelPerBytes(2);
	m_DTImage.PutTimeOut(10000);
	m_DTImage.SetCallBack(&OnDTEvent, &OnDTError, &OnFrameReady);

	//m_DTImage.PutExFrameTrigger(0);
	m_ImgWidth = NSE_DETECTOR_WIDTH;
	m_ImgHeight = NSE_TRIGER_HEIGHT;

	if(m_Detector.Open())
	{ 
		m_DTImage.PutDetectorObject(&m_Detector);
		m_Commander.PutDetectorObject(&m_Detector);
		m_Commander.GetPixelNumber(&m_ImgWidth);

		m_DTImage.PutImgWidth(m_ImgWidth);

		m_ImgSize = m_ImgWidth * m_ImgHeight * m_BytesPerPixel;

		m_pImgBuf = new BYTE[m_ImgSize];

		if(m_DTImage.Open())
		{
			m_pImageObject = m_DTImage.GetImageObject();
			g_bDetector=true;
		}
		else
		{
			g_StopKind = 2; //20191202
			g_bDetector=false;
			OnWriteLog(100);
			CMessageBoxDlg dlg;
			dlg.m_Message=g_MessageArr[133];//_T("디텍터와 연결이 끊어졌습니다. USB 케이블 또는 아답터와 제대로 연결되었는지 확인하세요. (E2001)");
			dlg.DoModal();

			delete[]  m_pImgBuf;
			m_Detector.Close();
		}
	}
	else
	{
		g_StopKind = 2; //20191202
		m_pImageObject = NULL;
		g_bDetector=false;
		OnWriteLog(100);
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[133];//_T("디텍터와 통신할 수 없습니다. USB 케이블 또는 아답터와 제대로 연결되었는지 확인하세요. (E2001)");
		dlg.DoModal();
			
	}

	if(g_bDetector)
	{
		//m_Commander.PutOverallGain(1.5);
		m_Commander.PutOverallGain(2);
		m_Commander.PutDetectorObject(&m_Detector);
		m_Commander.PutOutputBits(8);
		m_Commander.PutSensitivityLevel(1);
	}
}

DWORD ConvertIP(DWORD IP)
{
	DWORD temp;
	temp = (BYTE)IP;
	temp <<= 8;
	temp |= (BYTE)(IP >> 8);
	temp <<= 8;
	temp |= (BYTE)(IP >> 16);
	temp <<= 8;
	temp |= (BYTE)(IP >> 24);
	return temp;
}

CString GetMacString(uint8_t* mac_)
{
	char mac_str[20];
	for (int8_t i = 0; i < 6; i++)
		sprintf(&mac_str[2 * i], "%02X", mac_[i]);
	mac_str[12] = 0;
	CString str(mac_str);
	return str;
}

void CIntellisenseView::InitConfig()
{
	_mac_str = _T("");
	_cmd_port = 0;
	_img_port = 0;
	_line_number = NSE_TRIGER_HEIGHT;
	_color_mode = 0;
	_xgcu_type = 0;
	_card_type = 0;
	_op_mode = 0;
	_pixel_depth = 0;
	_card_num1 = 0;
	_card_num2 = 0;
	_card_num3 = 0;
	_card_num4 = 0;
	_card_num5 = 0;
	_binning_mode = 0;
	_dm_pixel_num = 0;
	_energy_mode = 0;
}

void CIntellisenseView::InitOperate()
{
	_gamma = 1.0f;
	_file_name = _T("image.tif");
	_gain_target = 50000;
	_flash_index = 0;
	_en_gf_correct = FALSE;
	_baseline_value = 0;
	_en_baseline_correct = FALSE;
	_en_line_trigger = FALSE;
	_line_trigger_mode = 0;
	_line_fine_delay = 0;
	_line_raw_delay = 0;
	_en_frame_trigger = FALSE;
	_frame_trigger_mode = 0;
	_frame_delay = 0;
	_average_filter = 0;
	_sum_filter = 0;
	_en_gf_correct_offboard = FALSE;

	if (_xcommand.GetIsOpen())
	{
		uint64_t para_data;

		_xcommand.GetPara(XPARA_EN_GAIN_CORRECT, para_data);
		_en_gf_correct = static_cast<BOOL>(para_data);

		_xcommand.GetPara(XPARA_BASE_LINE, para_data);
		_baseline_value = static_cast<UINT>(para_data);
		_xcommand.GetPara(XPARA_EN_BASELINE_CORRECT, para_data);
		_en_baseline_correct = static_cast<BOOL>(para_data);

		_xcommand.GetPara(XPARA_EN_LINE_TRIGGER, para_data);
		_en_line_trigger = static_cast<BOOL>(para_data);

		_xcommand.GetPara(XPARA_LINE_TRIGGER_MODE, para_data);
		_line_trigger_mode = static_cast<int>(para_data);

		_xcommand.GetPara(XPARA_LINETRG_FINE_DELAY, para_data);
		_line_fine_delay = static_cast<BYTE>(para_data);

		_xcommand.GetPara(XPARA_LINETRG_RAW_DELAY, para_data);
		_line_raw_delay = static_cast<BYTE>(para_data);

		_xcommand.GetPara(XPARA_EN_FRAME_TRIGGER, para_data);
		if (para_data)
			_en_frame_trigger = 1;
		else
			_en_frame_trigger = 0;

		_xcommand.GetPara(XPARA_FRAME_TRIGGER_MODE, para_data);
		_frame_trigger_mode = static_cast<int>(para_data);

		_xcommand.GetPara(XPARA_FRAME_TRIGGER_DELAY, para_data);
		_frame_delay = static_cast<BYTE>(para_data);

		_xcommand.GetPara(XPARA_AVERAGE_MODE, para_data);
		_average_filter = static_cast<int>(para_data);

		_xcommand.GetPara(XPARA_SUMMING_MODE, para_data);
		_sum_filter = static_cast<int>(para_data);
	}
}

void CIntellisenseView::Initial_DetectorCB01(void)
{
	//로그 0324
	/*


	OpenLogFile();

	WriteLog(_T("program_start"),1);

	CloseLogFile();
	*/
	//로그 0324


	InitConfig();

	USES_CONVERSION;

	// Device의 ip주소를 받아오고
	WSADATA wsData;
	::WSAStartup(MAKEWORD(2, 2), &wsData);
	char host_name[128];
	CString ip_str;
	LPCSTR psz;

	if (gethostname(host_name, 128) == 0)
	{
		// Get host adresses
		struct hostent* host_ = gethostbyname(host_name);
		for (int i = 0; host_ != NULL && host_->h_addr_list[i] != NULL; i++)
		{
			psz = inet_ntoa(*(struct in_addr*)host_->h_addr_list[i]);		// ip address
			vPsz.emplace_back(psz);
		}
	}

	//AfxMessageBox(ip_str);

	// 연결
	_xsystem.Close();

	for (const auto& ip : vPsz) {
		if (ip.rfind("192.168.1.100", 0) == 0) {
			_xsystem.SetLocalIP(ip.c_str());
			break;
		}
	}

	if (_xsystem.Open())
	{
		// Find device
		int dev_num = _xsystem.FindDevice();
		if (dev_num > 0)
		{
			// Trace the first deviece info
			XDevice* dev_ = _xsystem.GetDevice(0);

			if (NULL == dev_)
			{
				AfxMessageBox(_T("Find no device"));
				return;
			}

			// 디텍터의 정보 얻기 (mac, ip, cmd port, img port)
			uint8_t* mac_ = dev_->GetMAC();
			_mac_str = GetMacString(mac_);

			DWORD ip_value = ConvertIP(inet_addr(dev_->GetIP()));

			_ip_control.SetAddress(ip_value);
			_cmd_port = dev_->GetCmdPort();
			_img_port = dev_->GetImgPort();

			//CString cmdport;
			//cmdport.Format(_T("%d"), static_cast<int>(_cmd_port));
			//CString imgport;
			//imgport.Format(_T("%d"), static_cast<int>(_img_port));

			//std::wcout << _T("mac : ") << (CStringA)_mac_str << _T("  cmdport : ") << cmdport << _T("  imgport : ") << imgport << std::endl;

			g_bDetector = true;
		}
		else
		{
			g_bDetector = false;
			OnWriteLog(100);

			DWORD ip_value = 0;
			_ip_control.SetAddress(ip_value);
			_cmd_port = 0;
			_img_port = 0;
			AfxMessageBox(_T("Find no device"));
		}


		if (g_bDetector)
		{
			XDevice* dev_ = _xsystem.GetDevice(0);
			if (NULL == dev_)
				return;

			// 디텍터 연결
			//_xtransfer_ = &_xtransfer;
			//20251029 lim 개선
			_xcommand.SetTimeout(0);
			_xacquisition.SetTimeout(0);
			//20251029 lim 개선
			_xcommand.SetFactory(&_xfactory);
			_xacquisition.SetFactory(&_xfactory);
			_xtransfer.SetLineNum(_line_number);
			//_xtransfer_->SetLineNum(NSE_TRIGER_HEIGHT);		// 64
			_xacquisition.RegisterFrameTransfer(&_xtransfer);
			// Add info header to image line
			//_xacquisition.EnableLineInfo(1);		// lostline 무시
			// b_UseLineInfo = true;

			_xacquisition.EnableLineInfo(0);
			b_UseLineInfo = false;
			// lineinfo를 사용하면 lost line이 0의 값으로 채워짐
			// 사용하지 않으면 다음 라인의 데이터가 lost line으로 채워짐 (밀리는 듯?)

			if (_xcommand.Open(dev_))
			{
				// Check whether it tis dual energy LCS system
				_is_non_hilo = 0;
				_is_dual_line_lcs = 0;

				uint32_t op_mode = dev_->GetOPMode();
				if (op_mode == XOP_MODE_NON_HILO)
				{
					_xoff_correct_ = &(_xoff_hl_correct);
				}
				else
				{
					_xoff_correct_ = &(_xoff_correct);
				}

				if (dev_->GetCardType() == XCARD_TYPE_DM_DOUBLE)		// Dual-row LCS2 DM
				{
					// Dual line lcs system
					if (op_mode == XOP_MODE_NON_HILO)
					{
						_is_dual_line_lcs = 2;		// Dual
						_xdual_line_correct.SetDualEnergy(1);
					}
					else
					{
						_is_dual_line_lcs = 1;		// Single
						_xdual_line_correct.SetDualEnergy(0);
					}
				}
				else			// LCS2 DM(0) - 일반 ?
				{
					if (op_mode == XOP_MODE_NON_HILO)
						_is_non_hilo = 1;
				}


				if (_xacquisition.Open(dev_, &_xcommand))
				{
					//_xoff_correct_ = &_xoff_correct;
					if (_xoff_correct_ != nullptr)
						_xoff_correct_->Open(dev_);

					_xpiecewise_correct.Open(dev_);

				}
				else
					std::wcout << _T("xacqutision open fail") << std::endl;

				GetSN();

				// detector 설정 값들 받아오기
				uint64_t para_data;

				_xcommand.GetPara(XPARA_GCU_TYPE, para_data);
				_xgcu_type = static_cast<int>(para_data);

				_xcommand.GetPara(XPARA_CARD_TYPE, para_data);
				_card_type = static_cast<int>(para_data);

				_xcommand.GetPara(XPARA_OPE_MODE, para_data);
				_op_mode = static_cast<int>(para_data);

				_xcommand.GetPara(XPARA_PIXEL_DEPTH, para_data);
				switch (para_data)
				{
				case 16:
					_pixel_depth = 0;
					break;
				case 18:
					_pixel_depth = 1;
					break;
				case 20:
					_pixel_depth = 2;
					break;
				case 24:
					_pixel_depth = 3;
					break;
				default:
					_pixel_depth = 0;
					break;
				}

				_xcommand.GetPara(XPARA_BINNING_MODE, para_data);
				_binning_mode = static_cast<int>(para_data);
				_xcommand.GetPara(XPARA_ENERGY_MODE, para_data);
				_energy_mode = static_cast<int>(para_data);
				_xcommand.GetPara(XPARA_DM_PIXEL_NUM, para_data);
				_dm_pixel_num = static_cast<int>(para_data) - 5;
				_xcommand.GetPara(XPARA_CH_NUM, para_data);
				_card_num5 = static_cast<int>(para_data);
				_card_num4 = static_cast<int>(para_data >> 8);
				_card_num3 = static_cast<int>(para_data >> 16);
				_card_num2 = static_cast<int>(para_data >> 24);
				_card_num1 = static_cast<int>(para_data >> 32);

				_xcommand.GetPara(XPARA_LINE_TRIGGER_MODE, para_data);
				int nTriggermode = static_cast<int>(para_data);
				//std::wcout << _T("Triggermode : ") << nTriggermode << std::endl;
				_xcommand.GetPara(XPARA_EN_LINE_TRIGGER, para_data);
				int nLineTrigger = static_cast<int>(para_data);
				//std::wcout << _T("Line Trigger : ") << nLineTrigger << std::endl;
			}
		}
	}
	else
		AfxMessageBox(_T("XSystem object fail to open"));
}

void CIntellisenseView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIntellisenseView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIntellisenseView 진단

#ifdef _DEBUG
void CIntellisenseView::AssertValid() const
{
	CFormView::AssertValid();
}

void CIntellisenseView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CIntellisenseDoc* CIntellisenseView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIntellisenseDoc)));
	return (CIntellisenseDoc*)m_pDocument;
}
#endif //_DEBUG


// CIntellisenseView 메시지 처리기

void CIntellisenseView::LoadBackground(void)
{
	BITMAP bmpInfo; 
	GetDlgItem(IDC_MAINCOND1)->ShowWindow(false);
	GetDlgItem(IDC_MAINCOND2)->ShowWindow(false);
	GetDlgItem(IDC_MAINCOND3)->ShowWindow(false);
	GetDlgItem(IDC_MAINCOND4)->ShowWindow(false);
	GetDlgItem(IDC_MAINCOND5)->ShowWindow(false);
	GetDlgItem(IDC_MAINCOND6)->ShowWindow(false);

	if(g_NationalCode==0)
	{
		m_bmpMainBG.LoadBitmap(IDB_MAIN_SCREEN);
		m_bmpMaincond[0].LoadBitmap(IDB_BT_MAINCOND1);
		m_bmpMaincond[1].LoadBitmap(IDB_BT_MAINCOND2);
		m_bmpMaincond[2].LoadBitmap(IDB_BT_MAINCOND3);
		m_bmpMaincond[3].LoadBitmap(IDB_BT_MAINCOND4);
		m_bmpMaincond[4].LoadBitmap(IDB_BT_MAINCOND5);
		m_bmpMaincond[5].LoadBitmap(IDB_BT_MAINCOND6);
	}
	else if(g_NationalCode==1)
	{
		m_bmpMainBG.LoadBitmap(IDB_MAIN_SCREEN_ENG);
		m_bmpMaincond[0].LoadBitmap(IDB_BT_MAINCOND1_ENG);
		m_bmpMaincond[1].LoadBitmap(IDB_BT_MAINCOND2_ENG);
		m_bmpMaincond[2].LoadBitmap(IDB_BT_MAINCOND3_ENG);
		m_bmpMaincond[3].LoadBitmap(IDB_BT_MAINCOND4_ENG);
		m_bmpMaincond[4].LoadBitmap(IDB_BT_MAINCOND5_ENG);
		m_bmpMaincond[5].LoadBitmap(IDB_BT_MAINCOND6_ENG);
	}
	else if(g_NationalCode==2)
	{
		m_bmpMainBG.LoadBitmap(IDB_MAIN_SCREEN_CHI);
		m_bmpMaincond[0].LoadBitmap(IDB_BT_MAINCOND1_CHI);
		m_bmpMaincond[1].LoadBitmap(IDB_BT_MAINCOND2_CHI);
		m_bmpMaincond[2].LoadBitmap(IDB_BT_MAINCOND3_CHI);
		m_bmpMaincond[3].LoadBitmap(IDB_BT_MAINCOND4_CHI);
		m_bmpMaincond[4].LoadBitmap(IDB_BT_MAINCOND5_CHI);
		m_bmpMaincond[5].LoadBitmap(IDB_BT_MAINCOND6_CHI);
	}
	else if(g_NationalCode==3)
	{
		m_bmpMainBG.LoadBitmap(IDB_MAIN_SCREEN_SPN);
		m_bmpMaincond[0].LoadBitmap(IDB_BT_MAINCOND1_SPN);
		m_bmpMaincond[1].LoadBitmap(IDB_BT_MAINCOND2_SPN);
		m_bmpMaincond[2].LoadBitmap(IDB_BT_MAINCOND3_SPN);
		m_bmpMaincond[3].LoadBitmap(IDB_BT_MAINCOND4_SPN);
		m_bmpMaincond[4].LoadBitmap(IDB_BT_MAINCOND5_SPN);
		m_bmpMaincond[5].LoadBitmap(IDB_BT_MAINCOND6_SPN);
	}

    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth;
    m_nMainHeight = bmpInfo.bmHeight;
    SetWindowPos(NULL, 0, 0, m_nMainWidth, m_nMainHeight, SWP_NOZORDER | SWP_NOMOVE); 


	m_Menu_ExitBt.LoadBitmaps(IDB_MENU_EXITBT,IDB_MENU_EXITBT_0); 
	m_Menu_ExitBt.SizeToContent();

}

BOOL CIntellisenseView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    CDC MemDC; 
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SelectObject(&m_bmpMainBG);
    pDC->BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
	return false;//CFormView::OnEraseBkgnd(pDC);
}

void CIntellisenseView::DisplayTime(void)
{
	m_CurrentTime = CTime::GetCurrentTime();
	GetDlgItem(IDC_EDIT_TIME)->SetWindowText(m_CurrentTime.Format("%y/%m/%d %H:%M"));
}

void CIntellisenseView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(point.x>=690 && point.x<=980 && point.y>=225 && point.y<=445)
	{
		if(g_ProductName=="")
			return;
		else
			OnConditionsetup();
	}

	CFormView::OnLButtonDblClk(nFlags, point);
}

void CIntellisenseView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent==0)
	{
		KillTimer(0);
		DisplayTime();
		Sleepms(100);

		//CString cmd = _T("/C nvidia-smi -lgc 2700,2700");
		//ShellExecute(NULL,_T("runas"),_T("cmd.exe"),cmd,NULL,SW_HIDE);

		//ShellExecute(
		//	NULL,
		//	_T("runas"),
		//	_T("C:\\Windows\\System32\\nvidia-smi.exe"),
		//	_T("-lgc 3000,3000"),
		//	NULL,
		//	SW_HIDE
		//);

		Sleepms(1000);
		//e_GpuCla = std::make_unique<NSE::AI::GpuCla>(); //AI 추가 0324	//260810 AI 제거

		Sleepms(100);
		if (g_SelDetector == 0)
			Initial_Detector();
		else
			Initial_DetectorCB01();

		
		//260810 AI 제거
		/*CString str;
		CTime time;
		time = CTime::GetCurrentTime();
		str.Format(_T("%04d-%02d-%02d-%02d%02d%02d-%d  make_unique\r\n"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

		CStdioFile fileString;
		if (fileString.Open(_T("D:\\Init_AI.Log"),
			CStdioFile::modeWrite | CStdioFile::shareDenyNone))
		{
			fileString.SeekToEnd();
			fileString.WriteString(str);
			fileString.Close();
		}
		else
		{
			if (fileString.Open(_T("D:\\Init_AI.Log"),
				CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
			{
				fileString.WriteString(str);
				fileString.Close();
			}
		}*/
		

		//Init_AI(); //ai 추가
		MakeDirectory();
		MakeSubDirectory();
		MakeExternalDirectory();
	
		InitArray();
		CleanCycleAlarm();
		
		g_Continu_Ng = 0;
		g_Count_Flag = false;
		g_3D_Count = 0;
		g_Thread_Count = 0;  //20170914 수정
		m_Icount = 0;
		g_KindofOmit = 0;

		g_leftm = 0;
		g_rightm = 640;

		m_Aiflag = true;
		//20211221 엔코더 라이브러리 변경
		if(g_RejectType == 0 && g_Model_Select == 0)
		{
			m_comi.Initialize();
			m_comi.Reset();
		}
		


		Load_Info(); //20191202
		SetTimer(100, 6000, NULL);
		AfxBeginThread(ThreadSave, (LPVOID)this);


		if(g_RejectType == 1) //20190726
			m_Edit_Time3.ShowWindow(false);
		else
			m_Edit_Time3.ShowWindow(true);

	//	SetTimer(1000,50,NULL);
	}
	if(nIDEvent==100)
	{
		DisplayTime();
		DisplayEAperMin();
	}
	if(nIDEvent==200)
	{	
		DrawAnimation();
	}
	if(nIDEvent == 1000)
	{
		m_Icount++;
		if(m_Icount >= 50)
		{
			m_Icount = 1;
		}

		OnSaveImg(m_Icount);
	}
	//20180405
	if(nIDEvent==400)
	{
		KillTimer(400);
		//20170830
		if (g_Today != m_strdate)
		{
			bool g_bOpenDB = true;

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
				//strSQL.Format(_T("SELECT * FROM intellisense_db1"));
				strSQL.Format(_T("SELECT * FROM intellisense_db1 WHERE strName = '%s' AND strDate = '%s'"), g_ProductName, g_Today);

				if (mysql_query(&m_mysql, CStringA(strSQL)))
				{
					CString error(mysql_error(&m_mysql));
					AfxMessageBox(error);
				}
				m_Res = mysql_store_result(&m_mysql);

				if ((row = mysql_fetch_row(m_Res)) == NULL)
				{
					strQuery.Format(_T("Insert INTO intellisense_db1(strName, strdate, strOk, strNg, strOmit, strTotal)") _T("VALUES('%s', '%s', %d, %d, %d, %d)"),
						g_ProductName, g_Today, m_TempReportOk, m_TempReportNg, m_TempReportOmit, m_TempReportTotal);
					if (mysql_query(&m_mysql, CStringA(strQuery)))
					{
						CString error(mysql_error(&m_mysql));
						AfxMessageBox(error);
					}
				}
				else
				{
					int tmp = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;

					CString Total(row[2]);
					tmp3 = _wtoi(Total);
					CString OK(row[3]);
					tmp = _wtoi(OK);
					CString NG(row[4]);
					tmp1 = _wtoi(NG);
					CString Omit(row[5]);
					tmp2 = _wtoi(Omit);

					strQuery.Format(_T("Update intellisense_db1 SET strOk = %d, strNg = %d, strOmit = %d, strTotal = %d WHERE strName = '%s' AND strDate = '%s'"),
						tmp + m_TempReportOk, tmp1 + m_TempReportNg, tmp2 + m_TempReportOmit, tmp3 + m_TempReportTotal, g_ProductName, g_Today);

					if (mysql_query(&m_mysql, CStringA(strQuery)))
					{
						CString error(mysql_error(&m_mysql));
						AfxMessageBox(error);
					}

					m_TempReportOk = 0;
					m_TempReportNg = 0;
					m_TempReportOmit = 0;
					m_TempReportTotal = 0;
					g_Today = m_strdate;
				}
				mysql_free_result(m_Res);
			}

			Close_Mysql();

			//if(!m_pSet_1.Open_DB(g_StrDirPath[1]))		
			//{
			//	g_bOpenDB=false;
			//	//RecoveryDatabase(g_StrDirPath[1], g_StrDirPath_copy[1]);
			//	if(m_pSet_1.Open_DB(g_StrDirPath[1]))		
			//		g_bOpenDB=true;
			//}
			//if(g_bOpenDB)
			//{
			//	CString strSQL;
			//	strSQL.Format(_T("SELECT * FROM Product_Info WHERE strName = '%s' AND strDate = '%s'"), g_ProductName, g_Today);
			//	CString strQuery;
			//	if(m_pSet_1.Select_DB(strSQL))
			//	{
			//		if(m_pSet_1.IsEOF())
			//		{
			//			strQuery.Format(_T("Insert INTO Product_Info(strName, strdate, strOk, strNg, strOmit, strTotal)") _T("VALUES('%s', '%s', %d, %d, %d, %d)"), 
			//				g_ProductName, g_Today, m_TempReportOk, m_TempReportNg, m_TempReportOmit, m_TempReportTotal);
			//			m_pSet_1.Insert_DB(strQuery);
			//		}
			//		else
			//		{
			//			int tmp=0, tmp1=0, tmp2=0, tmp3=0;
			//			m_pSet_1.ChangeRsInt(_T("strOk"), tmp);
			//			m_pSet_1.ChangeRsInt(_T("strNg"), tmp1);
			//			m_pSet_1.ChangeRsInt(_T("strOmit"), tmp2);
			//			m_pSet_1.ChangeRsInt(_T("strTotal"), tmp3);
			//			strQuery.Format(_T("Update Product_Info SET strOk = %d, strNg = %d, strOmit = %d, strTotal = %d WHERE strName = '%s' AND strDate = '%s'"), 
			//				tmp+m_TempReportOk, tmp1+m_TempReportNg, tmp2+m_TempReportOmit, tmp3+m_TempReportTotal, g_ProductName, g_Today);
			//			m_pSet_1.Update_DB(strQuery);			
			//		}
			//	}
			//	if(m_pSet_1.b_IsOpenedDB)
			//		m_pSet_1.Close_DB();

			//	m_TempReportOk=0;
			//	m_TempReportNg=0;
			//	m_TempReportOmit=0;
			//	m_TempReportTotal=0;
			//	g_Today=m_strdate;
			//	
			//}
			//20180405
			b_CheckOverwrite = false;
		}
	}
	CFormView::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CIntellisenseView, CFormView)
	//ON_EVENT(CIntellisenseView, IDC_DTIMAGE1, 1, CIntellisenseView::FrameReadyDtimage1, VTS_I4)
	//ON_EVENT(CIntellisenseView, IDC_DTIMAGE1, 4, CIntellisenseView::DTErrorDtimage1, VTS_I4 VTS_BSTR) 
END_EVENTSINK_MAP()

int CIntellisenseView::OnCheckNormalization()
{
	int i=0, j=0, count=0, result=4;
	bool detectline=false;
	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{
		count=0;
		for(j=g_Margin; j<g_Width-g_Margin;j++)
		{	 
			m_BadImg[i][j]=0;
			if(g_Img_Buf[g_3D_Count][i][j]<=180)
				count++;
			if(g_Img_Buf[g_3D_Count][i][j]>=220)
				count++;
		}
		/*if(count>((g_Width-(g_Margin*2))*4/5))
		{
			for(int k=0; k<g_Width; k++)
				m_BadImg[i][k]=1;
			result=3;
		}*/
	}

	return result; 
}

//void CIntellisenseView::DTErrorDtimage1(long ErrorID, LPCTSTR ErrorInfo)
//{
//	// TODO: Add your control notification handler code here
//	if(ErrorID > 0)
//	{
//		g_StopKind = 2; //20191202
//		CString str;
//		CTime time;
//		time=CTime::GetCurrentTime();
//		str.Format(_T("%04d-%02d-%02d-%02d%02d%02d-%d Detector Log \r\n") ,time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(),ErrorID);
//
//		CStdioFile fileString;
//		if(fileString.Open(_T("D:\\Detector.Log"),
//			CStdioFile::modeWrite | CStdioFile::shareDenyNone))
//		{
//			fileString.SeekToEnd();
//			fileString.WriteString(str);
//			fileString.Close();
//		}
//		else
//		{
//			if(fileString.Open(_T("D:\\Detector.Log"),
//				CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))	
//			{
//				fileString.WriteString(str);
//				fileString.Close();
//			}
//		}
//
//		if(ErrorID==20)
//		{
//			PostMessageToCtrlView(WM_EYH_STOP, 20, 0); //DTP Packet length error (Restart)
//		}
//		else{
//			PostMessageToCtrlView(WM_EYH_STOP, 10, 0); //Stop
//		}
//	}
//}

void CIntellisenseView::PostMessageToCtrlView(UINT MessageID, WPARAM wparam, LPARAM lparam)
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();
	pCtrlView->PostMessage(MessageID , wparam, lparam);
}

LRESULT CIntellisenseView::OnEventMsg(WPARAM wParam, LPARAM lParam)
{
	CString msg;
	msg.Format(_T("Event - ID: %d, Num: %d"), (int)wParam, (int)lParam);
	return 0;
}
LRESULT CIntellisenseView::OnErrorMsg(WPARAM wParam, LPARAM lParam)
{
	csDtErrorMsg.Format(_T("Error: ID: %ld, Info: %s"), (LONG)wParam, (char*)lParam);
	int m_EventID;
	m_EventID = (int)wParam;
	CString str;
	CTime time;
	time = CTime::GetCurrentTime();
	str.Format(_T("%04d-%02d-%02d-%02d%02d%02d-%d Detector Log \r\n"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), m_EventID);

	CStdioFile fileString;
	if (fileString.Open(_T("D:\\Detector.Log"),
		CStdioFile::modeWrite | CStdioFile::shareDenyNone))
	{
		fileString.SeekToEnd();
		fileString.WriteString(str);
		fileString.Close();
	}
	else
	{
		if (fileString.Open(_T("D:\\Detector.Log"),
			CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
		{
			fileString.WriteString(str);
			fileString.Close();
		}
	}
	if (m_EventID == 20)
	{
		PostMessageToCtrlView(WM_EYH_STOP, 20, 0); //DTP Packet length error (Restart)
	}
	else if (m_EventID == 9)
	{
		PostMessageToCtrlView(WM_EYH_STOP, 20, 0); //DTP Packet length error (Restart)
	}
	else
	{
		PostMessageToCtrlView(WM_EYH_STOP, 10, 0); //Stop
	}
	OnWriteLog(101);

	return LRESULT();
}

LRESULT CIntellisenseView::OnFrameReadyMsg(WPARAM wParam, LPARAM lParam)
{
	if(g_Model_Select == 0)
	{
		if (m_Interock_State || g_XrayState == 0 || g_XrayState == 1 || b_IsNormalization == 2 || b_ConveyorOn)
			return 0;

		ULONG_PTR imgDataAddress = (ULONG_PTR)wParam;

		if(g_iGrab_Count<50)
		{
			g_iGrab_Count++;

			int a,b;
			int m_Width=m_pImageObject->GetWidth(); 
			int per=m_pImageObject->GetBytesPerPixel(); 
			
			//memcpy(m_pImgBuf, (BYTE*)m_pImageObject->GetImageDataAddress(), m_ImgSize);
			BYTE* pSrc = (BYTE*)imgDataAddress;
			
			for(a=0;a<1;a++) //384
			{
				for(b=1; b<125;b++) 
				{
					temp_Buf[a][b]=0;
					//BYTE* pData = pSrc;
					BYTE* pData = m_pImgBuf;
					pData += (a*(m_Width)+b)*per;

					temp_Buf[a][b]=*(WORD*)pData;  //kds
				}
			}
			//CString aaa;
			int m_iMaxValue=0;
			int m_iPosition=0;
			int m_iSubValue=0;
			for(int c=1;c<120;c++){
				m_iSubValue = (temp_Buf[0][c+1])-(temp_Buf[0][c]);
				if(m_iSubValue >=m_iMaxValue){
					m_iMaxValue = m_iSubValue;
					m_iPosition = c;
				}
			}	

			g_Margin22[g_iGrab_Count] = m_iPosition;
			//return 0;
		}

		int i, j;
		m_FrameCount++;
		//std::wcout << _T("m_FrameCount : ") << m_FrameCount << std::endl;

		if(m_pImageObject)
		{
			m_Height=m_pImageObject->GetHeight(); 
			int m_Width=m_pImageObject->GetWidth();  
			int per=m_pImageObject->GetBytesPerPixel(); 
			BYTE* pSrc = (BYTE*)imgDataAddress;
			if(NULL == pSrc)
			{
				m_FrameCount=0;
				return 0;
			}
		
			int k=m_Height*(m_FrameCount-1);
			int count=0, blackcount=0;
			for(i=0;i<m_Height;i++) //384
			{
				for(j=0; j<m_Width;j++) //640
				{
					temp_Buf[k][j]=0;
					BYTE* pData = pSrc;
					pData += (i*(m_Width)+j)*per;
					//temp_Buf[k][j]=*pData;
					temp_Buf[k][j]=*(WORD*)pData;  //kds
					if(temp_Buf[k][j]>255)
					{
						temp_Buf[k][j]=255;
					}
					else if(temp_Buf[k][j]<0)
					{
						temp_Buf[k][j]=0;
					}

				/*	if(g_TalMask_Bright ==1)
					{
						//if(j>=g_Margin2+2 && j<=m_Width-g_Margin)
						if(j>=g_Margin2+20 && j<=m_Width-g_Margin)
						{
							if(temp_Buf[k][j]>=0 && temp_Buf[k][j]<=g_TalMask_Bright2)  
							{
								count++;
								if(temp_Buf[k][j]<10)
									blackcount++;
							}
							if(temp_Buf[k][j]>=0 && temp_Buf[k][j]<=g_TalMask_Bright2-10)  
							{
								count=count+15;
								if(temp_Buf[k][j]<10)
									blackcount++;
							}
							if(temp_Buf[k][j]>=0 && temp_Buf[k][j]<=g_TalMask_Bright2-40)  
							{
								count=count+100;
								if(temp_Buf[k][j]<10)
									blackcount++;
							}
						}
					}
					else
					{*/
					if (g_TalMask_Bright2 < 100)
						g_TalMask_Bright2 = 180;

						if(j>=g_Margin2+20 && j<=m_Width-g_Margin)
						{
							if(temp_Buf[k][j]>=0 && temp_Buf[k][j]<=g_TalMask_Bright2)  
							{
								count++;
								if(temp_Buf[k][j]<10)
									blackcount++;
							}
							if(temp_Buf[k][j]>=0 && temp_Buf[k][j]<=g_TalMask_Bright2-15)  
							{
								count=count+15;
								if(temp_Buf[k][j]<10)
									blackcount++;
							}
							if(temp_Buf[k][j]>=0 && temp_Buf[k][j]<=g_TalMask_Bright2-40)  
							{
								count=count+100;
								if(temp_Buf[k][j]<10)
									blackcount++;
							}
						}
					//}
				}
				k++;
			}
	
			if(b_IsNormalization==1)
			{
				if(m_FrameCount==NSE_TRIGER_FRAME_SIZE)
				{
					int Num=0;

					m_FrameCount = 0;
					b_IsNormalization=2;

					b_IsNormalization=0;
					m_FrameCount = 0;
				}
			}
			else if(b_IsNormalization==3)
			{
			
				if(m_FrameCount==NSE_TRIGER_FRAME_SIZE)
				{
					int k=0;
					for(i=0;i<g_Width;i++)
					{
						for(j=0; j<g_Height;j++)
						{
							g_Img_temp[j][i]=temp_Buf[i][j];
							if(i<g_Height)
							{
								m_Buffer_3[k]=g_Img_temp[j][i];
								k++;
							}

							if(j < 15+g_Margin2 )
							{
								g_Img_temp[j][i] = 200;
								m_Buffer_3[k] = g_Img_temp[j][i];

							}
						}
				
					}

					g_EventDrawImage.PulseEvent();
					m_FrameCount = 0;
				}
			}
			////////////////////////////////////////////////////////////////////////
			else
			{
					if(count>=100) 
					{
					//	if(m_IsProduct==0 || blackcount<(m_Height*(g_Width-(g_Margin*2))*0.9))
						if(m_IsProduct==0)
						{
							m_IsProduct = 1;
						}
					}
					else 
					{ 
						if(m_IsProduct==1 || m_IsProduct==2) 
						{
							if(m_FrameCount>2)
							{
								if(g_RejectType == 1)
								{
									g_Starttime = timeGetTime(); //리젝트 변경
								}
								else
								{	
									//g_Starttime = m_ComiDaq1.EncRead(0);
									//20211221 엔코더 라이브러리 변경
									g_Starttime = m_comi.Read();
								}
								m_IsProduct = 0;

								int front=0, original=0;
								front=(NSE_TRIGER_FRAME_SIZE-m_FrameCount)/2;


								for(i=0;i<g_Width;i++)
								{
									for(j=0; j<g_Height-15;j++)
									{

										if(i<front*m_Height || i>=(front+m_FrameCount)*m_Height)
										{
											g_Img_temp[j+15][i]=200;
										}
										else
										{
											if(temp_Buf[original][j]<201)
												g_Img_temp[j+15][i]=temp_Buf[original][j];
											else
												g_Img_temp[j+15][i]=200;
										}

										if(j<15+g_Margin2)
										{
											g_Img_temp[j][i]=200;
										}

									}
									if(!(i<front*m_Height || i>=(front+m_FrameCount)*m_Height))
										original++;

								}

								m_FrameCount=0;

								g_3D_Count++; //2D 배열로 변경 해야됨.

								if(g_3D_Count >= 50)
								{
									g_3D_Count = 1;
								}

								for(int i=0; i<g_Height; i++)
								{
									for(int j=0; j<g_Width; j++)
									{
										g_Img_Buf3D[g_3D_Count][i][j] = g_Img_temp[i][j];
									}
								}

								OnThreadRecoginition();
							}
							else
							{
								m_IsProduct = 2;
								//m_FrameCount=0;
							}		
					
						}
						else 
						{
							m_FrameCount--;
						}
					}
				
				
					if(m_FrameCount==NSE_TRIGER_FRAME_SIZE)
					{
					
						int tempcount=0;
						m_IsProduct = 0;			
			
						m_FrameCount=0;
						return 0;
					
					}
			}
		}
	}
	else if(g_Model_Select == 1)  //20190327
	{
		if (m_Interock_State || g_XrayState == 0 || g_XrayState == 1 || b_IsNormalization == 2 || b_ConveyorOn)  //kds20090922
			return 0;

		ULONG_PTR imgDataAddress = (ULONG_PTR)wParam;

		if(g_iGrab_Count<50)
		{
			g_iGrab_Count++;

			int a,b;
			int m_Width=m_pImageObject->GetWidth(); 
			int per=m_pImageObject->GetBytesPerPixel(); 
			BYTE* pSrc = (BYTE*)imgDataAddress;
			for(a=0;a<1;a++) //384
			{
				for(b=1; b<125;b++) 
				{
					temp_Buf[a][b]=0;
					BYTE* pData = pSrc;
					pData += (a*(m_Width)+b)*per;

					temp_Buf[a][b]=*(WORD*)pData;  //kds
				}
			}

			//CString aaa;
			int m_iMaxValue=0;
			int m_iPosition=0;
			int m_iSubValue=0;
			for(int c=1;c<120;c++)
			{
				m_iSubValue = (temp_Buf[0][c+1])-(temp_Buf[0][c]);
				if(m_iSubValue >=m_iMaxValue)
				{
					m_iMaxValue = m_iSubValue;
					m_iPosition = c;
				}
			}	

			g_Margin22[g_iGrab_Count] = m_iPosition;
			//return 0;
		}

		int i, j;
		m_FrameCount++;
		m_InternalCount++;   //20190327

		if(m_pImageObject)
		{
			m_Height=m_pImageObject->GetHeight(); 
			int m_Width=m_pImageObject->GetWidth();  
			int per=m_pImageObject->GetBytesPerPixel(); 
			BYTE* pSrc = (BYTE*)imgDataAddress;
			if(NULL == pSrc)
			{
				m_FrameCount=0;
				return 0;
			}
		
			int k=m_Height*(m_FrameCount-1);
			int count=0, blackcount=0;
			for(i=0;i<m_Height;i++) //384
			{
				for(j=0; j<m_Width;j++) //640
				{
					temp_Buf[k][j]=0;
					BYTE* pData = pSrc;
					pData += (i*(m_Width)+j)*per;
					//temp_Buf[k][j]=*pData;
					temp_Buf[k][j]=*(WORD*)pData;  //kds
					if(temp_Buf[k][j]>255)
					{
						temp_Buf[k][j]=255;
					}
					else if(temp_Buf[k][j]<0)
					{
						temp_Buf[k][j]=0;
					}

					if(g_TalMask_Bright ==1)
					{
						if(j>=g_Margin2+2 && j<=m_Width-g_Margin)
						{
							if(temp_Buf[k][j]>=0 && temp_Buf[k][j]<=g_TalMask_Bright2)  
							{
								count++;
								if(temp_Buf[k][j]<10)
									blackcount++;
							}
							if(temp_Buf[k][j]>=0 && temp_Buf[k][j]<=g_TalMask_Bright2-10)  
							{
								count=count+15;
								if(temp_Buf[k][j]<10)
									blackcount++;
							}
							if(temp_Buf[k][j]>=0 && temp_Buf[k][j]<=g_TalMask_Bright2-40)  
							{
								count=count+100;
								if(temp_Buf[k][j]<10)
									blackcount++;
							}
						}
					}
					else
					{
						if(j>=g_Margin2+20 && j<=g_Margin2+200)
						{
							if(temp_Buf[k][j]>=0 && temp_Buf[k][j]<=g_TalMask_Bright2)  
							{
								count++;
								if(temp_Buf[k][j]<10)
									blackcount++;
							}
							if(temp_Buf[k][j]>=0 && temp_Buf[k][j]<=g_TalMask_Bright2-15)  
							{
								count=count+15;
								if(temp_Buf[k][j]<10)
									blackcount++;
							}
							if(temp_Buf[k][j]>=0 && temp_Buf[k][j]<=g_TalMask_Bright2-40)  
							{
								count=count+100;
								if(temp_Buf[k][j]<10)
									blackcount++;
							}
						}
					}
				}
				k++;
			}
	
			if(b_IsNormalization==1)
			{
				if(m_FrameCount==NSE_TRIGER_FRAME_SIZE)
				{
					int Num=0;

					m_FrameCount = 0;
					b_IsNormalization=2;

					b_IsNormalization=0;
					m_FrameCount = 0;
				}
			}
			else if(b_IsNormalization==3)
			{
			
				if(m_FrameCount==NSE_TRIGER_FRAME_SIZE)
				{
					int k=0;
					for(i=0;i<g_Width;i++)
					{
						for(j=0; j<g_Height;j++)
						{
							g_Img_temp[j][i]=temp_Buf[i][j];
							if(i<g_Height)
							{
								m_Buffer_3[k]=g_Img_temp[j][i];
								k++;
							}

							if(j < 15+g_Margin2 )
							{
								g_Img_temp[j][i] = 200;
								m_Buffer_3[k] = g_Img_temp[j][i];

							}
						}
				
					}

					g_EventDrawImage.PulseEvent();
					m_FrameCount = 0;
				}
			}
			////////////////////////////////////////////////////////////////////////
			else
			{
					if(count>=100) 
					{
					//	if(m_IsProduct==0 || blackcount<(m_Height*(g_Width-(g_Margin*2))*0.9))
						if(m_IsProduct==0)
						{
							m_IsProduct = 1;
						}
					}
					else 
					{ 
						if(m_IsProduct==1 || m_IsProduct==2) 
						{
							if(m_FrameCount>2)
							{
								if(g_RejectType == 1)
								{
									g_Starttime = timeGetTime(); //리젝트 변경
								}
								else
								{	
									g_Starttime = m_InternalCount; //20190327
								}
								m_IsProduct = 0;

								int front=0, original=0;
								front=(NSE_TRIGER_FRAME_SIZE-m_FrameCount)/2;


								for(i=0;i<g_Width;i++)
								{
									for(j=0; j<g_Height-15;j++)
									{

										if(i<front*m_Height || i>=(front+m_FrameCount)*m_Height)
										{
											g_Img_temp[j+15][i]=200;
										}
										else
										{
											if(temp_Buf[original][j]<201)
												g_Img_temp[j+15][i]=temp_Buf[original][j];
											else
												g_Img_temp[j+15][i]=200;
										}

										if(j<15+g_Margin2)
										{
											g_Img_temp[j][i]=200;
										}

									}
									if(!(i<front*m_Height || i>=(front+m_FrameCount)*m_Height))
										original++;

								}

								m_FrameCount=0;

								g_3D_Count++; //2D 배열로 변경 해야됨.

								if(g_3D_Count >= 50)
								{
									g_3D_Count = 1;
								}

								for(int i=0; i<g_Height; i++)
								{
									for(int j=0; j<g_Width; j++)
									{
										g_Img_Buf3D[g_3D_Count][i][j] = g_Img_temp[i][j];
									}
								}

								OnThreadRecoginition();
							}
							else
							{
								m_IsProduct = 2;
								//m_FrameCount=0;
							}		
					
						}
						else 
						{
							m_FrameCount--;
						}
					}
				
				
					if(m_FrameCount==NSE_TRIGER_FRAME_SIZE)
					{
					
						int tempcount=0;
						m_IsProduct = 0;			
			
						m_FrameCount=0;
						return 0;
					
					}
			}
		}
	}
	return 0;
}

void CIntellisenseView::RegisterSink()
{
	_xsystem.RegisterEventSink(&xcmd_sink);
	_xcommand.RegisterEventSink(&xcmd_sink);
	_xacquisition.RegisterEventSink(&ximg_sink);
	_xtransfer.RegisterEventSink(&ximg_sink);
}

LRESULT CIntellisenseView::OnEventMsgC01B(WPARAM wParam, LPARAM lParam)
{
	CString str;
	CTime time = CTime::GetCurrentTime();
	bool b_EventError = false;

	const UINT evt = static_cast<UINT>(wParam);
	const ULONGLONG now = ::GetTickCount64();

	switch (evt)
	{
	case 50:
	case 53:
		// ----- [버스트 감지] -----
				// 윈도우 초기화 또는 롤오버
		if (now - _ll_window_start_ms > WINDOW_MS) {
			_ll_window_start_ms = now;
			_ll_window_lost_sum = 0;
			_ll_window_evt_cnt = 0;
		}

		// 윈도우 내 누적
		_ll_window_lost_sum += static_cast<uint32_t>(lParam);
		_ll_window_evt_cnt += 1;

		// 조건 만족 && 쿨다운 해제 상태면 에러
		if (now >= _ll_cooldown_until_ms &&
			(_ll_window_lost_sum >= LOST_LINES_THRESH ||
				_ll_window_evt_cnt >= EVENTS_THRESH))
		{
			str.Format(_T("%04d-%02d-%02d-%02d%02d%02d Line Lost Burst: sum=%u, evts=%u\r\n"),
				time.GetYear(), time.GetMonth(), time.GetDay(),
				time.GetHour(), time.GetMinute(), time.GetSecond(),
				_ll_window_lost_sum, _ll_window_evt_cnt);

			b_EventError = true;

			// 쿨다운 시작
			_ll_cooldown_until_ms = now + COOLDOWN_MS;

			// 다음 윈도우로 넘어가도록 초기화
			_ll_window_start_ms = now;
			_ll_window_lost_sum = 0;
			_ll_window_evt_cnt = 0;
		}
		break;
	case 56:
		if (temperature > 50.0f)
		{
			str.Format(_T("%04d-%02d-%02d-%02d%02d%02d Temperature : %.1fC \r\n"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), temperature);
			b_EventError = true;
		}
		break;
	case 57:
		if (humidity > 90.0f)
		{
			str.Format(_T("%04d-%02d-%02d-%02d%02d%02d Humidity : %.2f%% \r\n"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), humidity);
			b_EventError = true;
		}
		break;
	default:
		break;
	}

	if (b_EventError)
	{
		CStdioFile fileString;
		if (fileString.Open(_T("D:\\Detector.Log"),
			CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone))
		{
			fileString.SeekToEnd();  // 항상 Append
			fileString.WriteString(str);
			fileString.Close();
		}

		//PostMessageToCtrlView(WM_EYH_STOP, 30, 0);
		//g_bDetector = false;
	}

	return 0;
}

LRESULT CIntellisenseView::OnErrorMsgC01B(WPARAM wParam, LPARAM lParam)
{
	CString str;
	CTime time;
	int m_ErrorCode;
	m_ErrorCode = (int)wParam;

	time = CTime::GetCurrentTime();
	str.Format(_T("%04d-%02d-%02d-%02d%02d%02d Detector Log : %d \r\n"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), m_ErrorCode);

	CStdioFile fileString;
	if (fileString.Open(_T("D:\\Detector.Log"),
		CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone))
	{
		fileString.SeekToEnd();  // 항상 Append
		fileString.WriteString(str);
		fileString.Close();
	}
	if (m_ErrorCode == 43)
	{
		PostMessageToCtrlView(WM_EYH_STOP, 30, 0);
	}
	else if (m_ErrorCode == 1)
	{
		PostMessageToCtrlView(WM_EYH_STOP, 20, 0);
	}
	else
		PostMessageToCtrlView(WM_EYH_STOP, 30, 0);
	//g_bDetector = false;

	return 0;
}






LRESULT CIntellisenseView::OnFrameReadyMsgC01B(WPARAM wParam, LPARAM lParam)
{
	uint16_t* pix16 = reinterpret_cast<uint16_t*>(lParam);
	if (!pix16)
	{
		//WriteLog(_T("pix16 NULL"),9999);
		return 0;
	}

	std::unique_ptr<uint16_t[], void(*)(uint16_t*)> guard(pix16, [](uint16_t* p) { delete[] p; });



	if (g_Model_Select == 0)
	{
		if (m_Interock_State || g_XrayState == 0 || g_XrayState == 1 || b_IsNormalization == 2)
			return 0;

		const int W = LOWORD(wParam), H = HIWORD(wParam);
		m_Height = H;
		int m_Width = W;

		if (g_iGrab_Count < 50)
		{
			g_iGrab_Count++;

			int a, b;
			//int m_Width = W;
			//int maxIndex = m_Width - 1;		//이미지 반전(256외 모든 것)
			int maxIndex = 125;
			for (a = 0; a < 1; a++) //384
			{
				const uint16_t* line = pix16 + a * m_Width;
				for (b = 1; b < 125; b++)
				{
					uint16_t v16 = line[b];
					temp_Buf[a][b] = 0;
					temp_Buf[a][b] = v16;
				
				}
			}
			//CString aaa;
			int m_iMaxValue = 0;
			int m_iPosition = 0;
			int m_iSubValue = 0;
			for (int c = 1; c < 120; c++)
			{
				m_iSubValue = (temp_Buf[0][c + 1]) - (temp_Buf[0][c]);
				if (m_iSubValue >= m_iMaxValue) 
				{
					m_iMaxValue = m_iSubValue;
					m_iPosition = c;
				}
			}

			g_Margin22[g_iGrab_Count] = m_iPosition;
			//return 0;
		}

		int i, j;
		m_FrameCount++;

		/*m_Height = H;
		int m_Width = W;*/

		int k = m_Height * (m_FrameCount - 1); //h =1
		int count = 0, blackcount = 0;
		int maxIndex = m_Width - 1;		//이미지 반전(256외 모든 것)

		for (i = 0; i < m_Height; i++) //384 h 1
		{
			const uint16_t* line = pix16 + i * m_Width;
			for (j = 0; j < m_Width; j++) //640
			{
				uint16_t v16 = line[j];
				//int flippedIndex = maxIndex - j;		// -- 수정 이미지 반전 (256외 모든 것)
				int idx = j; //j와 flippedIndex 통합시킴 idx사용
				// CB01 Detector - 20240215
				if (b_IsNormalization == 3)
				{
					//20251029 lim 개선
					temp_Buf[k + i][idx] = v16;
					//20251029 lim 개선
				}
				else
				{
					uint16_t v8 = (uint16_t)(v16 >> 6);
					
					if (v8 > 255) 
						v8 = 255;
					
					//20251029 lim 개선
					temp_Buf[k + i][idx] = v8;
					//20251029 lim 개선
					
					//20251029 lim 개선
					if (g_TalMask_Bright2 < 100)
						g_TalMask_Bright2 = 180;

					//j값만 사용하게 되면 반전된 flippedIndex 값이랑 j 다르기 때문에 count 하는게 오류 생김
					//if (j >= g_Margin2 + 20 && j <= m_Width - g_Margin)
					if (idx >= g_Margin2 + 20 && idx <= m_Width - g_Margin)
					{
						if (v8 <= g_TalMask_Bright2) 
						{
							++count; 
							if (v8 < 10) 
								++blackcount;
						
						}
						if (v8 <= g_TalMask_Bright2 - 15) 
						{ 
							count += 15; 
							if (v8 < 10)
								++blackcount; 
						}
						if (v8 <= g_TalMask_Bright2 - 40) 
						{
							count += 100; 
							if (v8 < 10)
								++blackcount; 
						}
					}
				}
			}
			//k++;
		}

		if (b_IsNormalization == 1)
		{
			if (m_FrameCount == NSE_TRIGER_FRAME_SIZE)
			{
				int Num = 0;

				m_FrameCount = 0;
				b_IsNormalization = 2;

				b_IsNormalization = 0;
				m_FrameCount = 0;

				// 기본 모델에 있는 발생장치, 디텍터 체크 기능 없음
			}
		}
		else if (b_IsNormalization == 3)
		{
			if (m_FrameCount == NSE_TRIGER_FRAME_SIZE)
			{
				int k = 0;
				for (i = 0; i < g_Width; i++)
				{
					for (j = 0; j < g_Height; j++)
					{
						g_Img_temp[j][i] = temp_Buf[i][j];
						if (i < g_Height)
						{
							m_Buffer_3[k] = g_Img_temp[j][i] / 256;
							k++;
						}

						if (j < 15 + g_Margin2)
						{
							g_Img_temp[j][i] = 200;
							m_Buffer_3[k] = g_Img_temp[j][i];

						}
					}

				}

				g_EventDrawImage.PulseEvent();
				m_FrameCount = 0;
			}
		}
		else
		{
			if (count >= 100)
			{
				//	if(m_IsProduct==0 || blackcount<(m_Height*(g_Width-(g_Margin*2))*0.9))
				if (m_IsProduct == 0)
				{
					m_IsProduct = 1;
				}
			}
			else
			{
				if (m_IsProduct == 1 || m_IsProduct == 2)
				{
					if (m_FrameCount > 2)
					{
						if (g_RejectType == 1)
						{
							g_Starttime = timeGetTime(); //리젝트 변경
						}
						else
						{
							//g_Starttime = m_ComiDaq1.EncRead(0);
							//20211221 엔코더 라이브러리 변경
							g_Starttime = m_comi.Read();
						}
						m_IsProduct = 0;

						int front = 0, original = 0;
						front = (NSE_TRIGER_FRAME_SIZE - m_FrameCount) / 2;

						for (i = 0; i < g_Width; i++)
						{
							for (j = 0; j < g_Height - 15; j++)
							{

								if (i < front * m_Height || i >= (front + m_FrameCount) * m_Height)
								{
									g_Img_temp[j + 15][i] = 200;
								}
								else
								{
									if (temp_Buf[original][j] < 201)
										g_Img_temp[j + 15][i] = temp_Buf[original][j];
									else
										g_Img_temp[j + 15][i] = 200;
								}

								if (j < 15 + g_Margin2)
								{
									g_Img_temp[j][i] = 200;
								}
								
							}
							if (!(i < front * m_Height || i >= (front + m_FrameCount) * m_Height))
								original++;

						}

						m_FrameCount = 0;

						g_3D_Count++; //2D 배열로 변경 해야됨.

						if (g_3D_Count >= 50)
						{
							g_3D_Count = 1;
						}

						for (int i = 0; i < g_Height; i++)
						{
							for (int j = 0; j < g_Width; j++)
							{
								g_Img_Buf3D[g_3D_Count][i][j] = g_Img_temp[i][j];
							}
						}

						OnThreadRecoginition();
					}
					else
					{
						m_IsProduct = 2;
						//m_FrameCount=0;
					}
				}
				else
				{
					m_FrameCount--;
				}
			}

			if (m_FrameCount == NSE_TRIGER_FRAME_SIZE)
			{
				int tempcount = 0;
				m_IsProduct = 0;

				m_FrameCount = 0;
				return 0;
			}
		}
	}
	else if (g_Model_Select == 1)
	{
		if (m_Interock_State || g_XrayState == 0 || g_XrayState == 1 || b_IsNormalization == 2 || b_ConveyorOn)
			return 0;

		const int W = LOWORD(wParam), H = HIWORD(wParam);
		m_Height = H;
		int m_Width = W;
		/*if (W != 1024 || H != 1)
		{
			
			return 0;
		}*/
		if (g_iGrab_Count < 50)
		{
			

			int a, b;
			//int m_Width = W;
			int maxIndex = m_Width - 1;		// -- 수정

			for (a = 0; a < 1; a++) //384
			{
				const uint16_t* line = pix16 + a * m_Width;
				//for (b = 1; b < m_Width; b++)	// -- 수정
				for (b = 1; b < 125; b++)
				{
					uint16_t v16;
					v16 = line[b];
					//20251029 lim 개선
					temp_Buf[a][b] = 0;
					temp_Buf[a][b] = v16;
					//20251029 lim 개선	
				}



			}
			//CString aaa;
			int m_iMaxValue = 0;
			int m_iPosition = 0;
			int m_iSubValue = 0;
			for (int c = 1; c < 120; c++) {
				m_iSubValue = (temp_Buf[0][c + 1]) - (temp_Buf[0][c]);
				if (m_iSubValue >= m_iMaxValue) {
					m_iMaxValue = m_iSubValue;
					m_iPosition = c;
				}
			}

			g_Margin22[g_iGrab_Count] = m_iPosition;
			g_iGrab_Count++;
			//return 0;
		}

		int i, j;
		m_FrameCount++;
		m_InternalCount++;

		/*m_Height = H;
		int m_Width = W;*/

		int k = m_Height * (m_FrameCount - 1); //h는 1
		int count = 0, blackcount = 0;
		int maxIndex = m_Width - 1;		//이미지 반전(256외 모든 것)

		for (i = 0; i < m_Height; i++) //384 5000은 1
		{
			const uint16_t* line = pix16 + i * m_Width;
			for (j = 0; j < m_Width; j++) //640
			{
				uint16_t v16 = line[j];
				int flippedIndex = maxIndex - j;	// -- 수정 이미지 반전 (256외 모든 것)

				if (b_IsNormalization == 3)
				{
					//20251029 lim 개선
					temp_Buf[k + i][j] = v16;
					//20251029 lim 개선
				}
				else
				{
					uint16_t v8 = (uint16_t)(v16 >> 6);
					if (v8 > 255)
						v8 = 255;

					//20251029 lim 개선
					temp_Buf[k + i][j] = v8;
					//20251029 lim 개선

					if (g_TalMask_Bright == 1)
					{
						if (j >= g_Margin2 && j <= m_Width - g_Margin)
						{
							if (v8 <= g_TalMask_Bright2)
							{
								++count;

								if (v8 < 10)
									++blackcount;
							}
							if (v8 <= g_TalMask_Bright2 - 10)
							{
								count += 15;
								if (v8 < 10)
									++blackcount;
							}
							if (v8 <= g_TalMask_Bright2 - 40)
							{
								count += 100;
								if (v8 < 10)
									++blackcount;
							}
						}
					}
					else
					{
						if (j >= g_Margin2 + 20 && j <= m_Width - 200)
						{
							if (v8 <= g_TalMask_Bright2)
							{
								++count;
								if (v8 < 10)
									++blackcount;
							}
							if (v8 <= g_TalMask_Bright2 - 15) { count += 15; if (v8 < 10)++blackcount; }
							if (v8 <= g_TalMask_Bright2 - 40) { count += 100; if (v8 < 10)++blackcount; }
						}
					}
				}
			}
			//k++;
		}

		if (b_IsNormalization == 1)
		{
			if (m_FrameCount == NSE_TRIGER_FRAME_SIZE)
			{
				int Num = 0;

				m_FrameCount = 0;
				b_IsNormalization = 2;

				b_IsNormalization = 0;
				m_FrameCount = 0;
			}
		}
		else if (b_IsNormalization == 3)
		{
			if (m_FrameCount == NSE_TRIGER_FRAME_SIZE)
			{
				int k = 0;
				for (i = 0; i < g_Width; i++)
				{
					for (j = 0; j < g_Height; j++)
					{
						g_Img_temp[j][i] = temp_Buf[i][j];
						if (i < g_Height)
						{
							m_Buffer_3[k] = g_Img_temp[j][i] / 256;
							k++;
						}

						if (j < 15 + g_Margin2)
						{
							g_Img_temp[j][i] = 200;
							m_Buffer_3[k] = g_Img_temp[j][i];

						}
					}
				}
				g_EventDrawImage.PulseEvent();
				m_FrameCount = 0;
			}
		}
		////////////////////////////////////////////////////////////////////////
		else
		{
			if (count >= 100)
			{
				//	if(m_IsProduct==0 || blackcount<(m_Height*(g_Width-(g_Margin*2))*0.9))
				if (m_IsProduct == 0)
				{
					m_IsProduct = 1;
				}
			}
			else
			{
				if (m_IsProduct == 1 || m_IsProduct == 2)
				{
					if (m_FrameCount > 2)
					{
						if (g_RejectType == 1)
						{
							g_Starttime = timeGetTime(); //리젝트 변경
						}
						else
						{
							g_Starttime = m_InternalCount; //20190327
						}
						m_IsProduct = 0;

						int front = 0, original = 0;
						front = (NSE_TRIGER_FRAME_SIZE - m_FrameCount) / 2;

						g_3D_Count++; //2D 배열로 변경 해야됨.
						if (g_3D_Count >= 50)
						{
							g_3D_Count = 1;
						}

						
						for (i = 0; i < g_Width; i++)
						{
							for (j = 0; j < g_Height - 15; j++)
							{
								if (i < front * m_Height || i >= (front + m_FrameCount) * m_Height)
								{
									g_Img_temp[j + 15][i] = 200;
								}
								else
								{
									if (temp_Buf[original][j] < 201)
										g_Img_temp[j + 15][i] = temp_Buf[original][j];
									else
										g_Img_temp[j + 15][i] = 200;
								}

								if (j < 15 + g_Margin2)
								{
									g_Img_temp[j][i] = 200;
								}
							}
							if (!(i < front * m_Height || i >= (front + m_FrameCount) * m_Height))
								original++;

						}

						m_FrameCount = 0;

						//g_3D_Count++; //2D 배열로 변경 해야됨.

						//if (g_3D_Count >= 50)
						//{
						//	g_3D_Count = 1;
						//}

						for (int i = 0; i < g_Height; i++)
						{
							for (int j = 0; j < g_Width; j++)
							{
								g_Img_Buf3D[g_3D_Count][i][j] = g_Img_temp[i][j];
							}
						}
						//b_EventReconi = 1;
						
						OnThreadRecoginition();
						
					}
					else
					{
						m_IsProduct = 2;
						//m_FrameCount=0;
					}

				}
				else
				{
					if (m_FrameCount > 0)// 수정
						m_FrameCount--;
				}
			}

			if (m_FrameCount == NSE_TRIGER_FRAME_SIZE)
			{
				//g_3D_Count++; //2D 배열로 변경 해야됨.

				//if (g_3D_Count >= 50)
				//{
				//	g_3D_Count = 1;
				//}

				//for (int i = 0; i < g_Height; i++)
				//{
				//	for (int j = 0; j < g_Width; j++)
				//	{
				//		g_Img_Buf3D[g_3D_Count][i][j] = temp_Buf[j][i];
				//	}
				//}

				/*OnThreadRecoginition();*/

				//CString cs = _T("정상적인 제품 투입이 아닙니다.");
				//AfxMessageBox(_T("제품 투입이 정상적이지 않습니다"));
				/*CMessageBoxDlg dlg;
				dlg.m_Message = cs;
				dlg.DoModal();*/

				int tempcount = 0;
				m_IsProduct = 0;

				m_FrameCount = 0;
				return 0;
			}
		}
	}
}

LRESULT CIntellisenseView::OnAnalysisMsgC01B(WPARAM wParam, LPARAM lParam)
{
	// Draw map
	return 0;
}

bool CIntellisenseView::SetC01BFrameLineNumber(UINT nLineNumber)
{
	// Grab 중이면 중지

	XDevice* dev_ = _xsystem.GetDevice(0);
	if (dev_ == NULL)
		return false;  // bool 함수이므로 return; 사용 불가

	if (_xacquisition.GetIsGrabbing())
	{
		_xacquisition.Stop();

		// SDK에 StopGrab이 비동기라면
		// 실제 종료 확인 코드가 추가로 필요할 수 있음
	}

	// SetLineNum은 Acquisition Open 전에 적용
	if (_xacquisition.GetIsOpen())
	{
		_xacquisition.Close();
	}

	_xtransfer.SetLineNum(nLineNumber);

	// 기존 Initial_Detector_C01B()에서 사용하던
	// Acquisition 설정을 동일하게 복구
	_xacquisition.SetFactory(&_xfactory);

	_xtransfer.RegisterEventSink(&ximg_sink);
	_xacquisition.RegisterEventSink(&ximg_sink);
	_xacquisition.RegisterFrameTransfer(&_xtransfer);
	_xacquisition.EnableLineInfo(1);
	_xacquisition.SetTimeout(0);

	if (!_xacquisition.Open(dev_, &_xcommand))
	{
		//CString strMsg;
		//strMsg.Format(_T("Detector %d : Acquisition Open failed. LineNum=%u"), nDetector + 1, nLineNumber);
		//WriteDetectorErrorLog(1, strMsg);

		return false;
	}

	return true;
}


void CIntellisenseView::GetSN()
{
	char sn_str[64];
	if (_xcommand.GetPara(XPARA_GCU_SERIAL, sn_str))
		CString cstr = _T("SN : ") + CString(sn_str);
}

CWnd* CIntellisenseView::GetDispWnd()
{
	return GetDlgItem(IDC_GRAB_IMAGE);
}

void CIntellisenseView::OnWriteLog(int Num)
{
	CString strdate, strtime;
	CTime time;
	time = CTime::GetCurrentTime();
	strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());		
	strtime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), time.GetYear(),time.GetMonth(),time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	CString strQuery;
	bool g_bOpenDB;
	if(Num == 1)
	{
		g_bOpenDB = true;

		Init_Mysql();

		if (g_Connecting == true)
		{
			MYSQL_RES* m_Res;
			MYSQL_ROW row;

			CString strSQL;
			strSQL.Format(_T("SELECT * FROM intellisense_db1 WHERE strName = '%s' AND strDate = '%s'"), g_ProductName, g_Today);
			CString strQuery;
			if (mysql_query(&m_mysql, CStringA(strSQL)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
			m_Res = mysql_store_result(&m_mysql);

			if ((row = mysql_fetch_row(m_Res)) == NULL)
			{
				strQuery.Format(_T("Insert INTO intellisense_db1(strName, strdate, strOk, strNg, strOmit, strTotal)") _T("VALUES('%s', '%s', %d, %d, %d, %d)"),
					g_ProductName, g_Today, m_ReportOk, m_ReportNg, m_ReportOmit, m_ReportTotal);
				if (mysql_query(&m_mysql, CStringA(strQuery)))
				{
					CString error(mysql_error(&m_mysql));
					AfxMessageBox(error);
				}
			}
			else
			{
				int tmp = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;

				CString strOk(row[3]);
				CString strNg(row[4]);
				CString strOmit(row[5]);
				CString strTotal(row[2]);

				tmp = _wtoi(strOk);
				tmp1 = _wtoi(strNg);
				tmp2 = _wtoi(strOmit);
				tmp3 = _wtoi(strTotal);

				strQuery.Format(_T("Update intellisense_db1 SET strOk = %d, strNg = %d, strOmit = %d, strTotal = %d WHERE strName = '%s' AND strDate = '%s'"),
					tmp + m_ReportOk, tmp1 + m_ReportNg, tmp2 + m_ReportOmit, tmp3 + m_ReportTotal, g_ProductName, g_Today);
				if (mysql_query(&m_mysql, CStringA(strQuery)))
				{
					CString error(mysql_error(&m_mysql));
					AfxMessageBox(error);
				}
			}
			mysql_free_result(m_Res);
		}

		Close_Mysql();

		//if(!m_pSet_1.Open_DB(g_StrDirPath[1]))		
		//{
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[1], g_StrDirPath_copy[1]);
		//	if(m_pSet_1.Open_DB(g_StrDirPath[1]))		
		//		g_bOpenDB=true;
		//}
		//if(g_bOpenDB)
		//{
		//	CString strSQL;
		//	//strSQL.Format(_T("SELECT * FROM Product_Info WHERE strName = '%s' AND strDate = '%s'"), g_ProductName, m_strdate);
		//	strSQL.Format(_T("SELECT * FROM Product_Info WHERE strName = '%s' AND strDate = '%s'"), g_ProductName, g_Today);
		//	CString strQuery;
		//	if(m_pSet_1.Select_DB(strSQL))
		//	{
		//		if(m_pSet_1.IsEOF())
		//		{
		//			strQuery.Format(_T("Insert INTO Product_Info(strName, strdate, strOk, strNg, strOmit, strTotal)") _T("VALUES('%s', '%s', %d, %d, %d, %d)"), 
		//				g_ProductName, g_Today, m_ReportOk, m_ReportNg, m_ReportOmit, m_ReportTotal);
		//			m_pSet_1.Insert_DB(strQuery);
		//		}
		//		else
		//		{
		//			int tmp=0, tmp1=0, tmp2=0, tmp3=0;
		//			m_pSet_1.ChangeRsInt(_T("strOk"), tmp);
		//			m_pSet_1.ChangeRsInt(_T("strNg"), tmp1);
		//			m_pSet_1.ChangeRsInt(_T("strOmit"), tmp2);
		//			m_pSet_1.ChangeRsInt(_T("strTotal"), tmp3);
		//			strQuery.Format(_T("Update Product_Info SET strOk = %d, strNg = %d, strOmit = %d, strTotal = %d WHERE strName = '%s' AND strDate = '%s'"), 
		//				tmp+m_ReportOk, tmp1+m_ReportNg, tmp2+m_ReportOmit, tmp3+m_ReportTotal, g_ProductName, g_Today);
		//			m_pSet_1.Update_DB(strQuery);			
		//		}
		//	}
		//	if(m_pSet_1.b_IsOpenedDB)
		//		m_pSet_1.Close_DB();
		//}
	}
	else if(Num == 100 || Num == 101)
	{
		CString strtext;
		if (Num == 100)
			strtext.Format(g_MessageArr[134]);//_T("디텍터와 연결되지 않음"));
		else if (Num == 101)
			strtext = csDtErrorMsg;

		g_bOpenDB=true;

		Init_Mysql();

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			mysql_query(&m_mysql, "set names euckr");

			strQuery.Format(_T("Insert INTO intellisense_db6(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
				strdate, strtext, strtime);
			if (mysql_query(&m_mysql, CStringA(strQuery)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
		}

		Close_Mysql();

		//if(!m_pSet_6.Open_DB(g_StrDirPath[6]))		
		//{
		//	g_bOpenDB=false;
		//	//RecoveryDatabase(g_StrDirPath[6], g_StrDirPath_copy[6]);
		//	if(m_pSet_6.Open_DB(g_StrDirPath[6]))		
		//		g_bOpenDB=true;	
		//}
		//if(g_bOpenDB)
		//{
		//	strQuery.Format(_T("Insert INTO Detector_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
		//		strdate, strtext, strtime);
		//	m_pSet_6.Insert_DB(strQuery);
		//	if(m_pSet_6.b_IsOpenedDB)
		//		m_pSet_6.Close_DB();
		//}		
	}
	else if(Num == 200)
	{
		CString strtext;
		//EYH20110602
		strtext.Format(_T("%s%s(%d/%d/%d/%d/%d/%d)"), g_ProductName, g_MessageArr[135], g_Condition1, g_Condition2, g_Condition3, g_Condition4, g_Condition5, g_Condition6);
		bool g_bOpenDB=true;

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
	else if(Num == 300)
	{
		CString strtext;
		//EYH20110602
		strtext.Format(_T("%s%s(%d~%d/%d/%d~%d)"), g_ProductName, g_MessageArr[136], g_OmitCondition1, g_OmitCondition2, g_OmitCondition3, g_OmitCondition4, g_OmitCondition5);
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
	else if(Num == 350)
	{
		CString strtext;
		strtext.Format(_T("%s%s(%d/%d%d)"), g_ProductName, g_MessageArr[137], g_MaskBoxCondition1, g_MaskBoxCondition2,g_MaskBoxCondition3);
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


void CIntellisenseView::OnLoadValue(void)
{
	m_ProductName=g_ProductName;
	UpdateData(false);
	if(m_ProductName=="")
	{
		g_Condition1=0;
		g_Condition2=0;
		g_Condition3=0;
		g_Condition4=0;	
		g_Condition5=0;
		g_Condition6=0;	
	}	
	else
	{
		if (g_SelDetector == 0)
			ChangeDetectorValue();
		else
			ChangeDetectorValueC01B();
	}
	OnDispCondActive();
}


void CIntellisenseView::ChangeDetectorValue(void)
{
	if(g_bDetector)
	{
		float fSpeed = (float)g_DTScanspeed;
		float fsize;
		m_Commander.GetPixelSize(&fsize);
		float fIntTime = (fsize/fSpeed)*100000;
		m_Commander.PutIntegrationTime((long)fIntTime);
	}
}

void CIntellisenseView::ChangeDetectorValueC01B(void)
{
	if (g_bDetector)
	{
		float fSpeed = (float)g_DTScanspeed;
		uint64_t llsize;
		_xcommand.GetPara(XPARA_PIXEL_SIZE, llsize);
		float fsize = (float)llsize / 10.0;

		float fIntTime = (fsize / fSpeed) * 100000;
		//float fIntTime = 1000;
		_xcommand.SetPara(XPARA_INT_TIME, (long)fIntTime);
	}
}

void CIntellisenseView::CleanCycleAlarm()
{
	bool b_clean1=false;
	bool b_clean2=false;
	bool b_clean3=false;
	CTime time;
	time = time.GetCurrentTime();
	int weekly=time.GetDayOfWeek(); //1 일요일 
	int lday[12] = {31,0,31,30,31,30,31,31,30,31,30,31};
	if((time.GetYear() % 400) == 0 || (time.GetYear() % 100) != 0 && (time.GetYear() % 4)== 0)
	{
		lday[1] = 29;
	}
	else {
		lday[1] = 28;
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
		strSQL.Format(_T("SELECT * FROM intellisense_db10 WHERE strName LIKE 'Clean%%'"));
		if (mysql_query(&m_mysql, CStringA(strSQL)))
		{
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}

		m_Res = mysql_store_result(&m_mysql);


		while ((row = mysql_fetch_row(m_Res)) != NULL)
		{
			CString tmp(row[0]);
			CString tmpd(row[1]);
			CString m_Cycle(row[3]);
			if (tmp == "Clean1")
			{
				int m_Clean1 = _wtoi(tmpd);
				if (m_Clean1 == 1)
					b_clean1 = true;
				else if (m_Clean1 == 2 && weekly - 1 == _wtoi(m_Cycle))
					b_clean1 = true;
				else if (m_Clean1 == 3 && (time.GetDay() - 1 == _wtoi(m_Cycle) || (lday[time.GetMonth() - 1] == time.GetDay() && time.GetDay() <= _wtoi(m_Cycle))))
					b_clean1 = true;
			}
			else if (tmp == "Clean2")
			{
				int m_Clean2 = _wtoi(tmpd);
				if (m_Clean2 == 1)
					b_clean2 = true;
				else if (m_Clean2 == 2 && weekly - 1 == _wtoi(m_Cycle))
					b_clean2 = true;
				else if (m_Clean2 == 3 && (time.GetDay() - 1 == _wtoi(m_Cycle) || (lday[time.GetMonth() - 1] == time.GetDay() && time.GetDay() <= _wtoi(m_Cycle))))
					b_clean2 = true;
			}
			else if (tmp == "Clean3")
			{
				int m_Clean3 = _wtoi(tmpd);
				if (m_Clean3 == 1)
					b_clean3 = true;
				else if (m_Clean3 == 2 && weekly - 1 == _wtoi(m_Cycle))
					b_clean3 = true;
				else if (m_Clean3 == 3 && (time.GetDay() - 1 == _wtoi(m_Cycle) || (lday[time.GetMonth() - 1] == time.GetDay() && time.GetDay() <= _wtoi(m_Cycle))))
					b_clean3 = true;
			}
		}
		mysql_free_result(m_Res);
	}

	Close_Mysql();

	//if(!m_pSet_10.Open_DB(g_StrDirPath[10]))		
	//{	
	//	g_bOpenDB=false;
	//	//RecoveryDatabase(g_StrDirPath[10], g_StrDirPath_copy[10]);
	//	if(m_pSet_10.Open_DB(g_StrDirPath[10]))		
	//		g_bOpenDB=true;
	//}
	//if(g_bOpenDB)
	//{
	//	CString strSQL;
	//	strSQL.Format(_T("SELECT * FROM PartChange WHERE strName LIKE 'Clean%%'"));
	//	if(m_pSet_10.Select_DB(strSQL))
	//	{
	//		while(m_pSet_10.IsEOF()!=1)
	//		{
	//			CString tmp=_T("");
	//			m_pSet_10.ChangeRsChar(_T("strName"), tmp);
	//			CString tmpd=_T("");
	//			CString m_Cycle=_T("");
	//			m_pSet_10.ChangeRsChar(_T("strInstallDate"), tmpd); 
	//			if(tmp=="Clean1")
	//			{
	//				int m_Clean1=_wtoi(tmpd);
	//				m_pSet_10.ChangeRsChar(_T("strCycle"), m_Cycle);
	//				if(m_Clean1==1)
	//					b_clean1=true;
	//				else if(m_Clean1==2 && weekly-1==_wtoi(m_Cycle))
	//					b_clean1=true;
	//				else if(m_Clean1==3 && (time.GetDay()-1==_wtoi(m_Cycle) || (lday[time.GetMonth()-1]==time.GetDay() && time.GetDay()<=_wtoi(m_Cycle))))
	//					b_clean1=true;
	//			}
	//			else if(tmp=="Clean2")
	//			{
	//				int m_Clean2=_wtoi(tmpd);
	//				m_pSet_10.ChangeRsChar(_T("strCycle"), m_Cycle);
	//				if(m_Clean2==1)
	//					b_clean2=true;
	//				else if(m_Clean2==2 && weekly-1==_wtoi(m_Cycle))
	//					b_clean2=true;
	//				else if(m_Clean2==3 && (time.GetDay()-1==_wtoi(m_Cycle) || (lday[time.GetMonth()-1]==time.GetDay() && time.GetDay()<=_wtoi(m_Cycle))))
	//					b_clean2=true;
	//			}
	//			else if(tmp=="Clean3")
	//			{
	//				int m_Clean3=_wtoi(tmpd);
	//				if(m_Clean3==1)
	//					b_clean3=true;
	//				else if(m_Clean3==2 && weekly-1==_wtoi(m_Cycle))
	//					b_clean3=true;
	//				else if(m_Clean3==3 && (time.GetDay()-1==_wtoi(m_Cycle) || (lday[time.GetMonth()-1]==time.GetDay() && time.GetDay()<=_wtoi(m_Cycle))))
	//					b_clean3=true;
	//			}		
	//			m_pSet_10.MoveNext();
	//		}
	//	}
	//	if(m_pSet_10.b_IsOpenedDB)
	//		m_pSet_10.Close_DB();
	//}

	CString CleanMessage=g_MessageArr[138];//_T("점검주기 알람 서비스\n\n다음의 항목을 점검하십시요.\n\n");

	if(b_clean1!=false || b_clean2!=false || b_clean3!=false)
	{
		if(b_clean1)
			CleanMessage+=_T("\n") + g_MessageArr[139];//"- 에어컨 또는 팬 필터 및 구동상태\n";
	    if(b_clean2)
			CleanMessage+=_T("\n") + g_MessageArr[140];//"- 컨베이어벨트 상하 이물질 오염상태\n";
		if(b_clean3)
			CleanMessage+=_T("\n") + g_MessageArr[141];//"- 컨베이어 구동롤러 및 텐션상태\n";

		CleanMessage+=_T("\n") + g_MessageArr[142];//"\n더이상 알람을 보지않으시려면\n'설정->리포트->부품수명 정보'에서\n알람 '사용안함'을 선택하십시요.";
		CMessageBoxDlg dlg;
		dlg.m_Message=CleanMessage;
		dlg.DoModal();
	}
}


void CIntellisenseView::ClearResultProgress()
{
	CRect rect, rect1;
	CBitmap bmp, bmp1;
	bmp.LoadBitmapW(IDB_PROGRESS);
	bmp1.LoadBitmapW(IDB_PROGRESS_1);

	CClientDC dc(this);
	CDC bmDC;
	bmDC.CreateCompatibleDC(&dc); 

	bmDC.SelectObject(&bmp);
	m_PicProgress.GetWindowRect(&rect);
	ScreenToClient(&rect);
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),&bmDC,0,0, SRCCOPY);
	
	bmDC.SelectObject(&bmp1);
	m_PicProgress2.GetWindowRect(&rect1);
	ScreenToClient(&rect1);
	dc.BitBlt(rect1.left, rect1.top, rect1.Width(), rect1.Height(),&bmDC,0,0, SRCCOPY);
	
	bmp.DeleteObject();
	bmp1.DeleteObject();
	bmDC.DeleteDC();
}


void CIntellisenseView::MaskingClip()
{
/*	if(g_MaskClipUse || (g_MaskClip_Length==0 && g_MaskClip_Deep==0))	
		return;

	int lcx=0, lcy=0; 
	int rcx=0, rcy=0;
	int ucx=0, ucy=0; 
	int dcx=0, dcy=0;
	int i, j;
	double a=0.0;
	double b=0.0;
	int g_centerx=0, g_centery=0, count=0;
	int CondValue=210;
	for(i=g_Margin; i<g_Height-g_Margin;i++)
	{
		for(j=g_Margin; j<g_Width-g_Margin;j++)
		{	
			if(g_Img_Buf[i][j]<=CondValue)
			{
				count++;
				g_centerx+=i;
				g_centery+=j;
			}
		}
	}
	if(count==0)
		return;
	
	g_centerx=g_centerx/count;
	g_centery=g_centery/count;

	if(g_MaskClip_Direction==0)
	{
		for(j=g_Margin; j<g_Width-g_Margin;j++)
		{
			for(i=g_Margin;i<g_Height-g_Margin;i++)
			{	
				if(g_Img_Buf[i][j]<=180)
				{
					rcx=i;
					rcy=j;
					break;
				}
			}
		}
		for(j=g_Width-g_Margin; j>g_Margin;j--)
		{
			for(i=g_Height-g_Margin;i>g_Margin;i--)
			{	
				if(g_Img_Buf[i][j]<=180)
				{
					lcx=i;
					lcy=j;
					break;
				}
			}
		}
		//y=ax+b;
		//Left
		if(g_MaskClip_Length>=0 && g_MaskClip_Deep>=0)
		{
			if(lcx-g_centerx!=0)
				a=double(lcy-g_centery)/double(lcx-g_centerx);
			else
				a=0;
			b=lcy+1/a*lcx;
			for(i=lcx-g_MaskClip_Length;i<=lcx+g_MaskClip_Length;i++)
			{

				int k=(-1/a)*i+b;
				for(j=k; j<k+g_MaskClip_Deep;j++)
				{	
					if(i>0 && i<g_Height && j>0 && j<g_Width)
					{
						if(g_Img_Buf[i][j]<=210)
						{
							m_Img_Buf[i][j]=255;
							m_BadImg[i][j]=7;
						}
					}
				}
			}	
		//Right
			if(rcx-g_centerx!=0)
				a=double(rcy-g_centery)/double(rcx-g_centerx);
			else
				a=0;
			b=rcy+1/a*rcx;
			for(i=rcx-g_MaskClip_Length;i<=rcx+g_MaskClip_Length;i++)
			{
				int k=(int)((-1/a)*i+b);
				for(j=k; j>k-g_MaskClip_Deep;j--)
				{	
					if(i>0 && i<g_Height && j>0 && j<g_Width)
					{
						if(g_Img_Buf[i][j]<210)
						{
							m_Img_Buf[i][j]=255;
							m_BadImg[i][j]=7;
						}
					}
				}
			}
		}
	}
	else
	{
		for(i=g_Margin;i<g_Height-g_Margin;i++)
		{
			for(j=g_Margin; j<g_Width-g_Margin;j++)	
			{	
				if(g_Img_Buf[i][j]<=180)
				{
					ucx=i;
					ucy=j;
					break;
				}
			}
		}
		for(i=g_Height-g_Margin;i>g_Margin;i--)
		{
			for(j=g_Width-g_Margin; j>g_Margin;j--)
			{	
				if(g_Img_Buf[i][j]<=180)
				{
					dcx=i;
					dcy=j;
					break;
				}
			}
		}
		//y=ax+b;
		//Left
		if(g_MaskClip_Length>=0 && g_MaskClip_Deep>=0)
		{
			if(ucx-g_centerx!=0)
				a=double(ucy-g_centery)/double(ucx-g_centerx);
			else
				a=0;
			b=ucy+1/a*ucx;
			for(j=ucy-g_MaskClip_Length;j<=ucy+g_MaskClip_Length;j++)
			{
				int k=(j-b)/(-1/a);
				for(i=k; i>k-g_MaskClip_Deep;i--)
				{
					if(i>0 && i<g_Height && j>0 && j<g_Width)
					{
						if(g_Img_Buf[i][j]<=210)
						{
							m_Img_Buf[i][j]=255;
							m_BadImg[i][j]=7;
						}
					}
				}
			}
			//Right
			if(dcx-g_centerx!=0)
				a=double(dcy-g_centery)/double(dcx-g_centerx);
			else
				a=0;
			b=dcy+1/a*dcx;
			for(j=dcy-g_MaskClip_Length;j<=dcy+g_MaskClip_Length;j++)
			{
				int k=(j-b)/(-1/a);
				for(i=k; i<k+g_MaskClip_Deep;i++)
				{	
					if(i>0 && i<g_Height && j>0 && j<g_Width)
					{
						if(g_Img_Buf[i][j]<210)
						{
							m_Img_Buf[i][j]=255;
							m_BadImg[i][j]=7;
						}
					}
				}
			}
		}		
	}*/
}

void CIntellisenseView::MaskingPaper()
{
/*	return;
	if(g_PageMaskLR==0 && g_PageMaskUD==0)
		return;

	int LRCount=2;
	int UPCount=2;
	int i, j;

	int CenterX=0, CenterY=0, count=0;
	int CondValue=220;
	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{
		for(j=g_Margin; j<g_Width-g_Margin;j++)
		{	
			if(m_Buffer_1[i][j]<=CondValue)
			{
				count++;
				CenterX+=i;
				CenterY+=j;
			}
		}
	}
	if(count==0)
		return;
	
	CenterX=CenterX/count;
	CenterY=CenterY/count;

	if(CenterX!=0 && CenterY!=0)
	{
		CondValue=200;
		int LeftDownX=CenterX, LeftDownY=CenterY;
		int RightDownX=CenterX, RightDownY=0;
		int LeftUpX=0, LeftUpY=CenterY;
		int RightUpX=0, RightUpY=0;
		
		int max1=0, max2=0, max3=0, max4=0;
		for(i=g_Margin;i<g_Height-g_Margin;i++)
		{
			for(j=g_Margin; j<g_Width-g_Margin;j++)
			{	
				if(m_Buffer_1[i][j]<=CondValue)
				{
					if((i>=0 && i<CenterX-20) && (j>=0 && j<CenterY-20)) 
					{
						if(max1<=(CenterX-i)*(CenterX-i)+(CenterY-j)*(CenterY-j))
						{
							max1=(CenterX-i)*(CenterX-i)+(CenterY-j)*(CenterY-j);
							LeftDownX=i;
							LeftDownY=j;
						}
					}
					if((i>=0 && i<CenterX-20) && (j>=CenterY+20 && j<g_Width)) 
					{
						if(max2<=(CenterX-i)*(CenterX-i)+(CenterY-j)*(CenterY-j))
						{
							max2=(CenterX-i)*(CenterX-i)+(CenterY-j)*(CenterY-j);
							RightDownX=i;
							RightDownY=j;
						}
					}
					if((i>=CenterX+20 && i<g_Height) && (j>=0 && j<CenterY-20)) 
					{
						if(max3<=(CenterX-i)*(CenterX-i)+(CenterY-j)*(CenterY-j))
						{
							max3=(CenterX-i)*(CenterX-i)+(CenterY-j)*(CenterY-j);
							LeftUpX=i;
							LeftUpY=j;
						}
					}	
					if((i>=CenterX+20 && i<g_Height) && (j>=CenterY+20 && j<g_Width)) 
					{
						if(max4<=(CenterX-i)*(CenterX-i)+(CenterY-j)*(CenterY-j))
						{
							max4=(CenterX-i)*(CenterX-i)+(CenterY-j)*(CenterY-j);
							RightUpX=i;
							RightUpY=j;
						}
					}	
				}
			}
		}

		int PerX=0, PerY=0, PerX1=0, PerY1=0;
		PerX=(LeftUpX-LeftDownX)/LRCount;
		PerY=(LeftUpY-LeftDownY)/LRCount;
		PerX1=(RightUpX-RightDownX)/LRCount;
		PerY1=(RightUpY-RightDownY)/LRCount;

		int xpoint[4][4], ypoint[4][4];
		for(i=0; i<LRCount; i++)
		{
			for(j=0; j<=UPCount; j++)
			{
				xpoint[i][j]=0;
				ypoint[i][j]=0;
			}
		}

		for(i=0; i<=LRCount; i++)
		{
			xpoint[i][0]=LeftDownX+((LeftUpX-LeftDownX)*i)/LRCount; 
			ypoint[i][0]=LeftDownY+((LeftUpY-LeftDownY)*i)/LRCount;
			xpoint[i][UPCount]=RightDownX+((RightUpX-RightDownX)*i)/LRCount; 
			ypoint[i][UPCount]=RightDownY+((RightUpY-RightDownY)*i)/LRCount;
		}

		for(i=0; i<=LRCount; i++)
		{
			for(j=1; j<UPCount; j++)
			{
				xpoint[i][j]=xpoint[i][0]+((xpoint[i][UPCount]-xpoint[i][0])*j)/UPCount; 
				ypoint[i][j]=ypoint[i][0]+((ypoint[i][UPCount]-ypoint[i][0])*j)/UPCount;
			}
		}

		POINT pt[2][4];
		int k=0;

		pt[0][0].x=xpoint[1][0]+g_PageMaskLR; pt[0][0].y=ypoint[1][0];
		pt[0][1].x=xpoint[1][2]+g_PageMaskLR; pt[0][1].y=ypoint[1][2];
		pt[0][2].x=xpoint[1][2]-g_PageMaskLR; pt[0][2].y=ypoint[1][2];
		pt[0][3].x=xpoint[1][0]-g_PageMaskLR; pt[0][3].y=ypoint[1][0];

		pt[1][0].x=xpoint[0][1]; pt[1][0].y=ypoint[0][1]-g_PageMaskUD;
		pt[1][1].x=xpoint[0][1]; pt[1][1].y=ypoint[0][1]+g_PageMaskUD;
		pt[1][2].x=xpoint[2][1]; pt[1][2].y=ypoint[2][1]+g_PageMaskUD;
		pt[1][3].x=xpoint[2][1]; pt[1][3].y=ypoint[2][1]-g_PageMaskUD;
 
		 unsigned int xnew,ynew;
		 unsigned int xold,yold;
		 unsigned int x1,y1;
		 unsigned int x2,y2;
		 int inside=0;
		 int kst=0;
		 int ked=1;

		if(g_PageMaskLR==0)
			kst=1;
		if(g_PageMaskUD==0)
			ked=0;

  		for(i=g_Margin;i<g_Height-g_Margin;i++)
		{
			for(j=g_Margin; j<g_Width-g_Margin;j++)
			{	
				for(k=kst; k<=ked; k++)
				{
					 xold=pt[k][4-1].x; 
					 yold=pt[k][4-1].y; 
					 inside=0;
					 for(int z=0 ; z<4 ; z++) 
					 {
						  xnew=pt[k][z].x;
						  ynew=pt[k][z].y;
						  if(xnew > xold) 
						  {
							   x1=xold;
							   x2=xnew;
							   y1=yold;
							   y2=ynew;
						  }
						  else 
						  {
							   x1=xnew;
							   x2=xold;
							   y1=ynew;
							   y2=yold;
						  }

						  if ((xnew < i) == (i <= xold)        
						   && ((long)j-(long)y1)*(long)(x2-x1)<((long)y2-(long)y1)*(long)(i-x1)) 
						  {
							   inside=!inside;
						  }

						  xold=xnew;
						  yold=ynew;
					 }

					 if(inside)
					 {
						m_Img_Buf[i][j]=255;
						m_BadImg[i][j]=7;
					 }
				}
			}
		}
	}*/
}

void CIntellisenseView::MaskingTalOx(void)
{
	/*
	if(g_MaskTalUse || (g_TalMask_Bright==0 || g_TalMask_Bright2==0))
		return;

	int i=0, j=0;
	int cd=0;
	for(i=0; i<g_Height; i++)
	{
		for(j=0; j<g_Width; j++)
		{
			g_Buffer_Label[i][j]=0;
			stackx[cd]=0;
			stacky[cd]=0;
			cd++;
		}
	}

	int max_num_h=0;
	int row_num_h=100000;
	int max_num_w=0;
	int row_num_w=100000;

	int Max_num =0;
	int m_Max=0;
	int m_Max_Area=200;
	int m_Max_Number=0;

	int m,n,top;
	short curColor=0, r, c, area;

	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{
		for(j=g_Margin;j<g_Width-g_Margin;j++)
		{		
			// 이미 방문한 점이거나 픽셀값이 255가 아니라면 처리 안함 
			if(g_Buffer_Label[i][j]!=0 || m_Img_Buf[i][j]<=g_TalMask_Bright && m_BadImg[i][j] != 7) 
				continue;

			r=i; 
			c=j;
			top=0; area=1;
			curColor++;
			m_Max += 1;
			area = 0;
			while(1) 
			{
GRASSFIRETAL:
				for(m=r-5; m<=r+5; m++)
				{
					for(n=c-5; n<=c+5; n++) 
					{
						if(m<0 || m>=g_Height || n<0 || n>=g_Width) 
							continue;

						if(m_Img_Buf[m][n]<=g_TalMask_Bright && g_Buffer_Label[m][n]==0)
						{
							g_Buffer_Label[m][n]=curColor; 
							if(push(stackx, stacky,(short)m,(short)n,&top)==-1) 
								continue;
							r=m; 
							c=n;
							area++;
									
							if(area>m_Max_Area)
							{ 
								if(max_num_h < r)
									max_num_h = r;
								if(row_num_h > r)
									row_num_h = r;
								if(max_num_w < c)
									max_num_w = c;
								if(row_num_w > c)
									row_num_w = c;
								m_Max_Number=m_Max;
								m_Max_Area=area;	
							}
							goto GRASSFIRETAL;
						}
					}
				}
				if(pop(stackx, stacky, &r, &c, &top)==-1) 
					break;
				
			}
			
		}
	}

	//실제 탈산소재 마스킹 영역
	if(row_num_h != 0 || row_num_w !=0)
	{
		for(i=row_num_h-g_TalMask_Bright2;i<max_num_h+g_TalMask_Bright2;i++)
		{
			for(j=row_num_w-g_TalMask_Bright2;j<max_num_w+g_TalMask_Bright2;j++)
			{
				if(i>0 && i<g_Height && j>0 && j<g_Width)
				{
					m_Img_Buf[i][j]=255; //실제 검사 제외 영역
					m_BadImg[i][j]=7;
				}
			}
		}
	}
/*		
	//화면상 보여주는 마스킹 영역
	int m_iGap = 5;
	for(i=g_Margin; i<g_Height-g_Margin; i++)
	{
		for(j=g_Margin; j<g_Width-g_Margin; j++)
		{
			if(g_Buffer_Label[i][j]==m_Max_Number)
			{
				for(int b=0; b<3; b++)
				{
					m_BadImg[i][j]=7;
					m_BadImg[i-m_iGap+b][j]=7;
					m_BadImg[i-m_iGap+b][j+m_iGap+b]=7;
					m_BadImg[i][j+m_iGap+b]=7;
					m_BadImg[i+m_iGap+b][j+m_iGap+b]=7;
					m_BadImg[i+m_iGap+b][j]=7;
					m_BadImg[i+m_iGap+b][j-m_iGap+b]=7;
					m_BadImg[i][j-m_iGap+b]=7;
					m_BadImg[i-m_iGap+b][j-m_iGap+b]=7;
				}
			}
		}
	}
*/
}

bool CIntellisenseView::Detect_Omit(int I_Count)
{
	if(g_OmitCondUse || g_OmitCondition3==0)
	{

		g_OmitCondition3_Disp=0;
		m_OmitAve=100;
		GetDlgItem(IDC_OMITAVE)->SetWindowText(_T(""));
		ClearResultProgress();
		return true;
	}
	/*
	m_OmitAve=0;
	int i, j;

	g_OmitCondition3_Disp=0;
	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{
		for(j=g_Margin; j<g_Width-g_Margin;j++)
		{	
			if(g_OmitCondition1==0 && g_OmitCondition2==0)
			{
				if(m_Buffer_1[I_Count][i][j]<220)
				{
					g_OmitCondition3_Disp += (220-m_Buffer_1[I_Count][i][j])/10;
				}
			}
			else
			{
				if((int)m_Buffer_1[I_Count][i][j]>=g_OmitCondition1 && (int)m_Buffer_1[I_Count][i][j]<=g_OmitCondition2)
				{
					if(m_Buffer_1[I_Count][i][j]<220)
					{
						g_OmitCondition3_Disp += (220-m_Buffer_1[I_Count][i][j])/10;
						m_Buffer_4[i*g_Width+j]=255;
					}
				}
			}
		}
	}
	g_OmitCondition3_Disp/=10;

	if(g_OmitCondition3!=0)
	{
		m_OmitAve=((g_OmitCondition3_Disp*100)/g_OmitCondition3);
		g_OmitAvr = m_OmitAve;
	}
	if(m_OmitAve<=0)
		m_OmitAve=0;*/

	m_OmitAve=0;
	int i, j, soupcount=0;
	for(i=g_Margin2+g_OmitCondition1;i<=g_Height-g_OmitCondition2;i++)
	{
		for(j=g_Margin; j<g_Width-g_Margin; j++)
		{	
			if(m_Buffer_1[I_Count][i][j]< g_TalMask_Bright2)
			{
				soupcount += (g_TalMask_Bright2 -m_Buffer_1[I_Count][i][j])/10;		// 가변설정 값으로 임시조치 20240701
				m_Buffer_4[i*g_Width+j]=0;

		//		soupcount += 1;
			}
			if(i==g_Margin2+g_OmitCondition1 || i==g_Height-g_OmitCondition2 )
			{
				m_BadImg[i][j]=8;
			}
		}
	}
	
	g_OmitCondition3_Disp = soupcount;
	
	if(g_OmitCondition3!=0)
	{
		m_OmitAve=((g_OmitCondition3_Disp*100)/g_OmitCondition3);
		g_OmitAvr = m_OmitAve;
	}
	
	if(m_OmitAve<=0)
		m_OmitAve=0;
		
	CString ss;
	ss.Format(_T("%d%%"), m_OmitAve);
	GetDlgItem(IDC_OMITAVE)->SetWindowText(ss);


	//WriteLog(_T("omit end"), I_Count);

	//	if(soupcount>=int(g_OmitCondition3 * g_OmitCondition4/100)) 
	if(g_OmitCondition3_Disp<int(g_OmitCondition3*g_OmitCondition4/100) )
	{
		g_KindofOmit =1;
		return false;
	}
	else if(g_OmitCondition3_Disp>int(g_OmitCondition3*g_OmitCondition5/100))
	{
		g_KindofOmit = 2;
		return false;
	}
	else
		return true;

	




}


//20160428
bool CIntellisenseView::Detect_Openbox(void)
{
	/*
	if(g_MassCondUse)
	{
		return true;
	}

	int i, j;
	int cx=0;
	int cy=0;
	int ccount=0;

	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{
		for(j=g_Margin; j<g_Width-g_Margin;j++)
		{	
			if(m_Buffer_1[i][j]<=220)
			{
				cx+=i;
				cy+=j;
				ccount++;
			}
		}
	}

	if(ccount!=0)
	{
		cx/=ccount;
		cy/=ccount;
	}
	else
		return true;

	int suml=0;
	for(i=g_Margin; i<g_Height-g_Margin; i++)
	{
		for(j=g_Margin; j<cx; j++)
		{
			if(m_Buffer_1[i][j]<=230)
			{
				suml+=(230*5-(m_Buffer_1[i][j+1]+m_Buffer_1[i][j+2]+m_Buffer_1[i][j+3]+m_Buffer_1[i][j+4]+m_Buffer_1[i][j+5]));
				j=cx;
			}
		}
	}

	int sumr=0;
	for(i=g_Margin; i<g_Height-g_Margin; i++)
	{
		for(j=g_Width-g_Margin; j>cx; j--)
		{
			if(m_Buffer_1[i][j]<=230)
			{
				sumr+=(230*5-(m_Buffer_1[i][j-1]+m_Buffer_1[i][j-2]+m_Buffer_1[i][j-3]+m_Buffer_1[i][j-4]+m_Buffer_1[i][j-5]));
				j=0;
			}
		}
	}
/*
	if(suml!=0)
		suml=((suml*100)/g_OmitCondition1);
	if(sumr!=0)
		sumr=((sumr*100)/g_OmitCondition1);
*/
/*	if(suml<=0)
		suml=0;
	if(sumr<=0)
		sumr=0;

	
	suml/=10;
	sumr/=10;
	g_DispOpenBox1=suml;
	g_DispOpenBox2=sumr;

	bool result=true;
	if(suml>=int(g_MassCondition1*g_MassCondition2/100) && suml<=int(g_MassCondition1*g_MassCondition3/100))
	{

	}
	else
	{
		for(i=g_Margin; i<g_Height-g_Margin; i++)
		{
			for(j=g_Margin; j<cx; j++)
			{
				if(m_Buffer_1[i][j]<=230)
				{
					m_BadImg[i][j-4]=8;
					m_BadImg[i][j+4]=8;
					m_BadImg[i][j-5]=8;
					m_BadImg[i][j+5]=8;
					j=cx;
				}
			}
		}
		result=false;
	}
	if(sumr>=int(g_MassCondition1*g_MassCondition2/100) && sumr<=int(g_MassCondition1*g_MassCondition3/100))
	{

	}
	else 
	{
		for(i=g_Margin; i<g_Height-g_Margin; i++)
		{
			for(j=g_Width-g_Margin; j>cx; j--)
			{
				if(m_Buffer_1[i][j]<=230)
				{
					m_BadImg[i][j-4]=8;
					m_BadImg[i][j-4]=8;
					m_BadImg[i][j+5]=8;
					m_BadImg[i][j+5]=8;
					j=cx;
				}
			}
		}
		result=false;
	}	
	if(g_MassCondition1==0)
		return true;*/
	return 0;
}

bool CIntellisenseView::Detect_Cond1(int I_Count)
{
	int i, j, cond1_num=0, m_MinValue=0;
	bool result=true;
	
	//20160707 검사범위수정
	//for(i=g_Margin;i<g_Height-g_Margin;i++)
	for(i= g_MaskBoxCondition2 + g_Margin2+ 50; i<g_Height-g_Margin; i++)
	//for(i=g_MaskBoxCondition1+g_Margin;i<g_Height-g_MaskBoxCondition2;i++)
	{
		for(j=g_Margin;j<g_Width-g_Margin;j++) 
		{
			if(g_Margin+120)
			{
				if(g_Condition1+5<=255-g_Img_Bufx2[I_Count][i][j] && g_Condition1!=0)
				{
					cond1_num++;
					m_BadImg[i][j] = 1;
					m_BadImg_Save[I_Count][i][j] = 1;
					result=false;
				}
			}
			else
			{
				if(g_Condition1<255-g_Img_Bufx2[I_Count][i][j] && g_Condition1!=0)
				{
					cond1_num++;
					m_BadImg[i][j]=1;
					m_BadImg_Save[I_Count][i][j]=1;
				}
			}
				
			if(m_MinValue<=255-g_Img_Bufx2[I_Count][i][j])
			{
				m_MinValue=255-g_Img_Bufx2[I_Count][i][j];
			}

		}
	}
	
	
	g_BackupCond1[g_BackupRear]=m_MinValue;
	if(cond1_num>=1 || !result)
	{
		if(b_TestMode)//20140821
			m_CountCond1++;
		return false;
	}
	else
		return true;
}

bool CIntellisenseView::Detect_Cond2(int I_Count)
{
	int i, j;
	bool result=true;

	int t=2;
	//int m_iArray = t*8;
	int m_iTemp[16];
	int a1,a2,a3,a4=0;
	for(i=0; i<8; i++)
		m_iTemp[i]=0;
	//20160707
	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{		
		for(j=g_Margin;j<g_Width-g_Margin;j++) 
		{
				if((g_Img_Bufx2[I_Count][i][j]<160) && (g_Img_Bufx2[I_Count][i-t][j]<220) && (g_Img_Bufx2[I_Count][i-t][j+t]<220) 
					&& (g_Img_Bufx2[I_Count][i][j+t]<220) && (g_Img_Bufx2[I_Count][i+t][j+t]<220) 
					&& (g_Img_Bufx2[I_Count][i+t][j]<220) && (g_Img_Bufx2[I_Count][i+t][j-t]<220)
					&& (g_Img_Bufx2[I_Count][i][j-t]<220) && (g_Img_Bufx2[I_Count][i-t][j-t]<220))
				{ 

				m_iTemp[0] = g_Img_Bufx2[I_Count][i+1][j-2] - g_Img_Bufx2[I_Count][i][j];
				m_iTemp[1] = g_Img_Bufx2[I_Count][i+2][j-2] - g_Img_Bufx2[I_Count][i][j];
				m_iTemp[2] = g_Img_Bufx2[I_Count][i+2][j-1] - g_Img_Bufx2[I_Count][i][j];
				m_iTemp[3] = g_Img_Bufx2[I_Count][i+2][j]   - g_Img_Bufx2[I_Count][i][j];
				m_iTemp[4] = g_Img_Bufx2[I_Count][i+2][j+1] - g_Img_Bufx2[I_Count][i][j];
				m_iTemp[5] = g_Img_Bufx2[I_Count][i+2][j+2] - g_Img_Bufx2[I_Count][i][j];
				m_iTemp[6] = g_Img_Bufx2[I_Count][i+1][j+2] - g_Img_Bufx2[I_Count][i][j];
			
				a1 = int((m_iTemp[0]+m_iTemp[1]+m_iTemp[2]+m_iTemp[3]+m_iTemp[4]+m_iTemp[5]+m_iTemp[6])/7);

				a2 = 200-g_Img_Bufx2[I_Count][i][j];
				a3 = int((a1*1.5)+((a2/10)*5));
				if(i>g_Margin2+70){ //kds20140701
					a3 = a3-25;
				}
					if(a3>=g_Condition2-10)
				{
					//정렬 처리>>>>>올림차순
					int tmp=0;
					int max=0;
					for (int a=0; a < 6; a++){
						for (int b=a+1; b < 7; b++){
							if (m_iTemp[b] < m_iTemp[a]){
								tmp = m_iTemp[b];
								m_iTemp[b] = m_iTemp[a];
								m_iTemp[a] = tmp;
							}
						}
					}
					//max = m_iTemp[1];
					max = m_iTemp[0];
					a2 = 200-g_Img_Bufx2[I_Count][i][j];
					a3 = int((max*1.5)+((a2/10)*5));
					if(i>g_Margin2+70){ //kds20140701
 						a3 = a3-15;
 					}
					//<<<<<<<<<<<<<<<<
					if(a3>=g_Condition2 && g_Condition2!=0){
						m_BadImg[i][j]=2;
						m_BadImg_Save[I_Count][i][j]=2;
						result=false;
					}
				}
				if(a3>g_BackupCond2[g_BackupRear]){  //검사 최고값
					g_BackupCond2[g_BackupRear] = a3;

				}
				
				//검사 최고값 등급 변경 
				

				/*	a4 = g_Img_Bufx2[I_Count][i][j];
			
					int m_count=0; 
					for(int a=0-t;a<=t;a++) 
					{ 
						m_iTemp[m_count]=g_Img_Bufx2[I_Count][i+a][j-t]-a4; 
						m_count++; 
						m_iTemp[m_count]=g_Img_Bufx2[I_Count][i+a][j+t]-a4; 
						m_count++; 
						if(a<t-1){
							m_iTemp[m_count]=g_Img_Bufx2[I_Count][i-t][j+a+1]-a4; 
							m_count++; 
							m_iTemp[m_count]=g_Img_Bufx2[I_Count][i+t][j+a+1]-a4; 
							m_count++; 
						}
					}

					int m_tot=0;
					for(int z=0;z<m_iArray;z++){
						m_tot += m_iTemp[z];
					}
					a1 = m_tot/m_iArray;

					a2 = 200-a4;
					a3 = int((a1*3)+((a2/10)*3));

					if(a3>=g_Condition2-20)
					{
						//정렬 처리>>>>>올림차순
						int tmp=0;
						int max=0;
						for(int a=0; a < m_iArray-1; a++){
							for (int b=a+1; b < m_iArray; b++){
								if (m_iTemp[b] < m_iTemp[a]){
									tmp = m_iTemp[b];
									m_iTemp[b] = m_iTemp[a];
									m_iTemp[a] = tmp;
								}
							}
						}
	
						max = m_iTemp[1] + m_iTemp[3] + m_iTemp[4];
						max = max/3;

						a2 = 200-a4;
						a3 = int((max*3)+((a2/10)*3));
						if(a3>255)
							a3=255;
						//<<<<<<<<<<<<<<<<
						if(a3>=g_Condition2 && g_Condition2!=0){
							m_BadImg[i][j]=2;
							m_BadImg_Save[I_Count][i][j] = 2;
							result=false;
						}
					}

					if(a3>g_BackupCond2[g_BackupRear])
					{  //검사 최고값
						g_BackupCond2[g_BackupRear] = a3;
					}*/
				}	
			
		}
	}

	if(g_Condition2==0)
		return true;

	if(!result)
	{	
		if(b_TestMode) //20140821
			m_CountCond2++;
		return false;
	}
	else
		return true;
}

void CIntellisenseView::OnDispCondActive(void)
{
	if(g_Condition1==0)
		GetDlgItem(IDC_MAINCOND1)->ShowWindow(false);
	else
	{
		m_PicCond1.SetBitmap(m_bmpMaincond[0]);
		GetDlgItem(IDC_MAINCOND1)->ShowWindow(true);
	}
	if(g_Condition2==0)
		GetDlgItem(IDC_MAINCOND2)->ShowWindow(false);
	else
	{
		m_PicCond2.SetBitmap(m_bmpMaincond[1]);
		GetDlgItem(IDC_MAINCOND2)->ShowWindow(true);
	}
	if(g_Condition3==0)
		GetDlgItem(IDC_MAINCOND3)->ShowWindow(false);
	else
	{
		m_PicCond3.SetBitmap(m_bmpMaincond[2]);
		GetDlgItem(IDC_MAINCOND3)->ShowWindow(true);
	}
	if(g_Condition4==0)
		GetDlgItem(IDC_MAINCOND4)->ShowWindow(false);
	else
	{
		m_PicCond4.SetBitmap(m_bmpMaincond[3]);
		GetDlgItem(IDC_MAINCOND4)->ShowWindow(true);
	}
	if(g_Condition5==0)
		GetDlgItem(IDC_MAINCOND5)->ShowWindow(false);
	else
	{
		m_PicCond5.SetBitmap(m_bmpMaincond[4]);
		GetDlgItem(IDC_MAINCOND5)->ShowWindow(true);
	}
	if(g_Condition6==0)
		GetDlgItem(IDC_MAINCOND6)->ShowWindow(false);
	else
	{
		m_PicCond6.SetBitmap(m_bmpMaincond[5]);
		GetDlgItem(IDC_MAINCOND6)->ShowWindow(true);
	}

}

bool CIntellisenseView::Detect_Cond3(int I_Count)     //kds20080728
{
	int i, j;
	bool result=true;

	int t=5;
	//int m_iArray = t*8;
	int m_iTemp[32];
	int a1,a2,a3,a4=0;
	for(i=0; i<8; i++)
		m_iTemp[i]=0;
	//20160707
	//for(i=g_Margin;i<g_Height-g_Margin;i++)
	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{		
		//for(j=iv_1;j<iv_2;j++) 
		for(j=g_Margin;j<g_Width-g_Margin;j++)
		{
			
				if( (g_Img_Bufx2[I_Count][i][j]<160) && (g_Img_Bufx2[I_Count][i-t][j]<220) && (g_Img_Bufx2[I_Count][i-t][j+t]<220) 
					&& (g_Img_Bufx2[I_Count][i][j+t]<220) && (g_Img_Bufx2[I_Count][i+t][j+t]<220) 
					&& (g_Img_Bufx2[I_Count][i+t][j]<220) && (g_Img_Bufx2[I_Count][i+t][j-t]<220)
					&& (g_Img_Bufx2[I_Count][i][j-t]<220) && (g_Img_Bufx2[I_Count][i-t][j-t]<220))
				{ 
					a4=g_Img_Bufx2[I_Count][i-1][j]+g_Img_Bufx2[I_Count][i-1][j+1]+g_Img_Bufx2[I_Count][i][j+1]+g_Img_Bufx2[I_Count][i+1][j+1]+g_Img_Bufx2[I_Count][i+1][j]+g_Img_Bufx2[I_Count][i+1][j-1]+g_Img_Bufx2[I_Count][i][j-1]+g_Img_Bufx2[I_Count][i-1][j-1]+g_Img_Bufx2[I_Count][i][j];
					a4 = (int)a4/9;	


					m_iTemp[0] = g_Img_Bufx2[I_Count][i-t][j]-a4;
					m_iTemp[1] = g_Img_Bufx2[I_Count][i-t][j+t]-a4;
					m_iTemp[2] = g_Img_Bufx2[I_Count][i][j+t]-a4;
					m_iTemp[3] = g_Img_Bufx2[I_Count][i+t][j+t]-a4;
					m_iTemp[4] = g_Img_Bufx2[I_Count][i+t][j]-a4;
					m_iTemp[5] = g_Img_Bufx2[I_Count][i+t][j-t]-a4;
					m_iTemp[6] = g_Img_Bufx2[I_Count][i][j-t]-a4;
					m_iTemp[7] = g_Img_Bufx2[I_Count][i-t][j-t]-a4;

					a1 = int((m_iTemp[0]+m_iTemp[1]+m_iTemp[2]+m_iTemp[3]+m_iTemp[4]+m_iTemp[5]+m_iTemp[6]+m_iTemp[7])/8);


					a2 = 200-a4;
					a3 = int((a1*1.5)+((a2/10)*5));
 					if(i>g_Margin2+70){ //kds20140701
 						a3 = a3-10;
 					}

					if(a3>=g_Condition3-10)
					{
						//정렬 처리>>>>>올림차순
						int tmp=0;
						int max=0;
						for (int a=0; a < 7; a++){
							for (int b=a+1; b < 8; b++){
								if (m_iTemp[b] < m_iTemp[a]){
									tmp = m_iTemp[b];
									m_iTemp[b] = m_iTemp[a];
									m_iTemp[a] = tmp;
								}
							}
						}
						max = m_iTemp[1];
						a2 = 200-a4;
						a3 = int((max*1.5)+((a2/10)*5));
 						if(i>g_Margin2+70){ //kds20140701
							a3 = a3-10;
 						}
						//<<<<<<<<<<<<<<<<
						if(a3>=g_Condition3 && g_Condition3!=0){
							m_BadImg[i][j]=3;
							m_BadImg_Save[I_Count][i][j]=3;
							result=false;
						}
					}
					/*
					int m_count=0; 
					for(int a=0-t;a<=t;a++) 
					{ 
						m_iTemp[m_count]=g_Img_Bufx2[I_Count][i+a][j-t]-a4; 
						m_count++; 
						m_iTemp[m_count]=g_Img_Bufx2[I_Count][i+a][j+t]-a4; 
						m_count++; 
						if(a<t-1){
							m_iTemp[m_count]=g_Img_Bufx2[I_Count][i-t][j+a+1]-a4; 
							m_count++; 
							m_iTemp[m_count]=g_Img_Bufx2[I_Count][i+t][j+a+1]-a4; 
							m_count++; 
					}
				}

				int m_tot=0;
				for(int z=0;z<m_iArray;z++){
					m_tot += m_iTemp[z];
				}
				a1 = m_tot/m_iArray;

				a2 = int(((160-a4)*2)/2.5);
				a3 = int((a1*3)+a2);

				if(a3>=g_Condition3-20)
				{
					//정렬 처리>>>>>올림차순
					int tmp=0;
					int max=0;
					for (int a=0; a < m_iArray-1; a++){
						for (int b=a+1; b < m_iArray; b++){
							if (m_iTemp[b] < m_iTemp[a]){
								tmp = m_iTemp[b];
								m_iTemp[b] = m_iTemp[a];
								m_iTemp[a] = tmp;
							}
						}
					}
					max = m_iTemp[1] + m_iTemp[5] + m_iTemp[8];
					max = max/3;

					a2 = int(((160-a4)*2)/2.5);
					a3 = int((max*3)+a2);
					if(a3>255)
						a3=255;
					//<<<<<<<<<<<<<<<<
					if(a3>=g_Condition3 && g_Condition3!=0){
						m_BadImg[i][j]=3;
						m_BadImg_Save[I_Count][i][j]=3;
						result=false;
					}
				}*/

				if(a3>g_BackupCond3[g_BackupRear])
				{  //검사 최고값
					g_BackupCond3[g_BackupRear] = a3;
				}
			}
		}
	}

	if(g_Condition3==0)
		return true;

	if(!result)
	{
		if(b_TestMode) //20140821
			m_CountCond3++;
		return false;
	}
	else
		return true;

}

bool CIntellisenseView::Detect_Cond4(int I_Count)   //kds20080728
{
	int i, j;
	bool result=true;

	int t=8;
	int m_iArray = t*8;
	int m_iTemp[64];
	int a1,a2,a3,a4=0;
	for(i=0; i<m_iArray; i++)
		m_iTemp[i]=0;
	//20160707
	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{		
		//for(j=iv_1;j<iv_2;j++) 
		for(j=g_Margin;j<g_Width-g_Margin;j++) 
		{
			if((g_Img_Bufx2[I_Count][i][j]<160) && (g_Img_Bufx2[I_Count][i-t][j]<220) && (g_Img_Bufx2[I_Count][i-t][j+t]<220) 
				&& (g_Img_Bufx2[I_Count][i][j+t]<220) && (g_Img_Bufx2[I_Count][i+t][j+t]<220) 
				&& (g_Img_Bufx2[I_Count][i+t][j]<220) && (g_Img_Bufx2[I_Count][i+t][j-t]<220)
				&& (g_Img_Bufx2[I_Count][i][j-t]<220) && (g_Img_Bufx2[I_Count][i-t][j-t]<220) )
			{ 
				a4=g_Img_Bufx2[I_Count][i-2][j]+g_Img_Bufx2[I_Count][i-2][j+2]+g_Img_Bufx2[I_Count][i][j+2]+g_Img_Bufx2[I_Count][i+2][j+2]+g_Img_Bufx2[I_Count][i+2][j]+g_Img_Bufx2[I_Count][i+2][j-2]+g_Img_Bufx2[I_Count][i][j-2]+g_Img_Bufx2[I_Count][i-2][j-2]+g_Img_Bufx2[I_Count][i][j];
				a4 = (int)a4/9;

				m_iTemp[0] = g_Img_Bufx2[I_Count][i-t][j]-a4;
				m_iTemp[1] = g_Img_Bufx2[I_Count][i-t][j+t]-a4;
				m_iTemp[2] = g_Img_Bufx2[I_Count][i][j+t]-a4;
				m_iTemp[3] = g_Img_Bufx2[I_Count][i+t][j+t]-a4;
				m_iTemp[4] = g_Img_Bufx2[I_Count][i+t][j]-a4;
				m_iTemp[5] = g_Img_Bufx2[I_Count][i+t][j-t]-a4;
				m_iTemp[6] = g_Img_Bufx2[I_Count][i][j-t]-a4;
				m_iTemp[7] = g_Img_Bufx2[I_Count][i-t][j-t]-a4;

				a1 = int((m_iTemp[0]+m_iTemp[1]+m_iTemp[2]+m_iTemp[3]+m_iTemp[4]+m_iTemp[5]+m_iTemp[6]+m_iTemp[7])/8);
			//	a1 = int((m_iTemp[0]+m_iTemp[2]+m_iTemp[4]+m_iTemp[6]/4));

				a2 = 200-a4;
				a3 = int((a1*1.5)+((a2/10)*5));
 				if(i>g_Margin2+40){				// 20251020 동원 커스텀 a3 조건 삭제
 					a3 = a3-30;
 				}

				if(a3>=g_Condition4-10)
				{
					//정렬 처리>>>>>
					int tmp=0;
					int max=0;
					for (int a=0; a < 7; a++){
						for (int b=a+1; b < 8; b++){
							if (m_iTemp[b] < m_iTemp[a]){
								tmp = m_iTemp[b];
								m_iTemp[b] = m_iTemp[a];
								m_iTemp[a] = tmp;
							}
						}
					}
					max = m_iTemp[1]; //kds201001 XIS-5000
					a2 = 200-a4;
					a3 = int((max*1.5)+((a2/10)*5));
 					if(i>g_Margin2+40){		// 20251020 동원 커스텀 a3 조건 삭제
						a3 = a3-30;
					}
					//<<<<<<<<<<<<<<<<
					if(a3>=g_Condition4 && g_Condition4!=0)
					{
						m_BadImg[i][j]=4;
						m_BadImg_Save[I_Count][i][j]=4;
						result=false;
					}
				}

				if(a3>g_BackupCond4[g_BackupRear])
				{  //검사 최고값
					g_BackupCond4[g_BackupRear] = a3;
				}

			}
		}
	}


	if(g_Condition4==0)
		return true;

	if(!result)
	{
		if(b_TestMode) //20140821
			m_CountCond4++;
		return false;
	}
	else
		return true;

}

bool CIntellisenseView::Detect_Cond5(int I_Count)  
{
	if(g_Condition5==0)
		return true;

	int i, j;
	int a1,a2,a3,a4,a5,a9=0;
	bool result=true;
	for(i= g_Margin;i<g_Height-g_Margin;i++)
	{
		//for(j=g_Margin;j<g_Width-g_Margin;j++)
		for(j=g_Margin;j<g_Width-g_Margin;j++) 
	//	for(j=iv_11;j<iv_22;j++) 
		{
			if(g_Img_Bufx2[I_Count][i][j]<150 && g_Img_Bufx2[I_Count][i-2][j]<220 && g_Img_Bufx2[I_Count][i+2][j]<220 )
			{ 
				a3=0;

				a4 = int(g_Img_Bufx2[I_Count][i-1][j] + g_Img_Bufx2[I_Count][i][j] + g_Img_Bufx2[I_Count][i+1][j] + g_Img_Bufx2[I_Count][i][j-1] +g_Img_Bufx2[I_Count][i][j+1])/5;
				//a4 = int(g_Img_Bufx2[I_Count][i - 1][j] + g_Img_Bufx2[I_Count][i][j] + g_Img_Bufx2[I_Count][i + 1][j] / 3);

				a5 = (150-a4)/2;

				if(g_Img_Bufx2[I_Count][i][j-4] < 150){
					a1 = g_Img_Bufx2[I_Count][i][j-4] - a4;
				}else{
					a1 = 100 - a4;
				}


				if(g_Img_Bufx2[I_Count][i][j+4] < 150){
					a2 = g_Img_Bufx2[I_Count][i][j+4] - a4;
				}else{
					a2 = 100 - a4;
				}

				a3 = a1 + a2 + a5;

				if(a3>=g_Condition5 && g_Condition5!=0)
				{
					m_BadImg[i][j]=5;
					m_BadImg_Save[I_Count][i][j] = 5; //20151224 3D 배열화
					result=false;
				}

				if(a3>g_BackupCond5[g_BackupRear])
				{  //검사 최고값
					g_BackupCond5[g_BackupRear] = a3;
				}
			}	
		}
	}

	if(g_Condition5==0)
		return true;

	if(!result)
	{
		if(b_TestMode) //20140821
			m_CountCond5++;
		return false;
	}
	else
		return true;

}

//kds20110105 - Labeling
bool CIntellisenseView::Detect_Cond6(int I_Count)   //kds20101223
{
	if(g_Condition6==0)
		return true;
	//
	int i, j;
	int a1,a2,a3,a4,a5,a6,a7,a8,a9=0;
	bool result=true;
	for(i=g_Margin+2;i<g_Height-12;i++)
	{
		for(j=g_Margin+2;j<g_Width-g_Margin+2;j++)
	//	for(j=iv_11;j<iv_22;j++) 
		{
			if( g_Img_Bufx2[I_Count][i][j]<150 && g_Img_Bufx2[I_Count][i-3][j]<150 && g_Img_Bufx2[I_Count][i+1][j]<150 )
			{ 
				a3=0;

				a4 = int(g_Img_Bufx2[I_Count][i-10][j] + g_Img_Bufx2[I_Count][i][j] + g_Img_Bufx2[I_Count][i+10][j] + g_Img_Bufx2[I_Count][i][j-10] +g_Img_Bufx2[I_Count][i][j+10])/5;
				//a5 = (150-a4)/2;
				a5=0;

				if(g_Img_Bufx2[I_Count][i][j-10] < 150){
					a1 = g_Img_Bufx2[I_Count][i][j-10] - a4;
				}else{
					a1 = 100 - a4;
				}


				if(g_Img_Bufx2[I_Count][i][j+10] < 150){
					a2 = g_Img_Bufx2[I_Count][i][j+10] - a4;
				}else{
					a2 = 100 - a4;
				}
				
				if(g_Img_Bufx2[I_Count][i+10][j] < 150){
					a6 = g_Img_Bufx2[I_Count][i+10][j] - a4;
				}else{
					a6 = 100 - a4;
				}
				
				if(g_Img_Bufx2[I_Count][i-10][j] < 150){
					a7 = g_Img_Bufx2[I_Count][i-10][j] - a4;
				}else{
					a7 = 100 - a4;
				}

				a3 = a1 + a2 + a5 + a6 + a7;

				if(a3>=g_Condition6 && g_Condition6!=0)
				{
					m_BadImg[i][j]=6;
					m_BadImg_Save[I_Count][i][j] = 6; //20151224 3D 배열화
					result=false;
				}

		
				if(a3>g_BackupCond6[g_BackupRear])
				{  //검사 최고값
					g_BackupCond6[g_BackupRear] = a3;
				}

			}
		}
	}
	
	if(g_Condition6==0)
		return true;

	if(!result)
	{
		if(b_TestMode)
			m_CountCond6++;
		return false;
	}
	else
		return true;

}

void CIntellisenseView::DrawAnimation()
{
	static int nAni=0;
	CRect rect;
	
	CBitmap bmp, bmp1;
	bmp.LoadBitmapW(IDB_XRAYANI);
	bmp1.LoadBitmapW(IDB_XRAYANI_1);
	
	CClientDC dc(this);
	CDC bmDC;
	bmDC.CreateCompatibleDC(&dc);
	
	if(b_AniState)
		bmDC.SelectObject(&bmp);
	else 
		bmDC.SelectObject(&bmp1);

	m_PicXrayAni.GetWindowRect(&rect);
	ScreenToClient(&rect);
	if(b_AniState)
	{
		if(nAni==1)
			DrawAnimationClear();
		dc.BitBlt(rect.left, rect.top, nAni*rect.Width()/6, rect.Height(), &bmDC, rect.Width()-(rect.Width()*nAni/6), 0, SRCCOPY);
		nAni++;
		if(nAni>6)
		{
			nAni=1;
			b_AniState=false;
		}
	}
	else
	{
		if(nAni==1)
			DrawAnimationClear();
		dc.BitBlt(rect.right-(rect.Width()*nAni/6), rect.top, rect.Width()*nAni/6, rect.Height(), &bmDC, 0, 0, SRCCOPY);
		nAni++;
		if(nAni>6)
		{
			nAni=1;
			b_AniState=true;
		}
	}
	bmp.DeleteObject();
	bmp1.DeleteObject();
	bmDC.DeleteDC();
}

void CIntellisenseView::DrawAnimationClear()
{
	CRect rect;
	BITMAP bi;
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_XRAYANI_CLEAR);
	bmp.GetBitmap(&bi);
	CClientDC dc(this);
	CDC bmDC;
	bmDC.CreateCompatibleDC(&dc);
	bmDC.SelectObject(&bmp);
	m_PicXrayAni.GetWindowRect(&rect);
	ScreenToClient(&rect);
	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),&bmDC,0,0,bi.bmWidth,bi.bmHeight, SRCCOPY);
	bmp.DeleteObject();
	bmDC.DeleteDC();
}

void CIntellisenseView::DrawResult()
{
	CRect rect;
	// 0: Ready 1: OK 2: NG 3: Soup NG
	CBitmap bmp;
	switch(m_Result_mode)
	{
		case 1:
			bmp.LoadBitmapW(IDB_RESULT_OK);
		break;
		case 2:
			bmp.LoadBitmapW(IDB_RESULT_NG);
		break;
		case 3:
			if(g_KindofOmit == 1)
			{
				if(g_NationalCode == 0) //한글
				{
					bmp.LoadBitmapW(IDB_RESULT_SOUP);
				}
				else if(g_NationalCode == 1) //영문
				{
					bmp.LoadBitmapW(IDB_RESULT_SOUP_ENG);
				}
				else if(g_NationalCode == 2) //중문
				{
					bmp.LoadBitmapW(IDB_RESULT_SOUP_CHI);
				}
				else if(g_NationalCode == 3) //스페인어
				{
					bmp.LoadBitmapW(IDB_RESULT_SOUP_SPN);
				}
				
			}
			else if(g_KindofOmit == 2)
			{
				if(g_NationalCode == 0)
				{
					bmp.LoadBitmapW(IDB_RESULT_SOUP2);
				}
				else if(g_NationalCode == 1) //영문
				{
					bmp.LoadBitmapW(IDB_RESULT_SOUP2_ENG);
				}
				else if(g_NationalCode == 2) //중문
				{
					bmp.LoadBitmapW(IDB_RESULT_SOUP2_CHI);
				}
				else if(g_NationalCode == 3) 
				{
					bmp.LoadBitmapW(IDB_RESULT_SOUP2_SPN);
				}
			}
			else if(g_KindofOmit == 3)
			{
				if(g_NationalCode == 0)
				{
					bmp.LoadBitmapW(IDB_RESULT_SOUP3);
				}
				else if(g_NationalCode == 1) //영문
				{
					bmp.LoadBitmapW(IDB_RESULT_SOUP3_ENG);
				}
				else if(g_NationalCode == 2) //중문
				{
					bmp.LoadBitmapW(IDB_RESULT_SOUP3_CHI);
				}
				else if(g_NationalCode == 3) 
				{
					bmp.LoadBitmapW(IDB_RESULT_SOUP3_SPN);
				}
			}
			
		break;
		default:
			bmp.LoadBitmapW(IDB_RESULT_READY);
		break;
	}

	BITMAP  bi;
	bmp.GetBitmap(&bi);
	CClientDC dc(this);
	CDC bmDC;
	bmDC.CreateCompatibleDC(&dc);
	bmDC.SelectObject(&bmp);
	m_PicResult.GetWindowRect(&rect);
	ScreenToClient(&rect);
	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),&bmDC,0,0,bi.bmWidth,bi.bmHeight, SRCCOPY);
	bmp.DeleteObject();
	bmDC.DeleteDC();	
}

void CIntellisenseView::DrawResultProgress()
{
	CRect rect;
	CBitmap bmp, bmp1;
	
	switch(m_Result_mode)
	{
		case 1:
		case 2:
			ClearResultProgress();
			bmp.LoadBitmapW(IDB_PROGRESS_OK);
			bmp1.LoadBitmapW(IDB_PROGRESS_OK_1);	
		break;
		case 3:
			ClearResultProgress();
			bmp.LoadBitmapW(IDB_PROGRESS_NG);
			bmp1.LoadBitmapW(IDB_PROGRESS_NG_1);	
		break;
		default:
			bmp.LoadBitmapW(IDB_PROGRESS);
			bmp1.LoadBitmapW(IDB_PROGRESS_1);	
		break;
	}

	CClientDC dc(this);
	CDC bmDC;
	bmDC.CreateCompatibleDC(&dc);

	if(m_OmitAve<100)
		bmDC.SelectObject(&bmp);
	else
		bmDC.SelectObject(&bmp1);

	
	int tempAve=0, tempMin=0, tempMax=200;

	if(m_OmitAve>200)
		m_OmitAve=200;

	if(g_OmitCondition3!=0)
	{
		if(m_OmitAve<100)
		{
			m_PicProgress.GetWindowRect(&rect);
			ScreenToClient(&rect);
	
			dc.BitBlt(rect.left+(m_OmitAve*rect.Width())/100, rect.top, rect.Width()-(m_OmitAve*rect.Width())/100, rect.Height(), &bmDC, (m_OmitAve*rect.Width())/100,0, SRCCOPY);
		}
		else if(m_OmitAve>=100)
		{
			m_PicProgress2.GetWindowRect(&rect);
			ScreenToClient(&rect);
			dc.BitBlt(rect.left, rect.top, ((m_OmitAve-100)*rect.Width())/100, rect.Height(), &bmDC, 0, 0, SRCCOPY);
		
		}
	}

	bmp.DeleteObject();
	bmp1.DeleteObject();
	bmDC.DeleteDC();	
}

void CIntellisenseView::DrawXrayState(void)
{
	CRect rect;
	CBitmap bmp;
	BITMAP  bi;
	if(g_NationalCode==0)
	{
		switch(g_XrayState)
		{
			case 1:
				bmp.LoadBitmapW(IDB_XRAY_PREPARE);
			break;
			case 2:
				bmp.LoadBitmapW(IDB_XRAY_ON);
			break;
			default:
				bmp.LoadBitmapW(IDB_XRAY_OFF);
			break;
		}
	}
	else if(g_NationalCode==1)
	{
		switch(g_XrayState)
		{
			case 1:
				bmp.LoadBitmapW(IDB_XRAY_PREPARE_ENG);
			break;
			case 2:
				bmp.LoadBitmapW(IDB_XRAY_ON_ENG);
			break;
			default:
				bmp.LoadBitmapW(IDB_XRAY_OFF_ENG);
			break;
		}
	}
	else if(g_NationalCode==2)
	{
		switch(g_XrayState)
		{
			case 1:
				bmp.LoadBitmapW(IDB_XRAY_PREPARE_CHI);
			break;
			case 2:
				bmp.LoadBitmapW(IDB_XRAY_ON_CHI);
			break;
			default:
				bmp.LoadBitmapW(IDB_XRAY_OFF_CHI);
			break;
		}
	}
	else if(g_NationalCode==3)
	{
		switch(g_XrayState)
		{
			case 1:
				bmp.LoadBitmapW(IDB_XRAY_PREPARE_SPN);
			break;
			case 2:
				bmp.LoadBitmapW(IDB_XRAY_ON_SPN);
			break;
			default:
				bmp.LoadBitmapW(IDB_XRAY_OFF_SPN);
			break;
		}
	}
	
	CClientDC dc(this);
	CDC bmDC;
	bmDC.CreateCompatibleDC(&dc);
	bmDC.SelectObject(&bmp);
	m_PicXrayState.GetWindowRect(&rect);
	ScreenToClient(&rect);
	bmp.GetBitmap(&bi);
	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),&bmDC,0,0,bi.bmWidth,bi.bmHeight, SRCCOPY);
	bmp.DeleteObject();
	bmDC.DeleteDC();
}

void CIntellisenseView::InitArray()
{
	int i, j;	
//	g_Img_Buf = new unsigned char *[g_Height];

	g_Img_Buf2 = new unsigned char *[g_Height];
	m_Img_Buf = new unsigned char *[g_Height];

	m_BadImg = new unsigned int *[g_Height];
	m_BadImg6 = new unsigned int *[g_Height];
	g_Buffer_Label = new unsigned char *[g_Height];

	temp_Buf = new unsigned int *[g_Width];
	center = new unsigned int *[g_Width];

	stackx = new short [g_Height*g_Width];
	stacky = new short [g_Height*g_Width];
	m_Buffer_2 = new unsigned char [g_Height*g_Width];
	m_Buffer_4 = new unsigned char [g_Height*g_Width];
	m_Buffer_3 = new unsigned char [g_Height*g_Height];
	g_Img_Buf_AI = new unsigned char[g_Height * g_Width];

	g_Img_Buf3D = new unsigned char **[50];
	g_Img_Bufx2 = new unsigned char **[50];
	g_Img_Bufx2_o = new unsigned char **[50];

	g_Img_temp = new unsigned char *[g_Height];
	m_BadImg_Save = new unsigned int **[50];
	g_Img_Buf = new unsigned char **[50];
	m_Buffer_1 = new unsigned char **[50];
	g_Buffer_Resize = new unsigned char **[50];

	//g_Buffer_Resize = new unsigned char *[g_Height];

	g_Savecount = new int [50];
	g_Arry_Save = new int [50];
	g_Savekind = new int [50];

	for(i=0; i< 50; i++)
	{
		g_Img_Buf3D[i] = new unsigned char *[g_Height];
		g_Img_Bufx2[i] = new unsigned char *[g_Height];
		g_Img_Bufx2_o[i] = new unsigned char *[g_Height];
		m_BadImg_Save[i] = new unsigned int *[g_Height];
		g_Img_Buf[i] = new unsigned char *[g_Height];
		m_Buffer_1[i]  = new unsigned char *[g_Height];
		g_Buffer_Resize[i] = new unsigned char *[g_Height];

		g_Savecount[i] = 0;
		g_Arry_Save[i] = 0;
		g_Savekind[i] = 0;
		g_STime[i] = "";

	}

	for(i=0; i<50; i++)
	{
		for(int j=0; j<g_Height; j++)
		{
			g_Img_Buf3D[i][j] = new unsigned char [g_Width];
			g_Img_Bufx2[i][j] = new unsigned char [g_Width];
			g_Img_Bufx2_o[i][j] = new unsigned char [g_Width];
			m_BadImg_Save[i][j] = new unsigned int [g_Width];
			g_Img_Buf[i][j] = new unsigned char [g_Width];
			m_Buffer_1[i][j] = new unsigned char [g_Width];
			g_Buffer_Resize[i][j] = new unsigned char [g_Width];

		}
	}

	for(i=0; i<50; i++)
	{
		for(int j=0; j<g_Height; j++)
		{
			for(int q=0; q<g_Width; q++)
			{
				g_Img_Buf3D[i][j][q] = 255;
				g_Img_Bufx2[i][j][q] = 255;
				g_Img_Bufx2_o[i][j][q] = 255;
				m_BadImg_Save[i][j][q] = 0;
				g_Img_Buf[i][j][q] = 255;
				m_Buffer_1[i][j][q] = 255;
				g_Buffer_Resize[i][j][q] = 200;
			}
		}
	}

	for(i=0;i<g_Height;i++)
	{
	//	g_Img_Buf[i] = new unsigned char [g_Width];
		g_Img_Buf2[i] = new unsigned char [g_Width];
		m_Img_Buf[i] = new unsigned char [g_Width];
//		m_Buffer_1[i] = new unsigned char [g_Width];
		m_BadImg[i] = new unsigned int [g_Width];
		m_BadImg6[i] = new unsigned int [g_Width];
		g_Buffer_Label[i] = new unsigned char [g_Width];
		g_Img_temp[i] = new unsigned char [g_Width];

	//	g_Buffer_Resize[i] = new unsigned char [g_Width];

	}

	for(i=0;i<g_Width;i++)
	{
		temp_Buf[i] = new unsigned int [g_Height];
		center[i] = new unsigned int [g_Height];
	}
	
	int k=0;
	for(i=0;i<g_Height;i++)
	{
		for(j=0;j<g_Width;j++)
		{
			
			g_Img_Buf2[i][j] = 255;
			m_Img_Buf[i][j] = 255;

			m_BadImg[i][j] = 0;
			m_BadImg6[i][j] =0;
			temp_Buf[j][i] = 255;
			center[j][i] = 255;

			stackx[k] = 0;
			stacky[k] = 0;
			m_Buffer_2[k]= 255;
			m_Buffer_4[k]= 255;
			g_Img_Buf_AI[k] = 255;
			k++;

			g_Img_temp[i][j] = 255;
		}
	}

	CheckDiskSpaceDE();
}

void CIntellisenseView::CheckDiskSpaceDE()
{
	CString strBuf, strBuf_1, strBuf_2;
	strBuf.Format(_T("D:\\"));
	ULARGE_INTEGER Fbatc, lnTotBytes, lnAvailable;
	double FreeBytes = 0, TotalBytes = 0, fUsed = 0 ;

	if(::GetDiskFreeSpaceEx(LPCTSTR(strBuf), &Fbatc, &lnTotBytes,&lnAvailable))
	{
		TotalBytes =lnTotBytes.LowPart+(double)lnTotBytes.HighPart*(double)4294967296;
		strBuf.Format(_T("%4.1fGB"), TotalBytes/1024/1024/1024 );
		FreeBytes = lnAvailable.LowPart+(double)lnAvailable.HighPart*(double)4294967296;
		if(FreeBytes*100/TotalBytes<10)
		{
			strBuf_1.Format(g_MessageArr[143]);//_T("D드라이브 하드디스크의 용량이 10%%，이하로 남았습니다. (E5003)\n\n"));
			CMessageBoxDlg dlg;
			dlg.m_Message=strBuf_1;
			dlg.DoModal();
		}
	}

	strBuf_2.Format(_T("E:\\"));
	ULARGE_INTEGER Fbatc2, lnTotBytes2, lnAvailable2;
	double FreeBytes2 = 0, TotalBytes2 = 0, fUsed2 = 0 ;
	UINT Disktype;
	Disktype = GetDriveType(strBuf_2);
	if(::GetDiskFreeSpaceEx(LPCTSTR(strBuf_2), &Fbatc2, &lnTotBytes2,&lnAvailable2))
	{
		TotalBytes2 =lnTotBytes2.LowPart+(double)lnTotBytes2.HighPart*(double)4294967296;
		strBuf_2.Format(_T("%4.1fGB"), TotalBytes2/1024/1024/1024);
		FreeBytes2 = lnAvailable2.LowPart+(double)lnAvailable2.HighPart*(double)4294967296;
		if(FreeBytes2*100/TotalBytes2<10 && Disktype == DRIVE_FIXED) 
		{
			strBuf_1.Format(g_MessageArr[144]);//_T("E드라이브 하드디스크의 용량이 10%%，이하로 남았습니다. (E5003)\n\n"));
			CMessageBoxDlg dlg;
			dlg.m_Message=strBuf_1;
			dlg.DoModal();
		}
	}
}

void CIntellisenseView::OnCountresetbt(void)
{
	m_Result_mode=0;
	m_Count_Total=0;
	m_Count_OK=0;
	m_Count_NG=0;	
	m_Count_Soup=0;

	m_CountCond1=0;
	m_CountCond2=0;
	m_CountCond3=0;
	m_CountCond4=0;
	m_CountCond5=0;
	m_CountCond6=0;

	SetDlgItemInt(IDC_CONDITION1_COUNT, m_CountCond1, true);
	SetDlgItemInt(IDC_CONDITION2_COUNT, m_CountCond2, true);
	SetDlgItemInt(IDC_CONDITION3_COUNT, m_CountCond3, true);
	SetDlgItemInt(IDC_CONDITION4_COUNT, m_CountCond4, true);
	SetDlgItemInt(IDC_CONDITION5_COUNT, m_CountCond5, true);
	SetDlgItemInt(IDC_CONDITION6_COUNT, m_CountCond6, true);
}

void CIntellisenseView::OnMenuStart(void)
{
	//OpenLogFile();

	//WriteLog(_T("scan_start"),1);

	

	for(int i=0; i<11; i++)
	{
		::CopyFile(g_StrDirPath[i], g_StrDirPath_copy[i], false);
	}

	if(g_Model_Select == 0)
	{
		if(g_RejectType == 0)
		{	
			/*
			m_ComiDaq1.SetDeviceId(0xB501);
			m_ComiDaq1.SetDevInstance(0);
			m_ComiDaq1.GnDeviceLoad();
			if(m_ComiDaq1.GnGetErrorCode() < 0)
			{
				g_Count_Flag = false;
				g_bComiDaq1 = false;
				CMessageBoxDlg dlg;
				dlg.m_Message= _T("카운터 보드와 연결되지 않았습니다! 리젝트 동작에 문제가 있습니다.");
				dlg.DoModal();

			}
			if(g_bComiDaq1)
			{
				m_ComiDaq1.EncConfig(0,0,0);
				Sleepms(10);
				m_ComiDaq1.EncReset(0);
				Sleepms(10);
				m_ComiDaq1.EncReset(0);
			}*/
		}
	}
	else if(g_Model_Select = 1)  //20190327
		g_bComiDaq1 = false;

	m_InternalCount = 0; //20190327
	
	if (g_SelDetector == 0)
	{
		if (m_DTImage.GetIsOpened())
		{
			b_IsNormalization = 1;
			m_FrameCount = 0;
			m_DTImage.Grab(0);
		}
	}
	else
	{
		 //시뮬레이션 할때 주석처리
		InitOperate();

		// 20240214 CB01 Detecor
		if (_xacquisition.GetIsOpen())
		{
			std::wcout << _T("xacquisition GetIsOpen") << std::endl;

			if (!_xacquisition.GetIsGrabbing())
			{
				std::wcout << _T("xacquisition GetIsGrabbing Not") << std::endl;

				b_IsNormalization = 1;
				_frame_counter = 0;
				_line_lost_num = 0;

				m_FrameCount = 0;
				OnEventMsg(53, 0);
				bool bGrab = _xacquisition.Grab(0);
				if (bGrab)
					std::wcout << _T("Grab Success") << std::endl;
				else
					std::wcout << _T("Grab Fail") << std::endl;
			}
			else
				std::wcout << _T("xacquisition GetIsGrabbing") << std::endl;
		}
		else
		{
			std::wcout << _T("xacquisition Not open") << std::endl;
			PostMessageToCtrlView(WM_EYH_STOP, 30, 0);
			return;
		}
	}
	

	m_Menu_ExitBt.ShowWindow(false);

	if(g_RejectType == 0)
	{
		g_Count_Flag = true;
		AfxBeginThread(ThreadRejectnew, (LPVOID)this);
	}

	b_ThreadAlarm = true;
	AfxBeginThread(ThreadAlarm, (LPVOID)this);

	b_StopPaint=false;
	b_ThreadPaint = true;
	
	AfxBeginThread(ThreadPaint, (LPVOID)this);
	b_ThreadReject = true;
	AfxBeginThread(ThreadReject, (LPVOID)this);

	//b_EventReconi = 0;
	//b_ThreadRecog = true;
	//AfxBeginThread(ThreadRecog, (LPVOID)this);



	if(b_UseSmartView)
		WriteMachineStatus(_T("0"));

	g_iRejectCounter = 0;

	g_SendKind = 1; //20191202

	SetTimer(200, 400, NULL);

	//20200514
	b_CheckOverwrite=false;

}


void CIntellisenseView::OnMenuReStart(void)
{
	if(m_DTImage.GetIsOpened())
	{
		b_IsNormalization=0;
		m_DTImage.Grab(NSE_TRIGER_HEIGHT); 
		Sleep(100);
		//m_DTImage.Stop();
	} 
	Sleep(100);
	m_DTImage.Close();
	m_Detector.Close();

	if(m_pImageObject) 
	{
		delete m_pImageObject;
		m_pImageObject = NULL;
	}

	Sleep(100);

	m_BytesPerPixel = 2;
	if (m_Detector.GetIsOpened())
		return;

	m_Detector.PutChannelType(DT_USB);
	m_Detector.PutCmdTimeOut(3000);

	m_DTImage.PutChannelType(DT_USB);
	m_DTImage.PutBytesPerPixel(m_BytesPerPixel);
	m_DTImage.PutStreamPixelPerBytes(2);
	m_DTImage.PutImgWidth(NSE_DETECTOR_WIDTH);
	m_DTImage.PutImgHeight(NSE_TRIGER_HEIGHT);
	m_DTImage.PutExFrameTrigger(0);
	m_DTImage.PutTimeOut(3000);

/*	m_Detector.SetChannelType(DT_USB);
	m_DTImage.SetChannelType(DT_USB);
	m_DTImage.SetExFrameTrigger(0); 
	m_DTImage.SetImgWidth(NSE_DETECTOR_WIDTH);
	m_DTImage.SetImgHeight(NSE_TRIGER_HEIGHT);
	m_DTImage.SetStreamPixelPerBytes(2);
	m_DTImage.SetBytesPerPixel(2);
	m_DTImage.SetDetectorObject(m_Detector.GetObjectHandle())*/;	

	if(m_Detector.Open())
	{ 
		m_DTImage.PutDetectorObject(&m_Detector);
		m_Commander.PutDetectorObject(&m_Detector);

		m_Commander.GetPixelNumber(&m_ImgWidth);

		m_DTImage.PutImgWidth(m_ImgWidth);
		m_pImgBuf = new BYTE[m_ImgSize];

		if(m_DTImage.Open())
		{
			//m_pImageObject = new CImageObject(m_DTImage.GetImageObject());	
			m_pImageObject = m_DTImage.GetImageObject();
			g_bDetector=true;
		}
		else
		{
			g_bDetector=false;
			OnWriteLog(100);
			CMessageBoxDlg dlg;
			dlg.m_Message=g_MessageArr[147]+_T("(Code : 11)");//"Failed to reconnect detector. Please restart the inspection program. (Code : 11)";
			dlg.DoModal();
			m_Detector.Close();
		}
	}
	else
	{
		//디텍터 연결안되었을시 종료시 오류를 없애기 위해 
		m_pImageObject = NULL;
		g_bDetector=false;
		OnWriteLog(100);
		CMessageBoxDlg dlg;
		//dlg.m_Message="디텍터 재구동 실패 프로그램을 다시 실행해 주세요(코드 : 22)";
		dlg.m_Message=g_MessageArr[147]+_T("(Code : 22)");//"Failed to reconnect detector. Please restart the inspection program. (Code : 22)";
		dlg.DoModal();
	}

	if(g_bDetector)
	{
		m_Commander.PutDetectorObject(&m_Detector);
		m_Commander.PutOutputBits(8);
		m_Commander.PutSensitivityLevel(1);
	}
	Sleepms(50);

	if(m_DTImage.GetIsOpened())
	{
		b_IsNormalization=0;
		m_FrameCount = 0;
		m_DTImage.Grab(0);
	}
	else
	{
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[145];//_T("Detector communication error. (Code : 10)");
		dlg.DoModal();
		return;
	}	

	CString str;
	CTime time;
	time=CTime::GetCurrentTime();
	str.Format(_T("%04d-%02d-%02d-%02d%02d%02d - Detector Restarted \r\n"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

	CStdioFile fileString;
	if(fileString.Open(_T("D:\\Detector_ReStart.Log"),
		CStdioFile::modeWrite | CStdioFile::shareDenyNone))
	{
		fileString.SeekToEnd();
		fileString.WriteString(str);
		fileString.Close();
	}
	else
	{
		if(fileString.Open(_T("D:\\Detector_ReStart.Log"),
			CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))	
		{
			fileString.WriteString(str);
			fileString.Close();
		}
	}
}

void CIntellisenseView::OnMenuStop(void)
{
	//WriteLog(_T("End of inspection"),1);
	//CloseLogFile();
	if (g_SelDetector == 0)
	{
		if (m_DTImage.GetIsOpened())
		{
			b_IsNormalization = 0;
			m_DTImage.Grab(NSE_TRIGER_HEIGHT);
			Sleep(100);
			//	m_DTImage.Stop();
		}
	}
	else
	{
		b_IsNormalization = 0;
		/*
		_xacquisition.Grab(NSE_TRIGER_HEIGHT);
		
		Sleep(100);*/
	/*	char s;
		_xcommand.SendAscCmd("[EL,W,0,0]", &s);
		Sleep(200);
		_xcommand.SendAscCmd("[OM,W,0,0]", &s);
		Sleep(200);
		*/
		if (_xacquisition.GetIsGrabbing())
		{
			_xacquisition.Stop();
		}
		
	}
	//b_ThreadRecog = false;

	g_Count_Flag = false;
	g_bSavingCondition=false;
	KillTimer(200);
	DrawAnimationClear();

	Sleepms(200);
	b_ThreadAlarm = false;
	Sleepms(60);
	g_EventAlarm.PulseEvent();	
	Sleepms(60);
	
	b_StopPaint=true;
	Sleepms(60);
	b_ThreadPaint = false;	
	g_EventPaint.PulseEvent();
	Sleepms(60);
	b_ThreadReject = false;
	g_EventReject.PulseEvent();	
	Sleepms(60);
	OnWriteLog(1);
	
	m_Menu_ExitBt.ShowWindow(true);

	m_ReportOk=0;
	m_ReportNg=0;
	m_ReportOmit=0;
	m_ReportTotal=0;
	

	g_RejectFront=0;
	g_RejectRear=0;

	if(b_UseSmartView)
		WriteMachineStatus(_T("1"));
	
//	if(g_bComiDaq1)
//		m_ComiDaq1.GnDeviceUnload();

	if(g_Model_Select == 1) //20190327
	{
		if (g_SelDetector == 0) //c5
		{
			//라인트리거 -> 일반 S/W트리거 변경 // 20180612
			char s[REV_BUF_SIZE];
			m_Detector.SendCommandA("[EL,W,0]", s);
			Sleep(200);
			m_Detector.SendCommandA("[SD,W,1]", s);
			Sleep(200);
		}
		else
		{
			_xcommand.SetPara(XPARA_OPE_MODE, 0);
			Sleep(200);
			_xcommand.SetPara(XPARA_LINE_TRIGGER_MODE, 0);
			Sleep(200);
			_xcommand.SetPara(XPARA_EN_LINE_TRIGGER, 1);
			Sleep(200);
		}
	}

		//20191202
	g_SendKind = 2;

}


void CIntellisenseView::OnConditionsetup(void)
{
	int result=0;
	if(!g_bAdmin)
	{
		CAdminDlg login;
		login.m_Mode=1;
		result=login.DoModal();
	}
	else
	{
		result=1;
	}
 
	if(result==1)
	{
		CConditionSetup dlg;
		dlg.DoModal();
		if(g_ChangeNGCond)
		{
 			OnWriteLog(200);
			g_ChangeNGCond=false;
		}
		if(g_ChangeOmitCond[0])
		{
			OnWriteLog(300);
			g_ChangeOmitCond[0]=false;
		}
		if(g_ChangeOmitCond[1])
		{
			OnWriteLog(310);
			g_ChangeOmitCond[1]=false;
		}
	/*	if(g_ChangeOmitCond[2])
		{
			OnWriteLog(320);
			g_ChangeOmitCond[2]=false;
		}*/
		if(g_ChangeMaskCond[0])
		{
			OnWriteLog(350);
			g_ChangeMaskCond[0]=false;
		}
		if(g_ChangeMaskCond[1])
		{
			OnWriteLog(360);
			g_ChangeMaskCond[1]=false;
		}
		if(g_ChangeMaskCond[2])
		{
			OnWriteLog(370);
			g_ChangeMaskCond[2]=false;
		}

		OnDispCondActive();
	}
}

void CIntellisenseView::OnBnClickedMenuExit()
{

	//OpenLogFile();

	//WriteLog(_T("Program exit"),1);

	//CloseLogFile();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(g_XrayState==1 || g_XrayState==2)
	{
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[146];//"검사정지 후 종료하여 주십시요.";
		dlg.DoModal();
		return;
	}


	CExitDlg Exitdlg;
	int result = Exitdlg.DoModal();
	if(result==IDCANCEL)
	{
		return;
	}
	
	SetFileAttributes(_T("D:\\Database\\password.dat"), FILE_ATTRIBUTE_HIDDEN);
	if(b_UseSmartView)
		WriteMachineStatus(_T("2"));

	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();
	pCtrlView->OnMenuExit();
	
	if (g_SelDetector == 0)
	{
		m_DTImage.Close();
		m_Detector.Close();
	}
	else
	{
		OnClose();
	}

	if(BmInfo)
	{
		delete BmInfo;
		BmInfo=NULL;
	}

	if (m_pImgBuf)
		delete[] m_pImgBuf;
	m_pImgBuf = NULL;

	/*if(m_pImageObject) 
	{
		delete m_pImageObject;
		m_pImageObject = NULL;
	}*/

	ReleaseArray(); 
	KillTimer(100);



	if(result==11)
	{
		if(GetOSVersionType() <= 3)
		{
			ExitWindowsEx(EWX_LOGOFF | EWX_POWEROFF | EWX_SHUTDOWN, 0);
			ExitWindowsEx(EWX_FORCE, 0);
		}
		else if(GetNtPrivilege())
		{
			ExitWindowsEx(EWX_FORCE | EWX_POWEROFF, 0);
		}
	}
	else
		AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE,0,0);

}

int CIntellisenseView::GetOSVersionType()
{
	int version = -1;
	OSVERSIONINFOEX osvi;
	BOOL version_ex_flag = 0;

	ZeroMemory(&osvi,sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if(!(version_ex_flag = GetVersionEx((OSVERSIONINFO *)&osvi)))
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if(!GetVersionEx((OSVERSIONINFO *)&osvi))
		{
			return -1;
		}
	}

	switch(osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		if(osvi.dwMajorVersion <= 4)
		{
			version = 4;
		}
		else if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
		{
			version = 5;
		}
		else if(version_ex_flag)
		{
			version = 6;
		}
		break;

	case VER_PLATFORM_WIN32_WINDOWS:
		if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			version = 1;
		}
		else if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			version = 2;
		}
		else if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			version = 3;
		}
		break;
	}
	return version;
}

//20141117
int CIntellisenseView::GetNtPrivilege()
{
	HANDLE h_token;
	TOKEN_PRIVILEGES privilege_info;
	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &h_token))
	{
		return 0;
	}

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privilege_info.Privileges[0].Luid);
	privilege_info.PrivilegeCount = 1;
	privilege_info.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(h_token, FALSE, &privilege_info, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if(GetLastError() != ERROR_SUCCESS)
	{
		return 0;
	}
	return 1;
}

void CIntellisenseView::MakeDirectory()
{
	CFileFind FileCheck;
	CString m_Directory_0, m_Directory_1, m_Directory_2, m_Directory_3;
	m_Directory_0.Format(_T("D:\\nse-temp"));
	m_Directory_1.Format(_T("D:\\nse-temp\\image"));
	m_Directory_2.Format(_T("D:\\Database\\backup"));
	m_Directory_3.Format(_T("D:\\Database\\image"));
	if(FileCheck.FindFile((LPCTSTR)m_Directory_1, 0))
	{
		MakeSubDirectory();
	}
	else
	{
		CreateDirectory(m_Directory_0, NULL);	
		CreateDirectory(m_Directory_1, NULL);
	}
	if(!FileCheck.FindFile((LPCTSTR)m_Directory_2, 0))
	{
		CreateDirectory(m_Directory_2, NULL);
	}
	if(!FileCheck.FindFile((LPCTSTR)m_Directory_3, 0))
	{
		CreateDirectory(m_Directory_3, NULL);
	}
}

void CIntellisenseView::MakeSubDirectory()
{
	CFileFind FileCheck;
	CString m_Directory_0, m_Directory_1, m_Directory_2, m_Directory_3, m_Directory_4;

	CTime time;
	time = time.GetCurrentTime();
	m_Directory_0.Format(_T("D:\\nse-temp\\image\\%04d-%02d-%02d"), time.GetYear(), time.GetMonth(),	time.GetDay());
	m_Directory_1.Format(_T("%s\\NG"), m_Directory_0);
	m_Directory_2.Format(_T("%s\\OMIT"), m_Directory_0);
	m_Directory_3.Format(_T("%s\\NG\\Log"), m_Directory_0);
	//20160229
	m_Directory_4.Format(_T("%s\\OMIT\\Log"), m_Directory_0);

	if(!(FileCheck.FindFile((LPCTSTR)m_Directory_1, 0) && FileCheck.FindFile((LPCTSTR)m_Directory_2, 0)))
	{
		if(!FileCheck.FindFile((LPCTSTR)m_Directory_0, 0))
		{
			CreateDirectory(m_Directory_0, NULL);		
		}	
		CreateDirectory(m_Directory_1, NULL);		
		CreateDirectory(m_Directory_2, NULL);			
	}
	if(!FileCheck.FindFile((LPCTSTR)m_Directory_3, 0))
	{
		CreateDirectory(m_Directory_3, NULL);	
	}
	//20160229
	if(!FileCheck.FindFile((LPCTSTR)m_Directory_4, 0))
	{
		CreateDirectory(m_Directory_4, NULL);	
	}

	CIntellisenseDoc* pDoc= GetDocument();
	pDoc->m_SaveFolderName = m_Directory_0;
}

void CIntellisenseView::MakeExternalDirectory()
{
	CFileFind FileCheck;
	CString m_Directory, m_Directory_0, m_Directory_1, m_Directory_2, m_Directory_3, m_Directory_4;
	CString m_Directory_5, m_Directory_6, m_Directory_7, m_Directory_8, m_Directory_9, m_Directory_10;

	CTime time;
	time = time.GetCurrentTime();
	m_Directory.Format(_T("E:\\image"));
	m_Directory_0.Format(_T("%s"), m_Directory);
	m_Directory_1.Format(_T("%s\\%04d-%02d-%02d"), m_Directory, time.GetYear(), time.GetMonth(), time.GetDay());
	m_Directory_2.Format(_T("%s\\OK"), m_Directory_1);
	m_Directory_3.Format(_T("%s\\NG"), m_Directory_1);
	m_Directory_4.Format(_T("%s\\OMIT"), m_Directory_1);
	m_Directory_5.Format(_T("%s\\%02d"), m_Directory_2, time.GetHour());
	m_Directory_6.Format(_T("%s\\%02d"), m_Directory_3, time.GetHour());
	m_Directory_7.Format(_T("%s\\%02d"), m_Directory_4, time.GetHour());
	m_Directory_8.Format(_T("%s\\%d0"), m_Directory_5, time.GetMinute()/10);
	m_Directory_9.Format(_T("%s\\%d0"), m_Directory_6, time.GetMinute()/10);
	m_Directory_10.Format(_T("%s\\%d0"), m_Directory_7, time.GetMinute()/10);
	
	if(!(FileCheck.FindFile((LPCTSTR)m_Directory_8, 0) && FileCheck.FindFile((LPCTSTR)m_Directory_9, 0) && FileCheck.FindFile((LPCTSTR)m_Directory_10, 0)))
	{
		if(!(FileCheck.FindFile((LPCTSTR)m_Directory_5, 0) && FileCheck.FindFile((LPCTSTR)m_Directory_6, 0) && FileCheck.FindFile((LPCTSTR)m_Directory_7, 0)))
		{
			if(!(FileCheck.FindFile((LPCTSTR)m_Directory_2, 0) && FileCheck.FindFile((LPCTSTR)m_Directory_3, 0) && FileCheck.FindFile((LPCTSTR)m_Directory_4, 0)))
			{
				if(!FileCheck.FindFile((LPCTSTR)m_Directory_1, 0))
				{
					if(!FileCheck.FindFile((LPCTSTR)m_Directory_0, 0))
					{
						CreateDirectory(m_Directory_0, NULL);	
					}
					CreateDirectory(m_Directory_1, NULL);	
				}
				CreateDirectory(m_Directory_2, NULL);	
				CreateDirectory(m_Directory_3, NULL);		
				CreateDirectory(m_Directory_4, NULL);	
			}
			CreateDirectory(m_Directory_5, NULL);	
			CreateDirectory(m_Directory_6, NULL);	
			CreateDirectory(m_Directory_7, NULL);		
		}
		CreateDirectory(m_Directory_8, NULL);	
		CreateDirectory(m_Directory_9, NULL);		
		CreateDirectory(m_Directory_10, NULL);	
	}

	CIntellisenseDoc* pDoc= GetDocument();
	pDoc->m_SaveExternalName = m_Directory_8;
	pDoc->m_SaveExternalName1 = m_Directory_9;
	pDoc->m_SaveExternalName2 = m_Directory_10;

	g_SaveExternalName = m_Directory_8;
	g_SaveExternalName1 = m_Directory_9;
	g_SaveExternalName2 = m_Directory_10;
}


void CIntellisenseView::Masking(int I_Count)
{

	
	int i=0, i1=0, i2=0;
	int j=0;

	int cd=0, a=0, b=0;

	for(a=0; a<g_Height; a++)//640
		{
			for(b=0; b<g_Width; b++)//960
			{
				m_BadImg[a][b]=0;
				m_BadImg6[a][b] = 0;
				if(NSE_DT_ISBULK==0 || NSE_DT_ISBULK==1)
				{
					if(!NSE_ROTATION_UPDOWN && NSE_ROTATION_LEFTRIGHT) //벌크 및 카톤 이미지 위치 바꾸기 수정
					{
						if(g_Img_Bufx2_o[I_Count][g_Height-1-a][b] > 230)
						{
							m_Img_Buf[a][b] = 230;
						}
						else
						{
							m_Img_Buf[a][b] = g_Img_Bufx2_o[I_Count][g_Height-1-a][b]; //500모델은 g_Height- 지우기
						}
					
						m_Buffer_1[I_Count][a][b] = g_Img_Bufx2_o[I_Count][g_Height-1-a][b];
						
						g_Img_Buf_AI[cd] = g_Img_Bufx2_o[I_Count][g_Height - 1 - a][b]; //20260102 AI 추가
						m_Buffer_2[cd] = g_Img_Bufx2_o[I_Count][g_Height-1-a][b];  //kds1212
						m_Buffer_4[cd] = g_Img_Bufx2_o[I_Count][g_Height-1-a][b];  //kds1212
						g_Img_Bufx2[I_Count][a][b] = g_Img_Bufx2_o[I_Count][g_Height-1-a][b]; //이미지 반전 가능
						cd++;
					}
					else if(NSE_ROTATION_UPDOWN && !NSE_ROTATION_LEFTRIGHT)
					{
						if(g_Img_Bufx2_o[I_Count][a][g_Width-1-b] > 230)
						{
							m_Img_Buf[a][b] = 230;
						}
						else
						{
							m_Img_Buf[a][b] = g_Img_Bufx2_o[I_Count][a][g_Width-1-b]; //500모델은 g_Height- 지우기
						}

						m_Buffer_1[I_Count][a][b] = g_Img_Bufx2_o[I_Count][a][g_Width-1-b];
					

						g_Img_Buf_AI[cd] = g_Img_Bufx2_o[I_Count][a][g_Width - 1 - b];//20260102 AI 추가
						  //kds1212
						m_Buffer_2[cd] = g_Img_Bufx2_o[I_Count][a][g_Width-1-b];  //kds1212
						m_Buffer_4[cd] = g_Img_Bufx2_o[I_Count][a][g_Width-1-b];  //kds1212
						g_Img_Bufx2[I_Count][a][b] = g_Img_Bufx2_o[I_Count][a][g_Width-1-b];
						cd++;
					}
					else if(NSE_ROTATION_UPDOWN && NSE_ROTATION_LEFTRIGHT)
					{
						if(g_Img_Bufx2_o[I_Count][g_Height-1-a][g_Width-1-b] > 230)
						{
							m_Img_Buf[a][b] = 230;
						}
						else
						{
							m_Img_Buf[a][b] = g_Img_Bufx2_o[I_Count][g_Height-1-a][g_Width-1-b]; //500모델은 g_Height- 지우기
						}
						
						m_Buffer_1[I_Count][a][b] = g_Img_Bufx2_o[I_Count][g_Height-1-a][g_Width-1-b];
						
						g_Img_Buf_AI[cd] = g_Img_Bufx2_o[I_Count][g_Height - 1 - a][g_Width - 1 - b];//20260102 AI 추가
					//	m_Buffer_1[a][b] = m_Buffer_1[g_Height-1-a][g_Width-1-b];  //kds1212
						m_Buffer_2[cd] = g_Img_Bufx2_o[I_Count][g_Height-1-a][g_Width-1-b];  //kds1212
						m_Buffer_4[cd] = g_Img_Bufx2_o[I_Count][g_Height-1-a][g_Width-1-b];  //kds1212
						g_Img_Bufx2[I_Count][a][b] = g_Img_Bufx2_o[I_Count][g_Height-1-a][g_Width-1-b];
						cd++;
					}
					else
					{
						if(g_Img_Bufx2_o[I_Count][a][b] > 230)
						{
							m_Img_Buf[a][b] = 230;
						}
						else
						{
							m_Img_Buf[a][b]=g_Img_Bufx2_o[I_Count][a][b]; //500모델은 g_Height- 지우기
						}
					
						m_Buffer_1[I_Count][a][b] = g_Img_Bufx2_o[I_Count][a][b];
						
						g_Img_Buf_AI[cd] = g_Img_Bufx2_o[I_Count][a][b];//20260102 AI 추가
					//	m_Buffer_1[a][b] = m_Buffer_1[a][b];  //kds1212
						m_Buffer_2[cd] = g_Img_Bufx2_o[I_Count][a][b];  //kds121
						m_Buffer_4[cd] = g_Img_Bufx2_o[I_Count][a][b];  //kds121
						g_Img_Bufx2[I_Count][a][b] = g_Img_Bufx2_o[I_Count][a][b];
						cd++;
					}
					
				}
				else
				{
					if(g_Img_Bufx2_o[I_Count][a][b] > 230)
					{
						m_Img_Buf[a][b] = 230;
					}
					else
					{
						m_Img_Buf[a][b] = g_Img_Bufx2_o[I_Count][a][b];
					}
					g_Img_Buf_AI[cd] = g_Img_Bufx2_o[I_Count][a][b];//20260102 AI 추가
					m_Buffer_1[I_Count][a][b] = m_Buffer_1[I_Count][a][b];  //kds1212
					m_Buffer_2[cd] = g_Img_Bufx2_o[I_Count][a][b];  //kds1212
					m_Buffer_4[cd] = g_Img_Bufx2_o[I_Count][a][b];  //kds1212
					g_Img_Bufx2[I_Count][a][b] = g_Img_Bufx2_o[I_Count][a][b];

			

					cd++;
				}
			}
		}
		
		

	
		//// 이미지 강제 보정 20171115
	
	if(g_ViewControl == 0)
	{
	}
	else
	{
		int cde=0;
		for(int x=0; x<g_Height; x++)
		{
			for(int y=0; y<g_Width; y++)
			{	
					float fscale = (float)(g_Img_Bufx2[I_Count][x][y]-0)/(200-0) ;
					float aaa;
					if(g_ViewControl == 3)
						aaa = 0.2;
					else if(g_ViewControl == 2)
						aaa = 0.5;
					else if(g_ViewControl == 1) 
						aaa = 0.8;

					int m_iValue = pow( fscale, aaa ) * 255;
					int temp = 0;
		
					//if(m_iValue < 125)
					//{
					//	temp = (125 - m_iValue) * 0.8;
					//	temp = m_iValue - temp; 
					//}
					//else
					//{
					//	temp = (m_iValue - 125) * 0.8;
					//	temp = m_iValue + temp; 
					//}
					
					//if(m_iValue >= 255)
					//	m_iValue = 255;
					//else if(m_iValue <= 0)
					//	m_iValue = 0;
				//	g_Img_Buf[x][y] = m_iValue;
					m_Buffer_2[cde] = m_iValue;
					m_Buffer_4[cde] = m_iValue; 
					m_Buffer_1[I_Count][x][y] = m_iValue;
					g_Img_Buf_AI[cde] = m_iValue;
					if(g_Accept == 1)  //20171130 영상 보정 검사 적용
						g_Img_Bufx2[I_Count][x][y] = m_iValue;
					cde++;                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
	
			}
		}
	}
	//// 이미지 강제 보정 20171115
	





	g_left = g_Width;
	g_right = g_Margin;
	g_bottom = g_Height;

	if(!g_MaskBoxUse)
	{

		if(g_MaskBoxCondition1!=0)  //상 마스킹
		{
			for(int i=g_Margin;i<g_Width;i++)
			{
				for(int j=g_Height;j>g_Margin;j--)
				{
					int k;
					for(k=j; k>j-g_MaskBoxCondition1; k--)
					{
						if(k>0 && k<g_Height)
						{
							g_Img_Bufx2[I_Count][k][i]=250;
						}
					}

					//m_BadImg[k][i]=7;
					if(g_ShowMaskUse == 0)
					{
						m_BadImg[k][i]=7;
					}
					break;
				}
			}
		}

		if(g_MaskBoxCondition2!=0) //하 마스킹
		{
			for(int i=g_Margin;i<g_Width-g_Margin;i++)
			{
				for(int j=0;j<g_Height;j++)
				{
					int k;
					for(k=j; k<j+g_MaskBoxCondition2+g_Margin2; k++)
					{
						if(k>0 && k<g_Height)
						{
							g_Img_Bufx2[I_Count][k][i]=210;
							
						}
					}
					if(g_ShowMaskUse == 0)
					{
						m_BadImg[k][i]=7;
					}
					//m_BadImg[k][i]=7;
					break;	
				}
			}
		}

		for(int i=g_Margin;i<g_Height-g_Margin;i++)
		{
			for(int j=g_Margin;j<g_Width-g_Margin;j++)
			{
				if(g_Img_Bufx2[I_Count][i][j]<180)
				{
					if(g_left > j)
					{
						g_left = j;
					}
					/*for(int k=j; k<j+g_MaskBoxCondition3; k++)
					{
					/*	if(k>0 && k<g_Width)
						{
							g_Img_Bufx2[I_Count][i][k]=255;
							if(g_ShowMaskUse == 0)
							{
								m_BadImg[i][k]=7;
							}
						}
					}*/
					break;
				}

			}
		}

		for(int i=g_Margin;i<g_Height-g_Margin;i++)
		{
			for(int j=g_Width-g_Margin;j>g_Margin;j--)
			{
				if(g_Img_Bufx2[I_Count][i][j]<180)
				{
					if(g_right < j)
					{
						g_right = j;
					}
						break;
				/*	for(int k=j; k>j-g_MaskBoxCondition3; k--)
					{
						if(k>0 && k<g_Width)
						{
							g_Img_Bufx2[I_Count][i][k]=255;
							if(g_ShowMaskUse == 0)
							{
								m_BadImg[i][k]=7;
							}
						}
					}*/
					break;
				}
			}
		}
	//	}


		for(i=g_Margin;i<g_Width-g_Margin;i++)
		{
			for(j=g_Margin;j<g_Height-g_Margin;j++)
			{

				if(g_Img_Bufx2[I_Count][j][i] < 180)
				{
					if(g_bottom > j)
					{
						g_bottom = j;
					}
				}
			}
		}




		if(g_MaskBoxCondition3!=0)
		{
			for(int i=g_Margin;i<g_Height-g_Margin;i++)
			{
				for(int j=g_Margin;j<g_Width-g_Margin;j++)
				{
					if(g_Img_Bufx2[I_Count][i][j]<195)
					{
						for(int k=j; k<j+g_MaskBoxCondition3; k++)
						{							
							if(k>0 && k<g_Width)
							{
								g_Img_Bufx2[I_Count][i][k]=230;
								if(g_ShowMaskUse == 0)
								{
									m_BadImg[i][k]=7;
								}
							}
						}
						break;
					}

				}
			}

			for(int i=g_Margin;i<g_Height-g_Margin;i++)
			{
				for(int j=g_Width-g_Margin;j>g_Margin;j--)
				{
					if(g_Img_Bufx2[I_Count][i][j]<195)
					{
						for(int k=j; k>j-(g_MaskBoxCondition3+1); k--)
						{
							if(k>0 && k<g_Width)
							{
								g_Img_Bufx2[I_Count][i][k]=230;
								if(g_ShowMaskUse == 0)
								{
									m_BadImg[i][k]=7;
								}
							}
						}
						break;
					}
				}
			}
		}



		for(int q=g_Margin;q<g_Width; q++)
		{
			j=g_Margin2+15;
			m_BadImg[j][q]=9;
		}
	}
}

bool CIntellisenseView::Labelling()
{
	if(g_Countnum==0)
		return true;

	int Countnum=0;
	int CondValue=210;
	int i, j;
	int m, n, top, area, BlobArea[10000];
	int curColor=0;
	short r, c;
	int m_LabelCount = 0;
	int m_OverlapCode = 0;

	for(i=0;i<g_Height;i++)
		for(j=0;j<g_Width;j++)
			g_Buffer_Label[i][j]=0;

	for(i=0;i<g_Height*g_Width;i++)
	{
		stackx[i]=0;
		stacky[i]=0;
	}
	CString temp;
	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{
		for(j=g_Margin;j<g_Width-g_Margin;j++)
		{
			if(g_Buffer_Label[i][j]!=0 || m_Img_Buf[i][j]>CondValue)
				continue;
			r=i; c=j; top=0; area=1;
			curColor++;
			while(1)
			{
GRASS:
				for(m=r-1; m<=r+1; m++)
				{
					for(n=c-1; n<=c+1; n++)
					{
						if(m<0 || m>=g_Height-g_Margin || n<0 || n>g_Width)
							continue;
						if(g_Buffer_Label[m][n]==0 && m_Img_Buf[m][n]<=CondValue)
						{
							if(curColor*5>255)
								g_Buffer_Label[m][n]=50;
							else
								g_Buffer_Label[m][n]=curColor*5;
							
							if(push(stackx, stacky, (short)m, (short)n, &top)==-1)
								continue;
							r=m;
							c=n;
							area++;
							goto GRASS;
						}
					}
				}
				if(pop(stackx, stacky, &r, &c, &top)==-1)
					break;
			}
			
			//CString ff;
			if(g_TalMask_Bright2<area && area>=10)
			{
				Countnum++;
				if(curColor<10000) 
					BlobArea[curColor]=area;
				//ff.Format("Label : %d EA, Area : %d \n", curColor, area);
				//temp += ff;
			}
		}
	}
/*	
	if(g_OmitCondUse || g_OmitCondition3==0)	
	{
		CString ss;
		ss.Format(_T("%d/%d"), Countnum, g_Countnum);
		GetDlgItem(IDC_OMITAVE)->SetWindowText(ss);
	}
	*/
	if(g_Countnum==Countnum)
		return true;
	else
		return false;
}

int CIntellisenseView::pop(short *stackx, short *stacky, short *vx, short *vy, int *top)
{
	if(*top==0)
		return -1;
	*vx=stackx[*top];
	*vy=stacky[*top];
	(*top)--;
	return 1;
}

int CIntellisenseView::push(short *stackx, short *stacky, short vx, short vy, int *top)
{
	if(*top>=g_Height*g_Width)
		return -1;
	(*top)++;
	stackx[*top]=vx;
	stacky[*top]=vy;
	return 1;
}

void CIntellisenseView::ReleaseArray(void)
{
	int i;
	for(i=0;i<g_Height;i++)
	{
		
		delete [] g_Img_Buf2[i];
		delete [] m_Img_Buf[i];

		delete [] m_BadImg[i];
		delete [] m_BadImg6[i];
		delete [] g_Buffer_Label[i];
	}

	for(i=0;i<g_Width;i++)
	{
		delete [] temp_Buf[i];
		delete [] center[i];
	}
	
	for(i=0; i<50; i++)
	{
		for(int j=0; j<g_Height; j++)
		{

			delete [] g_Img_Buf3D[i][j];
			delete [] g_Img_Bufx2[i][j];
			delete [] m_BadImg_Save[i][j];
			delete [] g_Img_Buf[i][j];
			delete [] m_Buffer_1[i][j];
		}
	}

	for(i=0; i<50; i++)
	{
		delete [] g_Img_Buf3D[i];
		delete [] g_Img_Bufx2[i];
		delete [] m_BadImg_Save[i];
		delete [] g_Img_Buf[i];
		delete [] m_Buffer_1[i];
	}


	delete [] m_BadImg_Save;
	delete [] g_Img_temp;
	delete [] g_Buffer_Resize;

	delete [] g_Img_Buf;
	delete [] g_Img_Buf2;
	delete [] m_Img_Buf;

	delete [] g_Buffer_Label;
	delete [] m_Buffer_1;
	delete [] m_BadImg;
	delete [] m_BadImg6;

	delete [] temp_Buf;
	delete [] center;

	delete [] stackx;
	delete [] stacky;
	delete [] m_Buffer_2;
	delete [] m_Buffer_3;
	delete [] m_Buffer_4;

	delete [] g_Img_Buf3D;
	delete [] g_Img_Bufx2;
	delete [] g_Img_Bufx2_o;

	delete [] g_Savecount;
	delete [] g_Arry_Save;
	delete [] g_Savekind;
	delete[] g_Img_Buf_AI;

}


/*UINT ThreadRecog(LPVOID lParam)
{

	CIntellisenseView* pView;
	pView = (CIntellisenseView*)lParam;
	int count;
	while (b_ThreadRecog)
	{
		if (b_EventReconi == 1)
		{
			count = g_3D_Count;
			//	pView->Masking();
			b_EventReconi = 0;
			pView->OnRecognition(count);
		}
		else
			Sleep(1);

	}
	return 0;
}*/





UINT ThreadSave(LPVOID lParam)
{
	DWORD_PTR affinityMask = ((DWORD_PTR)1 << 3);
	SetThreadAffinityMask(GetCurrentThread(), affinityMask);


	//CIntellisenseDoc* pDoc;
	CTime time;
	CString m_SaveFolder2;
	m_SaveFolder2 = "";
	CString g_filen;
	SYSTEMTIME m_Rstm;
	CString m_LogFilename;
	m_LogFilename = "";
	CString m_CondInfo;
	m_CondInfo = "";
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();

	CIntellisenseDoc *pDoc;
	pDoc = (CIntellisenseDoc *)lParam;

	CTime m_Rtime;
	while(1)
	{

		m_Icount++;
		if(m_Icount >= 50)
		{
			m_Icount = 1;
		}
	
		GetSystemTime(&m_Rstm);
		m_Rtime=CTime::GetCurrentTime();
		g_filen.Format(_T("%04d-%02d-%02d-%02d%02d%02d%03d%d"), m_Rtime.GetYear(), m_Rtime.GetMonth(), 
		m_Rtime.GetDay(), m_Rtime.GetHour(), m_Rstm.wMinute, m_Rstm.wSecond, m_Rstm.wMilliseconds,m_Icount);
	
			if(g_Arry_Save[m_Icount] == 1)
			{
				int m_Tcount = 0;
				m_Tcount = g_Savecount[m_Icount];

				if(g_SaveHard)
					g_Arry_Save[m_Icount]=0;  //전체저장이 활성화되면 동작하지 않음.  // 20170628


				time = time.GetCurrentTime();
				m_SaveFolder2.Format(_T("D:\\nse-temp\\image\\%04d-%02d-%02d"), time.GetYear(), time.GetMonth(),time.GetDay());
					
				

				if(g_Savekind[m_Icount] == 2)
				{
			//		m_LogFilename=m_SaveFolder2+_T("\\NG\\Log\\")+g_filen+_T(".dat");
					m_LogFilename=m_SaveFolder2+_T("\\NG\\Log\\")+g_STime[m_Tcount]+_T(".dat");

					m_CondInfo=pView->m_ConditionInfo[m_Tcount];
					CStdioFile fileString;
					CString bad;
		
					if(fileString.Open(m_LogFilename,
						CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
					{
						USES_CONVERSION;
						char* str = W2A(g_ProductName); //기록할 문자열을 유니코드에서 MBCS 문자열로 변환
						fileString.Write(str, strlen(str)); 

						bad.Format(_T("@%s!"), m_CondInfo);
						fileString.WriteString(bad);
					}
		
					int cou=0;
					char buf[15];


					for(int i=g_Margin;i<g_Height-g_Margin;i++)
					{
						for(int j=g_Margin; j<g_Width-g_Margin;j++)
						{
							if((pView->m_BadImg_Save[m_Tcount][i][j]!=0 && pView->m_BadImg_Save[m_Tcount][i][j]<7)|| pView->m_BadImg_Save[m_Tcount][i][j] ==10)
							{
						
								bad="";
								sprintf(buf, "%d.%d.%d+" ,pView->m_BadImg_Save[m_Tcount][i][j], i, j) ;
								bad += buf;	
								cou++;
								fileString.WriteString(bad);
							}
							else if(pView->m_BadImg_Save[m_Tcount][i][j]== 7)
							{
								bad = "";
								sprintf(buf, "%d.%d.%d+", pView->m_BadImg_Save[m_Tcount][i][j], i, j);
								bad += buf;
								cou++;
								fileString.WriteString(bad);
							}
						}
					}
		
		
					bad.Format(_T("#"));
					fileString.WriteString(bad);
					fileString.Close();

				}
				else if(g_Savekind[m_Icount] == 3)
				{
					if(g_Dent_Save == false)  //20151217
					{
						m_LogFilename=m_SaveFolder2+_T("\\OMIT\\Log\\")+g_STime[m_Tcount]+_T("_Type0")+_T(".dat");	
						//m_LogFilename = m_SaveFolder2 + _T("\\OMIT\\Log\\") + g_STime[m_Tcount]+ _T(".dat");
						m_CondInfo=pView->m_ConditionInfo1[m_Tcount];
					}
					else
					{
						m_LogFilename=m_SaveFolder2+_T("\\OMIT\\Log\\")+g_STime[m_Tcount]+_T("_Type1")+_T(".dat");
						m_CondInfo=pView->m_ConditionInfo2[m_Tcount];
					}

			
					CStdioFile fileString;
					CString bad;
					if(fileString.Open(m_LogFilename,
						CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
					{
						USES_CONVERSION;
						char* str = W2A(g_ProductName); //기록할 문자열을 유니코드에서 MBCS 문자열로 변환
						fileString.Write(str, strlen(str)); 

						bad.Format(_T("@%s!"), m_CondInfo);
						fileString.WriteString(bad);
					}

				}
				
				CString m_Fullname;
				if(g_Savekind[m_Icount] == 2)
				{
					m_Fullname=m_SaveFolder2+_T("\\NG\\")+g_STime[m_Tcount] + _T(".jpg");
				}
				else if(g_Savekind[m_Icount] ==3)
				{
					if(g_Dent_Save == false)  //20151217
					{
						m_Fullname=m_SaveFolder2+_T("\\OMIT\\")+g_STime[m_Tcount]+_T("_Type0")+_T(".jpg");	
						//m_Fullname = m_SaveFolder2 + _T("\\OMIT\\") + g_STime[m_Tcount] +  _T(".jpg");
					}
					else
					{
						m_Fullname=m_SaveFolder2+_T("\\OMIT\\")+g_STime[m_Tcount]+_T("_Type1")+_T(".jpg");	
					}
				}
			
				// cximage
			//	CxImage ximg(g_Width,g_Height,8,CXIMAGE_FORMAT_JPG);
			////	ximg.SetJpegQuality(30); 이미지 품질 줄이기 (30이하로 하지 말기)20170628
			//	BYTE* pBaseTo;
			//	pBaseTo = ximg.GetBits();
			//	for(int i=0;i<g_Height;i++)
			//		for(int j=0;j<g_Width;j++)
			//		{
			//			*(pBaseTo+i*g_Width+j) = m_Buffer_1[m_Tcount][i][j];  //kds1212 (m_Buffer)m_Buffer   m_Buffer_1
			//		}
			//	ximg.SetGrayPalette();
			//	ximg.Save(m_Fullname,ximg.GetType());

				cv::Mat mImg(g_Height, g_Width, CV_8UC1);
				uchar* pBaseTo = mImg.data;

				for (int i = 0; i < g_Height; i++)
				{
					for (int j = 0; j < g_Width; j++)
					{
						pBaseTo[i * g_Width + j] = m_Buffer_1[m_Tcount][i][j];
					}
				}

				cv::flip(mImg, mImg, 0);

				std::vector<int> params;
				params.push_back(cv::IMWRITE_JPEG_QUALITY);
				params.push_back(70);   // 0~100, 보통 50~80부터 테스트
				//NSE::Save(mImg, m_Fullname);		// 퀄리티 떨궈야되나??
				NSE::Save(mImg, m_Fullname, params);
				Sleepms(80);
		
			}

			///////////////////////////////////////////// 20170628  전체영상 저장
		//	CIntellisenseDoc* pDoc= GetDocument();
			CString m_Fullname;
			
			if(!g_SaveHard && g_Arry_Save[m_Icount] == 2 || g_Arry_Save[m_Icount] == 1)
			{
				
				switch(g_Savekind[m_Icount])
				{
				case 1:
					m_Fullname=g_SaveExternalName+_T("\\")+g_STime[m_Icount]+_T(".jpg"); //ok
					break;
				case 2:
					m_Fullname=g_SaveExternalName1+_T("\\")+g_STime[m_Icount]+_T(".jpg"); //ng
					break;
				case 3:
					m_Fullname=g_SaveExternalName2+_T("\\")+g_STime[m_Icount]+_T(".jpg");
					break;
				}

				// cximage
				//CxImage ximg(g_Width,g_Height,8,CXIMAGE_FORMAT_JPG);  //이미지 사이즈 반으로 줄임 20170628
	   // 		ximg.SetJpegQuality(50); //이미지 품질 줄이기 (30이하로 하지 말기)20170628
				//BYTE* pBaseTo;
				//pBaseTo = ximg.GetBits();
				//for(int i=0;i<g_Height;i++)
				//	for(int j=0;j<g_Width;j++)
				//	{
				//		*(pBaseTo+i*g_Width+j) = m_Buffer_1[m_Icount][i][j];  //왼쪽 오른쪽 짜르기 (256 픽셀)	
				//	}															
				//	ximg.SetGrayPalette();
				//
				//	ximg.Save(m_Fullname,ximg.GetType());
				//
				//	g_Arry_Save[m_Icount] = 0;

				cv::Mat mImg(g_Height, g_Width, CV_8UC1);
				uchar* pBaseTo = mImg.data;

				for (int i = 0; i < g_Height; i++)
				{
					for (int j = 0; j < g_Width; j++)
					{
						pBaseTo[i * g_Width + j] = m_Buffer_1[m_Icount][i][j];
					}
				}

				cv::flip(mImg, mImg, 0);
				std::vector<int> params;
				params.push_back(cv::IMWRITE_JPEG_QUALITY);
				params.push_back(70);   // 0~100, 보통 50~80부터 테스트
				//NSE::Save(mImg, m_Fullname);		// 퀄리티 떨궈야되나??
				NSE::Save(mImg, m_Fullname, params);
				g_Arry_Save[m_Icount] = 0;
			}
	}
			
		return 0;

}
UINT ThreadPaint(LPVOID lParam)
{
	
	CIntellisenseView *pView;
	pView=(CIntellisenseView *)lParam;

	CRect arect;
	pView->m_GrabImage.GetWindowRect(&arect);

	CDC *pDC = pView->m_GrabImage.GetDC();			
	pDC->SetStretchBltMode(COLORONCOLOR);
	
	while(b_ThreadPaint)
	{
		g_EventPaint.Lock();

		::StretchDIBits(pDC->m_hDC,0,0,arect.Width(),arect.Height(),0, 0, g_Width, g_Height,m_Buffer_2, BmInfo, DIB_RGB_COLORS,SRCCOPY);   
		
		
		
		if(!(pView->b_StopPaint) || m_Interock_State)
		{
			if((pView->m_Result_mode==2 || pView->m_Result_mode==3) && !g_bAutoSetup) 
			{
				int w=arect.right-arect.left;
				int h=arect.bottom-arect.top;
				if(pView->m_Result_mode==2)
				{
					//20160707
					for(int x=0; x<g_Height; x++)
					//for(int x=g_Margin+2; x<g_Height-g_Margin-2; x++)
					{
						for(int y=0; y<g_Width; y++)
						//for(int y=g_Margin; y<g_Width-g_Margin; y++)
						{
							if((pView->m_BadImg[x][y]<=9 &&  pView->m_BadImg[x][y]>=1))
							{
								if(x<=g_Margin+10 || x>=g_Height-g_Margin-10 || y<=g_Margin+10 || y>=g_Width-g_Margin-10)
								{
									switch(pView->m_BadImg[x][y])
									{
										
										case 9:
											pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(228, 238, 43));
										break;
										case 8:
											pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(238, 43, 233));
										break;
										case 6:	
											pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(200, 0, 200));
										break;
										case 5:
											pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(9, 232, 238));
										break;
										case 4:
											pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(255, 100, 0));
										break;
										case 3:
											pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(0, 0, 200));
										break;
										case 2:
											pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(0, 200, 0));
										break;
										case 1:
											pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(255, 200, 0));
										break;
									}
								}
								else
								{
									switch(pView->m_BadImg[x][y] )
									{
										/*case 10:
											pDC->SelectObject(pView->pen10);*/
										break;
										case 9:
											pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(228, 238, 43));
										break;
										case 8:
											pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(238, 43, 233));
										break;
										case 7:
											pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(255, 100, 0));
										break;
										case 6:	
											pDC->SelectObject(pView->pen6);	
										break;
										case 5:
											pDC->SelectObject(pView->pen5);	
										break;
										case 4:
											pDC->SelectObject(pView->pen3);	
										break;
										case 3:
											pDC->SelectObject(pView->pen2);	
										break;
										case 2:
											pDC->SelectObject(pView->pen1);	
										break;
										case 1:
											pDC->SelectObject(pView->pen);	
										break;
									}
									if(pView->m_BadImg[x][y]!=7 && pView->m_BadImg[x][y]!=8 && pView->m_BadImg[x][y]!=9/* && pView->m_BadImg[x][y] != 10*/)
									{
										pDC->MoveTo((w*y/g_Width)-8, (h*(g_Height-x)/g_Height)-8);
										pDC->LineTo((w*y/g_Width)-8, (h*(g_Height-x)/g_Height)+8);
										pDC->LineTo((w*y/g_Width)+8, (h*(g_Height-x)/g_Height)+8);
										pDC->LineTo((w*y/g_Width)+8, (h*(g_Height-x)/g_Height)-8);
										pDC->LineTo((w*y/g_Width)-8, (h*(g_Height-x)/g_Height)-8);
									}
									/*if(pView->m_BadImg[x][y] == 10)
									{
										pDC->MoveTo((w * y / g_Width) - 15, (h * (g_Height - x) / g_Height) - 15);
										pDC->LineTo((w * y / g_Width) - 15, (h * (g_Height - x) / g_Height) + 15);
										pDC->LineTo((w * y / g_Width) + 15, (h * (g_Height - x) / g_Height) + 15);
										pDC->LineTo((w * y / g_Width) + 15, (h * (g_Height - x) / g_Height) - 15);
										pDC->LineTo((w * y / g_Width) - 15, (h * (g_Height - x) / g_Height) - 15);
									}*/
								}
							}
						}
					}
				}
				else if(pView->m_Result_mode==3)
				{
					for(int x=g_Margin; x<g_Height-g_Margin; x++)
					{
						for(int y=g_Margin; y<g_Width-g_Margin; y++)
						{
							if(pView->m_BadImg[x][y]==8)
							{
								pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(238, 43, 233));
							}
						}
					}
				}
			}
			else
			{		
				int w=arect.right-arect.left;
				int h=arect.bottom-arect.top;

				for(int x=g_Margin; x<g_Height-g_Margin; x++)
				{
					for(int y=g_Margin; y<g_Width-g_Margin; y++)
					{
						if(pView->m_BadImg[x][y]==9)
						{
							pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(228, 238, 43));
						}

						if(x == g_Margin2 + g_OmitCondition1 || x == g_Height- g_OmitCondition2 )
						{
							pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(238, 43, 233));
						}
						
						if(pView->m_BadImg[x][y]==7)
						{
							pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(255, 100, 0));
						}
						/*
						if(x == g_MassCondition3 || x == g_Height - g_MassCondition2)
						{
							pDC->SetPixel((w*y/g_Width), (h*(g_Height-x)/g_Height), RGB(55, 192, 250));
							//printf("h %d \n", g_Height);
							//printf("mass %d \n", g_MassCondition2);
						}*/

					}
				}
			}
		}
	}
	//pView->ReleaseDC(pDC);
	pView->m_GrabImage.ReleaseDC(pDC);

	return 0;
}

UINT ThreadReject(LPVOID lParam)
{
	CIntellisenseView *pView;
	pView=(CIntellisenseView *)lParam;
	int g_TimeCheck;

	while(b_ThreadReject)
	{
		g_EventReject.Lock();
		while(g_RejectFront!=g_RejectRear)
		{
			//DWORD g_TimeCheck=timeGetTime();
			if(g_Model_Select == 0)
			{
				if(g_RejectType == 1)
				{
					
					g_TimeCheck = timeGetTime(); //리젝트 변경
				}
				else
				{					
					//20211221 엔코더 라이브러리 변경
					//g_TimeCheck = pView->m_ComiDaq1.EncRead(0);
					g_TimeCheck = pView->m_comi.Read();
				}
				
			}
			else if(g_Model_Select == 1) //20190327
			{
				
				g_TimeCheck = pView->m_InternalCount;
			}
			else
			{
				if(g_RejectType == 1)
				{
					
					g_TimeCheck = timeGetTime(); //리젝트 변경
				}
				else
				{
					//20211221 엔코더 라이브러리 변경
					//g_TimeCheck = pView->m_ComiDaq1.EncRead(0);
					g_TimeCheck = pView->m_comi.Read();
				}
			}

			if(g_TimeCheck>=g_Rejecttime[g_RejectFront])
			{
				if(NSE_IO_OUPUT_PORT == 2)
				{
					pView->RejectON_NewIO();
				}
				else
				{
					pView->RejectON();
				}
				

				g_RejectFront++;
				if(g_RejectFront>=10000)
					g_RejectFront=0;
			}
			Sleepms(1);
		}
	}	
	return 0;
}

HBRUSH CIntellisenseView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_STATIC) 
	{
		pDC->SetTextColor(RGB(255,255,255));  
        pOldBrush = (CBrush*)pDC->SelectObject(&Brush);
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH) Brush;
    }
	if(nCtlColor == CTLCOLOR_EDIT)
	{
		if(pWnd->m_hWnd == m_Edit_Time.m_hWnd || pWnd->m_hWnd == m_Edit_Eapermin || pWnd->m_hWnd == m_Edit_Time3.m_hWnd)
        {
			pDC->SetBkColor(RGB(3,15, 57));
			pDC->SetTextColor(RGB(255, 255, 255));
			return (HBRUSH) m_bkbrush1;
        }
		else
		{
			pDC->SetBkColor(RGB(75, 87, 118));
			pDC->SetTextColor(RGB(255,255,255));
			return (HBRUSH) m_bkbrush2;
		}
    }	
	return hbr;
}

void CIntellisenseView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//ClearResultProgress();
	DrawResult();
	DrawResultProgress();
	DrawXrayState();
}

void CIntellisenseView::OnRecognition(int count)
{
	if(m_Interock_State || g_XrayState==0 || g_XrayState==1 || b_IsNormalization==2 || b_ConveyorOn)  //kds20090922
		return;

	int m_Starttime2 = g_Starttime;
	if(g_bXrayAutoSetup)
	{
		OnComputeXray(count);
		return;
	}


	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CCtrlPanelView* pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();

	g_Dent_Save = false;

	b_TestMode=true; //20140821
	
	OnReSize(count);
	

	
	Masking(count); //0210
	
	if(g_TalMask_Bright == 2)
	{
		Masking2(count);
	}
	
	int m_iCount_1 = 0;
	for (int x = g_Margin; x < g_Width - 20; x++)
	{
		for (int y = g_Margin; y < g_Height - 20; y++)
		{

			if(g_Img_Bufx2[count][x][y] < 170)  //작은거 찍힌거 안보이게 하기
			{
				m_iCount_1++;
			}
		}
	}

	if(m_iCount_1 <= 2000)  //픽 셀갯수 2000개 미만 이미지 짤리는 문제 
		return;

	g_BackupRear++;
	if(g_BackupRear>=10000)
	{
		g_BackupRear=0;
		g_BackupFront++;
	}
	if(g_BackupRear<g_BackupFront)
		g_BackupFront++;

	

	//EYH20110602
	//g_BackupCond1[g_BackupRear]=m_MinValue;

//********************************************

	if(g_bAutoSetup)
	{
			bool r_cond1=true;
			bool r_cond2=true;
			bool r_cond3=true;
			bool r_cond4=true;
			bool r_cond5=true;
			bool r_cond6=true;

			if(g_TalMask_Bright ==1 || g_TalMask_Bright == 3)
			{
				#pragma omp parallel 
				{
					#pragma omp sections
					{
						#pragma omp section
									r_cond1=Detect_Cond1(count);
						#pragma omp section
									r_cond2=Detect_Cond22(count);
						#pragma omp section
									r_cond3=Detect_Cond33(count);
						#pragma omp section
									r_cond4=Detect_Cond44(count); 
						#pragma omp section
									r_cond5=Detect_Cond55(count);
						#pragma omp section
									r_cond6=Detect_Cond66(count); 
					}
				}
			}
			else
			{
				#pragma omp parallel 
				{
					#pragma omp sections
					{
						#pragma omp section
									r_cond1=Detect_Cond1(count);
						#pragma omp section
									r_cond2=Detect_Cond2(count);
						#pragma omp section
									r_cond3=Detect_Cond3(count);
						#pragma omp section
									r_cond4=Detect_Cond4(count); 
						#pragma omp section
									r_cond5=Detect_Cond5(count);
						#pragma omp section
						{
							if(g_TalMask_Bright == 2)
									r_cond6=Detect_Cond666(count);
							else
									r_cond6=Detect_Cond6(count);
						}
					}
				}
			}


		g_EventPaint.PulseEvent();
	}
	else
	{
	//***************************************검사***********************************************
		//20160428
		float m_Reject_count;
		m_Reject_count = (float)g_SetRejectrate / 100;
		if(Detect_Omit(count) && Dent_line(count))	// true : 정상 false : 이물
		{
			g_BackupCond1[g_BackupRear]=0;
			g_BackupCond2[g_BackupRear]=0;
			g_BackupCond3[g_BackupRear]=0;
			g_BackupCond4[g_BackupRear]=0;
			g_BackupCond5[g_BackupRear]=0;
			g_BackupCond6[g_BackupRear]=0;
			bool r_cond1=true;
			bool r_cond2=true;
			bool r_cond3=true;
			bool r_cond4=true;
			bool r_cond5=true;
			bool r_cond6=true;
			bool r_cond7 = true;

			
			//if(g_TalMask_Bright == 1 || g_TalMask_Bright == 3)
			//if(g_TalMask_Bright == 0 || g_TalMask_Bright == 1 || g_TalMask_Bright == 3)
			if(g_TalMask_Bright == 1 || g_TalMask_Bright == 3)
			{
				
				r_cond1=Detect_Cond1(count);
				
				
				r_cond2=Detect_Cond22(count);
				
				
				r_cond3=Detect_Cond33(count);
				
				
				//r_cond4 = Detect_Cond44(count);
				r_cond4=Detect_Cond4(count);
				
				
				r_cond5=Detect_Cond55(count);
				
				
				r_cond6=Detect_Cond66(count);
				
			}
			else
			{
				
				r_cond1=Detect_Cond1(count);

				r_cond2=Detect_Cond2(count);

				r_cond3=Detect_Cond3(count);
	
				r_cond4=Detect_Cond4(count); 

				r_cond5=Detect_Cond5(count);

				if(g_TalMask_Bright == 2)
				{
					r_cond6=Detect_Cond666(count); 
				}
				else
					r_cond6=Detect_Cond6(count); 
				
			}
			//20170914 수정 끝
			/*
			if (r_cond1 && r_cond2 && r_cond3 && r_cond4 && r_cond5 && r_cond6)
			{
				
				
				if (g_MassCondUse == 0)
					r_cond7 = Detect_Cond7(count);
				else
					r_cond7 = true;
					
				
				r_cond7 = true;
			}
			else
				r_cond7 = true;*/

			/*if (g_MassCondUse == 0)
				r_cond7 = Detect_Cond7(count);
			else
				r_cond7 = true;*/

			if(r_cond1 && r_cond2 && r_cond3 && r_cond4 && r_cond5 && r_cond6 && r_cond7)
			{
				m_Result_mode=1;
				g_iRejectCounter = 0;
			//	g_Endtime = timeGetTime();
			}
			else
			{
				m_Result_mode=2;
				
				//g_Endtime = timeGetTime();	

				if(g_RejectUse!=1 && g_bIOcard)
				{
					if(g_RejectType == 1)
					{
						g_Rejecttime[g_RejectRear]=g_Starttime+g_RejectDelay;
					}
					else
					{
						g_Rejecttime[g_RejectRear] = m_Starttime2 + g_RejectDelay + ((1000 - g_reject_new_time) * m_Reject_count);
					}
					
					if(g_Model_Select == 0 && g_RejectType == 0) //20170529 리젝트 타입이 Encoder 일때만 동작
					{					
						if(g_Rejecttime[g_RejectRear] > 8388607 )
						{
							int m_iGap = g_Rejecttime[g_RejectRear] - 8388607; 
							g_Rejecttime[g_RejectRear] = (-8388608) + m_iGap;
						}
					}
								
					g_RejectRear++;
					if(g_RejectRear>=10000)
						g_RejectRear=0;
					g_EventReject.PulseEvent();
				}

				if(g_SetRejectcount != 0)
				{
					g_iRejectCounter++;

					if(g_iRejectCounter>g_SetRejectcount && g_bIOcard)
					{
						g_Continu_Ng = 1;
						if(NSE_IO_OUPUT_PORT == 2)
						{
							g_Bit_Out[5]=1; //부져
							g_Bit_Out[3]=0;
							
							g_Bit_Out[12]=1; //연속불량
							OnDigitalOutPut_All22();
						}
						else
						{
							//20200525
							g_portWriteData = 0x59;
							ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
							if(error)
								return;
							//20200525
			/*
							USHORT      gwMask = NSE_CONVERYOR_MASKING;                  // mask
							USHORT      gwData = NSE_CONVERYOR_OFF;                  // output data

							m_ptDioWritePortByte.port = NSE_IO_OUPUT_PORT;
							m_ptDioWritePortByte.mask  = 0x44;
							m_ptDioWritePortByte.state = 0x40;

							if((m_ErrCde = DRV_DioWritePortByte(m_DriverHandle,
								 (LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
							{
								DRV_DioWritePortByte(m_DriverHandle,(LPT_DioWritePortByte)&m_ptDioWritePortByte);
								return;
							}*/
						}

						g_ConReject = 1;
						if(g_NationalCode==0)
							pCtrlView->m_Bt_Start.LoadBitmaps(IDB_BTCONVEYON,IDB_BTCONVEYON_1); 
						else if(g_NationalCode==1)
							pCtrlView->m_Bt_Start.LoadBitmaps(IDB_BTCONVEYON_ENG,IDB_BTCONVEYON_1_ENG); 
						else if(g_NationalCode==2)
							pCtrlView->m_Bt_Start.LoadBitmaps(IDB_BTCONVEYON_CHI,IDB_BTCONVEYON_1_CHI); 
						else if(g_NationalCode==3)
							pCtrlView->m_Bt_Start.LoadBitmaps(IDB_BTCONVEYON_SPN,IDB_BTCONVEYON_1_SPN); 
						pCtrlView->m_Bt_Start.Invalidate(); 
					}
				}
			}
		}
		else  //누락
		{
			m_Result_mode=3;

		//	g_Endtime = timeGetTime();	

			if(g_RejectUse!=1) 
			{
				if(g_RejectType == 1)
				{
					g_Rejecttime[g_RejectRear]=g_Starttime+g_RejectDelay;
					
				}
				else
				{
					g_Rejecttime[g_RejectRear] = m_Starttime2 + g_RejectDelay + ((1000 - g_reject_new_time) * m_Reject_count);
				}
			
				if(g_Model_Select == 0 && g_RejectType == 0)  //20170529 리젝트 타입이 Encoder 일때만 동작
				{					
					if(g_Rejecttime[g_RejectRear] > 8388607 )
					{
						int m_iGap = g_Rejecttime[g_RejectRear] - 8388607; 
						g_Rejecttime[g_RejectRear] = (-8388608) + m_iGap;
					}
				}		
					
				g_RejectRear++;
				if(g_RejectRear>=10000)
					g_RejectRear=0;
				g_EventReject.PulseEvent();
			}
			if(g_SetRejectcount != 0)
			{
				g_iRejectCounter++;

				if(g_iRejectCounter>g_SetRejectcount && g_bIOcard)
				{
					g_Continu_Ng = 1;
					if(NSE_IO_OUPUT_PORT == 2)
					{
						g_Bit_Out[3]=0;
						g_Bit_Out[5]=1;
						
						g_Bit_Out[12]=1;
						OnDigitalOutPut_All22();
					}
					else
					{//20200525
						g_portWriteData = 0x59;
						ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
						if(error)
							return;
						//20200525
			/*
						USHORT      gwMask = NSE_CONVERYOR_MASKING;                  // mask
						USHORT      gwData = NSE_CONVERYOR_OFF;                  // output data

						m_ptDioWritePortByte.port = NSE_IO_OUPUT_PORT;
						m_ptDioWritePortByte.mask  = 0x44;
						m_ptDioWritePortByte.state = 0x40;

						if((m_ErrCde = DRV_DioWritePortByte(m_DriverHandle,
								(LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
						{
							DRV_DioWritePortByte(m_DriverHandle,(LPT_DioWritePortByte)&m_ptDioWritePortByte);
							return;
						}*/
					}

					g_ConReject = 1;
					if(g_NationalCode==0)
						pCtrlView->m_Bt_Start.LoadBitmaps(IDB_BTCONVEYON,IDB_BTCONVEYON_1); 
					else if(g_NationalCode==1)
						pCtrlView->m_Bt_Start.LoadBitmaps(IDB_BTCONVEYON_ENG,IDB_BTCONVEYON_1_ENG); 
					else if(g_NationalCode==2)
						pCtrlView->m_Bt_Start.LoadBitmaps(IDB_BTCONVEYON_CHI,IDB_BTCONVEYON_1_CHI); 
					else if(g_NationalCode==3)
						pCtrlView->m_Bt_Start.LoadBitmaps(IDB_BTCONVEYON_SPN,IDB_BTCONVEYON_1_SPN); 
					pCtrlView->m_Bt_Start.Invalidate(); 
				}
			}
		}

		GetSystemTime(&m_Rstm);
		m_Rtime=CTime::GetCurrentTime();
		g_Filename.Format(_T("%04d-%02d-%02d-%02d%02d%02d%03d"), m_Rtime.GetYear(), m_Rtime.GetMonth(), 
		m_Rtime.GetDay(), m_Rtime.GetHour(), m_Rstm.wMinute, m_Rstm.wSecond, m_Rstm.wMilliseconds);
		m_strdate.Format(_T("%04d-%02d-%02d"), m_Rtime.GetYear(),m_Rtime.GetMonth(),m_Rtime.GetDay());		
		m_ConditionInfo[count].Format(_T("%d(%d)/%d(%d)/%d(%d)/%d(%d)/%d(%d)/%d(%d)"), g_Condition1, g_BackupCond1[g_BackupRear], g_Condition2, g_BackupCond2[g_BackupRear], g_Condition3, g_BackupCond3[g_BackupRear], g_Condition4, g_BackupCond4[g_BackupRear], g_Condition5, g_BackupCond5[g_BackupRear], g_Condition6, g_BackupCond6[g_BackupRear]);
		m_ConditionInfo1[count].Format(_T("%d/%d/%d(%d)/%d/%d/%d"), g_OmitCondition1, g_OmitCondition2, g_OmitCondition3, g_OmitCondition3_Disp, g_OmitCondition4, g_OmitCondition5, g_OmitAvr);
		m_ConditionInfo2[count].Format(_T("%d(%d)(%d)/%d/%d/%d/%d/%d"),g_MassCondition1,g_Dent_L,g_Dent_R,0,0,0,0,0);

		if(m_Result_mode == 2 || m_Result_mode == 3)
		{
			if(!(g_SaveFile && g_SaveHard))
			{
				if(!g_SaveFile)
					MakeSubDirectory();
				if(!g_SaveHard)
					MakeExternalDirectory();
				g_STime[count] = g_Filename; //20180528
				g_Savecount[count] = count;
				g_Arry_Save[count]=1;
				g_Savekind[count] = m_Result_mode;

			//	CIntellisenseDoc* pDoc= GetDocument();
			///	pDoc->OnFileSave(m_Result_mode);
			}
		}
		else  //20170628
		{
			if (!(g_SaveFile && g_SaveHard)) //ch 추가 
			{
				g_STime[count] = g_Filename; //20180528
				g_Savecount[count] = count;
			}

			if(!g_SaveFile)
				MakeSubDirectory();
			if(!g_SaveHard)
				MakeExternalDirectory();
		//	g_Savecount[count] = count;
			g_Arry_Save[count]=2;
			g_Savekind[count] = m_Result_mode;
		}

		g_EventPaint.PulseEvent();
		DrawResult();
		DrawResultProgress();
	}

	if(g_Today!=m_strdate && !b_CheckOverwrite)
	{
		b_CheckOverwrite=true;
		m_TempReportOk=m_ReportOk;
		m_TempReportNg=m_ReportNg;
		m_TempReportOmit=m_ReportOmit;
		m_TempReportTotal=m_ReportTotal;
		SetTimer(400, 50, NULL);

		//ch 음료 
		
		m_ReportOk=0;
		m_ReportNg=0;
		m_ReportOmit=0;
		m_ReportTotal=0;
	}


//************************************************************************************************************
	
	
	if(g_bAutoSetup)
	{
		g_AutoCount++;
		g_AutoCond1=g_BackupCond1[g_BackupRear];
		g_AutoCond2=g_BackupCond2[g_BackupRear];
		g_AutoCond3=g_BackupCond3[g_BackupRear];
		g_AutoCond4=g_BackupCond4[g_BackupRear];
		g_AutoCond5=g_BackupCond5[g_BackupRear];
		g_AutoCond6=g_BackupCond6[g_BackupRear];

		if(g_AutoCond1!=0)
			g_AutoCond1_1=(int)(((g_AutoCond1_1*(g_AutoCount-1))+g_AutoCond1)/g_AutoCount);
		if(g_AutoCond2!=0)
			g_AutoCond2_1=(int)(((g_AutoCond2_1*(g_AutoCount-1))+g_AutoCond2)/g_AutoCount);
		if(g_AutoCond3!=0)
			g_AutoCond3_1=(int)(((g_AutoCond3_1*(g_AutoCount-1))+g_AutoCond3)/g_AutoCount);
		if(g_AutoCond4!=0)
			g_AutoCond4_1=(int)(((g_AutoCond4_1*(g_AutoCount-1))+g_AutoCond4)/g_AutoCount);
		if(g_AutoCond5!=0)
			g_AutoCond5_1=(int)(((g_AutoCond5_1*(g_AutoCount-1))+g_AutoCond5)/g_AutoCount);
		if(g_AutoCond6!=0)
			g_AutoCond6_1=(int)(((g_AutoCond6_1*(g_AutoCount-1))+g_AutoCond6)/g_AutoCount);
		
	}
	else
	{

		m_BPMCount[m_BPMState]++;

		m_Count_Total++;	
		m_ReportTotal++;
		m_Count_Total2++;//20191202
		g_Sum_Total_Count++; //20191202

		if(g_RejectType == 0) //20190726
			Encoder_State(g_reject_new_time);

		switch(m_Result_mode)
		{
			case 1:
				++m_Count_OK;
				++m_ReportOk;
				++m_Count_OK2; //20191202
				++g_Sum_Ok_Count;//20191202
				break;
			case 2:
				++m_ReportNg;
				++m_Count_NG;
				++m_Count_NG2;//20191202
				++g_Sum_Ng_Count; //20191202
				SetDlgItemInt(IDC_CONDITION1_COUNT, m_CountCond1, true);
				SetDlgItemInt(IDC_CONDITION2_COUNT, m_CountCond2, true);
				SetDlgItemInt(IDC_CONDITION3_COUNT, m_CountCond3, true);
				SetDlgItemInt(IDC_CONDITION4_COUNT, m_CountCond4, true);
				SetDlgItemInt(IDC_CONDITION5_COUNT, m_CountCond5, true);
				SetDlgItemInt(IDC_CONDITION6_COUNT, m_CountCond6, true);
				break;
			case 3:
				++m_ReportOmit;
				++m_Count_Soup;
				++m_Count_Soup2; //20191202
				++g_Sum_Omit_Count;//20191202
				break;
		}

/*		bool g_bOpenDB=true;
		if(!m_pSet_1.Open_DB(g_StrDirPath[1]))		
		{
			g_bOpenDB=false;
			//RecoveryDatabase(g_StrDirPath[1], g_StrDirPath_copy[1]);
			if(m_pSet_1.Open_DB(g_StrDirPath[1]))		
				g_bOpenDB=true;
		}
		if(g_bOpenDB)
		{
			CString strSQL;
			strSQL.Format(_T("SELECT * FROM Product_Info WHERE strName = '%s' AND strDate = '%s'"), g_ProductName, m_strdate);
			CString strQuery;
			if(m_pSet_1.Select_DB(strSQL))
			{
				if(m_pSet_1.IsEOF())
				{
					strQuery.Format(_T("Insert INTO Product_Info(strName, strdate, strOk, strNg, strOmit, strTotal)") _T("VALUES('%s', '%s', %d, %d, %d, %d)"), 
						g_ProductName, m_strdate, m_ReportOk, m_ReportNg, m_ReportOmit, m_ReportTotal);
					m_pSet_1.Insert_DB(strQuery);
				}
				else
				{
					int tmp=0, tmp1=0, tmp2=0, tmp3=0;
					m_pSet_1.ChangeRsInt(_T("strOk"), tmp);
					m_pSet_1.ChangeRsInt(_T("strNg"), tmp1);
					m_pSet_1.ChangeRsInt(_T("strOmit"), tmp2);
					m_pSet_1.ChangeRsInt(_T("strTotal"), tmp3);
					strQuery.Format(_T("Update Product_Info SET strOk = %d, strNg = %d, strOmit = %d, strTotal = %d WHERE strName = '%s' AND strDate = '%s'"), 
						tmp+m_ReportOk, tmp1+m_ReportNg, tmp2+m_ReportOmit, tmp3+m_ReportTotal, g_ProductName, m_strdate);
					m_pSet_1.Update_DB(strQuery);			
				}
			}
			if(m_pSet_1.b_IsOpenedDB)
				m_pSet_1.Close_DB();
		}
*/		if(b_UseSmartView)
			WriteProductionInfo(m_Result_mode-1, m_Count_Total, m_Count_OK, m_Count_NG, m_Count_Soup);

		CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
		CCtrlPanelView *pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();
		pCtrlView->DisplayCount(m_Count_Total, m_Count_OK, m_Count_NG, m_Count_Soup);

/*		m_ReportOk=0;
		m_ReportNg=0;
		m_ReportOmit=0;
		m_ReportTotal=0;
*/	}




}



void CIntellisenseView::OnRecognition_Test(int count)
{

	b_TestMode=false; //20140821

	OnReSize(count);
	Masking(count);

	if(g_TalMask_Bright == 2)
	{
		Masking2(count);
	}

//	MaskingClip();
	//MaskingTalOx();
	//MaskingPaper();

	/*
	if(g_PreproUse==0) 
	{
		double value;
		double avr =0;
		int count_avr=0;
		for(int q=0; q<g_Height; q++)
		{
			for(int w=0; w<g_Width; w++)
			{
				if(m_Img_Buf[q][w]<170)
				{
					avr+=m_Img_Buf[q][w];
					count_avr++;
				}
			}
		}

		if(count_avr!=0)
		{
			avr = avr/count_avr;
			for(int i=4; i<g_Height-4; i++)
			{
				for(int j=4; j<g_Width-4; j++)
				{
					value=0;
					value = -10*m_Img_Buf[i][j] -10*m_Img_Buf[i+1][j] - 10*m_Img_Buf[i+2][j] 
							-10*m_Img_Buf[i][j+1] +10000*m_Img_Buf[i+1][j+1] -10*m_Img_Buf[i+2][j+1] 
							-10*m_Img_Buf[i][j+2] -10*m_Img_Buf[i+1][j+2] -10*m_Img_Buf[i+2][j+2];
									
					value = value/(avr*g_PreproValue);
			
					if(value>200)
					{
						m_Img_Buf[i][j]=200;
					}
					else 
					{
						m_Img_Buf[i][j]=(char)value;
					}
					if(g_PreproDisp==0)
					{
						m_Buffer_2[i*g_Width+j] = m_Img_Buf[i][j];
						m_Buffer_4[i*g_Width+j] = m_Img_Buf[i][j];
					}
				}
			}
		}
		else
		{
			for(int i=0; i<g_Height; i++)
			{
				for(int j=0; j<g_Width; j++)
				{
					if(m_Img_Buf[i][j]>200)
					{
						m_Img_Buf[i][j]=200;
					}
		
					if(g_PreproDisp==0)
					{
						m_Buffer_2[i*g_Width+j] = m_Img_Buf[i][j];
						m_Buffer_4[i*g_Width+j] = m_Img_Buf[i][j];
					}
				}
			}
		}
	}*/

	//20160428
	if(Detect_Omit(count))
	//if(Detect_Omit() && Labelling())
	{
		bool r_cond1=true;
		bool r_cond2=true;
		bool r_cond3=true;
		bool r_cond4=true;
		bool r_cond5=true;
		bool r_cond6=true;
		
		//if (g_TalMask_Bright == 1 || g_TalMask_Bright == 3)
			if(g_TalMask_Bright == 1 || g_TalMask_Bright == 3)
			{
				#pragma omp parallel 
				{
					#pragma omp sections
					{
						#pragma omp section
						{
							r_cond1=Detect_Cond1(count);
						}
						#pragma omp section
						{
							r_cond2=Detect_Cond22(count);
						}
						#pragma omp section
						{
							r_cond3=Detect_Cond33(count);
						}
						#pragma omp section
						{
							//r_cond4 = Detect_Cond44(count);
							r_cond4=Detect_Cond4(count);
						}
						#pragma omp section
						{
							r_cond5=Detect_Cond55(count);
						}
						#pragma omp section
						{
							r_cond6=Detect_Cond66(count); 
						}
					}
				}
			}
			else
			{
				#pragma omp parallel 
				{
					#pragma omp sections
					{
						#pragma omp section
						{
							r_cond1=Detect_Cond1(count);
						}
						#pragma omp section
						{
							r_cond2=Detect_Cond2(count);
						}
						#pragma omp section
						{
							r_cond3=Detect_Cond3(count);
						}
						#pragma omp section
						{
							r_cond4=Detect_Cond4(count); 
						}
						#pragma omp section
						{
							r_cond5=Detect_Cond5(count);
						}
						#pragma omp section
						{
							if(g_TalMask_Bright == 2)
								r_cond6=Detect_Cond666(count); 
							else
								r_cond6=Detect_Cond6(count); 
						}
					}
				}
			}

		if(r_cond1 && r_cond2 && r_cond3 && r_cond4 && r_cond5 && r_cond6)
		{
			m_Result_mode=1;
		}
		else
		{
			m_Result_mode=2;
		}
	}
	else  
	{
		m_Result_mode=3;
	}

	g_EventPaint.PulseEvent();
	DrawResult();
	DrawResultProgress();



	
}


void CIntellisenseView::RejectON()
{

	BOOL _ck = TRUE;
	if(g_bIOcard && g_RejectUse==0)//kds20130507 리젝트 선색시만 출력
	{
	//	while(_ck)
		for (int i = 0; (i<10) && _ck; i++) //kds20190428
		{
			// 연속 리젝트 중일때는 그대로 신호 유지하기 위해
			if (g_Continu_Ng == 1)
			{
				g_portWriteData = 0x5B;
			}
			else if (g_Continu_Ng == 0)
			{
				g_portWriteData = 0x1B;
			}
			//g_portWriteData = 0xB; 호남샤니
			ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
			if(error)
				return;

			byte m_state;
			m_state = m_instantDo->getState();

			if(m_state == g_portWriteData)
				_ck = false;
			//20200525
			/*
			USHORT      gwMask = NSE_REJECT_MASKING;                  
			USHORT      gwData = NSE_REJECT_ON;
			gwData = 38;
			CStdioFile FileString;
			CString s;
			s.Format(_T(" %d "), NSE_REJECT_ON);
			s = s+g_Filename+_T("\n");
		
			m_ptDioWritePortByte.port  = NSE_IO_OUPUT_PORT;
			m_ptDioWritePortByte.mask  = gwMask;
			m_ptDioWritePortByte.state = gwData;

			if((m_ErrCde = DRV_DioWritePortByte(m_DriverHandle,
					 (LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
			{
				DRV_DioWritePortByte(m_DriverHandle,(LPT_DioWritePortByte)&m_ptDioWritePortByte);
			}
			
		



			m_ptDioGetCurrentDOByte.port  = NSE_IO_OUPUT_PORT;
			m_ptDioGetCurrentDOByte.value = (USHORT far *)&gwDoState;

			if((m_ErrCde = DRV_DioGetCurrentDOByte(m_DriverHandle,
				(LPT_DioGetCurrentDOByte)&m_ptDioGetCurrentDOByte)) != 0)
			{
				DRV_DioGetCurrentDOByte(m_DriverHandle,(LPT_DioGetCurrentDOByte)&m_ptDioGetCurrentDOByte);
			}

			gwDoState &= NSE_REJECT_ON;

			if (gwDoState == NSE_REJECT_ON) {
				_ck = FALSE;
			}
			*/
		}

		

		Sleepms(g_RejectKeep);

		_ck = TRUE;
	//	while(_ck)
		for (int i = 0; (i<10) && _ck; i++) //kds20190428
		{
			// 연속 리젝트 중일때는 그대로 신호 유지하기 위해
			if (g_Continu_Ng == 1)
			{
				g_portWriteData = 0x59;
			}
			else if (g_Continu_Ng == 0)
			{
				g_portWriteData = 0x1D;
			}
			ErrorCode error = m_instantDo->Write(NSE_IO_OUPUT_PORT,1,&g_portWriteData);
			if(error)
				return;

			byte m_state;
			m_state = m_instantDo->getState();

			if(m_state == g_portWriteData)
				_ck = false;
			//20200525
			/*

			USHORT      gwMask = NSE_REJECT_MASKING;                  
			USHORT      gwData = NSE_REJECT_ON;

			gwMask = NSE_REJECT_MASKING;                  
			gwData = NSE_REJECT_OFF;
		//	USHORT gwData = 0x00;
			m_ptDioWritePortByte.port  = NSE_IO_OUPUT_PORT;
			m_ptDioWritePortByte.mask  = gwMask;
			m_ptDioWritePortByte.state = gwData;

			if((m_ErrCde = DRV_DioWritePortByte(m_DriverHandle,
					 (LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
			{
				//DRV_GetErrorMessage(m_ErrCde,(LPSTR)m_szErrMsg);
				DRV_DioWritePortByte(m_DriverHandle,(LPT_DioWritePortByte)&m_ptDioWritePortByte);
			}
			
			m_ptDioGetCurrentDOByte.port  = NSE_IO_OUPUT_PORT;
			m_ptDioGetCurrentDOByte.value = (USHORT far *)&gwDoState;

			if((m_ErrCde = DRV_DioGetCurrentDOByte(m_DriverHandle,
				(LPT_DioGetCurrentDOByte)&m_ptDioGetCurrentDOByte)) != 0)
			{
				DRV_DioGetCurrentDOByte(m_DriverHandle,(LPT_DioGetCurrentDOByte)&m_ptDioGetCurrentDOByte);
			}

			gwDoState &= NSE_REJECT_ON;

			if (gwDoState != NSE_REJECT_ON) {
				_ck = FALSE;
			}*/
		}
	}
	return;

	/*BOOL _ck = TRUE;
	if(g_bIOcard && g_RejectUse==0)//kds20130507 리젝트 선색시만 출력
	{
		while(_ck)
		{
			USHORT      gwMask = NSE_REJECT_MASKING;                  
			USHORT      gwData = NSE_REJECT_ON;                  
			m_ptDioWritePortByte.port  = NSE_IO_OUPUT_PORT;
			m_ptDioWritePortByte.mask  = gwMask;
			m_ptDioWritePortByte.state = gwData;

			if((m_ErrCde = DRV_DioWritePortByte(m_DriverHandle,
					 (LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
			{
				DRV_DioWritePortByte(m_DriverHandle,(LPT_DioWritePortByte)&m_ptDioWritePortByte);
			}
			Sleep(20);
			m_ptDioGetCurrentDOByte.port  = NSE_IO_OUPUT_PORT;
			m_ptDioGetCurrentDOByte.value = (USHORT far *)&gwDoState;

			if((m_ErrCde = DRV_DioGetCurrentDOByte(m_DriverHandle,
				(LPT_DioGetCurrentDOByte)&m_ptDioGetCurrentDOByte)) != 0)
			{
				DRV_DioGetCurrentDOByte(m_DriverHandle,(LPT_DioGetCurrentDOByte)&m_ptDioGetCurrentDOByte);
			}

			gwDoState &= NSE_REJECT_ON;

			if (gwDoState == NSE_REJECT_ON) {
				_ck = FALSE;
			}
		}

		Sleepms(g_RejectKeep);

		_ck = TRUE;
		while(_ck)
		{
			USHORT gwMask = NSE_REJECT_MASKING;                  
			USHORT gwData = NSE_REJECT_OFF;
			m_ptDioWritePortByte.port  = NSE_IO_OUPUT_PORT;
			m_ptDioWritePortByte.mask  = gwMask;
			m_ptDioWritePortByte.state = gwData;

			if((m_ErrCde = DRV_DioWritePortByte(m_DriverHandle,
					 (LPT_DioWritePortByte)&m_ptDioWritePortByte)) != 0)
			{
				//DRV_GetErrorMessage(m_ErrCde,(LPSTR)m_szErrMsg);
				DRV_DioWritePortByte(m_DriverHandle,(LPT_DioWritePortByte)&m_ptDioWritePortByte);
			}
			
			m_ptDioGetCurrentDOByte.port  = NSE_IO_OUPUT_PORT;
			m_ptDioGetCurrentDOByte.value = (USHORT far *)&gwDoState;

			if((m_ErrCde = DRV_DioGetCurrentDOByte(m_DriverHandle,
				(LPT_DioGetCurrentDOByte)&m_ptDioGetCurrentDOByte)) != 0)
			{
				DRV_DioGetCurrentDOByte(m_DriverHandle,(LPT_DioGetCurrentDOByte)&m_ptDioGetCurrentDOByte);
			}

			gwDoState &= NSE_REJECT_ON;

			if (gwDoState != NSE_REJECT_ON) {
				_ck = FALSE;
			}
		}	
		//여기서 부터 안넘어감
	}*/
}

void CIntellisenseView::RejectON_NewIO() 
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CCtrlPanelView* pCtrlView = (CCtrlPanelView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetControlView();

	if(g_bIOcard && g_RejectUse==0)
	{	
		g_Bit_Out[1]=1;
		g_Bit_Out[2]=1;
		g_Bit_Out[5]=1;
		g_Bit_Out[3]=0;
		g_bReject=false;
		if(!OnDigitalOutPut_All22() )
		{
	/*		//log 저장 //kds20140819 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			CString str;
			CTime time;
			time=CTime::GetCurrentTime();
			str.Format(_T("%04d-%02d-%02d-%02d%02d%02d =====>>>> 실패1"),time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

			CString checktime;
			checktime=str+" \r\n";

			CString m_sYear;
			m_sYear.Format("D:\\Database\\Reject_Error_%04d.Log",time.GetYear());

			//HANDLE hFile = CreateFile(_T("D:\\Database\\Conveyor.Log"),
			HANDLE hFile = CreateFile(_T(m_sYear),
				GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ,
				NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile == INVALID_HANDLE_VALUE){
			}
			else
			{
				CFile myFile((int)hFile);
				LONG lOffset = 0, lActual;
				lActual = myFile.Seek( lOffset, CFile::end );
				CString sz = _T(checktime);
				myFile.Write(sz, lstrlen(sz))
				myFile.Close();
			}
		*/	//log 저장 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			if( !OnDigitalOutPut_All22() )
			{
				//log 저장 //kds20140819 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		/*		CString str;
				CTime time;
				time=CTime::GetCurrentTime();
				str.Format("%04d-%02d-%02d-%02d%02d%02d =====>>>> 실패2",time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

				CString checktime;
				checktime=str+" \r\n";

				CString m_sYear;
				m_sYear.Format("D:\\Database\\Reject_Error_%04d.Log",time.GetYear());

				//HANDLE hFile = CreateFile(_T("D:\\Database\\Conveyor.Log"),
				HANDLE hFile = CreateFile(_T(m_sYear),
					GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ,
					NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

				if (hFile == INVALID_HANDLE_VALUE){
				}
				else
				{
					CFile myFile((int)hFile);
					LONG lOffset = 0, lActual;
					lActual = myFile.Seek( lOffset, CFile::end );
					CString sz = _T(checktime);
					myFile.Write(sz, lstrlen(sz));
					myFile.Close();
				}
		*/		//log 저장 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
				OnDigitalOutPut_All22();
				//AfxMessageBox("리젝트 실패!");
			}
		}
		g_bReject=true;

		Sleepms(g_RejectKeep);//유지시간
	
		g_Bit_Out[1]=0;
		g_Bit_Out[2]=0;
		if(g_Continu_Ng == 1)
		{
			g_Bit_Out[5] = 1;
		}
		else if(g_Continu_Ng == 0)
		{
			g_Bit_Out[5] = 0;
		}
		g_Bit_Out[3]=1;
		//pCtrlView->OnDigitalOutPut_All();	
		g_bReject=false;
		if(!OnDigitalOutPut_All22())
		{
			if(!OnDigitalOutPut_All22())
			{
				OnDigitalOutPut_All22();
				//AfxMessageBox("리젝트 복귀 실패!");
			}
		}	
		g_bReject=true;
	}

	if(g_bIOcard && g_RejectUse==2)
	{ 
		g_bReject=false;
		pCtrlView->OnStopConveyor();
		g_bReject=true;

		g_Bit_Out[1]=1;
		g_Bit_Out[2]=1;
		g_Bit_Out[5]=1;
		g_Bit_Out[3]=0;
		//pCtrlView->OnDigitalOutPut_All();
		g_bReject=false;
		if( !OnDigitalOutPut_All22() ){
			if( !OnDigitalOutPut_All22() ){
				OnDigitalOutPut_All22();
				//AfxMessageBox("리젝트 실패!");
			}
		}
		g_bReject=true;

		Sleepms(g_RejectKeep);//유지시간
	
		g_Bit_Out[1]=0;
		g_Bit_Out[2]=0;
		g_Bit_Out[5]=0;
		g_Bit_Out[3]=1;
		//pCtrlView->OnDigitalOutPut_All();
		g_bReject=false;
		if( !OnDigitalOutPut_All22() ){
			if( !OnDigitalOutPut_All22() ){
				OnDigitalOutPut_All22();
				//AfxMessageBox("리젝트 실패!");
			}
		}
		g_bReject=true;
		
		b_ConveyorOn=true;  //영상촬영 정지 및 검사정지
		if(g_NationalCode==0)
			pCtrlView->m_Bt_Start.LoadBitmaps(IDB_BTCONVEYON,IDB_BTCONVEYON_1); 
		else if(g_NationalCode==1)
			pCtrlView->m_Bt_Start.LoadBitmaps(IDB_BTCONVEYON_ENG,IDB_BTCONVEYON_1_ENG); 
		else if(g_NationalCode==2)
			pCtrlView->m_Bt_Start.LoadBitmaps(IDB_BTCONVEYON_CHI,IDB_BTCONVEYON_1_CHI); 
		else if(g_NationalCode==3)
			pCtrlView->m_Bt_Start.LoadBitmaps(IDB_BTCONVEYON_SPN,IDB_BTCONVEYON_1_SPN); 
		pCtrlView->m_Bt_Start.Invalidate(); 
	}
}

UINT ThreadAlarm(LPVOID lParam)   //누락시 별도 처리
{/*
	CIntellisenseView *pView;
	pView=(CIntellisenseView *)lParam;

	UCHAR m_szErrMsg[50];
	while(b_ThreadAlarm)
	{
		g_EventAlarm.Lock();
		if(g_bIOcard && b_ThreadAlarm)
		{
				m_ptDioWriteBit.port  = NSE_IO_OUPUT_PORT;
				m_ptDioWriteBit.bit  = 7;
				m_ptDioWriteBit.state = 1;

				if((m_ErrCde = DRV_DioWriteBit(m_DriverHandle,
						 (LPT_DioWriteBit)&m_ptDioWriteBit)) != 0)
				{
					DRV_GetErrorMessage(m_ErrCde,(LPSTR)m_szErrMsg);
				}

				Sleepms(2000);

				m_ptDioWriteBit.port  = NSE_IO_OUPUT_PORT;
				m_ptDioWriteBit.bit  = 7;
				m_ptDioWriteBit.state = 0;

				if((m_ErrCde = DRV_DioWriteBit(m_DriverHandle,
						 (LPT_DioWriteBit)&m_ptDioWriteBit)) != 0)
				{
					DRV_GetErrorMessage(m_ErrCde,(LPSTR)m_szErrMsg);
				}


		}
	}		
*/
	return 0;
}

void CIntellisenseView::OnBnClickedBtload()
{
	CIntellisenseDoc* pDoc= GetDocument();
	pDoc->OnFileOpen();
}



void CIntellisenseView::OnComputeXray(int I_Count)
{
	int cd=0;
	for(int a=0; a<g_Height; a++)
	{
		for(int b=0; b<g_Width; b++)
		{
			g_Img_Buf[I_Count][a][b] = g_Img_Buf3D[I_Count][a][b];

			if(NSE_DT_ISBULK==0 || NSE_DT_ISBULK==1)
			{
				if(!NSE_ROTATION_UPDOWN && NSE_ROTATION_LEFTRIGHT) 
				{
					m_BadImg[a][b]=0;
					m_Img_Buf[a][b] = g_Img_Buf[I_Count][g_Height-1-a][b]; 
					m_Buffer_2[cd] = g_Img_Buf[I_Count][g_Height-1-a][b]; 
				}
				else if(NSE_ROTATION_UPDOWN && !NSE_ROTATION_LEFTRIGHT)
				{
					m_BadImg[a][b]=0;
					m_Img_Buf[a][b] = g_Img_Buf[I_Count][a][g_Width-1-b]; 
					m_Buffer_2[cd] = g_Img_Buf[I_Count][a][g_Width-1-b];  
				}
				else if(NSE_ROTATION_UPDOWN && NSE_ROTATION_LEFTRIGHT)
				{	
					m_BadImg[a][b]=0;
					m_Img_Buf[a][b] = g_Img_Buf[I_Count][g_Height-1-a][g_Width-1-b]; 
					m_Buffer_2[cd] = g_Img_Buf[I_Count][g_Height-1-a][g_Width-1-b];  
				}
				else
				{
					m_BadImg[a][b]=0;
					m_Img_Buf[a][b] = g_Img_Buf[I_Count][a][b]; 
					m_Buffer_2[cd] = g_Img_Buf[I_Count][a][b]; 
				}
				cd++;
			}
			else
			{
				m_BadImg[a][b]=0;
				m_Img_Buf[a][b] = g_Img_Buf[I_Count][a][b];
				m_Buffer_2[cd] = g_Img_Buf[I_Count][a][b];  
				cd++;
			}
		}
	}

	int i=0, j=0, gv;
	for(i=0;i<256;i++)
	{
		m_Histogram[i]=0;
	}

	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{
		for(j=g_Margin; j<g_Width-g_Margin;j++)
		{	
			gv=(int)m_Img_Buf[i][j];
			m_Histogram[gv]++;
		}
	}

	int m_MaxValue=230;
	int maxa=m_Histogram[0];
	for(i=0; i<230; i++)
	{
		if(maxa<m_Histogram[i])
		{
			maxa=m_Histogram[i];
			m_MaxValue=i;
		}
	}

	int minValue=m_MaxValue;
	for(i=m_MaxValue;i>=0;i--)
	{
		if(1>m_Histogram[i])
		{
			minValue=i;
			break;
		}
	}

	g_EventPaint.PulseEvent();

	g_AutoXrayCount++;
	if(g_AutoXrayCount!=0)
		g_AutoXrayValue=(int)(((g_AutoXrayValue*(g_AutoXrayCount-1))+m_MaxValue)/g_AutoXrayCount);
}


void CIntellisenseView::DisplayEAperMin(void)
{
	int sum=0;
	for(int i=0; i<10; i++)
		sum+=m_BPMCount[i];
	CString m_sTime2;
	m_sTime2.Format(_T("%d EA/Minute"), sum);
	GetDlgItem(IDC_EDIT_EAPERMIN)->SetWindowText(m_sTime2);//kds20140821
	if(m_BPMState>=9)
		m_BPMState=0;
	else
		m_BPMState++;
	m_BPMCount[m_BPMState]=0;
}



void CIntellisenseView::OnReSize(int I_Count)
{
	
	int i, j;
	int count=0;
	int g_Centerx=0;
	int miny=g_Width, maxy=0;
	int cd=0;

	//g_Picxel_count=0;
	
	for(i=0; i<g_Height; i++)
	{
		for(j=0; j<g_Width; j++)
		{

			g_Img_Buf[I_Count][i][j] = g_Img_Buf3D[I_Count][i][j];
			m_Img_Buf[i][j]= g_Img_Buf[I_Count][i][j];
			m_BadImg_Save[I_Count][i][j]=0;
			m_BadImg[i][j]=0;
		//	m_Img_Buf[i][j]= g_Img_Buf[I_Count][i][j];
			m_Buffer_1[I_Count][i][j] = g_Img_Buf[I_Count][i][j];
			m_BadImg6[i][j]=0;//kds20101223
			g_Buffer_Resize[I_Count][i][j]=200;
			g_Img_Bufx2_o[I_Count][i][j] = g_Img_Buf[I_Count][i][j];
			m_Buffer_2[cd] = g_Img_Buf[I_Count][i][j];
			cd++;

	/*		if(g_Img_Buf[I_Count][i][j] < 200)  //작은거 찍힌거 안보이게 하기
			{
				g_Picxel_count++;
			}
*/

			if(m_Buffer_1[I_Count][i][j]<=175 && (i>=g_Margin || i<=g_Height-g_Margin || j>=g_Margin || j<=g_Width-g_Margin))
			{
				count++;
				g_Centerx+=i;
				if(miny>j)
					miny=j;
				if(maxy<j)
					maxy=j;
			}
		}
	}
	
	
	
	g_SizeDisp=maxy-miny; //실제 크기 

	if(g_MaskClipUse == 1) //크기 조정 사용안하면 리턴
		return;

	if(g_SizeDisp<100 || g_MaskClip_Length<10 || g_MaskClip_Length>=g_SizeDisp && maxy-miny<=0)
	{
		return;
	}
	
	//	int m_plus=0;

	//	m_plus = -0.69*g_reject_new_time + 400;

	//	if(g_SizeDisp >= m_plus * 1.5)
	//		return;

	if(count==0)
	{
		count=1;
		return;
	}

	g_Centerx=g_Centerx/count;

	//if(g_Centerx>=200)

	if(g_Centerx>=100)
	{   
		float zoomoutfactor=g_MaskClip_Length/(float)(maxy-miny); 
		if(zoomoutfactor>=0.9)
			return;

		BYTE newValue;
		int new_width=(int)(g_Width*zoomoutfactor);
		int r, c;
		float r_orgc;
		int i_orgc;
		float sc;
		float I1, I2;

		for(r=0;r<g_Height;r++)
		{
			for(c=0; c<new_width;c++)
			{
				r_orgc=c/zoomoutfactor;
				i_orgc=(int)r_orgc;
				sc=r_orgc-i_orgc;
				//	if(r<g_Margin||r>g_Height-g_Margin || i_orgc<g_Margin || i_orgc>g_Width-g_Margin)
				if(r<g_Margin||r>g_Height-g_Margin || c<g_Margin || c>new_width-g_Margin)
				{
					g_Buffer_Resize[I_Count][r][c]=200;
				}
				else
				{
					if(m_Buffer_1[I_Count][r][i_orgc]<180)
					{
						I1=(float)m_Img_Buf[r][i_orgc];
						I2=(float)m_Img_Buf[r][i_orgc+1];
						//I3=(float)m_Buffer_1[r+1][i_orgc+1];
						//I4=(float)m_Buffer_1[r+1][i_orgc];
						newValue = (BYTE)(I1*(1-sc)+I2*sc);
						g_Buffer_Resize[I_Count][r][c]=newValue;
					}
					else
						g_Buffer_Resize[I_Count][r][c]=m_Buffer_1[I_Count][r][i_orgc];
				}	
			}
		}

		count=0;
		int tempy=0;
		//20151105
		iv_11=g_Width-1;
		iv_22=0;
		for(i=0;i<g_Height;i++)
		{
			for(j=0;j<g_Width;j++)
			{	
				g_Img_Bufx2_o[I_Count][i][j]=250;
				m_Buffer_1[I_Count][i][j]=200;
				m_Img_Buf[i][j]=200;
				if(g_Buffer_Resize[I_Count][i][j]<=175 && j>=1 && j<=new_width-1)
				{
					count++;
					tempy+=j;

				}
			}
		}

		if(count==0)
		{
			count=1;
			return;
		}
		tempy=tempy/count;

		iv_11=g_Width-tempy;
		if(iv_11<0)
			iv_11=0;
		iv_22=g_Width-tempy+new_width;
		if(iv_22>g_Width)
			iv_22=g_Width;

		for(i=1;i<g_Height-1;i++)
		{
			for(j=1; j<new_width-1; j++)
			{	
				if(j+g_Width/2-tempy>0 && j+g_Width/2-tempy<g_Width)
				{
					m_Buffer_1[I_Count][i][j+g_Width/2-tempy]=g_Buffer_Resize[I_Count][i][j];
					m_Img_Buf[i][j+g_Width/2 -tempy]=g_Buffer_Resize[I_Count][i][j];

				}
			}
		}
	}


	//20151109 축소 알고리즘
		cd=0;
	for(i=0; i<g_Height; i++)//640
	{
		for(j=0; j<g_Width; j++)//960
		{		  
			g_Img_Bufx2_o[I_Count][i][j] = m_Buffer_1[I_Count][i][j];
			m_Buffer_2[cd] = g_Img_Bufx2_o[I_Count][i][j];
				cd++;


		}
	}


/*
	for(i=0; i<g_Height; i++)
	{
		for(j=0; j<g_Width; j++)
		{
			if(cd<g_Height*g_Width)
			{
				m_Buffer_2[cd] = g_Img_Bufx2[I_Count][i][j];
				cd++;
			}
		}
	}*/
	
}


void CIntellisenseView::OnThreadRecoginition(void)
{
	//20170914 수정시작
	g_Thread_Count++;

	if(g_Thread_Count >= 50)
	{
		g_Thread_Count = 1;
	}
	//20170914 수정 끝


	AfxBeginThread(ThreadInspection,(LPVOID)this);
}

UINT ThreadInspection(LPVOID lParam)
{
	CIntellisenseView *pView;
	pView=(CIntellisenseView *)lParam;

	pView->OnRecognition(g_Thread_Count);
	return 0;
}

bool CIntellisenseView::Dent_line(int I_count)
{
	int line_count_Rmax = 0;
	int line_count_Rmin = 640;

	int line_count_Lmax = 0;
	int line_count_Lmin = 640;
	
	int Total_R=0;
	int Total_L=0;

	if(g_MassCondUse != 0 || g_MassCondition1 == 0)
		return true;

	for(int i=g_MassCondition3;i<g_Height-g_MassCondition2;i++)
	{
		for(int j=g_Margin;j<g_Width-g_Margin;j++)
		{
			if(m_Buffer_1[I_count][i][j]<170)
			{
				if(line_count_Lmax < j)
				{
					line_count_Lmax = j;
				}
				else if(line_count_Lmin > j)
				{
					line_count_Lmin = j;
				}
				break;	
			}
		}
	}
	

	for(int i=g_MassCondition3;i<g_Height-g_MassCondition2;i++)
	{
		for(int j=g_Width-g_Margin;j>g_Margin;j--)
		{
			if(m_Buffer_1[I_count][i][j]<170)
			{
				if(line_count_Rmax < j)
				{
					line_count_Rmax = j;
				}
				else if(line_count_Rmin > j)
				{
					line_count_Rmin = j;
				}
				break;
			}
			
		}
	}


	Total_R = line_count_Rmax - line_count_Rmin;
	Total_L = line_count_Lmax - line_count_Lmin;

	g_Dent_L = Total_L;
	g_Dent_R = Total_R;


	if(Total_R >= g_MassCondition1 || Total_L >= g_MassCondition1)
	{
		g_Dent_Save = true;
		g_KindofOmit = 3;
		return false;
	}
	else
	{
		g_Dent_Save = false;
		return true;
	}
}


void CIntellisenseView::OnSaveImg(int I_count)
{
	CIntellisenseDoc* pDoc= GetDocument();
	CTime time;
	CString m_SaveFolder2;
	m_SaveFolder2 = "";
	CString g_filen;
	SYSTEMTIME m_Rstm;
	CString m_LogFilename;
	m_LogFilename = "";
	CString m_CondInfo;
	m_CondInfo = "";
	
	CMainFrame* pFrm = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	CIntellisenseView* pView = (CIntellisenseView*)((CChildFrame*)pFrm->m_pFirstChildFrm)->GetDisplayView();

	
//	KillTimer(1000);
	
	GetSystemTime(&m_Rstm);
	m_Rtime=CTime::GetCurrentTime();
	g_filen.Format(_T("%04d-%02d-%02d-%02d%02d%02d%03d%d"), m_Rtime.GetYear(), m_Rtime.GetMonth(), 
		m_Rtime.GetDay(), m_Rtime.GetHour(), m_Rstm.wMinute, m_Rstm.wSecond, m_Rstm.wMilliseconds,I_count);
	

		if(g_Arry_Save[I_count] == 1)
		{
			int m_Tcount = 0;
			m_Tcount = g_Savecount[I_count];

			/*	CStdioFile FileString;
				CString s;
				s.Format(_T("save count = %d "), I_count);
				s = s + g_Filename+_T("\n");
				if(FileString.Open(_T("C:\\test_reject2.txt"),
					CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::modeNoTruncate | CStdioFile::shareDenyNone))
				{
					FileString.SeekToEnd();
					FileString.WriteString(s);
					FileString.Close();
				}

				*/
			if(g_SaveHard)
				g_Arry_Save[I_count]=0;  //전체저장이 활성화되면 동작하지 않음.  // 20170628


			time = time.GetCurrentTime();
			m_SaveFolder2.Format(_T("D:\\nse-temp\\image\\%04d-%02d-%02d"), time.GetYear(), time.GetMonth(),time.GetDay());
		
		

// 			GetSystemTime(&m_Rstm);
// 			m_Rtime=CTime::GetCurrentTime();
// 			g_filen.Format(_T("%04d-%02d-%02d-%02d%02d%02d%03d%d"), m_Rtime.GetYear(), m_Rtime.GetMonth(), 
// 			m_Rtime.GetDay(), m_Rtime.GetHour(), m_Rstm.wMinute, m_Rstm.wSecond, m_Rstm.wMilliseconds,I_count);

			if(g_Savekind[I_count] == 2)
			{
				m_LogFilename=m_SaveFolder2+_T("\\NG\\Log\\")+g_filen+_T(".dat");

				m_CondInfo=pView->m_ConditionInfo[m_Tcount];
				CStdioFile fileString;
				CString bad;
		
				if(fileString.Open(m_LogFilename,
					CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
				{
					USES_CONVERSION;
					char* str = W2A(g_ProductName); //기록할 문자열을 유니코드에서 MBCS 문자열로 변환
					fileString.Write(str, strlen(str)); 

					bad.Format(_T("@%s!"), m_CondInfo);
					fileString.WriteString(bad);
				}
		
				int cou=0;
				char buf[15];


				for(int i=g_Margin;i<g_Height-g_Margin;i++)
				{
					for(int j=g_Margin; j<g_Width-g_Margin;j++)
					{
						if(pView->m_BadImg_Save[m_Tcount][i][j]!=0 && pView->m_BadImg_Save[m_Tcount][i][j]<7)
						{
						
							bad="";
							sprintf(buf, "%d.%d.%d+" ,pView->m_BadImg_Save[m_Tcount][i][j], i, j) ;
							bad += buf;	
							cou++;
							fileString.WriteString(bad);
						}
					}
				}
		
		
				bad.Format(_T("#"));
				fileString.WriteString(bad);
				fileString.Close();

			}
			else if(g_Savekind[I_count] == 3)
			{
				if(g_Dent_Save == false)  //20151217
				{
					m_LogFilename=m_SaveFolder2+_T("\\OMIT\\Log\\")+g_filen+_T("_Type0")+_T(".dat");	
					m_CondInfo=pView->m_ConditionInfo1[m_Tcount];
				}
				else
				{
					m_LogFilename=m_SaveFolder2+_T("\\OMIT\\Log\\")+g_filen+_T("_Type1")+_T(".dat");
					m_CondInfo=pView->m_ConditionInfo2[m_Tcount];
				}

			
				CStdioFile fileString;
				CString bad;
				if(fileString.Open(m_LogFilename,
					CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
				{
					USES_CONVERSION;
					char* str = W2A(g_ProductName); //기록할 문자열을 유니코드에서 MBCS 문자열로 변환
					fileString.Write(str, strlen(str)); 

					bad.Format(_T("@%s!"), m_CondInfo);
					fileString.WriteString(bad);
				}

			}
				
			CString m_Fullname;
			if(g_Savekind[I_count] == 2)
			{
				m_Fullname=m_SaveFolder2+_T("\\NG\\")+g_filen + _T(".jpg");
			}
			else if(g_Savekind[I_count] ==3)
			{
				if(g_Dent_Save == false)  //20151217
				{
					m_Fullname=m_SaveFolder2+_T("\\OMIT\\")+g_filen+_T("_Type0")+_T(".jpg");	
				}
				else
				{
					m_Fullname=m_SaveFolder2+_T("\\OMIT\\")+g_filen+_T("_Type1")+_T(".jpg");	
				}
			}
			
			// cximage
		//	CxImage ximg(g_Width,g_Height,8,CXIMAGE_FORMAT_JPG);
		////	ximg.SetJpegQuality(30); 이미지 품질 줄이기 (30이하로 하지 말기)20170628
		//	BYTE* pBaseTo;
		//	pBaseTo = ximg.GetBits();
		//	for(int i=0;i<g_Height;i++)
		//		for(int j=0;j<g_Width;j++)
		//		{
		//			*(pBaseTo+i*g_Width+j) = m_Buffer_1[m_Tcount][i][j];  //kds1212 (m_Buffer)m_Buffer   m_Buffer_1
		//		}
		//	ximg.SetGrayPalette();
		//	ximg.Save(m_Fullname,ximg.GetType());

			cv::Mat mImg(g_Height, g_Width, CV_8UC1);
			uchar* pBaseTo = mImg.data;

			for (int i = 0; i < g_Height; i++)
			{
				for (int j = 0; j < g_Width; j++)
				{
					pBaseTo[i * g_Width + j] = m_Buffer_1[m_Tcount][i][j]; 
				}
			}

			//cv::flip(mImg, mImg, 0);

			NSE::Save(mImg, m_Fullname);		// 이미지 품질 줄여야되나???

			Sleepms(80);
		
		}

		///////////////////////////////////////////// 20170628
		// 모든제품 영상 저장
	//	CIntellisenseDoc* pDoc= GetDocument();
		CString m_Fullname;

		if(!g_SaveHard && g_Arry_Save[I_count] == 2 || g_Arry_Save[I_count] == 1)
		{
			switch(g_Savekind[I_count])
			{
			case 1:
				m_Fullname=pDoc->m_SaveExternalName+_T("\\")+g_filen+_T(".jpg");
				break;
			case 2:
				m_Fullname=pDoc->m_SaveExternalName1+_T("\\")+g_filen+_T(".jpg");
				break;
			case 3:
				m_Fullname=pDoc->m_SaveExternalName2+_T("\\")+g_filen+_T(".jpg");
				break;
			}


			// cximage
			//CxImage ximg(g_Width,g_Height,8,CXIMAGE_FORMAT_JPG);
	  //  //	ximg.SetJpegQuality(100); //이미지 품질 줄이기 (30이하로 하지 말기)20170628
			//BYTE* pBaseTo;
			//pBaseTo = ximg.GetBits();
			//for(int i=0;i<g_Height;i++)
			//	for(int j=0;j<g_Width;j++)
			//	{
			//		*(pBaseTo+i*g_Width+j) = m_Buffer_1[I_count][i][j];  //kds1212 (m_Buffer)m_Buffer   m_Buffer_1
			//	}
			//	ximg.SetGrayPalette();
			//	ximg.Save(m_Fullname,ximg.GetType());

			//	g_Arry_Save[I_count] = 0;

			cv::Mat mImg(g_Height, g_Width, CV_8UC1);
			uchar* pBaseTo = mImg.data;

			for (int i = 0; i < g_Height; i++)
			{
				for (int j = 0; j < g_Width; j++)
				{
					pBaseTo[i * g_Width + j] = m_Buffer_1[I_count][i][j];
				}
			}

			//cv::flip(mImg, mImg, 0);

			NSE::Save(mImg, m_Fullname);		// 이미지 품질 줄여야되나???
			g_Arry_Save[I_count] = 0;


		}
		
		g_EventSave.PulseEvent();
	//	SetTimer(1000,10,NULL);


}
UINT ThreadRejectnew(LPVOID lParam)
{
	DWORD dwStart, dwEnd, nCnt;

	while(g_Count_Flag)
	{
		double c_speed = 0;
 		CIntellisenseView *pView;
		pView = (CIntellisenseView *)lParam;

		if(g_Model_Select == 0)
		{
			dwStart = pView->m_comi.Read();
			Sleep(20);
			dwEnd = pView->m_comi.Read();
		}
		else if(g_Model_Select == 1) //20190327
		{
			dwStart = pView->m_InternalCount;
			Sleep(20);
			dwEnd = pView->m_InternalCount;
		}

		g_reject_new_time = dwEnd - dwStart;

		if (nCnt == 0)
		{
			CString ss;
			ss.Format(_T("AVR Pulse : %d"), g_reject_new_time);
			pView->GetDlgItem(IDC_EDIT_TIME3)->SetWindowText(ss);
		}
		nCnt > 4 ? nCnt = 0 : nCnt++;

	//	CString ss;
	//	ss.Format(_T("AVR Pulse : %d"), g_reject_new_time);
	//	pView->GetDlgItem(IDC_EDIT_TIME3)->SetWindowText(ss);



	////	pView->Encoder_State(g_reject_new_time); //엔코더 감지기능 20181119
	//	Sleepms(100);
	}

	return 0;
}

bool CIntellisenseView::Detect_Cond22(int I_count)
{
		int i, j;
	bool result=true;

	int m_iTemp[8];
	int a1,a2,a3,a4=0;
	int min0=255, min1=255, mink=-1;
	for(i=0; i<8; i++)
		m_iTemp[i]=0;
	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{
		for(j=g_Margin;j<g_Width-g_Margin;j++) 
	//	for(j=iv_11;j<iv_22;j++) 
		{
			
			if( (g_Img_Bufx2[I_count][i][j]<160) && (g_Img_Bufx2[I_count][i-2][j]<245) && (g_Img_Bufx2[I_count][i-2][j+2]<245)   //kds201001 XIS-5000 (260) 
				&& (g_Img_Bufx2[I_count][i][j+2]<245) && (g_Img_Bufx2[I_count][i+2][j+2]<245) 
				&& (g_Img_Bufx2[I_count][i+2][j]<245) && (g_Img_Bufx2[I_count][i+2][j-2]<245)
				&& (g_Img_Bufx2[I_count][i][j-2]<245) && (g_Img_Bufx2[I_count][i-2][j-2]<245))
			{ 
				m_iTemp[0] = g_Img_Bufx2[I_count][i-2][j]-g_Img_Bufx2[I_count][i][j];
				m_iTemp[1] = g_Img_Bufx2[I_count][i-2][j+2]-g_Img_Bufx2[I_count][i][j];
				m_iTemp[2] = g_Img_Bufx2[I_count][i][j+2]-g_Img_Bufx2[I_count][i][j];
				m_iTemp[3] = g_Img_Bufx2[I_count][i+2][j+2]-g_Img_Bufx2[I_count][i][j];
				m_iTemp[4] = g_Img_Bufx2[I_count][i+2][j]-g_Img_Bufx2[I_count][i][j];
				m_iTemp[5] = g_Img_Bufx2[I_count][i+2][j-2]-g_Img_Bufx2[I_count][i][j];
				m_iTemp[6] = g_Img_Bufx2[I_count][i][j-2]-g_Img_Bufx2[I_count][i][j];
				m_iTemp[7] = g_Img_Bufx2[I_count][i-2][j-2]-g_Img_Bufx2[I_count][i][j];
			
				a1 = int((m_iTemp[0]+m_iTemp[1]+m_iTemp[2]+m_iTemp[3]+m_iTemp[4]+m_iTemp[5]+m_iTemp[6]+m_iTemp[7])/8);

				a2 = 200-g_Img_Bufx2[I_count][i][j];
				a3 = int((a1*1.5)+((a2/10)*5));
 		//		if(i>g_Margin2+80){ //kds20140701
 		//			a3 = a3-25;
 		//		}
				min0=255, min1=255;

				if(a3>=g_Condition2-10)
				{
					//20141027
					//정렬 처리>>>>>올림차순
				/*	int tmp=0;
					int max=0;
					for (int a=0; a < 7; a++){
						for (int b=a+1; b < 8; b++){
							if (m_iTemp[b] < m_iTemp[a]){
								tmp = m_iTemp[b];
								m_iTemp[b] = m_iTemp[a];
								m_iTemp[a] = tmp;
							}
						}
					}
					max = m_iTemp[1];
				*/
					for(int k=0; k<8; k++)
					{
						if(min0>=m_iTemp[k])
						{
							min0=m_iTemp[k];
							mink=k;
						}
					}

					for(int k=0; k<8; k++)
					{
						if(min1>m_iTemp[k] && mink!=k)
							min1=m_iTemp[k];
					}

					a2 = 200-m_Img_Buf[i][j];
					a3 = int((min1*1.5)+((a2/10)*5));
 		//			if(i>g_Margin2+80){ //kds20140701
 		//				a3 = a3-15;
 		//			}
					//<<<<<<<<<<<<<<<<
					if(a3>=g_Condition2 && g_Condition2!=0){
						m_BadImg[i][j]=2;
						m_BadImg_Save[I_count][i][j]=2; //20151224 3D 배열화
						result=false;
					}
				}

				if(a3>g_BackupCond2[g_BackupRear]){  //검사 최고값
					g_BackupCond2[g_BackupRear] = a3;
				}
			}	
		}
	}
	
	if(g_Condition2==0)
		return true;

	if(!result)
	{
		if(b_TestMode == true)//20140508 테스트 모드 카운트 증가 방지 LIM
		{
			m_CountCond2++;
		}
		return false;
	}
	else
		return true;
}


bool CIntellisenseView::Detect_Cond33(int I_count)
{
	int i, j;
	bool result=true;
	int t=3;
	int m_iTemp[8];
	int a1,a2,a3,a4=0;
	int min0=255, min1=255, mink=-1;


	for(i=0; i<8; i++)
		m_iTemp[i]=0;
	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{
		for(j=g_Margin;j<g_Width-g_Margin;j++) 
	//	for(j=iv_11;j<iv_22;j++) 
		{
			if( (g_Img_Bufx2[I_count][i][j]<160) && (g_Img_Bufx2[I_count][i-t][j]<245) && (g_Img_Bufx2[I_count][i-t][j+t]<245)  //kds201001 XIS-5000 (260)
				&& (g_Img_Bufx2[I_count][i][j+t]<245) && (g_Img_Bufx2[I_count][i+t][j+t]<245) 
				&& (g_Img_Bufx2[I_count][i+t][j]<245) && (g_Img_Bufx2[I_count][i+t][j-t]<245)
				&& (g_Img_Bufx2[I_count][i][j-t]<245) && (g_Img_Bufx2[I_count][i-t][j-t]<245) )
			{ 
				//a4=g_Img_Bufx2[I_count][i-2][j]+g_Img_Bufx2[I_count][i-2][j+2]+g_Img_Bufx2[I_count][i][j+2]+g_Img_Bufx2[I_count][i+2][j+2]+g_Img_Bufx2[I_count][i+2][j]+g_Img_Bufx2[I_count][i+2][j-2]+g_Img_Bufx2[I_count][i][j-2]+g_Img_Bufx2[I_count][i-2][j-2]+g_Img_Bufx2[I_count][i][j];
				a4=g_Img_Bufx2[I_count][i-1][j]+g_Img_Bufx2[I_count][i-1][j+1]+g_Img_Bufx2[I_count][i][j+1]+g_Img_Bufx2[I_count][i+1][j+1]+g_Img_Bufx2[I_count][i+1][j]+g_Img_Bufx2[I_count][i+1][j-1]+g_Img_Bufx2[I_count][i][j-1]+g_Img_Bufx2[I_count][i-1][j-1]+g_Img_Bufx2[I_count][i][j];
				a4 = (int)a4/9;
				m_iTemp[0] = g_Img_Bufx2[I_count][i-t][j]-a4;
				m_iTemp[1] = g_Img_Bufx2[I_count][i-t][j+t]-a4;
				m_iTemp[2] = g_Img_Bufx2[I_count][i][j+t]-a4;
				m_iTemp[3] = g_Img_Bufx2[I_count][i+t][j+t]-a4;
				m_iTemp[4] = g_Img_Bufx2[I_count][i+t][j]-a4;
				m_iTemp[5] = g_Img_Bufx2[I_count][i+t][j-t]-a4;
				m_iTemp[6] = g_Img_Bufx2[I_count][i][j-t]-a4;
				m_iTemp[7] = g_Img_Bufx2[I_count][i-t][j-t]-a4;

				a1 = int((m_iTemp[0]+m_iTemp[1]+m_iTemp[2]+m_iTemp[3]+m_iTemp[4]+m_iTemp[5]+m_iTemp[6]+m_iTemp[7])/8);

				a2 = 200-a4;
				a3 = int((a1*1.5)+((a2/10)*5));
 			//	if(i>g_Margin2+80){ //kds20140701
 			//		a3 = a3-10;
 			//	}

				min0=255, min1=255;

				if(a3>=g_Condition3-10)
				{
					//정렬 처리>>>>>올림차순
					//20141027
					/*
					int tmp=0;
					int max=0;
					for (int a=0; a < 7; a++){
						for (int b=a+1; b < 8; b++){
							if (m_iTemp[b] < m_iTemp[a]){
								tmp = m_iTemp[b];
								m_iTemp[b] = m_iTemp[a];
								m_iTemp[a] = tmp;
							}
						}
					}

					max = m_iTemp[1];
					*/
				    for(int k=0; k<8; k++)
					{
						if(min0>=m_iTemp[k])
						{
							min0=m_iTemp[k];
							mink=k;
						}
					}

					for(int k=0; k<8; k++)
					{
						if(min1>m_iTemp[k] && mink!=k)
							min1=m_iTemp[k];
					}

					a2 = 200-a4;
					a3 = int((min1*1.5)+((a2/10)*5));
 			//		if(i>g_Margin2+80){ //kds20140701
 			//			a3 = a3-10;
 			//		}

					//<<<<<<<<<<<<<<<<
					if(a3>=g_Condition3 && g_Condition3!=0){
						m_BadImg[i][j]=3;
						m_BadImg_Save[I_count][i][j]=3; //20151224 3D 배열화
						result=false;
					}
				}

				if(a3>g_BackupCond3[g_BackupRear])
				{  //검사 최고값
					g_BackupCond3[g_BackupRear] = a3;
				}
			}	
		}
	}
	if(g_Condition3==0)
		return true;

	if(!result)
	{
		if(b_TestMode == true)//20140508 테스트 모드 카운트 증가 방지 LIM
		{
			m_CountCond3++;
		}
		return false;
	}
	else
		return true;
}


bool CIntellisenseView::Detect_Cond44(int I_count)
{
	int i, j;
	bool result=true;

	int t=14;     //kds201001 XIS-5000
	int m_iTemp[8];
	int a1,a2,a3,a4=0;
	int min0=255, min1=255, mink=-1;
	for(i=0; i<8; i++)
		m_iTemp[i]=0;
	for(i=g_Margin+5;i<g_Height-g_Margin-5;i++)
	{
		for(j=g_Margin+5;j<g_Width-g_Margin-5;j++) 
	//	for(j=iv_11+3;j<iv_22-3;j++) 
		{
			if((g_Img_Bufx2[I_count][i][j]<160) && (g_Img_Bufx2[I_count][i-t][j]<220) && (g_Img_Bufx2[I_count][i-t][j+t]<220)  //kds201001 XIS-5000 (260)
				&& (g_Img_Bufx2[I_count][i][j+t]<220) && (g_Img_Bufx2[I_count][i+t][j+t]<220) 
				&& (g_Img_Bufx2[I_count][i+t][j]<220) && (g_Img_Bufx2[I_count][i+t][j-t]<220)
				&& (g_Img_Bufx2[I_count][i][j-t]<220) && (g_Img_Bufx2[I_count][i-t][j-t]<220) )
			{ 
				a4=g_Img_Bufx2[I_count][i-2][j]+g_Img_Bufx2[I_count][i-2][j+2]+g_Img_Bufx2[I_count][i][j+2]+g_Img_Bufx2[I_count][i+2][j+2]+g_Img_Bufx2[I_count][i+2][j]+g_Img_Bufx2[I_count][i+2][j-2]+g_Img_Bufx2[I_count][i][j-2]+g_Img_Bufx2[I_count][i-2][j-2]+g_Img_Bufx2[I_count][i][j];
				a4 = (int)a4/9;

				m_iTemp[0] = g_Img_Bufx2[I_count][i-t][j]-a4;
				m_iTemp[1] = g_Img_Bufx2[I_count][i-t][j+t]-a4;
				m_iTemp[2] = g_Img_Bufx2[I_count][i][j+t]-a4;
				m_iTemp[3] = g_Img_Bufx2[I_count][i+t][j+t]-a4;
				m_iTemp[4] = g_Img_Bufx2[I_count][i+t][j]-a4;
				m_iTemp[5] = g_Img_Bufx2[I_count][i+t][j-t]-a4;
				m_iTemp[6] = g_Img_Bufx2[I_count][i][j-t]-a4;
				m_iTemp[7] = g_Img_Bufx2[I_count][i-t][j-t]-a4;

				//a1 = int((m_iTemp[0]+m_iTemp[1]+m_iTemp[2]+m_iTemp[3]+m_iTemp[4]+m_iTemp[5])/6);/*+m_iTemp[6]+m_iTemp[7])/8*/
				a1 = int((m_iTemp[0]+m_iTemp[4])/2);//+m_iTemp[2]+m_iTemp[3]+m_iTemp[4]+m_iTemp[5])/6);/*+m_iTemp[6]+m_iTemp[7])/8*/
				a2 = 200-a4;
				a3 = int((a1*1.5)+((a2/10)*5));
 		//		if(i>g_Margin2+80){ //kds20140701
 		//			a3 = a3-10;
 		//		}

				min0=255, min1=255;

				if(a3>=g_Condition4-10)
				{
					//정렬 처리>>>>>
			/*		int tmp=0;
					int max=0;
					for (int a=0; a < 7; a++){
						for (int b=a+1; b < 8; b++){
							if (m_iTemp[b] < m_iTemp[a]){
								tmp = m_iTemp[b];
								m_iTemp[b] = m_iTemp[a];
								m_iTemp[a] = tmp;
							}
						}
					}
					max = m_iTemp[1]; //kds201001 XIS-5000
			*/
					for(int k=0; k<8; k++)
					{
						if(min0>=m_iTemp[k])
						{
							min0=m_iTemp[k];
							mink=k;
						}
					}

					for(int k=0; k<8; k++)
					{
						if(min1>m_iTemp[k] && mink!=k)
							min1=m_iTemp[k];
					}

					a2 = 200-a4;
					a3 = int((min1*1.5)+((a2/10)*5));
			//		if(i>g_Margin2+80){ //kds20140701
 			//			a3 = a3-10;
 				//	}
					//<<<<<<<<<<<<<<<<
					if(a3>=g_Condition4 && g_Condition4!=0)
					{
						m_BadImg[i][j]=4;
						m_BadImg_Save[I_count][i][j]=4; //20151224 3D 배열화
						result=false;
					}
				}

				if(a3>g_BackupCond4[g_BackupRear])
				{  //검사 최고값
					g_BackupCond4[g_BackupRear] = a3;
				}

			}
		}
	}


	if(g_Condition4==0)
		return true;

	if(!result)
	{
		if(b_TestMode == true)//20140508 테스트 모드 카운트 증가 방지 LIM
		{
			m_CountCond4++;
		}
		return false;
	}
	else
		return true;
}


bool CIntellisenseView::Detect_Cond55(int I_count)
{
		int i, j;
	bool result=true;

	int t=12;     //kds201001 XIS-5000
	int m_iTemp[8];
	int a1,a2,a3,a4=0;
	int min0=255, min1=255, mink=-1;
	for(i=0; i<8; i++)
		m_iTemp[i]=0;
	//>>>>>>>>>> 1번 검사
	//for(i=g_Margin;i<g_Height-g_Margin;i++)
	//for(i=g_Margin;i<120;i++)
	for(i=g_Margin+5;i<g_Margin+80;i++)//kds20140701
	{
		for(j=200;j<g_Width-200;j++) //kds20100417 g_Margin->100
	//	for(j=iv_11+3;j<iv_22-3;j++) 
		{
			if((g_Img_Bufx2[I_count][i][j]<170) && (g_Img_Bufx2[I_count][i-t][j]<260) && (g_Img_Bufx2[I_count][i-t][j+t]<260)  //kds201001 XIS-5000 (260)
				&& (g_Img_Bufx2[I_count][i][j+t]<260) && (g_Img_Bufx2[I_count][i+t][j+t]<260) 
				&& (g_Img_Bufx2[I_count][i+t][j]<260) && (g_Img_Bufx2[I_count][i+t][j-t]<260)
				&& (g_Img_Bufx2[I_count][i][j-t]<260) && (g_Img_Bufx2[I_count][i-t][j-t]<260) )
			{ 
				a4=g_Img_Bufx2[I_count][i-2][j]+g_Img_Bufx2[I_count][i-2][j+2]+g_Img_Bufx2[I_count][i][j+2]+g_Img_Bufx2[I_count][i+2][j+2]+g_Img_Bufx2[I_count][i+2][j]+g_Img_Bufx2[I_count][i+2][j-2]+g_Img_Bufx2[I_count][i][j-2]+g_Img_Bufx2[I_count][i-2][j-2]+g_Img_Bufx2[I_count][i][j];
				a4 = (int)a4/9;

				m_iTemp[0] = g_Img_Bufx2[I_count][i-t][j]-a4;
				m_iTemp[1] = g_Img_Bufx2[I_count][i-t][j+t]-a4;
				m_iTemp[2] = g_Img_Bufx2[I_count][i][j+t]-a4;
				m_iTemp[3] = g_Img_Bufx2[I_count][i+t][j+t]-a4;
				m_iTemp[4] = g_Img_Bufx2[I_count][i+t][j]-a4;
				m_iTemp[5] = g_Img_Bufx2[I_count][i+t][j-t]-a4;
				m_iTemp[6] = g_Img_Bufx2[I_count][i][j-t]-a4;
				m_iTemp[7] = g_Img_Bufx2[I_count][i-t][j-t]-a4;

				a1 = int((m_iTemp[0]+m_iTemp[1]+m_iTemp[2]+m_iTemp[3]+m_iTemp[4]+m_iTemp[5]+m_iTemp[6]+m_iTemp[7])/8);

				//a2 = 200-a4;
				//a3 = int((a1*1.5)+((a2/10)*5));
				a2 = 200-a4;
				//a3 = int((a1*3)+((a2/10)*2));
				a3 = int((a1*3)+((a2/3)*2));

				min0=255, min1=255;

				if(a3>=g_Condition5-10)
				{
					//20141027
					//정렬 처리>>>>>
				/*	int tmp=0;
					int max=0;
					for (int a=0; a < 7; a++){
						for (int b=a+1; b < 8; b++){
							if (m_iTemp[b] < m_iTemp[a]){
								tmp = m_iTemp[b];
								m_iTemp[b] = m_iTemp[a];
								m_iTemp[a] = tmp;
							}
						}
					}
					max = m_iTemp[1]; //kds201001 XIS-5000
				*/	//a2 = 200-a4;
					//a3 = int((max*1.5)+((a2/10)*5));
					for(int k=0; k<8; k++)
					{
						if(min0>=m_iTemp[k])
						{
							min0=m_iTemp[k];
							mink=k;
						}
					}

					for(int k=0; k<8; k++)
					{
						if(min1>m_iTemp[k] && mink!=k)
							min1=m_iTemp[k];
					}

					a2 = 200-a4;
					a3 = int((min1*3)+((a2/10)*2));
					//<<<<<<<<<<<<<<<<
					if(a3>=g_Condition5 && g_Condition5!=0)
					{
						m_BadImg[i][j]=5;
						m_BadImg_Save[I_count][i][j] = 5;
						result=false;
					}
				}			

				if(a3>g_BackupCond5[g_BackupRear])
				{  //검사 최고값
					g_BackupCond5[g_BackupRear] = a3;
				}

			}
		}
	}
    //<<<<<<<<<<<<< 1번 검사

	if(g_Condition5==0)
		return true;

	if(!result)
	{
		if(b_TestMode == true)//20140508 테스트 모드 카운트 증가 방지 LIM
		{
			m_CountCond5++;
		}
		return false;
	}
	else
		return true;
}


bool CIntellisenseView::Detect_Cond66(int I_count)
{
	int i, j;
	bool result=true;

	int t=15;
	int m_iTemp[8];
	int a1,a2,a3,a4=0;
	for(i=0; i<8; i++)
		m_iTemp[i]=0;
	//kds201003>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//>>>>>>>>>>>>> 2번 검사
	//for(i=g_Margin;i<120;i++)
	for(i=g_Margin+6;i<g_Margin+80;i++)//kds20140701
	{
		for(j=g_Margin+6;j<g_Width-g_Margin-6;j++) 
			//		for(j=iv_11+6;j<iv_22-6;j++) 
		{
			if((g_Img_Bufx2[I_count][i][j]<160) && (g_Img_Bufx2[I_count][i-t][j]<210) && (g_Img_Bufx2[I_count][i-t][j+t]<210)  //kds202101 XIS-5000 (210)
				&& (g_Img_Bufx2[I_count][i][j+t]<210) && (g_Img_Bufx2[I_count][i+t][j+t]<210) 
				&& (g_Img_Bufx2[I_count][i+t][j]<210) && (g_Img_Bufx2[I_count][i+t][j-t]<210)
				&& (g_Img_Bufx2[I_count][i][j-t]<210) && (g_Img_Bufx2[I_count][i-t][j-t]<210) )
			{ 
				a4=g_Img_Bufx2[I_count][i-2][j]+g_Img_Bufx2[I_count][i-2][j+2]+g_Img_Bufx2[I_count][i][j+2]+g_Img_Bufx2[I_count][i+2][j+2]+g_Img_Bufx2[I_count][i+2][j]+g_Img_Bufx2[I_count][i+2][j-2]+g_Img_Bufx2[I_count][i][j-2]+g_Img_Bufx2[I_count][i-2][j-2]+g_Img_Bufx2[I_count][i][j];
				a4 = (int)a4/9;
				m_iTemp[0] = g_Img_Bufx2[I_count][i-t][j]-a4;
				m_iTemp[1] = g_Img_Bufx2[I_count][i-t][j+t]-a4;
				m_iTemp[2] = g_Img_Bufx2[I_count][i][j+t]-a4;
				m_iTemp[3] = g_Img_Bufx2[I_count][i+t][j+t]-a4;
				m_iTemp[4] = g_Img_Bufx2[I_count][i+t][j]-a4;
				m_iTemp[5] = g_Img_Bufx2[I_count][i+t][j-t]-a4;
				m_iTemp[6] = g_Img_Bufx2[I_count][i][j-t]-a4;
				m_iTemp[7] = g_Img_Bufx2[I_count][i-t][j-t]-a4;

				a1 = int((m_iTemp[0]+m_iTemp[1]+m_iTemp[2]+m_iTemp[3]+m_iTemp[4]+m_iTemp[5]+m_iTemp[6]+m_iTemp[7])/8);

				a2 = 200-a4;
				a3 = int((a1*1.5)+((a2/10)*5));
				a3=a3-50;


				if(a3>=g_Condition6-30)
				{
					//20141027
					//정렬 처리>>>>>
					int tmp=0;
					int max=0;
					for (int a=0; a < 7; a++){
						for (int b=a+1; b < 8; b++){
							if (m_iTemp[b] < m_iTemp[a]){
								tmp = m_iTemp[b];
								m_iTemp[b] = m_iTemp[a];
								m_iTemp[a] = tmp;
							}
						}
					}
					max = m_iTemp[6];

					a2 = 200-a4;
					a3 = int((max*1.5)+((a2/10)*5));
					a3=a3-50;

					//<<<<<<<<<<<<<<<<
					if(a3>=g_Condition6 && g_Condition6!=0)  
					{
						m_BadImg6[i][j]=6;
						m_BadImg_Save[I_count][i][j] = 6;
						//m_BadImg[i][j]=7;
					}
				}			

				if(a3>g_BackupCond6[g_BackupRear])
				{  //검사 최고값
					g_BackupCond6[g_BackupRear] = a3;
				}

			}
		}
	}

	//<<<<<<<<<<<<< 2번 검사
	//>>>>>>>>>>>>>>>>>>>>>>>>>>> 라벨링 시작

	int m,n,top, BlobArea[1000];
	short curColor=0, r,c, area;
	int height=NSE_VAR_SIZE;
	int width=NSE_VAR_SIZE;
	// 스택으로 사용할 메모리 할당 
	short* stackx=new short [height*width];
	short* stacky=new short [height*width];

	// 라벨링된 픽셀을 저장하기 위해 메모리 할당 
	short *coloring = new short [height*width];
	for(i=0; i<height*width; i++) coloring[i]=0; //메모리 초기화 

	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{
		for(j=g_Margin;j<g_Width-g_Margin;j++) 
		{	
			// 이미 방문한 점이거나 픽셀값이 255가 아니라면 처리 안함 
			if(coloring[i*width+j]!=0 || m_BadImg6[i][j]!=6) continue;

			r=i; 
			c=j;
			top=0; area=1;
			curColor++;

			while(1) 
			{
GRASSFIRE:
				for(m=r-1; m<=r+1; m++)
				{
					for(n=c-1; n<=c+1; n++) 
					{
						// 관심 픽셀이 영상경계를 벗어나면 처리 안함  
						if(m<0 || m>=height || n<0 || n>=width) continue;

						if((int)m_BadImg6[m][n]==6 && coloring[m*width+n]==0)
						{
							coloring[m*width+n]=curColor;  // 현재 라벨로 마크 
							if(push(stackx,stacky,(short)m,(short)n,&top)==-1) continue;
							r=m; 
							c=n;
							area++;
							if(area>20){ //kds20100328
								m_BadImg[i][j]=6;
							}
							goto GRASSFIRE;
						}
					}
				}
				if(pop(stackx,stacky,&r,&c,&top)==-1) break;
			}
			if(curColor<1000) BlobArea[curColor] = area;
		}
	}

	// 영역의 면적 정보
	CString tempStr;
	int max=0;
	for(i=1; i<=curColor; i++) 
	{
		if(max<BlobArea[i]){
			max = BlobArea[i];
		}
	}
	if(max>20){ //kds20100328
		result=false;
		int m_imax6 = g_Condition6+max;
		if(m_imax6>250){
			m_imax6 = 250;
		}
		g_BackupCond6[g_BackupRear] = m_imax6;
	}else{
		g_BackupCond6[g_BackupRear] = g_Condition6-20;
	}

	delete []coloring; delete []stackx; delete []stacky;

	//CString aaa;
	//aaa.Format("%d - %d",g_BackupCond6[g_BackupRear],max);
	//AfxMessageBox(aaa);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<  라벨링 종료
	//kds201003<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	if(g_Condition6==0)
		return true;

	if(!result)
	{
		if(b_TestMode == true)//20140508 테스트 모드 카운트 증가 방지
		{
			m_CountCond6++;
		}
		return false;
	}
	else
		return true;
}


bool CIntellisenseView::Detect_Cond666(int I_count)
{
	int i, j;
	bool result=true;

	int t=8;
	int m_iArray = t*8;
	int m_iTemp[64];
	int a1,a2,a3,a4=0;
	for(i=0; i<m_iArray; i++)
		m_iTemp[i]=0;
	//20160707
	//for(i=g_Margin;i<g_Height-g_Margin;i++)
	for(i=g_Margin;i<g_Height-g_Margin;i++)
	{		
		//for(j=iv_1;j<iv_2;j++) 
		for(j=g_Margin;j<g_Width-g_Margin;j++) 
		{
			if((m_BadImg[i][j] == 10 && g_Img_Bufx2[I_count][i][j]<160) && (g_Img_Bufx2[I_count][i-t][j]<220) && (g_Img_Bufx2[I_count][i-t][j+t]<220) 
				&& (g_Img_Bufx2[I_count][i][j+t]<220) && (g_Img_Bufx2[I_count][i+t][j+t]<220) 
				&& (g_Img_Bufx2[I_count][i+t][j]<220) && (g_Img_Bufx2[I_count][i+t][j-t]<220)
				&& (g_Img_Bufx2[I_count][i][j-t]<220) && (g_Img_Bufx2[I_count][i-t][j-t]<220) )
			{ 
				a4=g_Img_Bufx2[I_count][i-2][j]+g_Img_Bufx2[I_count][i-2][j+2]+g_Img_Bufx2[I_count][i][j+2]+g_Img_Bufx2[I_count][i+2][j+2]+g_Img_Bufx2[I_count][i+2][j]+g_Img_Bufx2[I_count][i+2][j-2]+g_Img_Bufx2[I_count][i][j-2]+g_Img_Bufx2[I_count][i-2][j-2]+g_Img_Bufx2[I_count][i][j];
				a4 = (int)a4/9;

				m_iTemp[0] = g_Img_Bufx2[I_count][i-t][j]-a4;
				m_iTemp[1] = g_Img_Bufx2[I_count][i-t][j+t]-a4;
				m_iTemp[2] = g_Img_Bufx2[I_count][i][j+t]-a4;
				m_iTemp[3] = g_Img_Bufx2[I_count][i+t][j+t]-a4;
				m_iTemp[4] = g_Img_Bufx2[I_count][i+t][j]-a4;
				m_iTemp[5] = g_Img_Bufx2[I_count][i+t][j-t]-a4;
				m_iTemp[6] = g_Img_Bufx2[I_count][i][j-t]-a4;
				m_iTemp[7] = g_Img_Bufx2[I_count][i-t][j-t]-a4;

				a1 = int((m_iTemp[0]+m_iTemp[1]+m_iTemp[2]+m_iTemp[3]+m_iTemp[4]+m_iTemp[5]+m_iTemp[6]+m_iTemp[7])/8);
			//	a1 = int((m_iTemp[0]+m_iTemp[2]+m_iTemp[4]+m_iTemp[6]/4));

				a2 = 200-a4;
				a3 = int((a1*1.5)+((a2/10)*5));

				if(a3>=g_Condition6-10)
				{
					//정렬 처리>>>>>
					int tmp=0;
					int max=0;
					for (int a=0; a < 7; a++){
						for (int b=a+1; b < 8; b++){
							if (m_iTemp[b] < m_iTemp[a]){
								tmp = m_iTemp[b];
								m_iTemp[b] = m_iTemp[a];
								m_iTemp[a] = tmp;
							}
						}
					}
					max = m_iTemp[1]; //kds201001 XIS-5000
					a2 = 200-a4;
					a3 = int((max*1.5)+((a2/10)*5));
 
					//<<<<<<<<<<<<<<<<
					if(a3>=g_Condition6 && g_Condition6!=0)
					{
						m_BadImg[i][j]=6;
						m_BadImg_Save[I_count][i][j]=6;
						result=false;
					}
				}
			
				if(a3>g_BackupCond6[g_BackupRear])
				{  //검사 최고값
					g_BackupCond6[g_BackupRear] = a3;
				}

			}
		}
	}


	if(g_Condition6==0)
		return true;

	if(!result)
	{
		if(b_TestMode) //20140821
			m_CountCond6++;
		return false;
	}
	else
		return true;
}


void CIntellisenseView::Masking2(int I_Count)
{
	int m_w_size=0;
	int m_h_size=0;

	if(g_MaskBoxCondition5 == 0 || g_MaskBoxCondition6 == 0)
		return;

	m_w_size = g_left + ((g_right - g_left)/2);
	m_h_size = g_bottom + g_MaskBoxCondition4;

	for(int i=g_Margin; i< g_Height-g_Margin; i++)
	{
		for(int j=g_Margin; j<g_Width-g_Margin; j++)
		{
			if((((i-m_h_size)*(i-m_h_size))/(g_MaskBoxCondition5*g_MaskBoxCondition5)) + (((j-m_w_size)*(j-m_w_size))/(g_MaskBoxCondition6*g_MaskBoxCondition6)) == 10)  //(좌표 - 중심X)^2/(타원높이/3)^2 + (좌표y - 중심Y)^2/(타원가로길이/3)^2 
			{
				m_BadImg[i][j]=7;	
			}
			else if((((i-m_h_size)*(i-m_h_size))/(g_MaskBoxCondition5*g_MaskBoxCondition5)) + (((j-m_w_size)*(j-m_w_size))/(g_MaskBoxCondition6*g_MaskBoxCondition6)) < 10)  //(좌표 - 중심X)^2/(타원높이/3)^2 + (좌표y - 중심Y)^2/(타원가로길이/3)^2 
			{
				m_BadImg[i][j] = 10;
			}
		}
	}


}

//엔코더 감지기능 20181119
void CIntellisenseView::Encoder_State(int E_count)
{
	CRect rect;
	HBITMAP hBmp;
	BITMAP bi;

	if(E_count < 10)
	{
		hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BG_ENCODER_0), IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	}
	else
	{
		hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BG_ENCODER), IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	}
	
	CBitmap bmp;
	bmp.Attach(hBmp);
	CClientDC dc(this);
	CDC bmDC;
	bmDC.CreateCompatibleDC(&dc);
	CBitmap *pOldbmp = bmDC.SelectObject(&bmp);
	
	m_Encoder_S.GetWindowRect(&rect);

	ScreenToClient(&rect);
	bmp.GetBitmap(&bi);
	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &bmDC, 0, 0,bi.bmWidth, bi.bmHeight,SRCCOPY);
	bmDC.SelectObject(pOldbmp);
	bmDC.DeleteDC();
	

}


void CIntellisenseView::OnGetcount(void)
{
	g_Sum_Total_Count = 0;
	g_Sum_Ok_Count = 0;
	g_Sum_Ng_Count = 0;
	g_Sum_Omit_Count = 0;

	CString Temp_date;
	COleDateTime stime=COleDateTime::GetCurrentTime();
	Temp_date.Format(_T("%04d-%02d-%02d"), stime.GetYear(),stime.GetMonth(),stime.GetDay());	
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
		strSQL.Format(_T("SELECT * FROM intellisense_db1 WHERE strName = '%s' AND strDate = '%s'"), g_ProductName, m_strdate);
		CString strQuery;
		if (mysql_query(&m_mysql, CStringA(strSQL)))
		{
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}
		m_Res = mysql_store_result(&m_mysql);

		if ((row = mysql_fetch_row(m_Res)) != NULL)
		{
			int tmp = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;
			CString strTotal(row[2]);
			CString strOk(row[3]);
			CString strNg(row[4]);
			CString strOmit(row[5]);
			tmp3 = _wtoi(strTotal);
			tmp = _wtoi(strOk);
			tmp1 = _wtoi(strNg);
			tmp2 = _wtoi(strOmit);

			g_Sum_Total_Count = tmp3;
			g_Sum_Ok_Count = tmp;
			g_Sum_Ng_Count = tmp1;
			g_Sum_Omit_Count = tmp2;
		}

		mysql_free_result(m_Res);
	}

	Close_Mysql();

	//if(!m_pSet_1.Open_DB(g_StrDirPath[1]))		
	//{
	//	g_bOpenDB=false;
	//	//RecoveryDatabase(g_StrDirPath[1], g_StrDirPath_copy[1]);
	//	if(m_pSet_1.Open_DB(g_StrDirPath[1]))		
	//	g_bOpenDB=true;
	//}
	//	if(g_bOpenDB)
	//	{
	//		CString strSQL;
	//		strSQL.Format(_T("SELECT * FROM Product_Info WHERE strName = '%s' AND strDate = '%s'"), g_ProductName, Temp_date);
	//		CString strQuery;
	//		if(m_pSet_1.Select_DB(strSQL))
	//		{
	//			if(m_pSet_1.IsEOF())
	//			{
	//			}
	//			else
	//			{
	//				int tmp=0, tmp1=0, tmp2=0, tmp3=0;
	//				m_pSet_1.ChangeRsInt(_T("strOk"), tmp);
	//				m_pSet_1.ChangeRsInt(_T("strNg"), tmp1);
	//				m_pSet_1.ChangeRsInt(_T("strOmit"), tmp2);
	//				m_pSet_1.ChangeRsInt(_T("strTotal"), tmp3);
	//			
	//				g_Sum_Total_Count = tmp3;
	//				g_Sum_Ok_Count = tmp;
	//				g_Sum_Ng_Count = tmp1;
	//				g_Sum_Omit_Count = tmp2;
	//			}
	//		}
	//		if(m_pSet_1.b_IsOpenedDB)
	//			m_pSet_1.Close_DB();
	//	}

}

#include <locale.h>
void CIntellisenseView::Load_Info(void)
{
	CString temp;
	CStdioFile fileString;
	CString m_stime;
	CString Language=_T("0");
	CString hvalue;
	setlocale(LC_ALL, "Korean");
	if(fileString.Open(_T("D:\\Database\\Client.ini"),
		CStdioFile::modeRead | CStdioFile::modeNoTruncate | CStdioFile::shareDenyNone))
	{
		fileString.SeekToBegin();
		fileString.ReadString(temp);
		g_Adress = temp;
		
		fileString.ReadString(temp);
		g_Line = temp;

		fileString.ReadString(temp);
		m_stime = temp;
		////20251029 lim 개선 (컨베이어 높이 수동 조절)
		//g_SignalTime = _ttoi(m_stime);
		//fileString.ReadString(temp);
		//hvalue = temp;
		//g_Margin2 = _ttoi(hvalue);
		
	}
	fileString.Close();
}


void CIntellisenseView::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	_xoff_correct.Close();
	_xpiecewise_correct.Close();
	_xdisplay.Close();
	_xacquisition.Close();
	_xcommand.Close();
	_xsystem.Close();
	//_xpix_order.Close();		// reverse

	CFormView::OnClose();
}

//0324

void CIntellisenseView::Init_AI()//20260102 AI 추가
{
	//// TODO: 여기에 구현 코드 추가.
	//CT2CA pszConvertedAnsiString(g_ProductName);
	//std::string str(pszConvertedAnsiString);

	//

	//int result_initail = 0;

	//result_initail = e_GpuCla->initialAIModel(str); //onnx 파일이 없는 제품명이 들어갔을때 터짐. 에러 수정 요청 필요
	//
	//// result_initail -> 0 정상, 1 실패
	//if (result_initail == 1)
	//{
	//	CMessageBoxDlg mdlg;
	//	mdlg.m_Message = _T("AI 모델 초기화 실패\nAI검사 기능을 사용 할 수 없습니다. NSE로 문의 하시기 바랍니다.");
	//	mdlg.DoModal();
	//}

}

 //0324
bool CIntellisenseView::Detect_Cond7(int count) //20260102 AI 추가
{
	
	// TODO: 여기에 구현 코드 추가.
	int result = 0;
	//if (m_Aiflag == true)
	//{
	//	m_Aiflag = false;
	//	cv::Mat image(g_Height, g_Width, CV_8UC1, g_Img_Buf_AI);
	//	//result = e_GpuCla->inferenceAIModel1D(g_Img_Buf_AI, g_Height, g_Width, 0.5f);


	//	result = e_GpuCla->inferenceAIModel(image, 0.5f); //mat으로 사용 shin


	//	
	//	m_Aiflag = true;
	//}
	//if (result > 0)
	//{
	//	

	//	cv::Point p_orig = e_GpuCla->getCamCenterOrig();
	//	if (p_orig.x >= 0 && p_orig.x < g_Width &&p_orig.y >= 0 && p_orig.y < g_Height)
	//	{
	//		m_BadImg[p_orig.y][p_orig.x] = 10;
	//		m_BadImg_Save[count][p_orig.y][p_orig.x] = 10;
	//	}
	//	else
	//	{
	//		m_BadImg[1][1] = 10;
	//		m_BadImg_Save[count][1][1] = 10;
	//	}

	//	result = false;
	//}
	//else
	//	result = true;

	return result;
}



/*
BOOL CIntellisenseView::OpenLogFile()
{
	if (m_bLogOpened)
		return TRUE;

	CString path;
	path = _T("D:\\Crash.Log");

	if (!m_LogFile.Open(path, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		m_bLogOpened = FALSE;
		return FALSE;
	}

	m_LogFile.SeekToEnd();
	m_bLogOpened = TRUE;
	return TRUE;
}
void CIntellisenseView::WriteLog(const CString& str, int count)
{
	if (!m_bLogOpened)
	{
		if (!OpenLogFile())
			return;
	}

	// ms 포함 시간
	SYSTEMTIME st;
	GetLocalTime(&st);

	CString timeStr;
	timeStr.Format(_T("[%04d-%02d-%02d %02d:%02d:%02d.%03d] "),
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	CString finalStr;
	finalStr.Format(_T("[%06d] %s%s\r\n"), count, timeStr, str);

	m_LogFile.WriteString(finalStr);

	// m_LogFile.Flush(); // 필요시
}

void CIntellisenseView::CloseLogFile()
{
	if (m_bLogOpened)
	{
		m_LogFile.Flush();
		m_LogFile.Close();
		m_bLogOpened = FALSE;
	}
}
*/