// SelectProduct.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "SelectProduct.h"
#include "afxdialogex.h"

#include "MessageBoxDlg.h"

#include "ImageProcessing.h"

// CSelectProduct 대화 상자입니다.
IMPLEMENT_DYNAMIC(CSelectProduct, CDialogEx)

CSelectProduct::CSelectProduct(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectProduct::IDD, pParent)
{
	temp_Product=_T("");
	m_Directory_1=_T("D:\\Database\\image\\");
	m_ImagePath=_T("");
}

CSelectProduct::~CSelectProduct()
{
}

void CSelectProduct::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD_PRODUCT, m_Bt_Add);
	DDX_Control(pDX, IDC_DEL_PRODUCT, m_Bt_Del);
	DDX_Control(pDX, IDC_MOD_PRODUCT, m_Bt_Mod);
	DDX_Control(pDX, IDCANCEL, m_Bt_Exit);
	DDX_Control(pDX, IDOK, m_Bt_Choose);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_Product_List);
	DDX_Control(pDX, IDC_BTADDIMAGE, m_BtAddimage);
	DDX_Control(pDX, IDC_BTDELIMAGE, m_BtDelimage);
	DDX_Control(pDX, IDC_PICIMAGE, m_Picture);
}


BEGIN_MESSAGE_MAP(CSelectProduct, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ADD_PRODUCT, &CSelectProduct::OnBnClickedAddProduct)
	ON_BN_CLICKED(IDC_DEL_PRODUCT, &CSelectProduct::OnBnClickedDelProduct)
	ON_BN_CLICKED(IDC_MOD_PRODUCT, &CSelectProduct::OnBnClickedModProduct)
	ON_BN_CLICKED(IDOK, &CSelectProduct::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PRODUCT, &CSelectProduct::OnItemchangedListProduct)
	ON_BN_CLICKED(IDC_BTADDIMAGE, &CSelectProduct::OnBnClickedBtaddimage)
	ON_BN_CLICKED(IDC_BTDELIMAGE, &CSelectProduct::OnBnClickedBtdelimage)
END_MESSAGE_MAP()


// CSelectProduct 메시지 처리기입니다.


