// AutoSetup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "AutoSetup.h"
#include "afxdialogex.h"

#include "MessageBoxDlg.h"
#include "AutoSetup_0.h"
#include "AutoSetup_1.h"
#include "AutoSetup_2.h"
#include "AutoSetup_3.h"
#include "AutoSetup_4.h"
#include "Product_Copy.h"

// CAutoSetup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoSetup, CDialogEx)

CAutoSetup::CAutoSetup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoSetup::IDD, pParent)
{
	m_CurrentPage=0;
	m_Step=0;
	g_AutoCount=0;
	g_AutoCond1=0;
	g_AutoCond2=0;
	g_AutoCond3=0;
	g_AutoCond4=0;
	g_AutoCond5=0;
	g_AutoCond6=0;
	g_AutoCond1_1=0;
	g_AutoCond2_1=0;
	g_AutoCond3_1=0;
	g_AutoCond4_1=0;
	g_AutoCond5_1=0;
	g_AutoCond6_1=0;
	g_AutoXrayCount=0;
	g_AutoXrayValue=0;
}

CAutoSetup::~CAutoSetup()
{
}

void CAutoSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_NEXT, m_Bt_Next);
	DDX_Control(pDX, IDCANCEL, m_Bt_Close);
	DDX_Control(pDX, IDC_PAGE, m_Page);
}


BEGIN_MESSAGE_MAP(CAutoSetup, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCANCEL, &CAutoSetup::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BT_NEXT, &CAutoSetup::OnBnClickedBtNext)
END_MESSAGE_MAP()


// CAutoSetup 메시지 처리기입니다.


BOOL CAutoSetup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 672, 119, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	LoadBackground();
	::ZeroMemory(&m_Pages, sizeof(m_Pages));	
	m_Pages[0]=new CAutoSetup_0;
	m_Pages[1]=new CAutoSetup_1;
	m_Pages[2]=new CAutoSetup_2;
	m_Pages[3]=new CAutoSetup_3;
	m_Pages[4]=new CAutoSetup_4;
	m_nNumberOfPages=5;
	InitPage();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAutoSetup::LoadBackground(void)
{
	BITMAP bmpInfo;
	if(g_NationalCode==0)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_AUTOSETUP); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT,IDB_SELPRODUCT_EXIT_0); 
		if(m_Step==1)
			m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_SAVE,IDB_BTSETUPMENU_SAVE_1); 
		else if(m_Step==2)
		{
			if(g_bAutoSetup)
				m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_NEXT,IDB_BTSETUPMENU_NEXT_1); 
			else
				m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_SAVE,IDB_BTSETUPMENU_SAVE_1); 
		}
		else
			m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_NEXT,IDB_BTSETUPMENU_NEXT_1); 
	}
	else if(g_NationalCode==1)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_AUTOSETUP_ENG); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_ENG,IDB_SELPRODUCT_EXIT_0_ENG); 
		if(m_Step==1)
			m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_SAVE_ENG,IDB_BTSETUPMENU_SAVE_1_ENG); 
		else if(m_Step==2)
		{
			if(g_bAutoSetup)
				m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_NEXT_ENG,IDB_BTSETUPMENU_NEXT_1_ENG); 
			else
				m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_SAVE_ENG,IDB_BTSETUPMENU_SAVE_1_ENG); 
		}
		else
			m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_NEXT_ENG,IDB_BTSETUPMENU_NEXT_1_ENG); 
	}
	else if(g_NationalCode==2)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_AUTOSETUP_CHI); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_CHI,IDB_SELPRODUCT_EXIT_0_CHI); 
		if(m_Step==1)
			m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_SAVE_CHI,IDB_BTSETUPMENU_SAVE_1_CHI); 
		else if(m_Step==2)
		{
			if(g_bAutoSetup)
				m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_NEXT_CHI,IDB_BTSETUPMENU_NEXT_1_CHI); 
			else
				m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_SAVE_CHI,IDB_BTSETUPMENU_SAVE_1_CHI); 
		}
		else
			m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_NEXT_CHI,IDB_BTSETUPMENU_NEXT_1_CHI);
	}
	else if(g_NationalCode==3)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_AUTOSETUP_SPN); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_SPN,IDB_SELPRODUCT_EXIT_0_SPN); 
		if(m_Step==1)
			m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_SAVE_SPN,IDB_BTSETUPMENU_SAVE_1_SPN); 
		else if(m_Step==2)
		{
			if(g_bAutoSetup)
				m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_NEXT_SPN,IDB_BTSETUPMENU_NEXT_1_SPN); 
			else
				m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_SAVE_SPN,IDB_BTSETUPMENU_SAVE_1_SPN); 
		}
		else
			m_Bt_Next.LoadBitmaps(IDB_BTSETUPMENU_NEXT_SPN,IDB_BTSETUPMENU_NEXT_1_SPN);
	}

    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 

	m_Bt_Next.SizeToContent();	
	m_Bt_Close.SizeToContent();	
}

