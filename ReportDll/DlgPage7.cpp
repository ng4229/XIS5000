// DlgPage7.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgPage7.h"
#include "afxdialogex.h"


// CDlgPage7 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgPage7, CDialogEx)

CDlgPage7::CDlgPage7(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPage7::IDD, pParent)
{

}

CDlgPage7::~CDlgPage7()
{
}

void CDlgPage7::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_Product_List);
}


BEGIN_MESSAGE_MAP(CDlgPage7, CDialogEx)
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PRODUCT, &CDlgPage7::OnItemchangedListProduct)
END_MESSAGE_MAP()


// CDlgPage7 메시지 처리기입니다.


void CDlgPage7::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpMainBG);
	dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &cdc, 0, 0, SRCCOPY);
	cdc.DeleteDC();
}


BOOL CDlgPage7::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		return false;
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgPage7::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadBackground();
	InitList();
	LoadList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}




void CDlgPage7::LoadBackground()
{
	BITMAP bmpInfo; // 비트맵 정보
	if(m_RNationalCode==0)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_7);
	else if (m_RNationalCode == 1)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_7_ENG);
	else if (m_RNationalCode == 2)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_7_CHI);
	else if (m_RNationalCode == 3)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_7_RUS);
	else if (m_RNationalCode == 4)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_7_UKR);
	else if (m_RNationalCode == 5)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_7_SPN);
	else if (m_RNationalCode == 6)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_7_POL);
	else if (m_RNationalCode == 7)
		m_bmpMainBG.LoadBitmap(IDB_SUBBACKGROUND_7_TIW);
	m_bmpMainBG.GetBitmap(&bmpInfo); // Main 배경 비트맵 정보 읽기
	m_nMainWidth = bmpInfo.bmWidth; // Main 윈도우 폭
	m_nMainHeight = bmpInfo.bmHeight; // Main 윈도우 높이
	SetWindowPos(NULL, 0, 0, m_nMainWidth, m_nMainHeight, SWP_NOZORDER | SWP_NOMOVE);

}
void CDlgPage7::InitList(void)
{
	CRect rect;
	m_Product_List.GetClientRect(&rect);
	m_Product_List.InsertColumn(0, _T(""), LVCFMT_CENTER, 0);
	m_Product_List.InsertColumn(1, _T("No")/*g_MessageArr[151]*/, LVCFMT_CENTER, rect.Width() / 4);
	if(m_RNationalCode==0)
		m_Product_List.InsertColumn(2, _T("제품명")/*g_MessageArr[152]*/, LVCFMT_CENTER, rect.Width() / 4 * 3 - 17);
	else if (m_RNationalCode == 1)
		m_Product_List.InsertColumn(2, _T("Product")/*g_MessageArr[152]*/, LVCFMT_CENTER, rect.Width() / 4 * 3 - 17);
	else if (m_RNationalCode == 2)
		m_Product_List.InsertColumn(2, _T("産品名")/*g_MessageArr[152]*/, LVCFMT_CENTER, rect.Width() / 4 * 3 - 17);
	else if (m_RNationalCode == 3)
		m_Product_List.InsertColumn(2, _T("Продукт")/*g_MessageArr[152]*/, LVCFMT_CENTER, rect.Width() / 4 * 3 - 17);
	else if (m_RNationalCode == 4)
		m_Product_List.InsertColumn(2, _T("Продукт")/*g_MessageArr[152]*/, LVCFMT_CENTER, rect.Width() / 4 * 3 - 17);
	else if (m_RNationalCode == 5)
		m_Product_List.InsertColumn(2, _T("Producto")/*g_MessageArr[152]*/, LVCFMT_CENTER, rect.Width() / 4 * 3 - 17);
	else if (m_RNationalCode == 6)
		m_Product_List.InsertColumn(2, _T("produkt")/*g_MessageArr[152]*/, LVCFMT_CENTER, rect.Width() / 4 * 3 - 17);
	else if (m_RNationalCode == 7)
		m_Product_List.InsertColumn(2, _T("產品")/*g_MessageArr[152]*/, LVCFMT_CENTER, rect.Width() / 4 * 3 - 17);
	m_Product_List.InsertColumn(3, _T(""), LVCFMT_CENTER, 0);
	m_Product_List.InsertColumn(4, _T(""), LVCFMT_CENTER, 0);
	m_Product_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}


