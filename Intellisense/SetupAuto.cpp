// SetupAuto.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "SetupAuto.h"
#include "afxdialogex.h"
#include "MessageBoxDlg.h"


// CSetupAuto 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupAuto, CDialogEx)

CSetupAuto::CSetupAuto(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupAuto::IDD, pParent)
{
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
}

CSetupAuto::~CSetupAuto()
{
}

void CSetupAuto::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_SAVE, m_Bt_Save);
	DDX_Control(pDX, IDCANCEL, m_Bt_Close);
}


BEGIN_MESSAGE_MAP(CSetupAuto, CDialogEx)
//	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BT_SAVE, &CSetupAuto::OnBnClickedBtSave)
	ON_BN_CLICKED(IDCANCEL, &CSetupAuto::OnBnClickedCancel)
//	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSetupAuto 메시지 처리기입니다.


BOOL CSetupAuto::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255,255,255));
	SetWindowPos(NULL, 672, 119, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	LoadBackground();
	SetTimer(1,1000,NULL);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupAuto::LoadBackground()
{
	int mainbgnum=0;
	if(g_NationalCode==0)
	{
		mainbgnum=IDB_BG_SETUPAUTO; 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE,IDB_BTSETUPMENU_SAVE_1); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT,IDB_SELPRODUCT_EXIT_0); 
	}
	else if(g_NationalCode==1)
	{
		mainbgnum=IDB_BG_SETUPAUTO_ENG; 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_ENG,IDB_BTSETUPMENU_SAVE_1_ENG); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_ENG,IDB_SELPRODUCT_EXIT_0_ENG); 
	}
	else if(g_NationalCode==2)
	{
		mainbgnum=IDB_BG_SETUPAUTO_CHI; 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_CHI,IDB_BTSETUPMENU_SAVE_1_CHI); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_CHI,IDB_SELPRODUCT_EXIT_0_CHI); 
	}
	else if(g_NationalCode==3)
	{
		mainbgnum=IDB_BG_SETUPAUTO_SPN; 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_SPN,IDB_BTSETUPMENU_SAVE_1_SPN); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_SPN,IDB_SELPRODUCT_EXIT_0_SPN); 
	}

	SetBackgroundImage(mainbgnum, CDialogEx::BACKGR_TOPLEFT);

	SetDlgItemText(IDC_STATIC_1, g_MessageArr[19]);//_T("제품을 5회이상 투입하세요."));
	SetDlgItemText(IDC_STATIC_2, g_MessageArr[20]);//_T("투입횟수"));
	SetDlgItemText(IDC_STATIC_3, g_MessageArr[21]);//_T("측정값"));
	SetDlgItemText(IDC_STATIC_4, g_MessageArr[22]);//_T("평균값"));
	SetDlgItemText(IDC_STATIC_5, g_MessageArr[23]);//_T("조건1"));
	SetDlgItemText(IDC_STATIC_6, g_MessageArr[24]);//_T("조건2"));
	SetDlgItemText(IDC_STATIC_7, g_MessageArr[25]);//_T("조건3"));
	SetDlgItemText(IDC_STATIC_8, g_MessageArr[26]);//_T("조건4"));
	SetDlgItemText(IDC_STATIC_9, g_MessageArr[27]);//_T("조건5"));
	SetDlgItemText(IDC_STATIC_10, g_MessageArr[28]);//_T("조건6"));

	m_Bt_Save.SizeToContent();	
	m_Bt_Close.SizeToContent();
}


BOOL CSetupAuto::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;		

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupAuto::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent==1)
	{
		ShowResult();
	}	
	CDialogEx::OnTimer(nIDEvent);
}


void CSetupAuto::OnDestroy()
{
	KillTimer(1);	

	CDialogEx::OnDestroy();
}


void CSetupAuto::OnBnClickedBtSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(g_AutoCount!=0)
	{
		OnAddCondition();
		AfxMessageBox(g_MessageArr[6]);//_T("저장되었습니다."));
		EndDialog(IDOK);
	}
	else
	{
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[1];//"제품을 1회이상 통과시켜 주십시요.";
		//dlg.m_Message="Please feed product into the machine one or more times.";
		dlg.DoModal();
	}
}


void CSetupAuto::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(g_bAutoSetup)
	{
		g_bAutoSetup=false;
	}
	CDialogEx::OnCancel();
}

