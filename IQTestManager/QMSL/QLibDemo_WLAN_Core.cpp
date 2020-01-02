/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo_WLAN/QLibDemo_WLAN/QLibDemo_WLAN_Core.cpp#10 $
 * $DateTime: 2019/02/07 22:59:08 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2018 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
 /******************************************************************************
 *
 * Copyright (c) 2013-2018 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
// QLibDemo_WLAN_Core.cpp : Defines the entry point for the console application.
// Contains TLV and DUT connection code

#define _CRT_SECURE_NO_WARNINGS
#include "windows.h"
#include "stdio.h"
#include "stdlib.h"

#include "QMSL_Core.h"
#include "QLibDemo_WLAN_Core.h"

#define _QLIB_WLAN_ATHEROS_H
#ifndef QLIB_API
#define QLIB_API __declspec(dllimport)
#endif

static unsigned long (*OpenUserDefinedTransport)(char* pIpPort );
static void (*CloseUserDefinedTransport)(char* pIpPort );
static unsigned long (*UserDefinedReceive)(HANDLE hQMSL_ContextID, HANDLE hUserContextID, unsigned long* piResponseSize,   unsigned char* piResponseBytes);
static unsigned long (*UserDefinedSend)(HANDLE hQMSL_ContextID, HANDLE hUserContextID, unsigned long iRequestSize, unsigned char* piRequestBytes,   unsigned long* piActualWriteSize);
static unsigned long (*UserDefinedFlushTxRx)(HANDLE hQMSL_ContextID, HANDLE hUserContextID);

// Loopback IP for local system or any remote system IP with DUT
static char UDT_IP[32] = "127.0.0.1";
static unsigned int qpstComNum = 30001;

char (*pQLIB_FTM_WLAN_TLV_Create)(HANDLE hResourceContext,unsigned char opCode);
char (*pQLIB_FTM_WLAN_TLV_Create16)(HANDLE hResourceContext,unsigned short opCode);
char (*pQLIB_FTM_WLAN_TLV_AddParam)(HANDLE hResourceContext, char *pKey, char *pData);
char (*pQLIB_FTM_WLAN_TLV_Complete)(HANDLE hResourceContext);
char (*pQLIB_FTM_WLAN_TLV_GetRspParam)(HANDLE hResourceContext,char *pKey, char *pData);
char (*pQLIB_FTM_WLAN_Atheros_Tx_FW_CAL2)(HANDLE hResourceContext, int phyId, int calScheme, double _1ptRangeDbUL, double _1ptRangeDbLL, int totalNumOfChains, int ultiChainMask, int numAverages, void *pAsyncSetChainHandlerCB, void *pAsyncPMHandlerCB2, void *pAsyncRFSHandlerCB);
char (*pQLIB_FTM_WLAN_Atheros_Save_BDF)(HANDLE hResourceContext, unsigned int optio, char * fname);

// Board data file where depends on chip variation
char BIN_FILE[200];

char *chip_select = NULL;
HINSTANCE hUDT = NULL;
HANDLE g_hResourceContext = NULL;

static char QMSL_Log_File[100] = "C:\\Qualcomm\\Log\\QlibDemo_QMSL_Log";

void Set_UDT_IP(char *newIP)

{
	strcpy(UDT_IP,newIP);
}

void Set_QPST_ComNum(unsigned int comNum)

{
	qpstComNum = comNum;
}

void CaptureQMSLTraceLog(void)
{
    CreateDirectory ("C:\\Qualcomm\\Log", NULL);
    
	// Automatically start the text log
	unsigned short _iLogFlags = LOG_FN | LOG_RET | LOG_ERR | LOG_C_HIGH_LEVEL_START | LOG_C_HIGH_LEVEL_STOP | LOG_ASYNC | LOG_IO;
	QLIB_SetLogFlags(g_hResourceContext, _iLogFlags);
	char LogFile[110];
	sprintf(LogFile, "%s-%d.txt", QMSL_Log_File, qpstComNum);
	QLIB_StartLogging(g_hResourceContext, LogFile);
	printf("Open QMSL log file: %s\n\n", LogFile);
}

static bool ConnectStandaloneWlanCardQPST_MDM()
{
	unsigned int comNum = qpstComNum;
	printf("ConnectStandaloneWlanCardQPST with QLIB_COM_AUTO_DETECT.\n");
	
	sscanf(&UDT_IP[3],"%d",&comNum);
	QLIB_SetLibraryMode(QLIB_LIB_MODE_QPST);        // useQPST
	QLIB_SetTargetType(QLIB_TARGET_TYPE_MSM_MDM);   // MSM_MDM = 0, APQ = 1
	QLIB_SetDiagType(QLIB_DIAG_TYPE_STANDARD);
	if ((g_hResourceContext = QLIB_ConnectServer(QLIB_COM_AUTO_DETECT)) == NULL)  // use AUTO detect comport.
		return false;
	if (QLIB_IsPhoneConnected(g_hResourceContext)  != 1)
		return false;

	CaptureQMSLTraceLog();
	return true;
}

static bool ConnectStandaloneWlanCardQPST()  // Deafult to use
{
	unsigned int comNum = qpstComNum;
	
	sscanf(&UDT_IP[3],"%d",&comNum);
	QLIB_SetLibraryMode(QLIB_LIB_MODE_QPST);    // useQPST
	QLIB_SetTargetType(QLIB_TARGET_TYPE_APQ);   // MSM_MDM = 0, APQ = 1
	if ((g_hResourceContext = QLIB_ConnectServer(comNum)) == NULL)
		return false;

	CaptureQMSLTraceLog();
	return true;
}

static bool ConnectStandaloneWlanCardTcpServer()
{
	unsigned int errCode = 0;
	QLIB_SetLibraryMode(0);                     // No QPST
	QLIB_SetTargetType(QLIB_TARGET_TYPE_APQ);   // APQ = 1
	if ((g_hResourceContext = QLIB_QPHONEMS_ConnectTCPIP(
		                                                99,  // COMPORT Number - not used
														0,   // Direction 0 = SERVER
														strlen(UDT_IP),  // IP Len
														UDT_IP,  // IP
														1,       // Check connection
														1000*15,      // timeout
														&errCode // error Code
														)) == NULL)
	{
		printf("TCP Server creation %s is failed : %d - %8.8X\n",UDT_IP, errCode, GetLastError());
		return false;
	}
	//if (QLIB_IsPhoneConnected(g_hResourceContext)  != 1)
	//	return false;

	CaptureQMSLTraceLog();
	return true;
}

bool ConnectQPSTWithComPortNum(unsigned int comNum, HANDLE* phResourceContext)
{
	qpstComNum = comNum;
	printf("QLIB_ConnectServer with COM%d.\n", comNum);
	QLIB_SetLibraryMode(QLIB_LIB_MODE_QPST);        // useQPST
//	QLIB_SetTargetType(QLIB_LIB_MODE_QPHONEMS);
	QLIB_SetTargetType(QLIB_TARGET_TYPE_MSM_MDM);   // MSM_MDM = 0, APQ = 1
	//if ((g_hResourceContext = QLIB_ConnectServer(QLIB_COM_AUTO_DETECT)) == NULL)  // use AUTO detect comport.
	if ((g_hResourceContext = QLIB_ConnectServer(comNum)) == NULL) {
		printf("QLIB_ConnectServer COM%d failed!\n", comNum);
		return false;
	}
	if (QLIB_IsPhoneConnected(g_hResourceContext)  != 1) {
		printf("QLIB_IsPhoneConnected failed!\n");
		return false;
	}
	*phResourceContext = g_hResourceContext;
	printf("QLIB_IsPhoneConnected ok!\n");

	CaptureQMSLTraceLog();
	return true;
}

bool ConnectStandaloneWlanCard(unsigned int connectionType)
{

   if (connectionType == TYPE_QPST_MDM)
	   return (ConnectStandaloneWlanCardQPST_MDM());

   if ((connectionType == TYPE_QPST) || (connectionType == TYPE_QPST_APQ))
	   return (ConnectStandaloneWlanCardQPST());

   if (connectionType == TYPE_TCPSERVER)
	   return (ConnectStandaloneWlanCardTcpServer());

   //80-WL400-11 Refer Section "Connect - standalone targets"
   hUDT = LoadLibrary("QMSL_WLAN_Transport.dll");  // Must reside same folder as QSPR.exe
   if (hUDT == NULL)
   {
      printf("Can not find QMSL_WLAN_Transport.dll.\n");
      return false;
   }

   OpenUserDefinedTransport = (_OpenUserDefinedTransport)GetProcAddress(hUDT,"OpenUserDefinedTransport");
   CloseUserDefinedTransport =(_CloseUserDefinedTransport)GetProcAddress(hUDT,"CloseUserDefinedTransport");
   UserDefinedReceive = (_UserDefinedReceive)GetProcAddress(hUDT,"UserDefinedReceive");
   UserDefinedSend = (_UserDefinedSend)GetProcAddress(hUDT,"UserDefinedSend");
   UserDefinedFlushTxRx = (_UserDefinedFlushTxRx)GetProcAddress(hUDT,"UserDefinedFlushTxRx");

   if ((OpenUserDefinedTransport == NULL) ||
      (CloseUserDefinedTransport == NULL) ||
      (UserDefinedReceive == NULL) ||
      (UserDefinedSend == NULL))
   {
      printf("Loading QMSL_WLAN_Transport function pointer failed.\n");
      return false;
   }

   if (OpenUserDefinedTransport(UDT_IP) != 0)  // IP can be remote system with DUT or local
   {
      printf("OpenUserDefinedTransport failed. [%s]\n", UDT_IP);
      return false;
   }

   // Setup Callback functions
   g_hResourceContext = QLIB_ConnectServer_UserDefinedTransport((HANDLE) USER_HANDLE,
      UserDefinedSend,
      UserDefinedReceive,
      UserDefinedFlushTxRx,
      true,
      true);
   if (g_hResourceContext == NULL)
   {
      printf("QLIB_ConnectServer_UserDefinedTransport failed.\n");
   }

   CaptureQMSLTraceLog();

   return (g_hResourceContext == NULL ? false : true);
}

/// <summary>
/// Load User Dut Type and setup Callback functions
/// </summary>
bool ConnectDiagTypeLiteWlanCard(unsigned int connectType)
{
   QLIB_SetLibraryMode(QLIB_LIB_MODE_QPHONEMS);
   QLIB_SetTargetType((unsigned char)QLIB_TARGET_TYPE_APQ);
   QLIB_SetDiagType(QLIB_DIAG_TYPE_LITE);
   
   return ConnectStandaloneWlanCard(connectType);
}

static void DisconnectStandaloneWlanCardQPST()
{
  QLIB_DisconnectAllServers();
}

void DisconnectQPSTServer(HANDLE hResourceContext)
{
   //Disable QMSL log.
   QLIB_StopLogging(hResourceContext);

	return(DisconnectStandaloneWlanCardQPST());
}

// ======================================================================================================
/// <summary>
/// Unload User Defined Transport
/// </summary>
void DisconnectStandaloneWlanCard(unsigned int connectionType)
{
   //Disable QMSL log.
   QLIB_StopLogging(g_hResourceContext);
   printf("Close QMSL log file: %s\n\n", QMSL_Log_File);

   //80-WL400-11 Refer Section "Disconnect - standalone targets"
   if (g_hResourceContext)
      QLIB_DisconnectServer(g_hResourceContext);

   if ((connectionType == TYPE_QPST) || (connectionType == TYPE_QPST_MDM) || (connectionType == TYPE_QPST_APQ))
	   return(DisconnectStandaloneWlanCardQPST());

   CloseUserDefinedTransport(UDT_IP);

   if (hUDT)
      FreeLibrary(hUDT);
}


void SetTLV1()
{
   pQLIB_FTM_WLAN_TLV_Create         = &QLIB_FTM_WLAN_TLV_Create;
   pQLIB_FTM_WLAN_TLV_AddParam       = &QLIB_FTM_WLAN_TLV_AddParam;
   pQLIB_FTM_WLAN_TLV_Complete       = &QLIB_FTM_WLAN_TLV_Complete;
   pQLIB_FTM_WLAN_TLV_GetRspParam    = &QLIB_FTM_WLAN_TLV_GetRspParam;
   pQLIB_FTM_WLAN_Atheros_Tx_FW_CAL2 = NULL;
   pQLIB_FTM_WLAN_Atheros_Save_BDF   = NULL;
}

void SetTLV2()
{
   pQLIB_FTM_WLAN_TLV_Create         = &QLIB_FTM_WLAN_TLV2_Create;
   pQLIB_FTM_WLAN_TLV_Create16         = &QLIB_FTM_WLAN_TLV2_Create16;
   pQLIB_FTM_WLAN_TLV_AddParam       = &QLIB_FTM_WLAN_TLV2_AddParam;
   pQLIB_FTM_WLAN_TLV_Complete       = &QLIB_FTM_WLAN_TLV2_Complete;
   pQLIB_FTM_WLAN_TLV_GetRspParam    = &QLIB_FTM_WLAN_TLV2_GetRspParam;
   pQLIB_FTM_WLAN_Atheros_Tx_FW_CAL2 = &QLIB_FTM_WLAN_Atheros_Tx_FW_CAL2;
   pQLIB_FTM_WLAN_Atheros_Save_BDF   = &QLIB_FTM_WLAN_Atheros_Save_BDF;
}
