#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linphone/private.h"
#include "mediastreamer2/mediastream.h"

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
#include "nsIThread.h"

#include "nsXPCOMCIDInternal.h"

#include "nsISIP.h"

#define SIP_CONTRACTID "@alcacoop.it/voip/sip-manager;1"
#define SIP_CLASSNAME "Simple XPCOM SIP stack"
#define SIP_CID  { 0xc5bf5079, 0x8bc3, 0x4fc5, { 0x88, 0x1f, 0xf3, 0xbd, 0x85, 0xa6, 0x3c, 0x61 } }





class nsSIP : public nsISIP
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISIP
  nsSIP();
  int call_in_progress;
  void CallObservers(const char* status, const char* data);
  

private:
  LinphoneCore* lc;
  long port;
  nsCOMPtr<nsIThread> mThread;
  LinphoneCoreVTable cb_table;
  ~nsSIP();
  nsCOMPtr<nsIMutableArray> mObservers;
  void getProxyForObserver(nsCOMPtr<nsSipStateObserver>, nsCOMPtr<nsSipStateObserver>*);
  void FlushObservers();
};


/* Linphone Callbacks*/
static void linphonec_display_something (LinphoneCore * lc, const char *something){};
static void linphonec_display_status (LinphoneCore * lc, const char *something){};
static void linphonec_display_warning (LinphoneCore * lc, const char *something){};
static void linphonec_display_url (LinphoneCore * lc, const char *something, const char *url){};
static void linphonec_prompt_for_auth(LinphoneCore *lc, const char *realm, const char *username){};
static void linphonec_notify_received(LinphoneCore *lc,const char *from,const char *msg){};
static void linphonec_notify_presence_received(LinphoneCore *lc,LinphoneFriend *fid){};
static void linphonec_new_unknown_subscriber(LinphoneCore *lc, LinphoneFriend *lf, const char *url){};
static void linphonec_bye_received(LinphoneCore *lc, const char *from){};


static void linphonec_call_received(LinphoneCore *lc, const char *from){
  nsSIP* app = (nsSIP*)linphone_core_get_user_data(lc);
  app->call_in_progress = 1;
  app->CallObservers("INCOMING", from);
};

static void linphonec_dtmf_received(LinphoneCore *lc, int dtmf){
  nsSIP* app = (nsSIP*)linphone_core_get_user_data(lc);
  char tone[2]; 
  tone[0] = dtmf;
  tone[1] = 0;
  app->CallObservers("DTMF", tone);
};

static void linphonec_general_state (LinphoneCore * lc, LinphoneGeneralState *gstate){
  nsSIP* app = (nsSIP*)linphone_core_get_user_data(lc);
  switch(gstate->new_state) {
    case GSTATE_POWER_OFF:
#ifdef DEBUG
      printf("GSTATE_POWER_OFF");
#endif
      break;
    case GSTATE_POWER_STARTUP:
#ifdef DEBUG
      printf("GSTATE_POWER_STARTUP");
#endif
      break;
    case GSTATE_POWER_ON:
#ifdef DEBUG
      printf("GSTATE_POWER_ON");
#endif
      break;
    case GSTATE_POWER_SHUTDOWN:
#ifdef DEBUG
      printf("GSTATE_POWER_SHUTDOWN");
#endif
      break;
    case GSTATE_REG_NONE:
#ifdef DEBUG
      printf("GSTATE_REG_NONE");
#endif
      break;
    case GSTATE_REG_OK:
#ifdef DEBUG
      printf("GSTATE_REG_OK");
#endif
      break;
    case GSTATE_REG_FAILED:
#ifdef DEBUG
      printf("GSTATE_REG_FAILED");
#endif
      break;
    case GSTATE_CALL_IDLE:
#ifdef DEBUG
      printf("GSTATE_CALL_IDLE");
#endif
      app->call_in_progress = 0;
      app->CallObservers("UPDATELOG", NULL);
      break;
    case GSTATE_CALL_OUT_INVITE:
#ifdef DEBUG
      printf("GSTATE_CALL_OUT_INVITE");
#endif
      app->call_in_progress = 1;
      app->CallObservers("CALLING", NULL);
      break;
    case GSTATE_CALL_OUT_CONNECTED:
#ifdef DEBUG
      printf("GSTATE_CALL_OUT_CONNECTED");
#endif
      app->CallObservers("CONNECTED", NULL);
      break;
    case GSTATE_CALL_IN_INVITE:
#ifdef DEBUG
      printf("GSTATE_CALL_IN_INVITE");
#endif
      break;
    case GSTATE_CALL_IN_CONNECTED:
#ifdef DEBUG
      printf("GSTATE_CALL_IN_CONNECTED");
#endif
      app->CallObservers("CONNECTED", NULL);
      break;
    case GSTATE_CALL_END:
#ifdef DEBUG
      printf("GSTATE_CALL_END");
#endif
      app->CallObservers("HANGUP", NULL);
      break;
    case GSTATE_CALL_ERROR:
#ifdef DEBUG
      printf("GSTATE_CALL_ERROR");
#endif
      if (strcmp(gstate->message, "User is busy.")==0)
        app->CallObservers("BUSY", NULL);
      else 
        app->CallObservers("INVALIDURI", NULL);
      break;
    default:
#ifdef DEBUG
      printf("GSTATE_UNKNOWN_%d",gstate->new_state);
#endif
      if (gstate->new_state == 28)
        app->CallObservers("RINGING", NULL);
  }
#ifdef DEBUG
  if (gstate->message) printf(" %s", gstate->message);
  printf("\n");
#endif
};
static void stub(){};
