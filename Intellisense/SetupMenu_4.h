#pragma once


// CSetupMenu_4 대화 상자입니다.

class CSetupMenu_4 : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMenu_4)

public:
	CSetupMenu_4(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMenu_4();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MENU_4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();

private:
	CBrush Brush;
	CBrush *pOldBrush;
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground();

public:
	CBitmapButton m_Bt_Close;
	CBitmapButton m_Bt_Save;
	CString m_NewPassword;
	CString m_OldPassword;
	afx_msg void OnBnClickedOk();
	bool b_IsChange;
	void OnWriteReport(void);
	CEYH_ADO m_pSet_7;
	int m_Language;
	bool b_ChangedLanguage;
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