void CDlgPage7::LoadList(void)
{
	CString m_ProductName;
	CString oneColumn;
	int index = 0;

	CFileFind finder;
	CString strQuery;

	if (!g_Connecting)
	{
		//AfxMessageBox(_T("실패"));
	}
	else
	{
		MYSQL_RES* m_Res;
		MYSQL_ROW row;
		mysql_query(&m_mysql, "set names euckr");
		SqlQuery.Format(_T("SELECT * FROM intellisense_product"));

		if (mysql_query(&m_mysql, CStringA(SqlQuery)))
		{
			CString error(mysql_error(&m_mysql));
			WriteErrorLog(23, error);
		}
		else
		{
			m_Product_List.DeleteAllItems();
			m_Res = mysql_store_result(&m_mysql);
			while ((row = mysql_fetch_row(m_Res)) != NULL)
			{
				CString tmp = _T("");
				tmp = row[0];
				if (tmp != _T(""))
				{
					oneColumn.Format(_T("%03d"), index + 1);
					m_Product_List.InsertItem(index, _T(""));
					m_Product_List.SetItemText(index, 1, oneColumn);
					m_Product_List.SetItemText(index, 2, tmp);
					index++;
				}

				/*if (mysql_query(&m_mysql, CStringA(strQuery)))
				{
					CString error(mysql_error(&m_mysql));
					AfxMessageBox(_T("24"));
				}*/
			}
		}
		mysql_free_result(m_Res);
	}


	//if (!m_pSet_p.Open_DB(g_StrDirPath[0]))
	//{
	//}
	//else
	//{
	//	CString strSQL;
	//	strSQL.Format(_T("SELECT * FROM Product ORDER BY ProductName ASC"));
	//	if (m_pSet_p.Select_DB(strSQL))
	//	{
	//		m_Product_List.DeleteAllItems();
	//		while (m_pSet_p.IsEOF() != 1)
	//		{
	//			// 새로운 행 등록
	//			CString tmp = +_T("");
	//			m_pSet_p.ChangeRsChar(_T("ProductName"), tmp);
	//			if (tmp != _T(""))
	//			{
	//				oneColumn.Format(_T("%03d"), (index + 1));
	//				m_Product_List.InsertItem(index, _T(""));
	//				m_Product_List.SetItemText(index, 1, oneColumn);
	//				//CString tmp=_T("");
	//				//m_pSet_p.ChangeRsChar(_T("ProductName"), tmp);
	//				m_Product_List.SetItemText(index, 2, tmp);
	//				/*					m_pSet_p.ChangeRsChar(_T("ProductRegisterDate"), tmp);
	//									m_Product_List.SetItemText(index, 3, tmp);
	//									m_pSet_p.ChangeRsChar(_T("ProductLastDate"), tmp);
	//									m_Product_List.SetItemText(index, 4, tmp);
	//				*/					index++;
	//			}
	//			m_pSet_p.MoveNext();
	//		}
	//	}
	//	if (m_pSet_p.b_IsOpenedDB)
	//	{
	//		m_pSet_p.Close_DB();
	//	}
	//}
}


