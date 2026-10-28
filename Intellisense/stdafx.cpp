
// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// Intellisense.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 

//Initial Variable
int NSE_VAR_SIZE;  
int NSE_DETECTOR_WIDTH;
int NSE_TRIGER_HEIGHT;  
int NSE_TRIGER_FRAME_SIZE;
int NSE_WIDTH_SIZE;       
int NSE_XRAY_KV;      
int NSE_XRAY_MA;      
int NSE_XRAY_POWER;  
int NSE_XRAY_KIND;   
int NSE_DT_KIND;      
int NSE_DT_ISBULK;   
bool NSE_ROTATION_UPDOWN;
bool NSE_ROTATION_LEFTRIGHT;
int NSE_BASIC_KV;  
int NSE_BASIC_MA;  
int NSE_BASIC_KV2;  
int NSE_BASIC_MA2;  
int NSE_BASIC_SPEED;  
int NSE_BASIC_DELAY;  
int NSE_BASIC_KEEP;  
int NSE_BASIC_CONVSPEED;   	
int NSE_BASIC_CONVFACTOR;
int NSE_CONVERYOR_MASKING;      
int NSE_CONVERYOR_ON;        
int NSE_CONVERYOR_OFF;  
int NSE_YELLO_ON;  
int NSE_GREEN_ON;
int NSE_REJECT_MASKING;    
int NSE_REJECT_ON;     
int NSE_REJECT_OFF;         
int NSE_IO_OUPUT_PORT;   
int NSE_INTEROCK_EX_LEFT;   
int NSE_INTEROCK_EX_RIGHT;  
int NSE_INTEROCK_MONITOR;  
int NSE_INTEROCK_BACK;         
int g_KindModel;
int	g_ConveyorSpeed;
int g_ConveyorFac;
int g_HandsensorLU; 
int g_HandsensorLD; 
int g_HandsensorRU;
int g_HandsensorRD;
int g_HandCycle; 
CString g_Model_Kind;

bool m_Interock_State;

BOOL g_bGenerator;
BOOL g_bDetector;
BOOL g_bIOcard;

//IO board Variable
CModbusSerial ModbusComm;
BOOL g_Bit_In[16];
BOOL g_Bit_Out[16];
BOOL g_bReject;
WORD g_Word_In;

LRESULT m_ErrCde;
LONG	m_DriverHandle;		
PT_DioReadPortByte	m_ptDioReadPortByte; 
PT_DioWritePortByte	m_ptDioWritePortByte;	
PT_DioWriteBit      m_ptDioWriteBit;    
PT_DioReadBit	m_ptDioReadBit;
PT_DioGetCurrentDOByte m_ptDioGetCurrentDOByte;
USHORT gwDoState;

unsigned char ***g_Img_Buf;
unsigned char **g_Img_Buf2;
unsigned char ***m_Buffer_1;
unsigned char *m_Buffer_2;
unsigned char *m_Buffer_3;
unsigned char *m_Buffer_4;
bool g_DispArea;

unsigned char g_BackupCond1[10000];
unsigned char g_BackupCond2[10000];
unsigned char g_BackupCond3[10000];
unsigned char g_BackupCond4[10000];
unsigned char g_BackupCond5[10000];
unsigned char g_BackupCond6[10000];
int g_BackupFront;
int g_BackupRear;

//int b_EventReconi; //ch 


CString g_Filename;
int iv_1;
int iv_2; 
int g_Xray_kv;
int g_Xray_ma;
int g_Xray_kv2; 
int g_Xray_ma2;
int	g_DTScanspeed;
float g_DTGain;
int g_RejectUse;
int	g_RejectDelay;
int g_RejectKeep;
int g_SaveHard;
int g_SaveFile;
int	g_Code;
int	g_Size;
int	g_Condition1;
int	g_Condition2;
int	g_Condition3;
int	g_Condition4;
int	g_Condition5;
int	g_Condition6;
int	g_OmitCondition1;
int	g_OmitCondition2;
int	g_OmitCondition3;
int	g_OmitCondition4;
int	g_OmitCondition5;
int  g_MassCondition1;
int  g_MassCondition2;
int  g_MassCondition3;