BOOL CSelectProduct::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(255,255,255));
	
	LoadBackground();
	CenterWindow();
	InitList();
	LoadList();
	if(!g_bAdmin)
	{
		m_Bt_Del.ShowWindow(false);
		m_Bt_Mod.ShowWindow(false);
	}
	DrawImage(m_ImagePath);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSelectProduct::InitList()
{
	CRect rect;
    m_Product_List.GetClientRect(&rect);
	m_Product_List.InsertColumn(0, _T(""), LVCFMT_CENTER, 0);
	m_Product_List.InsertColumn(1, g_MessageArr[151], LVCFMT_CENTER, rect.Width()/4);
	m_Product_List.InsertColumn(2, g_MessageArr[152], LVCFMT_CENTER, rect.Width()/4*3-17);
	m_Product_List.InsertColumn(3, _T(""), LVCFMT_CENTER, 0);
	m_Product_List.InsertColumn(4, _T(""), LVCFMT_CENTER, 0);
	m_Product_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void CSelectProduct::LoadList()
{
	int index=0;
	CString oneColumn;

	MYSQL_RES* m_Res;
	MYSQL_ROW row;

	Init_Mysql();

	if (!g_Connecting)
		AfxMessageBox(_T("실패"));
	else
	{
		mysql_query(&m_mysql, "set names euckr");

		if (g_bAdmin)
		{
			SqlQuery.Format(_T("SELECT * FROM intellisense_product ORDER BY ProductName DESC"));

			if (mysql_query(&m_mysql, CStringA(SqlQuery)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
			else
			{
				m_Res = mysql_store_result(&m_mysql);

				m_Product_List.DeleteAllItems();
				while ((row = mysql_fetch_row(m_Res)) != NULL)
				{
					CString strName(row[0]);
					if (strName != _T(""))
					{
						// 새로운 행 등록
						oneColumn.Format(_T("%03d"), (index + 1));
						m_Product_List.InsertItem(index, _T(""));
						m_Product_List.SetItemText(index, 1, oneColumn);
						m_Product_List.SetItemText(index, 2, strName);

						CString strRegisterDate(row[61]);
						m_Product_List.SetItemText(index, 3, strRegisterDate);
						CString strLastDate(row[60]);
						m_Product_List.SetItemText(index, 4, strLastDate);

						index++;
					}
				}
				mysql_free_result(m_Res);
			}
		}
		else
		{
			SqlQuery.Format(_T("SELECT * FROM intellisense_product ORDER BY ProductName DESC"));

			if (mysql_query(&m_mysql, CStringA(SqlQuery)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
			else
			{
				m_Res = mysql_store_result(&m_mysql);
				m_Product_List.DeleteAllItems();
				while ((row = mysql_fetch_row(m_Res)) != NULL)
				{
					CString strName(row[0]);

					if (strName != _T("") && strName.Left(6) != _T("BYPASS"))
					{
						// 새로운 행 등록
						oneColumn.Format(_T("%03d"), (index + 1));
						m_Product_List.InsertItem(index, _T(""));
						m_Product_List.SetItemText(index, 1, oneColumn);
						m_Product_List.SetItemText(index, 2, strName);

						CString strRegisterDate(row[61]);
						m_Product_List.SetItemText(index, 3, strRegisterDate);
						CString strLastDate(row[60]);
						m_Product_List.SetItemText(index, 4, strLastDate);

						index++;
					}
				}
				mysql_free_result(m_Res);
			}
		}
	}

	Close_Mysql();

	//20131226
	//if(!m_pSet_p.Open_DB(g_StrDirPath[0]))		
	//{
	//}
	//else
	//{
	//	CString strSQL;
	//	strSQL.Format(_T("SELECT * FROM Product ORDER BY ProductName ASC"));
	//	if(m_pSet_p.Select_DB(strSQL))
	//	{
	//		m_Product_List.DeleteAllItems();
	//		while(m_pSet_p.IsEOF()!=1)
	//		{
	//			// 새로운 행 등록
	//			oneColumn.Format(_T("%03d"), (index+1));
	//			m_Product_List.InsertItem(index, _T("")); 
	//			m_Product_List.SetItemText(index, 1, oneColumn);
	//			CString tmp=_T("");
	//			m_pSet_p.ChangeRsChar(_T("ProductName"), tmp);
	//			m_Product_List.SetItemText(index, 2, tmp);
	//			m_pSet_p.ChangeRsChar(_T("ProductRegisterDate"), tmp);
	//			m_Product_List.SetItemText(index, 3, tmp);
	//			m_pSet_p.ChangeRsChar(_T("ProductLastDate"), tmp);
	//			m_Product_List.SetItemText(index, 4, tmp);
	//			m_pSet_p.MoveNext();	
	//			index++;
	//		}
	//	}
	//	if(m_pSet_p.b_IsOpenedDB)
	//	{
	//		m_pSet_p.Close_DB();
	//	}
	//}	

}

BOOL CSelectProduct::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSelectProduct::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	DrawImage(m_ImagePath);
}


void CSelectProduct::LoadBackground(void)
{
	int mainbgnum=0;
	if(g_NationalCode==0)
	{
		mainbgnum=IDB_BG_SELECTPRODUCT; 
		m_Bt_Add.LoadBitmaps(IDB_SELPRODUCT_ADD,IDB_SELPRODUCT_ADD_0); 
		m_Bt_Del.LoadBitmaps(IDB_SELPRODUCT_DEL,IDB_SELPRODUCT_DEL_0); 
		m_Bt_Choose.LoadBitmaps(IDB_SELPRODUCT_CHOOSE,IDB_SELPRODUCT_CHOOSE_0); 
		m_Bt_Exit.LoadBitmaps(IDB_SELPRODUCT_EXIT,IDB_SELPRODUCT_EXIT_0); 
		m_Bt_Mod.LoadBitmaps(IDB_SELECTPRODUCT_REVISE,IDB_SELECTPRODUCT_REVISE_1); 
	}
	else if(g_NationalCode==1)
	{
		mainbgnum=IDB_BG_SELECTPRODUCT_ENG;
		m_Bt_Add.LoadBitmaps(IDB_SELPRODUCT_ADD_ENG,IDB_SELPRODUCT_ADD_0_ENG); 
		m_Bt_Del.LoadBitmaps(IDB_SELPRODUCT_DEL_ENG,IDB_SELPRODUCT_DEL_0_ENG); 
		m_Bt_Choose.LoadBitmaps(IDB_SELPRODUCT_CHOOSE_ENG,IDB_SELPRODUCT_CHOOSE_0_ENG); 
		m_Bt_Exit.LoadBitmaps(IDB_SELPRODUCT_EXIT_ENG,IDB_SELPRODUCT_EXIT_0_ENG); 
		m_Bt_Mod.LoadBitmaps(IDB_SELECTPRODUCT_REVISE_ENG,IDB_SELECTPRODUCT_REVISE_1_ENG); 
	}
	else if(g_NationalCode==2)
	{
		mainbgnum=IDB_BG_SELECTPRODUCT_CHI;
		m_Bt_Add.LoadBitmaps(IDB_SELPRODUCT_ADD_CHI,IDB_SELPRODUCT_ADD_0_CHI); 
		m_Bt_Del.LoadBitmaps(IDB_SELPRODUCT_DEL_CHI,IDB_SELPRODUCT_DEL_0_CHI); 
		m_Bt_Choose.LoadBitmaps(IDB_SELPRODUCT_CHOOSE_CHI,IDB_SELPRODUCT_CHOOSE_0_CHI); 
		m_Bt_Exit.LoadBitmaps(IDB_SELPRODUCT_EXIT_CHI,IDB_SELPRODUCT_EXIT_0_CHI); 
		m_Bt_Mod.LoadBitmaps(IDB_SELECTPRODUCT_REVISE_CHI,IDB_SELECTPRODUCT_REVISE_1_CHI); 
	}
	else if(g_NationalCode==3)
	{
		mainbgnum=IDB_BG_SELECTPRODUCT_SPN;
		m_Bt_Add.LoadBitmaps(IDB_SELPRODUCT_ADD_SPN,IDB_SELPRODUCT_ADD_0_SPN); 
		m_Bt_Del.LoadBitmaps(IDB_SELPRODUCT_DEL_SPN,IDB_SELPRODUCT_DEL_0_SPN); 
		m_Bt_Choose.LoadBitmaps(IDB_SELPRODUCT_CHOOSE_SPN,IDB_SELPRODUCT_CHOOSE_0_SPN); 
		m_Bt_Exit.LoadBitmaps(IDB_SELPRODUCT_EXIT_SPN,IDB_SELPRODUCT_EXIT_0_SPN); 
		m_Bt_Mod.LoadBitmaps(IDB_SELECTPRODUCT_REVISE_SPN,IDB_SELECTPRODUCT_REVISE_1_SPN); 
	}

    SetBackgroundImage(mainbgnum, CDialogEx::BACKGR_TOPLEFT);
//	SetWindowPos(NULL, 672, 119, m_nMainWidth, m_nMainHeight, SWP_NOZORDER | SWP_NOMOVE); 
	
	m_Bt_Add.SizeToContent();
	m_Bt_Del.SizeToContent();
	m_Bt_Choose.SizeToContent();
	m_Bt_Exit.SizeToContent();
	m_Bt_Mod.SizeToContent();

	m_BtAddimage.LoadBitmaps(IDB_BTADDIMAGE,IDB_BTADDIMAGE_1); 
	m_BtAddimage.SizeToContent();
	m_BtDelimage.LoadBitmaps(IDB_BTDELIMAGE,IDB_BTDELIMAGE_1); 
	m_BtDelimage.SizeToContent();

	if(g_bAdmin)
	{
		m_BtAddimage.ShowWindow(true);
		m_BtDelimage.ShowWindow(true);
	}
	else
	{
		m_BtAddimage.ShowWindow(false);
		m_BtDelimage.ShowWindow(false);
	}
}


void CSelectProduct::OnBnClickedAddProduct()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(IDC_ADD_PRODUCT);
}


void CSelectProduct::OnBnClickedDelProduct()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(temp_Product=="")
		return;
	CMessageBoxDlg dlg;
	dlg.m_Message=g_MessageArr[153];//"삭제를 원하시면 '확인' 버튼을 눌러주세요. ";
	dlg.m_Mode=2;
	int result=dlg.DoModal();
	if(result==IDOK)
	{
		Init_Mysql();

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			CString strQuery;
			strQuery.Format(_T("Delete From Intellisense_product WHERE ProductName = '%s'"), temp_Product);
			if (mysql_query(&m_mysql, CStringA(strQuery))) {
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
			OnWriteReport();
		}

		Close_Mysql();

		/*if(m_pSet_p.Open_DB(g_StrDirPath[0]))		
		{
			CString strQuery;
			strQuery.Format(_T("Delete From Product WHERE ProductName = '%s'"), temp_Product);
			m_pSet_p.Delete_DB(strQuery);

			if(m_pSet_p.b_IsOpenedDB)
				m_pSet_p.Close_DB();

			OnWriteReport();
		}	*/

		OnBnClickedBtdelimage();

		if(temp_Product==g_ProductName)
			g_ProductName="";
		
		temp_Product="";
		LoadList();
	}
}

void CSelectProduct::OnBnClickedModProduct()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(temp_Product!="")
		g_ProductName=temp_Product;
	EndDialog(IDC_MOD_PRODUCT);
}


void CSelectProduct::OnBnClickedOk()
{
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (temp_Product == "")
	{
		AfxMessageBox(_T("제품이 선택되지 않았습니다."));
		return;
	}

	g_ProductName=temp_Product;
	LoadValue(g_ProductName);

	if(b_UseSmartView)
		WriteProductInfo(g_ProductName);

	Init_Mysql();

	if (!g_Connecting)
		AfxMessageBox(_T("실패"));
	else
	{
		mysql_query(&m_mysql, "set names euckr");

		CTime time;
		time = CTime::GetCurrentTime();
		CString strdate;
		CString strQuery;
		strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(), time.GetMonth(), time.GetDay());

		strQuery.Format(_T("Update intellisense_product SET ProductLastDate = '%s' WHERE ProductName = '%s'"), strdate, g_ProductName);
		if (mysql_query(&m_mysql, CStringA(strQuery)))
		{
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}
	}

	Close_Mysql();

	/*if(!m_pSet_p.Open_DB(g_StrDirPath[0]))		
	{
			
	}
	else
	{
		CTime time;
		time = CTime::GetCurrentTime();
		CString strdate;
		CString strQuery;
		strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());		
		strQuery.Format(_T("Update Product SET ProductLastDate = '%s' WHERE ProductName = '%s'"), strdate, g_ProductName);
		m_pSet_p.Update_DB(strQuery);

		if(m_pSet_p.b_IsOpenedDB)
			m_pSet_p.Close_DB();
	}*/

	CDialogEx::OnOK();
}


