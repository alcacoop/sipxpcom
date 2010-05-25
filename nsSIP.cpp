#include "nsSIP.h"


NS_IMPL_ISUPPORTS1(nsSIP, nsISIP)


nsSIP::nsSIP() : mObservers(nsnull), proxy(nsnull) {
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
NS_IMETHODIMP nsSIP::Init(PRInt32 _port)
{
  if (port!=0){
    Destroy();
  }

  port = _port;
  
 /* 
  nsresult rv = NS_OK;
  nsCOMPtr<nsIProxyObjectManager> pIProxyObjectManager(do_GetService("@mozilla.org/xpcomproxy;1", &rv));
  if(NS_FAILED(rv)) return rv;

  if (proxy)
    NS_RELEASE(proxy);

  nsCOMPtr<nsIArray> pProxy;
  rv = pIProxyObjectManager->GetProxyForObject(
      NS_PROXY_TO_MAIN_THREAD,
      nsIArray::GetIID(),
      //NS_GET_IID(nsAutoVoidArray),
      mObservers,
      //NS_PROXY_SYNC | NS_PROXY_ALWAYS,
      NS_PROXY_SYNC,
      (void**)&pProxy
      //getter_AddRefs(pProxy));
      );
  if(NS_FAILED(rv)) return rv;

  //printf("\nPROXY AT: %p\n\n", proxy);

  /*
  if (pProxy->mObservers){
    printf("PROXY OBSERVERS: %d\n", pProxy->mObservers->Count());
    pProxy->callObservers((char*)"INIT-PROXIED"); 
  }
  */
  //proxy = pProxy;
  //proxy->CallObservers((char*)"INIT-PROXIED");

  //sipregister((int)port);
  sipregister((int)port, NULL);

  CallObservers("INIT");
  return NS_OK;
}


/* void destroy (); */
NS_IMETHODIMP nsSIP::Destroy() {
  sipderegister();

  CallObservers("DESTROY");
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
  NS_ENSURE_ARG_POINTER(cbk);

  if (!mObservers) {
    mObservers = do_CreateInstance(NS_ARRAY_CONTRACTID);
    NS_ENSURE_STATE(mObservers);

    if (mObservers == nsnull)
      return NS_ERROR_OUT_OF_MEMORY;
  }

  NS_ADDREF(cbk); //CHE CAZZO FA NON SI CAPISCE.. MA COSI FUNZIONA!!
  mObservers->AppendElement(cbk, PR_FALSE);
  printf("ADDED OBSERVER AT ADDR: %p \n", cbk);
  return NS_OK;
}


/* void removeObserver (in nsSipStateObserver cbk); */
NS_IMETHODIMP nsSIP::RemoveObserver(nsSipStateObserver *cbk)
{
    NS_ENSURE_ARG_POINTER(cbk);

    if (!mObservers)
        return NS_OK;

    PRUint32 count = 0;
    mObservers->GetLength(&count);
    if (count <= 0)
        return NS_OK;

    PRIntn i;
    nsSipStateObserver* pCallback;
    for (i = 0; i < count; ++i) {
      (nsIArray*)mObservers->QueryElementAt(i, NS_GET_IID(nsSipStateObserver), (void**)&pCallback);
      if (pCallback == cbk){
        printf("REMOVED OBSERVER AT ADDR: %p\n", pCallback);
        mObservers->RemoveElementAt(i);
        NS_RELEASE(pCallback);
        return NS_OK;
      }
    }


    return NS_OK;
}


//NS_IMETHODIMP nsSIP::CallObservers(const char *status)
void nsSIP::CallObservers(const char* status)
{
  if (!mObservers)
      return;

  PRUint32 count = 0;
  mObservers->GetLength(&count);
  //printf("COUNT: %d\n", count);
  if (count <= 0)
      return;

  PRIntn i;
  nsSipStateObserver* pCallback;
  for (i = 0; i < count; ++i) {
    (nsIArray*)mObservers->QueryElementAt(i, NS_GET_IID(nsSipStateObserver), (void**)&pCallback);
    //printf("CALLBACK ADDR: %p\n", pCallback);
    pCallback->OnStatusChange(status);
  }
}
