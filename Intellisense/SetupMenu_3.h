#pragma once


// CSetupMenu_3 대화 상자입니다.

class CSetupMenu_3 : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMenu_3)

public:
	CSetupMenu_3(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMenu_3();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MENU_3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEYH_ADO m_pSet_7;
	CEYH_ADO m_pSet_p;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();

private:
	CBrush Brush;
	CBrush *pOldBrush;
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground();
public:
	CBitmapButton m_Bt_Save;
	CBitmapButton m_Bt_Close;
	void LoadValue(void);
	int m_SetupSaveFile;
	int m_SetupSaveHard;
	BOOL b_ApplyAll;
	CComboBox m_ComboD_1;
	CComboBox m_ComboE_1;
	CComboBox m_ComboD_2;
	CComboBox m_ComboE_2;
	BOOL m_DelOptionD;
	BOOL m_DelOptionE;
	int m_DelOptionD_1;
	int m_DelOptionD_2;
	int m_DelOptionE_2;
	int m_DelOptionE_1;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void OnWriteReport(void);
	void OnWriteReportAll(void);
	afx_msg void OnBnClickedBtSave();

	CComboBox m_ViewControl; //20171115 영상 강제 보정
	int m_ViewControl2; //20171115 영상 강제 보정

	BOOL m_accet; //20171130 영상 검사 적용


};
