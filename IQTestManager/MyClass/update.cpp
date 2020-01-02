//#include <Windows.h>
#include "update.h"

#pragma comment(lib, "ws2_32.lib") 
using namespace std;

#define DEFAULT_PAGE_BUF_SIZE 1048576

queue<string> hrefUrl;
hash_set<string> visitedUrl;
hash_set<string> visitedImg;
int depth=0;
int g_ImgCnt=1;

//解析URL，解析出主机名，资源名
bool ParseURL( const string & url, string & host, string & resource){
   if ( strlen(url.c_str()) > 2000 ) {
      return false;
   }

   const char * pos = strstr( url.c_str(), "http://" );
   if( pos==NULL ) pos = url.c_str();
   else pos += strlen("http://");
   if( strstr( pos, "/")==0 )
      return false;
   char pHost[100];
   char pResource[2000];
   sscanf( pos, "%[^/]%s", pHost, pResource );
   host = pHost;
   resource = pResource;
   return true;
}
BOOL CUpdate::GetTime(struct tm *time_info){
   time_t const timer = time(NULL);
   errno_t err;
   //struct tm time_info;
   char timebuffer[26];

   //local time
   err = ctime_s(timebuffer, 26, &timer);
   cout << "local time : " << timebuffer << endl;

   //UTC
   err = gmtime_s(time_info,&timer);
   err = asctime_s(timebuffer,26, time_info);
   cout << "UTC time : " << timebuffer << endl;
  
   return TRUE;
}

BOOL CUpdate::GetWebResponse( const string & url, char * &response, int &bytesRead ){
   string host, resource;
   char strHttpPost[1024] = {0};
   char *pHttpPost;
   // int port = 80;
   char *msg;

   if(!ParseURL( url, host, resource )){
      cout << "Can not parse the url"<<endl;
      return false;
   }

   struct hostent *hp= gethostbyname( host.c_str() );
   if( hp==NULL ){
      cout<< "Can not find host address"<<endl;
      return false;
   }

   SOCKET sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if( sock == -1 || sock == -2 ){
      cout << "Can not create sock."<<endl;
      return false;
   }

   SOCKADDR_IN sa;
   sa.sin_family = AF_INET;
   sa.sin_port = htons( 80 );

   //char addr[5];
   //memcpy( addr, hp->h_addr, 4 );
   //sa.sin_addr.s_addr = inet_addr(hp->h_addr);
   memcpy( &sa.sin_addr, hp->h_addr, 4 );

   if( 0!= connect( sock, (SOCKADDR*)&sa, sizeof(sa) ) ){
      cout << "Can not connect: "<< url <<endl;
      closesocket(sock);
      return false;
   };

   switch (m_REQUEST)
   {
   case GET1:
      {

         //  string ttt = m_Resource_Stok;
         //  ttt = m_Resource_Stok + "/admin/system/devbase";
         //   string request = "GET " + ttt + " HTTP/1.1\r\nHost:" + host + "\r\nConnection:Close\r\n\r\n";
         //     string request = "GET " + resource + " HTTP/1.1\r\nHost:" + host + "\r\nConnection: keep-alive\r\n\r\n\r\n";
         //    sprintf(strHttpPost, request.c_str());
         resource = m_Resource_Stok;
         pHttpPost = "GET %s HTTP/1.1\r\n"
            "Host: %s\r\n\r\n";
         // "Connection:Close\r\n";
         //  "Accept: text/html,application/xhtml+xml,application/xml\r\n\r\n";
         //    "Connection: keep-alive\r\n";
         sprintf(strHttpPost, pHttpPost, resource.c_str(), host.c_str());
         //    printf("qqqq");
         break;
      }
   }
   if( SOCKET_ERROR ==send( sock, strHttpPost, strlen(strHttpPost), 0 ) ){
      cout << "send error" <<endl;
      closesocket( sock );
      return false;
   }

   //response get.
   int m_nContentLength = DEFAULT_PAGE_BUF_SIZE;
   char *pageBuf = (char *)malloc(m_nContentLength);
   memset(pageBuf, 0, m_nContentLength);

   bytesRead = 0;
   int ret = 1;
   cout <<"Read: ";
   while(ret > 0){
      ret = recv(sock, pageBuf + bytesRead, m_nContentLength - bytesRead, 0);

      if(ret > 0)
      {
         bytesRead += ret;
      }

      if( m_nContentLength - bytesRead<100){
         cout << "\nRealloc memorry"<<endl;
         m_nContentLength *=2;
         pageBuf = (char*)realloc( pageBuf, m_nContentLength); 
      }
      cout << ret <<" ";
   }
   cout <<endl;
   pageBuf[bytesRead] = '\0';
   response = pageBuf;
   closesocket( sock );
   return true;
   //cout<< response <<endl;
}


