#define UART_MESSAGE _T("UART_Message.txt")
#define UART_COMMAND _T("UART_Command.txt")
#define PIPE_MESSAGE _T("PIPE_Message.txt")
#define PIPE_COMMAND _T("PIPE_Command.txt")
#define TELNET_MESSAGE _T("TELNET_Message.txt")
#define TELNET_COMMAND _T("TELNET_Command.txt")
#define SOCKET_MESSAGE _T("SOCKET_Message.txt")
#define SOCKET_COMMAND _T("SOCKET_Command.txt")

#define UNKNOW_DEFINE _T("UNKNOW")
#define DUT_FILE_NAME _T("IQTestManager.dll")
#define DATA_VAL_INIT	999999
#define DATA_STR_INIT	"999999"

#define ERR_ENVIRONMENT _T("T089")

#define DATA_MAX_BUFFER_SIZE	10000

#define TX_CAL_FILE		_T("TXCalDataLogFile.txt")

enum OUTPUT_LOG_TYPE
{
	OUTPUT_LOG_ONLY,
	OUTPUT_LIST_ONLY,
	OUTPUT_MSG_BOTH
};
enum VAL_COMPARE
{
	PASS_SPEC = 0,
	OVER_SPEC = 1,
	UNDER_SPEC =2
};

//Test Mode
#define MODE_IQFACT_STR	_T("IQFACT")
#define MODE_WIFI_VERIFY_STR	_T("WIFI_VERIFY")
#define MODE_BT_VERIFY_STR	_T("BT_VERIFY")
#define MODE_CMD_STR	_T("COMMAND")
#define MODE_UPLOAD_STR	_T("UPLOAD")
#define MODE_COMPARE_STR	_T("COMPARE")
#define MODE_SFCS_STR		_T("SFCS")
#define MODE_FILE_STR		_T("FILE")

enum COMMON_TEST_MODE
{
	CMODE_IQFACT,
	CMODE_WIFI_VERIFY,
	CMODE_BT_VERIFY,
	CMODE_CMD,
	CMODE_UPLOAD,
	CMODE_COMPARE,
	CMODE_SFCS,
	CMODE_FILE,
	CMODE_NULL
};
static LPSTR TestModeTable[CMODE_NULL+1] = 
{
	MODE_IQFACT_STR,
	MODE_WIFI_VERIFY_STR,
	MODE_BT_VERIFY_STR,
	MODE_CMD_STR,
	MODE_UPLOAD_STR,
	MODE_COMPARE_STR,
	MODE_SFCS_STR,
	MODE_FILE_STR,
	UNKNOW_DEFINE
};		

//Command mode of type
#define IQF_LOAD_REFERENCE_SCRIPT_STR _T("LOAD_REFERENCE_SCRIPT")
#define IQF_TYPE_GLOBAL_SETTINGS_STR _T("GLOBAL_SETTINGS")
#define IQF_TYPE_CONNECT_IQ_TESTER_STR _T("CONNECT_IQ_TESTER")
#define IQF_TYPE_INSERT_DUT_STR _T("INSERT_DUT")
#define IQF_TYPE_INITIALIZE_DUT_STR _T("INITIALIZE_DUT")
#define IQF_TYPE_LOAD_PATH_LOSS_TABLE_STR _T("LOAD_PATH_LOSS_TABLE")
#define IQF_TYPE_TX_SET_PARAMETERS_STR _T("TX_SET_PARAMETERS")
#define IQF_TYPE_XTAL_CALIBRATION_STR _T("XTAL_CALIBRATION")
#define IQF_TYPE_TX_CALIBRATION_STR _T("TX_CALIBRATION")
#define IQF_TYPE_APT_GET_LOCK_STR _T("APT_GET_LOCK")
#define IQF_TYPE_RX_CALIBRATION_STR _T("RX_CALIBRATION")
#define IQF_TYPE_APT_RELEASE_LOCK_STR _T("APT_RELEASE_LOCK")
#define IQF_TYPE_TX_VERIFY_POWER_STR _T("TX_VERIFY_POWER")
#define IQF_TYPE_TX_VERIFY_EVM_STR _T("TX_VERIFY_EVM")
#define IQF_TYPE_TX_VERIFY_SPECTRUM_STR _T("TX_VERIFY_SPECTRUM")
#define IQF_TYPE_TX_VERIFY_MASK_STR _T("TX_VERIFY_MASK")
#define IQF_TYPE_RX_VERIFY_PER_STR _T("RX_VERIFY_PER")
#define IQF_TYPE_WRITE_MAC_ADDRESS_STR _T("WRITE_MAC_ADDRESS")
#define IQF_TYPE_FINALIZE_EEPROM_STR _T("FINALIZE_EEPROM")
#define IQF_TYPE_REMOVE_DUT_STR _T("REMOVE_DUT")
#define IQF_TYPE_DISCONNECT_IQ_TESTER_STR _T("DISCONNECT_IQ_TESTER")
#define IQF_TYPE_TX_BDR_STR _T("TX_BDR")
#define IQF_TYPE_TX_EDR_STR _T("TX_EDR")
#define IQF_TYPE_TX_LE_STR _T("TX_LE")
#define IQF_TYPE_RX_BDR_STR _T("RX_BDR")
#define IQF_TYPE_RX_EDR_STR _T("RX_EDR")
#define IQF_TYPE_RX_LE_STR _T("RX_LE")

