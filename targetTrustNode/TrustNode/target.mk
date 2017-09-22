ARCH:=x86_64
BOARDNAME:=TrustNode
DEFAULT_PACKAGES += kmod-button-hotplug kmod-e1000e kmod-e1000 kmod-r8169
ARCH_PACKAGES:=x86_64
MAINTAINER:=InnoRoute GmbH ulbricht@innoroute.de

define Target/Description
        Build images for 64 bit systems including virtualized guests.
endef
