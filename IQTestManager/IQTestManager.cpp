// IQTestManager.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "IQTestManager.h"
#include <iostream>
#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CIQTestManagerApp

BEGIN_MESSAGE_MAP(CIQTestManagerApp, CWinApp)
END_MESSAGE_MAP()

P_TM_RegisterTechnologyDll	TM_RegisterTechnologyDll;
P_TM_Run					TM_Run;
P_TM_ClearParameters		TM_ClearParameters;
P_TM_AddDoubleParameter		TM_AddDoubleParameter;
P_TM_AddIntegerParameter	TM_AddIntegerParameter;
P_TM_AddStringParameter		TM_AddStringParameter;
P_TM_GetDoubleReturnPair	TM_GetDoubleReturnPair;
P_TM_GetIntegerReturnPair	TM_GetIntegerReturnPair;
P_TM_GetStringReturnPair	TM_GetStringReturnPair;
P_TM_GetStringReturn		TM_GetStringReturn;
P_TM_GetIntegerReturn		TM_GetIntegerReturn;
P_TM_GetDoubleReturn		TM_GetDoubleReturn;

GLOBAL_INFO		g_GlobalInfo;
HWND			g_hParentWnd;

// CIQTestManagerApp construction

CIQTestManagerApp::CIQTestManagerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CIQTestManagerApp object

CIQTestManagerApp theApp;

// CIQTestManagerApp initialization

BOOL CIQTestManagerApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}


BOOL CIQTestManagerApp::ReadConfigStationIni()
{
	void *hndl;
	char szMessage[DLL_INFO_SIZE];
	TCHAR szFileName[MAX_PATH]={0};

	sprintf_s(szFileName, sizeof(szFileName), "%s%s", g_GlobalInfo.szLocalPath, CONFIG_STATION_FILE);

	if(!CreateIni(&hndl,szFileName,szMessage)) return FALSE;
	GetIniKeyNameResult(hndl, "GLOBAL_FILE_PATH", "PN_FOLDER_PATH", INI_TYPE_STRING, &g_GlobalInfo.PN_FOLDER_PATH[0], szMessage);
	GetIniKeyNameResult(hndl, "TEST_CONTROLLER", "TRACE_MESSAGE", INI_TYPE_BOOL, &g_GlobalInfo.TraceMessage, szMessage);
	if(!DestroyIni(hndl,szMessage)) return FALSE;

	return TRUE;
}

