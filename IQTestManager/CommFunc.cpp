#include "StdAfx.h"
#include "CommFunc.h"
#include <io.h>

LABELDATA  CCommFunc::g_LabelData;

CCommFunc::CCommFunc(void)
{	
}

CCommFunc::~CCommFunc(void)
{
}

BOOL CCommFunc::ExportTestMessage(funcGetMessage cbMessage, const TCHAR *format, ...)
{
	TCHAR szBuffer[DLL_INFO_SIZE];
	va_list marker;
	va_start(marker, format);
	vsprintf_s(szBuffer, sizeof(szBuffer), format, marker);
	va_end(marker);
	cbMessage(szBuffer);
	return TRUE;
}

BOOL CCommFunc::ExtractString(LPSTR szSrc, LPSTR szDest, int nDestLens, LPSTR szStart, LPSTR szEnd)
{
	CString strSrc,strTemp,strDest;
	int nAddr;
	strSrc.Format(_T("%s"),szSrc);
	nAddr = strSrc.Find(szStart);
	strTemp = strSrc.Mid(nAddr+strlen(szStart));
	nAddr = strTemp.Find(szEnd);
	if(nAddr != -1)
		strDest = strTemp.Left(strTemp.Find(szEnd));	
	strDest = strDest.Trim();
	sprintf_s(szDest,nDestLens,"%s",strDest.GetBuffer());
	return TRUE;
}

BOOL CCommFunc::CombineMessage(LPSTR lpszSrcData, LPSTR lpszAvailData, int nAvailLens, LPSTR szKey)
{
	TCHAR *pszToken;
	CString strKey,strSrcData;
	int nAddr;
	pszToken = strstr(lpszSrcData, szKey);
	if(pszToken)
	{
		nAddr = pszToken-lpszSrcData;
		strSrcData.Format(_T("%s"),lpszSrcData);
		//create avaiLABEL data
		strKey = strSrcData.Left(nAddr-1);//Skip chr(13)
		strcpy_s(lpszAvailData,nAvailLens,strKey);

		//separate unused data
		strKey = strSrcData.Mid(nAddr+1);
		strcpy_s(lpszSrcData, SOCKET_MAX_LENGTH, strKey);
		return TRUE;
	}

	return FALSE;
}

BOOL CCommFunc::DelFolderAllFile(LPSTR lpszDelPath, LPSTR lpszCmd, LPSTR lpszRunInfo)
{
	char szMessage[DLL_INFO_SIZE]={0};
	char szDestFileName[MAX_PATH]={0};
	FILE *stream = NULL;
	errno_t err;

	//Check delete folder is exist or not.
	if(!CheckFileExist(lpszDelPath,lpszRunInfo))
		return FALSE;

	sprintf_s(szDestFileName,sizeof(szDestFileName),"%s\\%s",lpszDelPath,"MyDelete.bat");	
	if((err = fopen_s(&stream, szDestFileName, "w")) == NULL)
	{
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"Can not create  cableLoss from DUT.[Name:%s]",szDestFileName);
		return FALSE;
	}

	fprintf(stream,"cd %s\n",lpszDelPath);
	fprintf(stream,"%c:\n",lpszDelPath[0]);
	fprintf(stream,"%s\n",lpszCmd);
	fclose(stream);

	WinExec(szDestFileName,SC_MINIMIZE);

	//Delete bat file
	//	if(CheckFileExist(szDestFileName,lpszRunInfo)) DeleteFile(szDestFileName);

	return TRUE;
}

