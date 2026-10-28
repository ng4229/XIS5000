#pragma once


// CMessageBoxDlg 대화 상자입니다.

class CMessageBoxDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageBoxDlg)

public:
	int m_Mode;
	CString m_Message;

	CMessageBoxDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMessageBoxDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MESSAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_Bt_Ok;
	CBitmapButton m_Bt_Cancel;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CBrush Brush;
	CBrush *pOldBrush;
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackgound(void);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
