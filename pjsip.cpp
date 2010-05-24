#include <pjsua-lib/pjsua.h>
#include "stdio.h"
#include "string.h"
#include "pjsip.h"
#include "nsSIP.h"

#define THIS_FILE	"APP"
#define current_acc	pjsua_acc_get_default()

#define REGISTER_THREAD()	\
  static pj_thread_desc desc;\
  static pj_thread_t *  thread;\
  if(!pj_thread_is_registered()) {\
    pj_thread_register(NULL,desc,&thread);\
  }

void state_handler(char*);

static nsCOMPtr<class nsSipStateObserver> observer;

/* Callback called by the library upon receiving incoming call */
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

/* Callback called by the library when call's state has changed */
static void on_call_state(pjsua_call_id call_id, pjsip_event *e){
  pjsua_call_info ci;
  PJ_UNUSED_ARG(e);
  pjsua_call_get_info(call_id, &ci);
  //printf("%s\n",ci.state_text.ptr);
  observer->OnStatusChange("PIPPO");
  if (strcmp(ci.state_text.ptr, "CALLING")==0)
    observer->OnStatusChange("CALLING");
  if (strcmp(ci.state_text.ptr, "CONFIRMED")==0)
    observer->OnStatusChange("ANSWER");
  if (strcmp(ci.state_text.ptr, "DISCONNCTD")==0){
    observer->OnStatusChange("HANGUP");
    siphangup();
  }

}

/* Callback called by the library when call's media state has changed */
static void on_call_media_state(pjsua_call_id call_id){
  pjsua_call_info ci;
  pjsua_call_get_info(call_id, &ci);
  if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
    // When media is active, connect call to sound device.
    pjsua_conf_connect(ci.conf_slot, 0);
    pjsua_conf_connect(0, ci.conf_slot);
  }
}

/* Display error and exit application */
static void error_exit(const char *title, pj_status_t status) {
  pjsua_perror(THIS_FILE, title, status);
  pjsua_destroy();
  exit(1);
}



PJSIP_API int sipregister(long sipPort, nsCOMPtr<class nsSipStateObserver> o) {

  pj_status_t status;
  pjsua_acc_id acc_id;
  REGISTER_THREAD();

  observer = o;
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

  return 0;
}


PJSIP_API int sipderegister(){
  REGISTER_THREAD();
  pjsua_acc_del(current_acc);
  pjsua_destroy();
  return 0;
}


PJSIP_API int sipmakecall(char *sipToAddr){
  pj_status_t status;
  pj_str_t uri = pj_str(sipToAddr);
  REGISTER_THREAD();
  status = pjsua_call_make_call(current_acc, &uri, 0, NULL, NULL, NULL);
  return 0;
}


PJSIP_API int siphangup(){
  REGISTER_THREAD();
	pjsua_call_hangup_all();
  return 0;
}
