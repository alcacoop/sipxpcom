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
//nsCOMPtr<nsSipStateObserver> nsSIP::proxy = nsnull;


nsSIP::nsSIP() : mObservers(nsnull) {
  port = 0;
}

nsSIP::~nsSIP() {
  /* destructor code */
  if (mObservers) {
    //(void)mObservers->EnumerateForwards(deleteObserver, nsnull);
    delete mObservers;
  }

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

  callObservers("INIT");
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

/* void addObserver (in nsSipStateObserver cbk); */
NS_IMETHODIMP nsSIP::AddObserver(nsSipStateObserver *cbk)
{
  nsCOMPtr<nsSipStateObserver> ref;

  NS_ENSURE_ARG_POINTER(cbk);

  if (!mObservers) {
    mObservers = new nsAutoVoidArray();
    if (mObservers == nsnull)
      return NS_ERROR_OUT_OF_MEMORY;
  }

  ref=cbk;
  NS_ADDREF(cbk); //CHE CAZZO FA NON SI CAPISCE.. MA FUNZIONA!!
  mObservers->AppendElement(cbk);
  printf("ADDED OBSERVER AT ADDR: %p\n", cbk);
  return NS_OK;
}

/* void removeObserver (in nsSipStateObserver cbk); */
NS_IMETHODIMP nsSIP::RemoveObserver(nsSipStateObserver *cbk)
{
    NS_ENSURE_ARG_POINTER(cbk);

    if (!mObservers)
        return NS_OK;

    PRIntn count = mObservers->Count();
    if (count <= 0)
        return NS_OK;

    PRIntn i;
    nsSipStateObserver* pCallback;
    for (i = 0; i < count; ++i) {
      pCallback = (nsSipStateObserver*)mObservers->ElementAt(i);
      printf("P1: %p - P2: %p\n", pCallback, cbk);
      if (pCallback == cbk){
        printf("REMOVED\n");
        mObservers->RemoveElementAt(i);
        NS_RELEASE(pCallback);
        return NS_OK;
      }
    }
    return NS_OK;
}

void nsSIP::callObservers(char* status){
    if (!mObservers)
        return;

    PRIntn count = mObservers->Count();
    if (count <= 0)
        return;

    PRIntn i;
    nsSipStateObserver* pCallback;
    for (i = 0; i < count; ++i) {
      pCallback = (nsSipStateObserver*)mObservers->ElementAt(i);
      pCallback->OnStatusChange(status);
    }
    return;

}
