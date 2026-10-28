#pragma once


// CAutoSetup_0 대화 상자입니다.

class CAutoSetup_0 : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoSetup_0)

public:
	CAutoSetup_0(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoSetup_0();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AUTOSETUP_0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	int m_Pcopy;
	CString m_Pname;
private:
	CBrush Brush;
	CBrush *pOldBrush;
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
