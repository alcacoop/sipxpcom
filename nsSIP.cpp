#include "nsSIP.h"


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

/* void call (in AString URI); */
NS_IMETHODIMP nsSIP::Call(const nsAString & URI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
