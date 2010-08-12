#include "pjsip.h"

#define ON_DURATION     80
#define OFF_DURATION      10
#define SAMPLES_PER_FRAME   64


/* Array di observer proxies */
static nsCOMPtr<nsIArray> mObservers;


/* Pjsua application data */
static struct config
{
    /* ID Chaiamata attiva */
    pjsua_call_id           cid;

    pjsua_config	    cfg;
    pjsua_logging_config    log_cfg;
    pjsua_media_config	    media_cfg;
    pjsua_transport_config  udp_cfg;
    pjsua_transport_config  rtp_cfg;
    pj_pool_t		    *pool;
    
    char		    *wav_file;
    int           	    wav_slot;
    pjmedia_port            *wav_port;
    int                     rtsetted;
    int                     ringing;

    int			    ringback_slot;
    pjmedia_port	    *ringback_port;
    
    int			    ring_slot;
    pjmedia_port	    *ring_port;
   
    int                     tone_slot;
    pjmedia_port            *tone_port;
    
    float		    mic_level;
    float         	    speaker_level;

} app_config;



/* Set default config. */
void configure() {

  printf("\n\nPJSUA v%s/%s\n\n", pj_get_version(), PJ_OS_NAME);

  app_config.cid = PJSUA_INVALID_ID;

  app_config.wav_slot = PJSUA_INVALID_ID;
  app_config.ringback_slot = PJSUA_INVALID_ID;
  app_config.ring_slot = PJSUA_INVALID_ID;
  app_config.tone_slot = PJSUA_INVALID_ID;

  app_config.wav_port = NULL;
  app_config.ringback_port = NULL;
  app_config.ring_port = NULL;
  app_config.tone_port = NULL;

  app_config.ringing = 0;
  app_config.rtsetted = 0;

  app_config.mic_level = 1.0;
  app_config.speaker_level = 1.0;


  /* GENERAL */
  pjsua_config_default(&app_config.cfg);
  app_config.cfg.use_srtp = PJMEDIA_SRTP_DISABLED;
  app_config.cfg.thread_cnt = 1;
  app_config.cfg.max_calls = 1;

  /* CALLBACKS */
  app_config.cfg.cb.on_incoming_call = &on_incoming_call;
  app_config.cfg.cb.on_call_media_state = &on_call_media_state;
  app_config.cfg.cb.on_call_state = &on_call_state;

  /* LOGGING */
  pjsua_logging_config_default(&app_config.log_cfg);
  app_config.log_cfg.level = 0;
  app_config.log_cfg.console_level = 0;

  /* MEDIA */
  pjsua_media_config_default(&app_config.media_cfg);
  app_config.media_cfg.clock_rate = 8000; 
  app_config.media_cfg.snd_clock_rate = 8000; 
  app_config.media_cfg.ice_no_rtcp = PJ_TRUE;
  app_config.media_cfg.enable_turn = PJ_FALSE;
  app_config.media_cfg.no_vad = PJ_TRUE;
  app_config.media_cfg.ec_tail_len = 0;
  app_config.media_cfg.quality = 5;
  app_config.media_cfg.snd_rec_latency = PJMEDIA_SND_DEFAULT_REC_LATENCY;
  app_config.media_cfg.snd_play_latency = PJMEDIA_SND_DEFAULT_PLAY_LATENCY;
  
  /* UDP TRANSPORT */
  pjsua_transport_config_default(&app_config.udp_cfg);
  app_config.udp_cfg.port = 5060;
  app_config.udp_cfg.qos_params.flags = PJ_QOS_PARAM_HAS_DSCP;
  app_config.udp_cfg.qos_params.dscp_val = 0x18;
  
  /* RTP TRANSPORT */
  pjsua_transport_config_default(&app_config.rtp_cfg);
  app_config.rtp_cfg.port = 4000;
  app_config.rtp_cfg.qos_type = PJ_QOS_TYPE_VOICE;

}


pj_status_t app_init()
{
    pjsua_transport_id transport_id = -1;
    pj_status_t status;

    /* Initialize default config */
    configure();
    
    /* Create pjsua */
    status = pjsua_create();
    if (status != PJ_SUCCESS) return status;

    /* Create pool for application */
    app_config.pool = pjsua_pool_create("pjsua-app", 2000, 2000);


    /* Initialize pjsua */
    status = pjsua_init(&app_config.cfg, &app_config.log_cfg, &app_config.media_cfg);
    if (status != PJ_SUCCESS) return status;

    /* WAV Player */ 

    /* Tone players */
    pjmedia_tonegen_create(app_config.pool, 8000, 1, SAMPLES_PER_FRAME, 16, 0, &app_config.tone_port);
    pjsua_conf_add_port(app_config.pool, app_config.tone_port, &app_config.tone_slot);
   
    /* Add UDP transport */
    pjsua_acc_id aid;
    status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &app_config.udp_cfg, &transport_id);
    if (status != PJ_SUCCESS) goto on_error;

    /* Add local account */
    pjsua_acc_add_local(transport_id, PJ_TRUE, NULL);


    /* Add RTP transports */
    status = pjsua_media_transports_create(&app_config.rtp_cfg);
    if (status != PJ_SUCCESS) goto on_error;

    return PJ_SUCCESS;

on_error:
    app_destroy();
    return status;
}



