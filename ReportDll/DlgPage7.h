#pragma once

#include "../ReportDll/resource.h"
// CDlgPage7 대화 상자입니다.

class CDlgPage7 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPage7)

public:
	CDlgPage7(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPage7();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE_7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	void InitList(void);
	CListCtrl m_Product_List;
	void LoadList(void);
	CEYH_ADO m_pSet_p;
	afx_msg void OnItemchangedListProduct(NMHDR* pNMHDR, LRESULT* pResult);

private:
	void LoadBackground();
	CBitmap m_bmpMainBG;
	int m_nMainWidth;
	int m_nMainHeight;
};
