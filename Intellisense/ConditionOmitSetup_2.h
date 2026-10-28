#pragma once


// CConditionOmitSetup_2 대화 상자입니다.

class CConditionOmitSetup_2 : public CDialogEx
{
	DECLARE_DYNAMIC(CConditionOmitSetup_2)

public:
	CConditionOmitSetup_2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConditionOmitSetup_2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONDITIONOMITSETUP_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

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
	int m_SetupNum;
	int m_SetupNoise;
	int m_SetupCountUse;
	afx_msg void OnClickedRusecount0();
	afx_msg void OnRusecount1();
};