BOOL CCommFunc::OutputCommand(LPSTR szFileName, const TCHAR *format, ...)
{
	DWORD dwWriteByte = 0;

	//#ifdef _DEBUG
	TCHAR szBuffer[65535];
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szLocalPath[MAX_PATH]={0};
	va_list marker;
	va_start(marker, format);
	vsprintf_s(szBuffer, sizeof(szBuffer), format, marker);
	va_end(marker);

	CString strLogPath;

	if(strstr(szFileName, ":"))
		strLogPath = szFileName; 
	else{
		if(GetLocalDirectory(szLocalPath,szMessage) != ERROR_SUCCESS)
			strLogPath = _T("c:\\");
		strLogPath.Format("%s",szLocalPath);
		strLogPath += szFileName;
	}

	CString strOutput;// = cTime.Format(_T("%Y-%m-%d %H:%M:%S "));

	strOutput += szBuffer;
	strOutput += _T("\r\n");

	HANDLE hFile = ::CreateFile(	strLogPath, 
		GENERIC_WRITE, 
		FILE_SHARE_WRITE,
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwRC = ::SetFilePointer(hFile,
			0,
			NULL,
			FILE_END);

		//always append
		if(dwRC != INVALID_SET_FILE_POINTER)
			::WriteFile(hFile, strOutput, (DWORD)strOutput.GetLength()*sizeof(TCHAR), &dwWriteByte, NULL);

		::CloseHandle(hFile);
	}
	//#endif

	return dwWriteByte;
}

BOOL CCommFunc::OutputMessage(LPSTR szFileName, const TCHAR *format, ...)
{
	DWORD dwWriteByte = 0;

	//#ifdef _DEBUG
	TCHAR szBuffer[65535];
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szLocalPath[MAX_PATH]={0};
	va_list marker;
	va_start(marker, format);
	vsprintf_s(szBuffer, sizeof(szBuffer), format, marker);
	va_end(marker);

	CString strLogPath;

	if(GetLocalDirectory(szLocalPath,szMessage) != ERROR_SUCCESS)
		strLogPath = _T("c:\\");
	strLogPath.Format("%s",szLocalPath);
	strLogPath += szFileName;

	CString strOutput;// = cTime.Format(_T("%Y-%m-%d %H:%M:%S "));

	strOutput += szBuffer;
	strOutput += _T("\r\n");

	HANDLE hFile = ::CreateFile(	strLogPath, 
		GENERIC_WRITE, 
		FILE_SHARE_WRITE,
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwRC = ::SetFilePointer(hFile,
			0,
			NULL,
			FILE_END);

		//always append
		if(dwRC != INVALID_SET_FILE_POINTER)
			::WriteFile(hFile, strOutput, (DWORD)strOutput.GetLength()*sizeof(TCHAR), &dwWriteByte, NULL);

		::CloseHandle(hFile);
	}
	//#endif

	return dwWriteByte;
}

CString CCommFunc::TraceStringMid(LPSTR szMessage, LPSTR szStart, LPSTR szEnd)
{
	CString strMsg="";
	int addr;

	strMsg.Format("%s",szMessage);
	addr = strMsg.Find(szStart);
	if(addr < 0 ) return _T("");
	strMsg = strMsg.Mid(addr+strlen(szStart));
	addr = strMsg.Find(szEnd);
	if(addr < 0) return _T("");
	return strMsg.Left(addr).Trim();
}



BOOL CCommFunc::QueryUsbRelay(UINT nComPort, BOOL *PortCtrl, LPSTR lpszRunInfo)
{
	DWORD dwCount=0, dwBytesRead=0;
	TCHAR szExpectData[8]={0x55,0x01,0x01,0x00,0x00,0x00,0x00,0x00};
	TCHAR szActualData[8]={0};

	for(int nAddr=0; nAddr<4; nAddr++)
		szExpectData[nAddr+3] = (PortCtrl[nAddr])? 0x02:0x01;

	for(int nAddr=0; nAddr<7; nAddr++)
		szExpectData[7] += szExpectData[nAddr];

	if(!SerialPortWriteDataEx(nComPort, szExpectData, sizeof(szExpectData), lpszRunInfo))
		return FALSE;

	if(!SerialPortReadDataEx(nComPort,szActualData,sizeof(szActualData),dwBytesRead, lpszRunInfo))
		return FALSE;

	if(szActualData[0] != 0x22)
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay return failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
			nComPort,szActualData[0],szActualData[1],szActualData[2],szActualData[3],szActualData[4],szActualData[5],szActualData[6],szActualData[7]);
		return FALSE;
	}
	for(int nAddr=3; nAddr<7; nAddr++)
	{
		if(szExpectData[nAddr] != szActualData[nAddr])
		{
			sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay write failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x;Expect:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
				nComPort,szActualData[0],szActualData[1],szActualData[2],szActualData[3],szActualData[4],szActualData[5],szActualData[6],szActualData[7],
				szExpectData[0],szExpectData[1],szExpectData[2],szExpectData[3],szExpectData[4],szExpectData[5],szExpectData[6],szExpectData[7]);
			return FALSE;
		}
	}

	for(int nAddr=0; nAddr<4; nAddr++)
		PortCtrl[nAddr] = (szActualData[nAddr+3] == 0x02)? TRUE:FALSE;

	return TRUE;
}

