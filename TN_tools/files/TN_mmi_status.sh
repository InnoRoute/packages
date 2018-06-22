#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to read out the current, self-clearing MMI statistics"
  echo "Parameter <ex> can have any value. Without it, this help is shown"
  echo "If nothing is shown, then the previous access cleared the statistics"
else
  echo "Checking for MMI access errors ..."
  tn_ll_mmi_status
  echo "Done"
fi
