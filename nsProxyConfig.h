#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linphone/linphonecore.h"

#include "nsCOMPtr.h"
#include "nsWeakPtr.h"
#include "nsServiceManagerUtils.h"
#include "nsComponentManagerUtils.h"
#include "nsISupports.h"
#include "nsStringAPI.h"
#include "nsMemory.h"

#include "nsXPCOMCIDInternal.h"

#include "nsISIP.h"


#define PROXYCFG_CONTRACTID "@alcacoop.it/voip/proxy-config;1"
#define PROXYCFG_CLASSNAME "SIP Proxy configuration object"
#define PROXYCFG_CID  { 0x3b88b334, 0x2836, 0x4ed5, { 0x9c, 0x5c, 0x49, 0x36, 0xdc, 0x9b, 0x73, 0xc5} }



class nsProxyConfig : public nsIProxyConfig
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPROXYCONFIG

  nsProxyConfig();

private:
  ~nsProxyConfig();

  nsCString sip_identity; 
  nsCString sip_proxy;
  nsCString sip_route;
  nsCString userid;
  nsCString password;
  long int duration;

protected:
  /* additional members */
};
