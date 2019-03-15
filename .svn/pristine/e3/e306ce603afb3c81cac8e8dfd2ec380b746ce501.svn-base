#include "../StdAfx.h"
#include "CustomizeTest.h"
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include <io.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

CCustomizeTest::CCustomizeTest(void)
{
	zeroIt(m_ReferenceScript);
}

CCustomizeTest::~CCustomizeTest(void)
{

}

BOOL CCustomizeTest::Clean()
{
	zeroIt(m_ScriptParam);
	zeroIt(m_RequiredParam);
	zeroIt(m_TestParam);
	m_ParamArray.RemoveAll();
	m_TestParam.dbTestTimeCost = GetTickCount();	
	return TRUE;
}
BOOL CCustomizeTest::SetParameter(LPSTR ParameterName,LPSTR ParameterValue)
{	
	SET_PARAMETER	Data={0};
	strCopy(Data.name, ParameterName);
	memcpy(Data.value, ParameterValue, strlen(ParameterValue));
	m_ParamArray.Add(Data);

	return TRUE;
}
BOOL CCustomizeTest::PreTest(LPSTR lpszRunInfo)
{
	return TRUE;
}

TM_RETURN CCustomizeTest::Test(funcGetMessage cbListMessage, funcGetMessage cbXmlMessage, funcGetMessage cbSfcsSpecMessage, funcGetMessage cbSfcsResultMessage, ERRORREPORT &ErrorReport, int nRetryCnt, BOOL bRetry, ...)
{
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szCmd[MAX_PATH]={0};
	TM_RETURN TmRtn=TM_RETURN_ERROR;
	CString strData="";
	TCHAR szData[DLL_INFO_SIZE]={0};
	CString strTemp;
	CString strOutputMsg="";

	m_cbListMessage = cbListMessage;
	m_cbXmlMessage = cbXmlMessage;
	m_cbSfcsSpecMessage = cbSfcsSpecMessage;
	m_cbSfcsResultMessage = cbSfcsResultMessage;
	m_pErrorReport= &ErrorReport; 
	if(m_TestParam.eRtnTM == TM_RETURN_SKIP)
	{
		CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", _T("Skip this test item."));
		return TM_RETURN_SKIP;
	}

	//Collect test result to XML
	CCommFunc::ExportTestMessage(m_cbXmlMessage, "<TestCase name=\"%s\">", _T("CUSTOMIZE_TEST"));
	CCommFunc::ExportTestMessage(m_cbXmlMessage, "<Data>");

	GetTestParameter("RUN_XML", szData, sizeof(szData));
	if(strcmp(szData,"TRUE") == NULL)	OutputTestParameterToXML();
	TmRtn = SyncTestMode();

	CCommFunc::ExportTestMessage(m_cbXmlMessage,"<TEST_TIME>%.3f</TEST_TIME>", (float)((GetTickCount() - m_TestParam.dbTestTimeCost)/1000));
	CCommFunc::ExportTestMessage(m_cbXmlMessage, "</Data>");
	CCommFunc::ExportTestMessage(m_cbXmlMessage, "</TestCase>");

	if(TmRtn == TM_RETURN_PASS)
	{
		strCopy(szCmd, _T("PASSMSG"));
		GetTestParameter(szCmd, szMessage, sizeof(szMessage));
	}else{
		strCopy(szCmd, _T("ERRORMSG"));
		GetTestParameter(szCmd, szMessage, sizeof(szMessage));

		strCopy(szCmd, _T("ERRORCODE"));
		GetTestParameter(szCmd, szData, sizeof(szData));
		{
			if(strcmp(m_pErrorReport->ErrorCode,"") == NULL)
				strCopy(m_pErrorReport->ErrorCode, szData);
		}
		strTemp = CCommFunc::TransXmlSymbo(m_pErrorReport->ErrorMsg);
		if(strTemp == "")
			strCopy(m_pErrorReport->ErrorMsg, szMessage);
		else
			strCopy(m_pErrorReport->ErrorMsg, strTemp.GetBuffer());
		strTemp = CCommFunc::TransXmlSymbo(m_pErrorReport->ErrorDetail);
		if(strTemp.GetLength()>DLL_INFO_SIZE-1)
			strCopy(m_pErrorReport->ErrorDetail, strTemp.Left(DLL_INFO_SIZE-1));
		else
			strCopy(m_pErrorReport->ErrorDetail, strTemp.GetBuffer());
	}
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "@@@@@@ Test Result :%s @@@@@@@ [%s]",(TmRtn == TM_RETURN_PASS)?_T("PASS"):_T("FAIL"), szMessage);

	return TmRtn;
}

BOOL CCustomizeTest::GetTestParameter(LPSTR lpszKey, LPSTR lpszValue, int nSize, int length)
{	
	LPSTR lpszName;

	for(int i=0 ; i<m_ParamArray.GetSize() ; i++)
	{
		lpszName = m_ParamArray.GetAt(i).name;
		if(strcmp(lpszName, lpszKey) == 0)
		{			
			if(length>0)
				memcpy(lpszValue, m_ParamArray.GetAt(i).value, length);
			else
				strcpy_s(lpszValue, nSize, m_ParamArray.GetAt(i).value);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CCustomizeTest::GetTestParameter(LPSTR lpszKey, double& lpszValue)
{	
	LPSTR lpszName;

	for(int i=0 ; i<m_ParamArray.GetSize() ; i++)
	{
		lpszName = m_ParamArray.GetAt(i).name;
		if(strcmp(lpszName, lpszKey) == 0)
		{			
			lpszValue  = atof(m_ParamArray.GetAt(i).value);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CCustomizeTest::GetTestParameter(LPSTR lpszKey, int& lpszValue)
{	
	LPSTR lpszName;

	for(int i=0 ; i<m_ParamArray.GetSize() ; i++)
	{
		lpszName = m_ParamArray.GetAt(i).name;
		if(strcmp(lpszName, lpszKey) == 0)
		{			
			lpszValue  = atoi(m_ParamArray.GetAt(i).value);
			return TRUE;
		}
	}
	return FALSE;
}

TM_RETURN CCustomizeTest::SyncTestMode()
{
	BOOL bRtn = FALSE;
	TCHAR szCmd[MAX_PATH]={0};
	COMMON_TEST_MODE eMode=CMODE_NULL;
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szDetail[ERRORDETAIL_STRING]={0};
	TCHAR szMode[64]={0};
	TCHAR szResult[DLL_INFO_SIZE]={0};

	strCopy(szCmd, _T("MODE"));
	if(!GetTestParameter(szCmd, szMode, sizeof(szMode))) goto NOT_FOUND;
	for(int idx=0 ; idx<CMODE_NULL; idx++)
	{		
		if(strcmp(szMode, TestModeTable[idx]) == NULL)
		{
			eMode = (COMMON_TEST_MODE)idx;
			break;
		}
	}

	sprintf_s(szMessage, sizeof(szMessage), "======== Mode : [%s] ========", szMode);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);

	switch(eMode)
	{
	case CMODE_IQFACT:
		return RunIQFact();
	case CMODE_CMD:
		return TestCommand();
	case CMODE_UPLOAD:
		return UploadVariable();
	case CMODE_COMPARE:
		return SyncCompareType();
	case CMODE_SFCS:
		return SfcsCollection();
	case CMODE_FILE:
		return SyncFileType();
	default:
		sprintf_s(szMessage, sizeof(szMessage), "Test mode not support, [Mode : %s]", szMode);
		goto ERRORED;
	}

ERRORED:
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage,  _T("CUSTOMIZE_TEST"), TEST_FAIL);
	return TM_RETURN_ERROR;

NOT_FOUND:
	sprintf_s(szDetail, sizeof(szDetail), "MODE :[%s]", TestModeTable[eMode]);
	sprintf_s(szMessage, DLL_INFO_SIZE, "Can not find parameter,[Name: %s]", szCmd);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage, _T("CUSTOMIZE_TEST"), TEST_FAIL);
	return TM_RETURN_ERROR;
}

CString CCustomizeTest::OutputTestParameterToXML()
{	
	SET_PARAMETER param={0};
	CString strData="";
	TCHAR szTemp[DATA_MAX_BUFFER_SIZE]={0};
	for(int i=0 ; i<m_ParamArray.GetSize() ; i++)
	{
		param = m_ParamArray.GetAt(i);
		//if(!strData.IsEmpty()) 
		//	strData += "\r\n";
		sprintf_s(szTemp, sizeof(szTemp), "<%s>%s</%s>", CCommFunc::TransXmlSymbo(param.name).GetBuffer(), CCommFunc::TransXmlSymbo(param.value).GetBuffer(), CCommFunc::TransXmlSymbo(param.name).GetBuffer());
		//		strData += szTemp;
		CCommFunc::SeparateListInfoSize(m_cbXmlMessage, szTemp, strlen(szTemp));
	}
	return strData;
}


TM_RETURN CCustomizeTest::TestCommand()
{
	TCHAR szMessage[DATA_MAX_BUFFER_SIZE]={0};
	TCHAR szCmd[MAX_PATH]={0};
	TCHAR szTmp[DLL_INFO_SIZE]={0};	
	TCHAR szData[DLL_INFO_SIZE]={0};
	int nTimeOut;	
	TM_RETURN TmRtn=TM_RETURN_PASS;
	TCHAR szDetail[ERRORDETAIL_STRING]={0};
	TCHAR szSend[DLL_INFO_SIZE]={0};
	TCHAR szResponse[DLL_INFO_SIZE]={0};

	strCopy(szCmd, _T("ERRORCODE"));
	if(!GetTestParameter(szCmd, m_TestParam.ErrorReport.ErrorCode, sizeof(m_TestParam.ErrorReport.ErrorCode))) goto NOT_FOUND;
	strCopy(szCmd, _T("ERRORMSG"));
	GetTestParameter(szCmd, m_TestParam.ErrorReport.ErrorMsg, sizeof(m_TestParam.ErrorReport.ErrorMsg));
	strCopy(szCmd, _T("SEND"));
	if(!GetTestParameter(szCmd, szSend, sizeof(szSend))) goto NOT_FOUND;
	sprintf_s(szTmp, sizeof(szTmp), ", Send :[%s]", szSend);
	strCat(szDetail, szTmp);

	strCopy(szCmd, _T("RESPONSE_PASS"));
	if(!GetTestParameter(szCmd, szResponse, sizeof(szResponse))) goto NOT_FOUND;
	sprintf_s(szTmp, sizeof(szTmp), ", Expect :[%s]", szResponse);
	strCat(szDetail, szTmp);

	strCopy(szCmd, _T("TIMEOUT"));
	if(!GetTestParameter(szCmd, szData, sizeof(szData))) goto NOT_FOUND;
	nTimeOut = atoi(szData);

	if(!CCommFunc::SerialPortQueryMultiDataRetry(szSend, szResponse, nTimeOut, szMessage))
		goto FAILED;

	if(TmRtn == TM_RETURN_FAIL) goto FAILED;
	else if(TmRtn == TM_RETURN_ERROR) goto ERRORED;

	CCommFunc::OutputMsgToBoth(m_cbListMessage, "Test cmd query is successful, [%s]", szDetail);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	return TM_RETURN_PASS;

FAILED:	
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szDetail);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	if(strlen(szMessage)>= DLL_INFO_SIZE)	szMessage[DLL_INFO_SIZE/2] = 0;
	if(strcmp(m_TestParam.ErrorReport.ErrorMsg, "") == NULL)
		sprintf_s(m_TestParam.ErrorReport.ErrorMsg, sizeof(m_TestParam.ErrorReport.ErrorMsg), "%s", szMessage);
	else{
		sprintf_s(m_TestParam.ErrorReport.ErrorMsg, sizeof(m_TestParam.ErrorReport.ErrorMsg), "%s,[%s]", m_TestParam.ErrorReport.ErrorMsg, szMessage);
	}
	CCommFunc::FillErrorReport(m_pErrorReport, ENABLE_SFCS, m_TestParam.ErrorReport.ErrorCode, szDetail, m_TestParam.ErrorReport.ErrorMsg,  _T("CUSTOMIZE_TEST"), TEST_FAIL);
	return TM_RETURN_FAIL;

ERRORED:
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szDetail);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	if(strlen(szMessage)>= DLL_INFO_SIZE)	szMessage[DLL_INFO_SIZE/2] = 0;
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage,  _T("CUSTOMIZE_TEST"), TEST_FAIL);
	return TM_RETURN_ERROR;

NOT_FOUND:
	sprintf_s(szMessage, DLL_INFO_SIZE, "Can not find parameter,[Name: %s]", szCmd);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szDetail);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	if(strlen(szMessage)>= DLL_INFO_SIZE)	szMessage[DLL_INFO_SIZE/2] = 0;
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage,  _T("CUSTOMIZE_TEST"), TEST_FAIL);
	return TM_RETURN_ERROR;
}

