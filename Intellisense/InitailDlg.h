#pragma once


// CInitailDlg 대화 상자입니다.

class CInitailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInitailDlg)

public:
	CInitailDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInitailDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INITIAL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	bool b_IsUse;
	virtual BOOL OnInitDialog();
	CProgressCtrl m_Progress;
	virtual BOOL Create();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void StepIt(int value);
	void DisplayText(void);
	CString m_Text;

protected:
	CWnd* m_pParent;
	int m_nID;
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
