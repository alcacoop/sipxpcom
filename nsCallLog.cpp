#include "nsCallLog.h"

/* ************************* */
/*           nsList          */
/* ************************* */
NS_IMPL_ISUPPORTS1(nsList, nsIList)

nsList::nsList(): mList(nsnull)
{
  length = 0;
  if (!mList) {
    mList = do_CreateInstance(NS_ARRAY_CONTRACTID);
  }
}

nsList::~nsList(){}


/* nsICallLog item (in unsigned long index); */
NS_IMETHODIMP nsList::Item(PRUint32 index, nsICallLog **_retval NS_OUTPARAM)
{
  if (index >= length)
    return NS_OK;

  nsCOMPtr<nsCallLog> cl;
  (nsIArray*)mList->QueryElementAt(index, NS_GET_IID(nsCallLog), getter_AddRefs(cl));
  *_retval = cl;
  return NS_OK;
}

/* readonly attribute unsigned long length; */
NS_IMETHODIMP nsList::GetLength(PRUint32 *aLength)
{
  *aLength = length;
  return NS_OK;
}

/* add call log to list */
/* [noscriptable] void add (in nsICallLog cl); */
NS_IMETHODIMP nsList::Add(nsICallLog *cl)
{
  NS_ENSURE_ARG_POINTER(cl);
  length++;
  mList->AppendElement(cl, PR_FALSE);
};






/* ************************* */
/*        nsCallLog          */
/* ************************* */
NS_IMPL_ISUPPORTS1(nsCallLog, nsICallLog)

nsCallLog::nsCallLog(): date(nsnull), from(nsnull), to(nsnull)
{
  duration = -1;
  status = -1;
  direction = -1;
}

nsCallLog::~nsCallLog(){}



/* attribute unsigned short direction; */
NS_IMETHODIMP nsCallLog::GetDirection(PRInt16 *aDirection)
{
  *aDirection = direction;
  return NS_OK;
}
NS_IMETHODIMP nsCallLog::SetDirection(PRInt16 aDirection)
{
  direction = aDirection;
  return NS_OK;
}

/* attribute unsigned short status; */
NS_IMETHODIMP nsCallLog::GetStatus(PRInt16 *aStatus)
{
  *aStatus = status;
  return NS_OK;
}
NS_IMETHODIMP nsCallLog::SetStatus(PRInt16 aStatus)
{
  status = aStatus;
  return NS_OK;
}

/* attribute unsigned long duration; */
NS_IMETHODIMP nsCallLog::GetDuration(PRInt32 *aDuration)
{
  *aDuration = duration;
  return NS_OK;
}
NS_IMETHODIMP nsCallLog::SetDuration(PRInt32 aDuration)
{
  duration = aDuration;
  return NS_OK;
}


/* attribute ACString date; */
NS_IMETHODIMP nsCallLog::GetDate(nsACString & aDate)
{
  aDate = date;
  return NS_OK;
}
NS_IMETHODIMP nsCallLog::SetDate(const nsACString & aDate)
{
  date = aDate;
  return NS_OK;
}

/* attribute ACString from; */
NS_IMETHODIMP nsCallLog::GetFrom(nsACString & aFrom)
{
  aFrom = from;
  return NS_OK;
}
NS_IMETHODIMP nsCallLog::SetFrom(const nsACString & aFrom)
{
  from = aFrom;
  return NS_OK;
}

/* attribute ACString to; */
NS_IMETHODIMP nsCallLog::GetTo(nsACString & aTo)
{
  aTo = to;
  return NS_OK;
}
NS_IMETHODIMP nsCallLog::SetTo(const nsACString & aTo)
{
  to = aTo;
  return NS_OK;
}

