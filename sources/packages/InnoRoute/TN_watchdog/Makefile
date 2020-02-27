include	$(TOPDIR)/rules.mk
PKG_NAME :=TN_watchdog
PKG_RELEASE := 0.1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk

define Package/TN_watchdog
 SECTION:=TN_watchdog
 CATEGORY:=InnoRoute
 TITLE:=TN_watchdog
# DEPENDS:=+INR_TN_MMI
 endef
define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef
define Package/TN_watchdog/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/TN_watchdog $(1)/etc/init.d/
	chmod +x $(1)/etc/init.d/TN_watchdog
	$(INSTALL_DIR) $(1)/usr/share/InnoRoute
	$(INSTALL_BIN) ./files/TN_watchdogd.sh $(1)/usr/share/InnoRoute/

endef


define Package/TN_watchdog/prerm
#!/bin/sh
# check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
	echo "Removing rc.d symlink for TN_watchdog"
	/etc/init.d/TN_watchdog disable
fi
exit 0
endef
$(eval $(call BuildPackage,TN_watchdog)) 


	
  
	
  


  

  
	
  

 	


