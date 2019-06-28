// IQTestManager.h : main header file for the IQTestManager DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include ".\MyClass\CustomizeTest.h"
#include ".\MyClass\GetTargetPower.h"
#include ".\MyClass\ParserCableLoss.h"

// CIQTestManagerApp
// See IQTestManager.cpp for the implementation of this class
//

class CIQTestManagerApp : public CWinApp
{
public:
	CIQTestManagerApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

public:
	BOOL LoadTestManagerDll(LPSTR lpszRunInfo);
	BOOL ReadConfigStationIni();
public:
	CCustomizeTest		m_CustomizeTest;
	CGetTargetPower	m_GetTargetPower;
	CParserCableLoss	m_ParserCableLoss;

public:
	TCHAR				   m_ActionName[64];
	DUT_PARAM			m_DutParamWiFi;
	EXT_DUT_PARAM		m_ExtDutParam;
};
