/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM nsISIP.idl
 */

#ifndef __gen_nsISIP_h__
#define __gen_nsISIP_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIExtensionManager_h__
#include "nsIExtensionManager.h"
#endif

#ifndef __gen_nsIFile_h__
#include "nsIFile.h"
#endif

#ifndef __gen_nsIRunnable_h__
#include "nsIRunnable.h"
#endif

#ifndef __gen_nsIArray_h__
#include "nsIArray.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsSipStateObserver */
#define NS_SIPSTATEOBSERVER_IID_STR "3869ed55-e06a-4ac1-84ea-4c3a4b696924"

#define NS_SIPSTATEOBSERVER_IID \
  {0x3869ed55, 0xe06a, 0x4ac1, \
    { 0x84, 0xea, 0x4c, 0x3a, 0x4b, 0x69, 0x69, 0x24 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsSipStateObserver : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_SIPSTATEOBSERVER_IID)

  /* void onStatusChange (in string status); */
  NS_SCRIPTABLE NS_IMETHOD OnStatusChange(const char *status) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsSipStateObserver, NS_SIPSTATEOBSERVER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSSIPSTATEOBSERVER \
  NS_SCRIPTABLE NS_IMETHOD OnStatusChange(const char *status); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSSIPSTATEOBSERVER(_to) \
  NS_SCRIPTABLE NS_IMETHOD OnStatusChange(const char *status) { return _to OnStatusChange(status); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSSIPSTATEOBSERVER(_to) \
  NS_SCRIPTABLE NS_IMETHOD OnStatusChange(const char *status) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStatusChange(status); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public nsSipStateObserver
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSSIPSTATEOBSERVER

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, nsSipStateObserver)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* void onStatusChange (in string status); */
NS_IMETHODIMP _MYCLASS_::OnStatusChange(const char *status)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsICallLog */
#define NS_ICALLLOG_IID_STR "248eaceb-8910-49a5-a775-f06b62eaa2cd"

#define NS_ICALLLOG_IID \
  {0x248eaceb, 0x8910, 0x49a5, \
    { 0xa7, 0x75, 0xf0, 0x6b, 0x62, 0xea, 0xa2, 0xcd }}

class NS_NO_VTABLE NS_SCRIPTABLE nsICallLog : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ICALLLOG_IID)

  /* attribute short direction; */
  NS_SCRIPTABLE NS_IMETHOD GetDirection(PRInt16 *aDirection) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetDirection(PRInt16 aDirection) = 0;

  /* attribute short status; */
  NS_SCRIPTABLE NS_IMETHOD GetStatus(PRInt16 *aStatus) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetStatus(PRInt16 aStatus) = 0;

  /* attribute long duration; */
  NS_SCRIPTABLE NS_IMETHOD GetDuration(PRInt32 *aDuration) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetDuration(PRInt32 aDuration) = 0;

  /* attribute ACString date; */
  NS_SCRIPTABLE NS_IMETHOD GetDate(nsACString & aDate) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetDate(const nsACString & aDate) = 0;

  /* attribute ACString from; */
  NS_SCRIPTABLE NS_IMETHOD GetFrom(nsACString & aFrom) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetFrom(const nsACString & aFrom) = 0;

  /* attribute ACString to; */
  NS_SCRIPTABLE NS_IMETHOD GetTo(nsACString & aTo) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetTo(const nsACString & aTo) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsICallLog, NS_ICALLLOG_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICALLLOG \
  NS_SCRIPTABLE NS_IMETHOD GetDirection(PRInt16 *aDirection); \
  NS_SCRIPTABLE NS_IMETHOD SetDirection(PRInt16 aDirection); \
  NS_SCRIPTABLE NS_IMETHOD GetStatus(PRInt16 *aStatus); \
  NS_SCRIPTABLE NS_IMETHOD SetStatus(PRInt16 aStatus); \
  NS_SCRIPTABLE NS_IMETHOD GetDuration(PRInt32 *aDuration); \
  NS_SCRIPTABLE NS_IMETHOD SetDuration(PRInt32 aDuration); \
  NS_SCRIPTABLE NS_IMETHOD GetDate(nsACString & aDate); \
  NS_SCRIPTABLE NS_IMETHOD SetDate(const nsACString & aDate); \
  NS_SCRIPTABLE NS_IMETHOD GetFrom(nsACString & aFrom); \
  NS_SCRIPTABLE NS_IMETHOD SetFrom(const nsACString & aFrom); \
  NS_SCRIPTABLE NS_IMETHOD GetTo(nsACString & aTo); \
  NS_SCRIPTABLE NS_IMETHOD SetTo(const nsACString & aTo); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICALLLOG(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetDirection(PRInt16 *aDirection) { return _to GetDirection(aDirection); } \
  NS_SCRIPTABLE NS_IMETHOD SetDirection(PRInt16 aDirection) { return _to SetDirection(aDirection); } \
  NS_SCRIPTABLE NS_IMETHOD GetStatus(PRInt16 *aStatus) { return _to GetStatus(aStatus); } \
  NS_SCRIPTABLE NS_IMETHOD SetStatus(PRInt16 aStatus) { return _to SetStatus(aStatus); } \
  NS_SCRIPTABLE NS_IMETHOD GetDuration(PRInt32 *aDuration) { return _to GetDuration(aDuration); } \
  NS_SCRIPTABLE NS_IMETHOD SetDuration(PRInt32 aDuration) { return _to SetDuration(aDuration); } \
  NS_SCRIPTABLE NS_IMETHOD GetDate(nsACString & aDate) { return _to GetDate(aDate); } \
  NS_SCRIPTABLE NS_IMETHOD SetDate(const nsACString & aDate) { return _to SetDate(aDate); } \
  NS_SCRIPTABLE NS_IMETHOD GetFrom(nsACString & aFrom) { return _to GetFrom(aFrom); } \
  NS_SCRIPTABLE NS_IMETHOD SetFrom(const nsACString & aFrom) { return _to SetFrom(aFrom); } \
  NS_SCRIPTABLE NS_IMETHOD GetTo(nsACString & aTo) { return _to GetTo(aTo); } \
  NS_SCRIPTABLE NS_IMETHOD SetTo(const nsACString & aTo) { return _to SetTo(aTo); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICALLLOG(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetDirection(PRInt16 *aDirection) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDirection(aDirection); } \
  NS_SCRIPTABLE NS_IMETHOD SetDirection(PRInt16 aDirection) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDirection(aDirection); } \
  NS_SCRIPTABLE NS_IMETHOD GetStatus(PRInt16 *aStatus) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStatus(aStatus); } \
  NS_SCRIPTABLE NS_IMETHOD SetStatus(PRInt16 aStatus) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetStatus(aStatus); } \
  NS_SCRIPTABLE NS_IMETHOD GetDuration(PRInt32 *aDuration) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDuration(aDuration); } \
  NS_SCRIPTABLE NS_IMETHOD SetDuration(PRInt32 aDuration) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDuration(aDuration); } \
  NS_SCRIPTABLE NS_IMETHOD GetDate(nsACString & aDate) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDate(aDate); } \
  NS_SCRIPTABLE NS_IMETHOD SetDate(const nsACString & aDate) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDate(aDate); } \
  NS_SCRIPTABLE NS_IMETHOD GetFrom(nsACString & aFrom) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFrom(aFrom); } \
  NS_SCRIPTABLE NS_IMETHOD SetFrom(const nsACString & aFrom) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetFrom(aFrom); } \
  NS_SCRIPTABLE NS_IMETHOD GetTo(nsACString & aTo) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTo(aTo); } \
  NS_SCRIPTABLE NS_IMETHOD SetTo(const nsACString & aTo) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetTo(aTo); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsCallLog : public nsICallLog
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSICALLLOG

  nsCallLog();

