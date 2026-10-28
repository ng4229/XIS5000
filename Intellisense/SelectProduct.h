#pragma once


// CSelectProduct 대화 상자입니다.

class CSelectProduct : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectProduct)

public:
	CSelectProduct(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSelectProduct();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SELECT_PRODUCT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEYH_ADO m_pSet_7;
	CEYH_ADO m_pSet_p;
	void OnWriteReport();
	void LoadValue(CString product);
	CString temp_Product;
	void LoadList();
	void InitList();

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();

private:
	void LoadBackground(void);
public:

	CBitmapButton m_Bt_Add;
	CBitmapButton m_Bt_Del;
	CBitmapButton m_Bt_Mod;
	CBitmapButton m_Bt_Exit;
	CBitmapButton m_Bt_Choose;
	afx_msg void OnBnClickedAddProduct();
	afx_msg void OnBnClickedDelProduct();
	afx_msg void OnBnClickedModProduct();
	afx_msg void OnBnClickedOk();
	CListCtrl m_Product_List;
	afx_msg void OnItemchangedListProduct(NMHDR *pNMHDR, LRESULT *pResult);
	CBitmapButton m_BtAddimage;
	CBitmapButton m_BtDelimage;
	afx_msg void OnBnClickedBtaddimage();
	afx_msg void OnBnClickedBtdelimage();
	void DrawImage(CString path);
	CString m_Directory_1;

	CString m_ImagePath;
	CStatic m_Picture;
};
