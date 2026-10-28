#pragma once


// CSetupAuto 대화 상자입니다.

class CSetupAuto : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupAuto)

public:
	CSetupAuto(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupAuto();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_AUTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEYH_ADO m_pSet_p;
	void OnAddCondition();
	void ShowResult();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtSave();
	afx_msg void OnBnClickedCancel();

private:
	void LoadBackground();

public:
	CBitmapButton m_Bt_Save;
	CBitmapButton m_Bt_Close;
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
