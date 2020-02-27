include	$(TOPDIR)/rules.mk
PKG_NAME :=INR_ftdi_eeprom
PKG_RELEASE := 1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk

define Package/INR_ftdi_eeprom
 SECTION:=InnoRoute
 CATEGORY:=InnoRoute
 TITLE:=INR_ftdi_eeprom
 DEPENDS:=+libftdi1 
 endef
define Build/Prepare

	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef
define Package/INR_ftdi_eeprom/install

	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/share/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/eeprom $(1)/usr/sbin/INR_ftdi_eeprom
endef



$(eval $(call BuildPackage,INR_ftdi_eeprom,libftdi1)) 


	
  
	
  


  

  
	
  

 	


