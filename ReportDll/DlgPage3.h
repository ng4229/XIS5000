#pragma once

#include "resource.h"
#include <chartdir.h>
#include "ChartDirector/mfcdemo/mfcdemo/ChartViewer.h"
// CDlgPage3 대화 상자입니다.

class CDlgPage3 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPage3)

public:
	CDlgPage3(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPage3();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE_3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int ErrorCount[31];
	int ChangeCount[31];
	void DrawChart1();
	void Refresh();
	CString m_SearchDate;
	void WriteReport();
	CEYH_ADO m_pSet_5;
	CEYH_ADO m_pSet_6;
	void InitList();
	//CMSChart	m_Chart3;

	CListCtrl m_List_D;
	CString m_Page3_3;
	CString m_Page3_4;
	COleDateTime m_Date3;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnDatetimechangeRdatetimepicker3(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void LoadBackground();
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
public:
	afx_msg void OnBnClickedInputdata1();
	CBitmapButton m_BtInput1;
	CChartViewer m_ChartViewer3;
};
