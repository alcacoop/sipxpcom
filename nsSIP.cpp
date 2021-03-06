#include "nsCallLog.h"
#include "nsProxyConfig.h"
#include "nsSIP.h"


class nsRunner : public nsIRunnable 
{
public:
  NS_DECL_ISUPPORTS

  nsRunner(LinphoneCore* _lc, long* _port){
    port=_port;
    lc = _lc;
  };

  NS_IMETHOD Run(){
    while (true){
      if (!*port) return NS_OK;
      PR_Sleep(PR_MillisecondsToInterval(250));
      linphone_core_iterate(lc);
    }
  } 

private:
  LinphoneCore* lc;
  long* port;
};
NS_IMPL_THREADSAFE_ISUPPORTS1(nsRunner, nsIRunnable)



NS_IMPL_ISUPPORTS1(nsSIP, nsISIP)



nsSIP::nsSIP() : mObservers(nsnull){
  lc = NULL;
  port = 0;
  call_in_progress = 0;

  cb_table.show =(ShowInterfaceCb)stub;
  cb_table.bye_recv = (ByeReceivedCb) stub;
  cb_table.auth_info_requested = (AuthInfoRequested) stub;
  cb_table.display_message = (DisplayMessageCb)stub;
  cb_table.display_url = (DisplayUrlCb) stub;
  cb_table.text_received = (TextMessageReceived)stub;
  cb_table.refer_received = (ReferReceived)stub;
  cb_table.buddy_info_updated = (BuddyInfoUpdated) stub;

  cb_table.call_log_updated = linphonec_calllog_update;
  cb_table.new_unknown_subscriber = linphonec_new_unknown_subscriber;
  cb_table.display_status = linphonec_display_status;
  cb_table.general_state = linphonec_general_state;
  cb_table.inv_recv = linphonec_call_received;
  cb_table.dtmf_received = linphonec_dtmf_received;
  cb_table.notify_recv = linphonec_notify_received;
  cb_table.notify_presence_recv = linphonec_notify_presence_received;
}

nsSIP::~nsSIP() {
  Destroy();
}


/* void init (in long port); */
NS_IMETHODIMP nsSIP::Init(PRInt32 _port)
{
  if (port!=0){
    Destroy();
  }
  if (_port<1024)
    return NS_ERROR_ILLEGAL_VALUE;
  port = _port;

  //LINPHONE INITIALIZATION
  //lc = linphone_core_new(&cb_table, "/etc/xuldiprc", NULL, this);
  lc = linphone_core_new(&cb_table, NULL, NULL, this);
  linphone_core_set_sip_port(lc, port);
  linphone_core_set_inc_timeout(lc, 20);
  linphone_core_enable_echo_cancellation(lc, false);
  linphone_core_set_use_rfc2833_for_dtmf(lc, true);

  NS_NewThread(getter_AddRefs(mThread), new nsRunner(lc, &port));

  CallObservers("INIT", NULL);
  return NS_OK;
}


/* void destroy (); */
NS_IMETHODIMP nsSIP::Destroy() {
  if (port==0) return NS_ERROR_NOT_INITIALIZED;

  CallObservers("DESTROY", NULL);
  FlushObservers();

  //HANGUP CALLS IN PROGRESS
  if (call_in_progress){
    Hangup();
    while (call_in_progress)
      PR_Sleep(PR_MillisecondsToInterval(50));
  }

  port = 0;
  linphone_core_destroy(lc);
  lc = NULL;


  debug("THREAD SHUTDOWN..");
  mThread->Shutdown();
  debug("..OK");


  return NS_OK;
}



/* void changesipport (in long port); */
NS_IMETHODIMP nsSIP::SetSipPort(PRInt32 _port)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  port = _port;
  linphone_core_set_sip_port(lc, _port);
  return NS_OK;
}

