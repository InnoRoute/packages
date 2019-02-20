include	$(TOPDIR)/rules.mk
PKG_NAME :=tnsysrepo
PKG_RELEASE := 0.1
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk
include	$(INCLUDE_DIR)/package-defaults.mk

#include $(INCLUDE_DIR)/kernel.mk
#include $(INCLUDE_DIR)/kernel-defaults.mk

define Package/tnsysrepo
	SECTION:=InnoRoute
	CATEGORY:=InnoRoute
	TITLE:=Trustnode sysrepo interface
	DEPENDS:=+libc +TN_tools +sysrepo
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) $(STAGING_DIR)/usr/include/tn_env.h $(PKG_BUILD_DIR)/
	$(CP) ./src/*.h $(STAGING_DIR)/usr/include/
	
endef

define Package/tnsysrepo/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/share/InnoRoute/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tnsysrepo_main $(1)/usr/sbin/TNsysrepo
#	$(INSTALL_BIN) files/*.sh $(1)/usr/share/InnoRoute/
	$(INSTALL_DIR) $(1)/etc/sysrepo/yang/
	$(INSTALL_CONF) ./files/yang/* $(1)/etc/sysrepo/yang/
endef


TARGET_LDFLAGS+=-lsysrepo
define Build/Compile

	rm -f $(PKG_BUILD_DIR)/*.o
	 $(MAKE)  -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" CFLAGS="$(TARGET_CFLAGS)" LDFLAGS="$(TARGET_LDFLAGS)" tnsysrepo_main
endef

$(eval $(call BuildPackage,tnsysrepo,+argp-standalone,libc,+TN_tools,+sysrepo))

