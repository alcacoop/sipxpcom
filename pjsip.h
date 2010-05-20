#include "nsCOMPtr.h"

#ifdef __GNUG__
	#define PJSIP_API 
#else
	#ifdef PJSIP_EXPORTS
		#define PJSIP_API __declspec(dllexport)
	#else
		#define PJSIP_API __declspec(dllimport)
	#endif
#endif

PJSIP_API int sipregister(long sipPort, nsCOMPtr<class nsSipStateObserver>);
PJSIP_API int sipderegister();
PJSIP_API int sipmakecall(char*);
PJSIP_API int siphangup();