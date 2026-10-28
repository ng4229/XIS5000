#pragma once


// CSpaceMsg 대화 상자입니다.

class CSpaceMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CSpaceMsg)

public:
	CSpaceMsg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSpaceMsg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPACE_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create();
	CStatic m_Pic;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	void DrawPic(void);

protected:
	CWnd* m_pParent;
	int m_nID;
};
