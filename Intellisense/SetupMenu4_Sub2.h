#pragma once

//20200331
// CSetupMenu4_Sub2 대화 상자입니다.

class CSetupMenu4_Sub2 : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMenu4_Sub2)

public:
	CSetupMenu4_Sub2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMenu4_Sub2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MENU_4_SUB2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCheckSetup2();

	afx_msg void OnBnClickedCheckSetup3();
	afx_msg void OnBnClickedCheckSetup4();
	afx_msg void OnBnClickedCheckSetup5();
	afx_msg void OnBnClickedCheckSetup6();
	afx_msg void OnBnClickedCheckSetup7();
	afx_msg void OnBnClickedCheckSetup9();
	afx_msg void OnBnClickedCheckSetup10();
	void LoadBackground(void);
};
