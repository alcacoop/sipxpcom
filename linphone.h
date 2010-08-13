#include "stdio.h"
#include "string.h"
#include "nsCOMPtr.h"
#include "nsIArray.h"
#include "nsIMutableArray.h"
#include "nsISIP.h"
#include "nsServiceManagerUtils.h"
#include "nsComponentManagerUtils.h"
#include "nsIProxyObjectManager.h"


int sipinit(long sipPort);
int sipdestroy();
int sipmakecall(char*);
int siphangup();


void SyncObservers(nsCOMPtr<nsIArray>);
static void CallObservers(const char*);
