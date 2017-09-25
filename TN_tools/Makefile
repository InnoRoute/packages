include	$(TOPDIR)/rules.mk
PKG_NAME :=TN_tools
PKG_RELEASE := 1
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
endef
define Package/TN_tools/install
	$(INSTALL_DIR) $(1)/usr/share/InnoRoute
	$(INSTALL_BIN) ./files/*.sh $(1)/usr/share/InnoRoute/
	$(INSTALL_CONF) ./files/*.conf $(1)/usr/share/InnoRoute/
endef
$(eval $(call BuildPackage,TN_tools)) 


	
  
	
  


  

  
	
  

 	