BOOL CUpdate::GetHttpResponse( const string & url, char * &response, int &bytesRead ){
   string host, resource;
   char strHttpPost[1024] = {0};
   char *pHttpPost;
  // int port = 80;
   char *msg;

   if(!ParseURL( url, host, resource )){
      cout << "Can not parse the url"<<endl;
      return false;
   }
   
   struct hostent *hp= gethostbyname( host.c_str() );
   if( hp==NULL ){
      cout<< "Can not find host address"<<endl;
      return false;
   }

   SOCKET sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if( sock == -1 || sock == -2 ){
      cout << "Can not create sock."<<endl;
      return false;
   }

   SOCKADDR_IN sa;
   sa.sin_family = AF_INET;
   sa.sin_port = htons( 80 );
  
   //char addr[5];
   //memcpy( addr, hp->h_addr, 4 );
   //sa.sin_addr.s_addr = inet_addr(hp->h_addr);
   memcpy( &sa.sin_addr, hp->h_addr, 4 );

   if( 0!= connect( sock, (SOCKADDR*)&sa, sizeof(sa) ) ){
      cout << "Can not connect: "<< url <<endl;
      closesocket(sock);
      return false;
   };

   switch (m_REQUEST)
   {
      case GET1:
      {
         
      //  string ttt = m_Resource_Stok;
       //  ttt = m_Resource_Stok + "/admin/system/devbase";
      //   string request = "GET " + ttt + " HTTP/1.1\r\nHost:" + host + "\r\nConnection:Close\r\n\r\n";
    //     string request = "GET " + resource + " HTTP/1.1\r\nHost:" + host + "\r\nConnection: keep-alive\r\n\r\n\r\n";
     //    sprintf(strHttpPost, request.c_str());
         resource = m_Resource_Stok;
         pHttpPost = "GET %s HTTP/1.1\r\n"
                     "Host: %s\r\n\r\n";
                    // "Connection:Close\r\n";
                   //  "Accept: text/html,application/xhtml+xml,application/xml\r\n\r\n";
                 //    "Connection: keep-alive\r\n";
         sprintf(strHttpPost, pHttpPost, resource.c_str(), host.c_str());
     //    printf("qqqq");
         break;
      }
      case GET2:
         {

            //  string ttt = m_Resource_Stok;
            //  ttt = m_Resource_Stok + "/admin/system/devbase";
            //   string request = "GET " + ttt + " HTTP/1.1\r\nHost:" + host + "\r\nConnection:Close\r\n\r\n";
            //     string request = "GET " + resource + " HTTP/1.1\r\nHost:" + host + "\r\nConnection: keep-alive\r\n\r\n\r\n";
            //    sprintf(strHttpPost, request.c_str());
            resource = m_Resource_Stok + "/admin/system/firmware";
            pHttpPost = "GET %s HTTP/1.1\r\n"
               "Host: %s\r\n\r\n";
            // "Connection:Close\r\n";
            //  "Accept: text/html,application/xhtml+xml,application/xml\r\n\r\n";
            //    "Connection: keep-alive\r\n";
            sprintf(strHttpPost, pHttpPost, resource.c_str(), host.c_str());
            //    printf("qqqq");
            break;
         }
      case LOGIN:
      {
         pHttpPost = "POST %s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: %d\r\n\r\n"
            "%s";
         msg = "username=admin&password=pass";
         sprintf(strHttpPost, pHttpPost, resource.c_str(), host.c_str(), strlen(msg), msg);
         break;
      } 
   }
   if( SOCKET_ERROR ==send( sock, strHttpPost, strlen(strHttpPost), 0 ) ){
      cout << "send error" <<endl;
      closesocket( sock );
      return false;
   }

   //response get.
   int m_nContentLength = DEFAULT_PAGE_BUF_SIZE;
   char *pageBuf = (char *)malloc(m_nContentLength);
   memset(pageBuf, 0, m_nContentLength);
 
   bytesRead = 0;
   int ret = 1;
   cout <<"Read: ";
   while(ret > 0){
      ret = recv(sock, pageBuf + bytesRead, m_nContentLength - bytesRead, 0);

      if(ret > 0)
      {
         bytesRead += ret;
      }

      if( m_nContentLength - bytesRead<100){
         cout << "\nRealloc memorry"<<endl;
         m_nContentLength *=2;
         pageBuf = (char*)realloc( pageBuf, m_nContentLength); 
      }
      cout << ret <<" ";
   }
   cout <<endl;
   pageBuf[bytesRead] = '\0';
   response = pageBuf;
   closesocket( sock );
   return true;
   //cout<< response <<endl;
}

