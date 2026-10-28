// Product_Copy.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "Product_Copy.h"
#include "afxdialogex.h"


// CProduct_Copy 대화 상자입니다.

IMPLEMENT_DYNAMIC(CProduct_Copy, CDialogEx)

CProduct_Copy::CProduct_Copy(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProduct_Copy::IDD, pParent)
{
	LoadBackground();
//	SetWindowPos(NULL, 320, 130, 0, 0, SWP_NOSIZE | SWP_NOZORDER);	
	CenterWindow();
	InitList();
	LoadList();
}

CProduct_Copy::~CProduct_Copy()
{
}

void CProduct_Copy::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_Product_List);
	DDX_Control(pDX, IDOK, m_Bt_Choose);
	DDX_Control(pDX, IDCANCEL, m_Bt_Close);
}


BEGIN_MESSAGE_MAP(CProduct_Copy, CDialogEx)
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PRODUCT, &CProduct_Copy::OnItemchangedListProduct)
	ON_BN_CLICKED(IDOK, &CProduct_Copy::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CProduct_Copy::OnBnClickedCancel)
END_MESSAGE_MAP()


// CProduct_Copy 메시지 처리기입니다.


void CProduct_Copy::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


BOOL CProduct_Copy::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadBackground();
//	SetWindowPos(NULL, 320, 130, 0, 0, SWP_NOSIZE | SWP_NOZORDER);	
	CenterWindow();
	InitList();
	LoadList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CProduct_Copy::LoadBackground()
{
	BITMAP bmpInfo;
	if(g_NationalCode==0)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_COPY); 
		m_Bt_Choose.LoadBitmaps(IDB_SELPRODUCT_CHOOSE,IDB_SELPRODUCT_CHOOSE_0); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT,IDB_SELPRODUCT_EXIT_0); 
	}
	else if(g_NationalCode==1)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_COPY_ENG); 
		m_Bt_Choose.LoadBitmaps(IDB_SELPRODUCT_CHOOSE_ENG,IDB_SELPRODUCT_CHOOSE_0_ENG); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_ENG,IDB_SELPRODUCT_EXIT_0_ENG); 
	}
	else if(g_NationalCode==2)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_COPY_CHI); 
		m_Bt_Choose.LoadBitmaps(IDB_SELPRODUCT_CHOOSE_CHI,IDB_SELPRODUCT_CHOOSE_0_CHI); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_CHI,IDB_SELPRODUCT_EXIT_0_CHI); 

	}
	else if(g_NationalCode==3)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_COPY_SPN); 
		m_Bt_Choose.LoadBitmaps(IDB_SELPRODUCT_CHOOSE_SPN,IDB_SELPRODUCT_CHOOSE_0_SPN); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_SPN,IDB_SELPRODUCT_EXIT_0_SPN); 

	}
    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 
	
	m_Bt_Choose.SizeToContent();
	m_Bt_Close.SizeToContent();
}


