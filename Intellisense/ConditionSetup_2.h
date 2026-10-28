#pragma once


// CConditionSetup_2 대화 상자입니다.

class CConditionSetup_2 : public CDialogEx
{
	DECLARE_DYNAMIC(CConditionSetup_2)

public:
	CConditionSetup_2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConditionSetup_2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONDITIONSETUP_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

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
	CBitmapButton m_Btcond_Up;
	CBitmapButton m_Btcond_Dn;
	int m_SetupCondition3;
	CStatic m_PicCondition;
	void OnChangeSetupcondition();
};
