#include "nsSIP.h"
#include "nsStringAPI.h"
#include "nsThreadUtils.h"


class nsRunner : public nsIRunnable 
{
public:
  NS_DECL_ISUPPORTS
  NS_IMETHOD Run(){
    while (nsSIP::port){
      PR_Sleep(PR_MillisecondsToInterval(300));
      linphone_core_iterate(nsSIP::lc);
    }
    return NS_OK;
  } 
};

NS_IMPL_THREADSAFE_ISUPPORTS1(nsRunner, nsIRunnable)




NS_IMPL_ISUPPORTS1(nsSIP, nsISIP)

LinphoneCore *nsSIP::lc = NULL;
long nsSIP::port = 0;


nsSIP::nsSIP() : mObservers(nsnull), proxy(nsnull) {
  port = 0;
  call_in_progress = 0;

  cb_table.show =(ShowInterfaceCb)stub;
  cb_table.inv_recv = linphonec_call_received;
  cb_table.bye_recv = (ByeReceivedCb) stub;
  cb_table.notify_recv = (NotifyReceivedCb)stub;
  cb_table.new_unknown_subscriber = (NewUnknownSubscriberCb)stub;
  cb_table.auth_info_requested = (AuthInfoRequested) stub;
  cb_table.display_status = (DisplayStatusCb) stub;
  cb_table.display_message = (DisplayMessageCb)stub;
  cb_table.display_url = (DisplayUrlCb) stub;
  cb_table.call_log_updated = (CallLogUpdated) stub;
  cb_table.text_received = (TextMessageReceived)stub;
  cb_table.general_state = linphonec_general_state;
  cb_table.dtmf_received = (DtmfReceived) stub;
  cb_table.refer_received = (ReferReceived)stub;
  cb_table.buddy_info_updated = (BuddyInfoUpdated) stub;
}

nsSIP::~nsSIP() {
  FlushObservers();
}


/* void init (in long port); */
NS_IMETHODIMP nsSIP::Init(PRInt32 _port)
{

  if (port!=0){
    Destroy();
  }
  if (_port<1024)
    return NS_ERROR_ILLEGAL_VALUE;
  port = _port;
  lc = linphone_core_new(&cb_table, NULL, NULL, NULL);


  nsCOMPtr<nsIRunnable> runner = new nsRunner();
  nsCOMPtr<nsIThread> aThread;

  NS_NewThread(getter_AddRefs(aThread), runner);


  return NS_OK;
}


/* void destroy (); */
NS_IMETHODIMP nsSIP::Destroy() {
  if (port==0)
    return NS_OK;

  port = 0;
  CallObservers("DESTROY");
  FlushObservers();
  
  //PR_Sleep(PR_MillisecondsToInterval(400));

  return NS_OK;
}


/* void changesipport (in long port); */
NS_IMETHODIMP nsSIP::Changesipport(PRInt32 port)
{
  linphone_core_set_sip_port(lc, port);
  return NS_OK;
}


/* void call (in AString URI); */
NS_IMETHODIMP nsSIP::Call(const char* URI) {
  linphone_core_invite(lc, URI);
  return NS_OK;
}


/* void hangup (); */
NS_IMETHODIMP nsSIP::Hangup() {
  //siphangup();
  return NS_OK;
}


/* void accept (); */
NS_IMETHODIMP nsSIP::Accept()
{
  return NS_ERROR_NOT_IMPLEMENTED;
}


/* void senddtmf (in char tone); */
NS_IMETHODIMP nsSIP::Senddtmf(char tone)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}



/* void setringtone (in string file); */
NS_IMETHODIMP nsSIP::Setringtone(const char *file)
{
  if (port==0) return NS_OK;
  //DO SOMETHING
  return NS_OK;
}



/* ************************************* *
 *          GESTIONE OBSERVERS           *
 * ************************************* */     

/* void addObserver (in nsSipStateObserver cbk); */
NS_IMETHODIMP nsSIP::AddObserver(nsSipStateObserver *cbk)
{
  NS_ENSURE_ARG_POINTER(cbk);

  if (!mObservers) {
    mObservers = do_CreateInstance(NS_ARRAY_CONTRACTID);
    proxy = do_CreateInstance(NS_ARRAY_CONTRACTID);
    NS_ENSURE_STATE(mObservers);
    NS_ENSURE_STATE(proxy);
  }

  /* TEST IF cbk OBSERVER IS ALREADY REGISTERED */
  PRUint32 count = 0;
  PRIntn i;
  mObservers->GetLength(&count);
  nsCOMPtr<nsSipStateObserver> pCallback;
  for (i = 0; i < count; ++i) {
    (nsIArray*)mObservers->QueryElementAt(i, NS_GET_IID(nsSipStateObserver), (void**)&pCallback);
    if (pCallback == cbk){
      printf("OBSERVER ALREADY REGISTERED\n");
      return NS_OK;
    }
  }

  NS_IF_ADDREF(cbk);
  mObservers->AppendElement(cbk, PR_FALSE);
  printf("ADDED OBSERVER AT ADDR: %p \n", cbk);
  /* PROXY */
  nsCOMPtr<nsSipStateObserver> pCbk;
  getProxyForObserver(cbk, &pCbk);
  NS_IF_ADDREF(pCbk);
  proxy->AppendElement(pCbk, PR_FALSE);
  //SYNC OBSERVERS ARRAY ON linphone BRIDGE
  //SyncObservers((nsIArray*)proxy);
  return NS_OK;
}


