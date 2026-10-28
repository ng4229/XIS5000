#pragma once

#include "../ReportDll/resource.h"

#include <chartdir.h>
#include "ChartDirector/mfcdemo/mfcdemo/ChartViewer.h"

// CDlgPage1 대화 상자입니다.

class CDlgPage1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPage1)

public:
	CDlgPage1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPage1();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	//CMSChart	m_Chart1;
	void DrawChart1();
	void Refresh();
	void WriteReport();
	void InitList();
	CEYH_ADO m_pSet_1;
	CEYH_ADO m_pSet_2;
	CString m_SearchDate;
	CListCtrl m_List_A;
	CListCtrl m_List_B;
	COleDateTime m_Date1;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnDatetimechangeRdatetimepicker(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void LoadBackground();
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
public:

	CChartViewer m_ChartViewer;
	CStatic m_Table;
};