extern "C" BOOL PASCAL EXPORT LOAD_GUI_INFORMATION(LPSTR lpszOP, LPSTR lpszBarcode, LPSTR lpszPN, SFCS_MODE eSfcsMode, BOOL bStopOnFailed, HWND hParentWnd, LPSTR lpszBarcode2, funcGetActualReport fGetActualReport, LPSTR lpszBarcode3, LPSTR lpszBarcode4, funcAssignVariable fAssignVariable, CString *strScanSrc, ...)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	GetLocalDirectory(g_GlobalInfo.szLocalPath, szMessage);
	g_GlobalInfo.fAssignVariable = fAssignVariable;
	strCopy(g_GlobalInfo.PN98, lpszPN);
   g_GlobalInfo.m_pWnd = CWnd::FromHandle(hParentWnd);
	return theApp.LoadTestManagerDll(szMessage);
}
extern "C" BOOL PASCAL EXPORT RestartDutTest()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TRUE;
}
extern "C" BOOL PASCAL EXPORT CleanToDutParameter(LPSTR ActionName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	zeroIt(theApp.m_ExtDutParam);
	if(strcmp(ActionName,_T("CUSTOMIZE_TEST")) == NULL)
		theApp.m_CustomizeTest.Clean();
	else if(strcmp(ActionName,_T("WIFI_CABLELOSS_PARSER")) == NULL)
		theApp.m_ParserCableLoss.Clean();
	else if((strcmp(ActionName,_T("WIFI_EVM_TEST_OEM")) == NULL) || (strcmp(ActionName,_T("WIFI_SPECTRUM_MASK_TEST_OEM")) == NULL) || (strcmp(ActionName,_T("WIFI_TX_POWER_VERIFICATION_OEM")) == NULL)){
		theApp.m_GetTargetPower.Clean(ActionName);
		theApp.m_CustomizeTest.Clean();
	}else if(strcmp(ActionName,_T("WIFI_RX_PER_TEST_OEM")) == NULL){
		theApp.m_CustomizeTest.Clean();
	}else if(strcmp(ActionName,_T("BT_TX_TEST_OEM")) == NULL){
		theApp.m_CustomizeTest.Clean();
	}else if(strcmp(ActionName,_T("BT_RX_BER_TEST_OEM")) == NULL){
		theApp.m_CustomizeTest.Clean();
	}
	return TRUE;
}
extern "C" BOOL PASCAL EXPORT SetToDutParameter(LPSTR ActionName, LPSTR ParameterName,LPSTR ParameterValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRtn=TRUE;
	if(strcmp(ActionName,_T("CUSTOMIZE_TEST")) == NULL)
		bRtn = theApp.m_CustomizeTest.SetParameter(ParameterName, ParameterValue);
	else if(strcmp(ActionName,_T("WIFI_CABLELOSS_PARSER")) == NULL)
		bRtn = theApp.m_ParserCableLoss.SetParameter(ParameterName, ParameterValue);
	else if((strcmp(ActionName,_T("WIFI_EVM_TEST_OEM")) == NULL) || (strcmp(ActionName,_T("WIFI_SPECTRUM_MASK_TEST_OEM")) == NULL) || (strcmp(ActionName,_T("WIFI_TX_POWER_VERIFICATION_OEM")) == NULL)){
		if(!theApp.m_GetTargetPower.SetParameter(ActionName, ParameterName, ParameterValue)) return FALSE;
		bRtn = theApp.m_CustomizeTest.SetParameter(ParameterName, ParameterValue);
	}else if(strcmp(ActionName,_T("WIFI_RX_PER_TEST_OEM")) == NULL){
		if(strcmp(ParameterName,_T("PACKET_LENS")) == NULL)
			theApp.m_ExtDutParam.PACKET_LENS = atoi(ParameterValue);
		else if(strcmp(ParameterName,_T("WAVEFORM_FILE")) == NULL)
			strCopy(theApp.m_ExtDutParam.WAVEFORM, ParameterValue);
		bRtn = theApp.m_CustomizeTest.SetParameter(ParameterName, ParameterValue);
	}else if(strcmp(ActionName,_T("BT_TX_TEST_OEM")) == NULL){
		bRtn = theApp.m_CustomizeTest.SetParameter(ParameterName, ParameterValue);
	}else if(strcmp(ActionName,_T("BT_RX_BER_TEST_OEM")) == NULL){
		if(strcmp(ParameterName,_T("PACKET_LENS")) == NULL)
			theApp.m_ExtDutParam.PACKET_LENS = atoi(ParameterValue);
		else if(strcmp(ParameterName,_T("WAVEFORM_FILE")) == NULL)
			strCopy(theApp.m_ExtDutParam.WAVEFORM, ParameterValue);
		bRtn = theApp.m_CustomizeTest.SetParameter(ParameterName, ParameterValue);
   }else if(strcmp(ActionName,_T("WIFI_LOAD_DUT_REQUIRED")) == NULL)
      bRtn = theApp.m_CQDART_APIFun.SetParameter(ParameterName, ParameterValue);
	return bRtn;
}

extern "C" BOOL PASCAL EXPORT PreShootToDutParameter(LPSTR ActionName, LPSTR lpszRunInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	strCopy(theApp.m_ActionName, ActionName);
	if(strcmp(ActionName,_T("CUSTOMIZE_TEST")) == NULL)
		theApp.m_CustomizeTest.PreTest(lpszRunInfo);
	else if((strcmp(ActionName,_T("WIFI_EVM_TEST_OEM")) == NULL) || (strcmp(ActionName,_T("WIFI_SPECTRUM_MASK_TEST_OEM")) == NULL) || (strcmp(ActionName,_T("WIFI_TX_POWER_VERIFICATION_OEM")) == NULL))
		theApp.m_CustomizeTest.PreTest(lpszRunInfo);
	else if(strcmp(ActionName,_T("WIFI_RX_PER_TEST_OEM")) == NULL)
		theApp.m_CustomizeTest.PreTest(lpszRunInfo);
	else if(strcmp(ActionName,_T("BT_TX_TEST_OEM")) == NULL)
		theApp.m_CustomizeTest.PreTest(lpszRunInfo);
	else if(strcmp(ActionName,_T("BT_RX_BER_TEST_OEM")) == NULL)
		theApp.m_CustomizeTest.PreTest(lpszRunInfo);
	return TRUE;
}

//#define WEB 

