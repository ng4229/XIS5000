#pragma once


// CConditionSetup_3 대화 상자입니다.

class CConditionSetup_3 : public CDialogEx
{
	DECLARE_DYNAMIC(CConditionSetup_3)

public:
	CConditionSetup_3(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConditionSetup_3();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONDITIONSETUP_3 };

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
	int m_SetupCondition4;
	CStatic m_PicCondition;
	void OnChangeSetupcondition();
};
