#pragma once
//20200331
#include "SetupMenu4_TabCtrl.h"
// CSetupMenu_4_Admin 대화 상자입니다.

class CSetupMenu_4_Admin : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMenu_4_Admin)

public:
	CSetupMenu_4_Admin(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMenu_4_Admin();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MENU_4_ADMIN };

private:
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	CBitmapButton m_Bt_Close;
	CBitmapButton m_Bt_Save;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//20200331
	CEYH_ADO m_pSet_7;
	CSetupMenu4_TabCtrl m_tabCtrl_Setup4;
	afx_msg void OnPaint();
	void OnWriteReport(void);
	CEYH_ADO m_pSet_p;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

};
