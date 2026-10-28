#pragma once


// CSetupAutoXray 대화 상자입니다.

class CSetupAutoXray : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupAutoXray)

public:
	CSetupAutoXray(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupAutoXray();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_AUTOXRAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	void LoadBackground();

public:
	CEYH_ADO m_pSet_p;
	void OnAddCondition();
	CBitmapButton m_Bt_Save;
	CBitmapButton m_Bt_Close;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtSave();
};
