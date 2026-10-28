#pragma once


// CConditionMaskSetup_1 대화 상자입니다.

class CConditionMaskSetup_1 : public CDialogEx
{
	DECLARE_DYNAMIC(CConditionMaskSetup_1)

public:
	CConditionMaskSetup_1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConditionMaskSetup_1();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONDITIONMASKSETUP_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_Btcond1_Dn;
	CBitmapButton m_Btcond1_Up;
	CBitmapButton m_Btcond2_Dn;
	CBitmapButton m_Btcond2_Up;
	int m_SetupMaskClipUse;
	int m_SetupMaskClipDirection;
	int m_SetupMaskClipCond1;
	int m_SetupMaskClipCond2;
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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedRusemaskclip0();
	afx_msg void OnRusemaskclip1();
	afx_msg void OnClickedSetupmaskclipDirection0();
	afx_msg void OnSetupmaskclipDirection1();
	void OnChangeSetupcondition(void);
	afx_msg void OnDestroy();
};