TM_RETURN CCustomizeTest::RunIQFact()
{
	IQFACT_TYPE eType = IQF_TYPE_NULL;
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	BOOL bRtn = FALSE;
	TCHAR szCmd[MAX_PATH]={0};
	TCHAR szData[DLL_INFO_SIZE]={0};
	TCHAR szDetail[ERRORDETAIL_STRING]={0};
	TCHAR ActionName[64]={0};
	DUT_PARAM WifiDutParam={0};
	BT_DUT_PARAM BtDutParam={0};
	EXT_DUT_PARAM ExtDutParam={0};

	strCopy(szCmd, _T("TYPE"));
	if(!GetTestParameter(szCmd, szData, sizeof(szData))) goto NOT_FOUND;


	for(int idx=0 ; idx<IQF_TYPE_NULL; idx++)
	{		
		if(strcmp(szData, IQFactTypeTable[idx]) == NULL)
		{
			eType = (IQFACT_TYPE)idx;
			break;
		}
	}
	strCopy(ActionName, szData);

	//if(eType == IQF_TYPE_NULL)
	//{
	//	sprintf_s(szMessage, sizeof(szMessage), "Test type not support, [Type : %s]", szData);
	//	goto ERRORED;
	//}	

	//switch(eType)
	//{
	//case IQF_TYPE_GLOBAL_SETTINGS:
	//case IQF_TYPE_CONNECT_IQ_TESTER:
	//case IQF_TYPE_INSERT_DUT:
	//case IQF_TYPE_INITIALIZE_DUT:
	//case IQF_TYPE_LOAD_PATH_LOSS_TABLE:
	//case IQF_TYPE_TX_SET_PARAMETERS:
	//case IQF_TYPE_XTAL_CALIBRATION:
	//case IQF_TYPE_TX_CALIBRATION:
	//case IQF_TYPE_APT_GET_LOCK:
	//case IQF_TYPE_RX_CALIBRATION:
	//case IQF_TYPE_APT_RELEASE_LOCK:
	//case IQF_TYPE_WRITE_MAC_ADDRESS:
	//case IQF_TYPE_FINALIZE_EEPROM:
	//case IQF_TYPE_REMOVE_DUT:
	//case IQF_TYPE_DISCONNECT_IQ_TESTER:
	//case IQF_TYPE_TX_VERIFY_EVM:
	//case IQF_TYPE_RX_VERIFY_PER:
	//case IQF_TYPE_TX_VERIFY_SPECTRUM:
	//case IQF_TYPE_TX_VERIFY_MASK:
	//	break;
	//}

	if(eType == IQF_LOAD_REFERENCE_SCRIPT)
		return LoadRefenceScript(eType);
	else{
		return RunIQTestManager(ActionName, WifiDutParam, BtDutParam, ExtDutParam, CMODE_IQFACT);
	}

ERRORED:
	sprintf_s(szDetail, sizeof(szDetail), "Type :[%s]", IQFactTypeTable[eType]);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage, _T("CUSTOMIZE_TEST"), TEST_FAIL);
	return TM_RETURN_ERROR;

NOT_FOUND:
	sprintf_s(szDetail, sizeof(szDetail), "MODE :[%s]", IQFactTypeTable[eType]);
	sprintf_s(szMessage, DLL_INFO_SIZE, "Can not find parameter,[Name: %s]", szCmd);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage, _T("CUSTOMIZE_TEST"), TEST_FAIL);
	return TM_RETURN_ERROR;
}

TM_RETURN CCustomizeTest::RunIQTestManager(LPSTR ActionName, DUT_PARAM WifiDutParam, BT_DUT_PARAM BtDutParam, EXT_DUT_PARAM ExtDutParam, COMMON_TEST_MODE eMode)
{
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szCmd[MAX_PATH]={0};
	TCHAR szTmp[DLL_INFO_SIZE]={0};	
	TCHAR szData[DLL_INFO_SIZE]={0};
	TM_RETURN TmRtn=TM_RETURN_PASS;
	TCHAR szDetail[ERRORDETAIL_STRING]={0};
	TCHAR szFileName[DLL_INFO_SIZE]={0};
	TCHAR szResponse[DLL_INFO_SIZE]={0};	
	IQ_TM_RETURN eRtn;
	BOOL bVerifyItem=FALSE;
	BOOL bOutputMsg=TRUE;
	BOOL bSameTest=FALSE;
	static TCHAR PrevTestType[64]={0};
	IQFACT_PARAMETER stParameter;

	strCopy(szCmd, _T("ERRORCODE"));
	GetTestParameter(szCmd, m_TestParam.ErrorReport.ErrorCode, sizeof(m_TestParam.ErrorReport.ErrorCode));
	strCopy(szCmd, _T("ERRORMSG"));
	GetTestParameter(szCmd, m_TestParam.ErrorReport.ErrorMsg, sizeof(m_TestParam.ErrorReport.ErrorMsg));

	
	bVerifyItem = (eMode == CMODE_IQFACT)? FALSE : TRUE;

	if(strcmp(ActionName,PrevTestType) == NULL)
		bSameTest = TRUE;

	if(!bSameTest || !bVerifyItem){
		m_IQFactInput.RemoveAll();
		strCopy(szCmd, _T("FILE"));
		GetTestParameter(szCmd, szData, sizeof(szData));
		if(strcmp(szData,"") == NULL)
		{
			if(strcmp(m_ReferenceScript,"") == NULL)
				goto NOT_FOUND;
			strCopy(szData, m_ReferenceScript);
		}
		sprintf_s(szTmp, sizeof(szTmp), ", Send :[%s]", szData);
		strCat(szDetail, szTmp);

		if(strncmp(szData, _T(".\\"),2) == 0)
			sprintf_s(szFileName, sizeof(szFileName),"%s%s",g_GlobalInfo.szLocalPath, &szData[2]);
		else		
			strCopy(szFileName, szData);

		if(!LoadIQScriptFile(bOutputMsg, szFileName, ActionName, szMessage)) goto ERRORED;
	}

	
	if(bVerifyItem)
	{
		bOutputMsg = (g_GlobalInfo.TraceMessage)?TRUE:FALSE;
		if((strcmp(ActionName, IQF_TYPE_TX_BDR_STR) == NULL) || 
		   (strcmp(ActionName, IQF_TYPE_TX_EDR_STR) == NULL) || 
		   (strcmp(ActionName, IQF_TYPE_TX_LE_STR) == NULL) || 
		   (strcmp(ActionName, IQF_TYPE_RX_BDR_STR) == NULL) || 
		   (strcmp(ActionName, IQF_TYPE_RX_EDR_STR) == NULL) || 
		   (strcmp(ActionName, IQF_TYPE_RX_LE_STR) == NULL))
		{
			if(!UpdateBtDutParameter(ActionName, BtDutParam, ExtDutParam, szMessage)) goto ERRORED;
		}else{
			if(!UpdateWifiDutParameter(ActionName, WifiDutParam, ExtDutParam, szMessage)) goto ERRORED;
		}
	}
	if(!ExtInParameter(szMessage)) goto ERRORED;
	if((eRtn = TM_ClearParameters(g_GlobalInfo.technologyID)) != TM_ERR_OK)
	{
		sprintf_s(szMessage, DLL_INFO_SIZE, "Set TM_ClearParameters failed, [Index:%d]", eRtn);
		goto ERRORED;
	}

	for(int i=0; i<m_IQFactInput.GetSize(); i++)
	{
		stParameter = m_IQFactInput.GetAt(i);
		if(bOutputMsg)
			CCommFunc::OutputMsgToBoth(m_cbListMessage, "Input parameter {>%s [%s]  = %s}", stParameter.name, IQFactInterfaceTable[stParameter.type], stParameter.value);
		if(!SetParameterToTestManager(stParameter, szMessage)) goto ERRORED;
	}
	
	if(TM_Run(g_GlobalInfo.technologyID, ActionName) != TM_ERR_OK)
	{
		eRtn = TM_GetStringReturn(g_GlobalInfo.technologyID, _T("ERROR_MESSAGE"), szMessage, DLL_INFO_SIZE);
		PostTestEvent(ActionName, szMessage);
		goto FAILED;
	}
	if(!ExtOutParameter(szMessage)) goto ERRORED;
	if(!PostTestEvent(ActionName, szMessage)) goto ERRORED;


	strCopy(PrevTestType, ActionName);
	if(bOutputMsg)
	{
		CCommFunc::OutputMsgToBoth(m_cbListMessage, "Test cmd query is successful, [%s]", szDetail);
		CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	}
	return TM_RETURN_PASS;

FAILED:	
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szDetail);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	if(strlen(szMessage)>= DLL_INFO_SIZE)	szMessage[DLL_INFO_SIZE-1] = 0;
	if(strcmp(m_TestParam.ErrorReport.ErrorMsg, "") == NULL)
		sprintf_s(m_TestParam.ErrorReport.ErrorMsg, sizeof(m_TestParam.ErrorReport.ErrorMsg), "%s", szMessage);
	else{
		sprintf_s(m_TestParam.ErrorReport.ErrorMsg, sizeof(m_TestParam.ErrorReport.ErrorMsg), "%s,[%s]", m_TestParam.ErrorReport.ErrorMsg, szMessage);
	}
	CCommFunc::FillErrorReport(m_pErrorReport, ENABLE_SFCS, m_TestParam.ErrorReport.ErrorCode, szDetail, m_TestParam.ErrorReport.ErrorMsg,  ActionName, TEST_FAIL);
	return TM_RETURN_FAIL;

ERRORED:
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szDetail);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	if(strlen(szMessage)>= DLL_INFO_SIZE)	szMessage[DLL_INFO_SIZE-1] = 0;
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage,  ActionName, TEST_FAIL);
	return TM_RETURN_ERROR;