#define QMSL 
extern "C" TM_RETURN PASCAL EXPORT CustomizeTest(funcGetMessage cbListMessage, funcGetMessage cbXmlMessage, funcGetMessage cbSfcsSpecMessage, funcGetMessage cbSfcsResultMessage, ERRORREPORT &ErrorReport, int nRetryCnt, BOOL bRetry, ...)
{
   TCHAR szMessage[DLL_INFO_SIZE]={0};
   vector<string> strResponse;
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
#ifdef WEB
   string m_HTTPInfo="";
   theApp.m_CHttpClient;
   // theApp.m_CHttpClient.HttpGet("http://192.168.0.1/cgi-bin/cgi", NULL, strResponse);
   theApp.m_CHttpClient.HttpPost("http://192.168.0.1/cgi-bin/cgi", "username=admin&password=pass", strResponse);
   CCommFunc::OutputMsgToBoth(cbListMessage, "%s", "HTTP Message:\n");
   for(int i=0; i < strResponse.size(); i++){
      int pos=0;
      CCommFunc::OutputMsgToBoth(cbListMessage, "%s", _T(strResponse.at(i).c_str()));
     // if( ((pos = strResponse.at(i).find_first_of("a href=")) != string::npos) && ((pos2 = strResponse.at(i).find_first_of("admin/system/firmware")) != string::npos) ){
       if( ((pos = strResponse.at(i).find("a href=")) != string::npos) && (strResponse.at(i).find("admin/system/firmware") != string::npos) ){
          int pos2= strResponse.at(i).find("stok=", pos);
          int pos3= strResponse.at(i).find("/admin/system/firmware", pos2);
          int length = pos3 - (pos2+5);
          m_HTTPInfo = strResponse.at(i).substr((pos2+5),length);
          cout << "1";
      }
   }
   CCommFunc::OutputMsgToBoth(cbListMessage, "%s", "***************************************************************");
   strResponse.clear();

   string sendURL = "http://192.168.0.1/cgi-bin/cgi/;stok=" + m_HTTPInfo +  "/admin/system/firmware";
   char cStr[1024];
   strcpy(cStr , sendURL.c_str());

   theApp.m_CHttpClient.HttpPost(cStr, "", strResponse);
   CCommFunc::OutputMsgToBoth(cbListMessage, "%s", "HTTP Message:\n");
   for(int i=0; i < strResponse.size(); i++){
       CCommFunc::OutputMsgToBoth(cbListMessage, "%s", _T(strResponse.at(i).c_str()));
       cout << "1";
   }
   CCommFunc::OutputMsgToBoth(cbListMessage, "%s", "***************************************************************");
  /* if(DEBUG){
      for(int i=0; i < strResponse.size(); i++)
          CCommFunc::OutputMsgToBoth(cbListMessage, "%s", _T(strResponse.at(i).c_str()));
   } */
  
	//return theApp.m_CustomizeTest.Test(cbListMessage, cbXmlMessage, cbSfcsSpecMessage, cbSfcsResultMessage, ErrorReport, nRetryCnt, bRetry);
#endif  

#ifdef QMSL
   printf("1111");
   CEdit wEdit;
   CRect rect(85, 110, 180, 210);
   wEdit.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP |
      ES_AUTOHSCROLL | WS_BORDER, rect, g_GlobalInfo.m_pWnd, 1234);
   wEdit.SetWindowText("11111");
   wEdit.SetFocus();
   printf("1111");
   theApp.m_CQLibDemo_WLAN_Sample.QLibDemo_WLAN();
#endif 

   return TM_RETURN_PASS;
}


