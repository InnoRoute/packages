#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=INR_TN_MMI
PKG_VERSION:=0.2
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/INR_TN_MMI
  SUBMENU:=InnoRoute
  TITLE:=TrustNode mmi control functions
  FILES:=$(PKG_BUILD_DIR)/INR_TN_MMI.ko
  AUTOLOAD:=$(call AutoLoad,40,INR_TN_MMI)
  DEPENDS:=+TN_tools
#+kmod-openvswitch
#+openvswitch-dev +kmod-TNopenvswitch
endef

define KernelPackage/INR_TN_MMI/description
special mmi control functions
endef

include $(INCLUDE_DIR)/kernel-defaults.mk

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) src/Makefile src/*.c src/*.h $(PKG_BUILD_DIR)/
	$(CP) src/TN_MMI.h $(STAGING_DIR)/usr/include/
	$(CP) $(STAGING_DIR)/usr/include/tn_env.h $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) $(KERNEL_MAKEOPTS) SUBDIRS="$(PKG_BUILD_DIR)" modules
endef

#define KernelPackage/trustnode/conffiles
#/etc/config/trelay
#endef

#define KernelPackage/trelay/install
#	$(INSTALL_DIR) $(1)/etc/trustnode
#endef

$(eval $(call KernelPackage,INR_TN_MMI,+libsflow,+libofproto,+libovsdb,TN_tools))
