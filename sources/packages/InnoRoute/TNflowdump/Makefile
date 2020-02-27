include	$(TOPDIR)/rules.mk
PKG_NAME :=TN_flowdump
PKG_RELEASE := 1.2
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk

define Package/TN_flowdump
 SECTION:=TN_flowdump
 CATEGORY:=InnoRoute
 TITLE:=TN_flowdump
 DEPENDS:=+python
 endef
define Build/Prepare
#mkdir ‐p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef
define Package/TN_flowdump/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/TN_flowdump $(1)/etc/init.d/
	chmod +x $(1)/etc/init.d/TN_flowdump
	$(INSTALL_DIR) $(1)/usr/share/InnoRoute/scripts
	$(INSTALL_BIN) ./files/*.py $(1)/usr/share/InnoRoute/scripts/
	$(INSTALL_CONF) ./files/TNflowdump.conf $(1)/usr/share/InnoRoute/
endef
define Package/TN_flowdump/postinst
#!/bin/sh
# check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
	echo "Enabling rc.d symlink for TN_flowdump"
#	/etc/init.d/TN_flowdump enable
fi
exit 0
endef

define Package/TN_flowdump/prerm
#!/bin/sh
# check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
	echo "Removing rc.d symlink for TN_flowdump"
#	/etc/init.d/TN_flowdump disable
fi
exit 0
endef
$(eval $(call BuildPackage,TN_flowdump)) 


	
  
	
  


  

  
	
  

 	


