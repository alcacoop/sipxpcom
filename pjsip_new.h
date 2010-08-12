#include <pjmedia.h>
#include <pjlib-util.h>
#include <pjlib.h>
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

void playring();
void stopring();
void setringtone(char*);

void playtone(char);

int sipaddaccount(char*, char*, char*);
void sipsetstunserver(char*);

void SyncObservers(nsCOMPtr<nsIArray>);
static void CallObservers(const char*);


static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata);
static void on_call_state(pjsua_call_id call_id, pjsip_event *e);
static void on_call_media_state(pjsua_call_id call_id);

void configure();
pj_status_t app_init();
pj_status_t app_destroy();