BOOL CIQTestManagerApp::LoadTestManagerDll(LPSTR lpszRunInfo)
{
	TCHAR szPath[MAX_PATH]={0}, szName[MAX_PATH]={0};
	TCHAR szLocalPath[MAX_PATH]={0};
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	IQ_TM_RETURN eRtn;	
	HINSTANCE hDLL=NULL;

	//// https://blog.csdn.net/CNHK1225/article/details/76167469
	//TCHAR szBuffer[MAX_PATH] = { 0 };
	//hDLL = GetModuleHandle(szName);
	//GetModuleFileName(hDLL, szBuffer, sizeof(szBuffer) / sizeof(TCHAR) - 1);
	//string str;
	//str.append(szBuffer);
	//size_t pos=str.find_last_of("\\");
	//str.erase(pos);
	//SetDllDirectory(str.c_str());

	sprintf_s(szName, sizeof(szName), "TestManager.dll", g_GlobalInfo.szLocalPath);
//	sprintf_s(szName, sizeof(szName), "%sOEM\\IQTestManager\\TestManager.dll", g_GlobalInfo.szLocalPath);
	if(GetModuleHandle(szName) != NULL) return TRUE;

	sprintf_s(szPath, sizeof(szPath), "%s", g_GlobalInfo.szLocalPath);
//	sprintf_s(szPath, sizeof(szPath), "%sOEM\\IQTestManager", g_GlobalInfo.szLocalPath);
	SetDllDirectory(szPath);

	hDLL = LoadLibrary(szName);
	if(hDLL == NULL)
	{
		sprintf_s(szMessage, DLL_INFO_SIZE, "Load dll failed, [Name:%s]", szName);
		AfxMessageBox(szMessage, MB_SETFOREGROUND);
		return FALSE;
	}else{
		TM_RegisterTechnologyDll	=(P_TM_RegisterTechnologyDll)	GetProcAddress	(hDLL,"TM_RegisterTechnologyDll");
		TM_Run						   =(P_TM_Run)						GetProcAddress	(hDLL,"TM_Run");
		TM_ClearParameters			=(P_TM_ClearParameters)			GetProcAddress	(hDLL,"TM_ClearParameters");
		TM_AddDoubleParameter		=(P_TM_AddDoubleParameter)		GetProcAddress	(hDLL,"TM_AddDoubleParameter");
		TM_AddIntegerParameter		=(P_TM_AddIntegerParameter)		GetProcAddress	(hDLL,"TM_AddIntegerParameter");
		TM_AddStringParameter		=(P_TM_AddStringParameter)		GetProcAddress	(hDLL,"TM_AddStringParameter");
		TM_GetDoubleReturnPair		=(P_TM_GetDoubleReturnPair)		GetProcAddress	(hDLL,"TM_GetDoubleReturnPair");
		TM_GetIntegerReturnPair		=(P_TM_GetIntegerReturnPair)	GetProcAddress	(hDLL,"TM_GetIntegerReturnPair");
		TM_GetStringReturnPair		=(P_TM_GetStringReturnPair)		GetProcAddress	(hDLL,"TM_GetStringReturnPair");
		TM_GetStringReturn			=(P_TM_GetStringReturn)			GetProcAddress	(hDLL,"TM_GetStringReturn");
		TM_GetIntegerReturn			=(P_TM_GetIntegerReturn)		GetProcAddress	(hDLL,"TM_GetIntegerReturn");
		TM_GetDoubleReturn			=(P_TM_GetDoubleReturn)			GetProcAddress	(hDLL,"TM_GetDoubleReturn");
	}
	if((eRtn = TM_RegisterTechnologyDll(_T("WIFI_11AC"), _T("WiFi_11AC_Test.dll"), &g_GlobalInfo.ID_WIFI_11AC)) == TM_ERR_OK)
		eRtn = TM_RegisterTechnologyDll(_T("BT"), _T("BT_Test.dll"), &g_GlobalInfo.ID_BT);	
//	SetDllDirectory(NULL);
	ReadConfigStationIni();
	return (eRtn == TM_ERR_OK)? TRUE:FALSE;
}

extern "C" BOOL PASCAL EXPORT StartFrameModulatedTx(DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	memcpy(&theApp.m_DutParamWiFi, &DutParam, sizeof(DUT_PARAM));
	return theApp.m_CustomizeTest.WifiVerificationTest(theApp.m_ActionName, DutParam, theApp.m_ExtDutParam, lpszRunInfo);
}
extern "C" BOOL PASCAL EXPORT TxEvmReturn(double *evm, LPSTR lpszRunInfo)
{
	return theApp.m_CustomizeTest.GetTestResult(_T("EVM_AVG_DB"), evm, lpszRunInfo);
}

extern "C" BOOL PASCAL EXPORT TxPowerReturn(double *power, LPSTR lpszRunInfo)
{
	if(strcmp(theApp.m_ActionName, _T("WIFI_SPECTRUM_MASK_TEST_OEM")) == NULL)
		return theApp.m_CustomizeTest.GetTestResult(_T("POWER_AVERAGE_DBM"), power, lpszRunInfo);
	else
		return theApp.m_CustomizeTest.GetTestResult(_T("POWER_AVG_DBM"), power, lpszRunInfo);
}

extern "C" BOOL PASCAL EXPORT TxPpmReturn(double *ppm, LPSTR lpszRunInfo)
{
	return theApp.m_CustomizeTest.GetTestResult(_T("FREQ_ERROR_AVG"), ppm, lpszRunInfo);
}

extern "C" BOOL PASCAL EXPORT TxLoLeakageReturn(double *lo, LPSTR lpszRunInfo)
{
	strCopy(theApp.m_ActionName, _T("TX_VERIFY_SPECTRUM"));
	if(!StartFrameModulatedTx(theApp.m_DutParamWiFi, lpszRunInfo)) return FALSE;
	return theApp.m_CustomizeTest.GetTestResult(_T("LO_LEAKAGE_DB"), lo, lpszRunInfo);
}