BOOL CCommFunc::WriteUsbRelay(UINT nComPort, BOOL *PortCtrl, LPSTR lpszRunInfo)
{
	DWORD dwCount=0, dwBytesRead=0;
	TCHAR szData[8]={0x55,0x01,0x01,0x00,0x00,0x00,0x00,0x00};

	for(int nAddr=0 ; nAddr<4 ; nAddr++)
		szData[nAddr+3] = (PortCtrl[nAddr])? 0x02:0x01;

	for(int nAddr=0 ; nAddr<7 ; nAddr++)
		szData[7] += szData[nAddr];

	if(!SerialPortWriteDataEx(nComPort, szData, sizeof(szData), lpszRunInfo))
		return FALSE;

	return TRUE;
}

BOOL CCommFunc::ReadUsbRelay(UINT nComPort, BOOL *PortCtrl, LPSTR lpszRunInfo)
{
	DWORD dwCount=0, dwBytesRead=0;
	TCHAR szData[8]={0x55,0x01,0x0,0x00,0x00,0x00,0x00,0x56};
	TCHAR szMessage[DLL_INFO_SIZE]={0};

	if(!SerialPortWriteDataEx(nComPort, szData, sizeof(szData), lpszRunInfo))
		return FALSE;
	if(!SerialPortReadDataEx(nComPort,szData,sizeof(szData),dwBytesRead, lpszRunInfo))
		return FALSE;

	if(szData[0] != 0x22)
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay return failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
			nComPort,szData[0],szData[1],szData[2],szData[3],szData[4],szData[5],szData[6],szData[7]);
		return FALSE;
	}
	for(int nAddr=0 ; nAddr<4 ; nAddr++)
		PortCtrl[nAddr] = (szData[nAddr+3] == 0x02)? TRUE:FALSE;

	return TRUE;
}

BOOL CCommFunc::CloseRelayAll(UINT nComPort, LPSTR lpszRunInfo)
{
	DWORD dwCount=0, dwBytesRead=0;
	TCHAR szExpectData[8]={0x55,0x01,0x01,0x01,0x01,0x01,0x01,0x5B};
	TCHAR szActualData[8]={0};

	if(!SerialPortWriteDataEx(nComPort, szExpectData, sizeof(szExpectData), lpszRunInfo))
		return FALSE;

	Sleep(100);
	if(!SerialPortReadDataEx(nComPort,szActualData,sizeof(szActualData),dwBytesRead, lpszRunInfo))
		return FALSE;

	if(szActualData[0] != 0x22)
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay return failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
			nComPort,szActualData[0],szActualData[1],szActualData[2],szActualData[3],szActualData[4],szActualData[5],szActualData[6],szActualData[7]);
		return FALSE;
	}
	for(int nAddr=3; nAddr<7; nAddr++)
	{
		if(szActualData[nAddr] == 0x02)
		{
			sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay write failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x;Expect:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
				nComPort,szActualData[0],szActualData[1],szActualData[2],szActualData[3],szActualData[4],szActualData[5],szActualData[6],szActualData[7],
				szExpectData[0],szExpectData[1],szExpectData[2],szExpectData[3],szExpectData[4],szExpectData[5],szExpectData[6],szExpectData[7]);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CCommFunc::WriteUsbRelayOnePort(UINT nComPort, RELAY_OUT nOutNum, RELAY_WORKING bMode, LPSTR lpszRunInfo)
{
	DWORD dwCount=0, dwBytesRead=0;
	TCHAR szExpectData[8]={0x55,0x01,0x12,0x00,0x00,0x00,0x00,0x00};
	TCHAR szActualData[8]={0};

	szExpectData[2] = (bMode == RELAY_ON)? 0x12 : 0x11;

	szExpectData[6] = nOutNum;

	szExpectData[7]=0;
	for(int nAddr=0; nAddr<7; nAddr++)
		szExpectData[7] += szExpectData[nAddr];

	if(!SerialPortWriteDataEx(nComPort, szExpectData, sizeof(szExpectData), lpszRunInfo))
		return FALSE;

	Sleep(100);
	if(!SerialPortReadDataEx(nComPort,szActualData,sizeof(szActualData),dwBytesRead, lpszRunInfo))
		return FALSE;

	if(szActualData[0] != 0x22)
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay return failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
			nComPort,szActualData[0],szActualData[1],szActualData[2],szActualData[3],szActualData[4],szActualData[5],szActualData[6],szActualData[7]);
		return FALSE;
	}

	if( (szActualData[6] & (1 << (nOutNum-1))) != (bMode << (nOutNum-1)))
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay write failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x;Expect:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
			nComPort,szActualData[0],szActualData[1],szActualData[2],szActualData[3],szActualData[4],szActualData[5],szActualData[6],szActualData[7],
			szExpectData[0],szExpectData[1],szExpectData[2],szExpectData[3],szExpectData[4],szExpectData[5],szExpectData[6],szExpectData[7]);
		return FALSE;
	}

	return TRUE;
}

