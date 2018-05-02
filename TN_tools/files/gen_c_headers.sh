#!/bin/bash
cat files/tn_env.sh | grep export | cut -d# -f1 | tr '=' ' ' | sed -e "s/export/#define /" >files/tn_env.h
echo '#define	 printconst(constante) printf(#constante ":0x%lx\n",constante)'>>files/tn_env.h