enum IQFACT_TYPE
{
	IQF_LOAD_REFERENCE_SCRIPT = 0,
	IQF_TYPE_GLOBAL_SETTINGS,
	IQF_TYPE_CONNECT_IQ_TESTER,
	IQF_TYPE_INSERT_DUT,
	IQF_TYPE_INITIALIZE_DUT,
	IQF_TYPE_LOAD_PATH_LOSS_TABLE,
	IQF_TYPE_TX_SET_PARAMETERS,
	IQF_TYPE_XTAL_CALIBRATION,
	IQF_TYPE_TX_CALIBRATION,
	IQF_TYPE_APT_GET_LOCK,
	IQF_TYPE_RX_CALIBRATION,
	IQF_TYPE_APT_RELEASE_LOCK,
	IQF_TYPE_TX_VERIFY_POWER,
	IQF_TYPE_TX_VERIFY_EVM,
	IQF_TYPE_TX_VERIFY_SPECTRUM,
	IQF_TYPE_TX_VERIFY_MASK,
	IQF_TYPE_RX_VERIFY_PER,
	IQF_TYPE_WRITE_MAC_ADDRESS,
	IQF_TYPE_FINALIZE_EEPROM,
	IQF_TYPE_REMOVE_DUT,
	IQF_TYPE_DISCONNECT_IQ_TESTER,
	IQF_TYPE_TX_BDR,
	IQF_TYPE_TX_EDR,
	IQF_TYPE_TX_LE,
	IQF_TYPE_RX_BDR,
	IQF_TYPE_RX_EDR,
	IQF_TYPE_RX_LE,
	IQF_TYPE_NULL
};
static LPSTR IQFactTypeTable[IQF_TYPE_NULL+1] = 
{
	IQF_LOAD_REFERENCE_SCRIPT_STR,
	IQF_TYPE_GLOBAL_SETTINGS_STR,
	IQF_TYPE_CONNECT_IQ_TESTER_STR,
	IQF_TYPE_INSERT_DUT_STR,
	IQF_TYPE_INITIALIZE_DUT_STR,
	IQF_TYPE_LOAD_PATH_LOSS_TABLE_STR,
	IQF_TYPE_TX_SET_PARAMETERS_STR,
	IQF_TYPE_XTAL_CALIBRATION_STR,
	IQF_TYPE_TX_CALIBRATION_STR,
	IQF_TYPE_APT_GET_LOCK_STR,
	IQF_TYPE_RX_CALIBRATION_STR,
	IQF_TYPE_APT_RELEASE_LOCK_STR,
	IQF_TYPE_TX_VERIFY_POWER_STR,
	IQF_TYPE_TX_VERIFY_EVM_STR,
	IQF_TYPE_TX_VERIFY_SPECTRUM_STR,
	IQF_TYPE_TX_VERIFY_MASK_STR,
	IQF_TYPE_RX_VERIFY_PER_STR,
	IQF_TYPE_WRITE_MAC_ADDRESS_STR,
	IQF_TYPE_FINALIZE_EEPROM_STR,
	IQF_TYPE_REMOVE_DUT_STR,
	IQF_TYPE_DISCONNECT_IQ_TESTER_STR,
	IQF_TYPE_TX_BDR_STR,
	IQF_TYPE_TX_EDR_STR,
	IQF_TYPE_TX_LE_STR,
	IQF_TYPE_RX_BDR_STR,
	IQF_TYPE_RX_EDR_STR,
	IQF_TYPE_RX_LE_STR,
	UNKNOW_DEFINE
};	

#define IQ_INTERFACE_TYPE_INT_STR _T("Integer")
#define IQ_INTERFACE_TYPE_DOUBLE_STR _T("Double")
#define IQ_INTERFACE_TYPE_STRING_STR _T("String")

enum IQFACT_INTERFACE_TYPE
{
	IQ_INTERFACE_TYPE_INT =0,
	IQ_INTERFACE_TYPE_DOUBLE,
	IQ_INTERFACE_TYPE_STRING,
	IQF_INTERFACE_TYPE_NULL
};

