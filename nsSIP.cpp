#include "nsSIP.h"
#include "pjsip.h"
#include "nsStringAPI.h"


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
  *aPort = port;
  return NS_OK;
}
NS_IMETHODIMP nsSIP::SetPort(PRInt32 aPort)
{
  port = aPort;
  return NS_OK;
}

/* void init (in long port); */
NS_IMETHODIMP nsSIP::Init(PRInt32 port)
{
  
  sipregister((int)port);
  return NS_OK;
}

/* void destroy (); */
NS_IMETHODIMP nsSIP::Destroy()
{
  sipderegister();
  return NS_OK;
}

/* void call (in AString URI); */
NS_IMETHODIMP nsSIP::Call(const char* URI)
{
  sipmakecall((char*)URI);
  return NS_OK;
}

/* void hangup (); */
NS_IMETHODIMP nsSIP::Hangup()
{
  siphangup();
  return NS_OK;
}


void state_handler(char* status){
  /*
  Possible status are:
  "CALLING"
  "ANSWER"
  "HANGUP"
  */
  printf(">%s\n", status);
}; 
