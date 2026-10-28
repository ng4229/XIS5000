#pragma once


// CSetupMenu4_Sub3 대화 상자입니다.

class CSetupMenu4_Sub3 : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMenu4_Sub3)

public:
	CSetupMenu4_Sub3(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMenu4_Sub3();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MENU_4_SUB3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void LoadBackground(void);
	CBitmapButton m_List_Save;
	CBitmapButton m_List_Close;
	CString m_TempID;
	CString m_NameID;
	CString m_NamePW;
	CEYH_ADO m_pSet_p;
	int m_Level;
	CEYH_ADO m_pSet_7;
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedClose();
	void OnWrite_Log(void);
	afx_msg void OnBnClickedRusemaskbox0();
	afx_msg void OnBnClickedRusemaskbox1();
	CString Message1;
	CString Message2;
	CString Message3;
//	afx_msg void OnXButtonDown(UINT nFlags, UINT nButton, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
