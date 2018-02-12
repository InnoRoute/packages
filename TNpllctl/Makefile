include	$(TOPDIR)/rules.mk
PKG_NAME :=TNpllctl
PKG_RELEASE := 1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
 SECTION:=InnoRoute
 CATEGORY:=InnoRoute
 TITLE:=TNpllctl
 DEPENDS:=+i2c-tools TN_tools
 endef
TARGET_CPPFLAGS += -I$(PKG_BUILD_DIR)/include
define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/share
	$(INSTALL_DIR) $(1)/usr/share/InnoRoute/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pllctl $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/pllload.sh $(1)/usr/sbin/INRpllload
#	$(INSTALL_CONF) ./files/*.stp $(1)/usr/share/InnoRoute/
endef

$(eval $(call BuildPackage,$(PKG_NAME))) 


	
  
	
  


  

  
	
  

 	