void HTMLParse ( string & htmlResponse, vector<string> & imgurls, const string & host ){
   
   const char *p= htmlResponse.c_str();
   char *tag="href=\"";
   const char *pos = strstr( p, tag );
   ofstream ofile("url.txt", ios::app);
   while( pos ){
      pos +=strlen(tag);
      const char * nextQ = strstr( pos, "\"" );
      if( nextQ ){
         char * url = new char[ nextQ-pos+1 ];
         sscanf( pos, "%[^\"]", url);
         string surl = url;
         if( visitedUrl.find( surl ) == visitedUrl.end() ){
            visitedUrl.insert( surl );
            ofile << surl<<endl;
            hrefUrl.push( surl );
         }
         pos = strstr(pos, tag );
         delete [] url;
      }
   }
   ofile << endl << endl;
   ofile.close();

   tag ="<img ";
   const char* att1= "src=\"";
   const char* att2="lazy-src=\"";
   const char *pos0 = strstr( p, tag );
   while( pos0 ){
      pos0 += strlen( tag );
      const char* pos2 = strstr( pos0, att2 );
      if( !pos2 || pos2 > strstr( pos0, ">") ) {
         pos = strstr( pos0, att1);
         if(!pos) {
            pos0 = strstr(att1, tag );
            continue;
         } else {
            pos = pos + strlen(att1);
         }
      }
      else {
         pos = pos2 + strlen(att2);
      }

      const char * nextQ = strstr( pos, "\"");
      if( nextQ ){
         char * url = new char[nextQ-pos+1];
         sscanf( pos, "%[^\"]", url);
         cout << url<<endl;
         string imgUrl = url;
         if( visitedImg.find( imgUrl ) == visitedImg.end() ){
            visitedImg.insert( imgUrl );
            imgurls.push_back( imgUrl );
         }
         pos0 = strstr(pos0, tag );
         delete [] url;
      }
   }
   cout << "end of Parse this html"<<endl;
}

string CUpdate::ToFileName( const string &url ){
   string fileName;
   struct tm szTime;
 //  char timebuffer[1024];
   char requestInfo[16];
   errno_t err;
   string s,str;
   vector<string> vs;
   
   //GetTime(&szTime);
   //time_t timer = time(NULL);
   //sprintf(timebuffer, "%d", m_REQUEST);
  /* err = asctime_s(timebuffer,26, &szTime);
   str = timebuffer;
   istringstream strcin(str);
   while(strcin >> s) vs.push_back(s); */
   sprintf(requestInfo, "%d", m_REQUEST);

   fileName.resize( url.size());
   int k=0;
   for( int i=0; i<(int)url.size(); i++){
      char ch = url[i];
      if( ch!='\\'&&ch!='/'&&ch!=':'&&ch!='*'&&ch!='?'&&ch!='"'&&ch!='<'&&ch!='>'&&ch!='|')
         fileName[k++]=ch;
   }
   string rtn = fileName.substr(0,k) + "-" +  std::string(requestInfo) + ".txt";
   return rtn;
}

