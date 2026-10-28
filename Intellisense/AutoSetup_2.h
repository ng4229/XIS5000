#pragma once


// CAutoSetup_2 대화 상자입니다.

class CAutoSetup_2 : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoSetup_2)

public:
	CAutoSetup_2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoSetup_2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AUTOSETUP_2 };

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
	int m_PIsNow;
	int m_PIsNowXray;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
};
