#pragma once


// CAdminDlg 대화 상자입니다.

class CAdminDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdminDlg)

public:
	CAdminDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAdminDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ADMIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CBitmapButton m_Bt_Admin0;
	CBitmapButton m_Bt_Admin1;
	CBitmapButton m_Bt_Admin2;
	CBitmapButton m_Bt_Admin3;
	CBitmapButton m_Bt_Admin4;
	CBitmapButton m_Bt_Admin5;
	CBitmapButton m_Bt_Admin6;
	CBitmapButton m_Bt_Admin7;
	CBitmapButton m_Bt_Admin8;
	CBitmapButton m_Bt_Admin9;
	CBitmapButton m_Bt_Reset;
	CBitmapButton m_Bt_Close;
	CString m_Password;
	int m_Mode;

private:
	void LoadBackground();

public:
	afx_msg void OnBnClickedBtAdmin1();
	afx_msg void OnBnClickedBtAdmin2();
	afx_msg void OnBnClickedBtAdmin3();
	afx_msg void OnBnClickedBtAdmin4();
	afx_msg void OnBnClickedBtAdmin5();
	afx_msg void OnBnClickedBtAdmin6();
	afx_msg void OnBnClickedBtAdmin7();
	afx_msg void OnBnClickedBtAdmin8();
	afx_msg void OnBnClickedBtAdmin9();
	afx_msg void OnBnClickedBtAdmin0();
	afx_msg void OnBnClickedBtReset();
	void OnCompare(void);
};
