#include "pjsip.h"




/*
   Array di callback proxies
*/
static nsCOMPtr<nsIArray> mObservers;


/* 
   Callback called by the library upon receiving incoming call 
*/
static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata){
  pjsua_call_info ci;
  PJ_UNUSED_ARG(acc_id);
  PJ_UNUSED_ARG(rdata);
  pjsua_call_get_info(call_id, &ci);
  PJ_LOG(3,(THIS_FILE, "Incoming call from %.*s!!",
        (int)ci.remote_info.slen,
        ci.remote_info.ptr));
  pjsua_call_answer(call_id, 200, NULL, NULL);
}


/* 
   Callback called by the library when call's state has changed 
*/
static void on_call_state(pjsua_call_id call_id, pjsip_event *e){
  pjsua_call_info ci;
  PJ_UNUSED_ARG(e);
  pjsua_call_get_info(call_id, &ci);
  int code;
  pj_str_t reason;
  pjsip_msg *msg;

  printf("\n->STATO: %s - %d\n",ci.state_text.ptr, ci.state);
  if (e->body.tsx_state.type == PJSIP_EVENT_RX_MSG) {
    msg = e->body.tsx_state.src.rdata->msg_info.msg;
  } else {
    msg = e->body.tsx_state.src.tdata->msg;
  }
  code = msg->line.status.code;
  reason = msg->line.status.reason;
  //printf("CODE: %d - REASON: %s\n", code, reason);

  if (code==180)
    CallObservers("RINGING");
  if (code==603)
    CallObservers("DECLINE");
  if (strcmp(ci.state_text.ptr, "CALLING")==0)
    CallObservers("CALLING");
  if (strcmp(ci.state_text.ptr, "CONFIRMED")==0)
    CallObservers("ANSWER");
  if (strcmp(ci.state_text.ptr, "DISCONNCTD")==0){
    CallObservers("HANGUP");
    siphangup();
  }
  
}


/* 
   Callback called by the library when call's media state has changed 
*/
static void on_call_media_state(pjsua_call_id call_id){
  pjsua_call_info ci;
  pjsua_call_get_info(call_id, &ci);
  if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
    // When media is active, connect call to sound device.
    pjsua_conf_connect(ci.conf_slot, 0);
    pjsua_conf_connect(0, ci.conf_slot);
  }
}


/*
   Stack, socket e thread constructor
*/
PJSIP_API int sipregister(long sipPort) {

  pj_status_t status;
  pjsua_acc_id acc_id;
  REGISTER_THREAD();


  status = pjsua_create();

  if (status != PJ_SUCCESS){ pjsua_destroy();}
  {
    pjsua_config cfg;
    pjsua_logging_config log_cfg;
    pjsua_media_config m_cfg;

    cfg.user_agent = pj_str((char*)"pjsip");
    cfg.thread_cnt = 1;

    cfg.use_srtp = PJMEDIA_SRTP_DISABLED;
    
    pjsua_config_default(&cfg);
    cfg.cb.on_incoming_call = &on_incoming_call;
    cfg.cb.on_call_media_state = &on_call_media_state;
    cfg.cb.on_call_state = &on_call_state;
    pjsua_logging_config_default(&log_cfg);
    log_cfg.console_level = 1;

    pjsua_media_config_default(&m_cfg);
    m_cfg.no_vad = PJ_TRUE;
    m_cfg.ec_tail_len = 0;

    status = pjsua_init(&cfg, &log_cfg, &m_cfg);
    if (status != PJ_SUCCESS){ pjsua_destroy();}
  }

  /* Add UDP transport. */
  {
    pjsua_transport_config cfg;
    pjsua_transport_config_default(&cfg);
    cfg.port = sipPort;
    status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, NULL);
    if (status != PJ_SUCCESS){ pjsua_destroy();}
  }


  status = pjsua_start();
  if (status != PJ_SUCCESS){ pjsua_destroy();}

  {
    pjsua_acc_config cfg;
    pjsua_acc_config_default(&cfg);
    cfg.id = pj_str((char*)"sip:tc@localhost");
    cfg.cred_count = 1;
    status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
    if (status != PJ_SUCCESS){ pjsua_destroy();}
  }

  CallObservers("INIT");
  return 0;
}


/*
   Stack, socket e thread destructor
*/
PJSIP_API int sipderegister(){
  REGISTER_THREAD();
  pjsua_acc_del(current_acc);
  pjsua_destroy();

  CallObservers("DESTROY");
  return 0;
}


/*
   Inizializzazione chiamata in uscita
*/
PJSIP_API int sipmakecall(char *sipToAddr){
  pj_status_t status;
  pj_str_t uri = pj_str(sipToAddr);
  REGISTER_THREAD();
  status = pjsua_call_make_call(current_acc, &uri, 0, NULL, NULL, NULL);
  if (status!=PJ_SUCCESS)
    CallObservers("INVALIDURI");
  return 0;
}


/*
   Chiusura chiamate attive
*/
PJSIP_API int siphangup(){
  REGISTER_THREAD();
	pjsua_call_hangup_all();
  return 0;
}


/* 
   Bridge verso l'API XPCOM
   richiama gli observer registrati mediante proxy alla callbacks
*/
static void CallObservers(const char* status)
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
    _pCallback->OnStatusChange(status);
  }

  return;
}



void SyncObservers(nsCOMPtr<nsIArray> o){
  //mObservers = do_CreateInstance(NS_ARRAY_CONTRACTID);
  mObservers = o;
}
