#pragma once
#include "afxcmn.h"
#include "resource.h"

//20200331
class CSetupMenu4_TabCtrl :	public CTabCtrl
{
public:
	CSetupMenu4_TabCtrl(void);
	~CSetupMenu4_TabCtrl(void);

	CDialog *m_tabPagesSub[3];
	int m_tabCurrent;
	int m_nNumberOfPages;

	void Init();
	void SetRectangle();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

};

