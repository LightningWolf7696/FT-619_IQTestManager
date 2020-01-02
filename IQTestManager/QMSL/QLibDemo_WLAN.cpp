/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo_WLAN/QLibDemo_WLAN/QLibDemo_WLAN.cpp#6 $
 * $DateTime: 2019/01/09 22:31:09 $
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

// QLibDemo_WLAN.cpp : Defines the entry point for the console application.
//

#include "QLibDemo_WLAN.h"

CQLibDemo_WLAN_Sample::CQLibDemo_WLAN_Sample(){
   chipSel = 0;
}

CQLibDemo_WLAN_Sample::~CQLibDemo_WLAN_Sample(){}

/*BOOL CQLibDemo_WLAN_Sample::SetParameter(string var){
   chipSel = std::stoi(var);
   return TRUE;
}*/

BOOL CQLibDemo_WLAN_Sample::SetChipParameter(int var){
   if(var != 0)
      chipSel = var;
   else{
      printf("chipSel Fail.\n\n");
      return FALSE; 
   }
   return TRUE;
}

//int main(int argc, char argv[])
BOOL CQLibDemo_WLAN_Sample::QLibDemo_WLAN()
{
  // unsigned int chipSel = 0;
   char sLibraryVersion[50];

   // Get the library version
   QLIB_GetLibraryVersion(sLibraryVersion);

   printf("QLibrary Demo\n\n");
   printf("QLIB DLL Version: %s\n\n", sLibraryVersion );
   printf("11111111111");
   
//   while (chipSel == 0)
   if(chipSel != 0)
   {
     printf("\n\n");
     printf("Please select 1:qca61x4_qca65x4_qca93xx, 2:qca998x_qca999x, 3:IPQ40xx, 4:qca9984_qca9994, 5:qca988x,  6:QCA40xx, 7:AR93xx, 8:WCN39x0, 9:IPQ807x, 10:WCN36XX\n");
     //scanf("%d",&chipSel);
	 switch (chipSel)
      {
        case 1 :
			Testqca61x4_qca65x4_qca93xx();
			break;
		/*case 2 :
			Testqca998x_qca999x();
			break;
		case 3 :
			TestIPQ40xx();
			break;
		case 4 :
			Testqca9984_qca9994();
			break;
		case 5 :
			Testqca988x();
			break;
		case 6 :
			TestQCA40xx();
			break;
		case 7 :
			TestAr93xx();
			break;
		case 8 :
			TestWCN39x0();
			break;
		case 9 :
			TestIPQ807x();
			break;
		case 10 :
			TestWCN36XX();
			break; */
		default :
			break;
	 }
   }
   return TRUE;
}

