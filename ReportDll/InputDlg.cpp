// InputDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InputDlg.h"
#include "afxdialogex.h"


// CInputDlg 대화 상자입니다.
//20180323
//static CString m_strCombo1[] = {_T("X3542"), _T("X4269"), _T("X4392"), _T("VJ Tech"), _T("RXG-3071")};
static CString m_strCombo1[] = {_T("X4765(X3542)"), _T("X4269"), _T("X5339(X4392)"), _T("VJ Tech"), _T("RXG-3071"), _T("NXG-1001"), _T("NXG-1002")};

IMPLEMENT_DYNAMIC(CInputDlg, CDialogEx)

CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputDlg::IDD, pParent)
{

	m_Input1_Date = COleDateTime::GetCurrentTime();
	m_Input1_Sn = _T("");
	m_Input1_Time = _T("");
	m_Combo_1_Index = 0;
}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Combo_1);
	DDX_DateTimeCtrl(pDX, IDC_INPUT1_DATE, m_Input1_Date);
	DDX_Text(pDX, IDC_INPUT1_SN, m_Input1_Sn);
	DDX_Text(pDX, IDC_INPUT1_TIME, m_Input1_Time);
	DDX_CBIndex(pDX, IDC_COMBO1, m_Combo_1_Index);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInputDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RESET, &CInputDlg::OnBnClickedReset)
END_MESSAGE_MAP()


// CInputDlg 메시지 처리기입니다.


