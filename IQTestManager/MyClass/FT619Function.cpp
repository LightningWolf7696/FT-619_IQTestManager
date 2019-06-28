#include "../StdAfx.h"
#include <time.h>     
#include <io.h>
#include <stdio.h>
#include <sys/types.h>

#include "FT619Function.h"

/*template <class T>
BOOL FT619::Compare(T upperLimit, T lowerLimit, LPSTR lpszRunInfo){
   
   return TRUE;
} */

void FT619::Split(const string& src, const string& separator, vector<string>& dest) 
{
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

BOOL FT619::FileConfig(LPSTR lpszFileName, LPSTR index, LPSTR div, LPSTR val, LPSTR lpszRunInfo){
   ifstream  infile(lpszFileName, ios::in);
   string   line;
   vector<string> inputContent, multiIndex, multival;
   istringstream strline(line) ;
   int pos=0, currentPos=0, offset=0; 

   FT619::Split(std::string(index), "-", multiIndex);
   FT619::Split(std::string(val), "-", multival);

   if(multiIndex.size() != multival.size())
   {
      strcpy(lpszRunInfo, "please check Config Parameters");
      return FALSE;
   }
   
   while ( getline( infile , line)) {
      strline.str(line);
      for(int i=0; i<multiIndex.size(); ++i){
         if(line.find(multiIndex.at(i)) != string::npos)
            if((pos = line.find(div)) != string::npos )
            {
               offset = line.size() - (pos+1);
               line.erase(pos+1,offset);
               line.replace(pos+1, multival.at(i).size(), multival.at(i));
              // printf("1111");
            }  
      } 
      inputContent.push_back(line);
   }
   infile.close();

   ofstream  outfile(lpszFileName, ios::out);
   for(int i=0; i< inputContent.size(); i++){
      outfile <<  inputContent[i] << endl;
   }
   outfile.close();
   return TRUE;
}
         /*   pos    = infile.tellp();
            offset = pos - (line.length() - (line.find(div)+1)) -2; 
            infile.seekp(offset); 
            infile << endl;
            currentPos = infile.tellp();
            infile.seekp(currentPos); 
         infile.write("11", 2);
         infile << "1111111" << endl;
         for(int i=0; i< (line.length() - (line.find(div)+1)); i++)
            infile << " " ;
         infile << endl;
         infile << " 22 " << endl
          int tmp,tmp2;
            tmp = line.length();
           tmp2 =  line.find(div);
            infile << "\0" << endl;
            currentPos = infile.tellp();
            printf("111666666666666666666666666666666");
      //      infile << " 22 " << endl;
            //<< endl;
            currentPos = infile.tellp();

            infile.seekp(currentPos); */  

/*BOOL FT619::RemoveIQFactParamArray(){
     IQFactParamArray.RemoveAll();
     return TRUE;
} */

int FT619::GetIQFactParamArraySize(){
   return IQFactParamArray.GetSize();
}

char *FT619::GetIQFactParamArrayDataName(int index){
   return IQFactParamArray.GetAt(index).name;
}

char *FT619::GetIQFactParamArrayDataValue(int index){
   return IQFactParamArray.GetAt(index).value;
}

IQFACT_INTERFACE_TYPE FT619::GetIQFactParamArrayDataType(int index){
   return IQFactParamArray.GetAt(index).type;
}

BOOL FT619::LoadOutputIQScriptFile(LPSTR lpszFileName, LPSTR lpszSectionName,LPSTR lpszRunInfo){

   CString strStrat;
   strStrat.Format("\t%s", lpszSectionName);
   return FileOutputParser(lpszFileName, strStrat.GetBuffer(), lpszRunInfo);	
}

BOOL FT619::ParseIQOutputParam(CString &strRaw, IQFACT_PARAMETER &stParameter, BOOL &find){

   CString strTmp, strData;
   int addr=SEARCH_NOT_FOUND, addr2=SEARCH_NOT_FOUND, addrNext=SEARCH_NOT_FOUND;

   addr = strRaw.Find("<");
   if(addr == SEARCH_NOT_FOUND) goto NOT_FOUND;
   addr2 = strRaw.Find("[");
   if(addr2 == SEARCH_NOT_FOUND) goto NOT_FOUND;
   strData = strRaw.Mid(addr+1,addr2-addr-1);
   strData.Trim();
   strCopy(stParameter.name, strData);

   addr = strRaw.Find("]");
   strData = strRaw.Mid(addr2+1,addr-addr2-1);
   strData.Trim();

   if(strData.CompareNoCase(_T("Integer")) == 0)
      stParameter.type = IQ_INTERFACE_TYPE_INT;
   else if(strData.CompareNoCase(_T("Double")) == 0)
      stParameter.type = IQ_INTERFACE_TYPE_DOUBLE;
   else if(strData.CompareNoCase(_T("String")) == 0)
      stParameter.type = IQ_INTERFACE_TYPE_STRING;
   else
      goto NOT_FOUND;

   find = TRUE;
   return TRUE;

NOT_FOUND:
   find = FALSE;
   if(addrNext == SEARCH_NOT_FOUND)
      return FALSE;
   return TRUE;
}

BOOL FT619::FileOutputParser(LPSTR lpszFileName, LPSTR lpszSection, LPSTR lpszRunInfo){

   TCHAR szMessage[DLL_INFO_SIZE]={0};
   FILE *stream;
   int FileNo;
   long streamLength, ActuallyRead;
   TCHAR *Content = NULL;
   CString strData="", strResult="";
   BOOL bFind=FALSE, bResult=FALSE;
   IQFACT_PARAMETER stParameter;
   errno_t err;

   IQFactParamArray.RemoveAll();
   if(err = fopen_s(&stream, lpszFileName,"rb") == NULL)
   {
      CString strTemp;
      int functionPos,keyWordPos,outputParamStart,outputParamEnd,len;

      FileNo = _fileno(stream);
      streamLength = _filelength(FileNo);

      Content = (TCHAR*)malloc((size_t)(streamLength+1));
      memset(Content, 0x00, (streamLength+1));
      fseek(stream, SEEK_SET, 0);
      ActuallyRead = (long)fread(Content, sizeof(TCHAR), streamLength, stream);
      fclose(stream) ;
      strData.Format("%s", Content);
      if(Content)	free(Content);

      functionPos = strData.Find(lpszSection);
      if(functionPos != -1)
      {
         keyWordPos = strData.Find("#Return Values:",functionPos);
         while(1)
         {
            outputParamStart = strData.Find("\t",keyWordPos);
            outputParamEnd   = strData.Find("\r\n",outputParamStart);
            if((outputParamStart != -1) &&(outputParamEnd   != -1))  
            {
               len = outputParamEnd - outputParamStart;
               strTemp = strData.Mid(outputParamStart,len);
               int tmp = strTemp.Find("<");
               if(tmp != -1)
               { 
                  bResult = ParseIQOutputParam(strTemp, stParameter, bFind);
                  if(bFind)	
                     IQFactParamArray.Add(stParameter);              
                  keyWordPos = outputParamEnd;
               }
               else
                  break;
            }
            else
            {
               sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Can not Find output Param");
               return FALSE;
            }
         }
      }
      else
      {
         sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Can not Find Section,[Name:%s]", lpszSection);
         return FALSE; 
      }
   }
   else
   {
      sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Can not open file,[Name:%s]", lpszFileName);
      return FALSE;
   }
   return TRUE;
}



