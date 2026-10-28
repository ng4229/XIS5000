#pragma once

#include "../ReportDll/resource.h"
// CInputDlg2 대화 상자입니다.

class CInputDlg2 : public CDialogEx
{
	DECLARE_DYNAMIC(CInputDlg2)

public:
	CInputDlg2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputDlg2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INPUTDIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Combo_1;
	int m_Combo_1_Index;
	COleDateTime m_Input1_Date;
	CString m_Input1_Time;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnBnClickedKeyboard();
};