void CSelectProduct::OnItemchangedListProduct(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	int num=pNMLV->iItem;
	temp_Product=m_Product_List.GetItemText(num, 2);
	if(temp_Product!=_T(""))
	{
		m_ImagePath.Format(_T("%s%s.bmp"), m_Directory_1, temp_Product);
		DrawImage(m_ImagePath);

		GetDlgItem(IDC_EDIT_REGISTDAY)->SetWindowText(m_Product_List.GetItemText(num, 3));
		GetDlgItem(IDC_EDIT_LASTDAY)->SetWindowText(m_Product_List.GetItemText(num, 4));
	}
	*pResult = 0;
}


void CSelectProduct::LoadValue(CString product)
{

	Init_Mysql();

	if (!g_Connecting)
		AfxMessageBox(_T("실패"));
	else
	{
		CString SqlQuery;
		SqlQuery.Format(_T("SELECT * FROM intellisense_product WHERE ProductName = '%s'"), product);

		if (mysql_query(&m_mysql, CStringA(SqlQuery)))
		{
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}
		else
		{
			MYSQL_RES* m_Res;
			MYSQL_ROW row;

			m_Res = mysql_store_result(&m_mysql);

			row = mysql_fetch_row(m_Res);

			CString ProductCode(row[1]);
			g_Code = _wtoi(ProductCode);
			CString ProductSize(row[2]);
			g_Size = _wtoi(ProductSize);
			CString ProductXraykv(row[3]);
			g_Xray_kv = _wtoi(ProductXraykv);
			CString ProductXrayma(row[4]);
			g_Xray_ma = _wtoi(ProductXrayma);
			CString ProductDTscanspeed(row[5]);
			g_DTScanspeed = _wtoi(ProductDTscanspeed);
			CString ProductDTGain(row[6]);
			g_DTGain = _wtoi(ProductDTGain);
			CString ProductRejectUse(row[7]);
			g_RejectUse = _wtoi(ProductRejectUse);
			CString ProductRejectDelay(row[8]);
			g_RejectDelay = _wtoi(ProductRejectDelay);
			CString ProductRejectKeep(row[9]);
			g_RejectKeep = _wtoi(ProductRejectKeep);
			CString ProductConveyorSpeed(row[10]);
			g_ConveyorSpeed = _wtoi(ProductConveyorSpeed);
			CString ProductSaveFile(row[11]);
			g_SaveFile = _wtoi(ProductSaveFile);
			CString ProductSaveHard(row[12]);
			g_SaveHard = _wtoi(ProductSaveHard);
			CString ProductCond1(row[13]);
			g_Condition1 = _wtoi(ProductCond1);
			CString ProductCond2(row[14]);
			g_Condition2 = _wtoi(ProductCond2);
			CString ProductCond3(row[15]);
			g_Condition3 = _wtoi(ProductCond3);
			CString ProductCond4(row[16]);
			g_Condition4 = _wtoi(ProductCond4);
			CString ProductCond5(row[17]);
			g_Condition5 = _wtoi(ProductCond5);
			CString ProductCond6(row[18]);
			g_Condition6 = _wtoi(ProductCond6);
			CString ProductOmitCond1(row[19]);
			g_OmitCondition1 = _wtoi(ProductOmitCond1);
			CString ProductOmitCond2(row[20]);
			g_OmitCondition2 = _wtoi(ProductOmitCond2);
			CString ProductOmitCond3(row[21]);
			g_OmitCondition3 = _wtoi(ProductOmitCond3);
			CString ProductOmitCond4(row[22]);
			g_OmitCondition4 = _wtoi(ProductOmitCond4);
			CString ProductOmitCond5(row[23]);
			g_OmitCondition5 = _wtoi(ProductOmitCond5);
			CString ProductMaskx(row[24]);
			g_MaskBoxCondition1 = _wtoi(ProductMaskx);
			CString ProductMasky(row[25]);
			g_MaskBoxCondition2 = _wtoi(ProductMasky);
			CString ProductXraykv2(row[26]);
			g_Xray_kv2 = _wtoi(ProductXraykv2);
			CString ProductXrayma2(row[27]);
			g_Xray_ma2 = _wtoi(ProductXrayma2);
			CString ProductDelD(row[28]);
			int tempbool;
			tempbool = _wtoi(ProductDelD);
			if (tempbool == 1)
				g_bOptionD = true;
			else
				g_bOptionD = false;

			CString ProductDelD_1(row[29]);
			g_bOptionD_1 = _wtoi(ProductDelD_1);
			CString ProductDelD_2(row[30]);
			g_bOptionD_2 = _wtoi(ProductDelD_2);

			CString ProductDelE(row[31]);
			tempbool = _wtoi(ProductDelE);
			if (tempbool == 1)
				g_bOptionE = true;
			else
				g_bOptionE = false;

			CString ProductDelE_1(row[32]);
			g_bOptionE_1 = _wtoi(ProductDelE_1);
			CString ProductDelE_2(row[33]);
			g_bOptionE_2 = _wtoi(ProductDelE_2);

			CString ProductClipnum(row[34]);
			g_MaskClip_Direction = _wtoi(ProductClipnum);
			CString ProductClipgray(row[35]);
			g_MaskClip_Length = _wtoi(ProductClipgray);
			CString ProductClipdeep(row[36]);
			g_MaskClip_Deep = _wtoi(ProductClipdeep);
			CString ProductCountnum(row[37]);
			g_Countnum = _wtoi(ProductCountnum);
			CString ProductCountnoise(row[38]);
			g_Countnoise = _wtoi(ProductCountnoise);
			CString ProductEtc1(row[39]);
			g_PreproUse = _wtoi(ProductEtc1);
			CString ProductEtc2(row[40]);
			g_PreproValue = _wtoi(ProductEtc2);
			CString ProductEtc3(row[41]);
			g_PreproDisp = _wtoi(ProductEtc3);
			CString ProductEtc4(row[42]);
			g_PageMaskLR = _wtoi(ProductEtc4);
			CString ProductEtc5(row[43]);
			g_PageMaskUD = _wtoi(ProductEtc5);
			CString ProductEtc6(row[44]);
			g_SetRejectcount = _wtoi(ProductEtc6);
			CString ProductEtc7(row[45]);
			g_SetRejectrate = _wtoi(ProductEtc7);
			CString ProductEtc8(row[46]);
			g_MaskBoxCondition3 = _wtoi(ProductEtc8);
			CString ProductEtc9(row[47]);
			g_ShowMaskUse = _wtoi(ProductEtc9);
			CString ProductEtc10(row[48]);
			g_MaskBoxCondition4 = _wtoi(ProductEtc10);

			CString ProductUseOmit(row[49]);
			g_OmitCondUse = _wtoi(ProductUseOmit);
			CString ProductUseMass(row[50]);
			g_MassCondUse = _wtoi(ProductUseMass);
			CString ProductUseCount(row[51]);
			g_CountCondUse = _wtoi(ProductUseCount);
			CString ProductUseMask(row[52]);
			g_MaskBoxUse = _wtoi(ProductUseMask);
			CString ProductUseClip(row[53]);
			g_MaskClipUse = _wtoi(ProductUseClip);
			CString ProductUseTalox(row[54]);
			g_MaskTalUse = _wtoi(ProductUseTalox);
			CString ProductTalBright(row[55]);
			g_TalMask_Bright = _wtoi(ProductTalBright);
			CString ProductTalBright2(row[56]);
			g_TalMask_Bright2 = _wtoi(ProductTalBright2);
			CString ProductMassCond1(row[57]);
			g_MassCondition1 = _wtoi(ProductMassCond1);
			CString ProductMassCond2(row[58]);
			g_MassCondition2 = _wtoi(ProductMassCond2);
			CString ProductMassCond3(row[59]);
			g_MassCondition3 = _wtoi(ProductMassCond3);
			CString ProductLastDate(row[60]);
			CString ProductRegisterDate(row[61]);

			// etc16~ 추가
			CString ProductEtc11(row[62]);
			g_MaskBoxCondition5 = _wtoi(ProductEtc11);
			CString ProductEtc12(row[63]);
			g_MaskBoxCondition6 = _wtoi(ProductEtc12);
			CString ProductEtc13(row[64]);
			g_ViewControl = _wtoi(ProductEtc13);
			
			int tempbool2;
			CString ProductEtc14(row[65]);
			tempbool2 = _wtoi(ProductEtc14);
			if(tempbool2 == 1)
				g_Accept = true;
			else if(tempbool2 == 0)
				g_Accept = false;
				
			CString ProductEtc15(row[66]);
			CString ProductEtc16(row[67]);
			CString ProductEtc17(row[68]);
			CString ProductEtc18(row[69]);
			CString ProductEtc19(row[70]);
			CString ProductEtc20(row[71]);
			CString ProductEtc21(row[72]);
			CString ProductEtc22(row[73]);
			CString ProductEtc23(row[74]);
			CString ProductEtc24(row[75]);
			CString ProductEtc25(row[76]);

			mysql_free_result(m_Res);
		}
	}

	Close_Mysql();

	//20131226
	//if(!m_pSet_p.Open_DB(g_StrDirPath[0]))		
	//{

	//}
	//else
	//{
	//
	//	CString strSQL;
	//	strSQL.Format(_T("SELECT * FROM Product WHERE ProductName = '%s'"), product);
	//
	//	if(m_pSet_p.Select_DB(strSQL))
	//	{

	//		if(m_pSet_p.IsEOF())
	//		{
	//			if(m_pSet_p.b_IsOpenedDB)
	//				m_pSet_p.Close_DB();
	//			return;

	//		}
	//		else
	//		{
	//		
	//			m_pSet_p.ChangeRsInt(_T("ProductXraykv"), g_Xray_kv);
	//			m_pSet_p.ChangeRsInt(_T("ProductXrayma"), g_Xray_ma);
	//			m_pSet_p.ChangeRsInt(_T("ProductXraykv2"), g_Xray_kv2);
	//			m_pSet_p.ChangeRsInt(_T("ProductXrayma2"), g_Xray_ma2);
	//		
	//			m_pSet_p.ChangeRsInt(_T("ProductDTscanspeed"), g_DTScanspeed);
	//			m_pSet_p.ChangeRsFloat(_T("ProductDTGain"), g_DTGain);
	//			m_pSet_p.ChangeRsInt(_T("ProductRejecUse"), g_RejectUse);
	//			m_pSet_p.ChangeRsInt(_T("ProductRejectDelay"), g_RejectDelay);
	//			m_pSet_p.ChangeRsInt(_T("ProductRejectKeep"), g_RejectKeep);
	//			m_pSet_p.ChangeRsInt(_T("ProductCode"), g_Code);
	//			m_pSet_p.ChangeRsInt(_T("ProductSize"), g_Size);
	//			m_pSet_p.ChangeRsInt(_T("ProductConveyorSpeed"), g_ConveyorSpeed);
	//	
	//			m_pSet_p.ChangeRsInt(_T("ProductSaveFile"), g_SaveFile);
	//			m_pSet_p.ChangeRsInt(_T("ProductSaveHard"), g_SaveHard);
	//			m_pSet_p.ChangeRsInt(_T("ProductCond1"), g_Condition1);
	//			m_pSet_p.ChangeRsInt(_T("ProductCond2"), g_Condition2);
	//			m_pSet_p.ChangeRsInt(_T("ProductCond3"), g_Condition3);
	//			m_pSet_p.ChangeRsInt(_T("ProductCond4"), g_Condition4);
	//			m_pSet_p.ChangeRsInt(_T("ProductCond5"), g_Condition5);
	//			m_pSet_p.ChangeRsInt(_T("ProductCond6"), g_Condition6);
	//			m_pSet_p.ChangeRsInt(_T("ProductOmitCond1"), g_OmitCondition1);
	//			m_pSet_p.ChangeRsInt(_T("ProductOmitCond2"), g_OmitCondition2);
	//			m_pSet_p.ChangeRsInt(_T("ProductOmitCond3"), g_OmitCondition3);
	//			m_pSet_p.ChangeRsInt(_T("ProductOmitCond4"), g_OmitCondition4);
	//			m_pSet_p.ChangeRsInt(_T("ProductOmitCond5"), g_OmitCondition5);
	//			m_pSet_p.ChangeRsInt(_T("ProductMaskx"), g_MaskBoxCondition1);
	//			m_pSet_p.ChangeRsInt(_T("ProductMasky"), g_MaskBoxCondition2);
	//			m_pSet_p.ChangeRsInt(_T("ProductEtc8"), g_MaskBoxCondition3);
	//	
	//			int tempbool=0;
	//			m_pSet_p.ChangeRsInt(_T("ProductDelD"), tempbool);
	//			if(tempbool==1)
	//				g_bOptionD=true;
	//			else
	//				g_bOptionD=false;
	//			
	//			m_pSet_p.ChangeRsInt(_T("ProductDelD_1"), g_bOptionD_1);
	//		
	//			m_pSet_p.ChangeRsInt(_T("ProductDelD_2"), g_bOptionD_2);	
	//	
	//			m_pSet_p.ChangeRsInt(_T("ProductDelE"), tempbool);
	//			if(tempbool==1)
	//				g_bOptionE=true;
	//			else
	//				g_bOptionE=false;
	//	
	//			m_pSet_p.ChangeRsInt(_T("ProductDelE_1"), g_bOptionE_1);
	//			m_pSet_p.ChangeRsInt(_T("ProductDelE_2"), g_bOptionE_2);
	//		
	//			m_pSet_p.ChangeRsInt(_T("ProductClipnum"), g_MaskClip_Direction);
	//			m_pSet_p.ChangeRsInt(_T("ProductClipgray"), g_MaskClip_Length);
	//			m_pSet_p.ChangeRsInt(_T("ProductClipdeep"), g_MaskClip_Deep);
	//	
	//			m_pSet_p.ChangeRsInt(_T("ProductCountnum"), g_Countnum);
	//			m_pSet_p.ChangeRsInt(_T("ProductCountnoise"), g_Countnoise);

	//			//변수명 바꿔버리자
	//			m_pSet_p.ChangeRsInt(_T("ProductEtc1"), g_PreproUse);
	//			m_pSet_p.ChangeRsInt(_T("ProductEtc2"), g_PreproValue);
	//			m_pSet_p.ChangeRsInt(_T("ProductEtc3"), g_PreproDisp);
	//			m_pSet_p.ChangeRsInt(_T("ProductEtc4"), g_PageMaskLR);
	//			m_pSet_p.ChangeRsInt(_T("ProductEtc5"), g_PageMaskUD);
	//			m_pSet_p.ChangeRsInt(_T("ProductTalBright"), g_TalMask_Bright);
	//			m_pSet_p.ChangeRsInt(_T("ProductTalBright2"), g_TalMask_Bright2);
	//	
	//			m_pSet_p.ChangeRsInt(_T("ProductUseOmit"), g_OmitCondUse);
	//			m_pSet_p.ChangeRsInt(_T("ProductUseMass"), g_MassCondUse);
	//			m_pSet_p.ChangeRsInt(_T("ProductUseCount"), g_CountCondUse);
	//			m_pSet_p.ChangeRsInt(_T("ProductUseMask"), g_MaskBoxUse);
	//			m_pSet_p.ChangeRsInt(_T("ProductUseClip"), g_MaskClipUse);
	//			m_pSet_p.ChangeRsInt(_T("ProductUseTalox"), g_MaskTalUse);

	//			m_pSet_p.ChangeRsInt(_T("ProductMassCond1"), g_MassCondition1);
	//			m_pSet_p.ChangeRsInt(_T("ProductMassCond2"), g_MassCondition2);
	//			m_pSet_p.ChangeRsInt(_T("ProductMassCond3"), g_MassCondition3);	

	//			m_pSet_p.ChangeRsInt(_T("ProductEtc6"), g_SetRejectcount);
	//			m_pSet_p.ChangeRsInt(_T("ProductEtc7"), g_SetRejectrate);
	//			m_pSet_p.ChangeRsInt(_T("ProductEtc9"), g_ShowMaskUse);

	//			m_pSet_p.ChangeRsInt(_T("ProductEtc10"), g_MaskBoxCondition4);
	//			m_pSet_p.ChangeRsInt(_T("ProductEtc11"), g_MaskBoxCondition5);
	//			m_pSet_p.ChangeRsInt(_T("ProductEtc12"), g_MaskBoxCondition6);

	//			m_pSet_p.ChangeRsInt(_T("ProductEtc13"), g_ViewControl);
	//			int tempbool2 = 0;
	//			m_pSet_p.ChangeRsInt(_T("ProductEtc14"),tempbool2);
	//				if(tempbool2 == 1)
	//				g_Accept = true;
	//			else if(tempbool2 == 0)
	//				g_Accept = false;
	//			
	//		}
	//	}
	//	if(m_pSet_p.b_IsOpenedDB)
	//		m_pSet_p.Close_DB();
	//}
}


