#pragma once


// CAutoSetup 대화 상자입니다.

class CAutoSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoSetup)

public:
	CAutoSetup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoSetup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AUTOSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();

private:
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground(void);
public:
	int m_Step;
	CEYH_ADO m_pSet_p;
	CEYH_ADO m_pSet_7;
	CBitmapButton m_Bt_Next;
	CBitmapButton m_Bt_Close;
	CStatic m_Page;
	bool OnNameUsed(CString Pname);
	bool OnAddNewProduct(void);
	bool OnAddCondition(void);
	bool OnAddXray(void);
	void InitPage(void);
	CString m_OldProductName;
	int m_CurrentPage;
	CDialog *m_Pages[5];
	int m_nNumberOfPages;
	

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtNext();
};
