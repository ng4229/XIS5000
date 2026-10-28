#pragma once


// CExitDlg 대화 상자입니다.

class CExitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExitDlg)

public:
	CExitDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CExitDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EXIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtTurnoff();
	afx_msg void OnBnClickedBtExitprogram();

private:
	void LoadBackground();

public:
//	afx_msg void OnPaint();
	afx_msg void OnBnClickedCancel();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBitmapButton m_BtMenu2;
	CBitmapButton m_BtMenu1;
	CBitmapButton m_BtMenu3;
};
