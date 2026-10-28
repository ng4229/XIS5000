#pragma once

// CConditionSetup_0 대화 상자입니다.

class CConditionSetup_0 : public CDialogEx
{
	DECLARE_DYNAMIC(CConditionSetup_0)

public:
	CConditionSetup_0(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConditionSetup_0();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONDITIONSETUP_0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	int m_SetupCondition1;
	CBitmapButton m_Btcond_Up;
	CBitmapButton m_Btcond_Dn;
	CStatic m_PicCondition;


private:
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	CPen pen;
	CPen pen1;
	void ClearPicGrid();
	void DrawConditionValue();
	void LoadBackground();
	void DrawData();

public:
	void OnChangeSetupcondition();
};
