#include "nsIGenericFactory.h"
#include "nsSIP.h"
#include "nsCallLog.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(nsSIP)

NS_GENERIC_FACTORY_CONSTRUCTOR(nsCallLog)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsList)

static nsModuleComponentInfo components[] =
{
    {
       SIP_CLASSNAME, 
       SIP_CID,
       SIP_CONTRACTID,
       nsSIPConstructor,
    },
    {
       LIST_CLASSNAME, 
       LIST_CID,
       LIST_CONTRACTID,
       nsListConstructor,
    },
    {
       LOG_CLASSNAME, 
       LOG_CID,
       LOG_CONTRACTID,
       nsCallLogConstructor,
    },
};

NS_IMPL_NSGETMODULE("nsSIPModule", components) 

