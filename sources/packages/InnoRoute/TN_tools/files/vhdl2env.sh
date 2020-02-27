#!/bin/bash

#converts VHDL address definitions to tn_env.sh

if [[ $# == 1 ]]; then
  cat tn_env_head > ./tn_env.sh

  echo "#automatic generated from $1" >> ./tn_env.sh

  python ./vhdl2env.py $1 >> ./tn_env.sh

  hg commit -m "tn_env.sh gen from vhdl"
else
  echo "provide vhdl-mmi input file!"
fi
