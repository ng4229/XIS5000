// SetupAutoXray.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "SetupAutoXray.h"
#include "afxdialogex.h"

#include "MessageBoxDlg.h"
// CSetupAutoXray 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupAutoXray, CDialogEx)

CSetupAutoXray::CSetupAutoXray(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupAutoXray::IDD, pParent)
{
	g_AutoXrayCount=0;
	g_AutoXrayValue=0;
}

CSetupAutoXray::~CSetupAutoXray()
{
}

void CSetupAutoXray::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_SAVE, m_Bt_Save);
	DDX_Control(pDX, IDCANCEL, m_Bt_Close);
}


BEGIN_MESSAGE_MAP(CSetupAutoXray, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BT_SAVE, &CSetupAutoXray::OnBnClickedBtSave)
END_MESSAGE_MAP()


// CSetupAutoXray 메시지 처리기입니다.


BOOL CSetupAutoXray::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(255,255,255));
	SetWindowPos(NULL, 672, 119, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	// TODO: Add extra initialization here
	LoadBackground();
	SetTimer(1,1000,NULL);	
	GetDlgItem(IDC_PKV)->SetWindowText(_T("..."));
	GetDlgItem(IDC_PMA)->SetWindowText(_T("..."));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSetupAutoXray::LoadBackground()
{	
	int mainbgnum=0;
	if(g_NationalCode==0)
	{
		mainbgnum=IDB_BG_SETUPAUTOXRAY; 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE,IDB_BTSETUPMENU_SAVE_1); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT,IDB_SELPRODUCT_EXIT_0); 

	}
	else if(g_NationalCode==1)
	{
		mainbgnum=IDB_BG_SETUPAUTOXRAY_ENG; 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_ENG,IDB_BTSETUPMENU_SAVE_1_ENG); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_ENG,IDB_SELPRODUCT_EXIT_0_ENG); 
	}
	else if(g_NationalCode==2)
	{
		mainbgnum=IDB_BG_SETUPAUTOXRAY_CHI; 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_CHI,IDB_BTSETUPMENU_SAVE_1_CHI); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_CHI,IDB_SELPRODUCT_EXIT_0_CHI); 
	}
	else if(g_NationalCode==3)
	{
		mainbgnum=IDB_BG_SETUPAUTOXRAY_SPN; 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_SPN,IDB_BTSETUPMENU_SAVE_1_SPN); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_SPN,IDB_SELPRODUCT_EXIT_0_SPN); 
	}

	SetBackgroundImage(mainbgnum, CDialogEx::BACKGR_TOPLEFT);

	SetDlgItemText(IDC_STATIC_1, g_MessageArr[29]);//_T("제품을 5회이상 투입하세요."));
	SetDlgItemText(IDC_STATIC_2, g_MessageArr[30]);//_T("투입횟수"));
	SetDlgItemText(IDC_STATIC_3, g_MessageArr[31]);//_T("X-ray 조사량 최적값"));


	m_Bt_Save.SizeToContent();
	m_Bt_Close.SizeToContent();	
}

BOOL CSetupAutoXray::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;		

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupAutoXray::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent==1)
	{
		SetDlgItemInt(IDC_PCOUNT, g_AutoXrayCount);
		if(g_AutoXrayCount>0)
			OnAddCondition();
	}	
	CDialogEx::OnTimer(nIDEvent);
}


