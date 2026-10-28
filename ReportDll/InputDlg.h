#pragma once


// CInputDlg 대화 상자입니다.
#include "resource.h"

class CInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INPUTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Combo_1;
	//20180309
	CEYH_ADO m_pSet_1;
	CEYH_ADO m_pSet_2;
	CEYH_ADO m_pSet_4;
	CEYH_ADO m_pSet_6;
	CEYH_ADO m_pSet_7;
	CEYH_ADO m_pSet_8;
	COleDateTime m_Input1_Date;
	CString m_Input1_Sn;
	CString m_Input1_Time;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_Combo_1_Index;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedKeyboard();
};
