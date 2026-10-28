#pragma once


// CInterockDlg 대화 상자입니다.

class CInterockDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInterockDlg)

public:
	CInterockDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInterockDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INTERLOCK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int What_Interock;
	void DrawInterock();
	CStatic m_PicInterock;
	virtual BOOL Create();
	virtual BOOL OnInitDialog();
//	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();

protected:
	CWnd* m_pParent;
	int m_nID;
};
