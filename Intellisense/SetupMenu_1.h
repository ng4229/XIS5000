#pragma once


// CSetupMenu_1 대화 상자입니다.

class CSetupMenu_1 : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMenu_1)

public:
	CSetupMenu_1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMenu_1();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MENU_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEYH_ADO m_pSet_6;
	CEYH_ADO m_pSet_p;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CBrush Brush;
	CBrush *pOldBrush;

	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground();
	
public:
	void LoadValue(void);
	CBitmapButton m_Bt_Save;
	CBitmapButton m_Bt_Close;
	CBitmapButton m_Btscanspeed_Up;
	CBitmapButton m_Btscanspeed_Dn;
	CBitmapButton m_Btconspeed_Up;
	CBitmapButton m_Btconspeed_Dn;
	int m_SetupScanspeed;
	int m_SetupConspeed;
	CBitmapButton m_Btvalueprepro_Dn;
	CBitmapButton m_Btvalueprepro_Up;
	int m_SetupPreproValue;
	int m_SetupPreproDisp;
	int m_SetupPreproUse;
	int Changmtocm(int value);
	afx_msg void OnBnClickedBtSave();
	void OnWriteReport(void);
	void OnWriteReport1(void);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedRuseprepro0();
	afx_msg void OnClickedRdispprepro0();
	afx_msg void OnRdispprepro1();
	afx_msg void OnRuseprepro1();
};
