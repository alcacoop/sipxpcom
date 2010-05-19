/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM nsISIP.idl
 */

#ifndef __gen_nsISIP_h__
#define __gen_nsISIP_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
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


/* starting interface:    nsISIP */
#define NS_ISIP_IID_STR "c5bf5079-8bc3-4fc5-881f-f3bd85a63c61"

#define NS_ISIP_IID \
  {0xc5bf5079, 0x8bc3, 0x4fc5, \
    { 0x88, 0x1f, 0xf3, 0xbd, 0x85, 0xa6, 0x3c, 0x61 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsISIP : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISIP_IID)

  /* void init (in long port, in nsSipStateObserver cbk); */
  NS_SCRIPTABLE NS_IMETHOD Init(PRInt32 port, nsSipStateObserver *cbk) = 0;

  /* void destroy (); */
  NS_SCRIPTABLE NS_IMETHOD Destroy(void) = 0;

  /* void call (in string URI); */
  NS_SCRIPTABLE NS_IMETHOD Call(const char *URI) = 0;

  /* void hangup (); */
  NS_SCRIPTABLE NS_IMETHOD Hangup(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISIP, NS_ISIP_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISIP \
  NS_SCRIPTABLE NS_IMETHOD Init(PRInt32 port, nsSipStateObserver *cbk); \
  NS_SCRIPTABLE NS_IMETHOD Destroy(void); \
  NS_SCRIPTABLE NS_IMETHOD Call(const char *URI); \
  NS_SCRIPTABLE NS_IMETHOD Hangup(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISIP(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(PRInt32 port, nsSipStateObserver *cbk) { return _to Init(port, cbk); } \
  NS_SCRIPTABLE NS_IMETHOD Destroy(void) { return _to Destroy(); } \
  NS_SCRIPTABLE NS_IMETHOD Call(const char *URI) { return _to Call(URI); } \
  NS_SCRIPTABLE NS_IMETHOD Hangup(void) { return _to Hangup(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISIP(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(PRInt32 port, nsSipStateObserver *cbk) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(port, cbk); } \
  NS_SCRIPTABLE NS_IMETHOD Destroy(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Destroy(); } \
  NS_SCRIPTABLE NS_IMETHOD Call(const char *URI) { return !_to ? NS_ERROR_NULL_POINTER : _to->Call(URI); } \
  NS_SCRIPTABLE NS_IMETHOD Hangup(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Hangup(); } 

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

/* void init (in long port, in nsSipStateObserver cbk); */
NS_IMETHODIMP nsSIP::Init(PRInt32 port, nsSipStateObserver *cbk)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void destroy (); */
NS_IMETHODIMP nsSIP::Destroy()
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

/* End of implementation class template. */
#endif


#endif /* __gen_nsISIP_h__ */
