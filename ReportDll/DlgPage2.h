#pragma once

#include "../ReportDll/resource.h"
#include <chartdir.h>
#include "ChartDirector/mfcdemo/mfcdemo/ChartViewer.h"

// CDlgPage2 대화 상자입니다.

class CDlgPage2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPage2)

public:
	CDlgPage2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPage2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE_2 };

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
	CEYH_ADO m_pSet_3;
	CEYH_ADO m_pSet_4;
	void InitList();
	//CMSChart	m_Chart2;

	CListCtrl m_List_C;
	CString m_Page2_1;
	CString m_Page2_2;
	CString m_Page2_3;
	CString m_Page2_4;
	COleDateTime m_Date2;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnDatetimechangeRdatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void LoadBackground();
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
public:
	afx_msg void OnBnClickedInputdata();
	CBitmapButton m_BtInput;
	CChartViewer m_ChartViewer2;
};