private:
  ~nsCallLog();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsCallLog, nsICallLog)

nsCallLog::nsCallLog()
{
  /* member initializers and constructor code */
}

nsCallLog::~nsCallLog()
{
  /* destructor code */
}

/* attribute short direction; */
NS_IMETHODIMP nsCallLog::GetDirection(PRInt16 *aDirection)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsCallLog::SetDirection(PRInt16 aDirection)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute short status; */
NS_IMETHODIMP nsCallLog::GetStatus(PRInt16 *aStatus)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsCallLog::SetStatus(PRInt16 aStatus)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute long duration; */
NS_IMETHODIMP nsCallLog::GetDuration(PRInt32 *aDuration)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsCallLog::SetDuration(PRInt32 aDuration)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute ACString date; */
NS_IMETHODIMP nsCallLog::GetDate(nsACString & aDate)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsCallLog::SetDate(const nsACString & aDate)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute ACString from; */
NS_IMETHODIMP nsCallLog::GetFrom(nsACString & aFrom)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsCallLog::SetFrom(const nsACString & aFrom)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute ACString to; */
NS_IMETHODIMP nsCallLog::GetTo(nsACString & aTo)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsCallLog::SetTo(const nsACString & aTo)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIList */
#define NS_ILIST_IID_STR "fe4d239b-8766-4386-999d-bcd68903eb71"