pj_status_t app_destroy()
{
    pj_status_t status;


    /* Close ringback port */
    if (app_config.ringback_port && 
	app_config.ringback_slot != PJSUA_INVALID_ID) 
    {
	pjsua_conf_remove_port(app_config.ringback_slot);
	app_config.ringback_slot = PJSUA_INVALID_ID;
	pjmedia_port_destroy(app_config.ringback_port);
	app_config.ringback_port = NULL;
    }

    /* Close ring port */
    if (app_config.ring_port && app_config.ring_slot != PJSUA_INVALID_ID) {
	pjsua_conf_remove_port(app_config.ring_slot);
	app_config.ring_slot = PJSUA_INVALID_ID;
	pjmedia_port_destroy(app_config.ring_port);
	app_config.ring_port = NULL;
    }

    /* Close tone generators */
    pjsua_conf_remove_port(app_config.tone_slot);


    if (app_config.pool) {
	pj_pool_release(app_config.pool);
	app_config.pool = NULL;
    }

    status = pjsua_destroy();

    pj_bzero(&app_config, sizeof(app_config));

    return status;
}


/* 
   Callback called by the library upon receiving incoming call 
*/
static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata){
  pjsua_call_info ci;
  PJ_UNUSED_ARG(acc_id);
  PJ_UNUSED_ARG(rdata);
  pjsua_call_get_info(call_id, &ci);
  CallObservers("INCOMING");

  if (app_config.cid != PJSUA_INVALID_ID)
    pjsua_call_answer(call_id, 486, NULL, NULL);
  else {
    pjsua_call_answer(call_id, 200, NULL, NULL);
    app_config.cid = call_id;
  }
}


/* 
   Callback called by the library when call's state has changed 
*/
static void on_call_state(pjsua_call_id call_id, pjsip_event *e){

  if ((app_config.cid!=PJSUA_INVALID_ID)&&(call_id != app_config.cid))
    return;

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
  if ((strcmp(ci.state_text.ptr, "DISCONNCTD")==0)&&(code!=486)){//Hangup and reason != busy here
    CallObservers("HANGUP");
    app_config.cid = PJSUA_INVALID_ID;
  }
}


/* 
   Callback called by the library when call's media state has changed 
*/
static void on_call_media_state(pjsua_call_id call_id){
  pjsua_call_info ci;
  pjsua_call_get_info(call_id, &ci);

  // When media is active, connect call to sound device.
  if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
    pjsua_conf_connect(ci.conf_slot, 0);
    pjsua_conf_connect(0, ci.conf_slot);
  }
}


/*
   STUN Server configuration
*/
void sipsetstunserver(char* stun){
  /* FUTURO STUN */
  //pjsua_config cfg;
  //cfg.stun_host = pj_str("stun.ekiga.net");
}


/*
   Account management
*/
int sipaddaccount(char* user, char* domain, char* password){
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
int sipregister(long sipPort) {
  app_init();
  CallObservers("INIT");
  return 0;
}


/*
   Stack, socket e thread destructor
*/
int sipderegister(){
  pjsua_destroy();
  CallObservers("DESTROY");
  return 0;
}


/*
   Inizializzazione chiamata in uscita
*/
int sipmakecall(char *sipToAddr){
  if (app_config.cid != PJSUA_INVALID_ID)
    return 1;
  pj_status_t status;
  pjsua_call_id _cid;
  pj_str_t uri = pj_str(sipToAddr);

  status = pjsua_call_make_call(current_acc, &uri, 0, NULL, NULL, &_cid);
  if (status!=PJ_SUCCESS)
    CallObservers("INVALIDURI");
  else
    app_config.cid = _cid;
  return 0;
}


/*
   Chiusura chiamate attive
*/
int siphangup(){
	pjsua_call_hangup_all();
  app_config.cid = PJSUA_INVALID_ID;
  CallObservers("HANGUP");
  return 0;
}


void playtone(char tone){

  pjsua_conf_connect(app_config.tone_slot, 0);

  pjmedia_tone_digit digits[2]; 
  digits[0].digit = tone; 
  digits[0].on_msec = ON_DURATION; 
  digits[0].off_msec = OFF_DURATION; 

  pjmedia_tonegen_rewind(app_config.tone_port);
  pjmedia_tonegen_play_digits(app_config.tone_port, 1, digits, 0); 

  pj_thread_sleep(ON_DURATION + OFF_DURATION + 10);
  pjsua_conf_disconnect(app_config.tone_slot, 0);
}


void setringtone(char* rt){

  if (app_config.rtsetted){
    pjmedia_port_destroy(app_config.wav_port);
    app_config.wav_slot = PJSUA_INVALID_ID;
  }

  app_config.wav_file = (char*)rt;
  pjmedia_wav_player_port_create(app_config.pool, (char*)app_config.wav_file, 20, 0, 0, &app_config.wav_port);
  pjsua_conf_add_port(app_config.pool, app_config.wav_port, &app_config.wav_slot);

  if (!app_config.rtsetted) {
    app_config.rtsetted = 1;
  } 
}


void playring(){ 
  if (app_config.ringing){
    stopring();
    app_config.ringing=0;
  }

  if ((app_config.rtsetted)&&(!app_config.ringing)){
    pjsua_conf_connect(app_config.wav_slot, 0);
    app_config.ringing = 1;
  }
}


void stopring(){
  if ((app_config.rtsetted)&&(app_config.ringing)){
    printf(!"\nDATA: -> %d\n",app_config.wav_slot);
    pjsua_conf_disconnect(app_config.wav_slot, 0);
    app_config.ringing = 0;
  }
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