BOOL CInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//20180323
	for(int i=0 ; i<7 ; i++)
		m_Combo_1.AddString(m_strCombo1[i]);
	m_Combo_1.SetCurSel(m_Combo_1_Index);

	if(m_RNationalCode==1)
	{
		SetDlgItemText(IDC_STATIC_1, _T("X-ray Generator"));
		SetDlgItemText(IDC_STATIC_2, _T("Model     :"));
		SetDlgItemText(IDC_STATIC_3, _T("S/N       :"));
		SetDlgItemText(IDC_STATIC_4, _T("Installed :"));
		SetDlgItemText(IDC_STATIC_5, _T("Used time :"));	
	}
	else if(m_RNationalCode==2)
	{
		SetDlgItemText(IDC_STATIC_1, _T("X-ray 信息"));
		SetDlgItemText(IDC_STATIC_2, _T("型号     :"));
		SetDlgItemText(IDC_STATIC_3, _T("序列号   :"));
		SetDlgItemText(IDC_STATIC_4, _T("安装日期 :"));
		SetDlgItemText(IDC_STATIC_5, _T("使用时间 :"));	
	}
	else
	{
		SetDlgItemText(IDC_STATIC_1, _T("X-ray 발생장치"));
		SetDlgItemText(IDC_STATIC_2, _T("모 델 명 :"));
		SetDlgItemText(IDC_STATIC_3, _T("S  /  N  :"));
		SetDlgItemText(IDC_STATIC_4, _T("설치일자 :"));
		SetDlgItemText(IDC_STATIC_5, _T("사용시간 :"));	
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

#include "../Keyboard/KeyboardDlg.h"

BOOL CInputDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_ESCAPE)
		return FALSE; 
	
	if(pMsg->wParam == VK_RETURN)
		return FALSE;
	
	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_INPUT1_SN)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CKeyboardDlg dlg;
			dlg.m_NationalCode = m_RNationalCode;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				GetDlgItem(IDC_INPUT1_SN)->SetWindowText(dlg.m_TestName);
			}
			else
				GetDlgItem(IDC_INPUT1_SN)->SetWindowText(_T(""));
		}
	}
	if(FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_INPUT1_TIME)
	{			
		if(pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
		{			
			CKeyboardDlg dlg;
			dlg.m_NationalCode = m_RNationalCode;
			int result = dlg.DoModal();
			if(result == IDOK)
			{
				GetDlgItem(IDC_INPUT1_TIME)->SetWindowText(dlg.m_TestName);
			}
			else
				GetDlgItem(IDC_INPUT1_TIME)->SetWindowText(_T(""));
		}
	}
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CInputDlg::OnBnClickedOk()
{
	/*
	CString temp1, temp2;
	GetDlgItem(IDC_INPUT1_SN)->GetWindowText(temp1);
	GetDlgItem(IDC_INPUT1_TIME)->GetWindowText(temp2);
	m_Input1_Sn = temp1;
	m_Input1_Time = temp2;
	*/
	

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CInputDlg::OnBnClickedReset()
{
	CFileFind  finder;
	CString m_strFilter;
	int c=0;

	CString strSQL;
	strSQL.Format(_T("Delete From intellisense_db1"));
	if (mysql_query(&m_mysql, CStringA(strSQL)))
	{
		CString error(mysql_error(&m_mysql));
		AfxMessageBox(_T("27"));
	}
	strSQL.Format(_T("Delete From intellisense_db2"));
	if (mysql_query(&m_mysql, CStringA(strSQL)))
	{
		CString error(mysql_error(&m_mysql));
		AfxMessageBox(_T("28"));
	}
	strSQL.Format(_T("Delete From intellisense_db4"));
	if (mysql_query(&m_mysql, CStringA(strSQL)))
	{
		CString error(mysql_error(&m_mysql));
		AfxMessageBox(_T("29"));
	}
	strSQL.Format(_T("Delete From intellisense_db6"));
	if (mysql_query(&m_mysql, CStringA(strSQL)))
	{
		CString error(mysql_error(&m_mysql));
		AfxMessageBox(_T("30"));
	}
	strSQL.Format(_T("Delete From intellisense_db7"));
	if (mysql_query(&m_mysql, CStringA(strSQL)))
	{
		CString error(mysql_error(&m_mysql));
		AfxMessageBox(_T("31"));
	}
	strSQL.Format(_T("Delete From intellisense_db8"));
	if (mysql_query(&m_mysql, CStringA(strSQL)))
	{
		CString error(mysql_error(&m_mysql));
		AfxMessageBox(_T("32"));
	}
	

	/*
	if(finder.FindFile(g_StrDirPath[1]))
	{
		if(!m_pSet_1.Open_DB(g_StrDirPath[1]))		
		{
				
		}
		else
		{
			CString strSQL;
			strSQL.Format(_T("Delete From Product_Info"));
			m_pSet_1.Delete_DB(strSQL);

			if(m_pSet_1.b_IsOpenedDB)
				m_pSet_1.Close_DB();
		}
	}

	if(finder.FindFile(g_StrDirPath[2]))
	{
		if(!m_pSet_2.Open_DB(g_StrDirPath[2]))		
		{
				
		}
		else
		{
			CString strSQL;
			strSQL.Format(_T("Delete From Running_Info"));
			m_pSet_2.Delete_DB(strSQL);

			if(m_pSet_2.b_IsOpenedDB)
				m_pSet_2.Close_DB();
		}
	}
	//Xray-Log 삭제
	if(finder.FindFile(g_StrDirPath[4]))
	{
		if(!m_pSet_4.Open_DB(g_StrDirPath[4]))		
		{
				
		}
		else
		{
			CString strSQL;
			strSQL.Format(_T("Delete From Monoblock_Log"));
			m_pSet_4.Delete_DB(strSQL);

			if(m_pSet_4.b_IsOpenedDB)
				m_pSet_4.Close_DB();
		}
	}

	//Detector Log 삭제
	if(finder.FindFile(g_StrDirPath[6]))
	{
		if(!m_pSet_6.Open_DB(g_StrDirPath[6]))		
		{
				
		}
		else
		{
			CString strSQL;
			strSQL.Format(_T("Delete From Detector_Log"));
			m_pSet_6.Delete_DB(strSQL);

			if(m_pSet_6.b_IsOpenedDB)
				m_pSet_6.Close_DB();
		}
	}

	//Computer_Log 삭제
	if(finder.FindFile(g_StrDirPath[7]))
	{
		if(!m_pSet_7.Open_DB(g_StrDirPath[7]))		
		{
				
		}
		else
		{
			CString strSQL;
			strSQL.Format(_T("Delete From Computer_Log"));
			m_pSet_7.Delete_DB(strSQL);

			if(m_pSet_7.b_IsOpenedDB)
				m_pSet_7.Close_DB();
		}
	}

	//IO_Log 삭제
	if(finder.FindFile(g_StrDirPath[8]))
	{
		if(!m_pSet_8.Open_DB(g_StrDirPath[8]))		
		{
				
		}
		else
		{
			CString strSQL;
			strSQL.Format(_T("Delete From IO_Log"));
			m_pSet_8.Delete_DB(strSQL);

			if(m_pSet_8.b_IsOpenedDB)
				m_pSet_8.Close_DB();
		}
	}

	*/
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//20180309 DB 초기화 버튼 
}