void CSetupAuto::OnAddCondition()
{
	//20131226
	Init_Mysql();

	if (!g_Connecting)
		AfxMessageBox(_T("실패"));
	else
	{
		if (g_AutoCond1_1 + 20 <= 255 && g_AutoCond1_1 != 0)
			g_AutoCond1_1 += 20;
		else
			g_AutoCond1_1 = 0;
		if (g_AutoCond2_1 + 7 <= 255 && g_AutoCond2_1 != 0)
			g_AutoCond2_1 += 7;
		else
		{
			if (g_AutoCond2_1 == 0)
				g_AutoCond2_1 = 50;
			else
				g_AutoCond2_1 = 0;
		}
		if (g_AutoCond3_1 + 7 <= 255 && g_AutoCond3_1 != 0)
			g_AutoCond3_1 += 7;
		else
		{
			if (g_AutoCond3_1 == 0)
				g_AutoCond3_1 = 50;
			else
				g_AutoCond3_1 = 0;
		}
		if (g_AutoCond4_1 + 7 <= 255 && g_AutoCond4_1 != 0)
			g_AutoCond4_1 += 7;
		else
		{
			if (g_AutoCond4_1 == 0)
				g_AutoCond4_1 = 50;
			else
				g_AutoCond4_1 = 0;
		}
		if (g_AutoCond5_1 + 15 <= 255 && g_AutoCond5_1 != 0)
			g_AutoCond5_1 += 15;
		else
			g_AutoCond5_1 = 0;
		if (g_AutoCond6_1 + 15 <= 255 && g_AutoCond6_1 != 0)
			g_AutoCond6_1 += 15;
		else
			g_AutoCond6_1 = 0;

		CString strQuery;
		strQuery.Format(_T("Update intellisense_product SET ProductCond1 = %d, ProductCond2 = %d, ProductCond3 = %d, ProductCond4 = %d, ProductCond5 = %d, ProductCond6 = %d WHERE ProductName = '%s'"),
			g_AutoCond1_1, g_AutoCond2_1, g_AutoCond3_1, g_AutoCond4_1, g_AutoCond5_1, g_AutoCond6_1, g_ProductName);
		
		if (mysql_query(&m_mysql, CStringA(strQuery)))
		{
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
	//	if(g_AutoCond1_1+20<=255 && g_AutoCond1_1!=0)
	//		g_AutoCond1_1+=20;
	//	else
	//		g_AutoCond1_1=0;
	//	if(g_AutoCond2_1+7<=255 && g_AutoCond2_1!=0)
	//		g_AutoCond2_1+=7;
	//	else
	//	{
	//		if(g_AutoCond2_1==0)
	//			g_AutoCond2_1=50;
	//		else
	//			g_AutoCond2_1=0;
	//	}
	//	if(g_AutoCond3_1+7<=255 && g_AutoCond3_1!=0)
	//		g_AutoCond3_1+=7;
	//	else
	//	{
	//		if(g_AutoCond3_1==0)
	//			g_AutoCond3_1=50;
	//		else
	//			g_AutoCond3_1=0;
	//	}
	//	if(g_AutoCond4_1+7<=255 && g_AutoCond4_1!=0)
	//		g_AutoCond4_1+=7;
	//	else
	//	{
	//		if(g_AutoCond4_1==0)
	//			g_AutoCond4_1=50;
	//		else
	//			g_AutoCond4_1=0;
	//	}
	//	if(g_AutoCond5_1+15<=255 && g_AutoCond5_1!=0)
	//		g_AutoCond5_1+=15;
	//	else
	//		g_AutoCond5_1=0;
	//	if(g_AutoCond6_1+15<=255 && g_AutoCond6_1!=0)
	//		g_AutoCond6_1+=15;
	//	else
	//		g_AutoCond6_1=0;

	//	CString strQuery;
	//	strQuery.Format(_T("Update Product SET ProductCond1 = %d, ProductCond2 = %d, ProductCond3 = %d, ProductCond4 = %d, ProductCond5 = %d, ProductCond6 = %d WHERE ProductName = '%s'"),
	//		g_AutoCond1_1, g_AutoCond2_1, g_AutoCond3_1, g_AutoCond4_1, g_AutoCond5_1, g_AutoCond6_1, g_ProductName);
	//	m_pSet_p.Update_DB(strQuery);
	//}

	//if(m_pSet_p.b_IsOpenedDB)
	//	m_pSet_p.Close_DB();
}

void CSetupAuto::ShowResult()
{
	SetDlgItemInt(IDC_PCOUNT, g_AutoCount, true);

	SetDlgItemInt(IDC_PCOND1, g_AutoCond1, true);
	SetDlgItemInt(IDC_PCOND2, g_AutoCond2, true);
	SetDlgItemInt(IDC_PCOND3, g_AutoCond3, true);
	SetDlgItemInt(IDC_PCOND4, g_AutoCond4, true);
	SetDlgItemInt(IDC_PCOND5, g_AutoCond5, true);
	SetDlgItemInt(IDC_PCOND6, g_AutoCond6, true);

	SetDlgItemInt(IDC_PCOND1_1, g_AutoCond1_1, true);
	SetDlgItemInt(IDC_PCOND2_1, g_AutoCond2_1, true);
	SetDlgItemInt(IDC_PCOND3_1, g_AutoCond3_1, true);
	SetDlgItemInt(IDC_PCOND4_1, g_AutoCond4_1, true);
	SetDlgItemInt(IDC_PCOND5_1, g_AutoCond5_1, true);
	SetDlgItemInt(IDC_PCOND6_1, g_AutoCond6_1, true);
}