//AutoSetup
bool g_bAutoSetup;
int g_AutoCount;
int g_AutoCond1;
int g_AutoCond2;
int g_AutoCond3;
int g_AutoCond4;
int g_AutoCond5;
int g_AutoCond6;
int g_AutoCond1_1;
int g_AutoCond2_1;
int g_AutoCond3_1;
int g_AutoCond4_1;
int g_AutoCond5_1;
int g_AutoCond6_1;
bool g_bXrayAutoSetup;
int g_AutoXrayValue;
int g_AutoXrayCount;

int g_OmitCondUse;
int g_MassCondUse;
int g_CountCondUse;
int g_MaskBoxUse;
int g_MaskClipUse;
int g_MaskTalUse;


int g_MaskBoxCondition1;
int g_MaskBoxCondition2;
int g_MaskBoxCondition3;
int g_MaskBoxCondition4;
int g_MaskBoxCondition5;
int g_MaskBoxCondition6;

int g_MaskClip_Direction;
int g_MaskClip_Length;
int g_MaskClip_Deep;
int g_Countnum;
int g_Countnoise;
int g_PageMaskLR;
int g_PageMaskUD;
int g_TalMask_Bright;
int g_TalMask_Bright2;

bool g_bOptionD;
bool g_bOptionE;
int g_bOptionD_1;
int g_bOptionD_2;
int g_bOptionE_1;
int g_bOptionE_2;

bool g_bUseNormal;
int g_PreproUse;
int g_PreproDisp;
int g_PreproValue;

bool g_ChangeNGCond;
bool g_ChangeOmitCond[3];
bool g_ChangeMaskCond[3];

CEvent g_EventAlarm;
CEvent g_EventPaint;
CEvent g_EventReject;
CEvent g_EventDrawImage;
CEvent g_EventSave;

//Reject
int g_Starttime;
DWORD g_Endtime;
int g_Rejecttime[10000];	
int g_RejectFront;
int g_RejectRear;

//Login
bool g_bAdmin;
CString g_Password;

CString g_ProductName;

int g_Margin;
int g_Height;
int g_Width;
int g_XrayState;

bool g_OffInspection;


//20160428
int g_DispOpenBox1;
int g_DispOpenBox2;

int g_iXrayOn;
int g_AirconCycle;
//SmartView
bool b_UseSmartView;
CStdioFile m_SmartFile;
static CString m_FilePath[3]={_T("D:\\SmartView\\MachineStatus.dat"), _T("D:\\SmartView\\ProductInfo.dat"),
							_T("D:\\SmartView\\ProductionInfo.dat")};

BITMAPINFO *BmInfo;

CCriticalSection g_cs;

void WriteProductionInfo(int value, int total, int ok, int ng, int omit)
{
	CString temp;
	if(m_SmartFile.Open(m_FilePath[2], CFile::modeWrite|CFile::modeCreate|CFile::shareDenyNone))
	{
		temp.Format(_T("%d\n"), value);
		m_SmartFile.WriteString(temp);
		temp.Format(_T("%d\n"), total);
		m_SmartFile.WriteString(temp);
		temp.Format(_T("%d\n"), ok);
		m_SmartFile.WriteString(temp);
		temp.Format(_T("%d\n"), ng);
		m_SmartFile.WriteString(temp);
		temp.Format(_T("%d\n"), omit);
		m_SmartFile.WriteString(temp);
		m_SmartFile.Close();
	}
}