static LPSTR IQFactInterfaceTable[IQF_INTERFACE_TYPE_NULL+1] = 
{
	IQ_INTERFACE_TYPE_INT_STR,
	IQ_INTERFACE_TYPE_DOUBLE_STR,
	IQ_INTERFACE_TYPE_STRING_STR,
	UNKNOW_DEFINE
};

//Compare mode of type
#define COMPARE_TYPE_STRING_STR _T("STRING")
#define COMPARE_TYPE_VALUE_STR _T("VALUE")
#define COMPARE_TYPE_TIME_STR _T("TIME")
#define COMPARE_TYPE_LENGTH_STR _T("LENGTH")
#define COMPARE_TYPE_ASCII_STR _T("ASCII")
enum COMPARE_TYPE
{
	COMPARE_TYPE_STRING = 0,
	COMPARE_TYPE_VALUE,
	COMPARE_TYPE_TIME,
	COMPARE_TYPE_LENGTH,
	COMPARE_TYPE_ASCII,
	COMPARE_TYPE_NULL
};
static LPSTR CompareTypeTable[COMPARE_TYPE_NULL+1] = 
{
	COMPARE_TYPE_STRING_STR,
	COMPARE_TYPE_VALUE_STR,
	COMPARE_TYPE_TIME_STR,
	COMPARE_TYPE_LENGTH_STR,
	COMPARE_TYPE_ASCII_STR,
	UNKNOW_DEFINE
};		

//Compare mode of method
#define COMPARE_ML_SAME_CAPS_STR		_T("SAME_CAPS")
#define COMPARE_ML_IGNORE_CAPS_STR	_T("IGNORE_CAPS")
#define COMPARE_ML_SEARCH_STR	_T("SEARCH")
#define COMPARE_ML_DELTA_STR	_T("DELTA")
enum COMPARE_METHOD
{
	COMPARE_ML_SAME_CAPS = 0,
	COMPARE_ML_IGNORE_CAPS,
	COMPARE_ML_SEARCH,
	COMPARE_ML_DELTA,
	COMPARE_ML_NULL
};
static LPSTR CompareMethodTable[COMPARE_ML_NULL+1] = 
{
	COMPARE_ML_SAME_CAPS_STR,
	COMPARE_ML_IGNORE_CAPS_STR,
	COMPARE_ML_SEARCH_STR,
	COMPARE_ML_DELTA_STR,
	UNKNOW_DEFINE
};	

//file mode of type
#define FILE_TYPE_CREATE_STR _T("CREATE")
#define FILE_TYPE_PARSER_STR _T("PARSER")
#define FILE_TYPE_DELETE_STR _T("DELETE")
#define FILE_TYPE_OUTPUT_STR _T("OUTPUT")
enum FILE_TYPE
{
	FILE_TYPE_CREATE,
	FILE_TYPE_PARSER,
	FILE_TYPE_DELETE,
	FILE_TYPE_OUTPUT,
	FILE_TYPE_NULL
};
static LPSTR FileTypeTable[FILE_TYPE_NULL+1] = 
{
	FILE_TYPE_CREATE_STR,
	FILE_TYPE_PARSER_STR,
	FILE_TYPE_DELETE_STR,
	FILE_TYPE_OUTPUT_STR,
	UNKNOW_DEFINE
};

typedef struct _EXT_DUT_PARAM
{
	int		PACKET_LENS;
	TCHAR	WAVEFORM[MAX_PATH];
}EXT_DUT_PARAM,*PEXT_DUT_PARAM;

typedef struct _GLOBAL_INFO{
	TCHAR	CONSOLE_KEYWORD[DLL_INFO_SIZE];
	int		CONSOLE_TIMEOUT;
	TCHAR	CMD_EOF[32];
	int		CONSOLE_SLEEP;
	TCHAR	szLocalPath[MAX_PATH];
	TM_ID	technologyID;
	TM_ID	ID_WIFI_11AC;
	TM_ID	ID_BT;
	bool	TraceMessage;
	TCHAR   PN98[MAX_PATH];
	TCHAR   PN_FOLDER_PATH[MAX_PATH];
	funcAssignVariable fAssignVariable;
	inline _GLOBAL_INFO()
	{
		ID_WIFI_11AC = 0;
		ID_BT = 0;
		strCopy(CONSOLE_KEYWORD, "#");
		CONSOLE_TIMEOUT = 3;
		strCopy(CMD_EOF,"\r\n");
		CONSOLE_SLEEP = 0;
		TraceMessage = false;
		zeroIt(PN_FOLDER_PATH);
	}
}GLOBAL_INFO, *pGLOBAL_INFO;

extern GLOBAL_INFO	g_GlobalInfo;