BOOL CCommFunc::ExportTestLABEL(const COLORREF *Color,const TCHAR *format, ...)
{
	TCHAR szBuffer[MAX_PATH];
	va_list marker;
	va_start(marker, format);
	vsprintf_s(szBuffer, sizeof(szBuffer), format, marker);
	va_end(marker);

	if(!Color || (strcmp(szBuffer, _T("")) == 0)) return FALSE;

	g_LabelData.Color = *Color;
	g_LabelData.Keep = FALSE;
	strcpy_s(g_LabelData.LabelName, sizeof(g_LabelData.LabelName), szBuffer);

	::SendMessage(g_hParentWnd, WM_ERASELABEL, (WPARAM)NULL, (LPARAM)&g_LabelData);
	::SendMessage(g_hParentWnd, WM_CREATELABEL, (WPARAM)NULL, (LPARAM)&g_LabelData);

	return TRUE;
}

BOOL CCommFunc::OutputMsgToBoth(funcGetMessage cbMessage, const TCHAR *format, ...)
{
	TCHAR szBuffer[DATA_MAX_BUFFER_SIZE];
	va_list marker;
	va_start(marker, format);
	vsprintf_s(szBuffer, sizeof(szBuffer), format, marker);
	va_end(marker);
	ExportTestLog("%s", szBuffer);
	SeparateListInfoSize(cbMessage, szBuffer, strlen(szBuffer));
	return TRUE;
}

BOOL CCommFunc::SeparateListInfoSize(funcGetMessage cbMessage, LPSTR lpszData, int size)
{
	TCHAR szBuffer[DLL_INFO_SIZE];
	CString strData, strTemp, strSend;
	if(size >= DLL_INFO_SIZE)
	{
		strData.Format("%s", lpszData);
		strTemp = strData;
		do 
		{
			if(strTemp.GetLength() >= DLL_INFO_SIZE)
			{
				strSend = strTemp.Left(DLL_INFO_SIZE-1);
				cbMessage(strSend.GetBuffer());
				strTemp = strTemp.Mid(DLL_INFO_SIZE);
			}else{
				cbMessage(strTemp.GetBuffer());
				break;
			}
		} while (TRUE);
		sprintf_s(lpszData, DLL_INFO_SIZE, "%s", strData.Right(DLL_INFO_SIZE-1));
		lpszData[DLL_INFO_SIZE-1] = 0;
	}else
		cbMessage(lpszData);
	return TRUE;
}

BOOL CCommFunc::FillErrorReport( ERRORREPORT *pErrorReport, 
	BOOL bDisableSfcsLink,
	char *szErrorCode,
	char *szErrorDetail,
	char *szErrorMsg,
	char *szFailedTest,
	BOOL bTestResult)

