#pragma once


#include "IntellisenseView.h"
#include "GeneratorComm.h"
#include "InterockDlg.h"
#include "SpaceMsg.h"
// CCtrlPanelView 폼 뷰입니다.

class CCtrlPanelView : public CFormView
{
	DECLARE_DYNCREATE(CCtrlPanelView)

protected:
	CCtrlPanelView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CCtrlPanelView();

public:
	enum { IDD = IDD_CTRLPANNEL_FORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_PreName;
	CGeneratorComm *m_GComm;
	CBitmapButton m_Bt_Viewer;
	CBitmapButton m_Bt_Stop;
	CBitmapButton m_Bt_Start;
	CBitmapButton m_Bt_Setup;
	CBitmapButton m_Bt_Window;
	CBitmapButton m_Bt_ResetCount;
private:
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground(void);
public:
	bool b_HandDetect;
	CBrush m_bkbrush;
	CSpaceMsg *d_SpaceMsg;
	CInterockDlg d_InterockDlg;
	CEYH_ADO m_pSet_2;
	CEYH_ADO m_pSet_3;
	CEYH_ADO m_pSet_4;
	CEYH_ADO m_pSet_5;
	CEYH_ADO m_pSet_7;
	CEYH_ADO m_pSet_8;
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedMenuSetup();
	afx_msg void OnBnClickedMenuViewer();
	afx_msg void OnBnClickedMenuStart();
	afx_msg void OnBnClickedMenuStop();
	afx_msg void OnBnClickedMenuResetcount();
	afx_msg LRESULT OnEyhStop(WPARAM wparam, LPARAM lpara);
	virtual void OnInitialUpdate();
	void AutoCondition();
	void AutoXraySetup();
	void Initial_Delay(void);
	void Initial_IOCard(void);
	void OnWriteLog(int Num);
	void Initial_Monoblock(void);
	bool CheckXrayError(int Mode, int Result);
	void XrayWarmUp_Rtec(void);
	void OnTurnoffXray(void);
	void OnMenuExit(void);
	void OnTurnonXray(void);
	void XrayWarmUp(void);
	void CheckPartsUseTime(void);
	void SelectProduct(void);
	void OnLoadValue(void);
	void DisplayCount(int c1, int c2, int c3, int c4);
	void OnReadPassword(void);
	void AutoSetup(void);
	void WriteXrayLog(CString Message);
	void Checkhard(void);
	int CheckHardSpace(int num);
	CString CheckSpaceD(void);
	CString CheckSpaceE(void);
	void DrawXrayState(void);
	bool OnTurnonXrayState(void);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void OnNormalization(void);
	void OnNormalizationC01B(void);
	void OnTrunonXray_Auto(void);
	void OnTurnonXray_Low(void);
	void OnStartConveyor(void);
	void OnStopConveyor(void);
	void OnMenuReport(void);
	afx_msg void OnBnClickedMini();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void OnAutoTeaching(void);
	void OnMenuInterlock(void);
	void SendData(void);
	SYSTEMTIME m_Rstm;
	CString m_Comparedate;
	int	 m_Comparehour;
	int m_Comparemin;
	CString m_StrDate;
	int temp_total;
	int temp_ok;
	int temp_ng;
	int temp_omit;
};

UINT ThreadGenerator(LPVOID lParam);
UINT ThreadClient(LPVOID lParam); //20191202

