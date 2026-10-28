#pragma once


// CSetupMenu_0 대화 상자입니다.

class CSetupMenu_0 : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMenu_0)

public:
	CSetupMenu_0(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMenu_0();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MENU_0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	CEYH_ADO m_pSet_4;
	CEYH_ADO m_pSet_p;
	int m_SetupXraykv;
	int m_SetupXraykv2;
	int m_SetupXrayma;
	int m_SetupXrayma2;
	CBitmapButton m_Btkv_Up;
	CBitmapButton m_Btkv_Up2;
	CBitmapButton m_Btkv_Dn;
	CBitmapButton m_Btkv_Dn2;
	CBitmapButton m_Btma_Dn;
	CBitmapButton m_Btma_Dn2;
	CBitmapButton m_Btma_Up;
	CBitmapButton m_Btma_Up2;
	CBitmapButton m_Bt_Save;
	CBitmapButton m_Bt_Close;
	CBitmapButton m_BtXrayAuto;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground();
public:
	void LoadValue(void);
	afx_msg void OnBnClickedBtSave();
	void OnWriteReport(void);
	afx_msg void OnBnClickedBtXrayautosetup();

};
