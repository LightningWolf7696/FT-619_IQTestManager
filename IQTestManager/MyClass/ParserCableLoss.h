#pragma once

#include "../CommFunc.h"


class CParserCableLoss
{
public:
	CParserCableLoss(void);
public:
	~CParserCableLoss(void);

public:
	BOOL Clean();
	BOOL SetParameter(LPSTR ParameterName,LPSTR ParameterValue);
	BOOL Test(double *dbCableLoss, LPSTR lpszRunInfo);


private:
	typedef struct _CABLELOSS
	{
		double TxPath[WIFI_CABLELOSS_ANTENNA][WIFI_CABLELOSS_MAX_ARY][WIFI_CABLELOSS_RESULT];
		double RxPath[WIFI_CABLELOSS_ANTENNA][WIFI_CABLELOSS_MAX_ARY][WIFI_CABLELOSS_RESULT];
		double GuPath[WIFI_CABLELOSS_ANTENNA][WIFI_CABLELOSS_MAX_ARY][WIFI_CABLELOSS_RESULT];
		inline _CABLELOSS()
		{
			zeroIt(TxPath);
			zeroIt(RxPath);
			zeroIt(GuPath);
		}
	}CABLELOSS, *pCABLELOSS;
	CABLELOSS	m_CableLoss;
	TCHAR		m_DestFileName[MAX_PATH];
    TCHAR       m_Rx_DestFileName[MAX_PATH];
};