/* void setRTPAudioPort (in long port); */
NS_IMETHODIMP nsSIP::SetRTPAudioPort(PRInt32 _port)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  
  linphone_core_set_audio_port(lc, _port);
  return NS_OK;
}

/* void setNOFirewall (); */
NS_IMETHODIMP nsSIP::SetNOFirewall()
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  
  linphone_core_set_firewall_policy(lc,LINPHONE_POLICY_NO_FIREWALL);
  debug("NO FIREWALL: %d", linphone_core_get_firewall_policy(lc));
  return NS_OK;
}

/* void setNATFirewall (in string fw_addr); */
NS_IMETHODIMP nsSIP::SetNATFirewall(const char *fw_addr)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  
  linphone_core_set_nat_address(lc, fw_addr);
  linphone_core_set_firewall_policy(lc,LINPHONE_POLICY_USE_NAT_ADDRESS);
  debug("NAT ADDRESS: %s - %d", linphone_core_get_nat_address(lc), linphone_core_get_firewall_policy(lc));
  
  return NS_OK;
}

/* void setSTUNFirefall (in string stun_addr); */
NS_IMETHODIMP nsSIP::SetSTUNFirewall(const char *stun_addr)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  
  linphone_core_set_stun_server(lc, stun_addr);
  linphone_core_set_firewall_policy(lc, LINPHONE_POLICY_USE_STUN);
  debug("STUN SERVER: %s - %d", linphone_core_get_stun_server(lc), linphone_core_get_firewall_policy(lc));

  return NS_OK;
}

/* void setPrimaryIdentity (in string username); */
NS_IMETHODIMP nsSIP::SetPrimaryIdentity(const char *username)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  
  linphone_core_set_primary_contact(lc, username);
  return NS_OK;
}



/* void getCurrentIdentity ([retval] out long current); */
NS_IMETHODIMP nsSIP::GetCurrentIdentity(PRInt32 *current NS_OUTPARAM)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  LinphoneProxyConfig *cfg;
  linphone_core_get_default_proxy(lc, &cfg);
  if (cfg == NULL) *current = 0;
  else *current = 1;
  return NS_OK;
}

/* void getIdentity (in long identity_num, [retval] out ACString username); */
NS_IMETHODIMP nsSIP::GetIdentity(PRInt32 identity_num, nsACString & username NS_OUTPARAM)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;

  const char* identity;
  if (identity_num == 0){
    identity = linphone_core_get_primary_contact(lc);
    username = Substring(identity, (PRUint32)strlen(identity));
  } else {
    const MSList *proxies;
    LinphoneProxyConfig *cfg;
    proxies=linphone_core_get_proxy_config_list(lc);
    if (proxies==NULL){
      debug("No such proxy");
      return NS_ERROR_FAILURE;
    } else {
      cfg=(LinphoneProxyConfig*)ms_list_nth_data(proxies, identity_num-1);
      identity = linphone_proxy_config_get_identity(cfg); 
      char* tmp = (char *)calloc(sizeof(char*), strlen(identity)+3);
      sprintf(tmp, "<%s>", identity);
      username = Substring(tmp, (PRUint32)strlen(tmp));
      free(tmp);
    }
  }

  return NS_OK;
}

/* void setIdentity (in long identity_num); */
NS_IMETHODIMP nsSIP::SetIdentity(PRInt32 identity_num)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  
  switch (identity_num){
    case 0:
      linphone_core_set_default_proxy(lc,NULL);
      break;
    case 1:
      const MSList *proxies;
      LinphoneProxyConfig *cfg;
      proxies=linphone_core_get_proxy_config_list(lc);
      if (proxies==NULL){
        debug("No such proxy");
        return NS_ERROR_FAILURE;
      } else {
        cfg=(LinphoneProxyConfig*)ms_list_nth_data(proxies,0);
        linphone_core_set_default_proxy(lc, cfg);
      }
      break;
  }
  return NS_OK;
}


