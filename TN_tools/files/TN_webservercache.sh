#!/bin/bash
#disable webserver cache for development
uci set luci.ccache.enable=0; uci commit luci
echo "Webserver cache disabled"