void CAutoSetup::InitPage(void)
{
	m_Pages[0]->Create(IDD_AUTOSETUP_0, this);
	m_Pages[1]->Create(IDD_AUTOSETUP_1, this);
	m_Pages[2]->Create(IDD_AUTOSETUP_2, this);
	m_Pages[3]->Create(IDD_AUTOSETUP_3, this);
	m_Pages[4]->Create(IDD_AUTOSETUP_4, this);

	if(m_Step==1)
	{
		m_Pages[3]->ShowWindow(SW_SHOW);
		m_Pages[0]->ShowWindow(SW_HIDE);
		m_Pages[4]->ShowWindow(SW_HIDE);
	}
	else if(m_Step==2)
	{
		m_Pages[4]->ShowWindow(SW_SHOW);
		m_Pages[0]->ShowWindow(SW_HIDE);
		m_Pages[3]->ShowWindow(SW_HIDE);
	}
	else
	{
		m_Pages[0]->ShowWindow(SW_SHOW);
		m_Pages[3]->ShowWindow(SW_HIDE);
		m_Pages[4]->ShowWindow(SW_HIDE);
	}
	m_Pages[1]->ShowWindow(SW_HIDE);
	m_Pages[2]->ShowWindow(SW_HIDE);

	CRect itemRect;
	int nX, nY, nXc, nYc;

	m_Page.GetWindowRect(&itemRect);
	ScreenToClient(&itemRect);
	nX=itemRect.left;
	nY=itemRect.top;
	nXc=itemRect.right;
	nYc=itemRect.bottom;

	if(m_Step==1)
	{
		m_Pages[3]->SetWindowPos(NULL, nX, nY, nXc-nX, nYc-nY, SWP_NOSIZE | SWP_NOZORDER |SWP_SHOWWINDOW);
		for(int nCount=0; nCount < m_nNumberOfPages; nCount++)
		{
			if(nCount!=3)
				m_Pages[nCount]->SetWindowPos(&wndTop,  nX, nY, nXc-nX, nYc-nY, SWP_NOSIZE | SWP_NOZORDER |SWP_HIDEWINDOW);
		}
	}
	else if(m_Step==2)
	{
		m_Pages[4]->SetWindowPos(NULL, nX, nY, nXc-nX, nYc-nY, SWP_NOSIZE | SWP_NOZORDER |SWP_SHOWWINDOW);
		for(int nCount=0; nCount < m_nNumberOfPages; nCount++)
		{
			if(nCount!=4)
				m_Pages[nCount]->SetWindowPos(&wndTop,  nX, nY, nXc-nX, nYc-nY, SWP_NOSIZE | SWP_NOZORDER |SWP_HIDEWINDOW);
		}
	}
	else
	{
		m_Pages[0]->SetWindowPos(NULL, nX, nY, nXc-nX, nYc-nY, SWP_NOSIZE | SWP_NOZORDER |SWP_SHOWWINDOW);
		for(int nCount=1; nCount < m_nNumberOfPages; nCount++)
		{
			m_Pages[nCount]->SetWindowPos(&wndTop,  nX, nY, nXc-nX, nYc-nY, SWP_NOSIZE | SWP_NOZORDER |SWP_HIDEWINDOW);
		}
	}
}

