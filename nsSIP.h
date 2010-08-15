#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linphone/linphonecore.h"

#include "nsCOMPtr.h"
#include "nsIArray.h"
#include "nsIMutableArray.h"
#include "nsWeakPtr.h"
#include "nsServiceManagerUtils.h"
#include "nsComponentManagerUtils.h"
#include "nsIProxyObjectManager.h"
#include "nsISupports.h"
#include "nsStringAPI.h"
#include "nsThreadUtils.h"
#include "nsXPCOMCIDInternal.h"

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
  static LinphoneCore* lc;
  static long port;
  int call_in_progress;

  void getProxyForObserver(nsCOMPtr<nsSipStateObserver>, nsCOMPtr<nsSipStateObserver>*);
  void CallObservers(const char* status);
  void FlushObservers();
  

private:
  nsCOMPtr<nsIRunnable> mRunner;
  nsCOMPtr<nsIThread> mThread;
  LinphoneCoreVTable cb_table;
  ~nsSIP();


protected:
  nsCOMPtr<nsIMutableArray> mObservers;
  nsCOMPtr<nsIMutableArray> proxy;
};


/* Linphone Callbacks*/
static void linphonec_display_something (LinphoneCore * lc, const char *something){};
static void linphonec_display_status (LinphoneCore * lc, const char *something){};
static void linphonec_display_warning (LinphoneCore * lc, const char *something){};
static void linphonec_display_url (LinphoneCore * lc, const char *something, const char *url){};
static void linphonec_call_received(LinphoneCore *lc, const char *from){};
static void linphonec_prompt_for_auth(LinphoneCore *lc, const char *realm, const char *username){};
static void linphonec_notify_received(LinphoneCore *lc,const char *from,const char *msg){};
static void linphonec_notify_presence_received(LinphoneCore *lc,LinphoneFriend *fid){};
static void linphonec_new_unknown_subscriber(LinphoneCore *lc, LinphoneFriend *lf, const char *url){};
static void linphonec_bye_received(LinphoneCore *lc, const char *from){};
static void linphonec_dtmf_received(LinphoneCore *lc, int dtmf){};
static void linphonec_general_state (LinphoneCore * lc, LinphoneGeneralState *gstate){
  nsSIP* app = (nsSIP*)linphone_core_get_user_data(lc);

  switch(gstate->new_state) {
    case GSTATE_POWER_OFF:
      printf("GSTATE_POWER_OFF");
      break;
    case GSTATE_POWER_STARTUP:
      printf("GSTATE_POWER_STARTUP");
      break;
    case GSTATE_POWER_ON:
      printf("GSTATE_POWER_ON");
      break;
    case GSTATE_POWER_SHUTDOWN:
      printf("GSTATE_POWER_SHUTDOWN");
      break;
    case GSTATE_REG_NONE:
      printf("GSTATE_REG_NONE");
      break;
    case GSTATE_REG_OK:
      printf("GSTATE_REG_OK");
      break;
    case GSTATE_REG_FAILED:
      printf("GSTATE_REG_FAILED");
      break;
    case GSTATE_CALL_IDLE:
      printf("GSTATE_CALL_IDLE");
      app->call_in_progress = 0;
      break;
    case GSTATE_CALL_OUT_INVITE:
      printf("GSTATE_CALL_OUT_INVITE");
      app->call_in_progress = 1;
      break;
    case GSTATE_CALL_OUT_CONNECTED:
      printf("GSTATE_CALL_OUT_CONNECTED");
      break;
    case GSTATE_CALL_IN_INVITE:
      printf("GSTATE_CALL_IN_INVITE");
      app->call_in_progress = 1;
      break;
    case GSTATE_CALL_IN_CONNECTED:
      printf("GSTATE_CALL_IN_CONNECTED");
      break;
    case GSTATE_CALL_END:
      printf("GSTATE_CALL_END");
      break;
    case GSTATE_CALL_ERROR:
      printf("GSTATE_CALL_ERROR");
      break;
    default:
      printf("GSTATE_UNKNOWN_%d",gstate->new_state);
  }
  if (gstate->message) printf(" %s", gstate->message);
  printf("\n");
};
static void stub(){};
