// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 클래스입니다.
#include <afxodlgs.h>       // MFC OLE 대화 상자 클래스입니다.
#include <afxdisp.h>        // MFC 자동화 클래스입니다.
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcontrolbars.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS     
#pragma warning(push)
#pragma warning(disable:4146)
#import "C:\Program Files\Common Files\System\ado\msado15.dll" rename ("EOF","adoEOF") no_namespace //20210504_GSM_경로 변경
//#import "c:\program files\common files\system\ado\msado15.dll" rename ("EOF","adoEOF") no_namespace
//20160621 중국청도 XP사용
//#import "c:\program files\common files\system\ado\xp\msado15.dll" rename ("EOF","adoEOF") no_namespace
#pragma warning(pop) 

#include <mysql.h>  //mariadb

#include "EYH_ADO.h"
#include "ChartCtrl\ChartCtrl.h"

//#include "MSChart\mschart.h"
//
//#include "MSChart\vcbackdrop.h"
//#include "MSChart\vcfill.h"
//#include "MSChart\vcbrush.h"
//#include "MSChart\vccolor.h"
//
////y label value
//#include "MSChart\vcseriescollection.h"
//#include "MSChart\vcplot.h"
//#include "MSChart\vcseries.h"
//#include "MSChart\vcdatapoints.h"
//#include "MSChart\vcdatapoint.h"
//#include "MSChart\vcdatapointlabel.h"
//
////xy label scale
//#include "MSChart\vcplot.h"
//#include "MSChart\vcaxis.h"
//#include "MSChart\vcvaluescale.h"
//#include "MSChart\vcaxisscale.h"
//#include "MSChart\vclabels.h"
//#include "MSChart\vclabel.h"
//enum { 
//        VtChAxisIdX = 0, 
//        VtChAxisIdY = 1, 
//        VtChAxisIdY2 = 2, 
//        VtChAxisIdZ = 3, 
//        VtChAxisIdNone = 4 
//    };    //VtChAxisId;



extern CString g_StrDirPath[11];

extern int m_RNationalCode;

extern CString g_StrDate;

extern bool g_Save;

extern MYSQL m_mysql;
extern bool g_Connecting;
extern CString SqlQuery;

extern void WriteErrorLog(int code, CString err);