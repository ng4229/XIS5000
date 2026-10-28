#pragma once


// CModifyProductName 대화 상자입니다.

class CModifyProductName : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyProductName)

public:
	CModifyProductName(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CModifyProductName();
	CEYH_ADO m_pSet_p;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_MODIFY_PRODUCTNAME };
private:
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CBitmapButton m_Bt_Ok;
	CBitmapButton m_Bt_Cancel;
	afx_msg void OnBnClickedOk();
	CString m_NewName;
	CString m_OldName;
};
