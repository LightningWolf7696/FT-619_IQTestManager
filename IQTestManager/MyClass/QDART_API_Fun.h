#include "../CommFunc.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

typedef enum BAND{CCK,OFDM,N_HT20,N_HT40,AC_VHT20,AC_VHT40,AC_VHT80};
typedef enum MODE{PARSER, MAPPING};

class CQDART_APIFun{
public:
   CQDART_APIFun(void);
public:
   ~CQDART_APIFun(void);
private:
   typedef struct _RateData{
      string BAND;
      string WIDE_BAND;
      string RATE;
   }RateData;
    typedef struct _QDARTAPI{
      string key;
      string data;
    }QDARTAPI;

    char m_DestFileName[MAX_PATH];
    char m_Index[MAX_PATH];
    char m_Key[MAX_PATH];
    vector<RateData> TX_RATE;
    MODE m_Mode;
    BAND m_BAND;
   
  /*  vector<QDARTData> RX_RATE;
    vector<QDARTData> CHAIN_MASK;
    vector<QDARTData> MODULATION;*/
public:
	BOOL SetParameter(LPSTR ParameterName,LPSTR ParameterValue);
   void Split(const string& src, const string& separator, vector<string>& dest);
   BOOL Mapping2INI(const string& src, LPSTR lpszRunInfo);
   BOOL FileParser(ifstream &file, string line, vector<string>& fout);
   BOOL FileParser(ifstream &file, string line, string index, vector<string>& fout);
   BOOL FileHandle(LPSTR lpszRunInfo);
};


