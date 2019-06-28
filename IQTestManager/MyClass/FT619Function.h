#pragma once
#include "../CommFunc.h"
#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class FT619{
public:
   FT619(){
      IQFactParamArray.RemoveAll();
   }
public:
    ~FT619(void){};

private:
   typedef struct _IQFACT_PARAMETER
   {
      TCHAR	name[DLL_INFO_SIZE];
      TCHAR	value[DLL_INFO_SIZE];
      IQFACT_INTERFACE_TYPE type;
   }IQFACT_PARAMETER;

   CArray <IQFACT_PARAMETER, IQFACT_PARAMETER> IQFactParamArray;
   funcGetMessage	m_cbListMessage;

public:
   BOOL LoadOutputIQScriptFile(LPSTR lpszFileName, LPSTR lpszSectionName,LPSTR lpszRunInfo);
   BOOL ParseIQOutputParam(CString &strRaw, IQFACT_PARAMETER &stParameter, BOOL &find);
   BOOL FileOutputParser(LPSTR lpszFileName, LPSTR lpszSection,LPSTR lpszRunInfo);
   BOOL Compare(int hight, int low, LPSTR lpszRunInfo);
   int  GetIQFactParamArraySize();
   char *GetIQFactParamArrayDataName(int index);
   char *GetIQFactParamArrayDataValue(int index);
   IQFACT_INTERFACE_TYPE GetIQFactParamArrayDataType(int index);
   void Split(const string& src, const string& separator, vector<string>& dest);
   BOOL FileConfig(LPSTR lpszFileName, LPSTR key,  LPSTR div, LPSTR Val, LPSTR lpszRunInfo);
};