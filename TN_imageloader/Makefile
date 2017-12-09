include	$(TOPDIR)/rules.mk
PKG_NAME :=TN_imageloader
PKG_RELEASE := 1.1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk

define Package/TN_imageloader
 SECTION:=TN_imageloader
 CATEGORY:=InnoRoute
 TITLE:=TN_imageloader
 DEPENDS:=+ftdi_opto_tool
 endef
define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef
define Package/TN_imageloader/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/TN_imageloader $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/TN_iloadhelp $(1)/etc/init.d/
	chmod +x $(1)/etc/init.d/TN_imageloader
	chmod +x $(1)/etc/init.d/TN_iloadhelp
	$(INSTALL_DIR) $(1)/usr/share/InnoRoute
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/TNchangemod $(1)/usr/sbin/
	$(INSTALL_CONF) ./files/TNmod.conf $(1)/usr/share/InnoRoute/
endef
define Package/TN_imageloader/postinst
#!/bin/sh
# check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
	echo "Enabling rc.d symlink for TN_imageloader"
	/etc/init.d/TN_imageloader enable
	/etc/init.d/TN_iloadhelp enable
fi
exit 0
endef

define Package/TN_imageloader/prerm
#!/bin/sh
# check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
	echo "Removing rc.d symlink for TN_imageloader"
	/etc/init.d/TN_imageloader disable
	/etc/init.d/TN_iloadhelp disable
fi
exit 0
endef
$(eval $(call BuildPackage,TN_imageloader)) 


	
  
	
  


  

  
	
  

 	


