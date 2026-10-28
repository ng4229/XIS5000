#pragma once


// CConditionMaskSetup_2 대화 상자입니다.

class CConditionMaskSetup_2 : public CDialogEx
{
	DECLARE_DYNAMIC(CConditionMaskSetup_2)

public:
	CConditionMaskSetup_2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConditionMaskSetup_2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONDITIONMASKSETUP_2 };

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
	CBitmapButton m_Btcond1_Dn;
	CBitmapButton m_Btcond1_Up;
	CBitmapButton m_Btcond2_Dn;
	CBitmapButton m_Btcond2_Up;
	int m_SetupMaskTalUse;
	int m_SetupMaskTalCond1;
	int m_SetupMaskTalCond2;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedRusemasktal0();
	afx_msg void OnRusemasktal1();
	void OnChangeSetupcondition(void);
};
