#pragma once
#include "afxwin.h"


// CConditionMaskSetup_0 대화 상자입니다.

class CConditionMaskSetup_0 : public CDialogEx
{
	DECLARE_DYNAMIC(CConditionMaskSetup_0)

public:
	CConditionMaskSetup_0(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConditionMaskSetup_0();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONDITIONMASKSETUP_0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CBitmapButton m_Btcond1_Dn;
	CBitmapButton m_Btcond1_Up;
	CBitmapButton m_Btcond2_Up;
	CBitmapButton m_Btcond2_Dn;
	CBitmapButton m_Btcond3_Up;
	CBitmapButton m_Btcond3_Dn;

	CBitmapButton m_Btcond4_Dn;
	CBitmapButton m_Btcond4_Up;
	CBitmapButton m_Btcond5_Up;
	CBitmapButton m_Btcond5_Dn;
	CBitmapButton m_Btcond6_Up;
	CBitmapButton m_Btcond6_Dn;

	int m_SetupMaskBoxUse;
	int m_SetupMaskBoxCond1;
	int m_SetupMaskBoxCond2;
	int m_SetupMaskBoxCond3;
	int m_SetupMaskBoxCond4;
	int m_SetupMaskBoxCond5;
	int m_SetupMaskBoxCond6;

private:
	CBrush Brush;
	CBrush *pOldBrush;
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground();
	void LoadValue();
public:
	afx_msg void OnClickedRusemaskbox0();
	afx_msg void OnRusemaskbox1();
	void OnChangeSetupcondition(void);
	void Draw_Data(void);
	int m_ShowMasking;
	afx_msg void OnBnClickedRusemaskbox2();
	afx_msg void OnBnClickedRusemaskbox3();
	CEdit m_SetMasking4;
	CEdit m_SetMasking5;
	CEdit m_SetMasking6;
};
