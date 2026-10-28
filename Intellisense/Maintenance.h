#pragma once

// CMaintenance 대화 상자입니다.

class CMaintenance : public CDialogEx
{
	DECLARE_DYNAMIC(CMaintenance)

public:
	CMaintenance(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMaintenance();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAINTENANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	void OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	CString m_ErrorName;
	bool CheckXrayError(int Result);

private:
	int m_ImageCount;
	void LoadBackground();

public:
	void ClearPicGrid();
	CBitmapButton m_BtClose;
	afx_msg void OnBnClickedBtClose();
	afx_msg void OnBnClickedBtXrayStatus();
	afx_msg void OnBnClickedBtXrayReset();
	afx_msg void OnBnClickedBtXrayOnoff();
	afx_msg void OnBnClickedBtXrayWarmup();
	afx_msg void OnBnClickedBtDtNormal();
	afx_msg void OnBnClickedBtDtReset();
	afx_msg void OnBnClickedBtDtSavecheckimage();
	afx_msg void OnBnClickedBtDtSavenowimage();
	afx_msg void OnBnClickedBtIoConveyor();
	afx_msg void OnBnClickedBtIoReject();
	afx_msg void OnBnClickedBtIoYellowlamp();
	afx_msg void OnBnClickedBtIoGreenlamp();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL b_XrayOnOff;
	CString m_SavePath;
	int m_DispXraykV;
	int m_DispXrayuA;
	BOOL b_ConveyorOnOff;
	BOOL b_GreenlampOnOff;
	BOOL b_RejectOnOff;
	BOOL b_YellowlampOnOff;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void DrawImage(void);
	CStatic m_GrabImage;

	CStatic m_PicGraph;
	CListBox m_ListInfo;
	void ComputeNoise();
	bool m_Interlock;
	void OnNormalization(void);
	void OnTurnonXray(void);
	void OnTurnoffXray(void);
	CButton m_BtXray;
	void CheckDetectorCell(void);
	virtual BOOL DestroyWindow();
	bool b_XrayPower;
	afx_msg void OnBnClickedBtIoEncoder();
	afx_msg void OnBnClickedBtIoReject2();
	afx_msg void OnBnClickedBtIoReject3();
};

UINT ThreadDrawImage(LPVOID lParam);