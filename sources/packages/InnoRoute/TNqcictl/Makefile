include	$(TOPDIR)/rules.mk
PKG_NAME :=tnqcictl
PKG_RELEASE := 0.1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DEPENDS:= TN_tools
include	$(INCLUDE_DIR)/package.mk


define Package/tnqcictl
	SECTION:=InnoRoute
	CATEGORY:=InnoRoute
	TITLE:=Trustnode QCI config tool
	DEPENDS:=+libc +TN_tools
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) $(STAGING_DIR)/usr/include/tn_env.h $(PKG_BUILD_DIR)/
	$(CP) ./src/*.h $(STAGING_DIR)/usr/include/
	
endef

define Package/tnqcictl/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/share/InnoRoute/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tnqcictl $(1)/usr/sbin/TNqcictl
#	$(INSTALL_BIN) files/*.sh $(1)/usr/share/InnoRoute/
endef





define Build/Compile
	rm -f $(PKG_BUILD_DIR)/*.o
	$(MAKE) -C $(PKG_BUILD_DIR) tnqcictl
endef

$(eval $(call BuildPackage,tnqcictl,+argp-standalone,libc,TN_tools))

