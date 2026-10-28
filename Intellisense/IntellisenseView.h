
// IntellisenseView.h : CIntellisenseView 클래스의 인터페이스
//

#pragma once

#include "resource.h"

#include "CtrlPanelView.h"
#include "imageobject.h"
#include "comidaqctrl2.h"
#include "afxwin.h"

#include "ComiEncoderMng.h"
#include "Deep.h"

#define WM_EVENT			WM_USER+1
#define WM_ERROR			WM_USER+2
#define WM_FRAME_READY		WM_USER+3

#define WMU_BASE				(WM_APP + 100)
#define WMU_EVENT_C01B			(WMU_BASE + 0)
#define WMU_ERROR_C01B			(WMU_BASE + 1)
#define WMU_FRAME_READY_C01B	(WMU_BASE + 2)
#define WMU_ANALYSIS_C01B		(WMU_BASE + 3)

class CIntellisenseDoc;

class CIntellisenseView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CIntellisenseView();
	DECLARE_DYNCREATE(CIntellisenseView)

public:
	enum{ IDD = IDD_INTELLISENSE_FORM };

// 특성입니다.
public:
	CIntellisenseDoc* GetDocument() const;

// C01B
public:
	std::vector<std::string> vPsz;

	//XSystem* _xsystem_;
	//XCommand* _xcommand_;
	//XAcquisition* _xacquisition_;
	//XFrameTransfer* _xtransfer_;
	//XDisplay* _xdisplay_;
	////XOffCorrect* _xoff_correct_;
	//XGigFactory* _xfactory_;
	//XPiecewiseCorrect* _xpiecewise_correct_;

	// config
	CString				_mac_str;
	CIPAddressCtrl		_ip_control;
	UINT				_cmd_port;
	UINT				_img_port;
	UINT				_line_number;
	int					_color_mode;
	int					_xgcu_type;
	int					_card_type;
	int					_op_mode;
	int					_pixel_depth;
	UCHAR				_card_num1;
	UCHAR				_card_num2;
	UCHAR				_card_num3;
	UCHAR				_card_num4;
	UCHAR				_card_num5;
	int					_binning_mode;
	int					_dm_pixel_num;

	int					_energy_mode;

	// operate
	float				_gamma;
	UINT				_gain_target;
	CString				_file_name;
	UINT				_flash_index;

	BOOL				_en_gf_correct;
	BOOL				_en_baseline_correct;
	BOOL				_en_gf_correct_offboard;
	UINT				_baseline_value;
	BOOL				_en_line_trigger;
	int					_line_trigger_mode;
	BYTE				_line_fine_delay;
	BYTE				_line_raw_delay;
	BOOL				_en_frame_trigger;
	int					_frame_trigger_mode;
	BYTE				_frame_delay;
	int					_average_filter;
	int					_sum_filter;

	XSystem			_xsystem;
	XCommand		_xcommand;
	XAcquisition	_xacquisition;
	XFrameTransfer	_xtransfer;
	XDisplay		_xdisplay;
	XOffCorrect* _xoff_correct_;
	XOffCorrect		_xoff_correct;
	XOffHLCorrect	_xoff_hl_correct;
	XGigFactory		_xfactory;
	XAnalyze		_xanalyze;
	XDualEngCorrect	_xdual_correct;
	XDualLineCorrect	_xdual_line_correct;

	XPiecewiseCorrect	_xpiecewise_correct;

	uint32_t		_frame_counter;
	uint32_t		_line_lost_num;
	XImage* _cur_image_;

	BOOL			_is_non_hilo;
	uint32_t		_is_dual_line_lcs;
	BOOL			_en_offboard_correct;
	BOOL			_en_piecewise_correct;

	// EnableLineInfo Use
	bool b_UseLineInfo;
private:
	// C01B Detector LostLine EventLog
	// line lost burst detection
	ULONGLONG _ll_window_start_ms = 0;
	ULONGLONG _ll_cooldown_until_ms = 0;
	uint32_t  _ll_window_lost_sum = 0;
	uint32_t  _ll_window_evt_cnt = 0;

	// 튜닝 파라미터
	static constexpr ULONGLONG WINDOW_MS = 1000;  // 1초
	static constexpr uint32_t  LOST_LINES_THRESH = 10;   // 1초 내 10라인 이상
	static constexpr uint32_t  EVENTS_THRESH = 5;     // 1초 내 5회 이상
	static constexpr ULONGLONG COOLDOWN_MS = 5000;  // 에러 후 5초 쿨다운