BOOL CAutoSetup::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAutoSetup::OnDestroy()
{
	CDialogEx::OnDestroy();

	for(int nCount=0; nCount <m_nNumberOfPages; nCount++)
	{
		if(m_Pages[nCount])
			delete m_Pages[nCount];
	}

}

void CAutoSetup::OnPaint()
{
	CPaintDC dc(this); // device context for painting

    CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}

bool CAutoSetup::OnNameUsed(CString Pname)
{
	Init_Mysql();

	MYSQL_RES* m_Res;
	//MYSQL_ROW row;
	//mysql_query(&m_mysql, "set names euckr");
	SqlQuery.Format(_T("SELECT * FROM intellisense_product WHERE ProductName = '%s'"), Pname);

	if (mysql_query(&m_mysql, CStringA(SqlQuery)))
	{
		CString error(mysql_error(&m_mysql));
		//AfxMessageBox(error);		// 에러메시지 로그에 남게??
		return false;
	}

	m_Res = mysql_store_result(&m_mysql);

	if (mysql_fetch_row(m_Res) == NULL)
	{
		mysql_free_result(m_Res);
		Close_Mysql();
		return true;
	}
	else
	{
		return false;
		Close_Mysql();
	}

	//if(!m_pSet_p.Open_DB(g_StrDirPath[0]))		
	//{
	//}
	//else
	//{
	//	CString strSQL;
	//	strSQL.Format(_T("SELECT * FROM Product WHERE ProductName = '%s'"), Pname);
	//	if(m_pSet_p.Select_DB(strSQL))
	//	{
	//		if(m_pSet_p.IsEOF())
	//		{
	//			if(m_pSet_p.b_IsOpenedDB)
	//				m_pSet_p.Close_DB();
	//			return true;
	//		}
	//		else
	//		{
	//			Pname="";
	//			if(m_pSet_p.b_IsOpenedDB)
	//				m_pSet_p.Close_DB();
	//			return false;
	//		}
	//	}
	//}
	//return false;
}


