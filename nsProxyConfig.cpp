#include "nsProxyConfig.h"

NS_IMPL_ISUPPORTS1(nsProxyConfig, nsIProxyConfig)

nsProxyConfig::nsProxyConfig()
{
  /* member initializers and constructor code */
}

nsProxyConfig::~nsProxyConfig()
{
  /* destructor code */
}



/* attribute ACString sip_identity; */
NS_IMETHODIMP nsProxyConfig::GetSip_identity(nsACString & aSip_identity)
{
  aSip_identity = sip_identity;
  return NS_OK;
}
NS_IMETHODIMP nsProxyConfig::SetSip_identity(const nsACString & aSip_identity)
{
  sip_identity = aSip_identity;
  return NS_OK;
}

/* attribute ACString sip_proxy; */
NS_IMETHODIMP nsProxyConfig::GetSip_proxy(nsACString & aSip_proxy)
{
  aSip_proxy = sip_proxy;
  return NS_OK;
}
NS_IMETHODIMP nsProxyConfig::SetSip_proxy(const nsACString & aSip_proxy)
{
  sip_proxy = aSip_proxy;
  return NS_OK;
}

/* attribute ACString sip_route; */
NS_IMETHODIMP nsProxyConfig::GetSip_route(nsACString & aSip_route)
{
  aSip_route = sip_route; 
  return NS_OK;
}
NS_IMETHODIMP nsProxyConfig::SetSip_route(const nsACString & aSip_route)
{
  sip_route = aSip_route;
  return NS_OK;
}

/* attribute ACString userid; */
NS_IMETHODIMP nsProxyConfig::GetUserid(nsACString & aUserid)
{
  aUserid = userid;
  return NS_OK;
}
NS_IMETHODIMP nsProxyConfig::SetUserid(const nsACString & aUserid)
{
  userid = aUserid;
  return NS_OK;
}

/* attribute ACString password; */
NS_IMETHODIMP nsProxyConfig::GetPassword(nsACString & aPassword)
{
  aPassword = password;
  return NS_OK;
}
NS_IMETHODIMP nsProxyConfig::SetPassword(const nsACString & aPassword)
{
  password = aPassword;
  return NS_OK;
}

/* attribute long duration; */
NS_IMETHODIMP nsProxyConfig::GetDuration(PRInt32 *aDuration)
{
  *aDuration = duration;
  return NS_OK;
}
NS_IMETHODIMP nsProxyConfig::SetDuration(PRInt32 aDuration)
{
  duration = aDuration;
  return NS_OK;
}
