#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#changed to support TrustNode ulbricht@innoroute.de 2017
include $(TOPDIR)/rules.mk

ARCH:=i386
BOARD:=TrustNode
BOARDNAME:=TrustNode
FEATURES:=ext4 uefi
SUBTARGETS=TrustNode
MAINTAINER:=InnoRoute GmbH ulbricht@innoroute.de

DEFAULT_PACKAGES += partx-utils

KERNEL_PATCHVER:=4.14

KERNELNAME:=bzImage

include $(INCLUDE_DIR)/target.mk

$(eval $(call BuildTarget))