{

	pErrorReport->DisableSfcsLink = bDisableSfcsLink;

	if(strlen(szErrorCode) > (sizeof(pErrorReport->ErrorCode)-1))
		szErrorCode[sizeof(pErrorReport->ErrorCode)-1] = 0;
	strcpy_s(pErrorReport->ErrorCode,sizeof(pErrorReport->ErrorCode),szErrorCode);

	if(strlen(szErrorDetail) > (sizeof(pErrorReport->ErrorDetail)-1))
		szErrorDetail[sizeof(pErrorReport->ErrorDetail)-1] = 0;
	strcpy_s(pErrorReport->ErrorDetail,sizeof(pErrorReport->ErrorDetail),szErrorDetail);

	if(strlen(szErrorMsg) > (sizeof(pErrorReport->ErrorMsg)-1))
		szErrorMsg[sizeof(pErrorReport->ErrorMsg)-1] = 0;
	strcpy_s(pErrorReport->ErrorMsg,sizeof(pErrorReport->ErrorMsg),szErrorMsg);

	if(strlen(szFailedTest) > (sizeof(pErrorReport->FailedTest)-1))
		szFailedTest[sizeof(pErrorReport->FailedTest)-1] = 0;
	strcpy_s(pErrorReport->FailedTest,sizeof(pErrorReport->FailedTest),szFailedTest);

	pErrorReport->TestResult = bTestResult;
	return TRUE;
}

CString CCommFunc::TransXmlSymbo(CString strData)
{
	TCHAR key[2]={0};
	strData.Replace("<", "&lt;");
	strData.Replace(">", "&gt;");
	strData.Replace("&", "&amp;");
	strData.Replace("'", "&apos;");
	strData.Replace("\"", "&quot;");
	key[0] = 0x8;	
	strData.Replace(key, "");
	strData.Replace("%","%%");
	return strData;
}

CString CCommFunc::TransXmlSymbo(LPSTR lpszData)
{
	CString strData;
	TCHAR key[2]={0};

	strData.Format("%s", lpszData);
	strData.Replace("<", "&lt;");
	strData.Replace(">", "&gt;");
	strData.Replace("&", "&amp;");
	strData.Replace("'", "&apos;");
	strData.Replace("\"", "&quot;");
	key[0] = 0x8;	
	strData.Replace(key, "");
	strData.Replace("%","%%");
	return strData;
}

VAL_COMPARE CCommFunc::ValueCompare(double High, double Low, double Actual)
{	
	if((Actual > High) && (High != DATA_VAL_INIT))
		return OVER_SPEC;
	else if((Actual < Low) && (Low != DATA_VAL_INIT))
		return UNDER_SPEC;
	else
		return PASS_SPEC;
}
BOOL CCommFunc::SubDeleteFile(const TCHAR *format, ...)
{
	TCHAR szFilePath[MAX_PATH]={0};
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	va_list marker;
	va_start(marker, format);
	vsprintf_s(szFilePath, sizeof(szFilePath), format, marker);
	va_end(marker);

	if(CheckFileExist(szFilePath,szMessage))
		DeleteFile(szFilePath);
	return TRUE;
}
CString CCommFunc::GetCurrSystemTime()
{
	CString strSystemTime;
	CTime cTime = CTime::GetCurrentTime();
	strSystemTime = cTime.Format(_T("%Y/%m/%d %H:%M:%S"));

	return strSystemTime;
}

BOOL CCommFunc::ExportTestLable(const COLORREF *Color,const TCHAR *format, ...)
{
	TCHAR szBuffer[MAX_PATH];
	va_list marker;
	va_start(marker, format);
	vsprintf_s(szBuffer, sizeof(szBuffer), format, marker);
	va_end(marker);

	if(!Color || (strcmp(szBuffer, _T("")) == 0)) return FALSE;

	g_LabelData.Color = *Color;
	g_LabelData.Keep = FALSE;
	strcpy_s(g_LabelData.LabelName, sizeof(g_LabelData.LabelName), szBuffer);
	::SendMessage(g_hParentWnd, WM_CREATELABEL, (WPARAM)NULL, (LPARAM)&g_LabelData);

	return TRUE;
}

void CCommFunc::LoadTestDataFile(LPCSTR lpszFileName,LPSTR lpszRunInfo, int length)
{
	USHORT check = 0;

	FILE *stream;
	int FileNo;
	long streamLength, ActuallyRead;
	TCHAR *Content = NULL;
	TCHAR szMessage[MAX_PATH]={0};

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
	memcpy(lpszRunInfo, Content, length);

	if(Content)	free(Content);
}

