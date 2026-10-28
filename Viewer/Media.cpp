// Media.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Media.h"
#include "afxdialogex.h"
#include "ViewerDlg.h"

// CMedia 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMedia, CDialogEx)

CMedia::CMedia(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMedia::IDD, pParent)
	, m_Path(_T(""))
{

}

CMedia::~CMedia()
{
}

void CMedia::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OCX1, m_MediaP);
	DDX_Control(pDX, IDOK, m_Exit);
}


BEGIN_MESSAGE_MAP(CMedia, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMedia 메시지 처리기입니다.


BOOL CMedia::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	CFont m_font;
	m_font.CreatePointFont(200,TEXT("Minion Pro"));
	GetDlgItem(IDC_STATIC)->SetFont(&m_font);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Exit.LoadBitmaps(IDB_BTEXITPLAY, IDB_BTEXITPLAY_0); 
	Play_avi();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMedia::Play_avi(void)
{
	CFileFind finder;
	BOOL find = finder.FindFile(g_path);

	if(!find)
	{
		AfxMessageBox(_T("동영상 파일이 없습니다."));
		EndDialog(IDD_MEDIA);
		//	m_player.close();
		//return;
	}
	else
	{	
		m_MediaP.put_URL(g_path);
		SetTimer(1,500,NULL);
		
	}
}


void CMedia::OnTimer(UINT_PTR nIDEvent)
{
	
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent == 1)
	{
		if(m_MediaP.get_playState() == 1)
		{
			EndDialog(IDD_MEDIA);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
