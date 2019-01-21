/**
*@file INR-NAPI.h
*@brief settings and definitions NAPI
*@author  M.Ulbricht 2015
**/

void INR_NAPI_init (uint8_t index);
void INR_NAPI_remove (uint8_t index);
void INR_NAPI_schedule (uint8_t index);
void INR_NAPI_complete (uint8_t index);
int INR_NAPI_poll_0 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_1 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_2 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_3 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_4 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_5 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_6 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_7 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_8 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_9 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_10 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_11 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_12 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_13 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_14 (struct napi_struct *napi, int budget);
int INR_NAPI_poll_15 (struct napi_struct *napi, int budget);

#define NAPI_enable	1
#define INR_NAPI_maxpoll	128
