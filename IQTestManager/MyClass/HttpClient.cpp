#include "..\stdafx.h"
#include "HttpClient.h"
 
#define  BUFFER_SIZE       1024
 
#define  NORMAL_CONNECT    INTERNET_FLAG_KEEP_CONNECTION
#define  SECURE_CONNECT    NORMAL_CONNECT | INTERNET_FLAG_SECURE
#define  NORMAL_REQUEST    INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE 
#define  SECURE_REQUEST    NORMAL_REQUEST | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID
 
CHttpClient::CHttpClient(char * strAgent)
{
	m_pSession = new CInternetSession(strAgent);
	m_pConnection = NULL;
	m_pFile = NULL;
   m_Resource = "";
}
 
 
CHttpClient::~CHttpClient(void)
{
	Clear();
	if (NULL != m_pSession)
	{
		m_pSession->Close();
		delete m_pSession;
		m_pSession = NULL;
	}
}
 
void CHttpClient::Clear()
{
	if (NULL != m_pFile)
	{
		m_pFile->Close();
		delete m_pFile;
		m_pFile = NULL;
	}
 
	if (NULL != m_pConnection)
	{
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}
}

int CHttpClient::ExecuteRequest(int strMethod, char *strUrl, char *strPostData,vector<string> &strResponse)
{
	int result =FAILURE ;
	//WCHAR* wPostData = strPostData.GetBuffer();
	CString strServer;
	CString strObject;
	DWORD dwServiceType;
	INTERNET_PORT nPort;

	AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);
	if (AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)
	{
		return FAILURE;
	}
 
	try
	{
		m_pConnection = m_pSession->GetHttpConnection(strServer,
			dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_CONNECT : SECURE_CONNECT,
			nPort);
		m_pFile = m_pConnection->OpenRequest(strMethod, strObject,
			NULL, 1, NULL, NULL,
			(dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_REQUEST : SECURE_REQUEST));
 
      //m_pFile->AddRequestHeaders("Accept: */*,application/json");
	   //m_pFile->AddRequestHeaders("Accept-Charset:UTF8");
	   //m_pFile->AddRequestHeaders("Accept-Language: zh-cn;q=0.8,en;q=0.6,ja;q=0.4");
	   //m_pFile->AddRequestHeaders("Content-Type:application/json");
      CString postDataLen;
      if (strPostData != NULL)
         postDataLen.Format("%d", strlen(strPostData)); 
      else
         postDataLen = "0";
      m_pFile->AddRequestHeaders("Content-Type: application/x-www-form-urlencoded\r\n");
      m_pFile->AddRequestHeaders("Content-Length:"+ postDataLen + "\r\n\r\n");

		char *cword;
		if (strPostData != NULL){
			cword = (char*)calloc(strlen(strPostData), sizeof(char));
			if (cword == NULL)                    
			{
				free(cword);
			}
			//memset(cword, 0, num*sizeof(char));
         strcpy(cword,strPostData);
			printf("content%d\n", strlen(cword));
			m_pFile->SendRequest(NULL, 0, cword, strlen(cword));
		}
		else{
			m_pFile->SendRequest(NULL, 0, NULL, 0);
		}
	
		DWORD dwRet;
		m_pFile->QueryInfoStatusCode(dwRet);
		printf("HTTP_STATUS_code:%d\n", dwRet);
		if (dwRet == HTTP_STATUS_OK){
			result = SUCCESS;
		}

		char szChars[BUFFER_SIZE + 1] = { 0 };
		string strRawResponse = "";
		UINT nReaded = 0;

      while((nReaded = m_pFile->Read((void*)szChars, BUFFER_SIZE)) > 0){
         szChars[nReaded] = '\0';
         strResponse.push_back(szChars);
         memset(szChars, 0, BUFFER_SIZE + 1);
         cout << "111";
      }
		//do
		//{
		//}while (nReaded && !(strRawResponse.find("</html>")));
     // strResponse = strRawResponse;
		/*utf8转unicode*/
	/*	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, strRawResponse.c_str(), -1, NULL, 0);
		WCHAR *pUnicode = new WCHAR[unicodeLen + 1];
		memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, strRawResponse.c_str(), -1, pUnicode, unicodeLen);
		strResponse = pUnicode;
		//TRACE(strResponse + L"");
		delete[]pUnicode;
		pUnicode = NULL; */
		
		Clear();
	}
	catch (CInternetException* e)
	{
		Clear();
		DWORD dwErrorCode = e->m_dwError;
		e->Delete();
 
		DWORD dwError = GetLastError();
 
		printf("dwError = %d", dwError, 0);
 
		strResponse.push_back("CInternetException\n");
 
		if (ERROR_INTERNET_TIMEOUT == dwErrorCode)
		{
			return OUTTIME;
		}
		else
		{
			return FAILURE;
		}
	}
	return result;
}
 
int CHttpClient::HttpGet(char *strUrl, char *strPostData, vector<string> &strResponse)
{
	return ExecuteRequest(CHttpConnection::HTTP_VERB_GET, strUrl, NULL, strResponse);
}
 
int CHttpClient::HttpPost(char *strUrl, char *strPostData, vector<string> &strResponse)
{
	return ExecuteRequest(CHttpConnection::HTTP_VERB_POST, strUrl, strPostData, strResponse);
}

int CHttpClient::HttpPut(char *strUrl, char * strPostData, vector<string> &strResponse)
{
	return ExecuteRequest(CHttpConnection::HTTP_VERB_PUT, strUrl, strPostData, strResponse);
}
