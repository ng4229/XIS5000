#pragma once


// CReportDlg 대화 상자입니다.
#include "../ReportDll/resource.h"
#include "ReportTabCtrl.h"

class AFX_EXT_CLASS CReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReportDlg)

public:
	CReportDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CReportDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_REPORTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CReportTabCtrl m_tabCtrl;
	CBitmapButton m_BtExcel;
	CBitmapButton m_BtClose;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	void ReportDlg1(CString strDate);

private:
	void LoadBackground();
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;

public:
	afx_msg void OnBnClickedBtExcel();
	void Init_Mysql(void);
};
