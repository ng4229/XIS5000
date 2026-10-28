// SetupMenu_2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Intellisense.h"
#include "SetupMenu_2.h"
#include "afxdialogex.h"

#include "NumPadBack.h"
// CSetupMenu_2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMenu_2, CDialogEx)

CSetupMenu_2::CSetupMenu_2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMenu_2::IDD, pParent)
	, m_RejectRate(0)
{

	m_SetupRejectKeep = 0;
	m_SetupRejectDelay = 0;
	m_SetupRejectUse = 0;
	m_SetupHandCycle = 0;
}

CSetupMenu_2::~CSetupMenu_2()
{
}

void CSetupMenu_2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_BT_SAVE, m_Bt_Close);
	DDX_Control(pDX, IDC_BT_SAVE, m_Bt_Save);
	DDX_Control(pDX, IDCANCEL, m_Bt_Close);
	DDX_Control(pDX, IDC_REJECTDELAY_UP, m_Btrejectdelay_Up);
	DDX_Control(pDX, IDC_REJECTDELAY_DN, m_Btrejectdelay_Dn);
	DDX_Control(pDX, IDC_REJECTKEEP_DN, m_Btrejectkeep_Dn);
	DDX_Control(pDX, IDC_REJECTKEEP_UP, m_Btrejectkeep_Up);
	DDX_Text(pDX, IDC_SETUPREJECTKEEP, m_SetupRejectKeep);
	DDX_Text(pDX, IDC_SETUPREJECTDELAY, m_SetupRejectDelay);
	DDX_Radio(pDX, IDC_SETUPREJECTUSE, m_SetupRejectUse);
	DDX_Check(pDX, IDC_APPLYALL, b_ApplyAll);
	DDX_Control(pDX, IDC_HANDCYCLE_UP, m_Bthandcycle_Up);
	DDX_Control(pDX, IDC_HANDCYCLE_DN, m_Bthandcycle_Dn);
	DDX_Text(pDX, IDC_SETUPHANDCYCLE, m_SetupHandCycle);
	DDX_Text(pDX, IDC_SETUPHANDCYCLE2, m_RejectRate);
	DDX_Control(pDX, IDC_HANDCYCLE_UP2, m_Bthandcycle_Up2);
	DDX_Control(pDX, IDC_HANDCYCLE_DN2, m_Bthandcycle_Dn2);
}


BEGIN_MESSAGE_MAP(CSetupMenu_2, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BT_SAVE, &CSetupMenu_2::OnBnClickedBtSave)
	ON_BN_CLICKED(IDC_SETUPREJECTUSE, &CSetupMenu_2::OnClickedSetuprejectuse)
	ON_COMMAND(IDC_SETUPREJECTUSE1, &CSetupMenu_2::OnSetuprejectuse1)
	ON_COMMAND(IDC_SETUPREJECTUSE2, &CSetupMenu_2::OnSetuprejectuse2)
	ON_BN_CLICKED(IDC_APPLYALL, &CSetupMenu_2::OnClickedApplyall)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSetupMenu_2 메시지 처리기입니다.


