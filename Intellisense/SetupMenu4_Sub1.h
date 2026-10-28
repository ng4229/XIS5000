#pragma once
#include "afxwin.h"

//20200331
// CSetupMenu4_Sub1 대화 상자입니다.

class CSetupMenu4_Sub1 : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMenu4_Sub1)

public:
	CSetupMenu4_Sub1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMenu4_Sub1();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MENU_4_SUB1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool b_IsChangedLanguage;
	

	int m_Language;

private:
	void LoadBackground();
public:
	afx_msg void OnClickedRlanguage();
	afx_msg void OnRlanguage1();
	afx_msg void OnRlanguage2();
	CComboBox m_National;
	afx_msg void OnCbnSelchangeLangcombo();
	CBitmapButton m_List_Add;
	CBitmapButton m_List_Del;
	CBitmapButton m_List_Revise;;
	CListCtrl m_ListID;
	CEYH_ADO m_pSet_p;
	CString m_TempListID;
	CEYH_ADO m_pSet_7;
	afx_msg void OnBnClickedAddId();
	afx_msg void OnBnClickedReviceId();
	afx_msg void OnBnClickedDelId();
	void OnWrite_Log(int m_what);
	void Load_List(void);
	void Init_List(void);
	afx_msg void OnLvnItemchangedListId(NMHDR *pNMHDR, LRESULT *pResult);
	CString L_ID;
	CString L_PW;
	CString L_Level;
	CString Message1;
	CString Level_1;
	CString Level_2;
	CString Level_3;
};
