// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PJSIP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PJSIP_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

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