BOOL CSetupMenu_2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Brush.CreateStockObject(NULL_BRUSH);
	SetWindowPos(NULL, 672, 119, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	LoadBackground();
	LoadValue();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSetupMenu_2::LoadBackground()
{
	BITMAP bmpInfo;
	if(g_NationalCode==0)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_2); 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE,IDB_BTSETUPMENU_SAVE_1);
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT,IDB_SELPRODUCT_EXIT_0); 
	}
	else if(g_NationalCode==1)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_2_ENG); 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_ENG,IDB_BTSETUPMENU_SAVE_1_ENG); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_ENG,IDB_SELPRODUCT_EXIT_0_ENG); 
	}
	else if(g_NationalCode==2)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_2_CHI); 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_CHI,IDB_BTSETUPMENU_SAVE_1_CHI); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_CHI,IDB_SELPRODUCT_EXIT_0_CHI); 
	}
	else if(g_NationalCode==3)
	{
		m_bmpMainBG.LoadBitmap(IDB_BG_SETUPMENU_2_SPN); 
		m_Bt_Save.LoadBitmaps(IDB_BTSETUPMENU_SAVE_SPN,IDB_BTSETUPMENU_SAVE_1_SPN); 
		m_Bt_Close.LoadBitmaps(IDB_SELPRODUCT_EXIT_SPN,IDB_SELPRODUCT_EXIT_0_SPN); 
	}
    m_bmpMainBG.GetBitmap(&bmpInfo);
    m_nMainWidth = bmpInfo.bmWidth; 
    m_nMainHeight = bmpInfo.bmHeight; 	

	m_Btrejectdelay_Up.LoadBitmaps(IDB_CONDPLUSBT,IDB_CONDPLUSBT_1); 
	m_Btrejectdelay_Up.SizeToContent();
	m_Btrejectdelay_Dn.LoadBitmaps(IDB_CONDMINUSBT,IDB_CONDMINUSBT_1); 
	m_Btrejectdelay_Dn.SizeToContent();
	m_Btrejectkeep_Up.LoadBitmaps(IDB_CONDPLUSBT,IDB_CONDPLUSBT_1); 
	m_Btrejectkeep_Up.SizeToContent();
	m_Btrejectkeep_Dn.LoadBitmaps(IDB_CONDMINUSBT,IDB_CONDMINUSBT_1); 
	m_Btrejectkeep_Dn.SizeToContent();
	m_Bthandcycle_Up.LoadBitmaps(IDB_CONDPLUSBT,IDB_CONDPLUSBT_1); 
	m_Bthandcycle_Up.SizeToContent();
	m_Bthandcycle_Dn.LoadBitmaps(IDB_CONDMINUSBT,IDB_CONDMINUSBT_1); 
	m_Bthandcycle_Dn.SizeToContent();
	
	m_Bthandcycle_Up2.LoadBitmaps(IDB_CONDPLUSBT,IDB_CONDPLUSBT_1); 
	m_Bthandcycle_Up2.SizeToContent();
	m_Bthandcycle_Dn2.LoadBitmaps(IDB_CONDMINUSBT,IDB_CONDMINUSBT_1); 
	m_Bthandcycle_Dn2.SizeToContent();

	m_Bt_Save.SizeToContent();
	m_Bt_Close.SizeToContent();
}
BOOL CSetupMenu_2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;

	if(pMsg->hwnd == m_Btrejectdelay_Up.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(1, 80, NULL);
			SendMessage(WM_TIMER,1);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(1);
		}
	}
	else if(pMsg->hwnd == m_Btrejectdelay_Dn.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(2, 80, NULL);
			SendMessage(WM_TIMER,2);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(2);
		}
	}//	
	else if(pMsg->hwnd == m_Btrejectkeep_Up.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(3, 80, NULL);
			SendMessage(WM_TIMER,3);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(3);
		}
	}
	else if(pMsg->hwnd == m_Btrejectkeep_Dn.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(4, 80, NULL);
			SendMessage(WM_TIMER,4);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(4);
		}
	}	
	else if(pMsg->hwnd == m_Bthandcycle_Up.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(5, 80, NULL);
			SendMessage(WM_TIMER,5);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(5);
		}
	}
	else if(pMsg->hwnd == m_Bthandcycle_Dn.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(6, 80, NULL);
			SendMessage(WM_TIMER,6);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(6);
		}
	}
	else if(pMsg->hwnd == m_Bthandcycle_Up2.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(7, 80, NULL);
			SendMessage(WM_TIMER,7);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(7);
		}
	}
	else if(pMsg->hwnd == m_Bthandcycle_Dn2.m_hWnd)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			SetTimer(8, 80, NULL);
			SendMessage(WM_TIMER,8);
		}
		else if(pMsg->message==WM_LBUTTONUP)
		{			
			KillTimer(8);
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPREJECTDELAY)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=100000;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupRejectDelay=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPREJECTDELAY)->SetWindowText(dlg.m_Value);
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPREJECTKEEP)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=100000;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupRejectKeep=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPREJECTKEEP)->SetWindowText(dlg.m_Value);
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPHANDCYCLE)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=100000;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_SetupHandCycle=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPHANDCYCLE)->SetWindowText(dlg.m_Value);
			}
		}
	}

	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_SETUPHANDCYCLE2)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CNumPadBack dlg;
			dlg.m_Max=100000;
			dlg.m_Min=0;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				m_RejectRate=_wtoi(dlg.m_Value);
				GetDlgItem(IDC_SETUPHANDCYCLE2)->SetWindowText(dlg.m_Value);
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSetupMenu_2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
		case 1:
			if(m_SetupRejectDelay>=0)
			{
				m_SetupRejectDelay += 10;
				UpdateData(false);
			}	
			break;
		case 2:
			if(m_SetupRejectDelay>10)
			{
				m_SetupRejectDelay -= 10;
				UpdateData(false);
			}	
			else
			{
				m_SetupRejectDelay = 0;
				UpdateData(false);
			}	
			break;
		case 3:
			if(m_SetupRejectKeep>=0)
			{
				m_SetupRejectKeep += 10;
				UpdateData(false);
			}	
			break;
		case 4:
			if(m_SetupRejectKeep>10)
			{
				m_SetupRejectKeep -= 10;
				UpdateData(false);
			}	
			else
			{
				m_SetupRejectKeep=0;
				UpdateData(false);
			}	

			break;
		case 5:
			if(m_SetupHandCycle>=0)
			{
				m_SetupHandCycle++;
				UpdateData(false);
			}	
			break;
		case 6:
			if(m_SetupHandCycle>0)
			{
				m_SetupHandCycle--;
				UpdateData(false);
			}	
			else
			{
				m_SetupHandCycle=0;
				UpdateData(false);
			}
			break;
		case 7:
			if(m_RejectRate>=0)
			{
				m_RejectRate++;
				UpdateData(false);
			}	
			break;
		case 8:
			if(m_RejectRate>0)
			{
				m_RejectRate--; 
				UpdateData(false);
			}	
			else
			{
				m_RejectRate=0;
				UpdateData(false);
			}	
	}		

	CDialogEx::OnTimer(nIDEvent);
}


