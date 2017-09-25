include	$(TOPDIR)/rules.mk
PKG_NAME :=TN_start
PKG_RELEASE := 1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk

define Package/TN_start
 SECTION:=TN_start
 CATEGORY:=InnoRoute
 TITLE:=TN_start
 DEPENDS:=+lm-sensors
 endef
define Build/Prepare
#mkdir ‐p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef
define Package/TN_start/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/TN_start $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/usr/share/InnoRoute
	$(INSTALL_CONF) ./files/INRsensor.conf $(1)/usr/share/InnoRoute/
endef
define Package/TN_start/postinst
#!/bin/sh
# check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
	echo "Enabling rc.d symlink for TN_start"
	/etc/init.d/TN_start enable
fi
exit 0
endef

define Package/TN_start/prerm
#!/bin/sh
# check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
	echo "Removing rc.d symlink for TN_start"
	/etc/init.d/TN_start disable
fi
exit 0
endef
$(eval $(call BuildPackage,TN_start)) 


	
  
	
  


  

  
	
  

 	