bool CAutoSetup::OnAddNewProduct(void)
{
	Init_Mysql();

	if (!g_Connecting)
		AfxMessageBox(_T("실패"));
	else
	{
		MYSQL_RES* m_Res;
		MYSQL_ROW row;

		mysql_query(&m_mysql, "set names euckr");

		SqlQuery.Format(_T("SELECT * FROM intellisense_product WHERE ProductName = '%s'"), g_ProductName);
		if (mysql_query(&m_mysql, CStringA(SqlQuery)))
		{
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}
		else
		{

			m_Res = mysql_store_result(&m_mysql);

			if ((row = mysql_fetch_row(m_Res)) == NULL)
			{
				CTime time;
				time = CTime::GetCurrentTime();
				CString strdate;
				strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(), time.GetMonth(), time.GetDay());
				g_DTGain = 2.0;
				CString strQuery;
				int xraykv, xrayma;
				if (g_bXrayAutoSetup)
				{
					xraykv = 60;
					xrayma = 2000;
					if (NSE_XRAY_KIND == 3)
						xraykv = 60;
					else if (NSE_XRAY_KIND == 2)
					{
						xraykv = 45;
						xrayma = 800;
					}
					//strQuery.Format(_T("Insert INTO intellisense_product(ProductName, ProductCode, ProductXraykv, ProductXrayma, ProductDTscanspeed, ProductConveyorSpeed, ProductXraykv2, ProductXrayma2, ProductDTGain, ProductMaskx, ProductMasky, ProductRejectUse, ProductRejectDelay, ProductRejectKeep, ProductDelD, ProductDelD_1, ProductDelD_2, ProductDelE, ProductDelE_1, ProductDelE_2, ProductEtc1, ProductEtc2,ProductEtc3, ProductEtc4, ProductEtc5, ProductSaveHard, ProductTalBright, ProductTalBright2, ProductUseOmit, ProductUseMass, ProductUseCount, ProductUseMask, ProductUseClip, ProductUseTalox, ProductMassCond1, ProductMassCond2, ProductMassCond3, ProductRegisterDate, ProductLastDate, ProductEtc6, ProductEtc12, ProductEtc13, ProductEtc14, ProductEtc15, ProductEtc16, ProductEtc17, ProductEtc20)") _T("VALUES('%s', %d, %d, %d, %d, %d, %d, %d, %f, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', '%s')"), g_ProductName, g_Code, xraykv, xrayma, g_DTScanspeed, g_ConveyorSpeed, g_Xray_kv2, g_Xray_ma2, g_DTGain, 0, 0, 0, NSE_BASIC_DELAY, NSE_BASIC_KEEP, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 100, strdate, strdate, 1, 1, 2, 3, 4, 5, 6, g_Barcode);
					strQuery.Format(_T("Insert INTO intellisense_product(ProductName, ProductCode, ProductXraykv, ProductXrayma, ProductDTscanspeed, ProductConveyorSpeed, ProductXraykv2, ProductXrayma2, ProductDTGain, ProductMaskx, ProductMasky, ProductRejectDelay, ProductRejectKeep, ProductDelD, ProductDelD_1, ProductDelD_2, ProductDelE, ProductDelE_1, ProductDelE_2, ProductEtc1, ProductEtc2, ProductEtc3, ProductEtc4, ProductEtc5, ProductSaveHard, ProductTalBright, ProductTalBright2, ProductUseOmit, ProductUseMass, ProductUseCount, ProductUseMask, ProductUseClip, ProductUseTalox, ProductMassCond1, ProductMassCond2, ProductMassCond3, ProductEtc9, ProductEtc10, ProductEtc11, ProductEtc12, ProductEtc13, ProductEtc14, ProductRegisterDate, ProductLastDate)") _T("VALUES('%s', %d, %d, %d, %d, %d, %d, %d, %f, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', '%s')"), g_ProductName, g_Code, xraykv, xrayma, g_DTScanspeed, g_ConveyorSpeed, g_Xray_kv2, g_Xray_ma2, g_DTGain, 0, 0, NSE_BASIC_DELAY, NSE_BASIC_KEEP, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 180, 1, 1, 1, 1, 1, 1, 0, 0, 100, 0, 0, 0, 0, 0, 0, strdate, strdate);
				}
				else
					//strQuery.Format(_T("Insert INTO intellisense_product(ProductName, ProductCode, ProductXraykv, ProductXrayma, ProductDTscanspeed, ProductConveyorSpeed, ProductXraykv2, ProductXrayma2, ProductDTGain, ProductMaskx, ProductMasky, ProductRejectUse, ProductRejectDelay, ProductRejectKeep, ProductDelD, ProductDelD_1, ProductDelD_2, ProductDelE, ProductDelE_1, ProductDelE_2, ProductEtc1, ProductEtc2, ProductEtc3, ProductEtc4, ProductEtc5, ProductSaveHard, ProductTalBright, ProductTalBright2, ProductUseOmit, ProductUseMass, ProductUseCount, ProductUseMask, ProductUseClip, ProductUseTalox, ProductMassCond1, ProductMassCond2, ProductMassCond3, ProductRegisterDate, ProductLastDate, ProductEtc6, ProductEtc12, ProductEtc13, ProductEtc14, ProductEtc15, ProductEtc16, ProductEtc17, ProductEtc20)") _T("VALUES('%s', %d, %d, %d, %d, %d, %d, %d, %f, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', '%s')"), g_ProductName, g_Code, g_Xray_kv, g_Xray_ma, g_DTScanspeed, g_ConveyorSpeed, g_Xray_kv2, g_Xray_ma2, g_DTGain, 0, 0, 0, NSE_BASIC_DELAY, NSE_BASIC_KEEP, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 100, strdate, strdate, 1, 1, 2, 3, 4, 5, 6, g_Barcode);
					strQuery.Format(_T("Insert INTO intellisense_product(ProductName, ProductCode, ProductXraykv, ProductXrayma, ProductDTscanspeed, ProductConveyorSpeed, ProductXraykv2, ProductXrayma2, ProductDTGain, ProductMaskx, ProductMasky, ProductRejectDelay, ProductRejectKeep, ProductDelD, ProductDelD_1, ProductDelD_2, ProductDelE, ProductDelE_1, ProductDelE_2, ProductEtc1, ProductEtc2, ProductEtc3, ProductEtc4, ProductEtc5, ProductSaveHard, ProductTalBright, ProductTalBright2, ProductUseOmit, ProductUseMass, ProductUseCount, ProductUseMask, ProductUseClip, ProductUseTalox, ProductMassCond1, ProductMassCond2, ProductMassCond3, ProductEtc9, ProductEtc10, ProductEtc11, ProductEtc12, ProductEtc13, ProductEtc14, ProductRegisterDate, ProductLastDate)") _T("VALUES('%s', %d, %d, %d, %d, %d, %d, %d, %f, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', '%s')"), g_ProductName, g_Code, xraykv, xrayma, g_DTScanspeed, g_ConveyorSpeed, g_Xray_kv2, g_Xray_ma2, g_DTGain, 0, 0, NSE_BASIC_DELAY, NSE_BASIC_KEEP, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 180, 1, 1, 1, 1, 1, 1, 0, 0, 100, 0, 0, 0, 0, 0, 0, strdate, strdate);

				if (mysql_query(&m_mysql, CStringA(strQuery)))
				{
					CString error(mysql_error(&m_mysql));
					AfxMessageBox(error);
				}
			}
		}

		mysql_free_result(m_Res);
	}

	Close_Mysql();

	/*if(!m_pSet_p.Open_DB(g_StrDirPath[0]))		
	{
	}
	else
	{
		CString strSQL;
		strSQL.Format(_T("SELECT * FROM Product WHERE ProductName = '%s'"), g_ProductName);
		if(m_pSet_p.Select_DB(strSQL))
		{
			if(m_pSet_p.IsEOF())
			{
				CTime time;
				time = CTime::GetCurrentTime();
				CString strdate;
				strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());	
				g_DTGain=2.0;
				CString strQuery;
				if(g_bXrayAutoSetup)
				{
					int xraykv=60;
					int xrayma=2000;
					if(NSE_XRAY_KIND==3)
						xraykv=60;
					else if(NSE_XRAY_KIND==2)
					{
						xraykv=45;
						xrayma=800;
					}
					strQuery.Format(_T("Insert INTO Product(ProductName, ProductCode, ProductXraykv, ProductXrayma, ProductDTscanspeed, ProductConveyorSpeed, ProductXraykv2, ProductXrayma2, ProductDTGain, ProductMaskx, ProductMasky, ProductRejectDelay, ProductRejectKeep, ProductDelD, ProductDelD_1, ProductDelD_2, ProductDelE, ProductDelE_1, ProductDelE_2, ProductEtc1, ProductEtc2, ProductEtc3, ProductEtc4, ProductEtc5, ProductSaveHard, ProductTalBright, ProductTalBright2, ProductUseOmit, ProductUseMass, ProductUseCount, ProductUseMask, ProductUseClip, ProductUseTalox, ProductMassCond1, ProductMassCond2, ProductMassCond3, ProductEtc9, ProductEtc10, ProductEtc11, ProductEtc12, ProductEtc13, ProductEtc14, ProductRegisterDate, ProductLastDate)") _T("VALUES('%s', %d, %d, %d, %d, %d, %d, %d, %f, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', '%s')"), g_ProductName, g_Code, xraykv, xrayma, g_DTScanspeed, g_ConveyorSpeed, g_Xray_kv2, g_Xray_ma2, g_DTGain, 0, 0, NSE_BASIC_DELAY, NSE_BASIC_KEEP, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 180, 1, 1, 1, 1, 1, 1, 0, 0, 100, 0, 0, 0, 0, 0, 0, strdate, strdate);
				}
				else
					strQuery.Format(_T("Insert INTO Product(ProductName, ProductCode, ProductXraykv, ProductXrayma, ProductDTscanspeed, ProductConveyorSpeed, ProductXraykv2, ProductXrayma2, ProductDTGain, ProductMaskx, ProductMasky, ProductRejectDelay, ProductRejectKeep, ProductDelD, ProductDelD_1, ProductDelD_2, ProductDelE, ProductDelE_1, ProductDelE_2, ProductEtc1, ProductEtc2, ProductEtc3, ProductEtc4, ProductEtc5, ProductSaveHard, ProductTalBright, ProductTalBright2, ProductUseOmit, ProductUseMass, ProductUseCount, ProductUseMask, ProductUseClip, ProductUseTalox, ProductMassCond1, ProductMassCond2, ProductMassCond3, ProductEtc9, ProductEtc10, ProductEtc11, ProductEtc12, ProductEtc13, ProductEtc14, ProductRegisterDate, ProductLastDate)") _T("VALUES('%s', %d, %d, %d, %d, %d, %d, %d, %f, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', '%s')"), g_ProductName, g_Code, g_Xray_kv, g_Xray_ma, g_DTScanspeed, g_ConveyorSpeed, g_Xray_kv2, g_Xray_ma2, g_DTGain, 0, 0, NSE_BASIC_DELAY, NSE_BASIC_KEEP, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 180, 1, 1, 1, 1, 1, 1, 0, 0, 100, 0, 0, 0, 0, 0, 0, strdate, strdate);
				m_pSet_p.Insert_DB(strQuery);
			}
		}
		else
		{
			if(m_pSet_p.b_IsOpenedDB)
				m_pSet_p.Close_DB();
			return false;
		}

		if(m_pSet_p.b_IsOpenedDB)
			m_pSet_p.Close_DB();
	}
	*/
	return true;
}