BOOL CCommFunc::RunApplication(LPSTR lpszMethod, LPSTR lpszFilePath, LPSTR lpszRunInfo, LPSTR lpszName)
{
	TCHAR szLocalPath[MAX_PATH]={0};
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	BOOL bRtn;
	CString strFileName;
	int iPos=0;
	double dbTestTimeCost;
	CString strTemp;
	TCHAR szFilePath[MAX_PATH];

	strCopy(szFilePath, lpszFilePath);
	if(szFilePath[0] == '.')
	{
		GetLocalDirectory(szLocalPath, szMessage);
		strTemp.Format("%s", &szFilePath[2]);
		sprintf_s(szFilePath, sizeof(szFilePath), "%s%s", szLocalPath, strTemp.GetBuffer());
	}

	if(ProcessFind(szFilePath, szMessage))
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Found Process : %s", szFilePath);
		return TRUE;
	}

	strFileName.Format("%s",szFilePath);
	iPos = strFileName.ReverseFind('\\');
	strFileName.Delete(0, iPos+1);
	while(ProcessFind(strFileName.GetBuffer(), szMessage)){ProcessKill(strFileName.GetBuffer(), szMessage);};
	if(strcmp(lpszMethod,"NORMAL") == NULL){
		ShellExecute(NULL, "open",szFilePath, NULL, NULL, SW_NORMAL);
//		WinExec(szFilePath,SW_NORMAL);
		return TRUE;
	}else{
		if(!MyShellExecute(szFilePath, NULL, SW_HIDE, lpszRunInfo)) return FALSE;
	}

	dbTestTimeCost=GetTickCount();
	do
	{
		if(strcmp(lpszName, "") != NULL)
		{
			if(bRtn = ProcessFind(lpszName, lpszRunInfo))
				break;
		}else{
			if(bRtn = ProcessFind(szFilePath, lpszRunInfo))
				break;
		}
	} while (((GetTickCount() - dbTestTimeCost)/1000) < 5);

	return bRtn;
}

BOOL CCommFunc::MyShellExecute(LPSTR lpszFile, LPSTR lpszArguments, int nShowCmd, LPSTR lpszRunInfo)
{
	int iPos = 0;
	CString strPath,strAppName;
	char szMessage[DLL_INFO_SIZE]={0};
	char szDestFileName[MAX_PATH]={0};
	FILE *stream = NULL;

	strPath.Format(_T("%s"), lpszFile);	
	iPos = strPath.ReverseFind('\\');

	strAppName = strPath.Mid(iPos+1,strPath.GetLength()-iPos);
	strPath.Delete(iPos+1, strPath.GetLength()-iPos-1);

	sprintf_s(szDestFileName,sizeof(szDestFileName),"%s%s",strPath.GetBuffer(),"MyShellExecute.bat");	

	if(CheckFileExist(szDestFileName, szMessage)) DeleteFile(szDestFileName);

	if((stream = fopen(szDestFileName, "w")) == NULL)
	{
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"Can't open file, [Name : %s]",szDestFileName);
		return FALSE;
	}

	fprintf(stream,"cd %s\n",strPath);
	fprintf(stream,"%c:\n",szDestFileName[0]);
	fprintf(stream,"START \"%s\" /MIN %s %s\n",lpszFile,strAppName,lpszArguments);
	fclose(stream);

	//WinExec(szDestFileName,SC_MINIMIZE);
	ShellExecute(NULL, "open",szDestFileName, NULL, NULL, nShowCmd);

	return TRUE;
}