void CSelectProduct::OnWriteReport()
{
	CString strdate, strtime;
	CTime time;
	time = CTime::GetCurrentTime();
	strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());		
	strtime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), time.GetYear(),time.GetMonth(),time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

	CString strtext;
	strtext.Format(_T("%s(%s)"), g_MessageArr[154], temp_Product);

	bool g_bOpenDB=true;

	Init_Mysql();

	if (!g_Connecting)
		AfxMessageBox(_T("실패"));
	else
	{
		CString strQuery;
		strQuery.Format(_T("Insert INTO intellisense_db7(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
			strdate, strtext, strtime);
		if (mysql_query(&m_mysql, CStringA(strQuery)))
		{
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}
	}

	Close_Mysql();

	//if(!m_pSet_7.Open_DB(g_StrDirPath[7]))		
	//{	
	//	g_bOpenDB=false;
	//	//RecoveryDatabase(g_StrDirPath[7], g_StrDirPath_copy[7]);
	//	if(m_pSet_7.Open_DB(g_StrDirPath[7]))		
	//	{
	//		g_bOpenDB=true;
	//	}
	//}
	//if(g_bOpenDB)
	//{
	//	CString strQuery;
	//	strQuery.Format(_T("Insert INTO Computer_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
	//		strdate, strtext, strtime);
	//	m_pSet_7.Insert_DB(strQuery);
	//	if(m_pSet_7.b_IsOpenedDB)
	//		m_pSet_7.Close_DB();
	//}		
}

