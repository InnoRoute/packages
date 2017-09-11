include	$(TOPDIR)/rules.mk
PKG_NAME :=tnflowtable
PKG_RELEASE := 1.3
PKG_BUILD_DIR:=	$(BUILD_DIR)/$(PKG_NAME)
include	$(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/kernel-defaults.mk
#include $(INCLUDE_DIR)/cmake.mk 

define Package/tnflowtable
	SECTION:=InnoRoute
	CATEGORY:=InnoRoute
	TITLE:=Trustnode flowtable config tool
	DEPENDS:=+libc
#	+argp-standalone
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/tnflowtable/install
	$(INSTALL_DIR) $(1)/usr/sbin
#	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/share/InnoRoute/
	$(INSTALL_CONF) ./files/tn_fc.bit $(1)/usr/share/InnoRoute/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tnflowtable $(1)/usr/sbin/TNflowtable
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libtnlibflowtable.so $(1)/usr/lib/	
endef

define KernelPackage/tnflowtable
	SUBMENU:=InnoRoute
	TITLE:=tnflowtable Kernel Package
#	+kmod-TNopenvswitch
	FILES:= $(PKG_BUILD_DIR)/TNflowtable.ko
	AUTOLOAD:=$(call AutoLoad,50,TNflowtable)
endef

define KernelPackage/tnflowtable/description
	tnflowtable kernel module for OVS interaction
endef

define Build/Compile
#	$(MAKE) -f $(PKG_BUILD_DIR)/KMakefile $(KERNEL_MAKEOPTS)  SUBDIRS="$(PKG_BUILD_DIR) modules" 
#	rm $(PKG_BUILD_DIR)/*.o
#	cmake $(PKG_BUILD_DIR)
#	$(MAKE) -C $(PKG_BUILD_DIR)
#	$(MAKE) -C $(PKG_BUILD_DIR) CXX="$(TARGET_CROSS)g++" tnflowtable
#	rm $(PKG_BUILD_DIR)/*.o
	$(MAKE) $(KERNEL_MAKEOPTS) SUBDIRS="$(PKG_BUILD_DIR)" modules
	rm $(PKG_BUILD_DIR)/*.o
	$(MAKE) -C $(PKG_BUILD_DIR) tnflowtable
endef

$(eval $(call BuildPackage,tnflowtable,+argp-standalone,libc))
$(eval $(call KernelPackage,tnflowtable))