extern "C" BOOL PASCAL EXPORT TxMaskPercentReturn(double *fail_percent, LPSTR lpszRunInfo)
{
	return theApp.m_CustomizeTest.GetTestResult(_T("VIOLATION_PERCENT"), fail_percent, lpszRunInfo);
}

extern "C" BOOL PASCAL EXPORT GetTargetPower(DUT_PARAM &DutParam,LPSTR lpszRunInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	return theApp.m_GetTargetPower.Test(theApp.m_ActionName, DutParam, lpszRunInfo);
}

extern "C" BOOL PASCAL EXPORT StartRxPacketReceive(DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.m_CustomizeTest.WifiVerificationTest(theApp.m_ActionName, DutParam, theApp.m_ExtDutParam, lpszRunInfo);
}

extern "C" BOOL PASCAL EXPORT GetRxPacketCount(DUT_PARAM DutParam, double &dbRxPacketCount, LPSTR lpszRunInfo)
{
	BOOL bRtn;
	int TotalPackets=0, GoodPackets=0;
	bRtn = theApp.m_CustomizeTest.GetTestResult(_T("GOOD_PACKETS"), &GoodPackets, lpszRunInfo);
	bRtn = theApp.m_CustomizeTest.GetTestResult(_T("TOTAL_PACKETS"), &TotalPackets, lpszRunInfo);
	if(TotalPackets != theApp.m_ExtDutParam.PACKET_LENS){
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Different packet sizes,[Expect:%d, Actual:%d]", theApp.m_ExtDutParam.PACKET_LENS, TotalPackets);
		return FALSE;
	}
	dbRxPacketCount = (double)GoodPackets;
	return bRtn;
}

extern "C" BOOL PASCAL EXPORT ParserWiFiCableLossToDut(double *dbCableLoss, LPSTR lpszRunInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	return theApp.m_ParserCableLoss.Test(dbCableLoss, lpszRunInfo);
}


extern "C" BOOL PASCAL EXPORT BtTxReturn(BT_DUT_PARAM BtDutParam, BT_TX_RESULT *result, LPSTR lpszRunInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	return theApp.m_CustomizeTest.GetBtTestResult(BtDutParam, result, lpszRunInfo);
}

extern "C" BOOL PASCAL EXPORT BtStartFrameModulatedTx(BT_DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	return theApp.m_CustomizeTest.BtVerificationTest(theApp.m_ActionName, DutParam, theApp.m_ExtDutParam, lpszRunInfo);
}
extern "C" BOOL PASCAL EXPORT BtGetTargetPower(BT_DUT_PARAM &DutParam,LPSTR lpszRunInfo)
{
	return TRUE;
}
extern "C" BOOL PASCAL EXPORT BtGetPowerIndex(BT_DUT_PARAM &DutParam, LPSTR lpszRunInfo)
{
	return TRUE;
}
extern "C" BOOL PASCAL EXPORT BtResetRxPacketCount(BT_DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	return TRUE;
}
extern "C" BOOL PASCAL EXPORT BtStartRxPacketReceive(BT_DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	return theApp.m_CustomizeTest.BtVerificationTest(theApp.m_ActionName, DutParam, theApp.m_ExtDutParam, lpszRunInfo);
}
extern "C" BOOL PASCAL EXPORT BtStopRxPacketReceive(BT_DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	return TRUE;
}

extern "C" BOOL PASCAL EXPORT BtGetRxPacketCount(BT_DUT_PARAM DutParam, double &dbRxPacketCount, LPSTR lpszRunInfo)
{
	BOOL bRtn;
	double BER;
	if(strstr(DutParam.PACKET_TYPE, _T("LE")))
		bRtn = theApp.m_CustomizeTest.GetTestResult(_T("PER"), &BER, lpszRunInfo);
	else
		bRtn = theApp.m_CustomizeTest.GetTestResult(_T("BER"), &BER, lpszRunInfo);
	dbRxPacketCount = (double)theApp.m_ExtDutParam.PACKET_LENS - (theApp.m_ExtDutParam.PACKET_LENS*BER/100);
	return bRtn;
}

#define DEBUG 0
extern "C" BOOL PASCAL EXPORT LoadDutRequired(BOOL bRetry, LPSTR lpszRunInfo)
{
  TCHAR szMessage[DLL_INFO_SIZE]={0};
  vector<string> strResponse;
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  theApp.m_CHttpClient;
 // theApp.m_CHttpClient.HttpGet("http://192.168.0.1/cgi-bin/cgi", NULL, strResponse);
  theApp.m_CHttpClient.HttpPost("http://192.168.0.1/cgi-bin/cgi", "username=admin&password=pass", strResponse);
  return TRUE;
}

