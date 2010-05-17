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

/* starting interface:    nsISIP */
#define NS_ISIP_IID_STR "c5bf5079-8bc3-4fc5-881f-f3bd85a63c61"

#define NS_ISIP_IID \
  {0xc5bf5079, 0x8bc3, 0x4fc5, \
    { 0x88, 0x1f, 0xf3, 0xbd, 0x85, 0xa6, 0x3c, 0x61 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsISIP : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISIP_IID)

  /* attribute long port; */
  NS_SCRIPTABLE NS_IMETHOD GetPort(PRInt32 *aPort) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetPort(PRInt32 aPort) = 0;

  /* void init (in long port); */
  NS_SCRIPTABLE NS_IMETHOD Init(PRInt32 port) = 0;

  /* void call (in string URI); */
  NS_SCRIPTABLE NS_IMETHOD Call(const char *URI) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISIP, NS_ISIP_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISIP \
  NS_SCRIPTABLE NS_IMETHOD GetPort(PRInt32 *aPort); \
  NS_SCRIPTABLE NS_IMETHOD SetPort(PRInt32 aPort); \
  NS_SCRIPTABLE NS_IMETHOD Init(PRInt32 port); \
  NS_SCRIPTABLE NS_IMETHOD Call(const char *URI); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISIP(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetPort(PRInt32 *aPort) { return _to GetPort(aPort); } \
  NS_SCRIPTABLE NS_IMETHOD SetPort(PRInt32 aPort) { return _to SetPort(aPort); } \
  NS_SCRIPTABLE NS_IMETHOD Init(PRInt32 port) { return _to Init(port); } \
  NS_SCRIPTABLE NS_IMETHOD Call(const char *URI) { return _to Call(URI); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISIP(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetPort(PRInt32 *aPort) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPort(aPort); } \
  NS_SCRIPTABLE NS_IMETHOD SetPort(PRInt32 aPort) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPort(aPort); } \
  NS_SCRIPTABLE NS_IMETHOD Init(PRInt32 port) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(port); } \
  NS_SCRIPTABLE NS_IMETHOD Call(const char *URI) { return !_to ? NS_ERROR_NULL_POINTER : _to->Call(URI); } 

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

/* attribute long port; */
NS_IMETHODIMP nsSIP::GetPort(PRInt32 *aPort)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSIP::SetPort(PRInt32 aPort)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void init (in long port); */
NS_IMETHODIMP nsSIP::Init(PRInt32 port)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void call (in string URI); */
NS_IMETHODIMP nsSIP::Call(const char *URI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISIP_h__ */
