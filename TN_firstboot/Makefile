include	$(TOPDIR)/rules.mk
PKG_NAME :=TN_firstboot
PKG_RELEASE := 1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk

define Package/TN_firstboot
 SECTION:=TN_firstboot
 CATEGORY:=InnoRoute
 TITLE:=TN_firstboot
 DEPENDS:=+ftdi_opto_tool +TNpllctl +lm-sensors +openssh-keygen +TN_start
endef
 
define Build/Prepare
#mkdir ‐p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef
define Package/TN_firstboot/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/TN_firstboot $(1)/etc/init.d/
	chmod +x $(1)/etc/init.d/TN_firstboot
	$(INSTALL_DIR) $(1)/usr/share/InnoRoute
	$(INSTALL_BIN) ./files/firstboot.sh $(1)/usr/share/InnoRoute/
endef
define Package/TN_firstboot/postinst
#!/bin/sh
# check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
	echo "Enabling rc.d symlink for TN_firstboot"
	/etc/init.d/TN_firstboot enable
fi
exit 0
endef

define Package/TN_firstboot/prerm
#!/bin/sh
# check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
	echo "Removing rc.d symlink for TN_firstboot"
	/etc/init.d/TN_firstboot disable
fi
exit 0
endef
$(eval $(call BuildPackage,TN_firstboot)) 


	
  
	
  


  

  
	
  

 	


