//#include "../CommFunc.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "winsock2.h"
#include <time.h>
#include <queue>
#include <hash_set>

using namespace std;

class CUpdate{

public:
   CUpdate(void){m_REQUEST=NOT_DEFINE; m_Resource_Stok="";};
public:
   ~CUpdate(void){};

private:
  typedef enum DO_REQUEST{GET1, GET2, LOGIN, NOT_DEFINE};
  DO_REQUEST m_REQUEST;
  string     m_Resource_Stok;

public:
     BOOL GetTime(struct tm *time_info);
     string ToFileName( const string &url );
     BOOL GetWebResponse( const string & url, char * &response, int &bytesRead );
     BOOL GetHttpResponse( const string & url, char * &response, int &bytesRead );
     void BFS( const string & url );
     void Core();
};


