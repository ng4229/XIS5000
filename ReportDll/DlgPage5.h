#pragma once

#include "../ReportDll/resource.h"
#include <chartdir.h>
#include "ChartDirector/mfcdemo/mfcdemo/ChartViewer.h"
// CDlgPage5 대화 상자입니다.

class CDlgPage5 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPage5)

public:
	CDlgPage5(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPage5();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE_5 };

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
	CEYH_ADO m_pSet_8;
	void InitList();
	//CMSChart	m_Chart5;
	COleDateTime m_Date5;
	CListCtrl m_List_F;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnDatetimechangeRdatetimepicker5(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void LoadBackground();
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
public:
	CChartViewer m_ChartViewer5;
};
