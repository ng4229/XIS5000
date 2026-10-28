#pragma once


// CInputDlg1 대화 상자입니다.
#include "resource.h"

class CInputDlg1 : public CDialogEx
{
	DECLARE_DYNAMIC(CInputDlg1)

public:
	CInputDlg1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputDlg1();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INPUTDIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	COleDateTime m_Input2_Date;
	CString m_Input2_Time;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedKeyboard();
};
