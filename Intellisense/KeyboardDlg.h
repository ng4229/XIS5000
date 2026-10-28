#if !defined(AFX_KEYBOARDDLG_H__431E8584_674F_48E1_9B41_5F25856C9C98__INCLUDED_)
#define AFX_KEYBOARDDLG_H__431E8584_674F_48E1_9B41_5F25856C9C98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyboardDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyboardDlg dialog
#include "Resource.h"
#include "AfxTempl.h"
//#include "SkinButton.h"
#include "../Keyboard/SkinButton.h"
#include "imm.h"
#pragma comment(lib,"imm32.lib")


INT_PTR FAR PASCAL HookInit(HWND hMain);
INT_PTR FAR PASCAL HookOn();
INT_PTR FAR PASCAL HookOff();

struct KEYDEF
{
	char cNormal;
	char cShifted;
	INT_PTR cWidth;
};

class AFX_EXT_CLASS CKeyboardDlg : public CDialog
{
// Construction
public:
	bool b_Hanglepress;
	bool b_Shiftpress;
	bool b_Capitalpress;
	bool b_Ctrlpress;
	bool b_Altpress;
	bool b_IsType;
	INT_PTR m_Numchar;
	BYTE m_Char[1000];
	INT_PTR m_NationalCode;
	void LoadSkin();
	CBitmap m_bmpDlgUp;
	CBitmap m_bmpDlgDn;
	CBitmap m_bmpDlgFs;
	void LoadButton();
	CWnd* m_pTargetWnd; // ★추가★

	CSkinButton m_bt11;
	CSkinButton m_bt1;
	CSkinButton m_bt2;
	CSkinButton m_bt3;
	CSkinButton m_bt4;
	CSkinButton m_bt5;
	CSkinButton m_bt6;
	CSkinButton m_bt7;
	CSkinButton m_bt8;
	CSkinButton m_bt9;
	CSkinButton m_bt0;
	CSkinButton m_bt12;
	CSkinButton m_bt13;
	CSkinButton m_bt14;

	CSkinButton m_bt21;
	CSkinButton m_btq;
	CSkinButton m_btw;
	CSkinButton m_bte;
	CSkinButton m_btr;
	CSkinButton m_btt;
	CSkinButton m_bty;
	CSkinButton m_btu;
	CSkinButton m_bti;
	CSkinButton m_bto;
	CSkinButton m_btp;
	CSkinButton m_bt22;
	CSkinButton m_bt23;
	CSkinButton m_bt24;

	CSkinButton m_bt31;
	CSkinButton m_bta;
	CSkinButton m_bts;
	CSkinButton m_btd;
	CSkinButton m_btf;
	CSkinButton m_btg;
	CSkinButton m_bth;
	CSkinButton m_btj;
	CSkinButton m_btk;
	CSkinButton m_btl;
	CSkinButton m_bt32;
	CSkinButton m_bt33;
	CSkinButton m_bt34;

	CSkinButton m_bt41;
	CSkinButton m_btz;
	CSkinButton m_btx;
	CSkinButton m_btc;
	CSkinButton m_btv;
	CSkinButton m_btb;
	CSkinButton m_btn;
	CSkinButton m_btm;
	CSkinButton m_bt42;
	CSkinButton m_bt43;
	CSkinButton m_bt44;
	CSkinButton m_bt45;

	CSkinButton m_bt51;
	CSkinButton m_bt52;
	CSkinButton m_bt53;
	CSkinButton m_bt54;
	CSkinButton m_bt55;
	CSkinButton m_bt56;

	CKeyboardDlg(CWnd* pParent = NULL);   // standard constructor
	INT_PTR pKeyWidth;
	INT_PTR pKeyHeight;
	INT_PTR cCurrentKeyState;
	HWND gFocus;
	CTypedPtrList<CPtrList, KEYDEF*> cKeys;
	void AddKey(
		char pNormal,
		char pShifted,
		INT_PTR pWidth);
	void DrawKey(CDC * dc, CRect & rc, KEYDEF * key, BOOL cHilight=FALSE);
	void SendKey(KEYDEF * key);
	void DrawKeyboard();
	INT_PTR DescribeKeyState();
// Dialog Data
	//{{AFX_DATA(CKeyboardDlg)
	//enum { IDD = IDD_KEYBOARD };
	CBitmapButton	m_btCancel;
	CBitmapButton	m_btOk;
	CString	m_TestName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyboardDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeyboardDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LONG_PTR OnHookMsg(WPARAM wParam, LPARAM lParam);	//후킹 콜백 함수의 맵 원형정의
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void ClickButton(CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	void OnChangeTestname();
	bool m_Skinflag;
	int testtest;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDDLG_H__431E8584_674F_48E1_9B41_5F25856C9C98__INCLUDED_)
