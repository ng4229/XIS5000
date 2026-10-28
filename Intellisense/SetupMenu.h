#pragma once


// CSetupMenu 대화 상자입니다.

class CSetupMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMenu)

public:
	CSetupMenu(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMenu();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MENU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void LoadBackground();

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CBitmapButton m_Bt_Admin;
	CBitmapButton m_Bt_Autosetup;
	CBitmapButton m_Bt_Condition;
	CBitmapButton m_Bt_Detector;
	CBitmapButton m_Bt_Login;
	CBitmapButton m_Bt_Mainparts;
	CBitmapButton m_Bt_Product;
	CBitmapButton m_Bt_Reject;
	CBitmapButton m_Bt_Report;
	CBitmapButton m_Bt_Save;
	CBitmapButton m_Bt_Xray;
	CBitmapButton m_Bt_Close;
	void LoadList(void);
	afx_msg void OnBnClickedBtLogout();
	afx_msg void OnBnClickedBtProduct();
	afx_msg void OnBnClickedBtCondition();
	afx_msg void OnBnClickedBtXray();
	afx_msg void OnBnClickedBtDetector();
	afx_msg void OnBnClickedBtReject();
	afx_msg void OnBnClickedBtSave();
	afx_msg void OnBnClickedBtAutosetup();
	afx_msg void OnBnClickedBtAdmin();
	afx_msg void OnBnClickedBtReport();
	afx_msg void OnBnClickedBtMainparts();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
