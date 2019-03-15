#include "../StdAfx.h"
#include "ParserCableLoss.h"


CParserCableLoss::CParserCableLoss(void)
{
}

CParserCableLoss::~CParserCableLoss(void)
{

}

BOOL CParserCableLoss::Clean()
{
	zeroIt(m_DestFileName);
	return TRUE;
}
BOOL CParserCableLoss::SetParameter(LPSTR ParameterName,LPSTR ParameterValue)
{	
	if(strcmp(ParameterName, _T("DEST_FILE")) == NULL)
	{
		if(strncmp(ParameterValue, _T(".\\"),2) == 0)
			sprintf_s(m_DestFileName,sizeof(m_DestFileName),"%s%s",g_GlobalInfo.szLocalPath, &ParameterValue[2]);
		else
			strcpy_s(m_DestFileName,sizeof(m_DestFileName) ,ParameterValue);
	}

	return TRUE;
}

BOOL CParserCableLoss::Test(double *dbCableLoss, LPSTR lpszRunInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	TCHAR szMessage[DLL_INFO_SIZE]={0};	
	FILE *stream = NULL;

	memcpy(m_CableLoss.TxPath,&dbCableLoss[0],sizeof(m_CableLoss.TxPath));
	memcpy(m_CableLoss.RxPath,&dbCableLoss[WIFI_CABLELOSS_ANTENNA*WIFI_CABLELOSS_MAX_ARY*WIFI_CABLELOSS_RESULT],sizeof(m_CableLoss.RxPath));
	memcpy(m_CableLoss.GuPath,&dbCableLoss[WIFI_CABLELOSS_ANTENNA*WIFI_CABLELOSS_MAX_ARY*WIFI_CABLELOSS_RESULT*2],sizeof(m_CableLoss.GuPath));

	if(CheckFileExist(m_DestFileName,szMessage)) DeleteFile(m_DestFileName);

	if((stream = fopen(m_DestFileName, "w")) == NULL)
	{
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE, "Can't parser cableLoss from DUT.[Name:%s]", m_DestFileName);
		return FALSE;
	}
	for (int i=0; i < (WIFI_CABLELOSS_ANTENNA*WIFI_CABLELOSS_MAX_ARY*WIFI_CABLELOSS_RESULT); i++)
	{
		char szCableLoss[64]={0};
		double dbAntLoss[4]={0};
		double dbFreq;
		if((dbFreq = m_CableLoss.TxPath[0][i][0]) == 0) break;		
		for(int j=0 ; j<4 ; j++)
			dbAntLoss[j] = m_CableLoss.TxPath[j][i][1];
		fprintf(stream,"%.0lf,%.1lf,%.1lf,%.1lf,%.1lf\n",dbFreq,dbAntLoss[0],dbAntLoss[1],dbAntLoss[2],dbAntLoss[3]);			
	}
	fclose(stream);
	strcpy_s(lpszRunInfo,DLL_INFO_SIZE,"Success to ParserCableLoss from DUT.");
	return TRUE;
}

