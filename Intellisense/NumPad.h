#pragma once


// CNumPad 대화 상자입니다.

class CNumPad : public CDialogEx
{
	DECLARE_DYNAMIC(CNumPad)

public:
	int m_Min;
	int m_Max;
	CString m_Value;

	CNumPad(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNumPad();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NUMPAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void LoadBackgound();
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	CBitmapButton m_Bt_Num0;
	CBitmapButton m_Bt_Num1;
	CBitmapButton m_Bt_Num2;
	CBitmapButton m_Bt_Num3;
	CBitmapButton m_Bt_Num4;
	CBitmapButton m_Bt_Num5;
	CBitmapButton m_Bt_Num6;
	CBitmapButton m_Bt_Num7;
	CBitmapButton m_Bt_Num8;
	CBitmapButton m_Bt_Num9;
	CBitmapButton m_Bt_Reset;
	CBitmapButton m_Bt_Close;
	CBitmapButton m_Bt_Ok;
	afx_msg void OnBnClickedBtNum1();
	afx_msg void OnBnClickedBtNum2();
	afx_msg void OnBnClickedBtNum3();
	afx_msg void OnBnClickedBtNum4();
	afx_msg void OnBnClickedBtNum5();
	afx_msg void OnBnClickedBtNum6();
	afx_msg void OnBnClickedBtNum7();
	afx_msg void OnBnClickedBtNum8();
	afx_msg void OnBnClickedBtNum9();
	afx_msg void OnBnClickedBtNum0();
	afx_msg void OnBnClickedBtReset();
};
