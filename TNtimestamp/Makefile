include	$(TOPDIR)/rules.mk
PKG_NAME :=tntimestamp
PKG_RELEASE := 0.1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk


define Package/tntimestamp
	SECTION:=InnoRoute
	CATEGORY:=InnoRoute
	TITLE:=Hardware timestamp test tool
	DEPENDS:=@!USE_UCLIBC +librt +libc
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	
endef

define Package/tntimestamp/install
	$(INSTALL_DIR) $(1)/usr/sbin

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tntimestamp $(1)/usr/sbin/TNtimestamp

endef





#define Build/Compile
#	rm -f $(PKG_BUILD_DIR)/*.o
#	$(MAKE) -C $(PKG_BUILD_DIR) tntimestamp

#endef

$(eval $(call BuildPackage,tntimestamp,+argp-standalone,libc))