int CCommFunc::ReverseFindStr(CString strData, LPSTR szKeyWord, int nStopAddr)
{
	int addr1=0, addr2=0;
	CString strTmp;
	int nKey;
	//	TCHAR szData[1000]={0};
	int srcLens;
	int prev_addr2=SEARCH_NOT_FOUND;
	//	strCopy(szData, strData.GetBuffer());
	nKey = strlen(szKeyWord);
	do 
	{
		addr1 = strData.Find(szKeyWord);
		strData = strData.Mid(addr1+nKey);
		if(addr1>=0) addr2= (addr1+addr2+nKey);
		if(nStopAddr != SEARCH_NOT_FOUND)
		{
			if(addr2 > nStopAddr)
			{
				addr2 = prev_addr2;
				break;
			}
			prev_addr2 = addr2;
		}
	} while (addr1>=0);
	addr2 -= strlen(szKeyWord);
	if(addr2 <= 0)
		addr2 = 0;

	return addr2;	
}
BOOL CCommFunc::SerialPortQueryMultiDataRetry(LPSTR szInput, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo, LPSTR szTestKeyWord, LPSTR szTestKeySuccess)
{
	BOOL bRtn=FALSE;
	CStringArray strAryKeyword;
	CString strKey;

	strAryKeyword.Add(szKeyWord);
//	strAryKeyword.Add(CMD_FAIL);
	//	strAryKeyword.Add("input overrun");
	//strAryKeyword.Add("Fail: interface not opened");
	//	strAryKeyword.Add("bad address");
	//if(!strstr(szInput, "mp_stop"))
	//	strAryKeyword.Add("Fail: not in MP mode");


	for(int nRetry=0; nRetry<3; nRetry++)
	{
		bRtn = SerialPortQueryMultiData(szInput, strAryKeyword, nTimeOutSec,lpszRunInfo, szTestKeyWord, szTestKeySuccess);
		if(bRtn)
		{
			for(int i=1; i<strAryKeyword.GetSize() ; i++)
			{
				strKey = strAryKeyword.GetAt(i);
				if(strstr(lpszRunInfo, strKey))
				{
					bRtn=FALSE;
					break;
				}
			}	
		}
		if(bRtn) break;
	}
	if(!bRtn)
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Get com port data failed,[Key:%s].", szInput);

	return bRtn;

}

