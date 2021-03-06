# 
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk
CONFIG_TARGET_UEFI_PARTSIZE=400
FAT32_BLOCK_SIZE=1024
FAT32_BLOCKS=$(shell echo $$(($(CONFIG_TARGET_UEFI_PARTSIZE)*1024*1024/$(FAT32_BLOCK_SIZE))))
export PATH=$(TARGET_PATH):/sbin

GRUB2_MODULES = biosdisk boot chain configfile ext2 linux ls part_msdos reboot serial vga
GRUB2_MODULES_ISO = biosdisk boot chain configfile iso9660 linux ls part_msdos reboot serial vga
GRUB_TERMINALS =
GRUB_SERIAL_CONFIG =
GRUB_TERMINAL_CONFIG =
GRUB_CONSOLE_CMDLINE = console=tty0 console=ttyS0,115200n8

USE_ATKBD = generic kvm_guest

ifneq ($(strip $(foreach subtarget,$(USE_ATKBD),$(CONFIG_TARGET_x86_$(subtarget)))),)
  GRUB2_MODULES += at_keyboard
  GRUB2_MODULES_ISO += at_keyboard
endif

ifneq ($(CONFIG_GRUB_CONSOLE),)
  GRUB_CONSOLE_CMDLINE += console=tty0
  GRUB_TERMINALS += console
endif

GRUB_SERIAL:=$(call qstrip,$(CONFIG_GRUB_SERIAL))

ifneq ($(GRUB_SERIAL),)
  GRUB_CONSOLE_CMDLINE += console=$(GRUB_SERIAL),$(CONFIG_GRUB_BAUDRATE)n8
  GRUB_SERIAL_CONFIG := serial --unit=0 --speed=$(CONFIG_GRUB_BAUDRATE) --word=8 --parity=no --stop=1 --rtscts=off
  GRUB_TERMINALS += serial
endif

ifneq ($(GRUB_TERMINALS),)
  GRUB_TERMINAL_CONFIG := terminal_input $(GRUB_TERMINALS); terminal_output $(GRUB_TERMINALS)
endif

SIGNATURE:=$(shell dd if=/dev/urandom bs=4 count=1 2>/dev/null | hexdump -v -e '"%08x"')
ROOTPART:=$(call qstrip,$(CONFIG_TARGET_ROOTFS_PARTNAME))
ROOTPART:=$(if $(ROOTPART),$(ROOTPART),PARTUUID=$(SIGNATURE)-02)

GRUB_TIMEOUT:=$(call qstrip,$(CONFIG_GRUB_TIMEOUT))
  BOOTOPTS:=$(call qstrip,$(CONFIG_GRUB_BOOTOPTS))

  define Image/cmdline/ext4
    root=$(ROOTPART) rootfstype=ext4 rootwait
  endef

