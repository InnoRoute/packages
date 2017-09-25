include	$(TOPDIR)/rules.mk
PKG_NAME :=ftdi_opto_tool
PKG_RELEASE := 1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk

define Package/ftdi_opto_tool
 SECTION:=InnoRoute
 CATEGORY:=InnoRoute
 TITLE:=ftdi_opto_tool
 DEPENDS:=+libftdi1 +i2c-tools +libc
 endef
define Build/Prepare

	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef
define Package/ftdi_opto_tool/install

	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ftdi_opto_tool $(1)/usr/sbin/INR_ftdi_opto_tool
	$(INSTALL_BIN) ./files/TN_opto_prog $(1)/usr/sbin/
endef



$(eval $(call BuildPackage,ftdi_opto_tool,libftdi1,libc)) 


	
  
	
  


  

  
	
  

 	