NOT_FOUND:
	sprintf_s(szMessage, DLL_INFO_SIZE, "Can not find parameter,[Name: %s]", szCmd);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szDetail);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	if(strlen(szMessage)>= DLL_INFO_SIZE)	szMessage[DLL_INFO_SIZE-1] = 0;
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage,  ActionName, TEST_FAIL);
	return TM_RETURN_ERROR;
}
BOOL CCustomizeTest::LoadIQScriptFile(BOOL bOutputMsg, LPSTR lpszFileName, LPSTR lpszSectionName, LPSTR lpszRunInfo)
{
	CString strStrat;
	strStrat.Format("\t%s", lpszSectionName);
	return FileParser(bOutputMsg, lpszFileName, strStrat.GetBuffer(), _T("#Return Values"), lpszRunInfo);	
}

BOOL CCustomizeTest::FileParser(BOOL bOutputMsg, LPSTR lpszFileName, LPSTR lpszStart, LPSTR lpszStop, LPSTR lpszRunInfo)
{
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	FILE *stream;
	int FileNo;
	long streamLength, ActuallyRead;
	TCHAR *Content = NULL;
	CString strData="", strResult="";
	BOOL bFind=FALSE, bResult=FALSE;
	IQFACT_PARAMETER stParameter;
	errno_t err;

	if(err = fopen_s(&stream, lpszFileName,"rb") == NULL)
	{
		FileNo = _fileno(stream);
		streamLength = _filelength(FileNo);

		Content = (TCHAR*)malloc((size_t)(streamLength+1));
		memset(Content, 0x00, (streamLength+1));
		fseek(stream, SEEK_SET, 0);
		ActuallyRead = (long)fread(Content, sizeof(TCHAR), streamLength, stream);
		fclose(stream) ;
		strData.Format("%s", Content);
		if(Content)	free(Content);
		if(!FilterString(FALSE, FALSE, strData, lpszStart, lpszStop, strResult, szMessage))
		{
			sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Parse file data is not find, [Name:%s, Start:%s, Stop:%s]", lpszFileName, lpszStart, lpszStop);
			return FALSE;
		}
	}else{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Can not open file,[Name:%s]", lpszFileName);
		return FALSE;
	}
	do 
	{
		bResult = ParseIQInputParam(strResult, stParameter, bFind);
		if(bFind)	
			m_IQFactInput.Add(stParameter);
	} while (bResult);

	return TRUE;

}


BOOL CCustomizeTest::FilterString(BOOL bStartRev, BOOL bStopRev, CString strSource, LPSTR lpszStart, LPSTR lpszStop, CString &strResult, LPSTR lpszRunInfo)
{
	CString		strData, strStart, strStop, strFilter;
	TCHAR szFilterMethod[DLL_INFO_SIZE]={0};
	int	nStart=SEARCH_NOT_FOUND, nStop=SEARCH_NOT_FOUND;

	strData = strSource;
	strStart.Format("%s", lpszStart);
	strStop.Format("%s", lpszStop);
	if(strStart.GetLength()>0){
		if(bStartRev)
			nStart = CCommFunc::ReverseFindStr(strData, strStart.GetBuffer());
		else
			nStart = strData.Find(strStart);			
	}
	if(strStop.GetLength()>0)
	{
		if(nStart == SEARCH_NOT_FOUND){
			if(bStopRev)
				nStop = CCommFunc::ReverseFindStr(strData, strStop.GetBuffer());
			else
				nStop = strData.Find(strStop);
		}else{
			if(bStopRev)
				nStop = CCommFunc::ReverseFindStr(strData, strStop.GetBuffer(), nStart);
			else
				nStop = strData.Find(strStop, nStart+strStart.GetLength());
		}
	}
	if((nStart != SEARCH_NOT_FOUND) && (nStop != SEARCH_NOT_FOUND))
	{
		if(nStart == SEARCH_NOT_FOUND)
			strData = strData.Mid(0, nStop);
		else if(nStop == SEARCH_NOT_FOUND)
			strData = strData.Mid(nStart+strStart.GetLength());
		else{
			if(nStop < nStart){
				if(nStop == 0)
					strData = strData.Mid(nStop, nStart);
				else
					strData = strData.Mid(nStop+strStop.GetLength(), nStart - nStop - strStop.GetLength());
			}else
				strData = strData.Mid(nStart+strStart.GetLength(), nStop - nStart - strStart.GetLength());
		}
	}else
		return FALSE;
	
	strResult = strData;	

	return TRUE;
}

BOOL CCustomizeTest::ParseIQInputParam(CString &strRaw, IQFACT_PARAMETER &stParameter, BOOL &find)
{
	CString strTmp, strData;
	int addr=SEARCH_NOT_FOUND, addr2=SEARCH_NOT_FOUND, addrNext=SEARCH_NOT_FOUND;

	addrNext = addr = strRaw.Find("\r");
	if(addr == SEARCH_NOT_FOUND) goto NOT_FOUND;
	strTmp = strRaw.Left(addr-1);
	addr = strTmp.Find(">");
	if(addr == SEARCH_NOT_FOUND) goto NOT_FOUND;
	addr2 = strTmp.Find("[");
	if(addr2 == SEARCH_NOT_FOUND) goto NOT_FOUND;
	strData = strTmp.Mid(addr+1,addr2-addr-1);
	strData.Trim();
	strCopy(stParameter.name, strData);

	addr = strTmp.Find("]");
	strData = strTmp.Mid(addr2+1,addr-addr2-1);
	strData.Trim();
	if(strData.CompareNoCase(_T("Integer")) == 0)
		stParameter.type = IQ_INTERFACE_TYPE_INT;
	else if(strData.CompareNoCase(_T("Double")) == 0)
		stParameter.type = IQ_INTERFACE_TYPE_DOUBLE;
	else if(strData.CompareNoCase(_T("String")) == 0)
		stParameter.type = IQ_INTERFACE_TYPE_STRING;
	else
		goto NOT_FOUND;

	addr = strTmp.Find("=");
	strData = strTmp.Mid(addr+1);
	strData.Trim();
	strCopy(stParameter.value, strData);
	strRaw = strRaw.Mid(addrNext+1);
	find = TRUE;
	return TRUE;

NOT_FOUND:
	find = FALSE;
	if(addrNext == SEARCH_NOT_FOUND)
		return FALSE;
	else
		strRaw = strRaw.Mid(addrNext+1);
	return TRUE;
}
BOOL CCustomizeTest::SetParameterToTestManager(IQFACT_PARAMETER stParameter, LPSTR lpszRunInfo)
{
	IQFACT_INTERFACE_TYPE eType=stParameter.type;
	switch(eType)
	{
		case IQ_INTERFACE_TYPE_INT:
			return (TM_AddIntegerParameter(g_GlobalInfo.technologyID, stParameter.name, atoi(stParameter.value)) == TM_ERR_OK)? TRUE:FALSE;
		case IQ_INTERFACE_TYPE_DOUBLE:
			return (TM_AddDoubleParameter(g_GlobalInfo.technologyID, stParameter.name, atof(stParameter.value)) == TM_ERR_OK)? TRUE:FALSE;
		case IQ_INTERFACE_TYPE_STRING:
			return (TM_AddStringParameter(g_GlobalInfo.technologyID, stParameter.name, stParameter.value) == TM_ERR_OK)? TRUE:FALSE;
		default:
			sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Test type not support, [Type : %d]", eType);
			return FALSE;
	}

	return TRUE;
}

BOOL CCustomizeTest::WifiVerificationTest(LPSTR ActionName, DUT_PARAM DutParam, EXT_DUT_PARAM ExtDutParam, LPSTR lpszRunInfo)
{
	CString strTestName="";
	IQFACT_TYPE eType;
	BT_DUT_PARAM BtDutParam;

	if(!SyncWifi_GTK2IQFact(ActionName, strTestName, eType, lpszRunInfo)) return FALSE;
	if(RunIQTestManager(strTestName.GetBuffer(), DutParam, BtDutParam, ExtDutParam, CMODE_WIFI_VERIFY) != TM_RETURN_PASS)
	{
		sprintf_s(lpszRunInfo, ERRORMSG_STRING, "%s", m_pErrorReport->ErrorMsg);
		return FALSE;
	}
	return TRUE;
}
BOOL CCustomizeTest::BtVerificationTest(LPSTR ActionName, BT_DUT_PARAM DutParam, EXT_DUT_PARAM ExtDutParam, LPSTR lpszRunInfo)
{
	CString strTestName="";
	IQFACT_TYPE eType;
	DUT_PARAM WifiDutParam;

	if(!SyncBt_GTK2IQFact(ActionName, strTestName, DutParam, eType, lpszRunInfo)) return FALSE;
	if(RunIQTestManager(strTestName.GetBuffer(), WifiDutParam, DutParam, ExtDutParam, CMODE_WIFI_VERIFY) != TM_RETURN_PASS)
	{
		sprintf_s(lpszRunInfo, ERRORMSG_STRING, "%s", m_pErrorReport->ErrorMsg);
		return FALSE;
	}
	return TRUE;
}

