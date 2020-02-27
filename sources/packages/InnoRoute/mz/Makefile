# 
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
#
include $(TOPDIR)/rules.mk

PKG_NAME:=mz
PKG_VERSION:=0.40
PKG_RELEASE:=1
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://www.perihel.at/sec/mz/

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/mz
	SECTION:=net
	CATEGORY:=Network
	TITLE:=Mausezahn is a free fast traffic generator.
	DEPENDS:=+libpcap +libnet-1.2.x +libcli +libpthread +librt
endef

define Build/Prepare
	tar xvzf $(DL_DIR)/$(PKG_SOURCE) -C $(BUILD_DIR)/
	$(Build/Patch)
	$(if $(QUILT),touch $(PKG_BUILD_DIR)/.quilt_used)
	rm -f $(PKG_BUILD_DIR)/CMakeCache.txt
	rm -fR $(PKG_BUILD_DIR)/CMakeFiles
	rm -fR $(PKG_BUILD_DIR)/src/CMakeFiles
	rm -f $(PKG_BUILD_DIR)/Makefile
	rm -f $(PKG_BUILD_DIR)/src/Makefile
	rm -f $(PKG_BUILD_DIR)/cmake_install.cmake
	rm -f $(PKG_BUILD_DIR)/progress.make
endef

define Build/Configure
	IN_OPENWRT=1 \
	AR="$(TARGET_CROSS)ar" \
	AS="$(TARGET_CC) -c $(TARGET_CFLAGS)" \
	LD="$(TARGET_CROSS)ld" \
	NM="$(TARGET_CROSS)nm" \
	CC="$(TARGET_CC)" \
	GCC="$(TARGET_CC)" \
	CXX="$(TARGET_CROSS)g++" \
	RANLIB="$(TARGET_CROSS)ranlib" \
	STRIP="$(TARGET_CROSS)strip" \
	OBJCOPY="$(TARGET_CROSS)objcopy" \
	OBJDUMP="$(TARGET_CROSS)objdump" \
	TARGET_CPPFLAGS="$(TARGET_CPPFLAGS)" \
	TARGET_CFLAGS="$(TARGET_CFLAGS)" \
	TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
	LIBS="$(STAGING_DIR)/usr/lib/)" \
	cmake	-DCMAKE_PREFIX_PATH:PATH=$(STAGING_DIR)/usr \
		-DCMAKE_INSTALL_PREFIX:PATH=/usr \
		-DCMAKE_CXX_FLAGS=-g \
	VERBOSE=1 \
	$(PKG_BUILD_DIR)/CMakeLists.txt
endef

TARGET_CFLAGS :=-I$(STAGING_DIR)/usr/lib/libnet-1.2.x/include
TARGET_LDFLAGS := -L$(STAGING_DIR)/usr/lib/libnet-1.2.x/lib $(TARGET_LDFLAGS) 

define Package/mz/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/mz $(1)/usr/bin/
endef

$(eval $(call BuildPackage,mz,libnet,libpthread,librt,libcli,libpcap))
