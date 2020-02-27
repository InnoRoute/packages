#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=LPCtool
PKG_VERSION:=0.2
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/LPCtool
  SUBMENU:=InnoRoute
  TITLE:=LPCtool
  FILES:=$(PKG_BUILD_DIR)/LPCtool.ko
  AUTOLOAD:=$(call AutoLoad,50,LPCtool)
#  DEPENDS:=+kmod-openvswitch
#+openvswitch-dev +kmod-TNopenvswitch
endef

define KernelPackage/LPCtool/description
tool to acces the lpc bus
endef

include $(INCLUDE_DIR)/kernel-defaults.mk

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp src/Makefile src/*.c  $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) $(KERNEL_MAKEOPTS) SUBDIRS="$(PKG_BUILD_DIR)" modules
endef

#define KernelPackage/lpctool/conffiles
#/etc/config/trelay
#endef

#define KernelPackage/trelay/install
#	$(INSTALL_DIR) $(1)/etc/trustnode
#endef

$(eval $(call KernelPackage,LPCtool))
