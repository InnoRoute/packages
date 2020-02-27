include	$(TOPDIR)/rules.mk
PKG_NAME :=TN_docker
PKG_RELEASE := 1.4
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk

define Package/TN_docker
 SECTION:=TN_docker
 CATEGORY:=InnoRoute
 TITLE:=TN_docker
 DEPENDS:=
 endef
define Build/Prepare
#mkdir ‐p $(PKG_BUILD_DIR)
	
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	wget -P $(PKG_BUILD_DIR)/ https://download.docker.com/linux/static/stable/x86_64/docker-19.03.3.tgz
	tar xf  $(PKG_BUILD_DIR)/docker-19.03.3.tgz -C $(PKG_BUILD_DIR)/
	
endef
define Package/TN_docker/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/docker/* $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/TN_docker $(1)/etc/init.d/

endef

define Package/TN_docker/postinst
!/bin/sh
 check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
	echo "Enabling rc.d symlink for TN_docker"
	/etc/init.d/TN_docker enable
fi
exit 0
endef

define Package/TN_docker/prerm
#!/bin/sh
# check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
	echo "Removing rc.d symlink for TN_docker"
	/etc/init.d/TN_docker disable
fi
exit 0
endef
$(eval $(call BuildPackage,TN_docker)) 


	
  
	
  


  

  
	
  

 	