void CDlgPage7::OnItemchangedListProduct(NMHDR* pNMHDR, LRESULT* pResult)
{

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	UpdateData(true);
	int num = pNMLV->iItem;
	CString temp_Product = m_Product_List.GetItemText(num, 2);

	if (temp_Product != _T(""))
	{
		CString strQuery;

		MYSQL_RES* m_Res;
		MYSQL_ROW row;

		//mysql_query(&m_mysql, "set names euckr");
		SqlQuery.Format(_T("SELECT * FROM intellisense_product WHERE ProductName = '%s'"), temp_Product);

		if (mysql_query(&m_mysql, CStringA(SqlQuery)))
		{
			CString error(mysql_error(&m_mysql));
			WriteErrorLog(25, error);
		}
		else
		{
			m_Res = mysql_store_result(&m_mysql);
			while ((row = mysql_fetch_row(m_Res)) != NULL)
			{
				CString tmp, tmp1;
				CString disp;
				int nTemp;

				tmp = row[3];
				tmp1 = row[26];
				disp.Format(_T("%s(%s)"), tmp, tmp1);
				GetDlgItem(IDC_EDIT_XRAYKV)->SetWindowText(disp);

				tmp = row[4];
				tmp1 = row[27];
				disp.Format(_T("%s(%s)"), tmp, tmp1);
				GetDlgItem(IDC_EDIT_XRAYUA)->SetWindowText(disp);

				tmp = row[10];
				GetDlgItem(IDC_EDIT_DTCONV)->SetWindowText(tmp);
				tmp = row[5];
				GetDlgItem(IDC_EDIT_DTSCAN)->SetWindowText(tmp);

				tmp = row[11];
				nTemp = _ttoi(tmp);

				if (m_RNationalCode == 0)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("저장"));
					else
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("저장안함"));
				}
				else if (m_RNationalCode == 1)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Save"));
					else
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Don't Save"));
				}
				else if (m_RNationalCode == 2)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("保存"));
					else
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("不保存"));
				}
				else if (m_RNationalCode == 3)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Сохранить"));
					else
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Не сохранять"));
				}
				else if (m_RNationalCode == 4)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Зберегти"));
					else
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Не зберігати"));
				}
				else if (m_RNationalCode == 5)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Ahorrar"));
					else
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("No Ahorrar"));
				}
				else if (m_RNationalCode == 6)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("zapisać"));
					else
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Nie chroniony"));
				}
				else if (m_RNationalCode == 7)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("儲存"));
					else
						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("不要存錢"));
				}

				tmp = row[7];
				nTemp = _ttoi(tmp);

				if (m_RNationalCode == 0)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("사용"));
					else if (nTemp == 1)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("사용안함"));
					else
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("벨트정지"));
				}
				else if (m_RNationalCode == 1)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("On"));
					else if (nTemp == 1)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Off"));
					else
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Stop"));
				}
				else if (m_RNationalCode == 2)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("使用"));
					else if (nTemp == 1)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("不用"));
					else
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("传送带停止"));
				}
				else if (m_RNationalCode == 3)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("На"));
					else if (nTemp == 1)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Выключенный"));
					else
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Стоп"));
				}
				else if (m_RNationalCode == 4)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Увімкнено"));
					else if (nTemp == 1)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Вимкнено"));
					else
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Стій"));
				}
				else if (m_RNationalCode == 5)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("En"));
					else if (nTemp == 1)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Apagada"));
					else
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Detener"));
				}
				else if (m_RNationalCode == 6)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("na"));
					else if (nTemp == 1)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("wyłączony"));
					else
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("zatrzymać"));
				}
				else if (m_RNationalCode == 7)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("開啟"));
					else if (nTemp == 1)
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("關閉"));
					else
						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("輸送機停止"));
				}

				tmp = row[8];
				GetDlgItem(IDC_EDIT_REJECTDELAY)->SetWindowTextW(tmp);
				tmp = row[9];
				GetDlgItem(IDC_EDIT_REJECTKEEP)->SetWindowTextW(tmp);
				
				tmp = row[13];
				GetDlgItem(IDC_EDIT_COND1)->SetWindowTextW(tmp);
				tmp = row[14];
				GetDlgItem(IDC_EDIT_COND2)->SetWindowTextW(tmp);
				tmp = row[15];
				GetDlgItem(IDC_EDIT_COND3)->SetWindowTextW(tmp);
				tmp = row[16];
				GetDlgItem(IDC_EDIT_COND4)->SetWindowTextW(tmp);
				tmp = row[17];
				GetDlgItem(IDC_EDIT_COND5)->SetWindowTextW(tmp);
				tmp = row[18];
				GetDlgItem(IDC_EDIT_COND6)->SetWindowTextW(tmp);

				tmp = row[52];
				nTemp = _ttoi(tmp);
				if (m_RNationalCode == 0)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("사용"));
					else
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("사용안함"));
				}
				else if (m_RNationalCode == 1)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("Use"));
					else
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("Don't Use"));
				}
				else if (m_RNationalCode == 2)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("使用"));
					else
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("不用"));
				}
				else if (m_RNationalCode == 3)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("На"));
					else
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("Выключенный"));
				}
				else if (m_RNationalCode == 4)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("Увімкнено"));
					else
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("Вимкнено"));
				}
				else if (m_RNationalCode == 5)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("Usar"));
					else
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("No Usar"));
				}
				else if (m_RNationalCode == 6)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("posługiwać się"));
					else
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("nie używaj"));
				}
				else if (m_RNationalCode == 7)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("開啟"));
					else
						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("關閉"));
				}

				tmp = row[24];
				GetDlgItem(IDC_EDIT_MASK1)->SetWindowTextW(tmp);
				tmp = row[25];
				GetDlgItem(IDC_EDIT_MASK2)->SetWindowTextW(tmp);
				/*	m_pSet_p.ChangeRsInt(_T("ProductMaska"), temp);
					SetDlgItemInt(IDC_EDIT_MASK3, temp);
					m_pSet_p.ChangeRsInt(_T("ProductMaskb"), temp);
					SetDlgItemInt(IDC_EDIT_MASK4, temp);
*/
				tmp = row[49];
				nTemp = _ttoi(tmp);
				if (m_RNationalCode == 0)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("사용"));
					else
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("사용안함"));
				}
				else if (m_RNationalCode == 1)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("Use"));
					else
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("Don't Use"));
				}
				else if (m_RNationalCode == 2)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("使用"));
					else
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("不用"));
				}
				else if (m_RNationalCode == 3)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("На"));
					else
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("Выключенный"));
				}
				else if (m_RNationalCode == 4)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("Увімкнено"));
					else
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("Вимкнено"));
				}
				else if (m_RNationalCode == 5)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("Usar"));
					else
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("No Usar"));
				}
				else if (m_RNationalCode == 6)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("posługiwać się"));
					else
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("nie używaj"));
				}
				else if (m_RNationalCode == 7)
				{
					if (nTemp == 0)
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("開啟"));
					else
						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("關閉"));
				}

				tmp = row[19];
				GetDlgItem(IDC_EDIT_OMIT1)->SetWindowTextW(tmp);
				tmp = row[20];
				GetDlgItem(IDC_EDIT_OMIT2)->SetWindowTextW(tmp);
				tmp = row[21];
				GetDlgItem(IDC_EDIT_OMIT3)->SetWindowTextW(tmp);
				tmp = row[22];
				GetDlgItem(IDC_EDIT_OMIT4)->SetWindowTextW(tmp);
				tmp = row[23];
				GetDlgItem(IDC_EDIT_OMIT5)->SetWindowTextW(tmp);
			}
		}
		mysql_free_result(m_Res);
	}

	//if (temp_Product != _T(""))
	//{
	//	CString strSQL;
	//	strSQL.Format(_T("SELECT * FROM Product WHERE ProductName = '%s'"), temp_Product);

	//	//20131226
	//	if (!m_pSet_p.Open_DB(g_StrDirPath[0]))
	//	{

	//	}
	//	else
	//	{
	//		if (m_pSet_p.Select_DB(strSQL))
	//		{

	//			if (m_pSet_p.IsEOF())
	//			{
	//				if (m_pSet_p.b_IsOpenedDB)
	//					m_pSet_p.Close_DB();
	//				return;

	//			}
	//			else
	//			{
	//				CString disp;
	//				int temp, temp1;
	//				m_pSet_p.ChangeRsInt(_T("ProductXraykv"), temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductXraykv2"), temp1);
	//				disp.Format(_T("%d(%d)"), temp, temp1);
	//				GetDlgItem(IDC_EDIT_XRAYKV)->SetWindowText(disp);
	//				//					SetDlgItemInt(IDC_EDIT_XRAYKV, temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductXrayma"), temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductXrayma2"), temp1);
	//				disp.Format(_T("%d(%d)"), temp, temp1);
	//				GetDlgItem(IDC_EDIT_XRAYUA)->SetWindowText(disp);


	//				m_pSet_p.ChangeRsInt(_T("ProductConveyorSpeed"), temp);
	//				SetDlgItemInt(IDC_EDIT_DTCONV, temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductDTscanspeed"), temp);
	//				SetDlgItemInt(IDC_EDIT_DTSCAN, temp);

	//				m_pSet_p.ChangeRsInt(_T("ProductSaveFile"), temp);
	//				
	//				if (m_RNationalCode == 0)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("저장"));
	//					else
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("저장안함"));
	//				}
	//				else if (m_RNationalCode == 1)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Save"));
	//					else
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Don't Save"));
	//				}
	//				else if (m_RNationalCode == 2)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("保存"));
	//					else
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("不保存"));
	//				}
	//				else if (m_RNationalCode == 3)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Сохранить"));
	//					else
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Не сохранять"));
	//				}
	//				else if (m_RNationalCode == 4)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Зберегти"));
	//					else
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Не зберігати"));
	//				}
	//				else if (m_RNationalCode == 5)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Ahorrar"));
	//					else
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("No Ahorrar"));
	//				}
	//				else if (m_RNationalCode == 6)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("zapisać"));
	//					else
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("Nie chroniony"));
	//				}
	//				else if (m_RNationalCode == 7)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("儲存"));
	//					else
	//						GetDlgItem(IDC_EDIT_IMAGESAVE)->SetWindowText(_T("不要存錢"));
	//				}
	//				m_pSet_p.ChangeRsInt(_T("ProductRejecUse"), temp);

	//				if (m_RNationalCode == 0)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("사용"));
	//					else if (temp == 1)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("사용안함"));
	//					else
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("벨트정지"));
	//				}
	//				else if (m_RNationalCode == 1)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("On"));
	//					else if (temp == 1)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Off"));
	//					else
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Stop"));
	//				}
	//				else if (m_RNationalCode == 2)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("使用"));
	//					else if (temp == 1)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("不用"));
	//					else
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("传送带停止"));
	//				}
	//				else if (m_RNationalCode == 3)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("На"));
	//					else if (temp == 1)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Выключенный"));
	//					else
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Стоп"));
	//				}
	//				else if (m_RNationalCode == 4)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Увімкнено"));
	//					else if (temp == 1)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Вимкнено"));
	//					else
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Стій"));
	//				}
	//				else if (m_RNationalCode == 5)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("En"));
	//					else if (temp == 1)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Apagada"));
	//					else
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("Detener"));
	//				}
	//				else if (m_RNationalCode == 6)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("na"));
	//					else if (temp == 1)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("wyłączony"));
	//					else
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("zatrzymać"));
	//				}
	//				else if (m_RNationalCode == 7)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("開啟"));
	//					else if (temp == 1)
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("關閉"));
	//					else
	//						GetDlgItem(IDC_EDIT_REJECTUSE)->SetWindowText(_T("輸送機停止"));
	//				}
	//				m_pSet_p.ChangeRsInt(_T("ProductRejectDelay"), temp);
	//				SetDlgItemInt(IDC_EDIT_REJECTDELAY, temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductRejectKeep"), temp);
	//				SetDlgItemInt(IDC_EDIT_REJECTKEEP, temp);


	//				m_pSet_p.ChangeRsInt(_T("ProductCond1"), temp);
	//				SetDlgItemInt(IDC_EDIT_COND1, temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductCond2"), temp);
	//				SetDlgItemInt(IDC_EDIT_COND2, temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductCond3"), temp);
	//				SetDlgItemInt(IDC_EDIT_COND3, temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductCond4"), temp);
	//				SetDlgItemInt(IDC_EDIT_COND4, temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductCond5"), temp);
	//				SetDlgItemInt(IDC_EDIT_COND5, temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductCond6"), temp);
	//				SetDlgItemInt(IDC_EDIT_COND6, temp);

	//				m_pSet_p.ChangeRsInt(_T("ProductUseMask"), temp);
	//				if (m_RNationalCode == 0)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("사용"));
	//					else
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("사용안함"));
	//				}
	//				else if (m_RNationalCode == 1)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("Use"));
	//					else
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("Don't Use"));
	//				}
	//				else if (m_RNationalCode == 2)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("使用"));
	//					else
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("不用"));
	//				}					
	//				else if (m_RNationalCode == 3)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("На"));
	//					else
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("Выключенный"));
	//				}
	//				else if (m_RNationalCode == 4)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("Увімкнено"));
	//					else
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("Вимкнено"));
	//				}
	//				else if (m_RNationalCode == 5)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("Usar"));
	//					else
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("No Usar"));
	//				}
	//				else if (m_RNationalCode == 6)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("posługiwać się"));
	//					else
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("nie używaj"));
	//				}
	//				else if (m_RNationalCode == 7)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("開啟"));
	//					else
	//						GetDlgItem(IDC_EDIT_MASKUSE)->SetWindowText(_T("關閉"));
	//				}
	//				m_pSet_p.ChangeRsInt(_T("ProductMaskx"), temp);
	//				SetDlgItemInt(IDC_EDIT_MASK1, temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductMasky"), temp);
	//				SetDlgItemInt(IDC_EDIT_MASK2, temp);
	//				/*	m_pSet_p.ChangeRsInt(_T("ProductMaska"), temp);
	//					SetDlgItemInt(IDC_EDIT_MASK3, temp);
	//					m_pSet_p.ChangeRsInt(_T("ProductMaskb"), temp);
	//					SetDlgItemInt(IDC_EDIT_MASK4, temp);
	//*/
	//				m_pSet_p.ChangeRsInt(_T("ProductUseOmit"), temp);
	//				if (m_RNationalCode == 0)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("사용"));
	//					else
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("사용안함"));
	//				}
	//				else if (m_RNationalCode == 1)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("Use"));
	//					else
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("Don't Use"));
	//				}
	//				else if (m_RNationalCode == 2)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("使用"));
	//					else
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("不用"));
	//				}
	//				else if (m_RNationalCode == 3)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("На"));
	//					else
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("Выключенный"));
	//				}
	//				else if (m_RNationalCode == 4)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("Увімкнено"));
	//					else
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("Вимкнено"));
	//				}
	//				else if (m_RNationalCode == 5)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("Usar"));
	//					else
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("No Usar"));
	//				}
	//				else if (m_RNationalCode == 6)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("posługiwać się"));
	//					else
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("nie używaj"));
	//				}
	//				else if (m_RNationalCode == 7)
	//				{
	//					if (temp == 0)
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("開啟"));
	//					else
	//						GetDlgItem(IDC_EDIT_OMITUSE)->SetWindowText(_T("關閉"));
	//				}
	//				m_pSet_p.ChangeRsInt(_T("ProductOmitCond1"), temp);
	//				SetDlgItemInt(IDC_EDIT_OMIT1, temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductOmitCond2"), temp);
	//				SetDlgItemInt(IDC_EDIT_OMIT2, temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductOmitCond3"), temp);
	//				SetDlgItemInt(IDC_EDIT_OMIT3, temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductOmitCond4"), temp);
	//				SetDlgItemInt(IDC_EDIT_OMIT4, temp);
	//				m_pSet_p.ChangeRsInt(_T("ProductOmitCond5"), temp);
	//				SetDlgItemInt(IDC_EDIT_OMIT5, temp);




	//				/*




	//					m_pSet_p.ChangeRsInt(_T("ProductEtc4"), g_PageMaskLR);
	//					m_pSet_p.ChangeRsInt(_T("ProductEtc5"), g_PageMaskUD);
	//					m_pSet_p.ChangeRsInt(_T("ProductTalBright"), g_TalMask_Bright);
	//					m_pSet_p.ChangeRsInt(_T("ProductTalBright2"), g_TalMask_Bright2);

	//					m_pSet_p.ChangeRsInt(_T("ProductUseOmit"), g_OmitCondUse);
	//					m_pSet_p.ChangeRsInt(_T("ProductUseMass"), g_MassCondUse);
	//					m_pSet_p.ChangeRsInt(_T("ProductUseCount"), g_CountCondUse);
	//					m_pSet_p.ChangeRsInt(_T("ProductUseMask"), g_MaskBoxUse);
	//					m_pSet_p.ChangeRsInt(_T("ProductUseClip"), g_MaskClipUse);
	//					m_pSet_p.ChangeRsInt(_T("ProductUseTalox"), g_MaskTalUse);

	//					m_pSet_p.ChangeRsInt(_T("ProductMassCond1"), g_MassCondition1);
	//					m_pSet_p.ChangeRsInt(_T("ProductMassCond2"), g_MassCondition2);
	//					m_pSet_p.ChangeRsInt(_T("ProductMassCond3"), g_MassCondition3);

	//					//20160919
	//					m_pSet_p.ChangeRsInt(_T("ProductCond1_2"), g_Condition1_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductCond2_2"), g_Condition2_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductCond3_2"), g_Condition3_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductCond4_2"), g_Condition4_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductCond5_2"), g_Condition5_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductCond6_2"), g_Condition6_2);

	//					m_pSet_p.ChangeRsInt(_T("ProductOmitCond1_2"), g_OmitCondition1_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductOmitCond2_2"), g_OmitCondition2_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductOmitCond3_2"), g_OmitCondition3_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductOmitCond4_2"), g_OmitCondition4_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductOmitCond5_2"), g_OmitCondition5_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductMaskx_2"), g_MaskBoxCondition1_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductMasky_2"), g_MaskBoxCondition2_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductMaska_2"), g_MaskBoxCondition3_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductMaskb_2"), g_MaskBoxCondition4_2);

	//					m_pSet_p.ChangeRsInt(_T("ProductUseOmit_2"), g_OmitCondUse_2);
	//					m_pSet_p.ChangeRsInt(_T("ProductUseMask_2"), g_MaskBoxUse_2);

	//					m_pSet_p.ChangeRsInt(_T("ProductXray2kv"), g_Xray2_kv);
	//					m_pSet_p.ChangeRsInt(_T("ProductXray2ma"), g_Xray2_ma);
	//					m_pSet_p.ChangeRsInt(_T("ProductXray2kv2"), g_Xray2_kv2);
	//					m_pSet_p.ChangeRsInt(_T("ProductXray2ma2"), g_Xray2_ma2);

	//					//20170301
	//					m_pSet_p.ChangeRsInt(_T("ProductCond1_3"), g_Condition1_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductCond2_3"), g_Condition2_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductCond3_3"), g_Condition3_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductCond4_3"), g_Condition4_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductCond5_3"), g_Condition5_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductCond6_3"), g_Condition6_3);

	//					m_pSet_p.ChangeRsInt(_T("ProductOmitCond1_3"), g_OmitCondition1_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductOmitCond2_3"), g_OmitCondition2_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductOmitCond3_3"), g_OmitCondition3_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductOmitCond4_3"), g_OmitCondition4_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductOmitCond5_3"), g_OmitCondition5_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductMaskx_3"), g_MaskBoxCondition1_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductMasky_3"), g_MaskBoxCondition2_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductMaska_3"), g_MaskBoxCondition3_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductMaskb_3"), g_MaskBoxCondition4_3);

	//					m_pSet_p.ChangeRsInt(_T("ProductUseOmit_3"), g_OmitCondUse_3);
	//					m_pSet_p.ChangeRsInt(_T("ProductUseMask_3"), g_MaskBoxUse_3);
	//	*/
	//			}
	//		}
	//		if (m_pSet_p.b_IsOpenedDB)
	//			m_pSet_p.Close_DB();
	//	}
	//}
	//*pResult = 0;
}