// 작업입니다.
public:
	int m_BPMCount[10];
	int m_BPMState;

	int g_OmitCondition3_Disp;
	int g_MassCondition1_Disp;
	int m_Histogram[256];
	void OnComputeXray(int I_Count);
	CBitmap m_bmpMaincond[6];
	void RejectON();
	void RejectON_NewIO();
	bool b_StopPaint;
	CEYH_ADO m_pSet_1;
	CEYH_ADO m_pSet_6;
	CEYH_ADO m_pSet_7;
	CEYH_ADO m_pSet_10;
	bool b_TestMode;
	bool b_AniState;
	SYSTEMTIME m_Rstm;
	CTime m_Rtime;

	void ChangeDetectorValue(void);
	void ChangeDetectorValueC01B(void);
	void OnDispCondActive(void);
	void OnWriteLog(int Num);
	void OnLoadValue(void);
	int b_IsNormalization;
	int	m_Count_Total;
	int	m_Count_Soup;
	int	m_Count_OK;
	int	m_Count_NG;

	int	m_Result_mode;
	int m_OmitAve;
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground(void);

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CIntellisenseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	CBrush Brush;
	CBrush m_bkbrush;
	CBrush m_bkbrush1;
	CBrush m_bkbrush2;
	CBrush *pOldBrush;
	CFont m_Font_Name;
	CTime m_CurrentTime;


// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//afx_msg void FrameReadyDtimage1(long NumLines);
	//afx_msg void DTErrorDtimage1(long ErrorID, LPCTSTR ErrorInfo);
	DECLARE_MESSAGE_MAP()

public:
	int m_IsProduct;
	int m_ReportOmit;
	int m_ReportNg;
	int m_ReportOk;
	int m_ReportTotal;
	CString m_strdate;
	int m_Height;
	int m_FrameCount;
	unsigned int **temp_Buf;
	unsigned int **center;
	unsigned int **m_BadImg;
	unsigned int **m_BadImg6;

	unsigned int ***m_BadImg_Save;
	unsigned char ***g_Buffer_Resize;





	short *stackx;
	short *stacky;
	unsigned char **m_Img_Buf;
	unsigned char **g_Buffer_Label;
	CPen pen;
	CPen pen1;
	CPen pen2;
	CPen pen3;
	CPen pen4;
	CPen pen5;
	CPen pen6;
	CPen pen10;
	void OnRecognition(int count);
	bool Detect_Cond1(int I_Count);
	bool Detect_Cond2(int I_Count);
	bool Detect_Cond3(int I_Count);
	bool Detect_Cond4(int I_Count);
	bool Detect_Cond5(int I_Count);
	bool Detect_Cond6(int I_Count);
	bool Detect_Omit(int I_Count);
	void Masking(int I_Count);
	void MaskingClip();
	void MaskingPaper();
	void MaskingTalOx(void);
	bool Labelling();
	int push(short *stackx, short *stacky, short vx, short vy, int *top);
	int pop(short *stackx, short *stacky, short *vx, short *vy, int *top);
	void DrawAnimation();
	void DrawAnimationClear();
	void DrawResult();
	void DrawResultProgress();
	void ClearResultProgress();
	void CleanCycleAlarm();
	int GetNtPrivilege();
	int GetOSVersionType();
	void InitArray();
	CString m_ConditionInfo[50];
	CString m_ConditionInfo1[50];
	CString m_ConditionInfo2[50];
	void OnConditionsetup(void);
	void OnCountresetbt(void);
	int OnCheckNormalization();
	bool b_ConveyorOn;
	void CheckDiskSpaceDE();
	void OnMenuStart(void);
	void OnMenuReStart(void);
	void OnMenuStop(void);
	void PostMessageToCtrlView(UINT MessageID, WPARAM wparam, LPARAM lparam);
	void Initial_Detector(void);
	void Initial_DetectorCB01(void);
	void MakeDirectory();
	void MakeSubDirectory();
	void MakeExternalDirectory();
	CString m_ProductName;
	void DrawXrayState(void);
	CStatic m_PicXrayState;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CEdit m_Edit_Time;
	CEdit m_Edit_Time3;
	void DisplayTime(void);
	CImageObject*	m_pImageObject;
	CDTCommander m_Commander;
	CDTDetector m_Detector;
	CDTDisplay m_Display;
	CDTImage m_DTImage;

	ULONG			m_ImgWidth;
	ULONG			m_ImgHeight;
	ULONG			m_BytesPerPixel;
	ULONG			m_ImgSize;
	BYTE* m_pImgBuf;		//For image processing	

	LRESULT	OnEventMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnErrorMsg(WPARAM wParam, LPARAM lParam);
	LRESULT	OnFrameReadyMsg(WPARAM wParam, LPARAM lParam);

	// C01B
	void RegisterSink();
	void GetSN();
	CWnd* GetDispWnd();
	void InitConfig();
	void InitOperate();

	LRESULT	OnFrameReadyMsgC01B(WPARAM wParam, LPARAM lParam);
	LRESULT OnErrorMsgC01B(WPARAM wParam, LPARAM lParam);
	LRESULT OnEventMsgC01B(WPARAM wParam, LPARAM lParam);
	LRESULT	OnAnalysisMsgC01B(WPARAM wParam, LPARAM lParam);

	bool SetC01BFrameLineNumber(UINT nLineNumber);



	DECLARE_EVENTSINK_MAP()
	afx_msg void OnBnClickedMenuExit();

