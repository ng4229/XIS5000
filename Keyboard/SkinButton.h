#if !defined(AFX_SKINBUTTON_H__F430E083_5D7E_496B_8025_D02EA3FB5332__INCLUDED_)
#define AFX_SKINBUTTON_H__F430E083_5D7E_496B_8025_D02EA3FB5332__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkinButton window

class AFX_EXT_CLASS CSkinButton : public CButton
{
// Construction
public:
	CSkinButton();

// Attributes
protected:
	CBitmap* m_bmpDlgUp;
	CBitmap* m_bmpDlgFs;
	CBitmap* m_bmpDlgDn;

	INT_PTR	m_nXPos;
	INT_PTR m_nYPos;
	INT_PTR m_nWidth;
	INT_PTR m_nHeight;

	BOOL m_bIsPointed;
// Operations
public:
	void LoadBitmaps( CBitmap* dlgUp, CBitmap* dlgDn = NULL, CBitmap* dlgFs = NULL);
	void SizeToContent(INT_PTR x, INT_PTR y, INT_PTR nWidth, INT_PTR nHeight, BOOL bRedraw);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSkinButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINBUTTON_H__F430E083_5D7E_496B_8025_D02EA3FB5332__INCLUDED_)