void CSelectProduct::OnBnClickedBtaddimage()
{
	if(temp_Product=="")
		return;

	CString m_strFilePass, filename, extension;
	CFileDialog dlgFile (TRUE); 
	LPTSTR filter = _T("(*.bmp, *.jpg)\0*.bmp;*.jpg");
	dlgFile.m_ofn.Flags = 0x0;
	dlgFile.m_ofn.Flags = OFN_EXPLORER | OFN_ENABLEHOOK;
	dlgFile.m_ofn.lpstrCustomFilter = NULL;
	dlgFile.m_ofn.nFilterIndex = 0;
	dlgFile.m_ofn.lpstrFilter = filter;	

	int user_choice = dlgFile.DoModal();
	if (user_choice == IDOK) 
	{
		m_strFilePass = dlgFile.GetPathName();
		filename = dlgFile.GetFileName();
		extension = dlgFile.GetFileExt();
		extension.MakeLower();
		
		CFileFind check;
		if(extension=="bmp")
		{
			m_ImagePath=m_Directory_1+temp_Product+_T(".bmp");
			CopyFile(m_strFilePass, m_ImagePath, false);
			DrawImage(m_ImagePath);
		}
		else if(extension=="jpg")
		{
			// cximage
			m_ImagePath=m_Directory_1+temp_Product+_T(".jpg");
			CopyFile(m_strFilePass, m_ImagePath, false);
			//CxImage ximg;
			//ximg.Load(m_ImagePath, CXIMAGE_FORMAT_JPG);
			//m_ImagePath.Replace(_T("jpg"), _T("bmp"));
			//if(ximg.IsValid())
			//{
			//	ximg.Save(m_ImagePath, CXIMAGE_FORMAT_BMP);
			//}
			cv::Mat mImg;
			NSE::Load(mImg, m_ImagePath);
			m_ImagePath.Replace(_T("jpg"), _T("bmp"));
			if (!mImg.empty())
			{
				NSE::Save(mImg, m_ImagePath);
			}
			CString delpath;
			delpath=m_Directory_1+temp_Product+_T(".jpg");
			if(check.FindFile(delpath))
				DeleteFile(delpath);
			DrawImage(m_ImagePath);
		}
	}		
}


