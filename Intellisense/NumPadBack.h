#pragma once


// CNumPadBack 대화 상자입니다.

class CNumPadBack : public CDialogEx
{
	DECLARE_DYNAMIC(CNumPadBack)

public:
	int m_Min;
	int m_Max;
	CString m_Value;

	CNumPadBack(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNumPadBack();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NUMPAD_BACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void LoadBackgound(void);
	CStatic m_Bg_Picture;
};
