#include "linphone.h"

static nsCOMPtr<nsIArray> mObservers;

/* 
   Bridge verso l'API XPCOM
   richiama gli observer registrati mediante proxy alla callbacks
*/
static void CallObservers(const char* status)
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


void SyncObservers(nsCOMPtr<nsIArray> o){
  mObservers = o;
}


/*
   Stack, socket e thread constructor
*/
int sipinit(long sipPort) {
}


/*
   Stack, socket e thread destructor
*/
int sipdestroy(){
}


/*
   Inizializzazione chiamata in uscita
*/
int sipmakecall(char *sipToAddr){
}


/*
   Chiusura chiamate attive
*/
int siphangup(){
}