void CSelectProduct::OnBnClickedBtdelimage()
{
	if(temp_Product=="")
		return;

	CString path;
	path=m_Directory_1+temp_Product+_T(".bmp");
	CFileFind finder;
	if(finder.FindFile(path))
	{
		DeleteFile(path);
	}
	DrawImage(m_ImagePath);

}

void CSelectProduct::DrawImage(CString path)
{
	CRect rect;
	HBITMAP hBmp;
	BITMAP  bi;
	CFileFind finder;

	CString FileType = path.Right(3);
	CBitmap bmp;
	if(finder.FindFile(path) && path!=_T(""))
	{
		if(FileType=="bmp" || FileType=="BMP")
		{
			hBmp = (HBITMAP)LoadImage(NULL, path,
									IMAGE_BITMAP, 0, 0, 
									LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			bmp.Attach(hBmp);
		}
		else
		{
			bmp.LoadBitmapW(IDB_BGNOIMAGE);
		}
	}
	else
	{
		bmp.LoadBitmapW(IDB_BGNOIMAGE);
	}
	
	CClientDC dc(this);
	CDC bmDC;
	bmDC.CreateCompatibleDC(&dc);
	CBitmap *pOldbmp = bmDC.SelectObject(&bmp);
	m_Picture.GetWindowRect(&rect);
	ScreenToClient(&rect);
	bmp.GetBitmap(&bi);
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(rect.left+2, rect.top+2, rect.Width()-4, rect.Height()-4, &bmDC, 0, 0, bi.bmWidth, bi.bmHeight, SRCCOPY);
	bmDC.SelectObject(pOldbmp);
	bmp.DeleteObject();
	bmDC.DeleteDC();
}