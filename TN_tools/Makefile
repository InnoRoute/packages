include	$(TOPDIR)/rules.mk
PKG_NAME :=TN_tools
PKG_RELEASE := 1.5
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk

define Package/TN_tools
 SECTION:=TN_tools
 CATEGORY:=InnoRoute
 TITLE:=TN_tools
 DEPENDS:=TNbar1
 endef
define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	rm -f $(PKG_BUILD_DIR)/../TN6tree/.prepared*
endef
define Package/TN_tools/install
	$(INSTALL_DIR) $(1)/usr/share/InnoRoute
	$(INSTALL_BIN) ./files/tn*.sh $(1)/usr/share/InnoRoute/
	$(INSTALL_BIN) ./files/TN*.sh $(1)/usr/share/InnoRoute/
	$(INSTALL_CONF) ./files/*.conf $(1)/usr/share/InnoRoute/
	$(INSTALL_CONF) ./files/*.stp $(1)/usr/share/InnoRoute/
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include
	$(INSTALL_CONF) ./files/*.h $(STAGING_DIR)/usr/include/
endef
define Package/TN_tools/compile
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include
	$(INSTALL_CONF) ./files/*.h $(STAGING_DIR)/usr/include/
endef
$(eval $(call BuildPackage,TN_tools)) 


	
  
	
  


  

  
	
  

 	


