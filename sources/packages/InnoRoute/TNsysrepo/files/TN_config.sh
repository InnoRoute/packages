#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# -ne 1 ]]; then
  echo -e "TrustNode sysrepo persistent config tool, please specify action:"
  echo -e "\t [save]: saves actual config to be loaded after startup and FPGA reporogramming"
  echo -e "\t [load]: load stored configuration"
  echo -e "\t [clear]: clear stored configuration"
  echo -e "\t [print]: print saved config"
  echo -e "\t [running]: print actual running config"
  echo -e "\t [import]: import an external XML config, filename expected"
else

case $1 in
  save) sysrepocfg --export=/usr/share/InnoRoute/TNsysrepostorage.xml --format=xml --datastore=running TNsysrepo && cp /usr/share/InnoRoute/TNsysrepostorage.xml /www/luci-static/innoroute/TNsysrepostorage.xml   &&TN_text2display.sh "Conf saved" > /dev/null && echo "done";;
  clear) rm /usr/share/InnoRoute/TNsysrepostorage.xml && touch /usr/share/InnoRoute/TNsysrepostorage.xml && echo "done" ;;
  print) cat /usr/share/InnoRoute/TNsysrepostorage.xml ;;
  load) sysrepocfg --import=/usr/share/InnoRoute/TNsysrepostorage.xml --format=xml TNsysrepo&& TN_text2display.sh "Conf loaded">/dev/null && echo "done";;
  running) sysrepocfg --export=/tmp/TNsysrepobackup.xml --format=xml --datastore=running TNsysrepo && cat /tmp/TNsysrepobackup.xml ;;
  import) sysrepocfg --import=$2 --format=xml TNsysrepo&& cp $2 /usr/share/InnoRoute/TNsysrepostorage.xml &&cp /usr/share/InnoRoute/TNsysrepostorage.xml /www/luci-static/innoroute/TNsysrepostorage.xml   && TN_text2display.sh "Conf import done">/dev/null && echo "done";;
  *) echo "Unknown argument" ;;
esac

fi

