#pragma once

#include "../ReportDll/resource.h"
// CDlgPage6 대화 상자입니다.


class CDlgPage6 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPage6)

public:
	CDlgPage6(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPage6();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE_6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
void LoadClean();
	void InitCombo();
	CString temp_name;
	void WriteReport();
	CEYH_ADO m_pSet_10;
	void InitList();
	CBitmapButton m_BtAdd;
	CBitmapButton m_BtDel;
	CBitmapButton m_BtSave;
	int m_Clean1;
	CComboBox m_Clean1_Day;
	CComboBox m_Clean1_Week;
	int m_Clean2;
	CComboBox m_Clean2_Day;
	CComboBox m_Clean2_Week;
	int m_Clean3;
	CComboBox m_Clean3_Day;
	CComboBox m_Clean3_Week;
	CListCtrl m_List_G;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnClickedBtAdd();
	afx_msg void OnClickedBtDel();
	afx_msg void OnClickedBtSave();
	afx_msg void OnClickedClean10();
	afx_msg void OnClean11();
	afx_msg void OnClean12();
	afx_msg void OnClean13();
	afx_msg void OnClickedClean20();
	afx_msg void OnClean21();
	afx_msg void OnClean22();
	afx_msg void OnClean23();
	afx_msg void OnClickedClean30();
	afx_msg void OnClean31();
	afx_msg void OnClean32();
	afx_msg void OnClean33();
	afx_msg void OnItemchangedListG(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void LoadBackground();
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
};
