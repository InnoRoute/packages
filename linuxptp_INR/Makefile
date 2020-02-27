include $(TOPDIR)/rules.mk
PKG_NAME:=linuxptp_INR
PKG_RELEASE := 0.1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
include $(INCLUDE_DIR)/package.mk

define Package/linuxptp_INR
	SECTION:=InnoRoute
	CATEGORY:=InnoRoute
	TITLE:=linuxptp with sysrepo
	DEPENDS:=+libc +sysrepo
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
#	$(CP) ./files/* $(PKG_BUILD_DIR)/
#	$(CP) $(STAGING_DIR)/usr/include/tn_env.h $(PKG_BUILD_DIR)/
endef
EXTRA_CFLAGS += -DHAVE_CLOCK_ADJTIME -DHAVE_POSIX_SPAWN -DHAVE_ONESTEP_SYNC

MAKE_VARS += EXTRA_CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(EXTRA_CFLAGS)"
define Package/linuxptp_INR/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hwstamp_ctl $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/phc2sys $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/phc_ctl $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pmc $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ptp4l $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/timemaster $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/sysrepo/yang/
	$(INSTALL_CONF) ./files/yang/* $(1)/etc/sysrepo/yang/
endef
TARGET_LDFLAGS+=-lsysrepo
define Build/Compile
	$(MAKE)  -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" CFLAGS="$(TARGET_CFLAGS)" LDFLAGS="$(TARGET_LDFLAGS)"
endef

$(eval $(call BuildPackage,linuxptp_INR))
