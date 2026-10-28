#pragma once

#include "ConditionTabCtrl.h"
// CConditionSetup 대화 상자입니다.

class CConditionSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CConditionSetup)

public:
	CConditionSetup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConditionSetup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONDITIONSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CBrush Brush;
	CBrush *pOldBrush;
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground();

public:
	CEYH_ADO m_pSet_p;
	int m_Kindtab;
	CConditionTabCtrl m_tabCtrl;
	CBitmapButton m_Bt_Save;
	CBitmapButton m_Bt_Close;
	int m_RKindCondition;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedRkindcondition0();
	afx_msg void OnRkindcondition1();
	afx_msg void OnRkindcondition2();
	CBitmapButton m_BtKind0;
	CBitmapButton m_BtKind1;
	CBitmapButton m_BtKind2;
	CBitmap m_bmpTab0;
	CBitmap m_bmpTab0_0;
	CBitmap m_bmpTab1;
	CBitmap m_bmpTab1_0;
	CBitmap m_bmpTab2;
	CBitmap m_bmpTab2_0;
	afx_msg void OnBnClickedBtSave();
	BOOL m_OffInspection;
	afx_msg void OnClickedCheck1();
	afx_msg void OnBnClickedCancel();
};