bool CAutoSetup::OnAddCondition(void)
{
	if(g_AutoCount!=0)
	{
		Init_Mysql();

		//MYSQL_RES* m_Res;
		//MYSQL_ROW row;



		/*if(!m_pSet_p.Open_DB(g_StrDirPath[0]))		
		{
			
		}
		else
		{*/
			if(g_AutoCond1_1+20<=255 && g_AutoCond1_1!=0)
				g_AutoCond1_1+=20;
			else
				g_AutoCond1_1=0;
			if(g_AutoCond2_1+7<=255 && g_AutoCond2_1!=0)
				g_AutoCond2_1+=7;
			else
			{
				if(g_AutoCond2_1==0)
					g_AutoCond2_1=50;
				else
					g_AutoCond2_1=0;
			}
			if(g_AutoCond3_1+7<=255 && g_AutoCond3_1!=0)
				g_AutoCond3_1+=7;
			else
			{
				if(g_AutoCond3_1==0)
					g_AutoCond3_1=50;
				else
					g_AutoCond3_1=0;
			}
			if(g_AutoCond4_1+7<=255 && g_AutoCond4_1!=0)
				g_AutoCond4_1+=7;
			else
			{
				if(g_AutoCond4_1==0)
					g_AutoCond4_1=50;
				else
					g_AutoCond4_1=0;
			}
			if(g_AutoCond5_1+15<=255 && g_AutoCond5_1!=0)
				g_AutoCond5_1+=15;
			else
				g_AutoCond5_1=0;
			if(g_AutoCond6_1+15<=255 && g_AutoCond6_1!=0)
				g_AutoCond6_1+=15;
			else
				g_AutoCond6_1=0;

			CString strQuery;
			strQuery.Format(_T("Update intellisense_product SET ProductCond1 = %d, ProductCond2 = %d, ProductCond3 = %d, ProductCond4 = %d, ProductCond5 = %d, ProductCond6 = %d WHERE ProductName = '%s'"),
				g_AutoCond1_1, g_AutoCond2_1, g_AutoCond3_1, g_AutoCond4_1, g_AutoCond5_1, g_AutoCond6_1, g_ProductName);

			if (mysql_query(&m_mysql, CStringA(strQuery)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}

			Close_Mysql();

			/*CString strQuery;
			strQuery.Format(_T("Update Product SET ProductCond1 = %d, ProductCond2 = %d, ProductCond3 = %d, ProductCond4 = %d, ProductCond5 = %d, ProductCond6 = %d WHERE ProductName = '%s'"),
				g_AutoCond1_1, g_AutoCond2_1, g_AutoCond3_1, g_AutoCond4_1, g_AutoCond5_1, g_AutoCond6_1, g_ProductName);
			m_pSet_p.Update_DB(strQuery);

			if(m_pSet_p.b_IsOpenedDB)
				m_pSet_p.Close_DB();*/
		//}
		return true;
	}
	else
	{
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[1];//"제품을 1회이상 통과시켜 주세요.";
		dlg.DoModal();
		return false;
	}

}

bool CAutoSetup::OnAddXray(void)
{
	if(g_AutoXrayCount!=0)
	{
		Init_Mysql();

		//MYSQL_RES* m_Res;
		//MYSQL_ROW row;
		mysql_query(&m_mysql, "set names euckr");

		CString strQuery;
		strQuery.Format(_T("Update intellisense_product SET ProductXraykv = %d, ProductXrayma = %d, ProductXraykv2 = %d, ProductXrayma2 = %d WHERE ProductName = '%s'"),
			g_Xray_kv, g_Xray_ma, g_Xray_kv2, g_Xray_ma2, g_ProductName);

		if (mysql_query(&m_mysql, CStringA(strQuery))) {
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}

		Close_Mysql();

		/*if(m_pSet_p.Open_DB(g_StrDirPath[0]))		
		{
			CString strQuery;
			strQuery.Format(_T("Update Product SET ProductXraykv = %d, ProductXrayma = %d, ProductXraykv2 = %d, ProductXrayma2 = %d WHERE ProductName = '%s'"),
				g_Xray_kv, g_Xray_ma, g_Xray_kv2, g_Xray_ma2, g_ProductName);
			m_pSet_p.Update_DB(strQuery);
		}

		if(m_pSet_p.b_IsOpenedDB)
			m_pSet_p.Close_DB();*/
		return true;
	}

	else
	{		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[1];//"제품을 1회이상 통과시켜 주십시요.";
		dlg.DoModal();
		return false;
	}

}


void CAutoSetup::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(g_bAutoSetup || g_bXrayAutoSetup)
	{
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[2];//"취소되었습니다. 검사값은 추후 별도 설정 가능합니다.";
		dlg.DoModal();
		g_bAutoSetup=false;
		g_bXrayAutoSetup=false;
	}
	else
	{
		if(m_OldProductName!="")
			g_ProductName=m_OldProductName;
		else
			g_ProductName="";
	}

	CDialogEx::OnCancel();
}

