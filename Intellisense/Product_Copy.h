#pragma once


// CProduct_Copy 대화 상자입니다.

class CProduct_Copy : public CDialogEx
{
	DECLARE_DYNAMIC(CProduct_Copy)

public:
	CProduct_Copy(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CProduct_Copy();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PRODUCT_COPY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Product_List;
	CBitmapButton m_Bt_Choose;
	CBitmapButton m_Bt_Close;
	void InitList();
	void LoadList();
	void LoadValue(CString product);
	void CopyProduct();
	CEYH_ADO m_pSet_p;
	CString temp_Product;
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CBitmap m_bmpMainBG;
	int m_nMainWidth; 
    int m_nMainHeight;
	void LoadBackground();
public:
	afx_msg void OnItemchangedListProduct(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
