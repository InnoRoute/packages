include	$(TOPDIR)/rules.mk
PKG_NAME :=TNbar1
PKG_RELEASE := 1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk

define Package/TNbar1
 SECTION:=InnoRoute
 CATEGORY:=InnoRoute
 TITLE:=TNbar1
 DEPENDS:=+pcimem
 endef
define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef
define Package/TNbar1/install

	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_CONF) ./files/TNbar1 $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,TNbar1)) 


	
  
	
  


  

  
	
  

 	