/* void addFriend (in ACString name, in ACString addr); */
NS_IMETHODIMP nsSIP::AddFriend(const nsACString & name, const nsACString & addr)
{
	LinphoneFriend *newFriend;

	newFriend = linphone_friend_new();
  linphone_friend_set_sip_addr(newFriend, ToNewCString(addr));
  linphone_friend_set_name(newFriend, ToNewCString(name));
  linphone_friend_send_subscribe(newFriend, true);
  if (!newFriend)
    return NS_ERROR_FAILURE;

	linphone_core_add_friend(lc, newFriend);

  /*
int linphone_proxy_config_send_publish(LinphoneProxyConfig *cfg, LinphoneOnlineStatus os);
int linphone_online_status_to_eXosip(LinphoneOnlineStatus os);
  linphone_friend_notify(newFriend, LINPHONE_STATUS_ONLINE);
void linphone_friend_close_subscriptions(LinphoneFriend *lf);
*/

  return NS_OK;
}

/*
static int linphonec_friend_delete(LinphoneCore *lc, int num)
{
	const MSList *friend = linphone_core_get_friend_list(lc);
	unsigned int n;

	for(n=0; friend!=NULL; friend=ms_list_next(friend), ++n )
	{
		if ( n == num )
		{
			linphone_core_remove_friend(lc, friend->data);
			return 0;
		}
	}

	if (-1 == num) 
	{
		unsigned int i;
		for (i = 0 ; i < n ; i++)
			linphonec_friend_delete(lc, 0);
		return 0;
	}

	linphonec_out("No such friend %u\n", num);
	return 1;
}
*/

/* void setPresenceInfo (); */
NS_IMETHODIMP nsSIP::SetPresenceInfo(PRInt32 presence_status)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  
  LinphoneOnlineStatus status;
  switch (presence_status){
    case 0:
      status = LINPHONE_STATUS_OFFLINE;
      break;
    case 1:
      status = LINPHONE_STATUS_ONLINE;
      break;
    case 2:
      status = LINPHONE_STATUS_BUSY;
      break;
  }

  linphone_core_set_presence_info(lc, 0, NULL, status);

  /*
  LinphoneProxyConfig *proxy = NULL;
  linphone_core_get_default_proxy(lc, &proxy);
  if (proxy) linphone_proxy_config_send_publish(proxy, status);
  */

  return NS_OK;
}



/* void setProxyConfig (in nsIProxyConfig cfg); */
NS_IMETHODIMP nsSIP::SetProxyConfig(nsIProxyConfig *cfg)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  
  int exist = 0;

  nsCString sip_identity;
  cfg->GetSip_identity(sip_identity);
  nsCString sip_proxy;
  cfg->GetSip_proxy(sip_proxy);
  nsCString sip_route;
  cfg->GetSip_route(sip_route);
  nsCString userid;
  cfg->GetUserid(userid);
  nsCString password;
  cfg->GetPassword(password);
  PRInt32 duration;
  cfg->GetDuration(&duration);

  const MSList *proxies;
  LinphoneProxyConfig *proxy;
  proxies = linphone_core_get_proxy_config_list(lc);
  if (proxies == NULL) {
    proxy = linphone_proxy_config_new();
  } else {
    proxy = (LinphoneProxyConfig*)ms_list_nth_data(proxies,0);
    linphone_proxy_config_edit(proxy);
    exist = 1;
  }
  
  linphone_proxy_config_set_identity(proxy, ToNewCString(sip_identity));
  linphone_proxy_config_set_server_addr(proxy, ToNewCString(sip_proxy));
  linphone_proxy_config_set_route(proxy, ToNewCString(sip_route));
  linphone_proxy_config_enable_register(proxy, false);
  linphone_proxy_config_expires(proxy, duration);
  linphone_proxy_config_enable_publish(proxy, true);

  if (!exist) {
    linphone_core_add_proxy_config(lc, proxy);
    linphone_core_set_default_proxy(lc, proxy);
  } else {
    linphone_proxy_config_done(proxy);
  }
  
  linphone_core_clear_all_auth_info(lc);
  LinphoneAuthInfo *auth = linphone_auth_info_new(ToNewCString(userid), ToNewCString(userid), ToNewCString(password), NULL, NULL);
  linphone_core_add_auth_info(lc, auth);

  debug("Proxy configuration");
  debug("  Object address: %p", &proxy);
  debug("  SIP Proxy: %s", ToNewCString(sip_proxy));
  debug("  SIP Identity: %s", ToNewCString(sip_identity));
  debug("  SIP Route: %s", ToNewCString(sip_route));
  debug("  UserId: %s", ToNewCString(userid));
  debug("  Password: %s", ToNewCString(password));
  debug("  Reg. duration: %d", duration);

  return NS_OK;
}

