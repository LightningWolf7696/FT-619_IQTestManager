#pragma once
#include "../CommFunc.h"

class CCustomizeTest
{
public:
	CCustomizeTest(void);
public:
	~CCustomizeTest(void);


private:
	typedef struct _SCRIPT_PARAMETER
	{
	}SCRIPT_PARAMETER;

	typedef struct _REQUIRED_SCRIPT_PARAMETER
	{
	}REQUIRED_SCRIPT_PARAMETER;

	typedef struct _SET_PARAMETER
	{
		TCHAR	name[DLL_INFO_SIZE];
		TCHAR	value[DATA_MAX_BUFFER_SIZE];
		int		size;
	}SET_PARAMETER;

	typedef struct _IQFACT_PARAMETER
	{
		TCHAR	name[DLL_INFO_SIZE];
		TCHAR	value[DLL_INFO_SIZE];
		IQFACT_INTERFACE_TYPE	type;
	}IQFACT_PARAMETER;

	typedef struct _OUTPUT_TO_SFCS_DATA
	{
		TCHAR	header[50];
		TCHAR	value[50];
	}OUTPUT_TO_SFCS_DATA;

	typedef struct _TEST_PARAMETER
	{
		ERRORREPORT			ErrorReport;
		double				dbTestTimeCost;
		TM_RETURN			eRtnTM;
	}TEST_PARAMETER;

	SCRIPT_PARAMETER				m_ScriptParam;
	REQUIRED_SCRIPT_PARAMETER		m_RequiredParam;
	TEST_PARAMETER					m_TestParam;
	funcGetMessage					m_cbListMessage;
	funcGetMessage					m_cbXmlMessage;
	funcGetMessage					m_cbSfcsSpecMessage;
	funcGetMessage					m_cbSfcsResultMessage;
	PERRORREPORT					m_pErrorReport;
	TCHAR							m_ReferenceScript[MAX_PATH];

public:
	BOOL Clean();
	BOOL SetParameter(LPSTR ParameterName,LPSTR ParameterValue);
	BOOL PreTest(LPSTR lpszRunInfo);
	TM_RETURN Test(funcGetMessage cbListMessage, funcGetMessage cbXmlMessage, funcGetMessage cbSfcsSpecMessage, funcGetMessage cbSfcsResultMessage, ERRORREPORT &ErrorReport, int nRetryCnt, BOOL bRetry, ...);
	BOOL WifiVerificationTest(LPSTR ActionName, DUT_PARAM DutParam, EXT_DUT_PARAM ExtDutParam, LPSTR lpszRunInfo);
	BOOL BtVerificationTest(LPSTR ActionName, BT_DUT_PARAM DutParam, EXT_DUT_PARAM ExtDutParam, LPSTR lpszRunInfo);
	BOOL GetTestResult(LPSTR lpszItem, int *result, LPSTR lpszRunInfo);
	BOOL GetTestResult(LPSTR lpszItem, double *result, LPSTR lpszRunInfo);
	BOOL GetTestResult(LPSTR lpszItem, LPSTR result, int size, LPSTR lpszRunInfo);
	BOOL GetBtTestResult(BT_DUT_PARAM BtDutParam, BT_TX_RESULT *result, LPSTR lpszRunInfo);

private:
	CArray <SET_PARAMETER, SET_PARAMETER> m_ParamArray;
	CArray <IQFACT_PARAMETER, IQFACT_PARAMETER> m_IQFactInput;
	CArray <IQFACT_PARAMETER, IQFACT_PARAMETER> m_IQFactOutput;
	CArray <OUTPUT_TO_SFCS_DATA, OUTPUT_TO_SFCS_DATA> m_SfcsArray;

