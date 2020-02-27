/**
*@file tnsysrepo.h
*@brief functions to handle sysrepo connection
*M.Ulbricht 2019
**/

#define verblog if(TN_sr_verbose) 


void TN_sysrepo_init (uint8_t ver);
