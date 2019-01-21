#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=trustnode
PKG_VERSION:=0.8
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/trustnode
  SUBMENU:=InnoRoute
  TITLE:=TrustNode network iterface driver
  FILES:=$(PKG_BUILD_DIR)/trustnode.ko
  AUTOLOAD:=$(call AutoLoad,50,trustnode)
  DEPENDS:=+TN_tools +kmod-ptp +kmod-INR_TN_MMI
#+kmod-openvswitch
#+openvswitch-dev +kmod-TNopenvswitch
endef

define KernelPackage/trustnode/description
trelay relays ethernet packets between two devices (similar to a bridge), but
without any MAC address checks. This makes it possible to bridge client mode
or ad-hoc mode wifi devices to ethernet VLANs, assuming the remote end uses
the same source MAC address as the device that packets are supposed to exit
from.
endef

include $(INCLUDE_DIR)/kernel-defaults.mk

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) src/Makefile src/*.c src/*.h $(PKG_BUILD_DIR)/
	$(CP) $(STAGING_DIR)/usr/include/tn_env.h $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) $(KERNEL_MAKEOPTS) SUBDIRS="$(PKG_BUILD_DIR)" modules
endef

define KernelPackage/trustnode/conffiles
/etc/config/trelay
endef

define KernelPackage/trelay/install
	$(INSTALL_DIR) $(1)/etc/trustnode
endef

$(eval $(call KernelPackage,trustnode,+kmod-INR_TN_MMI,+libopenvswitch,+libsflow,+libofproto,+libovsdb,TN_tools,kmod-ptp))
