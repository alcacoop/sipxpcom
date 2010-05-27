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

#define REGISTER_THREAD()	\
  static pj_thread_desc desc;\
  static pj_thread_t *  thread;\
  if(!pj_thread_is_registered()) {\
    pj_thread_register(NULL,desc,&thread);\
  }

#ifdef __GNUG__
	#define PJSIP_API 
#else
	#ifdef PJSIP_EXPORTS
		#define PJSIP_API __declspec(dllexport)
	#else
		#define PJSIP_API __declspec(dllimport)
	#endif
#endif

PJSIP_API int sipregister(long sipPort);
PJSIP_API int sipderegister();
PJSIP_API int sipmakecall(char*);
PJSIP_API int siphangup();

PJSIP_API int sipaddaccount(char*, char*, char*);
PJSIP_API void sipsetstunserver(char*);

void SyncObservers(nsCOMPtr<nsIArray>);
static void CallObservers(const char*);
