include	$(TOPDIR)/rules.mk
PKG_NAME :=tnstatistics
PKG_RELEASE := 0.2
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DEPENDS:= TN_tools
include	$(INCLUDE_DIR)/package.mk


define Package/tnstatistics
	SECTION:=InnoRoute
	CATEGORY:=InnoRoute
	TITLE:=Trustnode statistics tool
	DEPENDS:=+libc +TN_tools
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) $(STAGING_DIR)/usr/include/tn_env.h $(PKG_BUILD_DIR)/
	$(CP) ./src/*.h $(STAGING_DIR)/usr/include/
	
endef

define Package/tnstatistics/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/share/InnoRoute/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tnstatistics $(1)/usr/sbin/TNstatistics
#	$(INSTALL_BIN) files/*.sh $(1)/usr/share/InnoRoute/
endef





define Build/Compile
	rm -f $(PKG_BUILD_DIR)/*.o
	$(MAKE) -C $(PKG_BUILD_DIR) tnstatistics
endef

$(eval $(call BuildPackage,tnstatistics,+argp-standalone,libc,TN_tools))