void CSetupAutoXray::OnDestroy()
{
	KillTimer(1);
	CDialogEx::OnDestroy();
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CSetupAutoXray::OnAddCondition()
{

	if(NSE_XRAY_KIND==2 || (NSE_XRAY_KIND==0 && NSE_XRAY_KV==80))
	{
		if(g_AutoXrayValue<=10)
		{
			g_Xray_kv=80;
			g_Xray_ma=1000;
			g_Xray_kv2=30;
			g_Xray_ma2=200;
		}
		else if(g_AutoXrayValue>10 && g_AutoXrayValue<=20)
		{
			g_Xray_kv=80;
			g_Xray_ma=1000;
			g_Xray_kv2=20;
			g_Xray_ma2=200;
		}
		else if(g_AutoXrayValue>20 && g_AutoXrayValue<=30)
		{
			g_Xray_kv=80;
			g_Xray_ma=1000;
			g_Xray_kv2=10;
			g_Xray_ma2=200;
		}
		else if(g_AutoXrayValue>30 && g_AutoXrayValue<=40)
		{
			g_Xray_kv=70;
			g_Xray_ma=1000;
			g_Xray_kv2=10;
			g_Xray_ma2=200;
		}
		else if(g_AutoXrayValue>40 && g_AutoXrayValue<=200)
		{
			g_Xray_kv=(-25*g_AutoXrayValue)/100+90;
			if(NSE_XRAY_KIND==0)
			{
				if(g_Xray_kv<60)
					g_Xray_kv=60;
				g_Xray_ma=1000;
			}
			else
			{
				if(g_Xray_kv<50)
					g_Xray_kv=50;
				g_Xray_ma=800;
			}
			g_Xray_kv2=0;
			g_Xray_ma2=0;
		}
		else
		{
			
			g_Xray_kv=45;
			g_Xray_ma=800;
			if(NSE_XRAY_KIND==0)
			{
				g_Xray_kv=60;
				g_Xray_ma=1000;
			}
			g_Xray_kv2=0;
			g_Xray_ma2=0;
		}
	}
	else if(NSE_XRAY_KIND==3)
	{
		if(g_AutoXrayValue<=10)
		{
			g_Xray_kv=80;
			g_Xray_ma=2500;
			g_Xray_kv2=30;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>10 && g_AutoXrayValue<=20)
		{
			g_Xray_kv=80;
			g_Xray_ma=2500;
			g_Xray_kv2=20;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>20 && g_AutoXrayValue<=30)
		{
			g_Xray_kv=80;
			g_Xray_ma=2500;
			g_Xray_kv2=10;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>30 && g_AutoXrayValue<=40)
		{
			g_Xray_kv=70;
			g_Xray_ma=2500;
			g_Xray_kv2=10;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>40 && g_AutoXrayValue<=200)
		{
			g_Xray_kv=(-25*g_AutoXrayValue)/100+90;
			g_Xray_ma=2500;
			g_Xray_kv2=0;
			g_Xray_ma2=0;
		}
		else
		{
			g_Xray_kv=40;
			g_Xray_ma=3000;
			g_Xray_kv2=0;
			g_Xray_ma2=0;
		}
	}
	else
	{
		if(g_AutoXrayValue<=10)
		{
			g_Xray_kv=100;
			g_Xray_ma=3000;
			g_Xray_kv2=30;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>10 && g_AutoXrayValue<=20)
		{
			g_Xray_kv=100;
			g_Xray_ma=3000;
			g_Xray_kv2=20;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>20 && g_AutoXrayValue<=30)
		{
			g_Xray_kv=100;
			g_Xray_ma=3000;
			g_Xray_kv2=10;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>30 && g_AutoXrayValue<=40)
		{
			g_Xray_kv=90;
			g_Xray_ma=2500;
			g_Xray_kv2=10;
			g_Xray_ma2=1000;
		}
		else if(g_AutoXrayValue>40 && g_AutoXrayValue<=200)
		{
			g_Xray_kv=(-25*g_AutoXrayValue)/100+90;
			g_Xray_ma=2500;
			g_Xray_kv2=0;
			g_Xray_ma2=0;
		}
		else
		{
			g_Xray_kv=40;
			g_Xray_ma=3000;
			g_Xray_kv2=0;
			g_Xray_ma2=0;
		}
	}

	CString str;
	if(g_Xray_kv2==0)
		str.Format(_T("%d"), g_Xray_kv);
	else
		str.Format(_T("%d(%d)"), g_Xray_kv, g_Xray_kv2);
	GetDlgItem(IDC_PKV)->SetWindowText(str);
	if(g_Xray_ma2==0)
		str.Format(_T("%d"), g_Xray_ma);
	else
		str.Format(_T("%d(%d)"), g_Xray_ma, g_Xray_ma2);
	GetDlgItem(IDC_PMA)->SetWindowText(str);
	
}	

void CSetupAutoXray::OnBnClickedBtSave()
{
	if(g_AutoXrayCount!=0)
	{
		OnAddCondition();

		if (!g_Connecting)
			AfxMessageBox(_T("실패"));
		else
		{
			CString strQuery;
			strQuery.Format(_T("Update intellisense_product SET ProductXraykv = %d, ProductXrayma = %d, ProductXraykv2 = %d, ProductXrayma2 = %d WHERE ProductName = '%s'"),
				g_Xray_kv, g_Xray_ma, g_Xray_kv2, g_Xray_ma2, g_ProductName);

			if (mysql_query(&m_mysql, CStringA(strQuery)))
			{
				CString error(mysql_error(&m_mysql));
				AfxMessageBox(error);
			}
		}

		Close_Mysql();

		//if(m_pSet_p.Open_DB(g_StrDirPath[0]))		
		//{
		//	CString strQuery;
		//	strQuery.Format(_T("Update Product SET ProductXraykv = %d, ProductXrayma = %d, ProductXraykv2 = %d, ProductXrayma2 = %d WHERE ProductName = '%s'"),
		//		g_Xray_kv, g_Xray_ma, g_Xray_kv2, g_Xray_ma2, g_ProductName);
		//	m_pSet_p.Update_DB(strQuery);
		//}

		//if(m_pSet_p.b_IsOpenedDB)
		//	m_pSet_p.Close_DB();

		//AfxMessageBox(g_MessageArr[6]);//);
		//AfxMessageBox(_T("Settings saved."));
		EndDialog(IDOK);
	}
	else
	{
		CMessageBoxDlg dlg;
		dlg.m_Message=g_MessageArr[29];//"제품을 1회이상 통과시켜 주십시요.";
		//dlg.m_Message="Please feed product into the machine one or more times.";
		dlg.DoModal();
	}
}
