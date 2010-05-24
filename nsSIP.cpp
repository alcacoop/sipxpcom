#include "nsCOMPtr.h"
#include "nsServiceManagerUtils.h"
#include "nsIProxyObjectManager.h"
#include "nsISupports.h"
#include "nsXPCOMCIDInternal.h"

#include "nsSIP.h"
#include "pjsip.h"


NS_IMPL_ISUPPORTS1(nsSIP, nsISIP)

nsCOMPtr<nsSipStateObserver> nsSIP::observer = nsnull;
nsCOMPtr<nsSipStateObserver> nsSIP::proxy = nsnull;


nsSIP::nsSIP() {
  port = 0;
}

nsSIP::~nsSIP() {
  /* destructor code */
}


/* void init (in long port); */
NS_IMETHODIMP nsSIP::Init(PRInt32 port, nsSipStateObserver *cbk) {
  if (nsSIP::port!=0){
    Destroy();
  }

  nsSIP::port = port;

  nsSIP::observer = cbk;
  nsresult rv = NS_OK;
  nsCOMPtr<nsIProxyObjectManager> pIProxyObjectManager(do_GetService("@mozilla.org/xpcomproxy;1", &rv));
  if(NS_FAILED(rv)) return rv;

  nsCOMPtr<nsSipStateObserver> pProxy;
  rv = pIProxyObjectManager->GetProxyForObject(
      NS_PROXY_TO_MAIN_THREAD,
      nsSipStateObserver::GetIID(),
      //NS_GET_IID(nsSipStateObserver),
      nsSIP::observer,
      NS_PROXY_SYNC | NS_PROXY_ALWAYS,
      getter_AddRefs(pProxy));
  if(NS_FAILED(rv)) return rv;

  nsSIP::proxy=pProxy;
  sipregister((int)port, proxy);
  nsSIP::proxy->OnStatusChange("INIT");
  return NS_OK;
}


/* void destroy (); */
NS_IMETHODIMP nsSIP::Destroy() {
  sipderegister();
  nsSIP::proxy->OnStatusChange("DESTROY");
  port = 0;
  return NS_OK;
}


/* void call (in AString URI); */
NS_IMETHODIMP nsSIP::Call(const char* URI) {
  sipmakecall((char*)URI);
  return NS_OK;
}


/* void hangup (); */
NS_IMETHODIMP nsSIP::Hangup() {
  siphangup();
  return NS_OK;
}

