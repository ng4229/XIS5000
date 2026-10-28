#pragma once


// CReportTabCtrl

class CReportTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CReportTabCtrl)

public:
	CReportTabCtrl();
	virtual ~CReportTabCtrl();
	
	CDialog *m_tabPages[7];
	int m_tabCurrent;
	int m_nNumberOfPages;
	void Init();
	void SetRectangle();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


