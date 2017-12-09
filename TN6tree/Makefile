include	$(TOPDIR)/rules.mk
PKG_NAME :=TN6tree
PKG_RELEASE := 1.1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk

define Package/TN6tree
 SECTION:=InnoRoute
 CATEGORY:=InnoRoute
 TITLE:=TN6tree
 DEPENDS:=+TN_tools
 endef

define Build/Prepare

	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) $(STAGING_DIR)/usr/include/tn_env.h $(PKG_BUILD_DIR)/
endef
define Package/TN6tree/install

	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/TN6tree $(1)/usr/sbin/
endef



$(eval $(call BuildPackage,TN6tree,TN_tools)) 


	
  
	
  


  

  
	
  

 	