void CSetupMenu_2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
    CDC MemDC; 
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&m_bmpMainBG);
    dc.BitBlt(0, 0, m_nMainWidth, m_nMainHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


void CSetupMenu_2::LoadValue(void)
{
	m_SetupHandCycle = g_SetRejectcount;
	m_RejectRate = g_SetRejectrate;
	m_SetupRejectDelay = g_RejectDelay;
	m_SetupRejectKeep = g_RejectKeep;
	m_SetupRejectUse = g_RejectUse;
	if(m_SetupRejectUse==0)
	{
		((CButton*)GetDlgItem(IDC_SETUPREJECTUSE))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_SETUPREJECTUSE1))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_SETUPREJECTUSE2))->SetCheck(false);
	}
	if(m_SetupRejectUse==1)
	{
		((CButton*)GetDlgItem(IDC_SETUPREJECTUSE))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_SETUPREJECTUSE1))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_SETUPREJECTUSE2))->SetCheck(false);
	}
	else
	if(m_SetupRejectUse==2)
	{
		((CButton*)GetDlgItem(IDC_SETUPREJECTUSE))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_SETUPREJECTUSE1))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_SETUPREJECTUSE2))->SetCheck(true);
	}
	SetDlgItemInt(IDC_SETUPHANDCYCLE, m_SetupHandCycle, true);
	SetDlgItemInt(IDC_SETUPREJECTDELAY, m_SetupRejectDelay, true);
	SetDlgItemInt(IDC_SETUPREJECTKEEP, m_SetupRejectKeep, true);
	SetDlgItemInt(IDC_SETUPHANDCYCLE2, m_RejectRate, true);
}