ifneq ($(CONFIG_GRUB_IMAGES),)

  BOOTOPTS:=$(call qstrip,$(CONFIG_GRUB_BOOTOPTS))

  define Image/cmdline/ext4
    root=$(ROOTPART) rootfstype=ext4 rootwait
  endef

  define Image/cmdline/jffs2-64k
    block2mtd.block2mtd=$(ROOTPART),65536,rootfs,5 root=/dev/mtdblock0 rootfstype=jffs2 rootwait
  endef

  define Image/cmdline/jffs2-128k
    block2mtd.block2mtd=$(ROOTPART),131072,rootfs,5 root=/dev/mtdblock0 rootfstype=jffs2 rootwait
  endef

  define Image/cmdline/squashfs
    block2mtd.block2mtd=$(ROOTPART),65536,rootfs,5 root=/dev/mtdblock0 rootfstype=squashfs rootwait
  endef

  define Image/Build/grub2
	# left here because the image builder doesnt need these
	$(INSTALL_DIR) $(KDIR)/root.grub/boot/grub $(KDIR)/grub2
	$(CP) $(KDIR)/bzImage $(KDIR)/root.grub/boot/vmlinuz
	grub-mkimage \
		-d $(STAGING_DIR_HOST)/lib/grub/i386-pc \
		-o $(KDIR)/grub2/core.img \
		-O i386-pc \
		-c ./grub-early.cfg \
		$(GRUB2_MODULES)
	$(CP) $(STAGING_DIR_HOST)/lib/grub/i386-pc/*.img $(KDIR)/grub2/
	echo '(hd0) $(BIN_DIR)/$(IMG_PREFIX)-combined-$(1).img' > $(KDIR)/grub2/device.map
	sed \
		-e 's#@SERIAL_CONFIG@#$(strip $(GRUB_SERIAL_CONFIG))#g' \
		-e 's#@TERMINAL_CONFIG@#$(strip $(GRUB_TERMINAL_CONFIG))#g' \
		-e 's#@CMDLINE@#$(strip $(call Image/cmdline/$(1)) $(BOOTOPTS) $(GRUB_CONSOLE_CMDLINE))#g' \
		-e 's#@TIMEOUT@#$(GRUB_TIMEOUT)#g' \
		./grub.cfg > $(KDIR)/root.grub/boot/grub/grub.cfg
	PADDING="$(CONFIG_TARGET_IMAGES_PAD)" SIGNATURE="$(SIGNATURE)" PATH="$(TARGET_PATH)" ./gen_image_generic.sh \
		$(BIN_DIR)/$(IMG_PREFIX)-combined-$(1).img \
		$(CONFIG_TARGET_KERNEL_PARTSIZE) $(KDIR)/root.grub \
		$(CONFIG_TARGET_ROOTFS_PARTSIZE) $(KDIR)/root.$(1) \
		256
	grub-bios-setup \
		--device-map="$(KDIR)/grub2/device.map" \
		-d "$(KDIR)/grub2" \
		-r "hd0,msdos1" \
		"$(BIN_DIR)/$(IMG_PREFIX)-combined-$(1).img"
	$(call Image/Build/grub/$(1))
  endef
endif

ROOTDELAY=10

define Image/Build/squashfs
	$(call prepare_generic_squashfs,$(KDIR)/root.squashfs)
endef

define Image/Build/iso
	$(INSTALL_DIR) $(KDIR)/root.grub/boot/grub $(KDIR)/grub2
	$(CP) $(KDIR)/bzImage $(KDIR)/root.grub/boot/vmlinuz
	grub-mkimage \
		-d $(STAGING_DIR_HOST)/lib/grub/i386-pc \
		-o $(KDIR)/grub2/eltorito.img \
		-O i386-pc \
		-c ./grub-early.cfg \
		$(GRUB2_MODULES_ISO)
	cat \
		$(STAGING_DIR_HOST)/lib/grub/i386-pc/cdboot.img \
		$(KDIR)/grub2/eltorito.img \
		> $(KDIR)/root.grub/boot/grub/eltorito.img
	sed \
		-e 's#@SERIAL_CONFIG@#$(strip $(GRUB_SERIAL_CONFIG))#g' \
		-e 's#@TERMINAL_CONFIG@#$(strip $(GRUB_TERMINAL_CONFIG))#g' \
		-e 's#@CMDLINE@#root=/dev/sr0 rootfstype=iso9660 rootwait $(strip $(call Image/cmdline/$(1)) $(BOOTOPTS) $(GRUB_CONSOLE_CMDLINE))#g' \
		-e 's#@TIMEOUT@#$(GRUB_TIMEOUT)#g' \
		./grub-iso.cfg > $(KDIR)/root.grub/boot/grub/grub.cfg
	$(CP) $(KDIR)/bzImage $(KDIR)/root.grub/boot/vmlinuz
	mkisofs -R -b boot/grub/eltorito.img -no-emul-boot -boot-info-table \
		-o $(KDIR)/root.iso $(KDIR)/root.grub $(TARGET_DIR)
endef

ifneq ($(CONFIG_VDI_IMAGES),)
  define Image/Build/vdi
	rm $(BIN_DIR)/$(IMG_PREFIX)-combined-$(1).vdi || true
	qemu-img convert -f raw -O vdi \
		$(BIN_DIR)/$(IMG_PREFIX)-combined-$(1).img \
		$(BIN_DIR)/$(IMG_PREFIX)-combined-$(1).vdi
	# XXX: VBoxManage insists on setting perms to 0600
	chmod 0644 $(BIN_DIR)/$(IMG_PREFIX)-combined-$(1).vdi
  endef
endif

ifneq ($(CONFIG_VMDK_IMAGES),)
  define Image/Build/vmdk
	rm $(BIN_DIR)/$(IMG_PREFIX)-combined-$(1).vmdk || true
	qemu-img convert -f raw -O vmdk \
		$(BIN_DIR)/$(IMG_PREFIX)-combined-$(1).img \
		$(BIN_DIR)/$(IMG_PREFIX)-combined-$(1).vmdk
  endef
endif

define Image/Build/gzip
#	gzip -f9n $(BIN_DIR)/$(IMG_PREFIX)-combined-$(1).img
#	gzip -f9n $(BIN_DIR)/$(IMG_PREFIX)-rootfs-$(1).img
	sed \
		-e 's#@SERIAL_CONFIG@#$(strip $(GRUB_SERIAL_CONFIG))#g' \
		-e 's#@TERMINAL_CONFIG@#$(strip $(GRUB_TERMINAL_CONFIG))#g' \
		-e 's#@CMDLINE@#$(strip $(call Image/cmdline/$(1)) $(BOOTOPTS) $(GRUB_CONSOLE_CMDLINE))#g' \
		-e 's#@TIMEOUT@#$(GRUB_TIMEOUT)#g' \
		./grub_skelet.cfg > grub.cfg
	rm -f $(BIN_DIR)/$(IMG_PREFIX)-uefi.img
	mkfs.fat -C $(BIN_DIR)/$(IMG_PREFIX)-uefi.img $(FAT32_BLOCKS)
	mmd -i $(BIN_DIR)/$(IMG_PREFIX)-uefi.img ::/efi
	mmd -i $(BIN_DIR)/$(IMG_PREFIX)-uefi.img ::/boot
	mmd -i $(BIN_DIR)/$(IMG_PREFIX)-uefi.img ::/boot/efi
	mmd -i $(BIN_DIR)/$(IMG_PREFIX)-uefi.img ::/efi/grub
	mcopy -i $(BIN_DIR)/$(IMG_PREFIX)-uefi.img inrcontent/* ::
	mcopy -i $(BIN_DIR)/$(IMG_PREFIX)-uefi.img grub_efi/* ::/efi/grub/
	mcopy -i $(BIN_DIR)/$(IMG_PREFIX)-uefi.img grub_efi/startup.nsh ::
	mcopy -i $(BIN_DIR)/$(IMG_PREFIX)-uefi.img grub.cfg ::/efi/grub/
	mcopy -i $(BIN_DIR)/$(IMG_PREFIX)-uefi.img $(KDIR)/vmlinux ::/boot/
	mcopy -i $(BIN_DIR)/$(IMG_PREFIX)-uefi.img $(KDIR)/bzImage ::/boot/efi/vmlinuz.efi
	mcopy -i $(BIN_DIR)/$(IMG_PREFIX)-uefi.img $(KDIR)/bzImage ::/boot/vmlinuz.efi
	mcopy -i $(BIN_DIR)/$(IMG_PREFIX)-uefi.img $(KDIR)/bzImage ::/boot/bootx64.efi
	mcopy -i $(BIN_DIR)/$(IMG_PREFIX)-uefi.img $(KDIR)/vmlinux.elf ::/boot/
	./gen_rpi_sdcard_img.sh $(BIN_DIR)/$(IMG_PREFIX)-combined-uefi.img $(BIN_DIR)/$(IMG_PREFIX)-uefi.img $(BIN_DIR)/$(IMG_PREFIX)-rootfs-$(1).img $(CONFIG_TARGET_UEFI_PARTSIZE) $(CONFIG_TARGET_ROOTFS_PARTSIZE) $(SIGNATURE)
	gzip -f9n $(BIN_DIR)/$(IMG_PREFIX)-combined-uefi.img
endef

#define Image/Build/uefi

#endef

ifneq ($(CONFIG_TARGET_IMAGES_GZIP),)
  define Image/Build/gzip/ext4
	$(call Image/Build/gzip,ext4)
  endef
  ifneq ($(CONFIG_TARGET_IMAGES_PAD),)
    define Image/Build/gzip/squashfs
	$(call Image/Build/gzip,squashfs)
    endef
    define Image/Build/gzip/jffs2-64k
	$(call Image/Build/gzip,jffs2-64k)
    endef
    define Image/Build/gzip/jffs2-128k
	$(call Image/Build/gzip,jffs2-128k)
    endef
  endif
endif

define Image/BuildKernel
	$(CP) $(KDIR)/bzImage $(BIN_DIR)/$(IMG_PREFIX)-vmlinuz
  ifneq ($(CONFIG_X86_ETHERBOOT_IMAGES),)
	rm -f $(BIN_DIR)/$(IMG_PREFIX)-etherboot
	$(STAGING_DIR_HOST)/bin/mkelfImage \
		--append=$(CONFIG_X86_ETHERBOOT_BOOTOPTS) \
		$(KDIR)/bzImage \
		$(BIN_DIR)/$(IMG_PREFIX)-etherboot
  endif
endef

define Image/Build/Profile/ar525w
	cp $(KDIR)/bzImage $(KDIR)/bzImage.tmp
	$(SCRIPT_DIR)/pad_image $(1) $(KDIR)/bzImage.tmp $(KDIR)/root.$(1) 32
	$(STAGING_DIR_HOST)/bin/airlink -b 1 $(KDIR)/bzImage.tmp $(KDIR)/root.$(1) \
		$(BIN_DIR)/$(IMG_PREFIX)-$(1)-ar525w.img
	$(STAGING_DIR_HOST)/bin/airlink -e -b 1 $(KDIR)/bzImage.tmp $(KDIR)/root.$(1) \
		$(BIN_DIR)/$(IMG_PREFIX)-$(1)-ar525w-web.img
endef

define Image/Build/Profile/sitecom
	cp $(KDIR)/bzImage $(KDIR)/bzImage.tmp
	#32k config data + 20 bytes header + 2 bytes checksum after kernel image
	$(SCRIPT_DIR)/pad_image $(1) $(KDIR)/bzImage.tmp $(KDIR)/root.$(1) 32790
	$(TOPDIR)/target/linux/rdc/image/mkimg_sitecom.pl $(KDIR)/bzImage.tmp > $(KDIR)/tmp.img
	cat $(KDIR)/root.$(1) >> $(KDIR)/tmp.img
	cp $(KDIR)/tmp.img $(BIN_DIR)/$(IMG_PREFIX)-$(1)-sitecom.img
	rm $(KDIR)/tmp.img $(KDIR)/bzImage.tmp
endef

define Image/Build/Profile/bifferboard
	$(TOPDIR)/target/linux/rdc/image/mkimg_bifferboard.py $(KDIR)/bzImage $(KDIR)/root.$(1) \
		$(BIN_DIR)/$(IMG_PREFIX)-$(1)-bifferboard.img
endef

define Image/Prepare
	$(call Image/Prepare/grub2)
endef

define Image/Build/Initramfs
	$(CP) $(KDIR)/bzImage-initramfs $(BIN_DIR)/$(IMG_PREFIX)-ramfs.bzImage
endef

define Image/Build
	$(call Image/Build/$(1))
  ifneq ($(1),iso)
	$(call Image/Build/grub2,$(1))
	$(call Image/Build/vdi,$(1))
	$(call Image/Build/vmdk,$(1))
	$(CP) $(KDIR)/root.$(1) $(BIN_DIR)/$(IMG_PREFIX)-rootfs-$(1).img
  else
	$(CP) $(KDIR)/root.iso $(BIN_DIR)/$(IMG_PREFIX).iso
  endif
	$(CP) $(KDIR)/bzImage $(BIN_DIR)/$(IMG_PREFIX)-vmlinuz
	$(call Image/Build/gzip/$(1))
	$(call Image/Build/uefi/$(1))
	$(call Image/Build/Profile/$(PROFILE),$(1))
ifeq ($(CONFIG_TARGET_ROOTFS_INITRAMFS),y)
	$(call Image/Build/Initramfs)
endif
endef

$(eval $(call BuildImage))
