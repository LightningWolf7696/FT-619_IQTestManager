#ifndef HTTPCLIENT_H  
#define HTTPCLIENT_H  

#include <afxinet.h>  
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
 
#define  IE_AGENT  _T("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)")
// 操作成功  
#define SUCCESS        0  
// 操作失敗  
#define FAILURE        1  
// 操作超時 www.it165.net  
#define OUTTIME        2  

class CHttpClient  
{  
public:  
   CHttpClient(char *strAgent = IE_AGENT);  
   virtual ~CHttpClient(void);  

   int HttpGet (char *strUrl, char *strPostData, vector<string> &strResponse);  
   int HttpPost(char *strUrl, char *strPostData, vector<string> &strResponse);  
   int HttpPut (char *strUrl, char *strPostData, vector<string> &strResponse);  

private:  
   int ExecuteRequest(int strMethod, char *strUrl, char *strPostData, vector<string> &strResponse);  
   void Clear();  

private:  
   CInternetSession *m_pSession;  
   CHttpConnection *m_pConnection;  
   CHttpFile *m_pFile; 
   string m_Resource;
};  

#endif // HTTPCLIENT_H  