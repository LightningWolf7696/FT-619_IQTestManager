#pragma once

#include "resource.h"		// main symbols
#include "../../Common/MVTDef.h"
#include "IQTestManagerDef.h"
#include "../../Common/UtilityDllDef.h"
#include "TestManager.h"
#include "MyDef.h"

#define CONSOLE_MAX_LENGTH 10000
#define PIPE_MAX_LENGTH  10000
#define TELNET_MAX_LENGTH 10000
#define SOCKET_MAX_LENGTH 10000
#define szSeparateKey  "\n"
#define UART_MESSAGE _T("UART_Message.txt")
#define UART_COMMAND _T("UART_Command.txt")
#define PIPE_MESSAGE _T("PIPE_Message.txt")
#define PIPE_COMMAND _T("PIPE_Command.txt")
#define GPIB_MESSAGE _T("GPIB_Message.txt")
#define GPIB_COMMAND _T("GPIB_Command.txt")
#define TELNET_MESSAGE _T("TELNET_Message.txt")
#define TELNET_COMMAND _T("TELNET_Command.txt")
#define SOCKET_MESSAGE _T("SOCKET_Message.txt")
#define SOCKET_COMMAND _T("SOCKET_Command.txt")
#define DEFINE_VARIABLE _T("DEFINE_VARIABLE.txt")
#define DEFINE_FLAG _T("DEFINE_FLAG.txt")

static enum INI_TYPE
{
	INI_TYPE_STRING = 0,
	INI_TYPE_INTEGER = 1,
	INI_TYPE_LONG = 2,
	INI_TYPE_FLOAT =3,
	INI_TYPE_DOUBLE =4,
	INI_TYPE_BOOL = 5
};

typedef enum _RELAY_OUT{
	OUT_PORT1 =1,
	OUT_PORT2,
	OUT_PORT3,
	OUT_PORT4
}RELAY_OUT;

typedef enum _RELAY_WORKING{
	RELAY_OFF = 0,
	RELAY_ON
}RELAY_WORKING;

#define RealyParameter(Array, Port1, Port2, Port3, Port4) \
{                     \
	Array[0] = Port1; \
	Array[1] = Port2; \
	Array[2] = Port3; \
	Array[3] = Port4; \
}

extern HWND						g_hParentWnd;

class CCommFunc
{
public:
	CCommFunc(void);
public:
	~CCommFunc(void);

public:
	static BOOL ExportTestMessage(funcGetMessage cbMessage, const TCHAR *format, ...);
	static BOOL ExtractString(LPSTR szSrc, LPSTR szDest, int nDestLens, LPSTR szStart, LPSTR szEnd);
	static BOOL CombineMessage(LPSTR lpszSrcData, LPSTR lpszAvailData, int nAvailLens, LPSTR szKey);
	static BOOL DelFolderAllFile(LPSTR lpszDelPath, LPSTR lpszCmd, LPSTR lpszRunInfo);
	static BOOL OutputCommand(LPSTR szFileName, const TCHAR *format, ...);
	static BOOL OutputMessage(LPSTR szFileName, const TCHAR *format, ...);
	static CString TraceStringMid(LPSTR szMessage, LPSTR szStart, LPSTR szEnd);
	static BOOL CloseRelayAll(UINT nComPort, LPSTR lpszRunInfo);
	static BOOL WriteUsbRelayOnePort(UINT nComPort, RELAY_OUT nOutNum, RELAY_WORKING bMode, LPSTR lpszRunInfo);
	static BOOL WriteUsbRelay(UINT nComPort, BOOL *PortCtrl, LPSTR lpszRunInfo);
	static BOOL ReadUsbRelay(UINT nComPort, BOOL *PortCtrl, LPSTR lpszRunInfo);
	static BOOL QueryUsbRelay(UINT nComPort, BOOL *PortCtrl, LPSTR lpszRunInfo);
	static BOOL ExportTestLABEL(const COLORREF *Color,const TCHAR *format, ...);
	static BOOL OutputMsgToBoth(funcGetMessage cbMessage, const TCHAR *format, ...);
	static LABELDATA   g_LabelData;
	static BOOL FillErrorReport( ERRORREPORT *pErrorReport, BOOL bDisableSfcsLink, char *szErrorCode, char *szErrorDetail, char *szErrorMsg, char *szFailedTest, BOOL bTestResult);
	static CString TransXmlSymbo(LPSTR lpszData);
	static CString TransXmlSymbo(CString strData);
	static VAL_COMPARE ValueCompare(double High, double Low, double Actual);
	static BOOL SubDeleteFile(const TCHAR *format, ...);
	static BOOL FindFlag(CString strData);
	static CString GetCurrSystemTime();
	static BOOL ExportTestLable(const COLORREF *Color,const TCHAR *format, ...);
	static void LoadTestDataFile(LPCSTR lpszFileName,LPSTR lpszRunInfo, int length);
	static BOOL RunApplication(LPSTR lpszMethod, LPSTR lpszFilePath, LPSTR lpszRunInfo, LPSTR lpszName);
	static BOOL MyShellExecute(LPSTR lpszFile, LPSTR lpszArguments, int nShowCmd, LPSTR lpszRunInfo);
	static BOOL SeparateListInfoSize(funcGetMessage cbMessage, LPSTR lpszData, int size);
	static int ReverseFindStr(CString strData, LPSTR szKeyWord, int nStopAddr=SEARCH_NOT_FOUND);
	static BOOL SerialPortQueryMultiDataRetry(LPSTR szInput, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo, LPSTR szTestKeyWord="", LPSTR szTestKeySuccess="");
	static BOOL SerialPortQueryMultiData(LPSTR szInput, const CStringArray& strAryKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo, LPSTR szTestKeyWord="", LPSTR szTestKeySuccess="");
	static double WIFI_ChannelToFrequencyMHz(int Channel);
	static BOOL SeparateTestLog(LPSTR lpszMessage, LPSTR lpszFileName="", int maxLength=LOG_MAX_SIZE);
	static double BT_ChannelToFrequencyMHz(int Channel);

};
