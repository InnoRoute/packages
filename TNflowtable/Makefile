include	$(TOPDIR)/rules.mk
PKG_NAME :=tnflowtable
PKG_RELEASE := 1.3
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/kernel-defaults.mk


define Package/tnflowtable
	SECTION:=InnoRoute
	CATEGORY:=InnoRoute
	TITLE:=Trustnode flowtable config tool
	DEPENDS:=+libc
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/tnflowtable/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/share/InnoRoute/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tnflowtable $(1)/usr/sbin/TNflowtable
endef

define KernelPackage/tnflowtable
	SUBMENU:=InnoRoute
	TITLE:=tnflowtable Kernel Package
	FILES:= $(PKG_BUILD_DIR)/TNflowtable.ko
	AUTOLOAD:=$(call AutoLoad,50,TNflowtable)
endef

define KernelPackage/tnflowtable/description
	tnflowtable kernel module for OVS interaction
endef

define Build/Compile
	rm -f $(PKG_BUILD_DIR)/*.o
	$(MAKE) $(KERNEL_MAKEOPTS) SUBDIRS="$(PKG_BUILD_DIR)" modules
	rm -f $(PKG_BUILD_DIR)/*.o
	$(MAKE) -C $(PKG_BUILD_DIR) tnflowtable
endef

$(eval $(call BuildPackage,tnflowtable,+argp-standalone,libc))
$(eval $(call KernelPackage,tnflowtable))