void CSetupMenu_2::OnBnClickedBtSave()
{
	UpdateData(true);

	if(g_RejectDelay == m_SetupRejectDelay && g_RejectKeep == m_SetupRejectKeep && g_RejectUse == m_SetupRejectUse && !b_ApplyAll && g_SetRejectcount==m_SetupHandCycle && g_SetRejectrate == m_RejectRate)
		return;
	else
	{/*
		if(g_HandCycle!=m_SetupHandCycle)
		{
			
			CStdioFile fileString;
			CString str, read;
			str.Format(_T("%d"), g_HandCycle);
			g_HandCycle=m_SetupHandCycle;
			int a=0;
			if(fileString.Open(_T("D:\\Database\\InfoMachine.dat"),
				CStdioFile::modeReadWrite | CStdioFile::modeNoTruncate | CStdioFile::shareDenyNone))
			{
				for(a=0; a<34; a++)
				{
					fileString.ReadString(read);	
				}
				fileString.Seek(fileString.GetPosition()-read.GetLength(), CStdioFile::begin);
				CString ss=_T("");
				for(a=0; a<read.GetLength(); a++)
					ss+=" ";
				fileString.WriteString(ss);
				fileString.Seek(fileString.GetPosition()-read.GetLength(), CStdioFile::begin);
				str.Format(_T("%d"), g_HandCycle);
				fileString.WriteString(str);
				fileString.Close();
			}
			
			if(g_bIOcard && NSE_IO_OUPUT_PORT==2)
			{
				BYTE DeviceID = (BYTE)1;
				WORD StartAddr = (WORD)10;
				WORD WriteData = (WORD)(g_HandCycle);
				if(!ModbusComm.WriteSingleRegister(DeviceID, StartAddr, WriteData))
					return;
			}
		}
		*/
		g_SetRejectcount = m_SetupHandCycle;
		g_SetRejectrate = m_RejectRate;
		g_RejectDelay = m_SetupRejectDelay;
		g_RejectKeep = m_SetupRejectKeep;	
		g_RejectUse = m_SetupRejectUse;

		Init_Mysql();

		if (b_ApplyAll)
		{
			if (!g_Connecting)
				AfxMessageBox(_T("실패"));
			else
			{
				MYSQL_RES* m_Res;
				MYSQL_ROW row;

				mysql_query(&m_mysql, "set names euckr");

				CString strSQL;
				strSQL.Format(_T("SELECT * FROM intellisense_product"));
				CString strQuery;
				if (mysql_query(&m_mysql, CStringA(strSQL)))
				{
					CString error(mysql_error(&m_mysql));
					AfxMessageBox(error);
				}

				m_Res = mysql_store_result(&m_mysql);

				if ((row = mysql_fetch_row(m_Res)) == NULL)
				{
					return;
				}
				else
				{
					int m_count = 0;
					mysql_data_seek(m_Res, m_count);

					while ((row = mysql_fetch_row(m_Res)) != NULL)
					{
						strQuery.Format(_T("Update intellisense_product SET ProductRejectUse = %d, ProductRejectDelay = %d, ProductRejectKeep = %d, ProductEtc6 = %d, ProductEtc7 = %d"),
							g_RejectUse, g_RejectDelay, g_RejectKeep, g_SetRejectcount, g_SetRejectrate);

						if (mysql_query(&m_mysql, CStringA(strQuery)))
						{
							CString error(mysql_error(&m_mysql));
							AfxMessageBox(error);
						}

						m_count++;
						mysql_data_seek(m_Res, m_count);
					}
					OnWriteReportAll();
				}
				mysql_free_result(m_Res);
			}
		}
		else
		{
			if (!g_Connecting)
				AfxMessageBox(_T("실패"));
			else
			{
				CString strQuery;

				strQuery.Format(_T("Update intellisense_product SET ProductRejectUse = %d, ProductRejectDelay = %d, ProductRejectKeep = %d, ProductEtc6 = %d, ProductEtc7 = %d WHERE ProductName = '%s'"),
					g_RejectUse, g_RejectDelay, g_RejectKeep, g_SetRejectcount, g_SetRejectrate, g_ProductName);

				if (mysql_query(&m_mysql, CStringA(strQuery)))
				{
					CString error(mysql_error(&m_mysql));
					AfxMessageBox(error);
				}

				OnWriteReport();
			}
		}

		Close_Mysql();

		/*if(b_ApplyAll)
		{
			if(!m_pSet_p.Open_DB(g_StrDirPath[0]))		
			{
			}
			else
			{
				CString strSQL;
				strSQL.Format(_T("SELECT * FROM Product"));
				CString strQuery;
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
						m_pSet_p.MoveFirst();
						while(!m_pSet_p.IsEOF())
						{		
							strQuery.Format(_T("Update Product SET ProductRejecUse = %d, ProductRejectDelay = %d, ProductRejectKeep = %d, ProductEtc6 = %d, ProductEtc7 = %d"),
								g_RejectUse, g_RejectDelay, g_RejectKeep, g_SetRejectcount, g_SetRejectrate);
							m_pSet_p.Update_DB(strQuery);
							m_pSet_p.MoveNext();
						}
					}
					if(m_pSet_p.b_IsOpenedDB)
						m_pSet_p.Close_DB();
					OnWriteReportAll();
				}
			}
}
		else
		{
			if(!m_pSet_p.Open_DB(g_StrDirPath[0]))		
			{

			}
			else
			{
				CString strQuery;
				strQuery.Format(_T("Update Product SET ProductRejecUse = %d, ProductRejectDelay = %d, ProductRejectKeep = %d, ProductEtc6 = %d, ProductEtc7 = %d WHERE ProductName = '%s'"),
					g_RejectUse, g_RejectDelay, g_RejectKeep, g_SetRejectcount, g_SetRejectrate, g_ProductName);
				m_pSet_p.Update_DB(strQuery);

				if(m_pSet_p.b_IsOpenedDB)
					m_pSet_p.Close_DB();
				OnWriteReport();
			}
		}
		*/
	}
}