BOOL CCommFunc::SerialPortQueryMultiData(LPSTR szInput, const CStringArray& strAryKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo, LPSTR szTestKeyWord, LPSTR szTestKeySuccess)
{
	char szMessage[CONSOLE_MAX_LENGTH]={0},szTemp[CONSOLE_MAX_LENGTH];
	BOOL bRtn = FALSE;
	double dbStartTime = GetTickCount();
	double dbTestTime = GetTickCount();
	int nBootTestKeyTime = 5;
	DWORD dwBytesRead;	
	TCHAR szCmd[2048]={0};
	int   nFailCnt=0;
	TCHAR szTimeMsg[2048]={0};
	CString strMessage="";
	int	  nLens;
	CString strKey;
	
Retry:
	OutputCommand(UART_COMMAND, szInput);
	
	sprintf_s(szCmd,sizeof(szCmd),"%s%s",szInput, g_GlobalInfo.CMD_EOF);
	Sleep(g_GlobalInfo.CONSOLE_SLEEP);
	if(!SerialPortWriteData(szCmd,lpszRunInfo)) return FALSE;
	int n=0;
	do
	{
		n++;
		zeroIt(szTemp);
		if(!SerialPortReadData(szTemp, sizeof(szTemp), dwBytesRead, lpszRunInfo))
			return FALSE;
		strMessage += szTemp;
/*
		if((strlen(szMessage)+strlen(szTemp)) >= sizeof(szMessage))
			strCopy(szMessage,&szMessage[strlen(szMessage)-100]);	
		strcat_s(szMessage,strlen(szMessage)+strlen(szTemp)+1,szTemp);
*/
		Sleep(10);
		if(((GetTickCount() - dbStartTime)/1000) > nTimeOutSec)
		{		
			nFailCnt++;
			if(nFailCnt >= 3)
			{				
				bRtn = FALSE;
				break;
			}
			ExportTestLog("[ERROR]Command time out,[%s]",szInput);
			for(int CmdCnt=0 ; CmdCnt<2 ; CmdCnt++)
			{
				Sleep(g_GlobalInfo.CONSOLE_SLEEP);
				if(!SerialPortWriteData("\n",lpszRunInfo)) 
					return FALSE;
				Sleep(100);
			}
			Sleep(500);
			SerialPortCleanData(lpszRunInfo);
			dbStartTime = GetTickCount();
			dbTestTime = GetTickCount();
			goto Retry;
			
		}else if((strlen(szTestKeyWord) > 0) && (strlen(szMessage) == 0)) {//Check DUT is ready.
			if(((GetTickCount() - dbTestTime)/1000) > nBootTestKeyTime)
			{
				OutputMessage(UART_COMMAND, szTestKeyWord);
				Sleep(g_GlobalInfo.CONSOLE_SLEEP);
				if(SerialPortWriteData(szTestKeyWord,lpszRunInfo))
				{
					Sleep(100);
					if(!SerialPortReadData(szTemp, sizeof(szTemp), dwBytesRead, lpszRunInfo))
						return FALSE;

					char szDbgMsg[2048];
					sprintf_s(szDbgMsg,2048,"szTemp = [%s],lens = %d; szTestKeySuccess = [%s], lens = %d",szTemp,strlen(szTemp),szTestKeySuccess,strlen(szTestKeySuccess));
					if(strstr(szTemp, szTestKeySuccess))
						return TRUE;
				}
				dbTestTime = GetTickCount();
			}
		}
		for(int i=0 ; i<strAryKeyWord.GetSize() ; i++)
		{
			strKey = strAryKeyWord.GetAt(i);
			if(strMessage.Find(strKey) != SEARCH_NOT_FOUND)
			{
				bRtn=TRUE;
				break;
			}
		}	
	} while(!bRtn);

	memset(lpszRunInfo,0x00,DLL_INFO_SIZE);
	zeroIt(szMessage);
	strMessage.Replace("%","p");
	nLens = strMessage.GetLength();
	if(nLens > 500)
		sprintf_s(szMessage,DLL_INFO_SIZE,strMessage.Right(500));
	else
		sprintf_s(szMessage,DLL_INFO_SIZE,"%s",strMessage.GetBuffer());
	if(bRtn){
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"%s",szMessage);	
	}else{
		zeroIt(szMessage);
		for(int i=0 ; i<strAryKeyWord.GetSize() ; i++){
			strcat(szMessage,strAryKeyWord.GetAt(i));
			strcat(szMessage,";");
		}
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"Get serial port keyword is timeout, [KeyWord = %s, TimeOut = %d]",szMessage,nTimeOutSec);
		if(nLens >= LOG_MAX_SIZE)
			ExportTestLog(strMessage.Right(LOG_MAX_SIZE-1));
		else
			ExportTestLog(strMessage);
	}

	dbTestTime = GetTickCount() - dbStartTime;
	sprintf_s(szTimeMsg,sizeof(szTimeMsg),"%.0lf\t,%s",dbTestTime,szInput);
	OutputMessage(UART_MESSAGE, "%s", strMessage.GetBuffer());
	return bRtn;
}


double CCommFunc::WIFI_ChannelToFrequencyMHz(int Channel)
{
	if(Channel < 15)//2G
	{
		if(Channel == 14)
			return 2484;
		else
		{
			return (Channel*5)+2407;
		}
	}else{//5G
		return (Channel*5) + 5000;
	}
}

BOOL CCommFunc::SeparateTestLog(LPSTR lpszMessage, LPSTR lpszFileName, int maxLength)
{
	int nLens;
	CString strData, strTemp, strMax;
	TCHAR szLineMsg[DLL_INFO_SIZE]={0};

	strData.Format("%s", lpszMessage);
	strTemp = strData;
	nLens = strData.GetLength();

	if(nLens >= maxLength){
		while(!strTemp.IsEmpty())
		{
			strMax = strTemp.Left(maxLength-1);
			if(strcmp(lpszFileName,"") == NULL)
				ExportTestLog("%s", strMax);
			else
				OutputMessage(lpszFileName, "%s", strMax.GetBuffer());
			nLens = strMax.GetLength();
			strTemp = strTemp.Mid(nLens);
		}
	}else{
		if(strcmp(lpszFileName,"") == NULL)
			ExportTestLog("%s", lpszMessage);
		else
			OutputMessage(lpszFileName, "%s", lpszMessage);
	}
	return TRUE;
}



double CCommFunc::BT_ChannelToFrequencyMHz(int Channel)
{
	return 2402+Channel;
}