# 
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
#
include $(TOPDIR)/rules.mk
PKG_NAME:=ft-jtag
PKG_VERSION:=0.2
PKG_RELEASE:=1
#PKG_NAME:=ftx-prog
#PKG_SOURCE_PROTO:=git
#PKG_SOURCE_URL:=https://github.com/richardeoin/ftx-prog.git
#PKG_SOURCE:=$(PKG_NAME).tar.gz
#PKG_SOURCE_VERSION:=HEAD
#PKG_SOURCE_SUBDIR:=$(PKG_NAME)
#PKG_VERSION:=01.08.2016
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
#PKG_NAME:=ft232r_prog
#PKG_VERSION:=1.25
#PKG_RELEASE:=1
#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
#PKG_SOURCE_URL:=http://rtr.ca/ft232r/

#PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp src/Makefile src/*.cxx  $(PKG_BUILD_DIR)/
	cp src/Makefile src/*.h  $(PKG_BUILD_DIR)/
endef

define Package/ft-jtag
	SECTION:=utils
  	CATEGORY:=Utilities
	TITLE:=FTDI jtag util
	DEPENDS:=+libftdi +libusb
endef


define Package/ft-jtag/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ft-jtag $(1)/usr/bin/
endef

$(eval $(call BuildPackage,ft-jtag))
