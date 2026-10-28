#pragma once


// CMedia 대화 상자입니다.
#include "../Viewer/resource.h"
#include "cwmpplayer4.h"
#include "afxext.h"

class CMedia : public CDialogEx
{
	DECLARE_DYNAMIC(CMedia)

public:
	CMedia(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMedia();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MEDIA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CWMPPlayer4 m_MediaP;
	CString m_Path;
	virtual BOOL OnInitDialog();
	void Play_avi(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CBitmapButton m_Exit;
};