BOOL CCustomizeTest::UpdateWifiDutParameter(LPSTR ActionName, DUT_PARAM DutParam, EXT_DUT_PARAM ExtDutParam, LPSTR lpszRunInfo)
{
	if(!SyncAntenna(_T("TX"), ActionName, DutParam, lpszRunInfo)) return FALSE;
	if(!SyncAntenna(_T("RX"), ActionName, DutParam, lpszRunInfo)) return FALSE;
	if(!SyncFrequency(ActionName, DutParam, lpszRunInfo)) return FALSE;
	if(!SyncPower(ActionName, DutParam, lpszRunInfo)) return FALSE;
	if(!SyncBandwidth(ActionName, DutParam, lpszRunInfo)) return FALSE;
	if(!SyncDataRate(ActionName, DutParam, lpszRunInfo)) return FALSE;
	if(!SyncDataPreamble(ActionName, DutParam, lpszRunInfo)) return FALSE;
	if(!SyncDataBand(ActionName, DutParam, lpszRunInfo)) return FALSE;
	if(!SyncPacketCount(ActionName, ExtDutParam.PACKET_LENS, lpszRunInfo)) return FALSE;
	if(!SyncWaveform(ActionName, ExtDutParam.WAVEFORM, lpszRunInfo)) return FALSE;
			
	return TRUE;
}
BOOL CCustomizeTest::UpdateBtDutParameter(LPSTR ActionName, BT_DUT_PARAM DutParam, EXT_DUT_PARAM ExtDutParam, LPSTR lpszRunInfo)
{
	if(!SyncFrequencyBT(ActionName, DutParam, lpszRunInfo)) return FALSE;
	if(!SyncPowerBT(ActionName, DutParam, lpszRunInfo)) return FALSE;
	if(!SyncPacketType(ActionName, DutParam, lpszRunInfo)) return FALSE;
	if(!SyncPacketCountBT(ActionName, DutParam, ExtDutParam.PACKET_LENS, lpszRunInfo)) return FALSE;
	if(!SyncWaveform(ActionName, ExtDutParam.WAVEFORM, lpszRunInfo)) return FALSE;

	return TRUE;
}
BOOL CCustomizeTest::SyncAntenna(LPSTR TxRx, LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	TCHAR szType[32]={0};
	if(strcmp(TxRx, _T("TX")) == NULL)
		strCopy(szType, DutParam.TX_CHAIN);
	else
		strCopy(szType, DutParam.RX_CHAIN);

	if(strstr(szType, _T("CHAIN_A")) != NULL)
		QuickUpdateAntenna(TxRx,1,0,0,0);
	else if(strstr(szType, _T("CHAIN_B")) != NULL)
		QuickUpdateAntenna(TxRx,0,1,0,0);
	else if(strstr(szType, _T("CHAIN_C")) != NULL)
		QuickUpdateAntenna(TxRx,0,0,1,0);
	else if(strstr(szType, _T("CHAIN_D")) != NULL)
		QuickUpdateAntenna(TxRx,0,0,0,1);
	else if(strstr(szType, _T("CHAIN_AB")) != NULL)
		QuickUpdateAntenna(TxRx,1,1,0,0);
	else if(strstr(szType, _T("CHAIN_AC")) != NULL)
		QuickUpdateAntenna(TxRx,1,0,1,0);
	else if(strstr(szType, _T("CHAIN_AD")) != NULL)
		QuickUpdateAntenna(TxRx,1,0,0,1);
	else if(strstr(szType, _T("CHAIN_BC")) != NULL)
		QuickUpdateAntenna(TxRx,0,1,1,0);
	else if(strstr(szType, _T("CHAIN_BD")) != NULL)
		QuickUpdateAntenna(TxRx,0,1,0,1);
	else if(strstr(szType, _T("CHAIN_CD")) != NULL)
		QuickUpdateAntenna(TxRx,0,0,1,1);
	else if(strstr(szType, _T("CHAIN_ABC")) != NULL)
		QuickUpdateAntenna(TxRx,1,1,1,0);
	else if(strstr(szType, _T("CHAIN_ABD")) != NULL)
		QuickUpdateAntenna(TxRx,1,1,0,1);
	else if(strstr(szType, _T("CHAIN_BCD")) != NULL)
		QuickUpdateAntenna(TxRx,0,1,1,1);
	else if(strstr(szType, _T("CHAIN_ABCD")) != NULL)
		QuickUpdateAntenna(TxRx,1,1,1,1);
	return TRUE;
}
BOOL CCustomizeTest::QuickUpdateAntenna(LPSTR TxRx, int a, int b, int c, int d)
{
	IQFACT_PARAMETER stParameter={0};
	CString strValue;

	strValue.Format("%d", a);
	OuputIQfactParameter((strcmp(TxRx, _T("TX")) == NULL)?_T("TX1"):_T("RX1"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_INT, stParameter);
	ReplaceIQfactInput(stParameter);		

	strValue.Format("%d", b);
	OuputIQfactParameter((strcmp(TxRx, _T("TX")) == NULL)?_T("TX2"):_T("RX2"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_INT, stParameter);
	ReplaceIQfactInput(stParameter);		

	strValue.Format("%d", c);
	OuputIQfactParameter((strcmp(TxRx, _T("TX")) == NULL)?_T("TX3"):_T("RX3"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_INT, stParameter);
	ReplaceIQfactInput(stParameter);		

	strValue.Format("%d", d);
	OuputIQfactParameter((strcmp(TxRx, _T("TX")) == NULL)?_T("TX4"):_T("RX4"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_INT, stParameter);
	ReplaceIQfactInput(stParameter);	

	return TRUE;
}
BOOL CCustomizeTest::SyncFrequency(LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	IQFACT_PARAMETER stParameter={0};
	double dbFreqMHz;
	CString strValue;

	dbFreqMHz = CCommFunc::WIFI_ChannelToFrequencyMHz(DutParam.CHANNEL);
	strValue.Format("%.0lf", dbFreqMHz);
	OuputIQfactParameter(_T("BSS_FREQ_MHZ_PRIMARY"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_INT, stParameter);
	ReplaceIQfactInput(stParameter);		
	OuputIQfactParameter(_T("CH_FREQ_MHZ"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_INT, stParameter);
	ReplaceIQfactInput(stParameter);		
	return TRUE;
}
BOOL CCustomizeTest::SyncPower(LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	IQFACT_PARAMETER stParameter={0};
	CString strValue;

	strValue.Format("%.3lf", DutParam.TX_POWER_TARGET);
	if(strcmp(ActionName, IQF_TYPE_RX_VERIFY_PER_STR) == NULL)
		OuputIQfactParameter(_T("RX_POWER_DBM"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_DOUBLE, stParameter);
	else
		OuputIQfactParameter(_T("TX_POWER_DBM"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_DOUBLE, stParameter);
	ReplaceIQfactInput(stParameter);		
	return TRUE;
}
BOOL CCustomizeTest::SyncBandwidth(LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	IQFACT_PARAMETER stParameter={0};
	CString strValue, strTmp;
	strTmp.Format("%s", DutParam.WIDE_BAND);
	strTmp.Replace("WB_", "");
	strTmp.Replace("MHZ", "");
	strValue.Format("BW-%s", strTmp);
	OuputIQfactParameter(_T("BSS_BANDWIDTH"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_STRING, stParameter);
	ReplaceIQfactInput(stParameter);		
	strValue.Format("CBW-%s", strTmp);
	OuputIQfactParameter(_T("CH_BANDWIDTH"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_STRING, stParameter);
	ReplaceIQfactInput(stParameter);		
	return TRUE;
}
BOOL CCustomizeTest::SyncDataRate(LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	IQFACT_PARAMETER stParameter={0};
	CString strValue, strTmp;

	strTmp.Format("%s", DutParam.RATE);
	if(strcmp(DutParam.RATE, _T("R_1M")) == NULL)
		strValue = "DSSS-1";
	else if(strcmp(DutParam.RATE, _T("R_2M")) == NULL)
		strValue = "DSSS-2";
	else if(strcmp(DutParam.RATE, _T("R_5_5M")) == NULL)
		strValue = "CCK-5_5";
	else if(strcmp(DutParam.RATE, _T("R_11M")) == NULL)
		strValue = "CCK-11";
	else if((strcmp(DutParam.RATE, _T("R_6M")) == NULL) || (strcmp(DutParam.RATE, _T("R_9M")) == NULL) ||
		    (strcmp(DutParam.RATE, _T("R_12M")) == NULL) || (strcmp(DutParam.RATE, _T("R_18M")) == NULL) || 
			(strcmp(DutParam.RATE, _T("R_24M")) == NULL) || (strcmp(DutParam.RATE, _T("R_36M")) == NULL) ||
			(strcmp(DutParam.RATE, _T("R_48M")) == NULL) || (strcmp(DutParam.RATE, _T("R_54M")) == NULL) ){
		strTmp.Replace("R_", "");
		strTmp.Replace("M", "");
		strValue.Format("OFDM-%s", strTmp);
	}else if(strstr(DutParam.RATE, _T("MCS")) != NULL){
		strValue = strTmp;
	}else{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Data rate is not support, [Rate:%s]", DutParam.RATE);
		return FALSE;
	}

	OuputIQfactParameter(_T("DATA_RATE"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_STRING, stParameter);
	ReplaceIQfactInput(stParameter);		
	return TRUE;
}
BOOL CCustomizeTest::SyncDataPreamble(LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	IQFACT_PARAMETER stParameter={0};
	CString strValue;

	if(strcmp(DutParam.BAND, _T("11b")) == NULL)
	{
		strValue = (strcmp(DutParam.PREAMBLE, "SHORT_PREAMBLE") == NULL)? _T("SHORT"): _T("LONG");
		OuputIQfactParameter(_T("PREAMBLE"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_STRING, stParameter);
		ReplaceIQfactInput(stParameter);		
	}else{
		strValue = (strcmp(DutParam.PREAMBLE, "SHORT_GI") == NULL)? _T("SHORT"): _T("LONG");
		OuputIQfactParameter(_T("GUARD_INTERVAL"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_STRING, stParameter);
		ReplaceIQfactInput(stParameter);
	}
	return TRUE;
}
BOOL CCustomizeTest::SyncDataBand(LPSTR ActionName, DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	IQFACT_PARAMETER stParameter={0};
	CString strValue;
	CString strPacketFormat;

	if(strcmp(DutParam.BAND, _T("11b")) == NULL){
		strValue = "802.11b";
		strPacketFormat = "NON_HT";
	}else if((strcmp(DutParam.BAND, _T("11a")) == NULL) || (strcmp(DutParam.RATE, _T("11g")) == NULL)){
		strValue = "802.11ag";
		strPacketFormat = "NON_HT";
	}else if(strcmp(DutParam.BAND, _T("11n")) == NULL){
		strValue = "802.11n";
		strPacketFormat = "HT_MF";
	}else if(strcmp(DutParam.BAND, _T("11ac")) == NULL){
		strValue = "802.11ac";
		strPacketFormat = "VHT";
	}else{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Band is not support, [Band:%s]", DutParam.BAND);
		return FALSE;
	}

	OuputIQfactParameter(_T("STANDARD"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_STRING, stParameter);
	ReplaceIQfactInput(stParameter);		
	OuputIQfactParameter(_T("PACKET_FORMAT"), strPacketFormat.GetBuffer(), IQ_INTERFACE_TYPE_STRING, stParameter);
	ReplaceIQfactInput(stParameter);		
	return TRUE;
}
BOOL CCustomizeTest::SyncPacketCount(LPSTR ActionName, int PacketCount, LPSTR lpszRunInfo)
{
	IQFACT_PARAMETER stParameter={0};
	CString strValue;

	if(PacketCount != 0)
	{
		strValue.Format("%d", PacketCount);
		OuputIQfactParameter(_T("FRAME_COUNT"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_INT, stParameter);
		ReplaceIQfactInput(stParameter);	
	}
	return TRUE;
}
BOOL CCustomizeTest::SyncWaveform(LPSTR ActionName, LPSTR lpszWaveform, LPSTR lpszRunInfo)
{
	IQFACT_PARAMETER stParameter={0};
	CString strValue;

	if(strcmp(lpszWaveform, "") != NULL)
	{
		strValue.Format("%s", lpszWaveform);
		OuputIQfactParameter(_T("WAVEFORM_NAME"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_STRING, stParameter);
		ReplaceIQfactInput(stParameter);	
	}
	return TRUE;
}
BOOL CCustomizeTest::SyncFrequencyBT(LPSTR ActionName, BT_DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	IQFACT_PARAMETER stParameter={0};
	double dbFreqMHz;
	CString strValue;

	dbFreqMHz = CCommFunc::BT_ChannelToFrequencyMHz(DutParam.CHANNEL);
	strValue.Format("%.0lf", dbFreqMHz);
	OuputIQfactParameter(_T("FREQ_MHZ"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_INT, stParameter);
	ReplaceIQfactInput(stParameter);		
	return TRUE;
}
BOOL CCustomizeTest::SyncPowerBT(LPSTR ActionName, BT_DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	IQFACT_PARAMETER stParameter={0};
	CString strValue;

	strValue.Format("%.3lf", DutParam.TX_POWER_TARGET);
	if(strcmp(ActionName, _T("BT_RX_BER_TEST_OEM")) == NULL) 
	{
		OuputIQfactParameter(_T("RX_POWER_LEVEL"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_DOUBLE, stParameter);
		ReplaceIQfactInput(stParameter);		
	}
	return TRUE;
}
BOOL CCustomizeTest::SyncPacketType(LPSTR ActionName, BT_DUT_PARAM DutParam, LPSTR lpszRunInfo)
{
	IQFACT_PARAMETER stParameter={0};
	CString strValue;
	if(strcmp(DutParam.PACKET_TYPE, _T("LE")) == NULL)
		strValue = _T("1LE");
	else if((strcmp(DutParam.PACKET_TYPE, _T("DH1")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("DH3")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("DH5")) == NULL))
		strValue.Format("1%s", DutParam.PACKET_TYPE);
	else
		strValue.Format("%s", DutParam.PACKET_TYPE);
	OuputIQfactParameter(_T("PACKET_TYPE"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_STRING, stParameter);
	ReplaceIQfactInput(stParameter);		
	return TRUE;
}
BOOL CCustomizeTest::SyncPacketCountBT(LPSTR ActionName, BT_DUT_PARAM DutParam, int PacketCount, LPSTR lpszRunInfo)
{
	IQFACT_PARAMETER stParameter={0};
	CString strValue;

	if(PacketCount != 0)
	{
		strValue.Format("%d", PacketCount);
		if((strcmp(ActionName, _T("BT_RX_BER_TEST_OEM")) == NULL) && (strcmp(DutParam.PACKET_TYPE, _T("LE")) == NULL))
			OuputIQfactParameter(_T("RX_PACKET_NUMBER"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_DOUBLE, stParameter);
		else
			OuputIQfactParameter(_T("PACKET_LENGTH"), strValue.GetBuffer(), IQ_INTERFACE_TYPE_INT, stParameter);
		ReplaceIQfactInput(stParameter);	
	}
	return TRUE;

}
BOOL CCustomizeTest::ReplaceIQfactInput(IQFACT_PARAMETER stNew)
{
	IQFACT_PARAMETER stParameter;
	for(int i=0; i<m_IQFactInput.GetSize(); i++)
	{
		stParameter = m_IQFactInput.GetAt(i);
		if(strcmp(stParameter.name, stNew.name) == NULL)
		{
			m_IQFactInput.SetAt(i, stNew);
			return TRUE;
		}
	}
	m_IQFactInput.Add(stNew);
	return TRUE;
}

BOOL CCustomizeTest::OuputIQfactParameter(LPSTR name, LPSTR value, IQFACT_INTERFACE_TYPE type, IQFACT_PARAMETER &stParameter)
{	
	strCopy(stParameter.name, name);
	strCopy(stParameter.value, value);
	stParameter.type = type;
	return TRUE;
}

BOOL CCustomizeTest::GetTestResult(LPSTR lpszItem, double *result, LPSTR lpszRunInfo)
{
	IQ_TM_RETURN eRtn;
	if((eRtn=TM_GetDoubleReturn(g_GlobalInfo.technologyID, lpszItem, result)) != TM_ERR_OK)
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Set TM_GetDoubleReturn failed, [Name:%s, Index:%d]", lpszItem, eRtn);
		return FALSE;
	}
	return TRUE;
}
BOOL CCustomizeTest::GetTestResult(LPSTR lpszItem, int *result, LPSTR lpszRunInfo)
{
	IQ_TM_RETURN eRtn;
	if((eRtn=TM_GetIntegerReturn(g_GlobalInfo.technologyID, lpszItem, result)) != TM_ERR_OK)
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Set TM_GetIntegerReturn failed, [Name:%s, Index:%d]", lpszItem, eRtn);
		return FALSE;
	}
	return TRUE;
}
BOOL CCustomizeTest::GetTestResult(LPSTR lpszItem, LPSTR result, int size, LPSTR lpszRunInfo)
{
	IQ_TM_RETURN eRtn;
	if((eRtn=TM_GetStringReturn(g_GlobalInfo.technologyID, lpszItem, result, size)) != TM_ERR_OK)
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Set TM_GetStringReturn failed, [Name:%s, Index:%d]", lpszItem, eRtn);
		return FALSE;
	}
	return TRUE;
}

BOOL CCustomizeTest::SyncWifi_GTK2IQFact(LPSTR ActionName, CString &strOutput, IQFACT_TYPE& eType, LPSTR lpszRunInfo)
{
	if(strcmp(ActionName, _T("WIFI_EVM_TEST_OEM")) == NULL){
		strOutput.Format("%s", IQF_TYPE_TX_VERIFY_EVM_STR);
		eType = IQF_TYPE_TX_VERIFY_EVM;
	}else if(strcmp(ActionName, _T("WIFI_TX_POWER_VERIFICATION_OEM")) == NULL){
		strOutput.Format("%s", IQF_TYPE_TX_VERIFY_POWER_STR);
		eType = IQF_TYPE_TX_VERIFY_POWER;
	}else if(strcmp(ActionName, _T("WIFI_SPECTRUM_MASK_TEST_OEM")) == NULL){
		strOutput.Format("%s", IQF_TYPE_TX_VERIFY_MASK_STR);
		eType = IQF_TYPE_TX_VERIFY_MASK;
	}else if(strcmp(ActionName, _T("WIFI_RX_PER_TEST_OEM")) == NULL){
		strOutput.Format("%s", IQF_TYPE_RX_VERIFY_PER_STR);
		eType = IQF_TYPE_RX_VERIFY_PER;
	}else if(strcmp(ActionName, IQF_TYPE_TX_VERIFY_SPECTRUM_STR) == NULL){
		strOutput.Format("%s", IQF_TYPE_TX_VERIFY_SPECTRUM_STR);
		eType = IQF_TYPE_TX_VERIFY_SPECTRUM;
	}else{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "SyncWifi_GTK2IQFact type is not define, [Name:%s]", ActionName);
		return FALSE;
	}
	return TRUE;
}
BOOL CCustomizeTest::SyncBt_GTK2IQFact(LPSTR ActionName, CString &strOutput, BT_DUT_PARAM DutParam, IQFACT_TYPE& eType, LPSTR lpszRunInfo)
{
	if(strcmp(ActionName, _T("BT_TX_TEST_OEM")) == NULL){
		if((strcmp(DutParam.PACKET_TYPE, _T("DH1")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("DH3")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("DH5")) == NULL) ||
		   (strcmp(DutParam.PACKET_TYPE, _T("1DH1")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("1DH3")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("1DH5")) == NULL)){
			strOutput.Format("%s", IQF_TYPE_TX_BDR_STR);
			eType = IQF_TYPE_TX_BDR;
		}else if((strcmp(DutParam.PACKET_TYPE, _T("2DH1")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("2DH3")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("2DH5")) == NULL)){
			strOutput.Format("%s", IQF_TYPE_TX_EDR_STR);
			eType = IQF_TYPE_TX_EDR;
		}else if((strcmp(DutParam.PACKET_TYPE, _T("3DH1")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("3DH3")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("3DH5")) == NULL)){
			strOutput.Format("%s", IQF_TYPE_TX_EDR_STR);
			eType = IQF_TYPE_TX_EDR;
		}else if((strcmp(DutParam.PACKET_TYPE, _T("LE")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("1LE")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("2LE")) == NULL)){
			strOutput.Format("%s", IQF_TYPE_TX_LE_STR);
			eType = IQF_TYPE_TX_LE;
		}else{
			sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "SyncBt_GTK2IQFact type is not define, [Name:%s]", ActionName);
			return FALSE;
		}
	}else if(strcmp(ActionName, _T("BT_RX_BER_TEST_OEM")) == NULL){
		if((strcmp(DutParam.PACKET_TYPE, _T("DH1")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("DH3")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("DH5")) == NULL) ||
			(strcmp(DutParam.PACKET_TYPE, _T("1DH1")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("1DH3")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("1DH5")) == NULL)){
			strOutput.Format("%s", IQF_TYPE_RX_BDR_STR);
			eType = IQF_TYPE_RX_BDR;
		}else if((strcmp(DutParam.PACKET_TYPE, _T("2DH1")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("2DH3")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("2DH5")) == NULL)){
			strOutput.Format("%s", IQF_TYPE_RX_EDR_STR);
			eType = IQF_TYPE_RX_EDR;
		}else if((strcmp(DutParam.PACKET_TYPE, _T("3DH1")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("3DH3")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("3DH5")) == NULL)){
			strOutput.Format("%s", IQF_TYPE_RX_EDR_STR);
			eType = IQF_TYPE_RX_EDR;
		}else if((strcmp(DutParam.PACKET_TYPE, _T("LE")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("1LE")) == NULL) || (strcmp(DutParam.PACKET_TYPE, _T("2LE")) == NULL)){
			strOutput.Format("%s", IQF_TYPE_RX_LE_STR);
			eType = IQF_TYPE_RX_LE;
		}else{
			sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "SyncBt_GTK2IQFact type is not define, [Name:%s]", ActionName);
			return FALSE;
		}
	}else{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "SyncBt_GTK2IQFact type is not define, [Name:%s]", ActionName);
		return FALSE;
	}
	return TRUE;
}
TM_RETURN CCustomizeTest::LoadRefenceScript(IQFACT_TYPE eType)
{
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szCmd[MAX_PATH]={0};
	TCHAR szData[DLL_INFO_SIZE]={0};
	TCHAR szDetail[ERRORDETAIL_STRING]={0};

	strCopy(szCmd, _T("FILE"));
	if(!GetTestParameter(szCmd, szData, sizeof(szData))) goto NOT_FOUND;

	strCopy(m_ReferenceScript, szData);
	g_GlobalInfo.technologyID = SyncTechnologyID(m_ReferenceScript);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "Load IQfact refence script is successful, [Name : %s]", m_ReferenceScript);
	return TM_RETURN_PASS;

NOT_FOUND:
	sprintf_s(szMessage, sizeof(szMessage), "Load IQfact refence script is fail, [Name : %s]", szData);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage,  IQFactTypeTable[eType], TEST_FAIL);
	return TM_RETURN_ERROR;
}

TM_RETURN CCustomizeTest::UploadVariable()
{
	TCHAR szCmd[MAX_PATH]={0};
	TCHAR szData[DLL_INFO_SIZE]={0};	
	TCHAR variable[DLL_INFO_SIZE]={0};
	TCHAR szMessage[DLL_INFO_SIZE]={0};

	sprintf_s(m_TestParam.ErrorReport.ErrorDetail, sizeof(m_TestParam.ErrorReport.ErrorDetail), "MODE :[%s]", MODE_UPLOAD_STR);
	if(!GetTestParameter(ASSIGN_VARIABLE, variable, sizeof(variable))) goto NOT_FOUND;

	strCopy(szCmd, _T("DATA"));
	if(!GetTestParameter(szCmd, szData, sizeof(szData)))
		goto NOT_FOUND;

	sprintf_s(szMessage, "VARIABLE:[%s], DATA:[%s]", variable, szData);

	if(!g_GlobalInfo.fAssignVariable(TRUE, variable, szData, 0))
		goto ERRORED;

	sprintf_s(szMessage, sizeof(szMessage), "Upload variable is successful, Name:[%s], Data:[%s]", variable, szData);

	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", m_TestParam.ErrorReport.ErrorDetail);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	return TM_RETURN_PASS;

ERRORED:
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", m_TestParam.ErrorReport.ErrorDetail);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, m_TestParam.ErrorReport.ErrorDetail, szMessage, MODE_UPLOAD_STR, TEST_FAIL);
	return TM_RETURN_ERROR;

NOT_FOUND:
	sprintf_s(szMessage, DLL_INFO_SIZE, "Can not find parameter,[Name: %s]", szCmd);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", m_TestParam.ErrorReport.ErrorDetail);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, m_TestParam.ErrorReport.ErrorDetail, szMessage, MODE_UPLOAD_STR, TEST_FAIL);
	return TM_RETURN_ERROR;
}

BOOL CCustomizeTest::ExtInParameter(LPSTR lpszRunInfo)
{
	SET_PARAMETER param;
	IQFACT_PARAMETER stParameter={0};
	CString strData;
	TCHAR szMessage[DLL_INFO_SIZE]={0};

	for(int idx=0; idx<m_ParamArray.GetSize(); idx++)
	{
		param = m_ParamArray.GetAt(idx);
		if(strcmp(param.name, _T("SET_PARAMETER")) == NULL){
			if(ReadActionParameter(param.value, stParameter))
				ReplaceIQfactInput(stParameter);
			else{
				sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Set external input parameter is failed, [Name:%s, Value:%s]", param.name, param.value);
				return FALSE;
			}
		}else if(strcmp(param.name, _T("LOAD_PARAMETER")) == NULL){
			if(ReadTestInfoParameter(param.value, stParameter))
				ReplaceIQfactInput(stParameter);
			else{
				sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Load external input parameter is failed, [Name:%s, Value:%s]", param.name, param.value);
				return FALSE;
			}
		}else if(strcmp(param.name, _T("PRE_TEST_EVENT")) == NULL){
			if(!PreTestEvent(param.value, szMessage))
			{
				sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Pre-test event is failed, [Name:%s, Message:%s]", param.name, param.value, szMessage);
				return FALSE;
			}
		}
	}
	return TRUE;
}
BOOL CCustomizeTest::ExtOutParameter(LPSTR lpszRunInfo)
{
	SET_PARAMETER param;
	IQFACT_PARAMETER stParameter={0};
	CString strData;
	TCHAR szMessage[DLL_INFO_SIZE]={0};

	for(int idx=0; idx<m_ParamArray.GetSize(); idx++)
	{
		param = m_ParamArray.GetAt(idx);
		if(strcmp(param.name, _T("VARIABLE")) == NULL){
			if(ReadActionParameter(param.value, stParameter))
				UploadIQfactOutput(stParameter);
			else{
				sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Get external input parameter is failed, [Name:%s, Value:%s]", param.name, param.value);
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CCustomizeTest::ReadTestInfoParameter(LPSTR input, IQFACT_PARAMETER &output)
{
	TCHAR szFileName[MAX_PATH]={0};
	TCHAR szName[DLL_INFO_SIZE]={0};
	TCHAR szValue[DLL_INFO_SIZE]={0};
	BOOL bRtn=TRUE;

	if(strcmp(g_GlobalInfo.PN_FOLDER_PATH, "") == NULL)
		sprintf_s(szFileName, sizeof(szFileName), "%s%s%s.ini", g_GlobalInfo.szLocalPath, _T("Customer\\"), g_GlobalInfo.PN98);
	else
		sprintf_s(szFileName, sizeof(szFileName), "%s%s%s\\%s.ini", g_GlobalInfo.szLocalPath, _T("Customer\\"), g_GlobalInfo.PN_FOLDER_PATH, g_GlobalInfo.PN98);

	sprintf_s(szName, sizeof(szName), "%s@Integer", input);
	if(GetPrivateProfileString("TEST_INFO", szName, _T(""), szValue, DLL_INFO_SIZE, szFileName) > 0)
		output.type = IQ_INTERFACE_TYPE_INT;
	else{
		sprintf_s(szName, sizeof(szName), "%s@Double", input);
		if(GetPrivateProfileString("TEST_INFO", szName, _T(""), szValue, DLL_INFO_SIZE, szFileName) > 0)
			output.type = IQ_INTERFACE_TYPE_DOUBLE;
		else{
			sprintf_s(szName, sizeof(szName), "%s@String", input);
			if(GetPrivateProfileString("TEST_INFO", szName, _T(""), szValue, DLL_INFO_SIZE, szFileName) > 0)
				output.type = IQ_INTERFACE_TYPE_STRING;
			else
				bRtn = FALSE;
		}
	}
	if(bRtn)
	{
		strCopy(output.name, input);
		strCopy(output.value, szValue);
	}
	return bRtn;
}
BOOL CCustomizeTest::ReadActionParameter(LPSTR input, IQFACT_PARAMETER &output)
{
	CString strData;
	int addr;

	strData.Format("%s", input);
	addr = strData.Find("@");
	if(addr == SEARCH_NOT_FOUND) return FALSE;
	strCopy(output.name, strData.Left(addr));
	strData = strData.Mid(addr+1);
	addr = strData.Find("@");
	if(addr == SEARCH_NOT_FOUND) return FALSE;
	strCopy(output.value, strData.Left(addr));
	strData = strData.Mid(addr+1);
	if(strData.Compare("Integer") == NULL)
		output.type = IQ_INTERFACE_TYPE_INT;
	else if(strData.Compare("Double") == NULL)
		output.type = IQ_INTERFACE_TYPE_DOUBLE;
	else if(strData.Compare("String") == NULL)
		output.type = IQ_INTERFACE_TYPE_STRING;
	else
		return FALSE;

	return TRUE;
}
BOOL CCustomizeTest::UploadIQfactOutput(IQFACT_PARAMETER stParameter)
{
	int iValue;
	double dbValue;
	TCHAR szValue[DLL_INFO_SIZE]={0};
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	BOOL bRtn=FALSE;

	if(stParameter.type == IQ_INTERFACE_TYPE_INT){
		bRtn = GetTestResult(stParameter.name, &iValue, szMessage);
		sprintf_s(szValue, sizeof(szValue), "%d", iValue);
	}else if(stParameter.type == IQ_INTERFACE_TYPE_DOUBLE){
		bRtn = GetTestResult(stParameter.name, &dbValue, szMessage);
		sprintf_s(szValue, sizeof(szValue), "%.6lf", dbValue);
	}else if(stParameter.type == IQ_INTERFACE_TYPE_STRING){
		bRtn = GetTestResult(stParameter.name, szValue, DLL_INFO_SIZE, szMessage);
	}
	g_GlobalInfo.fAssignVariable(TRUE, stParameter.value, szValue, 0);
	return bRtn;
}
TM_RETURN CCustomizeTest::SyncCompareType()
{
	COMPARE_TYPE eType=COMPARE_TYPE_NULL;
	TCHAR szDetail[ERRORDETAIL_STRING]={0};
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szCmd[MAX_PATH]={0};
	TCHAR szData[DLL_INFO_SIZE]={0};

	strCopy(szCmd, _T("TYPE"));
	if(!GetTestParameter(szCmd, szData, sizeof(szData))) goto NOT_FOUND;

	for(int idx=0 ; idx<COMPARE_TYPE_NULL; idx++)
	{		
		if(strcmp(szData,CompareTypeTable[idx]) == NULL)
		{
			eType = (COMPARE_TYPE)idx;
			break;
		}
	}

	switch(eType){
	//case COMPARE_TYPE_STRING:
	//	return CompareStringFunc(eMode, eType, lpszResult, size);
	case COMPARE_TYPE_VALUE:
		return CompareValueFunc();
	//case COMPARE_TYPE_TIME:
	//	return CompareTimeFunc(eMode, eType, lpszResult, size);
	//case COMPARE_TYPE_LENGTH:
	//	return CompareLengthFunc(eMode, eType, lpszResult, size);
	//case COMPARE_TYPE_ASCII:
	//	return CompareAsciiFunc(eMode, eType, lpszResult, size);
	default:
		sprintf_s(szMessage, sizeof(szMessage), "Test type not support, [Type : %s]", szData);
		goto ERRORED;
	}

ERRORED:
	sprintf_s(szDetail, sizeof(szDetail), "MODE :[%s]", MODE_COMPARE_STR);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage, MODE_COMPARE_STR, TEST_FAIL);
	return TM_RETURN_ERROR;

NOT_FOUND:
	sprintf_s(szDetail, sizeof(szDetail), "MODE :[%s]", MODE_COMPARE_STR);
	sprintf_s(szMessage, DLL_INFO_SIZE, "Can not find parameter,[Name: %s]", szCmd);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage, MODE_COMPARE_STR, TEST_FAIL);
	return TM_RETURN_ERROR;
}

TM_RETURN CCustomizeTest::CompareValueFunc()
{
	TCHAR szCmd[MAX_PATH]={0};
	TCHAR szData[DATA_MAX_BUFFER_SIZE]={0};	
	BOOL bRtn=FALSE;
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szTmp[DLL_INFO_SIZE]={0};
	double dbHigh=0, dbLow=0, dbActual;

	sprintf_s(m_TestParam.ErrorReport.ErrorDetail, sizeof(m_TestParam.ErrorReport.ErrorDetail), "Mode :[%s], Type:[%s]", MODE_COMPARE_STR, COMPARE_TYPE_VALUE_STR);

	strCopy(szCmd, _T("ERRORCODE"));
	if(!GetTestParameter(szCmd, m_TestParam.ErrorReport.ErrorCode, sizeof(m_TestParam.ErrorReport.ErrorCode))) goto NOT_FOUND;
	strCopy(szCmd, _T("ERRORMSG"));
	GetTestParameter(szCmd, m_TestParam.ErrorReport.ErrorMsg, sizeof(m_TestParam.ErrorReport.ErrorMsg));

	strCopy(szCmd, _T("ACTUAL"));
	if(!GetTestParameter(szCmd, szData, sizeof(szData))) goto NOT_FOUND;
	sprintf_s(szTmp, sizeof(szTmp), ", ACTUAL :[%s]", szData);
	strCat(m_TestParam.ErrorReport.ErrorDetail, szTmp);
	dbActual = atof(szData);

	strCopy(szCmd, _T("HIGH"));
	if(GetTestParameter(szCmd, szData, sizeof(szData)))
	{
		sprintf_s(szTmp, sizeof(szTmp), ", HIGH :[%s]", szData);
		strCat(m_TestParam.ErrorReport.ErrorDetail, szTmp);
		dbHigh =  atof(szData);
	}else{
		dbHigh = DATA_VAL_INIT;
	}

	strCopy(szCmd, _T("LOW"));
	if(GetTestParameter(szCmd, szData, sizeof(szData)))
	{
		sprintf_s(szTmp, sizeof(szTmp), ", LOW :[%s]", szData);
		strCat(m_TestParam.ErrorReport.ErrorDetail, szTmp);
		dbLow =  atof(szData);
	}else{
		dbLow = DATA_VAL_INIT;
	}

	if (CCommFunc::ValueCompare(dbHigh, dbLow, dbActual) != PASS_SPEC)
	{		
		sprintf_s(szMessage, "Compare is fail, %s", m_TestParam.ErrorReport.ErrorDetail);
		strCopy(m_TestParam.ErrorReport.ErrorDetail, szMessage);
		goto FAILED;
	}else{
		sprintf_s(szMessage, "Compare is successful, %s", m_TestParam.ErrorReport.ErrorDetail);
		CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	}
	return TM_RETURN_PASS;

FAILED:
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", m_TestParam.ErrorReport.ErrorDetail);
	CCommFunc::FillErrorReport(m_pErrorReport, ENABLE_SFCS, m_TestParam.ErrorReport.ErrorCode, m_TestParam.ErrorReport.ErrorDetail, (strcmp(m_TestParam.ErrorReport.ErrorMsg, "") == NULL)? szMessage: m_TestParam.ErrorReport.ErrorMsg, MODE_COMPARE_STR, TEST_FAIL);
	return TM_RETURN_FAIL;

NOT_FOUND:
	sprintf_s(szMessage, DLL_INFO_SIZE, "Can not find parameter,[Name: %s]", szCmd);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", m_TestParam.ErrorReport.ErrorDetail);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, m_TestParam.ErrorReport.ErrorDetail, szMessage, MODE_COMPARE_STR, TEST_FAIL);
	return TM_RETURN_ERROR;
}


TM_RETURN CCustomizeTest::SfcsCollection()
{
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szCmd[MAX_PATH]={0};
	TCHAR szData[DLL_INFO_SIZE]={0};
	BOOL bRtn=FALSE;
	TM_RETURN TmRtn=TM_RETURN_PASS;
	TCHAR szDetail[ERRORDETAIL_STRING]={0};
	TCHAR szTmp[DLL_INFO_SIZE]={0};
	TCHAR szType[32]={0};

	strCopy(szCmd, _T("ERRORCODE"));
	GetTestParameter(szCmd, m_TestParam.ErrorReport.ErrorCode, sizeof(m_TestParam.ErrorReport.ErrorCode));
	strCopy(szCmd, _T("ERRORMSG"));
	GetTestParameter(szCmd, m_TestParam.ErrorReport.ErrorMsg, sizeof(m_TestParam.ErrorReport.ErrorMsg));


	strCopy(szCmd, _T("SEND"));
	if(!GetTestParameter(szCmd, szData, sizeof(szData))) goto NOT_FOUND;
	sprintf_s(szTmp, sizeof(szTmp), ", Send :[%s]", szData);
	strCat(szDetail, szTmp);

	strCopy(szCmd, _T("TYPE"));
	if(!GetTestParameter(szCmd, szType, sizeof(szType))) goto NOT_FOUND;
	if(strcmp(szType, _T("WIFI_TX_CAL")) == NULL)
	{

	}else{
		if(strcmp(szType, _T("SPEC")) == NULL)
			m_cbSfcsSpecMessage(szData);
		else
			m_cbSfcsResultMessage(szData);
	}

	CCommFunc::OutputMsgToBoth(m_cbListMessage, "Collection SFCS data is successful, [%s]", szDetail);
	return TM_RETURN_PASS;

NOT_FOUND:
	sprintf_s(szMessage, DLL_INFO_SIZE, "Can not find parameter,[Name: %s]", szCmd);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szDetail);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage, MODE_SFCS_STR, TEST_FAIL);
	return TM_RETURN_ERROR;
}
BOOL CCustomizeTest::PreTestEvent(LPSTR name, LPSTR lpszRunInfo)
{
	return TRUE;
}

BOOL CCustomizeTest::PostTestEvent(LPSTR ActionName,  LPSTR lpszRunInfo)
{
	if(strcmp(ActionName, IQF_TYPE_TX_CALIBRATION_STR) == NULL)
	{
		QCA_WifiTxCalData(lpszRunInfo);		
	}
	return TRUE;
}

BOOL CCustomizeTest::QCA_WifiTxCalData(LPSTR lpszRunInfo)
{
	CStdioFile cFile;
	CString strLine;
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szFileName[MAX_PATH]={0};
	double dbExpPwr, dbActPwr;
	int    Gain, Freq, txChainMask;
	CString strResult;
	OUTPUT_TO_SFCS_DATA SfcsData;
	int i;

	sprintf_s(szFileName, sizeof(szFileName), "%s%s", g_GlobalInfo.szLocalPath, _T("Log\\TXCalDataLogFile.txt"));
	if(!cFile.Open(szFileName, CFile::modeRead | CFile::typeBinary)) return FALSE;
	m_SfcsArray.RemoveAll();
	while(cFile.ReadString(strLine))
	{
		if(strLine.GetLength()>DLL_INFO_SIZE-1)
			strLine = strLine.Left(DLL_INFO_SIZE-1);
		//WiFiTxPwrCal-F2412-A1-G5-P_-2.219222
        //F:Frequency / A:Antenna / G:Gain / P:Power
		//Trigger level:-25, Tx_Power_Goal:1.500000, Measured_Avg_Power:-5.421747, Gain:5, Freq:5180 ,txChainMask:1 
		if(!ParseRawData(strLine, dbExpPwr, _T("Tx_Power_Goal:"), _T(","))) continue;
		if(!ParseRawData(strLine, dbActPwr, _T("Measured_Avg_Power:"), _T(","))) continue;
		if(!ParseRawData(strLine, Gain, _T("Gain:"), _T(","))) continue;
		if(!ParseRawData(strLine, Freq, _T("Freq:"), _T(","))) continue;
		if(!ParseRawData(strLine, txChainMask, _T("txChainMask:"), "")) continue;		
		sprintf_s(SfcsData.header, sizeof(SfcsData.header), "WiFiTxPwrCal-F%d-A%d-G%d-P%.3lf", Freq, txChainMask, Gain, dbExpPwr);
		sprintf_s(SfcsData.value, sizeof(SfcsData.value), "%.3lf", dbActPwr);
		m_SfcsArray.Add(SfcsData);
	}
	cFile.Close();
	RemoveSfcsDuplicate();
	CCommFunc::ExportTestMessage(m_cbXmlMessage, "<TestCase name=\"%s\">",_T("WIFI_TX_POWER_CALIBRATION"));
	for(int i=0; i<m_SfcsArray.GetSize(); i++)
	{
		CCommFunc::ExportTestMessage(m_cbXmlMessage, "<Data>");
		SfcsData = m_SfcsArray.GetAt(i);
		strLine.Format("%s_%s",  SfcsData.header, SfcsData.value);
		m_cbSfcsResultMessage(strLine.GetBuffer());

		if(!ParseRawData(strLine, Freq, _T("-F"), _T("-A"))) continue;
		CCommFunc::ExportTestMessage(m_cbXmlMessage, "<Freq>%d</Freq>", Freq);
		if(!ParseRawData(strLine, txChainMask, _T("-A"), _T("-G"))) continue;
		CCommFunc::ExportTestMessage(m_cbXmlMessage, "<txChainMask>%d</txChainMask>", txChainMask);
		if(!ParseRawData(strLine, Gain, _T("-G"), _T("-P"))) continue;
		CCommFunc::ExportTestMessage(m_cbXmlMessage, "<Gain>%d</Gain>", Gain);
		if(!ParseRawData(strLine, dbExpPwr, _T("-P"), _T("_"))) continue;
		CCommFunc::ExportTestMessage(m_cbXmlMessage, "<Tx_Power_Goal>%.3lf</Tx_Power_Goal>", dbExpPwr);
		if(!ParseRawData(strLine, dbActPwr, _T("_"), _T(""))) continue;
		CCommFunc::ExportTestMessage(m_cbXmlMessage, "<Measured_Avg_Power>%.3lf</Measured_Avg_Power>", dbActPwr);
		CCommFunc::ExportTestMessage(m_cbXmlMessage, "</Data>");
	}
	CCommFunc::ExportTestMessage(m_cbXmlMessage, "</TestCase>");
	return TRUE;
}
BOOL CCustomizeTest::ParseRawData(CString src, double &dest, CString start, CString stop)
{
	int s=SEARCH_NOT_FOUND, t=SEARCH_NOT_FOUND;
	s = src.Find(start);
	if(s==SEARCH_NOT_FOUND) return FALSE;
	s += start.GetLength();
	if(stop == "")
		dest = atof(src.Mid(s));
	else{
		t = src.Find(stop, s+1);	
		dest = atof(src.Mid(s, t-s));
	}
	return TRUE;
}
BOOL CCustomizeTest::ParseRawData(CString src, int &dest, CString start, CString stop)
{
	int s=SEARCH_NOT_FOUND, t=SEARCH_NOT_FOUND;
	s = src.Find(start);
	if(s==SEARCH_NOT_FOUND) return FALSE;
	s += start.GetLength();
	if(stop == "")
		dest = atof(src.Mid(s));
	else{
		t = src.Find(stop, s+1);	
		dest = atoi(src.Mid(s, t-s));
	}
	return TRUE;
}
void CCustomizeTest::FileDataToTestLog(OUTPUT_LOG_TYPE eType, LPSTR lpszFileName)
{
	FILE *stream;
	int FileNo;
	long streamLength, ActuallyRead;
	TCHAR *Content = NULL;
	CStdioFile cFile;
	CString strLine;
	TCHAR szMessage[DLL_INFO_SIZE]={0};


	if((eType == OUTPUT_LOG_ONLY) || (eType == OUTPUT_MSG_BOTH))
	{
		if((stream = fopen(lpszFileName, _T("rb"))) != NULL)
		{
			FileNo = _fileno(stream);
			streamLength = _filelength(FileNo);

			Content = (TCHAR*)malloc((size_t)(streamLength+1));
			memset(Content, 0x00, (streamLength+1));
			fseek(stream, SEEK_SET, 0);
			ActuallyRead = (long)fread(Content, sizeof(TCHAR), streamLength, stream);
			fclose(stream) ;
			CCommFunc::SeparateTestLog(Content);
		}
		if(Content)	free(Content);
	}
	if((eType == OUTPUT_LIST_ONLY) || (eType == OUTPUT_MSG_BOTH))
	{
		cFile.Open(lpszFileName, CFile::modeRead | CFile::typeBinary);
		while(cFile.ReadString(strLine))
		{
			if(strLine.GetLength()>DLL_INFO_SIZE-1)
				strLine = strLine.Left(DLL_INFO_SIZE-1);
			strCopy(szMessage, strLine);
			m_cbListMessage(szMessage);
		}
		cFile.Close();
	}



	return;
}
BOOL CCustomizeTest::RemoveSfcsDuplicate()
{
	OUTPUT_TO_SFCS_DATA ref, data;
	int init=0;
FindAgain:
	for(int j=init; j<m_SfcsArray.GetSize()-1; j++)
	{
		ref = m_SfcsArray.GetAt(j);
		for(int i=j+1; i<m_SfcsArray.GetSize(); i++)
		{
			data = m_SfcsArray.GetAt(i);
			if(strcmp(ref.header, data.header) == NULL)
			{
				m_SfcsArray.RemoveAt(j);
				goto FindAgain;
			}
		}
		init++;
	}
	return TRUE;
}
TM_RETURN CCustomizeTest::SyncFileType()
{
	FILE_TYPE eType=FILE_TYPE_NULL;
	TCHAR szDetail[ERRORDETAIL_STRING]={0};
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szData[DLL_INFO_SIZE]={0};
	TCHAR szCmd[DLL_INFO_SIZE]={0};

	strCopy(szCmd, _T("TYPE"));
	if(!GetTestParameter(szCmd, szData, sizeof(szData))) goto NOT_FOUND;

	for(int idx=0 ; idx<FILE_TYPE_NULL; idx++)
	{		
		if(strcmp(szData,FileTypeTable[idx]) == NULL)
		{
			eType = (FILE_TYPE)idx;
			break;
		}
	}

	if(eType == FILE_TYPE_NULL)
	{
		sprintf_s(szMessage, sizeof(szMessage), "Test type not support, [Type : %s]", szData);
		goto ERRORED;
	}

	switch(eType){
	//case FILE_TYPE_CREATE:
	//	return FileCreate(eMode, eType);
	//case FILE_TYPE_PARSER:
	//	return FileParser(eMode, eType);
	case FILE_TYPE_DELETE:
		return FileDelete();
	case FILE_TYPE_OUTPUT:
		return FileOutput();
	default:
		sprintf_s(szMessage, "Test type not support, [Type : %s]", FileTypeTable[eType]);
		goto ERRORED;
	}

ERRORED:
	sprintf_s(szDetail, sizeof(szDetail), "MODE :[%s]", MODE_FILE_STR);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage, MODE_FILE_STR, TEST_FAIL);
	return TM_RETURN_ERROR;

NOT_FOUND:
	sprintf_s(szDetail, sizeof(szDetail), "MODE :[%s]", MODE_FILE_STR);
	sprintf_s(szMessage, DLL_INFO_SIZE, "Can not find parameter,[Name: %s]", szCmd);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, szDetail, szMessage, MODE_FILE_STR, TEST_FAIL);
	return TM_RETURN_ERROR;
}
TM_RETURN CCustomizeTest::FileDelete()
{
	TCHAR szCmd[MAX_PATH]={0};
	TCHAR szName[DLL_INFO_SIZE]={0};	
	TCHAR szMessage[DATA_MAX_BUFFER_SIZE]={0};
	TCHAR szData[DATA_MAX_BUFFER_SIZE]={0};
	OUTPUT_LOG_TYPE eMethod;

	strCopy(szCmd, _T("NAME"));
	if(!GetTestParameter(szCmd, szName, sizeof(szName))) goto NOT_FOUND;
	CCommFunc::SubDeleteFile("%s", szName);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "Delete file is successfully, [Name : %s]", szName);

	return TM_RETURN_PASS;

NOT_FOUND:
	sprintf_s(szMessage, DLL_INFO_SIZE, "Can not find parameter,[Name: %s]", szCmd);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, m_TestParam.ErrorReport.ErrorDetail, szMessage, MODE_FILE_STR, TEST_FAIL);
	return TM_RETURN_ERROR;
}

TM_RETURN CCustomizeTest::FileOutput()
{
	TCHAR szCmd[MAX_PATH]={0};
	TCHAR szName[DLL_INFO_SIZE]={0};	
	TCHAR szMessage[DATA_MAX_BUFFER_SIZE]={0};
	TCHAR szData[DATA_MAX_BUFFER_SIZE]={0};
	OUTPUT_LOG_TYPE eMethod;

	strCopy(szCmd, _T("NAME"));
	if(!GetTestParameter(szCmd, szName, sizeof(szName))) goto NOT_FOUND;

	strCopy(szCmd, _T("METHOD"));
	GetTestParameter(szCmd, szData, sizeof(szData));
	if(strcmp(szData, _T("LOG")) == NULL)
		eMethod = OUTPUT_LOG_ONLY;
	else if(strcmp(szData, _T("SCREEN")) == NULL)
		eMethod = OUTPUT_LIST_ONLY;
	else
		eMethod = OUTPUT_MSG_BOTH;

	CCommFunc::OutputMsgToBoth(m_cbListMessage, "================ Output File : %s ======================", szName);
	FileDataToTestLog(eMethod, szName);

	return TM_RETURN_PASS;

NOT_FOUND:
	sprintf_s(szMessage, DLL_INFO_SIZE, "Can not find parameter,[Name: %s]", szCmd);
	CCommFunc::OutputMsgToBoth(m_cbListMessage, "%s", szMessage);
	CCommFunc::FillErrorReport(m_pErrorReport, DISABLE_SFCS, ERR_ENVIRONMENT, m_TestParam.ErrorReport.ErrorDetail, szMessage, MODE_FILE_STR, TEST_FAIL);
	return TM_RETURN_ERROR;
}


TM_ID CCustomizeTest::SyncTechnologyID(LPSTR lpszFileName)
{
	USHORT check = 0;

	FILE *stream;
	int FileNo;
	long streamLength, ActuallyRead;
	TCHAR *Content = NULL;
	TCHAR szMessage[MAX_PATH]={0};
	TM_ID id;

	if((stream = fopen(lpszFileName, _T("rb"))) != NULL)
	{
		FileNo = _fileno(stream);
		streamLength = _filelength(FileNo);

		Content = (TCHAR*)malloc((size_t)(streamLength+1));
		memset(Content, 0x00, (streamLength+1));
		fseek(stream, SEEK_SET, 0);
		ActuallyRead = (long)fread(Content, sizeof(TCHAR), streamLength, stream);
		fclose(stream) ;
	}
	if(strstr(Content,_T("BT:")))
		id = g_GlobalInfo.ID_BT;
	else
		id = g_GlobalInfo.ID_WIFI_11AC;

	if(Content)	free(Content);
	return id;
}

BOOL CCustomizeTest::GetBtTestResult(BT_DUT_PARAM BtDutParam, BT_TX_RESULT *result, LPSTR lpszRunInfo)
{
	double dbResult;
	int    iResult;

	GetTestResult(_T("POWER_AVERAGE_DBM"), &dbResult, lpszRunInfo);
	result->P_AV_EACH_BURST = dbResult;
	GetTestResult(_T("POWER_PEAK_DBM"), &dbResult, lpszRunInfo);
	result->P_PK_EACH_BURST = dbResult;
	GetTestResult(_T("DATE_RATE_DETECT"), &dbResult, lpszRunInfo);
	result->DATE_RATE_DETECT = dbResult;
	if(strstr(BtDutParam.PACKET_TYPE, _T("LE")))
	{
		GetTestResult(_T("INITIAL_FREQ_OFFSET"), &dbResult, lpszRunInfo);
		result->LE_FREQ_OFFSET = dbResult;
		GetTestResult(_T("DELTA_F1_AVERAGE"), &dbResult, lpszRunInfo);
		result->LE_DELTA_F1_AVG = dbResult;
		GetTestResult(_T("DELTA_F2_AVERAGE"), &dbResult, lpszRunInfo);
		result->LE_DELTA_F2_AVG = dbResult;
		GetTestResult(_T("DELTA_F2_MAX"), &dbResult, lpszRunInfo);
		result->LE_DELTA_F2_MAX = dbResult;
		GetTestResult(_T("FREQ_DEV_SYNC_AVG"), &dbResult, lpszRunInfo);
		result->LE_FREQ_DEV_SYNC_AV = dbResult;
		GetTestResult(_T("Fn_MAX"), &dbResult, lpszRunInfo);
		result->LE_FN_MAX = dbResult;
		GetTestResult(_T("DELTA_F0_Fn_MAX"), &dbResult, lpszRunInfo);
		result->LE_DELTA_F0_FN_MAX = dbResult;
		GetTestResult(_T("DELTA_F1_F0"), &dbResult, lpszRunInfo);
		result->LE_DELTA_F1_F0 = dbResult;
		GetTestResult(_T("DELTA_Fn_Fn5_MAX"), &dbResult, lpszRunInfo);
		result->LE_DELTA_FNFN_5MAX = dbResult;
		GetTestResult(_T("CRC_OK"), &iResult, lpszRunInfo);
		result->LE_IS_CRC_OK = (double)iResult;
		GetTestResult(_T("DELTA_F2_F1_AV_RATIO"), &dbResult, lpszRunInfo);
		result->LE_DELTA_F2_F1_DIV_AVG = dbResult;		
	}else if((strcmp(BtDutParam.PACKET_TYPE, _T("2DH1")) == NULL) || (strcmp(BtDutParam.PACKET_TYPE, _T("2DH3")) == NULL) || (strcmp(BtDutParam.PACKET_TYPE, _T("2DH5")) == NULL) 
		   || (strcmp(BtDutParam.PACKET_TYPE, _T("3DH1")) == NULL) || (strcmp(BtDutParam.PACKET_TYPE, _T("3DH3")) == NULL) || (strcmp(BtDutParam.PACKET_TYPE, _T("3DH5")) == NULL)){
		
		GetTestResult(_T("FREQ_DEVIATION"), &dbResult, lpszRunInfo);
		result->FREQ_DEVIATION = dbResult;		
		GetTestResult(_T("FREQ_EST"), &dbResult, lpszRunInfo);
		result->FREQ_EST = dbResult;		
		GetTestResult(_T("FREQ_EST_HEADER"), &dbResult, lpszRunInfo);
		result->FREQ_EST_HEADER = dbResult;		
		GetTestResult(_T("EDR_EVM_AV"), &dbResult, lpszRunInfo);
		result->EDR_EVM_AV = dbResult;		
		GetTestResult(_T("EDR_EVM_PK"), &dbResult, lpszRunInfo);
		result->EDR_EVM_PK = dbResult;			
		GetTestResult(_T("EDR_POW_DIFF_DB"), &dbResult, lpszRunInfo);
		result->EDR_POW_DIFF_DB = dbResult;		
		GetTestResult(_T("EDR_OMEGA_I"), &dbResult, lpszRunInfo);
		result->EDR_OMEGA_I = dbResult;			
		GetTestResult(_T("EDR_EXTREME_OMEGA_0"), &dbResult, lpszRunInfo);
		result->EDR_EXTREME_OMEGA_0 = dbResult;		
		GetTestResult(_T("EDR_EXTREME_OMEGA_I0"), &dbResult, lpszRunInfo);
		result->EDR_EXTREME_OMEGA_I0 = dbResult;			
	}else if((strcmp(BtDutParam.PACKET_TYPE, _T("DH1")) == NULL) || (strcmp(BtDutParam.PACKET_TYPE, _T("DH3")) == NULL) || (strcmp(BtDutParam.PACKET_TYPE, _T("DH5")) == NULL)){

		GetTestResult(_T("FREQ_DEVIATION"), &dbResult, lpszRunInfo);
		result->FREQ_DEVIATION = dbResult;		
		GetTestResult(_T("DELTA_F2_AV_ACCESS"), &dbResult, lpszRunInfo);
		result->DELTA_F2_AV_ACCESS = dbResult;		
		GetTestResult(_T("DELTA_F2_MAX_ACCESS"), &dbResult, lpszRunInfo);
		result->DELTA_F2_MAX_ACCESS = dbResult;		
		GetTestResult(_T("FREQ_EST"), &dbResult, lpszRunInfo);
		result->FREQ_EST = dbResult;		
		GetTestResult(_T("FREQ_EST_HEADER"), &dbResult, lpszRunInfo);
		result->FREQ_EST_HEADER = dbResult;		
		GetTestResult(_T("FREQ_DRIFT"), &dbResult, lpszRunInfo);
		result->FREQ_DRIFT = dbResult;		
		GetTestResult(_T("DELTA_F1_AVERAGE"), &dbResult, lpszRunInfo);
		result->DELTA_F1_AVERAGE = dbResult;		
		GetTestResult(_T("DELTA_F2_AVERAGE"), &dbResult, lpszRunInfo);
		result->DELTA_F2_AVERAGE = dbResult;		
		GetTestResult(_T("DELTA_F2_MAX"), &dbResult, lpszRunInfo);
		result->DELTA_F2_MAX = dbResult;		
		GetTestResult(_T("MAX_FREQ_DRIFT_RATE"), &dbResult, lpszRunInfo);
		result->MAX_FREQ_DRIFT_RATE = dbResult;			
		GetTestResult(_T("BANDWIDTH_20DB"), &dbResult, lpszRunInfo);
		result->BAND_WIDTH_20DB = dbResult;		
		GetTestResult(_T("DELTA_F2_F1_AV_RATIO"), &dbResult, lpszRunInfo);
		result->DELTA_F2_F1_DIV_AVERAGE = dbResult;			
	}

//double LE_PDU_LENGTH;
//double INIT_FREQ_ERR;
//double EDR_EVM_VS_TIME;	
//double ACP_ERR_VALID;

	return TRUE;
}