void CAutoSetup::OnBnClickedBtNext()
{
	CString temp;
	bool IsOk =true;
	int m_IsNow=0,m_Iscopy=0;
	CAutoSetup_0 copy;
    m_Iscopy=((CButton*)m_Pages[0]->GetDlgItem(IDC_COPY))->GetCheck();
	int m_IsNowXray=0;

	switch(m_CurrentPage)
	{
		case 0:
			if(g_ProductName!="")
				m_OldProductName=g_ProductName;
			
			m_Pages[0]->GetDlgItem(IDC_PNAME)->GetWindowText(temp);
			if(temp=="")
			{
				CMessageBoxDlg dlg;
				dlg.m_Message=g_MessageArr[3];//"제품명을 입력하세요.";
				dlg.DoModal();
				return;
			}
			else
			{
				if(OnNameUsed(temp))
				{
					g_ProductName=temp;
				}
				else
				{
					temp="";
					CMessageBoxDlg dlg;
					dlg.m_Message=g_MessageArr[4];//"동일한 제품명이 존재합니다.";
					dlg.DoModal();
					m_Pages[0]->GetDlgItem(IDC_PNAME)->SetWindowText(temp);
					return;
				}
			}
			
			if(m_Iscopy==1)
			{
				CProduct_Copy dlg;
				int result=dlg.DoModal();
				if(result==IDOK)
				{
					EndDialog(IDD_AUTOSETUP_0);			
				}
				else
				{	
					g_ProductName=m_OldProductName;
					EndDialog(IDCANCEL);
				}
				break;
			}

			break;
		case 1:
			m_Pages[1]->GetDlgItem(IDC_PKV)->GetWindowText(temp);
			g_Xray_kv=_wtoi(temp);
			m_Pages[1]->GetDlgItem(IDC_PUA)->GetWindowText(temp);
			g_Xray_ma=_wtoi(temp);
			g_Xray_kv2=NSE_BASIC_KV2;
			g_Xray_ma2=NSE_BASIC_MA2;
			m_Pages[1]->GetDlgItem(IDC_PSCANSPEED)->GetWindowText(temp);
			g_DTScanspeed=_wtoi(temp);

			if(g_Xray_kv*g_Xray_ma > NSE_XRAY_POWER)
			{
				CMessageBoxDlg dlg;
				dlg.m_Message=g_MessageArr[5];//"X-Ray 출력을 초과 했습니다.";
				dlg.DoModal();
				return;
			}
			//temp.Format("%s %d %d %d %d ", g_ProductName, g_Code, g_Xray_kv, g_Xray_ma, g_DTScanspeed);
			
			break;

		case 2:
			m_IsNow=((CButton*)m_Pages[2]->GetDlgItem(IDC_PISNOW))->GetCheck(); 
			
			m_IsNowXray=((CButton*)m_Pages[2]->GetDlgItem(IDC_PISNOWXRAY))->GetCheck(); 

			if(m_IsNow==1 && m_IsNowXray==1)
			{
				g_bXrayAutoSetup=true;
				g_bAutoSetup=true;
				OnAddNewProduct();
				EndDialog(55);
			}
			else if(m_IsNow==1 && m_IsNowXray==0)
			{
				g_bXrayAutoSetup=false;
				g_bAutoSetup=true;
				OnAddNewProduct();
				EndDialog(33);
			}
			else if(m_IsNow==0 && m_IsNowXray==1)
			{
				g_bXrayAutoSetup=true;
				g_bAutoSetup=false;
				OnAddNewProduct();
				EndDialog(44);
			}
			else
			{
				g_bAutoSetup=false;
				g_bXrayAutoSetup=false;
				IsOk =	OnAddNewProduct();
				if(IsOk)
					//AfxMessageBox("저장되었습니다.");
					AfxMessageBox(g_MessageArr[6]);//
				else
				{
					if(m_OldProductName!="")
						g_ProductName=m_OldProductName;
					else
						g_ProductName="";
					AfxMessageBox(g_MessageArr[7]);//_T("이미 등록되어 있는 제품입니다."));
				}

				EndDialog(IDOK);
			}
			break;

		case 3:
				if(!OnAddXray())
					return;
				if(!g_bAutoSetup)
					AfxMessageBox(g_MessageArr[6]);//_T("저장되었습니다."));
				EndDialog(IDOK);
			break;

		case 4:
				if(!OnAddCondition())
					return;
				EndDialog(IDOK);
			break;
	}
		
	m_CurrentPage++;

	for(int i=0; i<m_nNumberOfPages; i++)
	{
		if(m_CurrentPage==i)
		{
			m_Pages[m_CurrentPage]->ShowWindow(SW_SHOW);
		}
		else
		{
			m_Pages[i]->ShowWindow(SW_HIDE);
		}
	}	
}