void WriteProductInfo(CString value)
{
	if(m_SmartFile.Open(m_FilePath[1], CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
	{
		m_SmartFile.WriteString(value);
		m_SmartFile.Close();
	}
}

void WriteMachineStatus(CString value)
{
	if(m_SmartFile.Open(m_FilePath[0], CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareDenyNone))
	{
		m_SmartFile.WriteString(value);
		m_SmartFile.Close();
	}
}

int g_NationalCode;
CString g_MessageArr[250];

CString g_StrDirPath[11]={
	_T("D:\\Database\\Product_new_global.mdb"),
	_T("D:\\Database\\Intellisense_DB1.mdb"),
	_T("D:\\Database\\Intellisense_DB2.mdb"),
	_T("D:\\Database\\Intellisense_DB3.mdb"),
	_T("D:\\Database\\Intellisense_DB4.mdb"),
	_T("D:\\Database\\Intellisense_DB5.mdb"),
	_T("D:\\Database\\Intellisense_DB6.mdb"),
	_T("D:\\Database\\Intellisense_DB7.mdb"),
	_T("D:\\Database\\Intellisense_DB8.mdb"),
	_T("D:\\Database\\Intellisense_DB9.mdb"),
	_T("D:\\Database\\Intellisense_DB10.mdb")};

CString g_StrDirPath_copy[11]={
	_T("D:\\Database\\backup\\Product_new_total.mdb"),
	_T("D:\\Database\\backup\\Intellisense_DB1.mdb"),
	_T("D:\\Database\\backup\\Intellisense_DB2.mdb"),
	_T("D:\\Database\\backup\\Intellisense_DB3.mdb"),
	_T("D:\\Database\\backup\\Intellisense_DB4.mdb"),
	_T("D:\\Database\\backup\\Intellisense_DB5.mdb"),
	_T("D:\\Database\\backup\\Intellisense_DB6.mdb"),
	_T("D:\\Database\\backup\\Intellisense_DB7.mdb"),
	_T("D:\\Database\\backup\\Intellisense_DB8.mdb"),
	_T("D:\\Database\\backup\\Intellisense_DB9.mdb"),
	_T("D:\\Database\\backup\\Intellisense_DB10.mdb")};

void RecoveryDatabase(CString path, CString path_copy)
{
	CFileFind finder;
	if(finder.FindFile(path))	
	{
		CString ldb=path;
		ldb.Replace(_T("mdb"), _T("ldb"));

		if(finder.FindFile(ldb))
			::DeleteFile(ldb);

		CString copyfile=path;
		CTime time = CTime::GetCurrentTime();
		copyfile.Replace(_T(".mdb"), _T(""));
		copyfile.Format(_T("%s_%04d%02d%02d_%02d%02d%02d.bak"), copyfile, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		::MoveFile(path, copyfile);
		::CopyFile(path_copy, path, false);
	}
	else
	{
		::CopyFile(path_copy, path, false);
	}
}


void OnAnalogOutPut_1(int speed) //컨베어 속도 출력
{
	if(g_ConveyorFac==7876)
	{
		if(speed>53)
			speed=53;
	}
	else if(g_ConveyorFac==6850) //XIS-100
	{
		if(speed>55)
			speed=55; // 모터 과부하를 막기 위하여 최대속도 제한 (10V 이상 출력 안되도록 보호) lim 20150902
	}
	else if(g_ConveyorFac==3150)
	{
		if(speed>65)
			speed=65;
	}
	else if(g_ConveyorFac==9750)
	{
		if(speed>40)
			speed=40;
	}
	else if (g_ConveyorFac == 2855)
	{
		if (speed > 90)
			speed = 90;
	}
	else
	{
		if(speed>94)
			speed=94;
	}

	BYTE DeviceID = (BYTE)1;
	WORD StartAddr = (WORD)0;
	WORD WriteData = (WORD)(speed);//임시 0
	WriteData = (WriteData*g_ConveyorFac)/100;
	/*
	1. 변수 관련 (setup 프로그램에서 모델별 계산값 자동 부여 및 수동 변경 필요...)
	  2178 : XIS-500, 2000
	  7876 : XIS-3000
	  9750 : XIS-4000
	  6850 : XIS-100
	  3150 : XIS-3500

    2. 컨베어 최대 속도 (프로그램에서 Limit 지정 필요)
	  1)XIS-500, 2000 : 94m/min
	  2)XIS-3000 : 26m/min (최대 53m/min 까지 가능 하지만 설정 오류로 인한 모터 과부하 방지로 우선 26m/min)
      3)XIS-100 : 55m/min 
	  4)XIS-3500 : 65m/min
	  5)XIS-4000 : 40m/min	
	*/	
	if(g_ConveyorFac==7876)
	{
		if(WriteData>4170)
			WriteData=4170;
	}
	else if(g_ConveyorFac==6850) //XIS-100
	{
		if(WriteData>3820)
			WriteData=3820;
	}
	else if(g_ConveyorFac==9750)
	{
		if(WriteData>3910)
			WriteData=3910;
	}
	else if (g_ConveyorFac == 2430) // XIS-5000
	{
		if (WriteData > 2187)
			WriteData = 2187;
	}
	else
	{
		if(WriteData>2048)
			WriteData=2048;
	}
	/****************************** Write Holding Register *****************************/
	if(!ModbusComm.WriteSingleRegister(DeviceID,StartAddr, WriteData) ) 
	{
		ModbusComm.WriteSingleRegister(DeviceID,StartAddr, WriteData);
	}
}

void OnAnalogOutPut_3(int kv1) //x-ray kv 셋팅
{
	// TODO: Add your control notification handler code here
	BYTE DeviceID = (BYTE)1;
	WORD StartAddr = (WORD)2;
	WORD WriteData = (WORD)( kv1 ); //IDC_EDIT3 => 전압설정값으로 대체
/*
	if(WriteData>80 || WriteData<20){
		AfxMessageBox("20~80 사이값을 입력하세요!");
		return;
	}
*/
	WORD kv = (2048*WriteData)/100;
	/****************************** Write Holding Register *****************************/
	if( !ModbusComm.WriteSingleRegister(DeviceID,StartAddr, kv) ) {
		ModbusComm.WriteSingleRegister(DeviceID,StartAddr, kv);
	}	
}

void OnAnalogOutPut_4(int ma1) //x-ray ma 셋팅
{
	// TODO: Add your control notification handler code here
	BYTE DeviceID = (BYTE)1;
	WORD StartAddr = (WORD)3;
	WORD WriteData = (WORD)( ma1 ); //IDC_EDIT4 => 전류설정값으로 대체
/*
	if(WriteData>7000 || WriteData<1000){
		AfxMessageBox("1000~7000 사이값을 입력하세요!");
		return;
	}
*/
	WORD ma = (2048*WriteData)/10000;
	/****************************** Write Holding Register *****************************/
	if( !ModbusComm.WriteSingleRegister(DeviceID,StartAddr, ma) ) {
		ModbusComm.WriteSingleRegister(DeviceID,StartAddr, ma);
	}	
}

int OnXray_kv_Output() //x-ray 전압 출력값
{
	// TODO: Add your control notification handler code here
	WORD ReadCount = 1;
	WORD StartAddr = (WORD)6;
	BYTE DeviceID = (BYTE)1;
	WORD pReadData[1];
	
	/****************************** Read Holding Register *****************************/
	if(!ModbusComm.ReadInputRegister( DeviceID, StartAddr, ReadCount, pReadData)) 
	{
		ModbusComm.ReadInputRegister( DeviceID, StartAddr, ReadCount, pReadData);
	}
	int kv=0;
	kv = (pReadData[0]*100)/2048;
	return kv;	
}

int OnXray_ma_Output()
{
	WORD ReadCount = 1;
	WORD StartAddr = (WORD)7;
	BYTE DeviceID = (BYTE)1;
	WORD pReadData[1];
	if(!ModbusComm.ReadInputRegister( DeviceID, StartAddr, ReadCount, pReadData)) 
	{
		ModbusComm.ReadInputRegister( DeviceID, StartAddr, ReadCount, pReadData);
	}
	int ma=0;
	ma = (pReadData[0]*10000)/2048;
	return ma;	
}

void OnDigitalOutPut_All()
{
	BYTE DeviceID = (BYTE)1;
	WORD StartAddr = (WORD)8;
	
	WORD WriteData = ModbusComm.BitToWord( g_Bit_Out);
	if(!ModbusComm.WriteSingleRegister(DeviceID,StartAddr, WriteData))
	{
		ModbusComm.WriteSingleRegister(DeviceID,StartAddr, WriteData);
	}
}

bool OnDigitalOutPut_All_Init() 
{
	BYTE DeviceID = (BYTE)1;
	WORD StartAddr = (WORD)8;
	
	WORD WriteData = ModbusComm.BitToWord(g_Bit_Out);
	if(!ModbusComm.WriteSingleRegister(DeviceID,StartAddr, WriteData)) 
	{
		return false;
	}
	return true;
}

void OnDigitalInPut_All() 
{
	WORD ReadCount = 1;
	WORD StartAddr = (WORD)9;
	BYTE DeviceID = (BYTE)1;

	WORD pReadData[1];
	g_cs.Lock();//kds20140819	
	/****************************** Read Holding Register *****************************/
	if(ModbusComm.ReadInputRegister( DeviceID, StartAddr, ReadCount, pReadData ) ) {
		ModbusComm.WordToBit(pReadData[0], g_Bit_In );
	}
	g_cs.Unlock();//kds20140819
	g_Word_In = pReadData[0];
	//if( ModbusComm.ReadInputRegister( DeviceID, StartAddr, ReadCount, pReadData ) ) {
	//	ModbusComm.WordToBit( pReadData[0], g_Bit_In );
	//}	
}

bool OnDigitalOutPut_All22() //디지털 출력 
{
	// TODO: Add your control notification handler code here

	BYTE DeviceID = (BYTE)1;
	WORD StartAddr = (WORD)8;
	
	WORD WriteData = ModbusComm.BitToWord( g_Bit_Out );

	g_cs.Lock();//kds20140819
	if(!ModbusComm.WriteSingleRegister(DeviceID,StartAddr, WriteData) ) {
		//AfxMessageBox("OutPut Error!");
		g_cs.Unlock();
		return false;
	}
	g_cs.Unlock();//kds20140819
	return true;
	
}

void Sleepms(DWORD msec)
{
	MSG msg;
	DWORD dwCurrent=GetTickCount();

	while(GetTickCount()-dwCurrent<msec)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message==WM_LBUTTONDOWN || msg.message==WM_LBUTTONUP || msg.message==WM_LBUTTONDBLCLK)
				continue;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void Init_Mysql()
{
	mysql_init(&m_mysql);

	if(mysql_real_connect(&m_mysql, "localhost", "root", "123456", CStringA("intellisense_db"), 3306, NULL, 0) == NULL)
	{ // 디비 접속
		g_Connecting = false;
		CString error(mysql_error(&m_mysql));
		AfxMessageBox(error);
	}
	else
	{
		g_Connecting = true;
	}

	mysql_query(&m_mysql, "set names euckr");
}

void Close_Mysql()
{
	if (g_Connecting == true)
	{
		g_Connecting = false;
		mysql_close(&m_mysql);

	}
}


unsigned char ***g_Img_Buf3D;
unsigned char ***g_Img_Bufx2;
unsigned char ***g_Img_Bufx2_o;
unsigned char **g_Img_temp;
int g_3D_Count;
int g_Thread_Count;

int g_Dsize;
int g_SizeDisp;

int iv_11;
int iv_22;

int g_Margin2;
int g_iGrap_Count;
int g_Margin22[50];

int g_iGrab_Count;

bool g_bSavingCondition;

int g_Dent_L;
int g_Dent_R;
bool g_Dent_Save;


int *g_Arry_Save;
int *g_Savecount;
int *g_Savekind;

bool g_bComiDaq1;

int g_iRejectCounter;

int g_ConReject;
int g_SetRejectcount;
int g_SetRejectrate;
int g_reject_new_time;

int g_Model_Select;

int g_KindofOmit;

int g_OmitAvr;

bool g_Count_Flag;

int g_RejectType;

int g_AlamUse;

int g_xray_il;

int g_ShowMaskUse;

int g_Continu_Ng;

int m_Icount;

CString g_SaveExternalName;
CString g_SaveExternalName1;
CString g_SaveExternalName2;
 ////////자동조사량 개선 20170930
int g_Masking1;
int g_Masking2;
int g_Masking3;
int g_Masking4;
int g_Masking5;

int g_Special1;
int g_Special2;
int g_Special3;
int g_Special4;
 ////////자동조사량 개선 20170930

int g_ExHardUse; //외장하드 선택 적용 개발 20171226

int g_ViewControl; //영상 강제 보정 20171115

bool g_Accept; //영상 보정 검사 적용 20171130

CString g_STime[50];

int g_left;
int g_right;
int g_bottom;

int g_KindBottle;
 //20200525
InstantDiCtrl* m_instantDi;
InstantDoCtrl* m_instantDo;
int g_portCount;
byte * g_portReadData;
byte g_portWriteData;
//20200525

//20191202
CString g_Today;
CString g_Adress;
CString g_Line;
//20191202
int g_Sum_Total_Count;
int g_Sum_Ok_Count;
int g_Sum_Ng_Count;
int g_Sum_Omit_Count;
//20191202
int g_SendKind;
CString g_PGStartTime;
CString g_PGEndTime;
int g_StopKind;
int g_SignalTime;

MYSQL m_mysql;
bool g_Connecting;
CString SqlQuery;

int g_SelDetector;

int g_leftm;
int g_rightm;

unsigned char* g_Img_Buf_AI; //20260102 AI 추가