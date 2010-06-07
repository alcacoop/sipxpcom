#include <pjsua-lib/pjsua.h>
#include "stdio.h"
#include "string.h"
#include "nsCOMPtr.h"
#include "nsIArray.h"
#include "nsIMutableArray.h"
#include "nsISIP.h"
#include "nsServiceManagerUtils.h"
#include "nsComponentManagerUtils.h"
#include "nsIProxyObjectManager.h"
 
#define THIS_FILE	"APP"
#define current_acc	pjsua_acc_get_default()


int sipregister(long sipPort);
int sipderegister();
int sipmakecall(char*);
int siphangup();

int sipaddaccount(char*, char*, char*);
void sipsetstunserver(char*);

void SyncObservers(nsCOMPtr<nsIArray>);
static void CallObservers(const char*);
