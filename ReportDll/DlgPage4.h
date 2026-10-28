#pragma once

#include "../ReportDll/resource.h"
#include <chartdir.h>
#include "ChartDirector/mfcdemo/mfcdemo/ChartViewer.h"
// CDlgPage4 대화 상자입니다.

class CDlgPage4 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPage4)

public:
int ErrorCount[31];
	int ChangeCount[31];
	void DrawChart1();
	void Refresh();
	CString m_SearchDate;
	void WriteReport();
	CEYH_ADO m_pSet_7;
	void InitList();
	void GetDiskSpace();
	void GetDriveList();
	void GetComputerInfo();
	CString DriverArray[10];
	CDlgPage4(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPage4();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE_4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List_E;
	CComboBox m_Combo_Drive;
	int m_Combo_Index;
	COleDateTime m_Date4;
	//CMSChart	m_Chart4;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnDatetimechangeRdatetimepicker4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangePage4Combo();

private:
	void LoadBackground();
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
public:
	CChartViewer m_ChartViewer4;
};
