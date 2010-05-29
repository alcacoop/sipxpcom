#include "pjsip.h"


/*
   Array di observer proxies
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
  //PJ_LOG(3,(THIS_FILE, "Incoming call from %.*s!!",(int)ci.remote_info.slen, ci.remote_info.ptr));
  pjsua_call_answer(call_id, 200, NULL, NULL);
}


/* 
   Callback called by the library when call's state has changed 
*/
static void on_call_state(pjsua_call_id call_id, pjsip_event *e){
  pjsua_call_info ci;
  PJ_UNUSED_ARG(e);
  pjsua_call_get_info(call_id, &ci);
  int code = 0;
  pj_str_t reason;
  pjsip_msg *msg;

  if (e->body.tsx_state.type == PJSIP_EVENT_RX_MSG) {
    msg = e->body.tsx_state.src.rdata->msg_info.msg;
    code = msg->line.status.code;
    reason = msg->line.status.reason;
    //printf("CODE: %d - REASON: %s\n", code, reason);
    if (code==180){
      CallObservers("RINGING");
      printf("STATO: RINGING - %d\n", code);
    } else if (code==486){
      CallObservers("BUSY");
      printf("STATO: BUSY - %d\n", code);
    } if (code==603){
      CallObservers("DECLINE");
      printf("STATO: DECLINE - %d\n", code);
    } 
  }  
  
  if (ci.state != 3 && ci.state != 4) //EARLY AND CONNECTING
    printf("STATO: %s - %d\n",ci.state_text.ptr, ci.state);

  if (strcmp(ci.state_text.ptr, "CALLING")==0)
    CallObservers("CALLING");
  if (strcmp(ci.state_text.ptr, "CONFIRMED")==0)
    CallObservers("ANSWER");
  if ((strcmp(ci.state_text.ptr, "DISCONNCTD")==0)&&(code!=486))//Hangup and reason != busy here
    CallObservers("HANGUP");
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
   STUN Server configuration
*/
PJSIP_API void sipsetstunserver(char* stun){
  /* FUTURO STUN */
  //pjsua_config cfg;
  //cfg.stun_host = pj_str("stun.ekiga.net");
}


/*
   Account management
*/
PJSIP_API int sipaddaccount(char* user, char* domain, char* password){
  /* FUTURA REGISTRAZIONE 
  #define SIP_DOMAIN	"ekiga.net"
  #define SIP_USER	"dom0"
  #define SIP_PASSWD	""
  pj_status_t status;
  pjsua_acc_config cfg;
  pjsua_acc_id acc_id;

  pjsua_acc_config_default(&cfg);
  cfg.cred_count = 1;

  cfg.id = pj_str("sip:" SIP_USER "@" SIP_DOMAIN);
  cfg.reg_uri = pj_str("sip:" SIP_DOMAIN);
  cfg.cred_count = 1;
  cfg.cred_info[0].realm = pj_str(SIP_DOMAIN);
  cfg.cred_info[0].username = pj_str(SIP_USER);
  cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
  cfg.cred_info[0].data = pj_str(SIP_PASSWD);

  status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
  */
}


/*
   Stack, socket e thread constructor
*/
PJSIP_API int sipregister(long sipPort) {

  pj_status_t status;
  pjsua_acc_id acc_id;

  status = pjsua_create();

  /* Init pjsip stack */
  if (status != PJ_SUCCESS){ pjsua_destroy();}
  {
    pjsua_config cfg;
    pjsua_logging_config log_cfg;
    pjsua_media_config m_cfg;

    pjsua_config_default(&cfg);
    cfg.thread_cnt = 1;
    cfg.use_srtp = PJMEDIA_SRTP_DISABLED;
    cfg.cb.on_incoming_call = &on_incoming_call;
    cfg.cb.on_call_media_state = &on_call_media_state;
    cfg.cb.on_call_state = &on_call_state;
    pjsua_logging_config_default(&log_cfg);
    log_cfg.console_level = 0;

    //MEDIA
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

  /* Add RTP transport. */
  {
    pjsua_transport_config cfg;
    pjsua_transport_config_default(&cfg);
    cfg.port = 7072;
    cfg.qos_type = PJ_QOS_TYPE_VOICE;
    status = pjsua_media_transports_create(&cfg);
  }

  status = pjsua_start();
  if (status != PJ_SUCCESS){ pjsua_destroy();}

  /* Add local account */
  {
    pjsua_acc_config cfg;
    pjsua_acc_config_default(&cfg);
    cfg.cred_count = 1;
    cfg.id = pj_str("sip:localhost");
    status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
  }

  CallObservers("INIT");
  return 0;
}


/*
   Stack, socket e thread destructor
*/
PJSIP_API int sipderegister(){
  pjsua_acc_id acc_id = pjsua_acc_get_default();
  if (acc_id != PJSUA_INVALID_ID)
    pjsua_acc_del(acc_id);

  //siphangup();
  //pjsua_handle_events(500);
  //pjsua_destroy();
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
  status = pjsua_call_make_call(current_acc, &uri, 0, NULL, NULL, NULL);
  if (status!=PJ_SUCCESS)
    CallObservers("INVALIDURI");
  return 0;
}


/*
   Chiusura chiamate attive
*/
PJSIP_API int siphangup(){
	pjsua_call_hangup_all();
  CallObservers("HANGUP");
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
  mObservers = o;
}