/* void getProxyConfig ([retval] out nsIProxyConfig cfg); */
NS_IMETHODIMP nsSIP::GetProxyConfig(nsIProxyConfig **cfg NS_OUTPARAM)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  
  LinphoneProxyConfig *proxy = NULL;
  linphone_core_get_default_proxy(lc, &proxy);
  if (!proxy) return NS_OK;
  nsCOMPtr<nsIProxyConfig> _cfg = do_CreateInstance(PROXYCFG_CONTRACTID);
  NS_ADDREF(_cfg);

  const char* str;
  str = linphone_proxy_config_get_identity(proxy);
  _cfg->SetSip_identity(Substring(str, (PRUint32)strlen(str)));
  str = linphone_proxy_config_get_addr(proxy);
  _cfg->SetSip_proxy(Substring(str, (PRUint32)strlen(str)));
  str = linphone_proxy_config_get_route(proxy);
  _cfg->SetSip_route(Substring(str, (PRUint32)strlen(str)));
  _cfg->SetDuration(linphone_proxy_config_get_expires(proxy));
  
  const MSList* auths = linphone_core_get_auth_info_list(lc);
  const LinphoneAuthInfo* auth = (LinphoneAuthInfo*)ms_list_nth_data(auths,0);
  str = linphone_auth_info_get_username(auth);
  _cfg->SetUserid(Substring(str, (PRUint32)strlen(str)));
  str = linphone_auth_info_get_passwd(auth);
  _cfg->SetPassword(Substring(str, (PRUint32)strlen(str)));

  *cfg = _cfg;
  return NS_OK;
}

/* void registerToProxy (); */
NS_IMETHODIMP nsSIP::RegisterToProxy()
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  
  LinphoneProxyConfig *proxy;
  linphone_core_get_default_proxy(lc, &proxy);
  if (!proxy) return NS_ERROR_FAILURE;
  linphone_proxy_config_edit(proxy);
  linphone_proxy_config_enable_register(proxy, true);
  linphone_proxy_config_done(proxy);
    
  return NS_OK;
}

/* void unregisterToProxy (); */
NS_IMETHODIMP nsSIP::UnregisterToProxy()
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  
  LinphoneProxyConfig *proxy;
  linphone_core_get_default_proxy(lc, &proxy);
  if (!proxy) return NS_ERROR_FAILURE;
  linphone_proxy_config_edit(proxy);
  linphone_proxy_config_enable_register(proxy, false);
  linphone_proxy_config_done(proxy);
    
  return NS_OK;
}




/* void call (in AString URI); */
NS_IMETHODIMP nsSIP::Call(const char* URI) {

  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  
  PRBool valid_URI;
  IsValidSipURI(URI, &valid_URI);
  if (!valid_URI) {
    CallObservers("INVALIDURI", NULL);
    return NS_ERROR_FAILURE;
  }

  if (!call_in_progress)
    linphone_core_invite(lc, URI);
  return NS_OK;
}


