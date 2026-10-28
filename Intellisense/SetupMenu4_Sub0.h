#pragma once

//20200331
// CSetupMenu4_Sub0 대화 상자입니다.


class CSetupMenu4_Sub0 : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMenu4_Sub0)

public:
	CSetupMenu4_Sub0(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMenu4_Sub0();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MENU_4_SUB0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	CString m_NewPassword;
	CString m_OldPassword;
	BOOL b_CheckAutoLogout;

private:
	void LoadBackground();
public:
	afx_msg void OnClickedCheckAutologout();
	afx_msg void OnCbnSelchangeLangcombo();
	CComboBox m_National;
};
