#pragma once


// CConditionOmitSetup_1 대화 상자입니다.

class CConditionOmitSetup_1 : public CDialogEx
{
	DECLARE_DYNAMIC(CConditionOmitSetup_1)

public:
	CConditionOmitSetup_1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConditionOmitSetup_1();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONDITIONOMITSETUP_1 };

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
	CBitmapButton m_Btcond3_Dn;
	CBitmapButton m_Btcond3_Up;
	int m_SetupMassUse;
	int m_SetupMassCond1;
	int m_SetupMassCond2;
	int m_SetupMassCond3;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedRusemass0();
	afx_msg void OnRusemass1();
	int m_DispOmitCond1;
	afx_msg void OnSetfocusDispmasscondition1();
	void ComputNum(void);
//	afx_msg void OnSetfocusDispleft();
//	afx_msg void OnSetfocusDispright();
	int m_DispOmitCond2;
	void DrawData(void);
};
