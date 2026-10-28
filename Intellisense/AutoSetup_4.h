#pragma once


// CAutoSetup_4 대화 상자입니다.

class CAutoSetup_4 : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoSetup_4)

public:
	CAutoSetup_4(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoSetup_4();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AUTOSETUP_4 };

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
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void ShowResult(void);
};
