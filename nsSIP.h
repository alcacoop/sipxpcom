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




static void debug( const char* format, ... ) {
#ifdef DEBUG
  va_list args;
  printf( "DEBUG - " );
  va_start( args, format );
  vprintf( format, args );
  va_end( args );
  printf( "\n" );
#endif
}



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
static void linphonec_calllog_update (LinphoneCore *lc, struct _LinphoneCallLog *newcl){
  nsSIP* app = (nsSIP*)linphone_core_get_user_data(lc);
  app->CallObservers("UPDATELOG", NULL);
  debug("UPDATELOG");
};

static void linphonec_new_unknown_subscriber(LinphoneCore *lc, LinphoneFriend *lf, const char *url){
  debug("NEW: %s", url);
};

static void linphonec_notify_presence_received(LinphoneCore *lc,LinphoneFriend *fid){
  char* addr = linphone_address_as_string(linphone_friend_get_address(fid));
  debug("PRESENCE: %s - %d", addr, linphone_friend_get_status(fid));
  ms_free(addr);
};

static void linphonec_display_status (LinphoneCore * lc, const char *something){
  debug("STATUS: %s", something);
  nsSIP* app = (nsSIP*)linphone_core_get_user_data(lc);
  app->CallObservers("STATUS", something);
};

static void linphonec_notify_received(LinphoneCore *lc,const char *from,const char *msg){
  debug("NOTIFY: %s - %s", from, msg);
};

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
      debug("GSTATE_POWER_OFF");
      break;
    case GSTATE_POWER_STARTUP:
      debug("GSTATE_POWER_STARTUP");
      break;
    case GSTATE_POWER_ON:
      debug("GSTATE_POWER_ON");
      break;
    case GSTATE_POWER_SHUTDOWN:
      debug("GSTATE_POWER_SHUTDOWN");
      break;
    case GSTATE_REG_NONE:
      debug("GSTATE_REG_NONE");
      break;
    case GSTATE_REG_OK:
      debug("GSTATE_REG_OK");
      break;
    case GSTATE_REG_FAILED:
      debug("GSTATE_REG_FAILED");
      break;
    case GSTATE_CALL_IDLE:
      debug("GSTATE_CALL_IDLE");
      app->call_in_progress = 0;
      break;
    case GSTATE_CALL_OUT_INVITE:
      debug("GSTATE_CALL_OUT_INVITE");
      app->call_in_progress = 1;
      app->CallObservers("CALLING", NULL);
      break;
    case GSTATE_CALL_OUT_CONNECTED:
      debug("GSTATE_CALL_OUT_CONNECTED");
      app->CallObservers("CONNECTED", NULL);
      break;
    case GSTATE_CALL_IN_INVITE:
      debug("GSTATE_CALL_IN_INVITE");
      break;
    case GSTATE_CALL_IN_CONNECTED:
      debug("GSTATE_CALL_IN_CONNECTED");
      app->CallObservers("CONNECTED", NULL);
      break;
    case GSTATE_CALL_END:
      debug("GSTATE_CALL_END");
      app->CallObservers("HANGUP", NULL);
      break;
    case GSTATE_CALL_ERROR:
      debug("GSTATE_CALL_ERROR");
      if (strcmp(gstate->message, "User is busy.")==0)
        app->CallObservers("BUSY", NULL);
      else 
        app->CallObservers("INVALIDURI", NULL);
      break;
    default:
      debug("GSTATE_UNKNOWN_%d",gstate->new_state);
      if (gstate->new_state == 28)
        app->CallObservers("RINGING", NULL);
  }
  if (gstate->message) debug(" %s", gstate->message);
};

static void stub(){ debug("STUB"); };
