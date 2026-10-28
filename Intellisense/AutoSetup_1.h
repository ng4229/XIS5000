#pragma once


// CAutoSetup_1 대화 상자입니다.

class CAutoSetup_1 : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoSetup_1)

public:
	CAutoSetup_1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoSetup_1();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AUTOSETUP_1 };

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
	int m_Pkv;
	int m_Pua;
	int m_Pscanspeed;
	CBitmapButton m_Btkv_Up;
	CBitmapButton m_Btkv_Dn;
	CBitmapButton m_Btscanspeed_Up;
	CBitmapButton m_Btscanspeed_Dn;
	CBitmapButton m_Btua_Dn;
	CBitmapButton m_Btua_Up;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