#define NS_ILIST_IID \
  {0xfe4d239b, 0x8766, 0x4386, \
    { 0x99, 0x9d, 0xbc, 0xd6, 0x89, 0x03, 0xeb, 0x71 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIList : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ILIST_IID)

  /* nsICallLog item (in unsigned long index); */
  NS_SCRIPTABLE NS_IMETHOD Item(PRUint32 index, nsICallLog **_retval NS_OUTPARAM) = 0;

  /* [noscript] void add (in nsICallLog cl); */
  NS_IMETHOD Add(nsICallLog *cl) = 0;

  /* readonly attribute unsigned long length; */
  NS_SCRIPTABLE NS_IMETHOD GetLength(PRUint32 *aLength) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIList, NS_ILIST_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSILIST \
  NS_SCRIPTABLE NS_IMETHOD Item(PRUint32 index, nsICallLog **_retval NS_OUTPARAM); \
  NS_IMETHOD Add(nsICallLog *cl); \
  NS_SCRIPTABLE NS_IMETHOD GetLength(PRUint32 *aLength); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSILIST(_to) \
  NS_SCRIPTABLE NS_IMETHOD Item(PRUint32 index, nsICallLog **_retval NS_OUTPARAM) { return _to Item(index, _retval); } \
  NS_IMETHOD Add(nsICallLog *cl) { return _to Add(cl); } \
  NS_SCRIPTABLE NS_IMETHOD GetLength(PRUint32 *aLength) { return _to GetLength(aLength); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSILIST(_to) \
  NS_SCRIPTABLE NS_IMETHOD Item(PRUint32 index, nsICallLog **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->Item(index, _retval); } \
  NS_IMETHOD Add(nsICallLog *cl) { return !_to ? NS_ERROR_NULL_POINTER : _to->Add(cl); } \
  NS_SCRIPTABLE NS_IMETHOD GetLength(PRUint32 *aLength) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLength(aLength); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsList : public nsIList
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSILIST

  nsList();

private:
  ~nsList();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsList, nsIList)

nsList::nsList()
{
  /* member initializers and constructor code */
}

nsList::~nsList()
{
  /* destructor code */
}

/* nsICallLog item (in unsigned long index); */
NS_IMETHODIMP nsList::Item(PRUint32 index, nsICallLog **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void add (in nsICallLog cl); */
NS_IMETHODIMP nsList::Add(nsICallLog *cl)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long length; */
NS_IMETHODIMP nsList::GetLength(PRUint32 *aLength)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISIP */
#define NS_ISIP_IID_STR "c5bf5079-8bc3-4fc5-881f-f3bd85a63c61"

#define NS_ISIP_IID \
  {0xc5bf5079, 0x8bc3, 0x4fc5, \
    { 0x88, 0x1f, 0xf3, 0xbd, 0x85, 0xa6, 0x3c, 0x61 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsISIP : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISIP_IID)

  /* void init (in long port); */
  NS_SCRIPTABLE NS_IMETHOD Init(PRInt32 port) = 0;

  /* void destroy (); */
  NS_SCRIPTABLE NS_IMETHOD Destroy(void) = 0;

  /* void setSipPort (in long port); */
  NS_SCRIPTABLE NS_IMETHOD SetSipPort(PRInt32 port) = 0;

  /* void setRTPAudioPort (in long port); */
  NS_SCRIPTABLE NS_IMETHOD SetRTPAudioPort(PRInt32 port) = 0;

  /* void setNOFirewall (); */
  NS_SCRIPTABLE NS_IMETHOD SetNOFirewall(void) = 0;

  /* void setNATFirewall (in string fw_addr); */
  NS_SCRIPTABLE NS_IMETHOD SetNATFirewall(const char *fw_addr) = 0;

  /* void setSTUNFirewall (in string stun_addr); */
  NS_SCRIPTABLE NS_IMETHOD SetSTUNFirewall(const char *stun_addr) = 0;

  /* void setPresenceInfo (in long presence_status); */
  NS_SCRIPTABLE NS_IMETHOD SetPresenceInfo(PRInt32 presence_status) = 0;

  /* void call (in string URI); */
  NS_SCRIPTABLE NS_IMETHOD Call(const char *URI) = 0;

  /* void hangup (); */
  NS_SCRIPTABLE NS_IMETHOD Hangup(void) = 0;

  /* void accept (); */
  NS_SCRIPTABLE NS_IMETHOD Accept(void) = 0;

  /* void sendDtmf (in char tone); */
  NS_SCRIPTABLE NS_IMETHOD SendDtmf(char tone) = 0;

  /* boolean isValidSipURI (in string uri); */
  NS_SCRIPTABLE NS_IMETHOD IsValidSipURI(const char *uri, PRBool *_retval NS_OUTPARAM) = 0;

  /* void setRingTone (in string file); */
  NS_SCRIPTABLE NS_IMETHOD SetRingTone(const char *file) = 0;

  /* void getCallLogs ([retval] out nsIList retv); */
  NS_SCRIPTABLE NS_IMETHOD GetCallLogs(nsIList **retv NS_OUTPARAM) = 0;

  /* void clearCallLogs (); */
  NS_SCRIPTABLE NS_IMETHOD ClearCallLogs(void) = 0;

  /* void addObserver (in nsSipStateObserver cbk); */
  NS_SCRIPTABLE NS_IMETHOD AddObserver(nsSipStateObserver *cbk) = 0;

  /* void removeObserver (in nsSipStateObserver cbk); */
  NS_SCRIPTABLE NS_IMETHOD RemoveObserver(nsSipStateObserver *cbk) = 0;

  /* void clearObservers (); */
  NS_SCRIPTABLE NS_IMETHOD ClearObservers(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISIP, NS_ISIP_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISIP \
  NS_SCRIPTABLE NS_IMETHOD Init(PRInt32 port); \
  NS_SCRIPTABLE NS_IMETHOD Destroy(void); \
  NS_SCRIPTABLE NS_IMETHOD SetSipPort(PRInt32 port); \
  NS_SCRIPTABLE NS_IMETHOD SetRTPAudioPort(PRInt32 port); \
  NS_SCRIPTABLE NS_IMETHOD SetNOFirewall(void); \
  NS_SCRIPTABLE NS_IMETHOD SetNATFirewall(const char *fw_addr); \
  NS_SCRIPTABLE NS_IMETHOD SetSTUNFirewall(const char *stun_addr); \
  NS_SCRIPTABLE NS_IMETHOD SetPresenceInfo(PRInt32 presence_status); \
  NS_SCRIPTABLE NS_IMETHOD Call(const char *URI); \
  NS_SCRIPTABLE NS_IMETHOD Hangup(void); \
  NS_SCRIPTABLE NS_IMETHOD Accept(void); \
  NS_SCRIPTABLE NS_IMETHOD SendDtmf(char tone); \
  NS_SCRIPTABLE NS_IMETHOD IsValidSipURI(const char *uri, PRBool *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetRingTone(const char *file); \
  NS_SCRIPTABLE NS_IMETHOD GetCallLogs(nsIList **retv NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD ClearCallLogs(void); \
  NS_SCRIPTABLE NS_IMETHOD AddObserver(nsSipStateObserver *cbk); \
  NS_SCRIPTABLE NS_IMETHOD RemoveObserver(nsSipStateObserver *cbk); \
  NS_SCRIPTABLE NS_IMETHOD ClearObservers(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISIP(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(PRInt32 port) { return _to Init(port); } \
  NS_SCRIPTABLE NS_IMETHOD Destroy(void) { return _to Destroy(); } \
  NS_SCRIPTABLE NS_IMETHOD SetSipPort(PRInt32 port) { return _to SetSipPort(port); } \
  NS_SCRIPTABLE NS_IMETHOD SetRTPAudioPort(PRInt32 port) { return _to SetRTPAudioPort(port); } \
  NS_SCRIPTABLE NS_IMETHOD SetNOFirewall(void) { return _to SetNOFirewall(); } \
  NS_SCRIPTABLE NS_IMETHOD SetNATFirewall(const char *fw_addr) { return _to SetNATFirewall(fw_addr); } \
  NS_SCRIPTABLE NS_IMETHOD SetSTUNFirewall(const char *stun_addr) { return _to SetSTUNFirewall(stun_addr); } \
  NS_SCRIPTABLE NS_IMETHOD SetPresenceInfo(PRInt32 presence_status) { return _to SetPresenceInfo(presence_status); } \
  NS_SCRIPTABLE NS_IMETHOD Call(const char *URI) { return _to Call(URI); } \
  NS_SCRIPTABLE NS_IMETHOD Hangup(void) { return _to Hangup(); } \
  NS_SCRIPTABLE NS_IMETHOD Accept(void) { return _to Accept(); } \
  NS_SCRIPTABLE NS_IMETHOD SendDtmf(char tone) { return _to SendDtmf(tone); } \
  NS_SCRIPTABLE NS_IMETHOD IsValidSipURI(const char *uri, PRBool *_retval NS_OUTPARAM) { return _to IsValidSipURI(uri, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetRingTone(const char *file) { return _to SetRingTone(file); } \
  NS_SCRIPTABLE NS_IMETHOD GetCallLogs(nsIList **retv NS_OUTPARAM) { return _to GetCallLogs(retv); } \
  NS_SCRIPTABLE NS_IMETHOD ClearCallLogs(void) { return _to ClearCallLogs(); } \
  NS_SCRIPTABLE NS_IMETHOD AddObserver(nsSipStateObserver *cbk) { return _to AddObserver(cbk); } \
  NS_SCRIPTABLE NS_IMETHOD RemoveObserver(nsSipStateObserver *cbk) { return _to RemoveObserver(cbk); } \
  NS_SCRIPTABLE NS_IMETHOD ClearObservers(void) { return _to ClearObservers(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISIP(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(PRInt32 port) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(port); } \
  NS_SCRIPTABLE NS_IMETHOD Destroy(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Destroy(); } \
  NS_SCRIPTABLE NS_IMETHOD SetSipPort(PRInt32 port) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSipPort(port); } \
  NS_SCRIPTABLE NS_IMETHOD SetRTPAudioPort(PRInt32 port) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetRTPAudioPort(port); } \
  NS_SCRIPTABLE NS_IMETHOD SetNOFirewall(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetNOFirewall(); } \
  NS_SCRIPTABLE NS_IMETHOD SetNATFirewall(const char *fw_addr) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetNATFirewall(fw_addr); } \
  NS_SCRIPTABLE NS_IMETHOD SetSTUNFirewall(const char *stun_addr) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSTUNFirewall(stun_addr); } \
  NS_SCRIPTABLE NS_IMETHOD SetPresenceInfo(PRInt32 presence_status) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPresenceInfo(presence_status); } \
  NS_SCRIPTABLE NS_IMETHOD Call(const char *URI) { return !_to ? NS_ERROR_NULL_POINTER : _to->Call(URI); } \
  NS_SCRIPTABLE NS_IMETHOD Hangup(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Hangup(); } \
  NS_SCRIPTABLE NS_IMETHOD Accept(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Accept(); } \
  NS_SCRIPTABLE NS_IMETHOD SendDtmf(char tone) { return !_to ? NS_ERROR_NULL_POINTER : _to->SendDtmf(tone); } \
  NS_SCRIPTABLE NS_IMETHOD IsValidSipURI(const char *uri, PRBool *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->IsValidSipURI(uri, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetRingTone(const char *file) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetRingTone(file); } \
  NS_SCRIPTABLE NS_IMETHOD GetCallLogs(nsIList **retv NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCallLogs(retv); } \
  NS_SCRIPTABLE NS_IMETHOD ClearCallLogs(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ClearCallLogs(); } \
  NS_SCRIPTABLE NS_IMETHOD AddObserver(nsSipStateObserver *cbk) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddObserver(cbk); } \
  NS_SCRIPTABLE NS_IMETHOD RemoveObserver(nsSipStateObserver *cbk) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveObserver(cbk); } \
  NS_SCRIPTABLE NS_IMETHOD ClearObservers(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ClearObservers(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSIP : public nsISIP
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISIP

  nsSIP();

private:
  ~nsSIP();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSIP, nsISIP)

nsSIP::nsSIP()
{
  /* member initializers and constructor code */
}

nsSIP::~nsSIP()
{
  /* destructor code */
}

/* void init (in long port); */
NS_IMETHODIMP nsSIP::Init(PRInt32 port)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void destroy (); */
NS_IMETHODIMP nsSIP::Destroy()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setSipPort (in long port); */
NS_IMETHODIMP nsSIP::SetSipPort(PRInt32 port)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setRTPAudioPort (in long port); */
NS_IMETHODIMP nsSIP::SetRTPAudioPort(PRInt32 port)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setNOFirewall (); */
NS_IMETHODIMP nsSIP::SetNOFirewall()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setNATFirewall (in string fw_addr); */
NS_IMETHODIMP nsSIP::SetNATFirewall(const char *fw_addr)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setSTUNFirewall (in string stun_addr); */
NS_IMETHODIMP nsSIP::SetSTUNFirewall(const char *stun_addr)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setPresenceInfo (in long presence_status); */
NS_IMETHODIMP nsSIP::SetPresenceInfo(PRInt32 presence_status)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void call (in string URI); */
NS_IMETHODIMP nsSIP::Call(const char *URI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void hangup (); */
NS_IMETHODIMP nsSIP::Hangup()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void accept (); */
NS_IMETHODIMP nsSIP::Accept()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void sendDtmf (in char tone); */
NS_IMETHODIMP nsSIP::SendDtmf(char tone)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isValidSipURI (in string uri); */
NS_IMETHODIMP nsSIP::IsValidSipURI(const char *uri, PRBool *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setRingTone (in string file); */
NS_IMETHODIMP nsSIP::SetRingTone(const char *file)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getCallLogs ([retval] out nsIList retv); */
NS_IMETHODIMP nsSIP::GetCallLogs(nsIList **retv NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void clearCallLogs (); */
NS_IMETHODIMP nsSIP::ClearCallLogs()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void addObserver (in nsSipStateObserver cbk); */
NS_IMETHODIMP nsSIP::AddObserver(nsSipStateObserver *cbk)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeObserver (in nsSipStateObserver cbk); */
NS_IMETHODIMP nsSIP::RemoveObserver(nsSipStateObserver *cbk)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void clearObservers (); */
NS_IMETHODIMP nsSIP::ClearObservers()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISIP_h__ */