/* void hangup (); */
NS_IMETHODIMP nsSIP::Hangup() {
  if (call_in_progress)
    linphone_core_terminate_call(lc, NULL);
  else
    CallObservers("HANGUP", NULL);
  return NS_OK;
}


/* void accept (); */
NS_IMETHODIMP nsSIP::Accept()
{
  linphone_core_accept_call(lc, NULL);
  return NS_OK;
}


/* void senddtmf (in char tone); */
NS_IMETHODIMP nsSIP::SendDtmf(char tone)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  if (call_in_progress)
    linphone_core_send_dtmf(lc, tone);
  
  return NS_OK;
}



/* boolean checkSipURI (in string uri); */
NS_IMETHODIMP nsSIP::IsValidSipURI(const char *uri, PRBool *_retval NS_OUTPARAM)
{
  LinphoneAddress* addr = linphone_address_new(uri);
  if (addr) {
    *_retval = true;
    linphone_address_destroy(addr);
  }
  else *_retval = false;

  return NS_OK;
}



/* void setringtone (in string file); */
NS_IMETHODIMP nsSIP::SetRingTone(const char *file)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  linphone_core_set_ring(lc, file);
  return NS_OK;
}

/* void setRingbackTone (in string file); */
NS_IMETHODIMP nsSIP::SetRingbackTone(const char *file)
{
  if (port==0) return NS_ERROR_NOT_INITIALIZED;
  linphone_core_set_ringback(lc, file);
  return NS_OK;
}


/* void getPlayLevel ([retval] out short level); */
NS_IMETHODIMP nsSIP::GetPlayLevel(PRInt16 *level NS_OUTPARAM)
{
  MSSndCard *sndcard;
  sndcard=lc->sound_conf.play_sndcard;
  if (sndcard) *level = ms_snd_card_get_level(sndcard, MS_SND_CARD_MASTER);
  return NS_OK;
}

/* void setPlayLevel (in short level); */
NS_IMETHODIMP nsSIP::SetPlayLevel(PRInt16 level)
{
  if (level<0) level = 0;
  if (level>100) level = 100;

  //HACK!
  MSSndCard *sndcard;
  sndcard=lc->sound_conf.play_sndcard;
  if (sndcard) ms_snd_card_set_level(sndcard,MS_SND_CARD_MASTER,level);
  linphone_core_set_play_level(lc, level);
  
  return NS_OK;
}

/* void getMicLevel ([retval] out short level); */
NS_IMETHODIMP nsSIP::GetMicLevel(PRInt16 *level NS_OUTPARAM)
{

  MSSndCard *sndcard;
  sndcard=lc->sound_conf.capt_sndcard;
  *level = sndcard->desc->get_level(sndcard, MS_SND_CARD_CAPTURE);

  return NS_OK;
}

/* void setMicLevel (in short level); */
NS_IMETHODIMP nsSIP::SetMicLevel(PRInt16 level)
{
  MSSndCard *sndcard;
  sndcard=lc->sound_conf.capt_sndcard;
  if (sndcard) ms_snd_card_set_level(sndcard, MS_SND_CARD_CAPTURE, level);
  linphone_core_set_rec_level(lc, level);
  return NS_OK;
}


/* void getCodec (in short index, [retval] out ACString codec); */
NS_IMETHODIMP nsSIP::GetCodec(PRInt16 index, nsACString & codec NS_OUTPARAM)
{
	PayloadType *pt;
  codecs_config_t *config = &lc->codecs_conf;
	int _index = 0;
	MSList *node = config->audio_codecs;

	for(;node!=NULL;node=ms_list_next(node)){
		pt=(PayloadType*)(node->data);
    if (index == _index) { 
      char* tmp = (char *)calloc(sizeof(char*), 200);
      sprintf(tmp, "%s (%d) %s", pt->mime_type, pt->clock_rate, linphone_core_payload_type_enabled(lc,pt) ? "enabled" : "disabled");
      codec = Substring(tmp, (PRUint32)strlen(tmp));
      free(tmp);
    }

		_index++;
	}
  return NS_OK;
}


