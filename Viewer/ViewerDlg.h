#pragma once


// CViewerDlg 대화 상자입니다.
#include "../Viewer/resource.h"
#include "afxwin.h"
#include <vector>
#include <string>

class AFX_EXT_CLASS CViewerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewerDlg)

public:
	CViewerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CViewerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEWERDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	CBrush m_bkbrush2;
	CBitmap m_BmpAllCheck;
	CBitmap m_BmpAllCheck_1;
	CBitmap m_bmpTabNg;
	CBitmap m_bmpTabNg_1;
	CBitmap m_bmpTabOmit;
	CBitmap m_bmpTabOmit_1;
	int m_NationalCode;
	CRect m_Picrect;
	CPoint m_FirstPoint;
	bool b_IsDragging;
	bool b_AllCheck;
	bool b_IsMakeArray;
	void ChangeFile();
	void ReleaseArray();
	void NewArray();

	bool GetXY();
	void DrawPicture();
	void LoadBackground();
	void InitList();
	void SearchListData();

	CPoint m_Mouse;
	int m_ZoomRate;
	int g_Width;
	int g_Height;

	unsigned char** m_BadImg;
	unsigned char** m_ImgBuffer;

	CString	m_ConditionInfo;
	bool b_AllChecked;

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

private:
	CPen pen1;
	CPen pen2;
	CPen pen3;
	CPen pen4;
	CPen pen5;
	CPen pen6;
	CPen pen10;

	CString m_StrLogName;
	CString m_StrFileName;
	CString m_StrDirectory;

	int m_ListNum;
	int m_SelectedTabNum;
	CString m_CurrentDate;
	CBrush Brush;
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;

	std::vector<CString> m_strName;
	int m_NameCount;
public:
	COleDateTime m_Date;
	CListCtrl m_CtrlList;
	CStatic m_Pic;
	int m_ShowPosition;

	CBitmapButton m_BtTabNg;
	CBitmapButton m_BtTabOmit;
	CBitmapButton m_BtExit;

	CEdit m_EditCondInfo;
	CEdit m_EditCondInfo2;
	CEdit m_EditCondInfo3;
	CEdit m_EditCondInfo4;
	CEdit m_EditCondInfo5;
	CEdit m_EditCondInfo6;
	afx_msg void OnDatetimechangeDatetimepicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedShowposition();
	afx_msg void OnShowposition1();
	afx_msg void OnBnClickedCopyusb();
	afx_msg void OnClickedAllcheck();
	afx_msg void OnBnClickedBtZoomin();
	afx_msg void OnBnClickedBtZoomout();
	afx_msg void OnClickedRadiotap();
	afx_msg void OnRadiotap1();
	afx_msg void OnItemchangedFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCopyreject();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();

	CBitmapButton m_BtAllCheck;
	CBitmapButton m_BtZoomIn;
	CBitmapButton m_BtZoomOut;
	CBitmapButton m_BtCopy;


	CString m_ProductName;
	void GetInfo(void);
	CString m_strNmae[10000];
	CBitmapButton m_Reject_Save;

	CBitmapButton m_ListUp; //20190611 Viewer 개선
	CBitmapButton m_ListDown; //20190611 Viewer 개선

	//20190819
	bool b_IsStart;

	//20200411 //20200410 테스트모드 뷰어 수정부분 11
	CBitmap m_bmpTabTest;
	CBitmap m_bmpTabTest_1;
	CBitmapButton m_BtTabTest;
	afx_msg void OnRadiotap2();

	//20200601
	bool m_bUseTestMode;
	//20200326
	afx_msg void OnBnClickedPlay();
	CString m_Fullname;
	CBitmapButton m_PlayReject;
	bool m_bVideo; 

public:
	std::vector<CString> tokenize_getlineCS(const CString data, const char chSep);
	std::vector<std::string> tokenize_getline(const std::string data, const char chSep);

	//HBITMAP Mat2DIB(const cv::Mat& img);
	void CreateBitmapInfo(int w, int h, int bpp);
	BITMAPINFO* m_pBitmapInfo;
};