public:
	void ReleaseArray(void);
	CBitmapButton m_Menu_ExitBt;

	CStatic m_GrabImage;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CStatic m_PicProgress;
	CEdit m_OmitaveEdit;
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	CString m_ProductName1;
	int m_CountCond1;
	int m_CountCond2;
	int m_CountCond3;
	int m_CountCond4;
	int m_CountCond5;
	int m_CountCond6;
	CStatic m_PicXrayAni;
	CStatic m_PicResult;
	void OnRecognition_Test(int count);
	CStatic m_PicCond1;
	CStatic m_PicCond2;
	CStatic m_PicCond3;
	CStatic m_PicCond4;
	CStatic m_PicCond5;
	CStatic m_PicProgress2;
	afx_msg void OnBnClickedBtload();
	CStatic m_PicCond6;
	void DisplayEAperMin(void);
	CEdit m_Edit_Eapermin;
	bool Detect_Openbox(void);
	void OnReSize(int I_Count);
	void OnThreadRecoginition(void);
	bool Dent_line(int I_count);
	void OnSaveImg(int I_count);
//	int m_Icount;
//	CComidaqctrl2 m_ComiDaq1;
	bool Detect_Cond22(int I_count);
	bool Detect_Cond33(int I_count);
	bool Detect_Cond44(int I_count);
	bool Detect_Cond55(int I_count);
	bool Detect_Cond66(int I_count);
	bool Detect_Cond666(int I_count);
	int m_InternalCount; //20190327
	void Masking2(int I_Count);
	void Encoder_State(int E_count); //엔코더 감지기능 20181119
	CStatic m_Encoder_S;
	void OnGetcount(void);

	


	int m_Count_Total2;
	int m_Count_OK2;
	int m_Count_NG2;
	int m_Count_Soup2;
	bool b_CheckOverwrite;
	int m_TempReportOk;
	int m_TempReportNg;
	int m_TempReportOmit;
	int m_TempReportTotal;


	void Load_Info(void);

	ComiEncoderMng m_comi;

	CString csDtErrorMsg;
	afx_msg void OnClose();
	
	//20260102 AI 추가
	//std::unique_ptr<NSE::AI::GpuCla> e_GpuCla; //0324

	void Init_AI();//0324
	bool Detect_Cond7(int count);
	bool m_Aiflag;
	//20260102 AI 추가



	//로그
	CStdioFile m_LogFile;
	BOOL m_bLogOpened = FALSE;

	//BOOL OpenLogFile();
	//void WriteLog(const CString& str, int count);

	//void CloseLogFile();





};

UINT ThreadAlarm(LPVOID lParam);
UINT ThreadPaint(LPVOID lParam);
UINT ThreadReject(LPVOID lParam);
UINT ThreadInspection(LPVOID lParam);
UINT ThreadRejectnew(LPVOID lParam);
UINT ThreadSave(LPVOID lParam);
UINT ThreadRecog(LPVOID lParam);
#ifndef _DEBUG  // IntellisenseView.cpp의 디버그 버전
inline CIntellisenseDoc* CIntellisenseView::GetDocument() const
   { return reinterpret_cast<CIntellisenseDoc*>(m_pDocument); }
#endif

