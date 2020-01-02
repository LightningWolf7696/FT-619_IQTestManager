#include "windows.h"
#include "stdio.h"
#include "stdlib.h"
#include "QMSL_Core.h"
#include "QLibDemo_WLAN_Core.h"

#include <string>

using namespace std;

class  CQLibDemo_WLAN_Sample
{
public:
   CQLibDemo_WLAN_Sample();
public:
   ~CQLibDemo_WLAN_Sample();
public:
   int chipSel;

   BOOL SetChipParameter(int var);
   BOOL SetParameter(string var); 
   BOOL QLibDemo_WLAN();

protected:
private:
};