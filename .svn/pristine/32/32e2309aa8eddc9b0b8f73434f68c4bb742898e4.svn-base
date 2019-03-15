#pragma once

#include "../CommFunc.h"

class CGetTargetPower
{
public:
	CGetTargetPower(void);
public:
	~CGetTargetPower(void);

public:
	BOOL Clean(LPSTR ActionName);
	BOOL SetParameter(LPSTR ActionName, LPSTR ParameterName,LPSTR ParameterValue);
	BOOL Test(LPSTR ActionName, DUT_PARAM &DutParam, LPSTR lpszRunInfo);

private:
	double m_OutputPwr;
};
