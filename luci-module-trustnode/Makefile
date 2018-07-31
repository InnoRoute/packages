include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/version.mk

PKG_NAME:=tn-luci-plugin
PKG_RELEASE:=1.2
PKG_BUILD_DEPENDS:=base-files
PKG_FILE_DEPENDS:=
PKG_LICENSE:=GPL-2.0

PKG_MAINTAINER:=m.Ulbricht <ulbricht@innoroute.de>
#based on https://github.com/Nossiac/mtk-openwrt-feeds
#adapted by ulbricht@innoroute.de
include $(INCLUDE_DIR)/package.mk

HTDOCS = /www
LUA_LIBRARYDIR = /usr/lib/lua
LUCI_LIBRARYDIR = $(LUA_LIBRARYDIR)/luci


define Package/tn-luci-plugin
  SECTION:=LuCI
  CATEGORY:=LuCI
  SUBMENU:=InnoRoute
  DEPENDS:=+lua
  TITLE:=TrustNode LuCI plugins.
  VERSION:=$(PKG_RELEASE)-$(REVISION)
endef


define Package/tn-luci-plugin/config
	config LUCI_APP_WEBCONSOLE
	bool "luci-app-webconsole"
	depends on PACKAGE_tn-luci-plugin
	default y
	help
	  "LuCI plugin for access shell from web"
	  
	config LUCI_APP_TNSTATISTICS
	bool "luci-app-TNstatistics"
	depends on PACKAGE_tn-luci-plugin
	default y
	help
	  "Distplay TrustNode FPGA core statistics"
	
	config LUCI_APP_TNDOKU
	bool "luci-app-TNdoku"
	depends on PACKAGE_tn-luci-plugin
	default y
	help
	  "Display TrustNode documentation"
	  
	config LUCI_APP_TNUPLOAD
	bool "luci-app-TNupload"
	depends on PACKAGE_tn-luci-plugin
	default y
	help
	  "Upload FPGA bitstreams from WEB-UI"
endef


define Build/Compile

endef

define Package/common/install
	if [ -d $(2)/img ]; then \
	  $(INSTALL_DIR) $(1)/www/luci-static/innoroute; \
	  cp -pR ./$(2)/img/* $(1)/www/luci-static/innoroute/; \
	else true; fi
	
	if [ -d $(2)/luasrc ]; then \
	  $(INSTALL_DIR) $(1)$(LUCI_LIBRARYDIR); \
	  cp -pR ./$(2)/luasrc/* $(1)$(LUCI_LIBRARYDIR)/; \
	else true; fi
	if [ -d $(2)/htdocs ]; then \
	  $(INSTALL_DIR) $(1)$(HTDOCS); \
	  cp -pR ./$(2)/htdocs/* $(1)$(HTDOCS)/; \
	else true; fi
	if [ -d $(2)/root ]; then \
	  $(INSTALL_DIR) $(1)/; \
	  cp -pR ./$(2)/root/* $(1)/; \
	else true; fi
	if [ -d $(2)/src ]; then \
	  $(call Build/Install/Default) \
	  $(CP) ./$(PKG_INSTALL_DIR)/* $(1)/; \
	else true; fi
endef


define Package/tn-luci-plugin/install
	$(call Package/common/install,$(1),luci-app-webconsole)
	$(call Package/common/install,$(1),luci-app-TNstatistics)
	$(call Package/common/install,$(1),luci-app-TNdoku)
	$(call Package/common/install,$(1),luci-app-TNupload)
endef


$(eval $(call BuildPackage,tn-luci-plugin))
