#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nsCOMPtr.h"
#include "nsIArray.h"
#include "nsIMutableArray.h"
#include "nsWeakPtr.h"
#include "nsServiceManagerUtils.h"
#include "nsComponentManagerUtils.h"
#include "nsIProxyObjectManager.h"
#include "nsISupports.h"
#include "nsXPCOMCIDInternal.h"
#include "linphone/linphonecore.h"

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
  
private:
  LinphoneCoreVTable cb_table;
  int call_in_progress;
  ~nsSIP();

  void getProxyForObserver(nsCOMPtr<nsSipStateObserver>, nsCOMPtr<nsSipStateObserver>*);
  void CallObservers(const char* status);
  void FlushObservers();


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
static void linphonec_general_state (LinphoneCore * lc, LinphoneGeneralState *gstate){};
static void stub(){};