void CProduct_Copy::InitList()
{
	CRect rect;
    m_Product_List.GetClientRect(&rect);
	m_Product_List.InsertColumn(0, _T(""), LVCFMT_CENTER, 0);
	m_Product_List.InsertColumn(1, g_MessageArr[151], LVCFMT_CENTER, rect.Width()/4);
	m_Product_List.InsertColumn(2, g_MessageArr[152], LVCFMT_CENTER, rect.Width()/4*3-17);
	m_Product_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void CProduct_Copy::LoadList()
{
	int index=0;
	CString oneColumn;

	Init_Mysql();

	SqlQuery.Format(_T("SELECT * FROM intellisense_product ORDER BY ProductName DESC"));

	if (mysql_query(&m_mysql, CStringA(SqlQuery))) {
		CString error(mysql_error(&m_mysql));
		AfxMessageBox(error);
	}
	else
	{
		MYSQL_RES* m_Res;
		MYSQL_ROW row;

		m_Res = mysql_store_result(&m_mysql);

		m_Product_List.DeleteAllItems();
		while ((row = mysql_fetch_row(m_Res)) != NULL)
		{
			CString strName(row[0]);

			if (strName != "")
			{
				oneColumn.Format(_T("%03d"), (index + 1));
				m_Product_List.InsertItem(index, _T(""));
				m_Product_List.SetItemText(index, 1, oneColumn);
				m_Product_List.SetItemText(index, 2, strName);
				index++;
			}
		}
		mysql_free_result(m_Res);
	}

	Close_Mysql();

	//if(!m_pSet_p.Open_DB(g_StrDirPath[0]))		
	//{
	//	
	//}
	//else
	//{
	//	CString strSQL;
	//	strSQL.Format(_T("SELECT * FROM Product ORDER BY ProductName ASC"));
	//	if(m_pSet_p.Select_DB(strSQL))
	//	{
	//		m_Product_List.DeleteAllItems();
	//		if(!m_pSet_p.IsEOF())
	//		{
	//			while(m_pSet_p.IsEOF() != 1)
	//			{
	//				// 새로운 행 등록
	//				oneColumn.Format(_T("%03d"), (index+1));
	//				m_Product_List.InsertItem(index, _T("")); 
	//				m_Product_List.SetItemText(index, 1, oneColumn);
	//				CString tmp=+_T("");
	//				m_pSet_p.ChangeRsChar(_T("ProductName"), tmp);
	//				m_Product_List.SetItemText(index, 2, tmp);
	//				m_pSet_p.MoveNext();	
	//				index++;
	//			}
	//		}
	//	}
	//	
	//	if(m_pSet_p.b_IsOpenedDB)
	//		m_pSet_p.Close_DB();
	//}	
}


BOOL CProduct_Copy::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CProduct_Copy::OnItemchangedListProduct(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	int num=pNMLV->iItem;
	temp_Product=m_Product_List.GetItemText(num, 2);
	*pResult = 0;
}

void CProduct_Copy::LoadValue(CString product)
{
	MYSQL_RES* m_Res;
	MYSQL_ROW row;

	Init_Mysql();

	if (!g_Connecting)
	{
		SqlQuery.Format(_T("SELECT * FROM intellisense_product WHERE ProductName = '%s'"), product);

		if (mysql_query(&m_mysql, CStringA(SqlQuery)))
		{
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}
		else
		{
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
			g_DTScanspeed - _wtoi(ProductDTscanspeed);
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
			g_MaskClip_Length = _wtoi(ProductClipnum);
			CString ProductClipgray(row[35]);
			g_MaskClip_Direction = _wtoi(ProductClipgray);
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

			CString ProductEtc11(row[62]);
			g_MaskBoxCondition5 = _wtoi(ProductEtc11);
			CString ProductEtc12(row[63]);
			g_MaskBoxCondition6 = _wtoi(ProductEtc12);
			CString ProductEtc13(row[64]);
			g_ViewControl = _wtoi(ProductEtc13);
			
			int tempbool2;
			CString ProductEtc14(row[65]);
			tempbool2 = _wtoi(ProductEtc14);
			if (tempbool2 == 1)
				g_Accept = true;
			else
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

	/*if(!m_pSet_p.Open_DB(g_StrDirPath[0]))		
	{
		
	}
	else
	{
		CString strSQL;
		strSQL.Format(_T("SELECT * FROM Product WHERE ProductName = '%s'"), product);
		if(m_pSet_p.Select_DB(strSQL))
		{
			if(m_pSet_p.IsEOF())
			{
				if(m_pSet_p.b_IsOpenedDB)
					m_pSet_p.Close_DB();
				return;
			}
			else
			{
				m_pSet_p.ChangeRsInt(_T("ProductXraykv"), g_Xray_kv);
				m_pSet_p.ChangeRsInt(_T("ProductXrayma"), g_Xray_ma);
				m_pSet_p.ChangeRsInt(_T("ProductXraykv2"), g_Xray_kv2);
				m_pSet_p.ChangeRsInt(_T("ProductXrayma2"), g_Xray_ma2);
				m_pSet_p.ChangeRsInt(_T("ProductDTscanspeed"), g_DTScanspeed);
				m_pSet_p.ChangeRsFloat(_T("ProductDTGain"), g_DTGain);
				m_pSet_p.ChangeRsInt(_T("ProductRejecUse"), g_RejectUse);
				m_pSet_p.ChangeRsInt(_T("ProductRejectDelay"), g_RejectDelay);
				m_pSet_p.ChangeRsInt(_T("ProductRejectKeep"), g_RejectKeep);

				m_pSet_p.ChangeRsInt(_T("ProductCode"), g_Code);
				m_pSet_p.ChangeRsInt(_T("ProductSize"), g_Size);
				m_pSet_p.ChangeRsInt(_T("ProductConveyorSpeed"), g_ConveyorSpeed);
				m_pSet_p.ChangeRsInt(_T("ProductSaveFile"), g_SaveFile);
				m_pSet_p.ChangeRsInt(_T("ProductSaveHard"), g_SaveHard);
				m_pSet_p.ChangeRsInt(_T("ProductCond1"), g_Condition1);
				m_pSet_p.ChangeRsInt(_T("ProductCond2"), g_Condition2);
				m_pSet_p.ChangeRsInt(_T("ProductCond3"), g_Condition3);
				m_pSet_p.ChangeRsInt(_T("ProductCond4"), g_Condition4);
				m_pSet_p.ChangeRsInt(_T("ProductCond5"), g_Condition5);
				m_pSet_p.ChangeRsInt(_T("ProductCond6"), g_Condition6);
		
				m_pSet_p.ChangeRsInt(_T("ProductOmitCond1"), g_OmitCondition1);
				m_pSet_p.ChangeRsInt(_T("ProductOmitCond2"), g_OmitCondition2);
				m_pSet_p.ChangeRsInt(_T("ProductOmitCond3"), g_OmitCondition3);
				m_pSet_p.ChangeRsInt(_T("ProductOmitCond4"), g_OmitCondition4);
				m_pSet_p.ChangeRsInt(_T("ProductOmitCond5"), g_OmitCondition5);
				m_pSet_p.ChangeRsInt(_T("ProductMaskx"), g_MaskBoxCondition1);
				m_pSet_p.ChangeRsInt(_T("ProductMasky"), g_MaskBoxCondition2);
				m_pSet_p.ChangeRsInt(_T("ProductEtc8"), g_MaskBoxCondition3);
				int tempbool=0;
				m_pSet_p.ChangeRsInt(_T("ProductDelD"), tempbool);
				if(tempbool==1)
					g_bOptionD=true;
				else
					g_bOptionD=false;
				m_pSet_p.ChangeRsInt(_T("ProductDelD_1"), g_bOptionD_1);
				m_pSet_p.ChangeRsInt(_T("ProductDelD_2"), g_bOptionD_2);
				
				m_pSet_p.ChangeRsInt(_T("ProductDelE"), tempbool);
				if(tempbool==1)
					g_bOptionE=true;
				else
					g_bOptionE=false;
				m_pSet_p.ChangeRsInt(_T("ProductDelE_1"), g_bOptionE_1);
				m_pSet_p.ChangeRsInt(_T("ProductDelE_2"), g_bOptionE_2);

				m_pSet_p.ChangeRsInt(_T("ProductClipnum"), g_MaskClip_Direction);
				m_pSet_p.ChangeRsInt(_T("ProductClipgray"), g_MaskClip_Length);
				m_pSet_p.ChangeRsInt(_T("ProductClipdeep"), g_MaskClip_Deep);

				m_pSet_p.ChangeRsInt(_T("ProductCountnum"), g_Countnum);
				m_pSet_p.ChangeRsInt(_T("ProductCountnoise"), g_Countnoise);
			
				m_pSet_p.ChangeRsInt(_T("ProductEtc1"), g_PreproUse);
				m_pSet_p.ChangeRsInt(_T("ProductEtc2"), g_PreproValue);
				m_pSet_p.ChangeRsInt(_T("ProductEtc3"), g_PreproDisp);
				m_pSet_p.ChangeRsInt(_T("ProductEtc4"), g_PageMaskLR);
				m_pSet_p.ChangeRsInt(_T("ProductEtc5"), g_PageMaskUD);
				m_pSet_p.ChangeRsInt(_T("ProductTalBright"), g_TalMask_Bright);
				m_pSet_p.ChangeRsInt(_T("ProductTalBright2"), g_TalMask_Bright2);
				
				m_pSet_p.ChangeRsInt(_T("ProductUseOmit"), g_OmitCondUse);
				m_pSet_p.ChangeRsInt(_T("ProductUseMass"), g_MassCondUse);
				m_pSet_p.ChangeRsInt(_T("ProductUseCount"), g_CountCondUse);
				m_pSet_p.ChangeRsInt(_T("ProductUseMask"), g_MaskBoxUse);
				m_pSet_p.ChangeRsInt(_T("ProductUseClip"), g_MaskClipUse);
				m_pSet_p.ChangeRsInt(_T("ProductUseTalox"), g_MaskTalUse);

				m_pSet_p.ChangeRsInt(_T("ProductMassCond1"), g_MassCondition1);
				m_pSet_p.ChangeRsInt(_T("ProductMassCond2"), g_MassCondition2);
				m_pSet_p.ChangeRsInt(_T("ProductMassCond3"), g_MassCondition3);	
				m_pSet_p.ChangeRsInt(_T("ProductEtc9"), g_ShowMaskUse);
			
				m_pSet_p.ChangeRsInt(_T("ProductEtc10"), g_MaskBoxCondition4);
				m_pSet_p.ChangeRsInt(_T("ProductEtc11"), g_MaskBoxCondition5);
				m_pSet_p.ChangeRsInt(_T("ProductEtc12"), g_MaskBoxCondition6);

				m_pSet_p.ChangeRsInt(_T("ProductEtc13"), g_ViewControl);
				int tempbool2 = 0;
				m_pSet_p.ChangeRsInt(_T("ProductEtc14"),tempbool2);
					if(tempbool2 == 1)
					g_Accept = true;
				else if(tempbool2 == 0)
					g_Accept = false;
			}
		}
		if(m_pSet_p.b_IsOpenedDB)
			m_pSet_p.Close_DB();
	}*/
}


void CProduct_Copy::CopyProduct()
{
	Init_Mysql();

	if (!g_Connecting)
		AfxMessageBox(_T("실패"));
	else
	{
		CString strSQL;
		CTime time;
		time = CTime::GetCurrentTime();
		CString strdate;
		strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(), time.GetMonth(), time.GetDay());
		strSQL.Format(_T("Insert INTO intellisense_product(ProductName, ProductXraykv, ProductXrayma, ProductXraykv2, ProductXrayma2, ProductDTscanspeed, ProductDTGain, ProductRejectUse, ProductRejectDelay, ProductRejectKeep, ProductCode, ProductSize, ProductConveyorSpeed, ProductSaveFile, ProductSaveHard, ProductCond1, ProductCond2, ProductCond3, ProductCond4, ProductCond5, ProductCond6, ProductOmitCond1, ProductOmitCond2, ProductOmitCond3, ProductOmitCond4, ProductOmitCond5, ProductMaskx, ProductMasky, ProductDelD, ProductDelD_1, ProductDelD_2, ProductDelE, ProductDelE_1, ProductDelE_2, ProductClipnum, ProductClipgray, ProductClipdeep, ProductCountnum, ProductCountnoise, ProductEtc1, ProductEtc2, ProductEtc3, ProductEtc4, ProductEtc5, ProductTalBright, ProductTalBright2, ProductUseOmit, ProductUseMass, ProductUseCount, ProductUseMask, ProductUseClip, ProductUseTalox, ProductMassCond1, ProductMassCond2, ProductMassCond3, ProductLastDate, ProductRegisterDate, ProductEtc8, ProductEtc9, ProductEtc10, ProductEtc11, ProductEtc12, ProductEtc13, ProductEtc14, ProductEtc6, ProductEtc7) ") _T("VALUES('%s', %d, %d, %d, %d, %d, %f, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d)"),
			g_ProductName, g_Xray_kv, g_Xray_ma, g_Xray_kv2, g_Xray_ma2, g_DTScanspeed, g_DTGain, g_RejectUse, g_RejectDelay, g_RejectKeep, g_Code, g_Size, g_ConveyorSpeed, g_SaveFile, g_SaveHard, g_Condition1, g_Condition2, g_Condition3, g_Condition4, g_Condition5, g_Condition6, g_OmitCondition1, g_OmitCondition2, g_OmitCondition3, g_OmitCondition4, g_OmitCondition5, g_MaskBoxCondition1, g_MaskBoxCondition2, g_bOptionD, g_bOptionD_1, g_bOptionD_2, g_bOptionE, g_bOptionE_1, g_bOptionE_2, g_MaskClip_Direction, g_MaskClip_Length, g_MaskClip_Deep, g_Countnum, g_Countnoise, g_PreproUse, g_PreproValue, g_PreproDisp, g_PageMaskLR, g_PageMaskUD, g_TalMask_Bright, g_TalMask_Bright2, g_OmitCondUse, g_MassCondUse, g_CountCondUse, g_MaskBoxUse, g_MaskClipUse, g_MaskTalUse, g_MassCondition1, g_MassCondition2, g_MassCondition3, strdate, strdate, g_MaskBoxCondition3, g_ShowMaskUse, g_MaskBoxCondition4, g_MaskBoxCondition5, g_MaskBoxCondition6, g_ViewControl, g_Accept, g_SetRejectcount, g_SetRejectrate);

		if (mysql_query(&m_mysql, CStringA(strSQL))) {
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}
	}

	Close_Mysql();

	//if(!m_pSet_p.Open_DB(g_StrDirPath[0]))		
	//{
	//	
	//}
	//else
	//{
	//	CString strSQL;
	//	strSQL.Format(_T("SELECT * FROM Product WHERE ProductName = '%s'"), g_ProductName);
	//	if(m_pSet_p.Select_DB(strSQL))
	//	{
	//		if(!m_pSet_p.IsEOF())
	//		{
	//		}
	//		else
	//		{
	//			
	//			CTime time;
	//			time = CTime::GetCurrentTime();
	//			CString strdate;
	//			strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());	
	//			CString strQuery;
	//			strQuery.Format(_T("Insert INTO Product(ProductName, ProductXraykv, ProductXrayma, ProductXraykv2, ProductXrayma2, ProductDTscanspeed, ProductDTGain, ProductRejecUse, ProductRejectDelay, ProductRejectKeep, ProductCode, ProductSize, ProductConveyorSpeed, ProductSaveFile, ProductSaveHard, ProductCond1, ProductCond2, ProductCond3, ProductCond4, ProductCond5, ProductCond6, ProductOmitCond1, ProductOmitCond2, ProductOmitCond3, ProductOmitCond4, ProductOmitCond5, ProductMaskx, ProductMasky, ProductDelD, ProductDelD_1, ProductDelD_2, ProductDelE, ProductDelE_1, ProductDelE_2, ProductClipnum, ProductClipgray, ProductClipdeep, ProductCountnum, ProductCountnoise, ProductEtc1, ProductEtc2, ProductEtc3, ProductEtc4, ProductEtc5, ProductTalBright, ProductTalBright2, ProductUseOmit, ProductUseMass, ProductUseCount, ProductUseMask, ProductUseClip, ProductUseTalox, ProductMassCond1, ProductMassCond2, ProductMassCond3, ProductRegisterDate, ProductLastDate, ProductEtc8, ProductEtc9, ProductEtc10, ProductEtc11, ProductEtc12) ") _T("VALUES('%s', %d, %d, %d, %d, %d, %f, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', '%s', %d, %d, %d, %d, %d)"), 
	//				g_ProductName, g_Xray_kv, g_Xray_ma, g_Xray_kv2, g_Xray_ma2, g_DTScanspeed, g_DTGain, g_RejectUse, g_RejectDelay, g_RejectKeep,	g_Code, g_Size, g_ConveyorSpeed, g_SaveFile, g_SaveHard, g_Condition1, g_Condition2, g_Condition3, g_Condition4, g_Condition5, g_Condition6, g_OmitCondition1, g_OmitCondition2, g_OmitCondition3, g_OmitCondition4, g_OmitCondition5, g_MaskBoxCondition1, g_MaskBoxCondition2, g_bOptionD, g_bOptionD_1, g_bOptionD_2, g_bOptionE, g_bOptionE_1, g_bOptionE_2, g_MaskClip_Direction, g_MaskClip_Length, g_MaskClip_Deep, g_Countnum, g_Countnoise, g_PreproUse, g_PreproValue, g_PreproDisp, g_PageMaskLR, g_PageMaskUD, g_TalMask_Bright, g_TalMask_Bright2, g_OmitCondUse, g_MassCondUse, g_CountCondUse, g_MaskBoxUse, g_MaskClipUse, g_MaskTalUse, g_MassCondition1, g_MassCondition2, g_MassCondition3, strdate, strdate, g_MaskBoxCondition3, g_ShowMaskUse, g_MaskBoxCondition4, g_MaskBoxCondition5, g_MaskBoxCondition6);
	//			m_pSet_p.Insert_DB(strQuery);
	//		}
	//	}
	//	if(m_pSet_p.b_IsOpenedDB)
	//		m_pSet_p.Close_DB();
	//}
}

void CProduct_Copy::OnBnClickedOk()
{
	if(temp_Product=="")
		return;
	
	LoadValue(temp_Product);
	CopyProduct();

	CDialogEx::OnOK();
}


void CProduct_Copy::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_ProductName=temp_Product;
	CDialogEx::OnCancel();
}
