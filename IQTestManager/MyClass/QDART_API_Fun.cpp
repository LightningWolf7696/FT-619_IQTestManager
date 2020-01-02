#include "../StdAfx.h"
#include <time.h>     
#include <io.h>
#include <stdio.h>
#include <sys/types.h>
#include "QDART_API_Fun.h"


string Mapping_Rate_Data[7][10]=
{
   {"CCK_1M","CCK_2M","CCK_5.5M","CCK_11M"},
   {"OFDM_6M","OFDM_9M","OFDM_12M","OFDM_18M","OFDM_24M","OFDM_36M","OFDM_48M","OFDM_54M"},
   {"N20_MCS0","N20_MCS1","N20_MCS2","N20_MCS3","N20_MCS4","N20_MCS5","N20_MCS6","N20_MCS7"},
   {"N40_MCS0","N40_MCS1","N40_MCS2","N40_MCS3","N40_MCS4","N40_MCS5","N40_MCS6","N40_MCS7"},
   {"AC20_MCS0","AC20_MCS1","AC20_MCS2","AC20_MCS3","AC20_MCS4","AC20_MCS5","AC20_MCS6","AC20_MCS7","AC20_MCS8","AC20_MCS9"},
   {"AC40_MCS0","AC40_MCS1","AC40_MCS2","AC40_MCS3","AC40_MCS4","AC40_MCS5","AC40_MCS6","AC40_MCS7","AC40_MCS8","AC40_MCS9"},
   {"AC80_MCS0","AC80_MCS1","AC80_MCS2","AC80_MCS3","AC80_MCS4","AC80_MCS5","AC80_MCS6","AC80_MCS7","AC80_MCS8","AC80_MCS9"}
};

CQDART_APIFun::CQDART_APIFun(void)
{
   TX_RATE.clear();
   zeroIt(m_DestFileName);
   zeroIt(m_Mode);
   zeroIt(m_Index);
   zeroIt(m_Key);
}

CQDART_APIFun::~CQDART_APIFun(void)
{
   
}

BOOL CQDART_APIFun::SetParameter(LPSTR ParameterName,LPSTR ParameterValue)
{
   char szMessage[DLL_INFO_SIZE]={'\0'};
   char szLocalPath[DLL_INFO_SIZE]={'\0'};
   GetLocalDirectory(szLocalPath, szMessage);

   if(strcmp(ParameterName, _T("DEST_FILE")) == NULL)
   {  
      CString strData, fileName;
      char var[MAX_PATH];
      int pos;

      if(strncmp(ParameterValue, _T(".\\"),2) == 0)
      {
         sprintf_s(m_DestFileName,sizeof(m_DestFileName),"%s%s",szLocalPath, &ParameterValue[2]);
         strData.Format("%s", m_DestFileName);
      }   
      else
      {
         strcpy_s(m_DestFileName,sizeof(m_DestFileName) ,ParameterValue);
         strData.Format("%s", m_DestFileName);
      } 
   }else if(strcmp(ParameterName, _T("MODE")) == NULL){
      if(strcmp(ParameterValue, _T("PARSER")) == NULL)
         m_Mode = PARSER;
      else
         m_Mode = MAPPING;
   }  
   else if(strcmp(ParameterName, _T("INDEX")) == NULL)
      strcpy_s(m_Index,sizeof(m_Index) ,ParameterValue);
   return TRUE;
}

void CQDART_APIFun::Split(const string& src, const string& separator, vector<string>& dest) {

   string str = src;
   string substring;
   string::size_type start = 0, index;
   dest.clear();
   index = str.find_first_of(separator,start);
   do
   {
      if (index != string::npos)
      {    
         substring = str.substr(start,index-start );
         dest.push_back(substring);
         start =index+separator.size();
         index = str.find(separator,start);
         if (start == string::npos) break;
      }
   }while(index != string::npos);

   substring = str.substr(start);
   dest.push_back(substring);
}

BOOL CQDART_APIFun::Mapping2INI(const string& src, LPSTR lpszRunInfo){

   int pos=0, end=0;
   string tmpInfo;
   vector<string> lineInfo;
   RateData szRateData;

   pos = src.find("_wlanRate__RATE");
   end = src.find("_ ",pos);
   tmpInfo = src.substr(pos,(end-pos));
   Split(tmpInfo, "_", lineInfo);

   if(lineInfo.at(4).empty() && lineInfo.at(5).empty() && lineInfo.at(6).empty()){
      sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Mapping rate info fail.");
      return FALSE;
   }
   else{
      szRateData.BAND      = lineInfo.at(4);
      szRateData.WIDE_BAND = lineInfo.at(5);
      szRateData.RATE      = lineInfo.at(6);
      TX_RATE.push_back(szRateData);
   }
   return TRUE;
}

BOOL CQDART_APIFun::FileParser(ifstream &file, string line, vector<string>& fout){
   
   fout.push_back(line);
   while ( getline( file, line)){
      int pos = line.find("	");
      int end = line.rfind(",");
      fout.push_back(line.substr(pos, (end - pos)));
      if((line.find("TEST_END")) != string::npos)
         break;
   }
   return TRUE;
}

BOOL CQDART_APIFun::FileHandle(LPSTR lpszRunInfo){

   string line,tmpInfo;
   vector<string> cmdInfo,indexInfo,keyInfo;
   int  pos=0, end=0,offset=0;
   BOOL FLAG=TRUE;
   char szMessage[DLL_INFO_SIZE]={'\0'};

   //Split(m_Index, "-", indexInfo);
   //Split(m_Key, "-", keyInfo);

   ifstream  infile(m_DestFileName, ios::in);
   if(!infile.is_open())
   {
      sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "File Can't open.");
      return FALSE;
   }
   istringstream strline(line);
   switch(m_Mode)
   {
      case PARSER:
      {
         if(strlen(m_Index) != 0){
            while ( getline( infile, line)){
               strline.str(line);
               for(int i=0; i<indexInfo.size(); i++){
                  if((line.find(indexInfo[i]) != string::npos) && (line.find("TEST_BEGIN") != string::npos))
                  {
                     
                  }
               }
            }
         }else{
            while ( getline( infile, line)){
               if(line.find("TEST_BEGIN") != string::npos)
                  FileParser(infile, line, cmdInfo);
            }
         }
         break;
      }
      case MAPPING:
      default:
         sprintf_s(szMessage, sizeof(szMessage), "Test mode not support, [Mode : %s]", m_Mode);
         return FALSE;
   }   
   
   ofstream  outfile("QART_API_PARSER_INFO", ios::out);
   if(!outfile.is_open())
   {
      sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Output File Can't open.");
      return FALSE;
   }
   for(int i=0; i< cmdInfo.size(); i++){
      outfile <<  cmdInfo[i] << endl;
   }

   return TRUE;
}