#pragma once
#include "resource.h"

class CConditionTabCtrl : public CTabCtrl
{
public:
	CConditionTabCtrl(void);
	~CConditionTabCtrl(void);

	CDialog *m_tabPagesNg[6];
	//20160428
	CDialog *m_tabPagesOmit[2];
	CDialog *m_tabPagesMask[3];

	int m_Kindtab;
	int m_tabCurrent;
	int m_nNumberOfPagesNg;
	int m_nNumberOfPagesOmit;
	int m_nNumberOfPagesMask;
	void Init();
	void SetRectangle();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

