#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linphone/linphonecore.h"

#include "nsCOMPtr.h"
#include "nsIArray.h"
#include "nsIMutableArray.h"
#include "nsWeakPtr.h"
#include "nsServiceManagerUtils.h"
#include "nsComponentManagerUtils.h"
#include "nsISupports.h"
#include "nsStringAPI.h"
#include "nsMemory.h"

#include "nsXPCOMCIDInternal.h"

#include "nsISIP.h"


#define LIST_CONTRACTID "@alcacoop.it/voip/call-log-list;1"
#define LIST_CLASSNAME "Simple List"
#define LIST_CID  { 0xfe4d239b, 0x8766, 0x4386, { 0x99, 0x9d, 0xbc, 0xd6, 0x89, 0x03, 0xeb, 0x71 } }


#define LOG_CONTRACTID "@alcacoop.it/voip/call-log;1"
#define LOG_CLASSNAME "Simple Call Log"
#define LOG_CID  { 0x248eaceb, 0x8910, 0x49a5, { 0xa7, 0x75, 0xf0, 0x6b, 0x62, 0xea, 0xa2, 0xcd } }



class nsList : public nsIList
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSILIST
  nsList();
  void Add(LinphoneCallLog* elem);

private:
  ~nsList();
  nsCOMPtr<nsIMutableArray> mList;
  int length;
};




class nsCallLog : public nsICallLog
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSICALLLOG

  nsCallLog();

private:
  ~nsCallLog();
  short int direction;
  short int status;
  long int duration;
  nsCString from; 
  nsCString to;
  nsCString date;
};
