////////////////////////////////////////////////////////////////////////////
// This H file define some const which is used to setup the hardware
// 7.18.2002   Liu Junxiang 
// SR-instruments
////////////////////

// Setup the gray scale value threshold
const WORD MIN_GRAY_SCALE_VALUE_THRESHOLD			=500 ;
const WORD MAX_GRAY_SCALE_VALUE_THRESHOLD			=6000;
const WORD DEFAULT_GRAY_SCALE_VALUE_THRESHOLD		=2000;
const WORD MIN_GSV_NO_XRAY                          =250;



// Setup the period time to query the status of the X-Ray generator
const long XRAY_TIMEROUT							= 1000; // 1 second
const long READ_TIMEOUT                             = 200;   // milli
const long WRITE_TIMEOUT                            = 200;   // milli
const int  MAX_ITERATION_TIMES                      = 6;


// Frame grabber trigger mode
const int INTERNAL_TRIGGER							= 0;
const int EXTERNAL_TRIGGER							= 1;

// X-Ray generator default voltage and current
const int   MAX_VOLTAGE             = 130;
const int   MIN_VOLTAGE             = 30;
const int	DEFAULT_VOLTAGE			= 35;
const int   STEP_WARMUP             = 5;

const float MAX_CURRENT             = 7;
const float MIN_CURRENT             = 2;
const int DEFAULT_CURRENT         = 2000;

const float MAX_WATT                = 400;


// Error code
const int NONE_ERROR             = 0;
const int COMM_NO_RESPONSE       = 1;
const int TEMP_HIGH              = 1;
const int ARC_FAULT				 = 2;
const int HIGH_MA	             = 3;
const int LOW_MA                 = 4;
const int LOW_KV                 = 5;
const int HIGH_KV                = 6;
const int WATCHDOG_TIMER_EXPIRED = 7;
const int POWER_LIMIT            = 8;

// BYTE into the parallel port
const BYTE REJECT_PULSE		   	 = 0x08;
const BYTE NO_XRAY_PULSE		 = 0x0E;
const BYTE NO_RESPONSE_PULSE     = 0x02;

const BYTE NORMAL_STATUS         = 0x0A;


// Frame grabber driver path
const char FRAME_GRABBER_DRIVER_NAME[]	
		=	"C:\\Program Files\\Common Vision Blox\\Drivers\\CVPCDIG.vin";




