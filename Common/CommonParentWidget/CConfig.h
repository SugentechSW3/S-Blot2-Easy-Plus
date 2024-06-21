#ifndef CCONFIG_H
#define CCONFIG_H

//#include <Common/Io/CSettingIo.h>
#include "Analysis/ConfigInformation.h"

class CConfig
{
public:
   explicit CConfig();
   virtual ~CConfig();

   virtual void readConfigData() = 0;
   virtual void writeConfigData() = 0;

   ConfigInformation* getConfigInstance();
};

#endif // CCONFIG_H