/*void DownLoadImg( vector<string> & imgurls, const string &url ){

   //生成保存该url下图片的文件夹
   string foldname = ToFileName( url );
   foldname = "./img/"+foldname;
   if(!CreateDirectory( foldname.c_str(),NULL ))
      cout << "Can not create directory:"<< foldname<<endl;
   char *image;
   int byteRead;
   for( int i=0; i<imgurls.size(); i++){
      //判断是否为图片，bmp，jgp，jpeg，gif 
      string str = imgurls[i];
      int pos = str.find_last_of(".");
      if( pos == string::npos )
         continue;
      else{
         string ext = str.substr( pos+1, str.size()-pos-1 );
         if( ext!="bmp"&& ext!="jpg" && ext!="jpeg"&& ext!="gif"&&ext!="png")
            continue;
      }
      //下载其中的内容
      if( GetHttpResponse(imgurls[i], image, byteRead)){
         if ( strlen(image) ==0 ) {
            continue;
         }
         const char *p=image;
         const char * pos = strstr(p,"\r\n\r\n")+strlen("\r\n\r\n");
         int index = imgurls[i].find_last_of("/");
         if( index!=string::npos ){
            string imgname = imgurls[i].substr( index , imgurls[i].size() );
            ofstream ofile( foldname+imgname, ios::binary );
            if( !ofile.is_open() )
               continue;
            cout <<g_ImgCnt++<< foldname+imgname<<endl;
            ofile.write( pos, byteRead- (pos-p) );
            ofile.close();
         }
         free(image);
      }
   }
} */

void CUpdate::BFS( const string & url ){
   char *response;
   int bytes,pos;
   string locationInfo;

   if( !GetHttpResponse( url, response, bytes ) ){
      cout << "The url is wrong! ignore." << endl;
      return;
   }
   string httpResponse=response;
   free( response );

   if(m_REQUEST == LOGIN)
      if((pos = httpResponse.find("Location:")) != string::npos){
         int len = httpResponse.find("\r",(httpResponse.find(": ",pos))) - (httpResponse.find(": ",pos));
         m_Resource_Stok = httpResponse.substr((httpResponse.find(": ",pos))+2, len-2);
      }

   string filename = ToFileName( url );
   ofstream ofile( "./html/"+filename );
   if( ofile.is_open() ){
      
      ofile << httpResponse << endl;
      ofile.close();
   }
   vector<string> imgurls;
   
 //  HTMLParse( httpResponse,  imgurls, url );
 //  DownLoadImg( imgurls, url );
}

void CUpdate::Core()
{
   WSADATA wsaData;
   if( WSAStartup(MAKEWORD(2,2), &wsaData) != 0 ){
      return;
   }

   CreateDirectory( "./img",0);
   CreateDirectory("./html",0);
   string urlStart = "http://192.168.0.1/cgi-bin/cgi";

   m_REQUEST = LOGIN;
   BFS(urlStart);
   m_REQUEST = GET1;
   BFS(urlStart);
   m_REQUEST = GET2;
   BFS(urlStart);
/*
   // 访问过的网址保存起来
   visitedUrl.insert( urlStart );

   while( hrefUrl.size()!=0 ){
      string url = hrefUrl.front();  // 从队列的最开始取出一个网址
      cout << url << endl;
      BFS( url );					  // 遍历提取出来的那个网页，找它里面的超链接网页放入hrefUrl，下载它里面的文本，图片
      hrefUrl.pop();                 // 遍历完之后，删除这个网址
   } */
   WSACleanup();
   return;
}
