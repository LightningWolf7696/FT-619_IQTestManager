#include "../StdAfx.h"
#include "GetTargetPower.h"

CGetTargetPower::CGetTargetPower(void)
{
}

CGetTargetPower::~CGetTargetPower(void)
{

}

BOOL CGetTargetPower::Clean(LPSTR ActionName)
{
	m_OutputPwr = 0;
	return TRUE;
}
BOOL CGetTargetPower::SetParameter(LPSTR ActionName, LPSTR ParameterName,LPSTR ParameterValue)
{	
	if(strcmp(ParameterName, _T("OUTPUT_POWER")) == NULL)
		m_OutputPwr = atof(ParameterValue);

	return TRUE;
}

BOOL CGetTargetPower::Test(LPSTR ActionName, DUT_PARAM &DutParam, LPSTR lpszRunInfo)
{
	DutParam.TX_POWER_TARGET = m_OutputPwr;	
	return TRUE;
}
