#include "nsIGenericFactory.h"
#include "nsSIP.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(nsSIP)

static nsModuleComponentInfo components[] =
{
    {
       SIP_CLASSNAME, 
       SIP_CID,
       SIP_CONTRACTID,
       nsSIPConstructor,
    }
};

NS_IMPL_NSGETMODULE("nsSIPModule", components) 

