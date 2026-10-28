#pragma once


// CSetupMenu_2 대화 상자입니다.

class CSetupMenu_2 : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMenu_2)

public:
	CSetupMenu_2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMenu_2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MENU_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();

private:
	CBrush Brush;
	CBrush *pOldBrush;

	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground();
public:
	CEYH_ADO m_pSet_p;
	CEYH_ADO m_pSet_8;
	CBitmapButton m_Bt_Save;
	CBitmapButton m_Bt_Close;
	CBitmapButton m_Btrejectdelay_Up;
	CBitmapButton m_Btrejectdelay_Dn;
	CBitmapButton m_Btrejectkeep_Dn;
	CBitmapButton m_Btrejectkeep_Up;
	int m_SetupRejectKeep;
	int m_SetupRejectDelay;
	void LoadValue(void);
	int m_SetupRejectUse;
	BOOL b_ApplyAll;
	afx_msg void OnBnClickedBtSave();
	void OnWriteReport(void);
	void OnWriteReportAll(void);
	afx_msg void OnClickedSetuprejectuse();
	afx_msg void OnSetuprejectuse1();
	afx_msg void OnSetuprejectuse2();
	afx_msg void OnClickedApplyall();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBitmapButton m_Bthandcycle_Up;
	CBitmapButton m_Bthandcycle_Dn;
	CBitmapButton m_Bthandcycle_Up2;
	CBitmapButton m_Bthandcycle_Dn2;
	int m_SetupHandCycle;
	int m_RejectRate;
};