/* void removeObserver (in nsSipStateObserver cbk); */
NS_IMETHODIMP nsSIP::RemoveObserver(nsSipStateObserver *cbk)
{
  NS_ENSURE_ARG_POINTER(cbk);

  if (!mObservers){
    printf("NO SUCH OBSERVER\n");
    return NS_OK;
  }

  PRUint32 count = 0;
  mObservers->GetLength(&count);
  if (count <= 0){
    printf("NO SUCH OBSERVER\n");
    return NS_OK;
  }

  PRIntn i;
  nsCOMPtr<nsSipStateObserver> pCallback;
  nsCOMPtr<nsSipStateObserver> _pCallback;

  for (i = 0; i < count; ++i) {
    (nsIArray*)mObservers->QueryElementAt(i, NS_GET_IID(nsSipStateObserver), (void**)&pCallback);
    (nsIArray*)proxy->QueryElementAt(i, NS_GET_IID(nsSipStateObserver), (void**)&_pCallback);
    if (pCallback == cbk){
      mObservers->RemoveElementAt(i);
      proxy->RemoveElementAt(i);
      printf("REMOVED OBSERVER AT ADDR: %p - %p\n", cbk, (nsSipStateObserver*)pCallback);
      //SYNC OBSERVERS ARRAY ON linphone BRIDGE
      //SyncObservers((nsIArray*)proxy);

      NS_RELEASE(_pCallback);
      NS_RELEASE(pCallback);
      return NS_OK;
    }
  }

  printf("NO SUCH OBSERVER\n");
  return NS_OK;
}



void nsSIP::FlushObservers(){
  if (!mObservers)
      return;

  PRUint32 count = 0;
  mObservers->GetLength(&count);

  PRIntn i;
  nsCOMPtr<nsSipStateObserver> pCallback;
  nsCOMPtr<nsSipStateObserver> _pCallback;

  for (i = 0; i < count; ++i) {
    (nsIArray*)mObservers->QueryElementAt(i, NS_GET_IID(nsSipStateObserver), (void**)&pCallback);
    (nsIArray*)proxy->QueryElementAt(i, NS_GET_IID(nsSipStateObserver), (void**)&_pCallback);
    mObservers->RemoveElementAt(i);
    proxy->RemoveElementAt(i);

    NS_RELEASE(pCallback);
    NS_RELEASE(_pCallback);
  }

  printf("FLUSHED ALL OBSERVERS\n");
  NS_RELEASE(mObservers);
  NS_RELEASE(proxy);
  //SyncObservers(NULL);
  return;
}


/* void clearObservers (); */
NS_IMETHODIMP nsSIP::ClearObservers()
{
  FlushObservers();
  return NS_OK;
}


void nsSIP::getProxyForObserver(nsCOMPtr<nsSipStateObserver> cbk, nsCOMPtr<nsSipStateObserver> *pCbk){
  nsresult rv = NS_OK;
  nsCOMPtr<nsIProxyObjectManager> pIProxyObjectManager(do_GetService("@mozilla.org/xpcomproxy;1", &rv));

  rv = pIProxyObjectManager->GetProxyForObject(
    NS_PROXY_TO_MAIN_THREAD,
    nsSipStateObserver::GetIID(),
    cbk,
    NS_PROXY_SYNC | NS_PROXY_ALWAYS,
    (void**)pCbk
  );
}


void nsSIP::CallObservers(const char* status)
{
  if (!mObservers)
      return;

  PRUint32 count = 0;
  mObservers->GetLength(&count);
  if (count <= 0)
      return;

  PRIntn i;
  nsCOMPtr<nsSipStateObserver> _pCallback;
  for (i = 0; i < count; ++i) {
    (nsIArray*)mObservers->QueryElementAt(i, NS_GET_IID(nsSipStateObserver), (void**)&_pCallback);
    _pCallback->OnStatusChange(status);
  }

  return;
}
