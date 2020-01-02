/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo_WLAN/QLibDemo_WLAN/QLibDemo_WLAN_qca61x4_qca65x4_qca93xx.cpp#3 $
 * $DateTime: 2019/02/07 22:59:08 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2018 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
 /******************************************************************************
 *
 * Copyright (c) 2013-2018 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#define _CRT_SECURE_NO_WARNINGS
#include "windows.h"
#include "stdio.h"
#include "stdlib.h"

#include "QMSL_Core.h"
#include "QLibDemo_WLAN_Core.h"

static char qca61x4_qca65x4_qca93xx_BINFILE[200] =      "C:\\Qualcomm\\WCN\\ProdTests\\refDesigns\\BoardData\\fakeBoardData_AR6320.bin";

static unsigned int qca61x4_qca65x4_qca93xx_CHIPID = 0x3E;
static char *qca61x4_qca65x4_qca93xx_DLLID = "qc6174";

// ======================================================================================================
/// <summary>
/// TxCal call back power measure function
/// </summary>
/// <param name="iGain">gain Index for logging</param>
/// <param name="iFreq">channel frequence in MHz</param>
/// <param name="dPowerLevel">expected measured power level</param>
/// <param name="iNumAvg">number of average for power measurements</param>
/// <param name="iTriggerType">trigger type</param>
/// <param name="iTriggerLevel">trigger level</param>
static double asyncPMCB(unsigned int iGain, unsigned int iFreq, double dPowerLevel,
   unsigned int iNumAvg, unsigned int iTriggerType, int iTriggerLevel)
{
   double power[100], pwr;
   double measuredAvgPower = 0;
   unsigned int it = 0;
   while (it < iNumAvg)
   {
      printf("[%d:%d] Please enter a power to simulate the measured power\n", iNumAvg, it);
      scanf("%lf",&pwr);
      // get measured power from your instrument for your calibration program
      power[it ++] = pwr;
      measuredAvgPower += power[it-1];
   }
   if (iNumAvg>0)
      measuredAvgPower = measuredAvgPower/iNumAvg; // average the measured iNumAvg powers;
   else
      measuredAvgPower = -999.00;
   return measuredAvgPower;
}

/// <summary>
/// TxOLPCCal
/// </summary>
/// <param name="channel">channel number</param>
/// <param name="txChain0">Tx Chain to turn on</param>
/// <param name="gainIdx">Gain Index array</param>
/// <param name="dacGain">Dac gain array</param>
/// <param name="paConfig">PA configuration array</param>
/// <param name="paConfig">PA configuration array</param>
static void TxOLPCCal(int channel, int txChain0, double measPwrDelta, int maxIt, int numMeasAvg )
{
   char buf[32], key[10];
   int i;

   int rateBitIndex0 = 16; // HT20_MCS0
   int wlanMode = 1;       // HT20_MCS0
   int pktLen0 = 1500;

   //80-WL400-13 Refer 3.2.2 Power Calibration QMSL calling sequence

   // Setting all OLPC calibration initial parameters by tlv calls
   QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_CAL);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel",_itoa(channel,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txChain0", _itoa(txChain0,buf,10));             //1(ch0),2(ch1),3(ch01)

   int txMode = 3; // Tx99
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txMode",_itoa(txMode,buf,10));                //3 = Tx99
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"rateBitIndex0", _itoa(rateBitIndex0,buf,10));   //Please refer WLAN_tlvDataRateTX, MCS 6 HT20
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"wlanMode", _itoa(wlanMode,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"pktLen0", _itoa(pktLen0,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"shortGuard", "0");           // 0: disable short GI
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"numPackets", "0");          // 0 = continuous, otherwise number of packets
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"aifsn", "0");

   int flags = 0x18;
   //if (ldpcRate)  flags |= DESC_LDPC_ENA_MASK;  // 0x00000002
   //if (dpdMode)   flags |= PAPRD_ENA_MASK;      // 0x00000004
   //if (STBCmode)  flags |= DESC_STBC_ENA_MASK;  // 0x00000001
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"flags", _itoa(flags,buf,10));

   // tpcm TX_PWR=0, FORCED_GAIN=1, TGT_PWR=2, TX_FORCED_GAIN=3, FORCED_GAINIDX=4, FORCED_TGTPWR=5
   int tpcm = 4;   // For OLPC, always force gainIndex
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"tpcm", _itoa(tpcm,buf,10));

   double targetPwr = 18.0;
   double slope=2.0;
   sprintf(buf, "%.2f", targetPwr);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"targetPower0", buf);
   sprintf(buf, "%.2f", measPwrDelta);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"measPowerDelta0", buf);
   sprintf(buf, "%d", maxIt);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"maxIteration0", buf);
   sprintf(buf, "%.2f", slope);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"gainSlope", buf);

   int gainIdx[5] = {15,4,5,0,0};
   int dacGain[5] = {-8,-8,-8,-8,-8};
   int paConfig[5] = {0,0,0,1,2};
   for (i=0; i<5; i++) {
      sprintf(key, "gainIdx%d", i);
      QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,key, _itoa(gainIdx[i],buf,10));
      sprintf(key, "dacGain%d", i);
      QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,key, _itoa(dacGain[i],buf,10));
      sprintf(key, "paConfig%d", i);
      QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,key, _itoa(paConfig[i],buf,10));
   }
   // Completed OLPC calibration initial parameters by TLV_Complete calls
   QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);

   // Start to run OLPC calibration iterations by power measurement call back
   asyncPMMessageCB pPMfunc = &asyncPMCB;
   QLIB_FTM_WLAN_Atheros_Tx_CAL(g_hResourceContext, pPMfunc, (unsigned int)numMeasAvg);

   // Pulling back OLPC calibration results.
   double measPwr[5];
   int calGainIndx[5], calDacGain[5], calPAconfig[5];
   for (i=0; i<5; i++) {
      sprintf(key, "measPower%d", i);
      QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      measPwr[i] = atof(buf);
      sprintf(key, "gainIdx%d", i);
      QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      calGainIndx[i] = atoi(buf);
      sprintf(key, "dacGain%d", i);
      QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      calDacGain[i] = atoi(buf);
      sprintf(key, "paConfig%d", i);
      QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      calPAconfig[i] = atoi(buf);
      printf("measPwr:%5.2f    gainIndx:0x%2x     dacGain:%2d     paConfig:%d\n",
         measPwr[i], calGainIndx[i], calDacGain[i], calPAconfig[i] );
   }
   QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"THERMCAL", buf);
   printf("Therm:%s\n", buf);
}

// ======================================================================================================
/// <summary>
/// TxCLPCCal
/// </summary>
/// <param name="channel">channel number</param>
/// <param name="txChain0">Tx Chain to turn on</param>
static void TxCLPCCal(int channel, int txChain0, double measPwrDelta, int maxIt, int numMeasAvg )
{
   char buf[32], key[10];
   int i;

   int rateBitIndex0 = 16; // HT20_MCS0
   int wlanMode = 1;       // HT20_MCS0
   int pktLen0 = 1500;

   //80-WL400-13 Refer 3.2.2 Power Calibration QMSL calling sequence

   // Setting all CLPC calibration initial parameters by tlv calls
   QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_CAL);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel",_itoa(channel,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txChain0", _itoa(txChain0,buf,10));             //1(ch0),2(ch1),3(ch01)

   int txMode = 8; // for CLPC
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txMode",_itoa(txMode,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"rateBitIndex0", _itoa(rateBitIndex0,buf,10));   //Please refer WLAN_tlvDataRateTX, MCS 6 HT20
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"wlanMode", _itoa(wlanMode,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"pktLen0", _itoa(pktLen0,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"shortGuard", "0");           // 0: disable short GI
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"numPackets", "0");          // 0 = continuous, otherwise number of packets
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"aifsn", "0");

   int flags = 0x18;
   //if (ldpcRate)  flags |= DESC_LDPC_ENA_MASK;  // 0x00000002
   //if (dpdMode)   flags |= PAPRD_ENA_MASK;      // 0x00000004
   //if (STBCmode)  flags |= DESC_STBC_ENA_MASK;  // 0x00000001
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"flags", _itoa(flags,buf,10));

   // tpcm TX_PWR=0, FORCED_GAIN=1, TGT_PWR=2, TX_FORCED_GAIN=3, FORCED_GAINIDX=4, FORCED_TGTPWR=5
   int tpcm = 4;   // For OLPC, always force gainIndex
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"tpcm", _itoa(tpcm,buf,10));

   double targetPwr = 20.0;
   sprintf(buf, "%.2f", targetPwr);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"targetPower0", buf);
   for (i=0; i<10; i++) {
      measPwrDelta = 0.5;
      sprintf(buf, "%.2f", measPwrDelta);
      sprintf(key, "measPowerDelta%d", i);
      QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, key, buf);
      sprintf(buf, "%d", maxIt);
      sprintf(key, "maxIteration%d", i);
      QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,key, buf);
   }
   double slope=8.0;
   sprintf(buf, "%.2f", slope);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"gainSlope", buf);
   int clpcSensitivePdadc=200;
   sprintf(buf, "%d", clpcSensitivePdadc);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"clpcSensitivePdadc", buf);

   int gainIdx0 = 17;
   int dacGain0 = 0;
   int paConfig0 = 0;
   sprintf(buf, "%d", gainIdx0);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "gainIdx0", buf);
   sprintf(buf, "%d", dacGain0);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "dacGain0", buf);
   sprintf(buf, "%d", paConfig0);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "paConfig0", buf);
   // Completed CLPC calibration initial parameters by TLV_Complete calls
   QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);

   // Start to run CLPC calibration iterations by power measurement call back
   asyncPMMessageCB pPMfunc = &asyncPMCB;
   QLIB_FTM_WLAN_Atheros_Tx_CAL(g_hResourceContext, pPMfunc, (unsigned int)numMeasAvg);

   // Pulling back CLPC calibration results.
   double measPwr[10];
   int calGainIndx[10], calDacGain[10], calPAconfig[10];
   for (i=0; i<10; i++) {
      sprintf(key, "measPower%d", i);
      QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      measPwr[i] = atof(buf);
      sprintf(key, "gainIdx%d", i);
      QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      calGainIndx[i] = atoi(buf);
      sprintf(key, "dacGain%d", i);
      QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      calDacGain[i] = atoi(buf);
      sprintf(key, "paConfig%d", i);
      QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      calPAconfig[i] = atoi(buf);
      printf("measPwr:%5.2f    gainIndx:0x%2x     dacGain:%2d     paConfig:%d\n",
         measPwr[i], calGainIndx[i], calDacGain[i], calPAconfig[i] );
   }
}

// ======================================================================================================
/// <summary>
/// XTAL cal call back freqErr measure function
/// <summary>
/// call back function for freqErr measurement used in xtal calibration
/// </summary>
/// <param name="freq">channel freq in MHz</param>
/// <param name="powerLevel">CW power level in dBm</param>
/// <param name="avgNum">number of measurements used for FreqErr average</param>
static double asyncFreqErrCB(unsigned int capValue, unsigned int freq, double powerLevel, unsigned int avgNum)
{
   double freqErr[100], fErr;
   double measurefreqErr = 0;

   unsigned int avgIteration = 0, num=0;

   while (avgIteration < avgNum)
   {
      printf("Please enter a number to simulate the measured freqErr in PPM\n");
      scanf("%lf",&fErr);
      // get measured frequence error from your instrument for your calibration program
      freqErr[avgIteration] = fErr;
      if (freqErr[avgIteration] > -999)
      {
         measurefreqErr += freqErr[avgIteration];
         num++;
      }
      avgIteration++;
   }
   if (num > 0)
      measurefreqErr = measurefreqErr / num;
   else
      measurefreqErr = -999;
   return measurefreqErr;
}

// ======================================================================================================
/// <summary>
/// XTAL Cal
/// </summary>
/// <param name="channel">channel number</param>
/// <param name="txChain0">Tx Chain to turn on</param>
static void XTALCal(int channel, int txChain0, int iXtalTargetPPM, unsigned int iXtalTolerancePPM, int numMeasAvg )
{
   char buf[32];

   //80-WL400-13 Refer 4.2.2 XTAL Calibration QMSL calling sequence

   // Setting all XTAL calibration initial parameters by tlv calls
   QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_CAL);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel",_itoa(channel,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txChain0", _itoa(txChain0,buf,10));             //1(ch0),2(ch1),3(ch01)

   int txMode = 1; // for xtal cal CW tone
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txMode",_itoa(txMode,buf,10));
   int wlanMode = 0;       // NO HT
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"wlanMode", _itoa(wlanMode,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"aifsn", "0");

   int flags = 0x18;
   //if (ldpcRate)  flags |= DESC_LDPC_ENA_MASK;  // 0x00000002
   //if (dpdMode)   flags |= PAPRD_ENA_MASK;      // 0x00000004
   //if (STBCmode)  flags |= DESC_STBC_ENA_MASK;  // 0x00000001
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"flags", _itoa(flags,buf,10));

   // tpcm TX_PWR=0, FORCED_GAIN=1, TGT_PWR=2, TX_FORCED_GAIN=3, FORCED_GAINIDX=4, FORCED_TGTPWR=5
   int tpcm = 4;   // For OLPC, always force gainIndex
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"tpcm", _itoa(tpcm,buf,10));

   int gainIdx = 10;  // user should decide this gainIdx for xtal calibraiton.
   int dacGain = 0;
   int paConfig = 0;
   sprintf(buf, "%d", gainIdx);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "gainIdx", buf);
   sprintf(buf, "%d", dacGain);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "dacGain", buf);
   sprintf(buf, "%d", paConfig);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "paConfig", buf);
   // Completed XTAL calibration initial parameters by TLV_Complete calls
   QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);

   // Start to run XTAL calibration iterations by frequence error measurement call back
   asyncCWFreqErrMessageCB pFErrfunc = &asyncFreqErrCB;
   QLIB_FTM_WLAN_Atheros_xtal_CAL(g_hResourceContext, pFErrfunc, (unsigned int)numMeasAvg);

   // Pulling back XTAL calibration results.
   double freqErrPPM;
   unsigned int capV;
   QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext, "XTAL_CAP_VALUE", buf);
   capV = atoi(buf);
   QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext, "XTAL_FREQERRPPM", buf);
   freqErrPPM = atof(buf);
   printf("cal freqErrPPM:%5.2f    CapVal:%2d\n", freqErrPPM, capV );
}

//=====================================================================================
/// <summary>
/// </summary>
static void Test_FTM_QCA_WLAN_CAL()
{
   int calType;

   printf ("=======================\n");
   printf (" 0: OLPC Cal\n");
   printf (" 1: CLPC Cal\n");
   printf (" 2: SCPC Cal    (qca61x4_qca65x4_qca93xx3.0 up)\n");
   printf (" 3: XTAL Cal\n");
   printf("Please enter to select a calibration type\n");
   scanf("%d",&calType);

   int iXtalTargetPPM=0;   // XTAL Cal Target Freq. error in PPM; default 0 PPM
   unsigned int iXtalTolerancePPM=1;   // XTAL Cal tolerable Freq. error in PPM
   int iXtalCapDelta=0; //  the delta value between the capout and capin.
   unsigned int iXtalCalSaveOption=0; // 0: not save xtal result to otp. 1: save xtal result to otp.

   if (calType==0) {      // OLPC
      //80-WL400-13 Refer 2.3.1.2 qca61x4_qca65x4_qca93xx OLPC Power Cal Initialization QMSL Calling Sequence
      int iCalPoints = 5;
      int iCalGainMode = 0;
      QLIB_FTM_WLAN_Atheros_Tx_CALIBRATE_INIT(g_hResourceContext, (unsigned char*)"cal-5p", iCalPoints, iCalGainMode);
   } else if (calType==1) {    // CLPC
      //80-WL400-13 Refer 3.2.1 qca61x4_qca65x4_qca93xx CLPC Power Cal Initialization QMSL Calling Sequence
      QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_CAL_INIT);
      QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"CalScheme", "3");
      // 3: clpc  use internal Pdet and Ext Coupler with multiple points calibration
      // 4: clpc  use Ext Pdet with FEM with multiple points calibration
      QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"CalMode", "1");
      QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"CalPoints", "10");
      QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
   } else if (calType==2) {    // SCPC
   } else if (calType==3) {    // XTAL
      //80-WL400-13 Refer 4.2.1 qca61x4_qca65x4_qca93xx XTAL Cal Initialization QMSL Calling Sequence
      QLIB_FTM_WLAN_Atheros_xtal_CAL_INIT(g_hResourceContext, iXtalTargetPPM, iXtalTolerancePPM, iXtalCapDelta, iXtalCalSaveOption, 0, 0); // both delayCapSettleMs, delayPostXtalMs are unused here.
   }
   //80-WL400-13 Refer 2.2.2 qca61x4_qca65x4_qca93xx OLPC Power Cal QMSL Calling Sequence

   int sel;
   // the following Tx setup for OLPC can be selected by user
   int channel = 2412;
   int txChain0 = 1;
   int maxIt=3;
   double measPwrDelta = 1.0;
   int numMeasAvg=1;
   int gainIdx = 60;  // Only use on TxGain cal, and IPA use 60, XPA use 40 but this is depended on board design.

   do {
      printf ("=======================\n");
      printf (" 1:channel             = %4d\n",channel);
      printf (" 2:txChain0            = %4d\n",txChain0);
      printf ("=======================\n");
      printf (" 99:Exit\n");
      printf (" 0:Start Cal\n");
      printf("Please enter selection\n");
      scanf("%d",&sel);
      switch (sel)
      {
      case  1:printf("Please enter new channel\n");
         scanf("%d",&channel);
         break;
      case  2:printf("Please enter new txChain0\n");
         scanf("%d",&txChain0);
         break;
      case  0:printf("Calibration Start\n");
         if (calType==0) // OLPC
            TxOLPCCal(channel, txChain0, measPwrDelta, maxIt, numMeasAvg);
         else if (calType==1) // CLPC
            TxCLPCCal(channel, txChain0, measPwrDelta, maxIt, numMeasAvg);
         else if (calType==3) // XTAL
            XTALCal(channel, txChain0, iXtalTargetPPM, iXtalTolerancePPM, numMeasAvg);
         break;
      case 99:
         break;
      default:printf("Invalid selection, please try again!\n");
      }
   } while (sel != 99);

    // Leave calibration state.
    QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_CAL_DONE);
    QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
}


//=====================================================================================
/// <summary>
/// Set BD Address
///
/// </summary>
static void SetBDAddress(unsigned char *addr)
{
   QLIB_FTM_WLAN_Atheros_SET_BD_ADDRESS_IN_OTP(g_hResourceContext, addr);
}

//=====================================================================================
/// <summary>
/// Get BD Address
///
/// </summary>
static void GetBDAddress(unsigned char *addr)
{
   QLIB_FTM_WLAN_Atheros_GET_BD_ADDRESS_FROM_OTP(g_hResourceContext, addr);
}

//=====================================================================================
/// <summary>
/// Set MAC Address
///
/// </summary>
static void SetMacAddress(unsigned char *addr)
{
   char buffer[128];

   //convert MAC address to a string
   sprintf(buffer, "%c%c%c%c%c%c\0", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

   //create the command to send
   QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_OTP);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_actions", "6"); //6 for NV Mac address set
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_MAC", buffer);
   QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
}

//=====================================================================================
/// <summary>
/// Get MAC Address
///
/// </summary>
static void GetMacAddress(char *buffer)
{
   //create the command to send
   QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_OTP);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_actions", "7"); //7 for NV Mac address get
   QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
   QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"",buffer);
}

//=====================================================================================
/// <summary>
/// Set Top Configuration Value
///
/// </summary>
static void setTopConfigValue(unsigned int action, unsigned int value)
{
   QLIB_FTM_WLAN_SET_TOP_CONFIG_IN_OTP(g_hResourceContext, action, value);
}

//=====================================================================================
/// <summary>
/// Get Top Configuration Value
///
/// </summary>
static void getTopConfigValue(unsigned int action, unsigned int *value)
{
   QLIB_FTM_WLAN_GET_TOP_CONFIG_FROM_OTP(g_hResourceContext, action, value);
}

//=====================================================================================
/// <summary>
/// Set BT Configuration Value
///
/// </summary>
static void setBTConfigValue(unsigned int action, unsigned int value)
{
   QLIB_FTM_WLAN_SET_BT_CONFIG_IN_OTP(g_hResourceContext, action, value);
}

//=====================================================================================
/// <summary>
/// Get BT Configuration Value
///
/// </summary>
static void getBTConfigValue(unsigned int action, unsigned int *value)
{
   QLIB_FTM_WLAN_GET_BT_CONFIG_FROM_OTP(g_hResourceContext, action, value);
}

//=====================================================================================
/// <summary>
/// Set PCIe Configuration Value
///
/// </summary>
static void setPCIeConfigValue(unsigned int action, unsigned int value)
{
   QLIB_FTM_WLAN_SET_PCIe_CONFIG_IN_OTP_STREAM(g_hResourceContext, action, value);
}

//=====================================================================================
/// <summary>
/// Get PCIe Configuration Value
///
/// </summary>
static void getPCIeConfigValue(unsigned int action, unsigned int *value)
{
   QLIB_FTM_WLAN_GET_PCIe_CONFIG_FROM_OTP_STREAM(g_hResourceContext, action, value);
}

//=====================================================================================
/// <summary>
/// Commit OTP Stream
///
/// </summary>
static void commitOTPStream(void)
{
   QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_OTP);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_actions", "2"); //2 for NV Stream Commit
   QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
}

//=====================================================================================
/// <summary>
/// Set Regulatory Domain Values
///
/// </summary>
static void setRegDomain(unsigned int *regDmn)
{
   QLIB_FTM_WLAN_Atheros_SET_OTP_REG_DOMAIN(g_hResourceContext, regDmn);
}

//=====================================================================================
/// <summary>
/// Get Regulatory Domain Values
///
/// </summary>
static void getRegDomain(unsigned int *regDmn)
{
   QLIB_FTM_WLAN_Atheros_GET_OTP_REG_DOMAIN(g_hResourceContext, regDmn);
}

//=====================================================================================
/// <summary>
/// Dump OTP contents to file
///
/// </summary>
static void dumpOTPToFile(char *filename)
{
   QLIB_FTM_WLAN_Atheros_DumpOTPToFile(g_hResourceContext, filename);
}

//=====================================================================================
/// <summary>
/// Compare OTP Contents against File
///
/// </summary>
static void CompareOTPAgainstFile(char *filename, char *resultsFilename, unsigned int *result)
{
   QLIB_FTM_WLAN_Atheros_CompareOTPAgainstFile(g_hResourceContext, filename, resultsFilename, result);
}

//=====================================================================================
/// <summary>
/// Read OTP Location (single Byte only)
///
/// </summary>
static void readOTPLocation(unsigned int address, unsigned int size, unsigned char *value)
{
   int i, j, k;
   char buffer[1024*6];

   QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_OTP);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_actions", "0"); //0 for NVMEM read
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_mem", "3");     //set NV memory type as OTP(3)
   sprintf(buffer, "%d\0", address);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_address", buffer);
   if (size > 1024)
   {
       printf("readOTPLocation size must <= 1024 !\n");
       return;
   }
   sprintf(buffer, "%d\0", size);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_size", buffer); // number of bytes.
   QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);

   if (!QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext, "", buffer))
   {
       printf("readOTPLocation error !\n");
       return;
   }

   // Transfer otp buffer from string format to user's data array.
   i=0, j=0;
   while (buffer[j] != '\0')
   {
       if ((buffer[j] == ',') || (buffer[j] == '.')) j++;
       sscanf(&buffer[j],"0x%x", &value[i]);
       i++; j+=2;
   }
   printf("readOTPLocation user buf format ===>\n");
   printf("readOTPLocation size = %d\n", i);
   for (j=0; j<i; j+=16)
   {
     printf("0x%04X: ", address + j);
     for (k=0; (k<16) && (j+k<i); k++)
        printf("0x%02X ", value[j+k]);
     printf("\n");
   }
   printf("readOTPLocation user buf format <===\n");
}

//=====================================================================================
/// <summary>
/// Write to OTP Location (single byte only)
///
/// </summary>
static void writeOTPLocation(unsigned int address, unsigned int value)
{
   char buffer[128];

   QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_OTP);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_actions", "1"); //0 for NVMEM write
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_mem", "3");     //set NV memory type as OTP(3)
   sprintf(buffer, "%d\0", address);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_address", buffer);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_size", "1"); //1 byte
   sprintf(buffer, "%d\0", value);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_value", buffer);
   QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
}

//=====================================================================================
/// <summary>
/// Prompt for MAC or BD address from user
///
/// </summary>
static int promtForAddress(unsigned char *address)
{
   char buffer[256];
   int temp;
   char delimin[] = " :"; //allow space or : as deliminter
   char *token;
   int index = 0;

   printf("Please enter address in the format 11:22:33:44:55:66\n");
   scanf("%s",&buffer);
   if(strlen((char *)buffer) != 17){
      printf("Error: address must be in the format 11:22:33:44:55:66 and must be 17 characters long\n");
      return 0;
   }
   token = strtok(buffer, delimin);
   while (token) {
      sscanf(token, "%x", &temp);
      address[index] = temp & 0xff;
      token = strtok( NULL, delimin);
      index++;
   }
   return 1;
}

//=====================================================================================
/// <summary>
/// Setup for OTP Access testing.  Exercise numerous OTP QMSL functions with user input
///
/// </summary>
static void Test_FTM_QCA_WLAN_OTP()
{
   int sel;
   unsigned char addr[6];
   unsigned int action;
   unsigned int value;
   unsigned int size;
   char buffer[2048];
   unsigned int regDmn[2];

   do {
      printf ("=======================\n");
      printf (" 1:Set BD Address\n");
      printf (" 2:Get BD Address\n");
      printf (" 3:Set Top Configuration\n");
      printf (" 4:Get Top Configuration\n");
      printf (" 5:Set BT Configuration\n");
      printf (" 6:Get BT Configuration\n");
      printf (" 7:Set PCIe Configuration\n");
      printf (" 8:Get PCIe Configuration\n");
      printf (" 9:Commit OTP Stream\n");
      printf (" 10:Set Reg Domain\n");
      printf (" 11:Get Reg Domain\n");
      printf (" 12:Set MAC Address\n");
      printf (" 13:Get MAC Address\n");
      printf (" 14:Dump OTP To File\n");
      printf (" 15:Compare DUT OTP Against File\n");
      printf (" 16:Read OTP Location\n");
      printf (" 17:Write OTP Location\n");
      printf ("=======================\n");
      printf (" 99:Exit\n");
      printf("Please enter selection\n");
      scanf("%d",&sel);
      switch (sel)
      {
      case  1:if(!promtForAddress(addr))
              {
                 printf("Error setting the BD address\n");
                 break;
              }
              SetBDAddress(addr);
              break;
      case  2:GetBDAddress(addr);
         printf("BD Address = %x:%x:%x:%x:%x:%x\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
         break;
      case  3:do {
         printf("Please enter 0:xtal settle time, 1:xtal cap in, 2:xtal cap out, 3:xtal sel, 4:SDIO VID, 5:SDIO DID\n");
         scanf("%d",&action);
              } while (action > 5);
         printf("Please enter value to apply\n");
         scanf("%x",&value);
         setTopConfigValue(action, value);
         break;
      case  4:do {
         printf("Please enter 0:xtal settle time, 1:xtal cap in, 2:xtal cap out, 3:xtal sel, 4:SDIO VID, 5:SDIO DID\n");
         scanf("%d",&action);
              } while (action > 5);
         getTopConfigValue(action, &value);
         printf("Value is set to 0x%x\n", value);
         break;
      case  5:do {
         printf("Please enter 0:PID DFU, 1:VID DFU, 2:PID FW, 3:VID FW, 4:PID HID, 5:VID HID\n");
         scanf("%d",&action);
              } while (action > 5);
         printf("Please enter value to apply\n");
         scanf("%x",&value);
         setBTConfigValue(action, value);
         break;
      case  6:do {
         printf("Please enter 0:PID DFU, 1:VID DFU, 2:PID FW, 3:VID FW, 4:PID HID, 5:VID HID\n");
         scanf("%d",&action);
              } while (action > 5);
         getBTConfigValue(action, &value);
         printf("Value is set to 0x%x\n", value);
         break;
      case 7:do {
         printf("Please enter 0:SSID, 1:SVID\n");
         scanf("%d",&action);
             } while (action > 1);
         printf("Please enter value to apply\n");
         scanf("%x",&value);
         setPCIeConfigValue(action, value);
         break;
      case  8:do {
         printf("Please enter 0:SSID, 1:SVID\n");
         scanf("%d",&action);
              } while (action > 1);
         getPCIeConfigValue(action, &value);
         printf("Value is set to 0x%x\n", value);
         break;
      case  9:commitOTPStream();
         break;
      case 10:printf("Please enter regulatory Domain[0]\n");
         scanf("%x",&regDmn[0]);
         regDmn[0]&=0xffff;
         printf("Please enter regulatory Domain[1]\n");
         scanf("%x",&regDmn[1]);
         regDmn[1]&=0xffff;
         setRegDomain(regDmn);
         break;
      case  11:regDmn[0]=0; regDmn[1]=0;
         getRegDomain(regDmn);
         printf("Value of regDmn[0]=0x%x. Value of regDmn[1]=0x%x\n", regDmn[0], regDmn[1]);
         break;
      case  12:if(!promtForAddress(addr))
               {
                  printf("Error setting the MAC address\n");
                  break;
               }
               SetMacAddress(addr);
               break;
      case  13:GetMacAddress(buffer);
         printf("MAC Address = %s\n", buffer);
         break;
      case  14:dumpOTPToFile(OTP_DUMP_FILE);
         break;
      case  15:CompareOTPAgainstFile(OTP_DUMP_FILE, OTP_RESULTS_FILE, &value);
         if(value==0) {
            printf("Compare Passed!\n");
         } else {
            printf("Compare FAILED! Refer to %s for more information\n", OTP_RESULTS_FILE);
         }
         break;
      case 16:printf("Please enter OTP location to read\n");
         scanf("%x",&action);
         printf("Please enter number of bytes of OTP to read\n");
         scanf("%d",&size);
         readOTPLocation(action, size, (unsigned char *)buffer);
         //printf("Value read 0x%x\n", value);
         break;
      case 17:printf("Please enter OTP location to write to\n");
         scanf("%x",&action);
         printf("Please enter the byte to write to this location\n");
         scanf("%x",&value);
         writeOTPLocation(action, (value&0xff));
         break;
      case 99:
         break;
      default:printf("Invalid selection, please try again!\n");
      }
   } while (sel != 99);
}

// =========
static bool LoadDut(char *dllID, char*bin_file, unsigned int chipID)
{
   int LoadOption=0;

   if (g_hResourceContext == NULL)
      return false;

   QLIB_SetLibraryMode(QLIB_LIB_MODE_QPHONEMS);
   QLIB_SetTargetType((unsigned char)QLIB_TARGET_TYPE_APQ);

   unsigned char bRet = QLIB_FTM_WLAN_Atheros_LoadDUT(g_hResourceContext,(unsigned char  *)dllID,(unsigned char *)bin_file, 5, chipID);
   return (bRet ? true : false);
}


// =========
static void CloseDut()
{
   QLIB_FTM_WLAN_Atheros_UNLoadDUT(g_hResourceContext);
}

// =========
static void StopTx(int channel)
{
   char buf[32];

   QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_TX);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel",_itoa(channel,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txMode","0");   //0 = xmit off
   QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
}

// =========
static void StartTx(int txMode,int channel,int rateBitIndex0,int wlanMode,int pktLen0,int txChain0,int shortGuard,int numPackets,int broadcast,int flags,int gainIdx,int dacGain,int paConfig,int tpcm,int txPower0, int aifsn)
{
   char buf[32];

   StopTx(channel);

   QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_TX);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel",_itoa(channel,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txMode",_itoa(txMode,buf,10));                  //3 = Tx99
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"rateBitIndex0", _itoa(rateBitIndex0,buf,10));   //Please refer WLAN_tlvDataRateTX, MCS 6 HT20
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"wlanMode", _itoa(wlanMode,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"pktLen0", _itoa(pktLen0,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txChain0", _itoa(txChain0,buf,10));             //1(ch0),2(ch1),3(ch01)
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"shortGuard", _itoa(shortGuard,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"numPackets", _itoa(numPackets,buf,10));         //0 = continuous, otherwise number of packets
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"broadcast",broadcast ? "true" :"False");

   //flags = 0x18;
   //if (ldpcRate)  flags |= DESC_LDPC_ENA_MASK;  // 0x00000002
   //if (dpdMode)   flags |= PAPRD_ENA_MASK;      // 0x00000004
   //if (STBCmode)  flags |= DESC_STBC_ENA_MASK;  // 0x00000001
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"flags", _itoa(flags,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "txPower0", _itoa(txPower0, buf, 10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "aifsn", _itoa(aifsn, buf, 10));

   // tpcm TX_PWR=0, FORCED_GAIN=1, TGT_PWR=2, TX_FORCED_GAIN=3, FORCED_GAINIDX=4, FORCED_TGTPWR=5
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"tpcm", _itoa(tpcm,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"gainIdx", _itoa(gainIdx,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"dacGain", _itoa(dacGain,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"paConfig", _itoa(paConfig,buf,10));

   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"agg", "1");
   //QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"dutyCycle", "10");

   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
}

// =========
static void StopTxWithReport()
{
   char strData[128];

   //80-WL400-11 Refer Section "Steps to do WLAN RF transmit tests"
   //            Refer Table "QRCT steps to perform the WLAN transmit test"
   // Execute _OP_TX_STATUS will stop current RX
   QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_TX_STATUS);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"needReport","1");
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"stopTx",_itoa(1,strData,10));
   QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);

   printf ("=============================\n");
   QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"goodPackets",strData);

   printf("Tx Report : totalPkt   = %4s\n",strData);
   QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"rssi",strData);
   printf("Tx Report : rssi       = %4s\n",strData);
}

// =========
static void Test_FTM_QCA_WLAN_TX()
{
   // Refer Section Tx test configuration in 2.4G
   //   Tx test configuration in 5G (HT20)
   //   Tx test configuration in 5G (HT40)
   int sel;
   int txMode = 3;
   int channel = 2412;
   int rateBitIndex0 = 22;  //MCS 6, 20MHz
   int wlanMode = 1; // HT20
   int pktLen0 = 1500;
   int txChain0 = 1;
   int shortGuard = 0;
   int numPackets = 0;
   int broadcast = 1;  // true, false
   int flags = 24;
   int tpcm = 4;
   int gainIdx = 5;
   int dacGain = 0;
   int paConfig = 1;
   int dutyCycle = 10;
   int txPower0 = 5;
   int txPattern = 0;
   int aifsn = 0;

   do {
      printf ("=======================\n");
      printf (" 1:txMode        = %4d\n",txMode);
      printf (" 2:channel       = %4d\n",channel);
      printf (" 3:rateBitIndex0 = %4d\n",rateBitIndex0);
      printf (" 4:wlanMode      = %4d\n",wlanMode);
      printf (" 5:pktLen0       = %4d\n",pktLen0);
      printf (" 6:txChain0      = %4d\n",txChain0);
      printf (" 7:shortGuard    = %4d\n",shortGuard);
      printf (" 8:numPackets    = %4d\n",numPackets);
      printf (" 9:broadcast     = %4d\n",broadcast);
      printf ("10:gainIdx       = %4d\n",gainIdx);
      printf ("11:dacGain       = %4d\n",dacGain);
      printf ("12:paConfig      = %4d\n",paConfig);
      printf ("13:tcpm          = %4d\n",tpcm);
      printf ("14:dutyCycle     = %4d\n",dutyCycle);
      printf ("15:txPower0      = %4d\n",txPower0);
      printf ("16:flags         = %4d, i.e.0x%x\n",flags,flags);
      printf ("17:txPattern     = %4d\n",txPattern);
      printf ("18:aifsn         = %4d\n",aifsn);
      printf ("=======================\n");
      printf (" 0:Start TX, 99:Exit\n");
      printf("Please enter selection (decimal)\n");
      scanf("%d",&sel);
      switch (sel)
      {
      case  1:printf("Please enter new txMode\n");
         scanf("%d",&txMode);
         break;
      case  2:printf("Please enter new channel\n");
         scanf("%d",&channel);
         break;
      case  3:printf("Please enter new rateBitIndex0\n");
         scanf("%d",&rateBitIndex0);
         break;
      case  4:printf("Please enter new wlanMode\n");
         scanf("%d",&wlanMode);
         break;
      case  5:printf("Please enter new pktLen0\n");
         scanf("%d",&pktLen0);
         break;
      case  6:printf("Please enter new txChain0\n");
         scanf("%d",&txChain0);
         break;
      case  7:printf("Please enter new  shortGuard\n");
         scanf("%d",&shortGuard);
         break;
      case  8:printf("Please enter new numPackets\n");
         scanf("%d",&numPackets);
         break;
      case  9:printf("Please enter new broadcast\n");
         scanf("%d",&broadcast);
         break;
      case 10:printf("Please enter new gainIdx\n");
         scanf("%d",&gainIdx);
         break;
      case 11:printf("Please enter new dacGain\n");
         scanf("%d",&dacGain);
         break;
      case 12:printf("Please enter new paConfig\n");
         scanf("%d",&paConfig);
         break;
      case 13:printf("Please enter new tpcm (TX_PWR=0, FORCED_GAIN=1, TGT_PWR=2, TX_FORCED_GAIN=3, FORCED_GAINIDX=4,FORCED_TGTPWR=5)\n");
         scanf("%d",&tpcm);
         break;
       case 14:printf("Please enter new dutyCycle\n");
         scanf("%d",&dutyCycle);
         if(dutyCycle > 100){
             printf("Error!!! Please enter correct dutyCycle!!!\n");
             dutyCycle = 99;
         }
         break;
      case 15:printf("Please enter new txPower0\n");
         scanf("%d",&txPower0);
         break;
      case 16:printf("Please enter new flags\n");
         scanf("%d",&flags);
         break;
      case 17:printf("Please enter new txPattern\n");
         scanf("%d",&txPattern);
         break;
      case 18:printf("Please enter new aifsn\n");
         scanf("%d",&aifsn);
         break;
     case  0: StartTx(txMode,channel,rateBitIndex0,wlanMode,pktLen0,txChain0,shortGuard,numPackets,broadcast,flags,gainIdx,dacGain,paConfig,tpcm,txPower0, aifsn);
         {
            int txSel;
            do {
               printf("Please enter 1:Stop Tx, 2: Stop Tx and Get Tx Report, 3:Continue Tx\n");
               scanf("%d",&txSel);
               switch (txSel)
               {
               case 1: StopTx(channel);
                  break;
               case 2: StopTxWithReport();
                  break;
               case 3: // do nothing
                  break;
               default:printf("Invalid selection, please try again.\n");
               }
            } while ((txSel < 1) || (txSel > 3));
         }
         break;
      case 99:
         break;
      default:printf("Invalid selection, please try again!\n");
      }
   } while (sel != 99);
}

// =========
static void StartRx(int channel, int rxMode, int rateMask, int rxChain, int wlanMode)
{
   char buf[32];

   QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_RX);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel",_itoa(channel,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"rxMode",_itoa(rxMode,buf,10));        //1 = filter mode
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"rateMask", _itoa(rateMask,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"rxChain", _itoa(rxChain,buf,10));     //1(ch0),2(ch1),3(ch01)
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"wlanMode", _itoa(wlanMode,buf,10));   //Refer TCMD_WLAN_MODE

   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"rateBitIndex0", _itoa(rateMask,buf,10));
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"flags", _itoa(0x30,buf,10));

   QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
}

// =========
static void StopRxWithReport()
{
   char strData[128];

   QLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_RX_STATUS);
   QLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"stopRx","1");
   QLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);

   printf ("==============================\n");
   QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"totalPkt",strData);
   printf("Rx Report : totalPkt    = %4s\n",strData);
   QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"goodPackets",strData);
   printf("Rx Report : goodPackets = %4s\n",strData);
   QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"crcErrPkt",strData);
   printf("Rx Report : crcErrPkt   = %4s\n",strData);
   QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"secErrPkt",strData);
   printf("Rx Report : secErrPkt   = %4s\n",strData);
   QLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"rssi",strData);
   printf("Rx Report : rssi        = %4s\n",strData);
}

// =========
static void Test_FTM_QCA_WLAN_RX()
{
   int sel;

   int channel = 2412;
   int channel2 = 0;
   int rxMode = 0;
   int rateMask = 15;  // MCS 0
   int rxChain = 1;
   int wlanMode = 0;

    do {
      printf ("=======================\n");
      printf (" 1:rxMode        = %4d\n",rxMode);
      printf (" 2:channel       = %4d\n",channel);
      printf (" 3:rateMask      = %4d\n",rateMask);
      printf (" 4:wlanMode      = %4d\n",wlanMode);
      printf (" 5:rxChain       = %4d\n",rxChain);
      printf ("=======================\n");
      printf (" 0:Start RX, 99:Exit\n");
      printf("Please enter selection\n");
      scanf("%d",&sel);
      switch (sel)
      {
      case  1:printf("Please enter new rxMode\n");
         scanf("%d",&rxMode);
         break;
      case  2:printf("Please enter new channel\n");
         scanf("%d",&channel);
         break;
      case  3:printf("Please enter new rateMask\n");
         scanf("%d",&rateMask);
         break;
      case  4:printf("Please enter new wlanMode\n");
         scanf("%d",&wlanMode);
         break;
      case  5:printf("Please enter new rxChain\n");
         scanf("%d",&rxChain);
         break;
      case  0:StartRx(channel, rxMode, rateMask, rxChain, wlanMode);
         int txSel;
         do {
            printf("Please enter 1:Stop Rx and Get Rx Report, 2:Continue Rx\n");
            scanf("%d",&txSel);
            switch (txSel)
            {
            case 1: StopRxWithReport();
               break;
            case 2: // do nothing
               break;
            default:printf("Invalid selection, please try again.\n");
            }
         } while ((txSel < 1) || (txSel > 2));
         break;
      case 99:
         break;
      default:printf("Invalid selection, please try again!\n");
      }
   } while (sel != 99);
}

// =========
static void Test_FTM_QCA_WLAN()
{
   int sel;

   do
   {
      printf("\n===============================\n");
      printf("Please enter 1:TX, 2:RX, 3:OTP, 4:Tx Calibration, 99:Exit\n");
      scanf("%d",&sel);

      switch (sel)
      {
      case 1 :Test_FTM_QCA_WLAN_TX();
         break;
      case 2 :Test_FTM_QCA_WLAN_RX();
         break;
      case 3 :Test_FTM_QCA_WLAN_OTP();
         break;
      case 4 :Test_FTM_QCA_WLAN_CAL();
         break;
      case 99 :// do nothing
         break;
      default:
         printf("Invalid selection! Please 1, 2 or 99\n");
      }
   } while (sel != 99);
}


// =========
void Testqca61x4_qca65x4_qca93xx()
{

   Set_UDT_IP("192.168.1.10"); // run with Qcmbr.exe at local host.
   if (!ConnectStandaloneWlanCard(TYPE_IP))
   {
	   printf("Connect QCMBR failed!\n");
	   return;
   }

   if (!LoadDut(qca61x4_qca65x4_qca93xx_DLLID , qca61x4_qca65x4_qca93xx_BINFILE, qca61x4_qca65x4_qca93xx_CHIPID))
   {
      printf("LoadDut() failed!\n");
      DisconnectStandaloneWlanCard(TYPE_IP);
      return;
   }
   SetTLV1();

   Test_FTM_QCA_WLAN();

   CloseDut();
   DisconnectStandaloneWlanCard(TYPE_IP);

}