/* void enableCodec (in short codec); */
NS_IMETHODIMP nsSIP::EnableCodec(PRInt16 codec)
{
	PayloadType *pt;
  codecs_config_t *config=&lc->codecs_conf;
	int index=0;
	MSList *node=config->audio_codecs;

  for(;node!=NULL;node=ms_list_next(node)){
    if (index == codec || codec == -1) {
      pt=(PayloadType*)(node->data);
      pt->flags|=PAYLOAD_TYPE_ENABLED;
      debug("%2d: %s (%d) %s", index, pt->mime_type, pt->clock_rate, "enabled");
    }
		index++;
	}
    
  return NS_OK;
}

/* void disableCodec (in short codec); */
NS_IMETHODIMP nsSIP::DisableCodec(PRInt16 codec)
{
	PayloadType *pt;
  codecs_config_t *config=&lc->codecs_conf;
	int index=0;
	MSList *node=config->audio_codecs;

  for(;node!=NULL;node=ms_list_next(node)){
    if (index == codec || codec == -1) {
      pt=(PayloadType*)(node->data);
      pt->flags&=~PAYLOAD_TYPE_ENABLED;
      debug("%2d: %s (%d) %s", index, pt->mime_type, pt->clock_rate, "disabled");
    }
		index++;
	}
    
  return NS_OK;
}


//NS_IMETHODIMP nsSIP::GetCallLogs()
NS_IMETHODIMP nsSIP::GetCallLogs(nsIList **retv NS_OUTPARAM)
{

  nsCOMPtr<nsIList> nl; 
  nl = do_CreateInstance(LIST_CONTRACTID);
  NS_ADDREF(nl);

  const MSList *elem=linphone_core_get_call_logs(lc);
  for (;elem!=NULL;elem=ms_list_next(elem))
  {
    LinphoneCallLog *_cl=(LinphoneCallLog*)elem->data;

    nsCOMPtr<nsICallLog> cl = do_CreateInstance(LOG_CONTRACTID);
    NS_ADDREF(cl);

    char* from = linphone_address_as_string(_cl->from);
    char* to = linphone_address_as_string(_cl->to);

    cl->SetDirection(_cl->dir);
    cl->SetStatus(_cl->status);
    cl->SetDuration(_cl->duration);

    cl->SetDate(Substring(_cl->start_date, (PRUint32)strlen(_cl->start_date)));
    cl->SetFrom(Substring(from, (PRUint32)strlen(from)));
    cl->SetTo(Substring(to, (PRUint32)strlen(to)));

    ms_free(from);
    ms_free(to);

    nl->Add(cl);    
  }

  *retv = nl;
  return NS_OK; 
}


/* void clearCallLogs (); */
NS_IMETHODIMP nsSIP::ClearCallLogs()
{
  linphone_core_clear_call_logs(lc);
  CallObservers("UPDATELOG", NULL);
  return NS_OK;
}


/* ************************************* *
 *          GESTIONE OBSERVERS           *
 * ************************************* */     

/* void addObserver (in nsSipStateObserver cbk); */
NS_IMETHODIMP nsSIP::AddObserver(nsSipStateObserver *cbk)
{
  NS_ENSURE_ARG_POINTER(cbk);

  if (!mObservers) {
    mObservers = do_CreateInstance(NS_ARRAY_CONTRACTID);
    NS_ENSURE_STATE(mObservers);
  }

  /* TEST IF cbk OBSERVER IS ALREADY REGISTERED */
  PRUint32 count = 0;
  PRIntn i;
  mObservers->GetLength(&count);
  nsCOMPtr<nsSipStateObserver> pCallback;
  for (i = 0; i < count; ++i) {
    (nsIArray*)mObservers->QueryElementAt(i, NS_GET_IID(nsSipStateObserver), (void**)&pCallback);
    if (pCallback == cbk){
      debug("OBSERVER ALREADY REGISTERED");
      return NS_OK;
    }
  }

  NS_IF_ADDREF(cbk);
  mObservers->AppendElement(cbk, PR_FALSE);
  debug("ADDED OBSERVER AT ADDR: %p", cbk);
  return NS_OK;
}


