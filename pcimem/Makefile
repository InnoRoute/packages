include	$(TOPDIR)/rules.mk
PKG_NAME :=pcimem
PKG_RELEASE := 1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk

define Package/pcimem
 SECTION:=utils
 CATEGORY:=Utilities
 TITLE:=pcimem
endef
define Build/Prepare

	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef
define Package/pcimem/install

	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pcimem $(1)/usr/sbin/
endef



$(eval $(call BuildPackage,pcimem)) 


	
  
	
  


  

  
	
  

 	


