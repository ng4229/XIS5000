#pragma once


// CConditionOmitSetup_0 대화 상자입니다.

class CConditionOmitSetup_0 : public CDialogEx
{
	DECLARE_DYNAMIC(CConditionOmitSetup_0)

public:
	CConditionOmitSetup_0(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConditionOmitSetup_0();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONDITIONOMITSETUP_0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CBrush Brush;
	CBrush *pOldBrush;
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground();
	void LoadValue();

public:
	int m_SetupOmitCond1;
	int m_SetupOmitCond2;
	int m_SetupOmitCond3;
	int m_SetupOmitCond4;
	int m_SetupOmitCond5;
	int m_SetupOmitUse;
	CBitmapButton m_Btcond1_Up;
	CBitmapButton m_Btcond2_Up;
	CBitmapButton m_Btcond3_Up;
	CBitmapButton m_Btcond4_Up;
	CBitmapButton m_Btcond5_Up;
	CBitmapButton m_Btcond1_Dn;
	CBitmapButton m_Btcond2_Dn;
	CBitmapButton m_Btcond3_Dn;
	CBitmapButton m_Btcond4_Dn;
	CBitmapButton m_Btcond5_Dn;
	int m_DispOmitCond3;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedRuseomit0();
	afx_msg void OnRuseomit1();
	void ComputNum(int I_Count);
	afx_msg void OnSetfocusDispomitcondition3();
	BOOL m_DispArea;
	void Draw_Data(void);
	void OnChangeSetupcondition(void);
};
