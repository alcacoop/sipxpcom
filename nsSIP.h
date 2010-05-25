#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pjsip.h"
#include "nsCOMPtr.h"
#include "nsIArray.h"
#include "nsIMutableArray.h"
#include "nsTArray.h"
#include "nsVoidArray.h"
#include "nsWeakPtr.h"
#include "nsServiceManagerUtils.h"
#include "nsComponentManagerUtils.h"
#include "nsIProxyObjectManager.h"
#include "nsISupports.h"
#include "nsXPCOMCIDInternal.h"
#include "pjsip.h"

#include "nsISIP.h"

#define SIP_CONTRACTID "@alcacoop.it/sip;1"
#define SIP_CLASSNAME "Simple XPCOM SIP stack"
#define SIP_CID  { 0xc5bf5079, 0x8bc3, 0x4fc5, { 0x88, 0x1f, 0xf3, 0xbd, 0x85, 0xa6, 0x3c, 0x61 } }


class nsSIP : public nsISIP
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISIP
  nsSIP();
  
private:
  ~nsSIP();
  void CallObservers(const char*);

protected:
  long port;
  nsCOMPtr<nsIMutableArray> mObservers;
  nsCOMPtr<nsIMutableArray> proxy;
};