void CSetupMenu_2::OnWriteReport(void)
{
	CString strdate, strtime;
	CTime time;
	time = CTime::GetCurrentTime();
	strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());		
	strtime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), time.GetYear(),time.GetMonth(),time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	
	CString strtext;
	if(g_RejectUse==1)
		strtext.Format(_T("%s%s"), g_ProductName, g_MessageArr[165]);
	else if(g_RejectUse==2) 
		strtext.Format(_T("%s(%s,%d,%d)%s"), g_ProductName, g_MessageArr[166], g_RejectDelay, g_RejectKeep, g_MessageArr[160]);
	else
		strtext.Format(_T("%s(%s,%d,%d)%s"), g_ProductName, g_MessageArr[167], g_RejectDelay, g_RejectKeep, g_MessageArr[160]);

	bool g_bOpenDB=true;

	Init_Mysql();

	if (!g_Connecting)
		AfxMessageBox(_T("실패"));
	else
	{
		CString strQuery;
		strQuery.Format(_T("Insert INTO intellisense_db8(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
			strdate, strtext, strtime);
		if (mysql_query(&m_mysql, CStringA(strQuery))) {
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}
	}

	Close_Mysql();

	//if(!m_pSet_8.Open_DB(g_StrDirPath[8]))		
	//{	
	//	g_bOpenDB=false;
	//	//RecoveryDatabase(g_StrDirPath[8], g_StrDirPath_copy[8]);
	//	if(m_pSet_8.Open_DB(g_StrDirPath[8]))		
	//		g_bOpenDB=true;
	//}
	//if(g_bOpenDB)
	//{
	//	CString strQuery;
	//	strQuery.Format(_T("Insert INTO IO_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
	//		strdate, strtext, strtime);
	//	m_pSet_8.Insert_DB(strQuery);
	//	if(m_pSet_8.b_IsOpenedDB)
	//		m_pSet_8.Close_DB();
	//}	
}


void CSetupMenu_2::OnWriteReportAll(void)
{
	CString strdate, strtime;
	CTime time;
	time = CTime::GetCurrentTime();
	strdate.Format(_T("%04d-%02d-%02d"), time.GetYear(),time.GetMonth(),time.GetDay());		
	strtime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), time.GetYear(),time.GetMonth(),time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	
	CString strtext;
	if(g_RejectUse==1)
		strtext.Format(_T("%s"),g_MessageArr[168]);
	else if(g_RejectUse==2) 
		strtext.Format(_T("%s,%d,%d)%s"), g_MessageArr[169], g_RejectDelay, g_RejectKeep, g_MessageArr[160]);
	else 
		strtext.Format(_T("%s,%d,%d)%s"), g_MessageArr[170], g_RejectDelay, g_RejectKeep, g_MessageArr[160]);

	bool g_bOpenDB=true;

	Init_Mysql();

	if (!g_Connecting)
		AfxMessageBox(_T("실패"));
	else
	{
		CString strQuery;
		strQuery.Format(_T("Insert INTO intellisense_db8(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
			strdate, strtext, strtime);
		if (mysql_query(&m_mysql, CStringA(strQuery))) {
			CString error(mysql_error(&m_mysql));
			AfxMessageBox(error);
		}
	}

	Close_Mysql();

	//if(!m_pSet_8.Open_DB(g_StrDirPath[8]))		
	//{	
	//	g_bOpenDB=false;
	//	//RecoveryDatabase(g_StrDirPath[8], g_StrDirPath_copy[8]);
	//	if(m_pSet_8.Open_DB(g_StrDirPath[8]))		
	//		g_bOpenDB=true;
	//}
	//if(g_bOpenDB)
	//{
	//	CString strQuery;
	//	strQuery.Format(_T("Insert INTO IO_Log(strDate, strErrorText, strErrorTime)") _T("VALUES('%s', '%s', '%s')"),
	//		strdate, strtext, strtime);
	//	m_pSet_8.Insert_DB(strQuery);
	//	if(m_pSet_8.b_IsOpenedDB)
	//		m_pSet_8.Close_DB();
	//}	
}


void CSetupMenu_2::OnClickedSetuprejectuse()
{
	UpdateData(true);
}


void CSetupMenu_2::OnSetuprejectuse1()
{
	UpdateData(true);
}


void CSetupMenu_2::OnSetuprejectuse2()
{
	UpdateData(true);
}


void CSetupMenu_2::OnClickedApplyall()
{
	UpdateData(true);
}


HBRUSH CSetupMenu_2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_STATIC) 
	{
        pOldBrush = (CBrush*)pDC->SelectObject(&Brush);
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH) Brush;
    }

	return hbr;
}
