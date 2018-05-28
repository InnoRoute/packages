include	$(TOPDIR)/rules.mk
PKG_NAME :=tsntest
PKG_RELEASE := 0.1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk


define Package/tsntest
	SECTION:=InnoRoute
	CATEGORY:=InnoRoute
	TITLE:=TSN test tool
	DEPENDS:=+libc 
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	
endef

define Package/tsntest/install
	$(INSTALL_DIR) $(1)/usr/sbin

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hwtstamp_config $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/timestamping $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/txtimestamp $(1)/usr/sbin/
#	$(INSTALL_BIN) files/*.sh $(1)/usr/share/InnoRoute/
endef





define Build/Compile
	rm -f $(PKG_BUILD_DIR)/*.o
	$(MAKE) -C $(PKG_BUILD_DIR) hwtstamp_config
	$(MAKE) -C $(PKG_BUILD_DIR) timestamping
	$(MAKE) -C $(PKG_BUILD_DIR) txtimestamp
endef

$(eval $(call BuildPackage,tsntest,+argp-standalone,libc))

