#!/bin/bash
opkg update
opkg upgrade `opkg list-upgradable|cut -f1 -d-`