/* void removeObserver (in nsSipStateObserver cbk); */
NS_IMETHODIMP nsSIP::RemoveObserver(nsSipStateObserver *cbk)
{
  NS_ENSURE_ARG_POINTER(cbk);

  if (!mObservers){
    debug("NO SUCH OBSERVER");
    return NS_OK;
  }

  PRUint32 count = 0;
  mObservers->GetLength(&count);
  if (count <= 0){
    debug("NO SUCH OBSERVER");
    return NS_OK;
  }

  PRIntn i;
  nsCOMPtr<nsSipStateObserver> pCallback;

  for (i = 0; i < count; ++i) {
    (nsIArray*)mObservers->QueryElementAt(i, NS_GET_IID(nsSipStateObserver), (void**)&pCallback);
    if (pCallback == cbk){
      mObservers->RemoveElementAt(i);
      debug("REMOVED OBSERVER AT ADDR: %p", (nsSipStateObserver*)pCallback);
      NS_RELEASE(pCallback);
      return NS_OK;
    }
  }

  debug("NO SUCH OBSERVER");
  return NS_OK;
}



void nsSIP::FlushObservers(){
  if (!mObservers)
      return;

  PRUint32 count = 0;
  mObservers->GetLength(&count);

  PRIntn i;
  nsCOMPtr<nsSipStateObserver> pCallback;

  for (i = 0; i < count; i++) {
    (nsIArray*)mObservers->QueryElementAt(0, NS_GET_IID(nsSipStateObserver), (void**)&pCallback);
    mObservers->RemoveElementAt(0);
    debug("REMOVED OBSERVER AT ADDR: %p", (nsSipStateObserver*)pCallback);
    NS_RELEASE(pCallback);
    debug("(MEMORY RELEASED)");
  }

  debug("FLUSHED ALL %d OBSERVERS", count);
  NS_RELEASE(mObservers);
  return;
}


/* void clearObservers (); */
NS_IMETHODIMP nsSIP::ClearObservers()
{
  FlushObservers();
  return NS_OK;
}



void nsSIP::getProxyForObserver(nsCOMPtr<nsSipStateObserver> cbk, nsCOMPtr<nsSipStateObserver> *pCbk){
  nsresult rv = NS_OK;
  nsCOMPtr<nsIProxyObjectManager> pIProxyObjectManager(do_GetService("@mozilla.org/xpcomproxy;1", &rv));

  rv = pIProxyObjectManager->GetProxyForObject(
    NS_PROXY_TO_MAIN_THREAD,
    nsSipStateObserver::GetIID(),
    cbk,
    NS_PROXY_SYNC | NS_PROXY_ALWAYS,
    (void**)pCbk
  );
}


void nsSIP::CallObservers(const char* status, const char* data)
{
  if (!mObservers)
      return;

  PRUint32 count = 0;
  mObservers->GetLength(&count);
  if (count <= 0)
      return;

  PRIntn i;
  nsCOMPtr<nsSipStateObserver> _pCallback;
  for (i = 0; i < count; ++i) {
    (nsIArray*)mObservers->QueryElementAt(i, NS_GET_IID(nsSipStateObserver), (void**)&_pCallback);
    nsCOMPtr<nsSipStateObserver> pCbk;
    getProxyForObserver(_pCallback, &pCbk);
    NS_IF_ADDREF(pCbk);
    pCbk->OnStatusChange(status, data);
  }

  return;
}
