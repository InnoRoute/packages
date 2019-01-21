/**
*@file accdpactions.h
*@brief functions to handle acceleration datapath actions
*M.Ulbricht 2016
**/
#include "tn_env.h"
void ACCDP_add (struct arguments *arguments);
void ACCDP_update (struct arguments arguments);
void ACCDP_del (struct arguments arguments);
void ACCDP_clear (void);
void ACCDP_print (struct arguments arguments);


#define INR_ACCDP_length 8
