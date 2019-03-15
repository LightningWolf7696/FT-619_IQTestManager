#ifndef _DEMO_DEFINE_H__
#define _DEMO_DEFINE_H__
#define	DLLEXPORT	__declspec(dllexport)
typedef BOOL (CALLBACK *funcGetMessage)(LPSTR lpszRunInfo);
typedef ERRORREPORT (CALLBACK *funcGetActualReport)();
typedef BOOL (CALLBACK *funcBtGetTxPower)(BOOL bRetry, int iChannelIdx, LPSTR szAntenna, double dbExpectedPower, LPSTR szPacketType, LPSTR szPatternType, double &dbActualPower, LPSTR lpszRunInfo);

typedef BOOL (CALLBACK *funcAssignVariable)(BOOL IsNew, LPSTR ParameterName, LPSTR ParameterValue, int size);
typedef BOOL (CALLBACK *funcCallExit)();
typedef BOOL (CALLBACK *funcGetGpibID)(int &ud, int nBoardNum, int nPAddr, int nSAddr, LPSTR lpszRunInfo);
typedef BOOL (CALLBACK *funcQueryGpib)(int ud, LPSTR lpszCmd, LPSTR lpszRunInfo);
typedef BOOL (CALLBACK *funcReadGpib)(int ud, LPSTR lpszRunInfo);
typedef BOOL (CALLBACK *funcWriteGpib)(int ud, LPSTR lpszCmd, LPSTR lpszRunInfo);

extern "C" BOOL PASCAL EXPORT LOAD_GUI_INFORMATION(LPSTR lpszOP, LPSTR lpszBarcode, LPSTR lpszPN, SFCS_MODE eSfcsMode, BOOL bStopOnFailed, HWND hParentWnd, LPSTR lpszBarcode2, funcGetActualReport fGetActualReport, LPSTR lpszBarcode3, LPSTR lpszBarcode4, funcAssignVariable fAssignVariable, CString *strScanSrc, ...);

extern "C" BOOL PASCAL EXPORT SetToDutParameter(LPSTR ActionName, LPSTR ParameterName,LPSTR ParameterValue);
extern "C" BOOL PASCAL EXPORT CleanToDutParameter(LPSTR ActionName);
extern "C" BOOL PASCAL EXPORT PreShootToDutParameter(LPSTR ActionName, LPSTR lpszRunInfo);

extern "C" BOOL PASCAL EXPORT RestartDutTest();
extern "C" TM_RETURN PASCAL EXPORT CustomizeTest(funcGetMessage cbListMessage, funcGetMessage cbXmlMessage, funcGetMessage cbSfcsSpecMessage, funcGetMessage cbSfcsResultMessage, ERRORREPORT &ErrorReport, int nRetryCnt, BOOL bRetry, ...);
extern "C" BOOL PASCAL EXPORT StartFrameModulatedTx(DUT_PARAM DutParam, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT TxEvmReturn(double *evm, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT TxPowerReturn(double *power, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT TxPpmReturn(double *ppm, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT TxLoLeakageReturn(double *lo, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT TxMaskPercentReturn(double *fail_percent, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT GetTargetPower(DUT_PARAM &DutParam,LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT StartRxPacketReceive(DUT_PARAM DutParam, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT GetRxPacketCount(DUT_PARAM DutParam, double &dbRxPacketCount, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT GetTestCableLoss(double loss, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT ParserWiFiCableLossToDut(double *dbCableLoss, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT ParserWiFiCableLossToDut(double *dbCableLoss, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT BtTxReturn(BT_DUT_PARAM BtDutParam, BT_TX_RESULT *result, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT BtStartFrameModulatedTx(BT_DUT_PARAM DutParam, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT BtGetTargetPower(BT_DUT_PARAM &DutParam,LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT BtGetPowerIndex(BT_DUT_PARAM &DutParam, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT BtResetRxPacketCount(BT_DUT_PARAM DutParam, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT BtStartRxPacketReceive(BT_DUT_PARAM DutParam, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT BtStopRxPacketReceive(BT_DUT_PARAM DutParam, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT BtGetRxPacketCount(BT_DUT_PARAM DutParam, double &dbRxPacketCount, LPSTR lpszRunInfo);

#endif