	TM_RETURN SyncTestMode();
	BOOL GetTestParameter(LPSTR lpszKey, LPSTR lpszValue, int nSize, int length=0);
	BOOL GetTestParameter(LPSTR lpszKey, int& lpszValue);
	BOOL GetTestParameter(LPSTR lpszKey, double& lpszValue);
	CString OutputTestParameterToXML();
	TM_RETURN TestCommand();
	TM_RETURN RunIQFact();
	TM_RETURN UploadVariable();
	BOOL LoadIQScriptFile(BOOL bOutputMsg, LPSTR lpszFileName, LPSTR lpszSectionName, LPSTR lpszRunInfo);
	BOOL FileParser(BOOL bOutputMsg, LPSTR lpszFileName, LPSTR lpszStart, LPSTR lpszStop, LPSTR lpszRunInfo);
	BOOL FilterString(BOOL bStartRev, BOOL bStopRev, CString strSource, LPSTR lpszStart, LPSTR lpszStop, CString &strResult, LPSTR lpszRunInfo);
	BOOL ParseIQInputParam(CString &strRaw, IQFACT_PARAMETER &stParameter, BOOL &find);
	BOOL SetParameterToTestManager(IQFACT_PARAMETER stParameter, LPSTR lpszRunInfo);
	BOOL UpdateWifiDutParameter(LPSTR ActionName, DUT_PARAM DutParam, EXT_DUT_PARAM ExtDutParam, LPSTR lpszRunInfo);
	BOOL UpdateBtDutParameter(LPSTR ActionName, BT_DUT_PARAM DutParam, EXT_DUT_PARAM ExtDutParam, LPSTR lpszRunInfo);
	BOOL SyncAntenna(LPSTR TxRx, LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo);
	BOOL SyncFrequency(LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo);
	BOOL SyncPower(LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo);
	BOOL SyncPowerBT(LPSTR ActionName, BT_DUT_PARAM DutParam, LPSTR lpszRunInfo);
	BOOL SyncBandwidth(LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo);
	BOOL SyncDataRate(LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo);
	BOOL SyncDataPreamble(LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo);
	BOOL SyncDataBand(LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo);
	BOOL SyncPacketCount(LPSTR ActionName, int PacketCount, LPSTR lpszRunInfo);
	BOOL SyncPacketCountBT(LPSTR ActionName, BT_DUT_PARAM DutParam, int PacketCount, LPSTR lpszRunInfo);
	BOOL SyncWaveform(LPSTR ActionName, LPSTR lpszWaveform, LPSTR lpszRunInfo);
	BOOL SyncFrequencyBT(LPSTR ActionName, BT_DUT_PARAM DutParam, LPSTR lpszRunInfo);
	BOOL SyncPacketType(LPSTR ActionName, BT_DUT_PARAM DutParam, LPSTR lpszRunInfo);
	BOOL OuputIQfactParameter(LPSTR name, LPSTR value, IQFACT_INTERFACE_TYPE type, IQFACT_PARAMETER &stParameter);
	BOOL ReplaceIQfactInput(IQFACT_PARAMETER stParameter);
	BOOL QuickUpdateAntenna(LPSTR TxRx, int a, int b, int c, int d);
	BOOL SyncWifi_GTK2IQFact(LPSTR ActionName, CString &strOutput, IQFACT_TYPE& eType, LPSTR lpszRunInfo);
	BOOL SyncBt_GTK2IQFact(LPSTR ActionName, CString &strOutput, BT_DUT_PARAM DutParam, IQFACT_TYPE& eType, LPSTR lpszRunInfo);	
	TM_RETURN LoadRefenceScript(IQFACT_TYPE eType);
	TM_RETURN RunIQTestManager(LPSTR ActionName, DUT_PARAM WifiDutParam, BT_DUT_PARAM BtDutParam, EXT_DUT_PARAM ExtDutParam, COMMON_TEST_MODE eMode);
	BOOL ExtInParameter(LPSTR lpszRunInfo);
	BOOL ReadTestInfoParameter(LPSTR input, IQFACT_PARAMETER &output);
	BOOL ReadActionParameter(LPSTR input, IQFACT_PARAMETER &output);
	TM_RETURN SyncCompareType();
	TM_RETURN CompareValueFunc();
	TM_RETURN SfcsCollection();
	BOOL ExtOutParameter(LPSTR lpszRunInfo);
	BOOL UploadIQfactOutput(IQFACT_PARAMETER stParameter);
	BOOL PreTestEvent(LPSTR name, LPSTR lpszRunInfo);
	BOOL PostTestEvent(LPSTR ActionName, LPSTR lpszRunInfo);
	void FileDataToTestLog(OUTPUT_LOG_TYPE eType, LPSTR lpszFileName);
	TM_RETURN SyncFileType();
	TM_RETURN FileDelete();
	TM_RETURN FileOutput();
	TM_ID SyncTechnologyID(LPSTR lpszFileName);
	BOOL ParseRawData(CString src, double &dest, CString start, CString stop);
	BOOL ParseRawData(CString src, int &dest, CString start, CString stop);
	BOOL QCA_WifiTxCalData(LPSTR lpszRunInfo);
	BOOL RemoveSfcsDuplicate();
};
