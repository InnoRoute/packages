#!/bin/sh

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

# derived from tn_lib/sim/mmi_stim.txt

# Addresses should be converted to base and sub addresses

echo "C_SUB_ADDR_BM_MMI_STATUS"
tn_ll_mmi_read 0x000000
echo "C_SUB_ADDR_BM_MMI_INTERRUPT"
tn_ll_mmi_read 0x000004

echo "C_SUB_ADDR_MDIO_WRITE"
tn_ll_mmi_read 0x000100
echo "C_SUB_ADDR_MDIO_READ"
tn_ll_mmi_read 0x000104
echo "C_SUB_ADDR_MDIO_INTERRUPT"
tn_ll_mmi_read 0x000108
echo "C_SUB_ADDR_MDIO_INTERRUPT_EN"
tn_ll_mmi_read 0x00010C

echo "C_SUB_ADDR_PERIPH_BUTTONS"
tn_ll_mmi_read 0x000200
echo "C_SUB_ADDR_PERIPH_LEDS"
tn_ll_mmi_read 0x000204
echo "C_SUB_ADDR_PERIPH_GPIOS"
tn_ll_mmi_read 0x000208
echo "C_SUB_ADDR_PERIPH_DISPLAY0"
tn_ll_mmi_read 0x00020C
echo "C_SUB_ADDR_PERIPH_DISPLAY1"
tn_ll_mmi_read 0x000210
echo "C_SUB_ADDR_PERIPH_DISPLAY2"
tn_ll_mmi_read 0x000214
echo "C_SUB_ADDR_PERIPH_DISPLAY3"
tn_ll_mmi_read 0x000218
echo "C_SUB_ADDR_PERIPH_BEEP"
tn_ll_mmi_read 0x00021C
echo "C_SUB_ADDR_PERIPH_BRIGHTNESS"
tn_ll_mmi_read 0x000220

echo "C_SUB_ADDR_COMMON_TN_MAJOR_REV"
tn_ll_mmi_read 0x000400
echo "C_SUB_ADDR_COMMON_TN_MINOR_REV"
tn_ll_mmi_read 0x000404
echo "C_SUB_ADDR_COMMON_USER_REV"
tn_ll_mmi_read 0x000408
echo "C_SUB_ADDR_COMMON_ADDR_MAP_REV"
tn_ll_mmi_read 0x00040C
echo "C_SUB_ADDR_COMMON_FPGA_ID0"
tn_ll_mmi_read 0x000410
echo "C_SUB_ADDR_COMMON_FPGA_ID1"
tn_ll_mmi_read 0x000414
echo "C_SUB_ADDR_COMMON_EFUSE"
tn_ll_mmi_read 0x000418
echo "C_SUB_ADDR_COMMON_BITGEN_TIME"
tn_ll_mmi_read 0x00041C
echo "C_SUB_ADDR_COMMON_RW_TEST"
tn_ll_mmi_read 0x000420
echo "C_SUB_ADDR_COMMON_FEATURES_6TREE"
tn_ll_mmi_read 0x000500
echo "C_SUB_ADDR_COMMON_FEATURES_ETH_SW"
tn_ll_mmi_read 0x000504
echo "C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE"
tn_ll_mmi_read 0x000508
echo "C_SUB_ADDR_COMMON_FEATURES_ACC_DP"
tn_ll_mmi_read 0x00050C
echo "C_SUB_ADDR_COMMON_FEATURES_TX_ROUTER"
tn_ll_mmi_read 0x000510
echo "C_SUB_ADDR_COMMON_FEATURES_SFP"
tn_ll_mmi_read 0x000514
echo "C_SUB_ADDR_COMMON_FEATURES_SODIMM"
tn_ll_mmi_read 0x000518
echo "C_SUB_ADDR_COMMON_FEATURES_USER"
tn_ll_mmi_read 0x00051C
echo "C_SUB_ADDR_COMMON_PARAM_PRT_CNT"
tn_ll_mmi_read 0x000520
echo "C_SUB_ADDR_COMMON_PARAM_DP_CNT"
tn_ll_mmi_read 0x000524
echo "C_SUB_ADDR_COMMON_PARAM_BUF_CNT"
tn_ll_mmi_read 0x000528
echo "C_SUB_ADDR_COMMON_PARAM_RX_MIN"
tn_ll_mmi_read 0x000530
echo "C_SUB_ADDR_COMMON_PARAM_RX_MAX"
tn_ll_mmi_read 0x000534
echo "C_SUB_ADDR_COMMON_PARAM_TX_PRE"
tn_ll_mmi_read 0x000538
echo "C_SUB_ADDR_COMMON_PARAM_TX_IFG"
tn_ll_mmi_read 0x00053C
echo "C_SUB_ADDR_COMMON_PARAM_DISPLAY"
tn_ll_mmi_read 0x000540
echo "C_SUB_ADDR_COMMON_PARAM_LED_SYNCE"
tn_ll_mmi_read 0x000544
echo "C_SUB_ADDR_COMMON_PARAM_LED_INTERNAL"
tn_ll_mmi_read 0x000548
echo "C_SUB_ADDR_COMMON_PARAM_BEEP"
tn_ll_mmi_read 0x00054C
echo "C_SUB_ADDR_COMMON_PARAM_PHY_RESET"
tn_ll_mmi_read 0x000550
echo "C_SUB_ADDR_COMMON_PARAM_PINHEADER"
tn_ll_mmi_read 0x000554
echo "C_SUB_ADDR_COMMON_PARAM_PMOD"
tn_ll_mmi_read 0x000558
echo "C_SUB_ADDR_COMMON_PARAM_EXT_BYPASS"
tn_ll_mmi_read 0x00055C
echo "C_SUB_ADDR_COMMON_WIDTH_HC_BAD"
tn_ll_mmi_read 0x000560
echo "C_SUB_ADDR_COMMON_WIDTH_ETH_SW"
tn_ll_mmi_read 0x000564
echo "C_SUB_ADDR_COMMON_WIDTH_RX_STAT_BAD"
tn_ll_mmi_read 0x000568
echo "C_SUB_ADDR_COMMON_FPGA_TEMP"
tn_ll_mmi_read 0x000600
echo "C_SUB_ADDR_COMMON_FPGA_ALARM"
tn_ll_mmi_read 0x000604
echo "C_SUB_ADDR_COMMON_CONFIG_CHECK"
tn_ll_mmi_read 0x000608
echo "C_SUB_ADDR_COMMON_FIFO_OVERFLOW0"
tn_ll_mmi_read 0x000610
echo "C_SUB_ADDR_COMMON_FIFO_OVERFLOW1"
tn_ll_mmi_read 0x000614
echo "C_SUB_ADDR_COMMON_FIFO_OVERFLOW2"
tn_ll_mmi_read 0x000618
echo "C_SUB_ADDR_COMMON_FIFO_OVERFLOW3"
tn_ll_mmi_read 0x00061C
echo "C_SUB_ADDR_COMMON_FIFO_UNDERRUN0"
tn_ll_mmi_read 0x000620
echo "C_SUB_ADDR_COMMON_FIFO_UNDERRUN1"
tn_ll_mmi_read 0x000624
echo "C_SUB_ADDR_COMMON_FIFO_UNDERRUN2"
tn_ll_mmi_read 0x000628
echo "C_SUB_ADDR_COMMON_FIFO_UNDERRUN3"
tn_ll_mmi_read 0x00062C
echo "C_SUB_ADDR_COMMON_PCIE_ERROR0"
tn_ll_mmi_read 0x000630
echo "C_SUB_ADDR_COMMON_PCIE_ERROR1"
tn_ll_mmi_read 0x000634
echo "C_SUB_ADDR_COMMON_PCIE_TIMEOUT_CNT"
tn_ll_mmi_read 0x000638
echo "C_SUB_ADDR_COMMON_BUFFER_FULL"
tn_ll_mmi_read 0x000640
echo "C_SUB_ADDR_COMMON_TXF_FULL"
tn_ll_mmi_read 0x000644
echo "C_SUB_ADDR_COMMON_CLK_MON_0"
tn_ll_mmi_read 0x000650
echo "C_SUB_ADDR_COMMON_CLK_MON_1"
tn_ll_mmi_read 0x000654
echo "C_SUB_ADDR_COMMON_CLK_MON_2"
tn_ll_mmi_read 0x000658
echo "C_SUB_ADDR_COMMON_CLK_MON_3"
tn_ll_mmi_read 0x00065C
echo "C_SUB_ADDR_COMMON_CLK_MON_4"
tn_ll_mmi_read 0x000660
echo "C_SUB_ADDR_COMMON_CLK_MON_5"
tn_ll_mmi_read 0x000664
echo "C_SUB_ADDR_COMMON_CLK_MON_6"
tn_ll_mmi_read 0x000668
echo "C_SUB_ADDR_COMMON_CLK_MON_7"
tn_ll_mmi_read 0x00066C
echo "C_SUB_ADDR_COMMON_CLK_MON_8"
tn_ll_mmi_read 0x000670
echo "C_SUB_ADDR_COMMON_CLK_MON_9"
tn_ll_mmi_read 0x000674
echo "C_SUB_ADDR_COMMON_CLK_MON_10"
tn_ll_mmi_read 0x000678
echo "C_SUB_ADDR_COMMON_CLK_MON_11"
tn_ll_mmi_read 0x00067C
echo "C_SUB_ADDR_COMMON_CLK_MON_12"
tn_ll_mmi_read 0x000680
echo "C_SUB_ADDR_COMMON_CLK_MON_13"
tn_ll_mmi_read 0x000684
echo "C_SUB_ADDR_COMMON_CLK_MON_14"
tn_ll_mmi_read 0x000688
echo "C_SUB_ADDR_COMMON_CLK_MON_15"
tn_ll_mmi_read 0x00068C
echo "C_SUB_ADDR_COMMON_CLK_MON_16"
tn_ll_mmi_read 0x000690
echo "C_SUB_ADDR_COMMON_CLK_MON_17"
tn_ll_mmi_read 0x000694
echo "C_SUB_ADDR_COMMON_CLK_MON_18"
tn_ll_mmi_read 0x000698
echo "C_SUB_ADDR_COMMON_CLK_MON_19"
tn_ll_mmi_read 0x00069C
echo "C_SUB_ADDR_COMMON_CLK_MON_20"
tn_ll_mmi_read 0x0006A0
echo "C_SUB_ADDR_COMMON_CLK_MON_21"
tn_ll_mmi_read 0x0006A4
echo "C_SUB_ADDR_COMMON_CLK_MON_22"
tn_ll_mmi_read 0x0006A8
echo "C_SUB_ADDR_COMMON_CLK_MON_23"
tn_ll_mmi_read 0x0006AC
echo "C_SUB_ADDR_COMMON_CLK_MON_24"
tn_ll_mmi_read 0x0006B0
echo "C_SUB_ADDR_COMMON_CLK_MON_25"
tn_ll_mmi_read 0x0006B4
echo "C_SUB_ADDR_COMMON_CLK_MON_26"
tn_ll_mmi_read 0x0006B8
echo "C_SUB_ADDR_COMMON_CLK_MON_27"
tn_ll_mmi_read 0x0006BC

echo "C_BASE_ADDR_ACC0 Word0"
tn_ll_mmi_read 0x100000
echo "C_BASE_ADDR_ACC0 Word1"
tn_ll_mmi_read 0x100004
echo "C_BASE_ADDR_ACC0 Word2"
tn_ll_mmi_read 0x100008
echo "C_BASE_ADDR_ACC0 Word3"
tn_ll_mmi_read 0x10000C
echo "C_BASE_ADDR_ACC1 Word0"
tn_ll_mmi_read 0x280000
echo "C_BASE_ADDR_ACC1 Word1"
tn_ll_mmi_read 0x280004
echo "C_BASE_ADDR_ACC1 Word2"
tn_ll_mmi_read 0x280008
echo "C_BASE_ADDR_ACC1 Word3"
tn_ll_mmi_read 0x28000C

echo "C_SUB_ADDR_NET_TAP (write 11 to all Ports)"
tn_ll_mmi_read 0x400000
tn_ll_mmi_write 0x400000 0x000BFFFF

echo "C_SUB_ADDR_NET_ENABLE (Enable all MACs)"
tn_ll_mmi_read 0x400010
tn_ll_mmi_write 0x400010 0x0000FFFF
echo "C_SUB_ADDR_NET_SPEED (100 Mbps to all MACs)"
tn_ll_mmi_read 0x400014
tn_ll_mmi_write 0x400014 0x55555555
echo "C_SUB_ADDR_NET_DUPLEX (Full Duplex to all MACs)"
tn_ll_mmi_read 0x400018
tn_ll_mmi_write 0x400018 0x0000FFFF
echo "C_SUB_ADDR_NET_PAUSE (Enable PAUSE to all MACs)"
tn_ll_mmi_read 0x40001C
tn_ll_mmi_write 0x40001C 0x0000FFFF
echo "C_SUB_ADDR_NET_OWN_MAC_ADDR_L (Own MAC: 00:12:24:36:...)"
tn_ll_mmi_read 0x400020
tn_ll_mmi_write 0x400020 0x36241200
echo "C_SUB_ADDR_NET_OWN_MAC_ADDR_H (Own MAC: 00:02:04:06:18:2C)"
tn_ll_mmi_read 0x400024
tn_ll_mmi_write 0x400024 0x00002C18

echo "C_SUB_ADDR_NET_6T_ADDR_L"
tn_ll_mmi_read 0x400030
tn_ll_mmi_write 0x400030 0xAABBCCDD
echo "C_SUB_ADDR_NET_6T_ADDR_H"
tn_ll_mmi_read 0x400034
tn_ll_mmi_write 0x400034 0xEEFF1122
echo "C_SUB_ADDR_NET_6T_SRC_PTR"
tn_ll_mmi_read 0x400038
tn_ll_mmi_write 0x400038 0x00000002
echo "C_SUB_ADDR_NET_6T_DST_PTR"
tn_ll_mmi_read 0x40003C
tn_ll_mmi_write 0x40003C 0x00000002

echo "C_SUB_ADDR_NET_TX_CONF_VLD"
tn_ll_mmi_read 0x400040
tn_ll_mmi_write 0x400040 0xFFFFFFFF

echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-ID Port 0"
tn_ll_mmi_read 0x400050
tn_ll_mmi_write 0x400050 0xFFFFFFFF
echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-Time Port 0"
tn_ll_mmi_read 0x400054
tn_ll_mmi_write 0x400054 0xFFFFFFFF

echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-ID Port 1"
tn_ll_mmi_read 0x400058
tn_ll_mmi_write 0x400058 0xFFFFFFFF
echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-Time Port 1"
tn_ll_mmi_read 0x40005C
tn_ll_mmi_write 0x40005C 0xFFFFFFFF

echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-ID Port 2"
tn_ll_mmi_read 0x400060
tn_ll_mmi_write 0x400060 0xFFFFFFFF
echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-Time Port 2"
tn_ll_mmi_read 0x400064
tn_ll_mmi_write 0x400064 0xFFFFFFFF

echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-ID Port 3"
tn_ll_mmi_read 0x400068
tn_ll_mmi_write 0x400068 0xFFFFFFFF
echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-Time Port 3"
tn_ll_mmi_read 0x40006C
tn_ll_mmi_write 0x40006C 0xFFFFFFFF

echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-ID Port 4"
tn_ll_mmi_read 0x400070
tn_ll_mmi_write 0x400070 0xFFFFFFFF
echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-Time Port 4"
tn_ll_mmi_read 0x400074
tn_ll_mmi_write 0x400074 0xFFFFFFFF

echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-ID Port 5"
tn_ll_mmi_read 0x400078
tn_ll_mmi_write 0x400078 0xFFFFFFFF
echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-Time Port 5"
tn_ll_mmi_read 0x40007C
tn_ll_mmi_write 0x40007C 0xFFFFFFFF

echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-ID Port 6"
tn_ll_mmi_read 0x400080
tn_ll_mmi_write 0x400080 0xFFFFFFFF
echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-Time Port 6"
tn_ll_mmi_read 0x400084
tn_ll_mmi_write 0x400084 0xFFFFFFFF

echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-ID Port 7"
tn_ll_mmi_read 0x400088
tn_ll_mmi_write 0x400088 0xFFFFFFFF
echo "C_SUB_ADDR_NET_TX_CONF_L - TX-Conf-Time Port 7"
tn_ll_mmi_read 0x40008C
tn_ll_mmi_write 0x40008C 0xFFFFFFFF

echo "C_SUB_ADDR_RTC_BRIDGE_LOW"
tn_ll_mmi_read 0x7F0000
echo "C_SUB_ADDR_RTC_BRIDGE_HIGH"
tn_ll_mmi_read 0x7F0004
echo "C_SUB_ADDR_RTC_CTRLD_LOW"
tn_ll_mmi_read 0x7F0008
echo "C_SUB_ADDR_RTC_CTRLD_HIGH"
tn_ll_mmi_read 0x7F000C
echo "C_SUB_ADDR_RTC_CTRLD_OFFSET_LOW"
tn_ll_mmi_read 0x7F0010
echo "C_SUB_ADDR_RTC_CTRLD_OFFSET_HIGH"
tn_ll_mmi_read 0x7F0014
echo "C_SUB_ADDR_RTC_CTRLD_RATE"
tn_ll_mmi_read 0x7F0018
echo "C_SUB_ADDR_RTC_INTERRUPT"   
tn_ll_mmi_read 0x7F001C
echo "C_SUB_ADDR_RTC_INTERRUPT_EN"
tn_ll_mmi_read 0x7F0020
echo "C_SUB_ADDR_RTC_CLKSEL"
tn_ll_mmi_read 0x7F0024

echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port0"
tn_ll_mmi_read 0x800000
echo "C_SUB_ADDR_HC_INTERRUPT Port0"
tn_ll_mmi_read 0x800004
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port0"
tn_ll_mmi_read 0x800008
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port1"
tn_ll_mmi_read 0x800100
echo "C_SUB_ADDR_HC_INTERRUPT Port1"
tn_ll_mmi_read 0x800104
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port1"
tn_ll_mmi_read 0x800108
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port2"
tn_ll_mmi_read 0x800200
echo "C_SUB_ADDR_HC_INTERRUPT Port2"
tn_ll_mmi_read 0x800204
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port2"
tn_ll_mmi_read 0x800208
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port3"
tn_ll_mmi_read 0x800300
echo "C_SUB_ADDR_HC_INTERRUPT Port3"
tn_ll_mmi_read 0x800304
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port3"
tn_ll_mmi_read 0x800308
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port4"
tn_ll_mmi_read 0x800400
echo "C_SUB_ADDR_HC_INTERRUPT Port4"
tn_ll_mmi_read 0x800404
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port4"
tn_ll_mmi_read 0x800408
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port5"
tn_ll_mmi_read 0x800500
echo "C_SUB_ADDR_HC_INTERRUPT Port5"
tn_ll_mmi_read 0x800504
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port5"
tn_ll_mmi_read 0x800508
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port6"
tn_ll_mmi_read 0x800600
echo "C_SUB_ADDR_HC_INTERRUPT Port6"
tn_ll_mmi_read 0x800604
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port6"
tn_ll_mmi_read 0x800608
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port7"
tn_ll_mmi_read 0x800700
echo "C_SUB_ADDR_HC_INTERRUPT Port7"
tn_ll_mmi_read 0x800704
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port7"
tn_ll_mmi_read 0x800708
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port8"
tn_ll_mmi_read 0x800800
echo "C_SUB_ADDR_HC_INTERRUPT Port8"
tn_ll_mmi_read 0x800804
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port8"
tn_ll_mmi_read 0x800808
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port9"
tn_ll_mmi_read 0x800900
echo "C_SUB_ADDR_HC_INTERRUPT Port9"
tn_ll_mmi_read 0x800904
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port9"
tn_ll_mmi_read 0x800908
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port10"
tn_ll_mmi_read 0x800A00
echo "C_SUB_ADDR_HC_INTERRUPT Port10"
tn_ll_mmi_read 0x800A04
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port10"
tn_ll_mmi_read 0x800A08
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port11"
tn_ll_mmi_read 0x800B00
echo "C_SUB_ADDR_HC_INTERRUPT Port11"
tn_ll_mmi_read 0x800B04
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port11"
tn_ll_mmi_read 0x800B08
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port12 (SFP)"
tn_ll_mmi_read 0x800C00
echo "C_SUB_ADDR_HC_INTERRUPT Port12 (SFP)"
tn_ll_mmi_read 0x800C04
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port12 (SFP)"
tn_ll_mmi_read 0x800C08
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port13 (SFP)"
tn_ll_mmi_read 0x800D00
echo "C_SUB_ADDR_HC_INTERRUPT Port13 (SFP)"
tn_ll_mmi_read 0x800D04
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port13 (SFP)"
tn_ll_mmi_read 0x800D08
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port14/PCIe0"
tn_ll_mmi_read 0x800E00
echo "C_SUB_ADDR_HC_INTERRUPT Port14/PCIe0"
tn_ll_mmi_read 0x800E04
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port14/PCIe0"
tn_ll_mmi_read 0x800E08
echo "C_SUB_ADDR_HC_CNT_BAD_FRAMES Port15/PCIe1"
tn_ll_mmi_read 0x800F00
echo "C_SUB_ADDR_HC_INTERRUPT Port15/PCIe1"
tn_ll_mmi_read 0x800F04
echo "C_SUB_ADDR_HC_INTERRUPT_EN Port15/PCIe1"
tn_ll_mmi_read 0x800F08

echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 0"
tn_ll_mmi_read 0x808000
tn_ll_mmi_read 0x808000
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 1"
tn_ll_mmi_read 0x808100
tn_ll_mmi_read 0x808100
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 2"
tn_ll_mmi_read 0x808200
tn_ll_mmi_read 0x808200
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 3"
tn_ll_mmi_read 0x808300
tn_ll_mmi_read 0x808300
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 4"
tn_ll_mmi_read 0x808400
tn_ll_mmi_read 0x808400
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 5"
tn_ll_mmi_read 0x808500
tn_ll_mmi_read 0x808500
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 6"
tn_ll_mmi_read 0x808600
tn_ll_mmi_read 0x808600
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 7"
tn_ll_mmi_read 0x808700
tn_ll_mmi_read 0x808700
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 8"
tn_ll_mmi_read 0x808800
tn_ll_mmi_read 0x808800
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 9"
tn_ll_mmi_read 0x808900
tn_ll_mmi_read 0x808900
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 10"
tn_ll_mmi_read 0x808A00
tn_ll_mmi_read 0x808A00
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 11"
tn_ll_mmi_read 0x808B00
tn_ll_mmi_read 0x808B00
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 12 (SFP)"
tn_ll_mmi_read 0x808C00
tn_ll_mmi_read 0x808C00
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 13 (SFP)"
tn_ll_mmi_read 0x808D00
tn_ll_mmi_read 0x808D00
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 14/PCIe0"
tn_ll_mmi_read 0x808E00
tn_ll_mmi_read 0x808E00
echo "C_SUB_ADDR_NOC_MON_STATUS (RO,COR) Port 15/PCIe1"
tn_ll_mmi_read 0x808F00
tn_ll_mmi_read 0x808F00

echo "C_SUB_ADDR_NOC_MON_FIRST_ERR to be added"

echo "C_BASE_ADDR_HT0 Word0"
tn_ll_mmi_read 0x808000
echo "C_BASE_ADDR_HT0 Word1"
tn_ll_mmi_read 0x808004
echo "C_BASE_ADDR_HT0 Word2"
tn_ll_mmi_read 0x808008
echo "C_BASE_ADDR_HT0 Word3"
tn_ll_mmi_read 0x80800C
echo "C_BASE_ADDR_HT1 Word0"
tn_ll_mmi_read 0x808100
echo "C_BASE_ADDR_HT1 Word1"
tn_ll_mmi_read 0x808104
echo "C_BASE_ADDR_HT1 Word2"
tn_ll_mmi_read 0x808108
echo "C_BASE_ADDR_HT1 Word3"
tn_ll_mmi_read 0x80810C
echo "C_BASE_ADDR_HT2 Word0"
tn_ll_mmi_read 0x808200
echo "C_BASE_ADDR_HT2 Word1"
tn_ll_mmi_read 0x808204
echo "C_BASE_ADDR_HT2 Word2"
tn_ll_mmi_read 0x808208
echo "C_BASE_ADDR_HT2 Word3"
tn_ll_mmi_read 0x80820C
echo "C_BASE_ADDR_HT3 Word0"
tn_ll_mmi_read 0x808300
echo "C_BASE_ADDR_HT3 Word1"
tn_ll_mmi_read 0x808304
echo "C_BASE_ADDR_HT3 Word2"
tn_ll_mmi_read 0x808308
echo "C_BASE_ADDR_HT3 Word3"
tn_ll_mmi_read 0x80830C
echo "C_BASE_ADDR_HT4 Word0"
tn_ll_mmi_read 0x808400
echo "C_BASE_ADDR_HT4 Word1"
tn_ll_mmi_read 0x808404
echo "C_BASE_ADDR_HT4 Word2"
tn_ll_mmi_read 0x808408
echo "C_BASE_ADDR_HT4 Word3"
tn_ll_mmi_read 0x80840C
echo "C_BASE_ADDR_HT5 Word0"
tn_ll_mmi_read 0x808500
echo "C_BASE_ADDR_HT5 Word1"
tn_ll_mmi_read 0x808504
echo "C_BASE_ADDR_HT5 Word2"
tn_ll_mmi_read 0x808508
echo "C_BASE_ADDR_HT5 Word3"
tn_ll_mmi_read 0x80850C
echo "C_BASE_ADDR_HT6 Word0"
tn_ll_mmi_read 0x808600
echo "C_BASE_ADDR_HT6 Word1"
tn_ll_mmi_read 0x808604
echo "C_BASE_ADDR_HT6 Word2"
tn_ll_mmi_read 0x808608
echo "C_BASE_ADDR_HT6 Word3"
tn_ll_mmi_read 0x80860C
echo "C_BASE_ADDR_HT7 Word0"
tn_ll_mmi_read 0x808700
echo "C_BASE_ADDR_HT7 Word1"
tn_ll_mmi_read 0x808704
echo "C_BASE_ADDR_HT7 Word2"
tn_ll_mmi_read 0x808708
echo "C_BASE_ADDR_HT7 Word3"
tn_ll_mmi_read 0x80870C
echo "C_BASE_ADDR_HT8 Word0"
tn_ll_mmi_read 0x808800
echo "C_BASE_ADDR_HT8 Word1"
tn_ll_mmi_read 0x808804
echo "C_BASE_ADDR_HT8 Word2"
tn_ll_mmi_read 0x808808
echo "C_BASE_ADDR_HT8 Word3"
tn_ll_mmi_read 0x80880C
echo "C_BASE_ADDR_HT9 Word0"
tn_ll_mmi_read 0x808900
echo "C_BASE_ADDR_HT9 Word1"
tn_ll_mmi_read 0x808904
echo "C_BASE_ADDR_HT9 Word2"
tn_ll_mmi_read 0x808908
echo "C_BASE_ADDR_HT9 Word3"
tn_ll_mmi_read 0x80890C
echo "C_BASE_ADDR_HT10 Word0"
tn_ll_mmi_read 0x808A00
echo "C_BASE_ADDR_HT10 Word1"
tn_ll_mmi_read 0x808A04
echo "C_BASE_ADDR_HT10 Word2"
tn_ll_mmi_read 0x808A08
echo "C_BASE_ADDR_HT10 Word3"
tn_ll_mmi_read 0x808A0C
echo "C_BASE_ADDR_HT11 Word0"
tn_ll_mmi_read 0x808B00
echo "C_BASE_ADDR_HT11 Word1"
tn_ll_mmi_read 0x808B04
echo "C_BASE_ADDR_HT11 Word2"
tn_ll_mmi_read 0x808B08
echo "C_BASE_ADDR_HT11 Word3"
tn_ll_mmi_read 0x808B0C
echo "C_BASE_ADDR_HT12 Word0"
tn_ll_mmi_read 0x808C00
echo "C_BASE_ADDR_HT12 Word1"
tn_ll_mmi_read 0x808C04
echo "C_BASE_ADDR_HT12 Word2"
tn_ll_mmi_read 0x808C08
echo "C_BASE_ADDR_HT12 Word3"
tn_ll_mmi_read 0x808C0C
echo "C_BASE_ADDR_HT13 Word0"
tn_ll_mmi_read 0x808D00
echo "C_BASE_ADDR_HT13 Word1"
tn_ll_mmi_read 0x808D04
echo "C_BASE_ADDR_HT13 Word2"
tn_ll_mmi_read 0x808D08
echo "C_BASE_ADDR_HT13 Word3"
tn_ll_mmi_read 0x808D0C
echo "C_BASE_ADDR_HT14 Word0"
tn_ll_mmi_read 0x808E00
echo "C_BASE_ADDR_HT14 Word1"
tn_ll_mmi_read 0x808E04
echo "C_BASE_ADDR_HT14 Word2"
tn_ll_mmi_read 0x808E08
echo "C_BASE_ADDR_HT14 Word3"
tn_ll_mmi_read 0x808E0C
echo "C_BASE_ADDR_HT15 Word0"
tn_ll_mmi_read 0x808F00
echo "C_BASE_ADDR_HT15 Word1"
tn_ll_mmi_read 0x808F04
echo "C_BASE_ADDR_HT15 Word2"
tn_ll_mmi_read 0x808F08
echo "C_BASE_ADDR_HT15 Word3"
tn_ll_mmi_read 0x808F0C

echo "C_SUB_ADDR_ETH_SW_COMMON DP0"
tn_ll_mmi_read 0x810000
echo "C_SUB_ADDR_ETH_SW_NUM_SOF DP0"
tn_ll_mmi_read 0x810010
echo "C_SUB_ADDR_ETH_SW_NUM_LEARNED DP0"
tn_ll_mmi_read 0x810014
echo "C_SUB_ADDR_ETH_SW_NUM_AGED_OUT DP0"
tn_ll_mmi_read 0x810018
echo "C_SUB_ADDR_ETH_SW_NUM_FLOOD_UNKNOWN DP0"
tn_ll_mmi_read 0x81001C
echo "C_SUB_ADDR_ETH_SW_NUM_FLOOD_FULL DP0"
tn_ll_mmi_read 0x810020
echo "C_SUB_ADDR_ETH_SW_NUM_PORT_ERROR DP0"
tn_ll_mmi_read 0x810024
echo "C_SUB_ADDR_ETH_SW_NUM_MATCH_SRC DP0"
tn_ll_mmi_read 0x810028
echo "C_SUB_ADDR_ETH_SW_NUM_MATCH_DST DP0"
tn_ll_mmi_read 0x81002C
echo "C_SUB_ADDR_ETH_SW_CMP_SRC_MATCH_L DP0"
tn_ll_mmi_read 0x810030
echo "C_SUB_ADDR_ETH_SW_CMP_SRC_MATCH_H DP0"
tn_ll_mmi_read 0x810034
echo "C_SUB_ADDR_ETH_SW_CMP_DST_MATCH_L DP0"
tn_ll_mmi_read 0x810038
echo "C_SUB_ADDR_ETH_SW_CMP_DST_MATCH_H DP0"
tn_ll_mmi_read 0x81003C
echo "C_SUB_ADDR_ETH_SW_TABLE_ADDR DP0"
tn_ll_mmi_read 0x810050
echo "C_SUB_ADDR_ETH_SW_TABLE_DATA_L DP0"
tn_ll_mmi_read 0x810054
echo "C_SUB_ADDR_ETH_SW_TABLE_DATA_H DP0"
tn_ll_mmi_read 0x810058
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_7_0 DP0"
tn_ll_mmi_read 0x810060
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_15_8 DP0"
tn_ll_mmi_read 0x810064
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_23_16 DP0"
tn_ll_mmi_read 0x810068
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_31_24 DP0"
tn_ll_mmi_read 0x81006C
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_39_32 DP0"
tn_ll_mmi_read 0x810070
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_47_40 DP0"
tn_ll_mmi_read 0x810074
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_55_48 DP0"
tn_ll_mmi_read 0x810078
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_63_56 DP0"
tn_ll_mmi_read 0x81007C
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_71_64 DP0"
tn_ll_mmi_read 0x810080
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_79_72 DP0"
tn_ll_mmi_read 0x810084
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_87_80 DP0"
tn_ll_mmi_read 0x810088
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_95_88 DP0"
tn_ll_mmi_read 0x81008C
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_103_96 DP0"
tn_ll_mmi_read 0x810090
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_111_104 DP0"
tn_ll_mmi_read 0x810094
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_119_112 DP0"
tn_ll_mmi_read 0x810098
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_127_120 DP0"
tn_ll_mmi_read 0x81009C
echo "C_SUB_ADDR_ETH_SW_COMMON DP1"
tn_ll_mmi_read 0x820000
echo "C_SUB_ADDR_ETH_SW_NUM_SOF DP1"
tn_ll_mmi_read 0x820010
echo "C_SUB_ADDR_ETH_SW_NUM_LEARNED DP1"
tn_ll_mmi_read 0x820014
echo "C_SUB_ADDR_ETH_SW_NUM_AGED_OUT DP1"
tn_ll_mmi_read 0x820018
echo "C_SUB_ADDR_ETH_SW_NUM_FLOOD_UNKNOWN DP1"
tn_ll_mmi_read 0x82001C
echo "C_SUB_ADDR_ETH_SW_NUM_FLOOD_FULL DP1"
tn_ll_mmi_read 0x820020
echo "C_SUB_ADDR_ETH_SW_NUM_PORT_ERROR DP1"
tn_ll_mmi_read 0x820024
echo "C_SUB_ADDR_ETH_SW_NUM_MATCH_SRC DP1"
tn_ll_mmi_read 0x820028
echo "C_SUB_ADDR_ETH_SW_NUM_MATCH_DST DP1"
tn_ll_mmi_read 0x82002C
echo "C_SUB_ADDR_ETH_SW_CMP_SRC_MATCH_L DP1"
tn_ll_mmi_read 0x820030
echo "C_SUB_ADDR_ETH_SW_CMP_SRC_MATCH_H DP1"
tn_ll_mmi_read 0x820034
echo "C_SUB_ADDR_ETH_SW_CMP_DST_MATCH_L DP1"
tn_ll_mmi_read 0x820038
echo "C_SUB_ADDR_ETH_SW_CMP_DST_MATCH_H DP1"
tn_ll_mmi_read 0x82003C
echo "C_SUB_ADDR_ETH_SW_TABLE_ADDR DP1"
tn_ll_mmi_read 0x820050
echo "C_SUB_ADDR_ETH_SW_TABLE_DATA_L DP1"
tn_ll_mmi_read 0x820054
echo "C_SUB_ADDR_ETH_SW_TABLE_DATA_H DP1"
tn_ll_mmi_read 0x820058
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_7_0 DP1"
tn_ll_mmi_read 0x820060
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_15_8 DP1"
tn_ll_mmi_read 0x820064
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_23_16 DP1"
tn_ll_mmi_read 0x820068
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_31_24 DP1"
tn_ll_mmi_read 0x82006C
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_39_32 DP1"
tn_ll_mmi_read 0x820070
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_47_40 DP1"
tn_ll_mmi_read 0x820074
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_55_48 DP1"
tn_ll_mmi_read 0x820078
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_63_56 DP1"
tn_ll_mmi_read 0x82007C
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_71_64 DP1"
tn_ll_mmi_read 0x820080
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_79_72 DP1"
tn_ll_mmi_read 0x820084
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_87_80 DP1"
tn_ll_mmi_read 0x820088
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_95_88 DP1"
tn_ll_mmi_read 0x82008C
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_103_96 DP1"
tn_ll_mmi_read 0x820090
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_111_104 DP1"
tn_ll_mmi_read 0x820094
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_119_112 DP1"
tn_ll_mmi_read 0x820098
echo "C_SUB_ADDR_ETH_SW_BUCKET_FILL_127_120 DP1"
tn_ll_mmi_read 0x82009C

echo "C_BASE_ADDR_STATISTIC_BADRSN_LOWER DP0 Word0"
tn_ll_mmi_read 0x830000
echo "C_BASE_ADDR_STATISTIC_BADRSN_LOWER DP0 Word1"
tn_ll_mmi_read 0x830004
echo "C_BASE_ADDR_STATISTIC_BADRSN_LOWER DP0 Word2"
tn_ll_mmi_read 0x830008
echo "C_BASE_ADDR_STATISTIC_BADRSN_LOWER DP0 Word3"
tn_ll_mmi_read 0x83000C
echo "C_BASE_ADDR_STATISTIC_BADRSN_LOWER DP1 Word0"
tn_ll_mmi_read 0x850000
echo "C_BASE_ADDR_STATISTIC_BADRSN_LOWER DP1 Word1"
tn_ll_mmi_read 0x850004
echo "C_BASE_ADDR_STATISTIC_BADRSN_LOWER DP1 Word2"
tn_ll_mmi_read 0x850008
echo "C_BASE_ADDR_STATISTIC_BADRSN_LOWER DP1 Word3"
tn_ll_mmi_read 0x85000C

echo "C_BASE_ADDR_STATISTIC_PKT_CNT_LOWER_0 DP0 Word 0"
tn_ll_mmi_read 0x831000
echo "C_BASE_ADDR_STATISTIC_PKT_CNT_LOWER_0 DP0 Word 1"
tn_ll_mmi_read 0x831004
echo "C_BASE_ADDR_STATISTIC_PKT_CNT_LOWER_0 DP0 Word 2"
tn_ll_mmi_read 0x831008
echo "C_BASE_ADDR_STATISTIC_PKT_CNT_LOWER_0 DP0 Word 3"
tn_ll_mmi_read 0x83100c
echo "C_BASE_ADDR_STATISTIC_PKT_CNT_LOWER_1 DP1 Word 0"
tn_ll_mmi_read 0x851000
echo "C_BASE_ADDR_STATISTIC_PKT_CNT_LOWER_1 DP1 Word 1"
tn_ll_mmi_read 0x851004
echo "C_BASE_ADDR_STATISTIC_PKT_CNT_LOWER_1 DP1 Word 2"
tn_ll_mmi_read 0x851008
echo "C_BASE_ADDR_STATISTIC_PKT_CNT_LOWER_1 DP1 Word 3"
tn_ll_mmi_read 0x85100c

echo "C_BASE_ADDR_STATISTIC_GOOD_FLOWID_LOWER_0 DP0 Word 0"
tn_ll_mmi_read 0x832000
echo "C_BASE_ADDR_STATISTIC_GOOD_FLOWID_LOWER_0 DP0 Word 1"
tn_ll_mmi_read 0x832004
echo "C_BASE_ADDR_STATISTIC_GOOD_FLOWID_LOWER_0 DP0 Word 2"
tn_ll_mmi_read 0x832008
echo "C_BASE_ADDR_STATISTIC_GOOD_FLOWID_LOWER_0 DP0 Word 3"
tn_ll_mmi_read 0x83200c
echo "C_BASE_ADDR_STATISTIC_GOOD_FLOWID_LOWER_1 DP1 Word 0"
tn_ll_mmi_read 0x852000
echo "C_BASE_ADDR_STATISTIC_GOOD_FLOWID_LOWER_1 DP1 Word 1"
tn_ll_mmi_read 0x852004
echo "C_BASE_ADDR_STATISTIC_GOOD_FLOWID_LOWER_1 DP1 Word 2"
tn_ll_mmi_read 0x852008
echo "C_BASE_ADDR_STATISTIC_GOOD_FLOWID_LOWER_1 DP1 Word 3"
tn_ll_mmi_read 0x85200c

echo "C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_0 DP0 Word 0"
tn_ll_mmi_read 0x833000
echo "C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_0 DP0 Word 1"
tn_ll_mmi_read 0x833004
echo "C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_0 DP0 Word 2"
tn_ll_mmi_read 0x833008
echo "C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_0 DP0 Word 3"
tn_ll_mmi_read 0x83300c
echo "C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_1 DP1 Word 0"
tn_ll_mmi_read 0x853000
echo "C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_1 DP1 Word 1"
tn_ll_mmi_read 0x853004
echo "C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_1 DP1 Word 2"
tn_ll_mmi_read 0x853008
echo "C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_1 DP1 Word 3"
tn_ll_mmi_read 0x85300c

echo "C_SUB_ADDR_USER_MOD_MMI_ID"
tn_ll_mmi_read 0x880000
echo "C_SUB_ADDR_USER_MOD_MMI_BEEP_EN"
tn_ll_mmi_read 0x880004
echo "C_SUB_ADDR_USER_MOD_MMI_DISPLAY_EN"
tn_ll_mmi_read 0x880008
echo "C_SUB_ADDR_USER_MOD_MMI_PKT_CNT0"
tn_ll_mmi_read 0x88000C
echo "C_SUB_ADDR_USER_MOD_MMI_PKT_CNT_WIDE0"
tn_ll_mmi_read 0x880010
echo "C_SUB_ADDR_USER_MOD_MMI_PKT_CNT1"
tn_ll_mmi_read 0x880014
echo "C_SUB_ADDR_USER_MOD_MMI_PKT_CNT_WIDE1"
tn_ll_mmi_read 0x880018

echo "C_BASE_ADDR_NOC_ACTION_LOWER Word0"
tn_ll_mmi_read 0x890000
echo "C_BASE_ADDR_NOC_ACTION_LOWER Word1"
tn_ll_mmi_read 0x890004
echo "C_BASE_ADDR_NOC_ACTION_LOWER Word2"
tn_ll_mmi_read 0x890008
echo "C_BASE_ADDR_NOC_ACTION_LOWER Word3"
tn_ll_mmi_read 0x89000C

echo "C_BASE_ADDR_FLOW_CACHE_HASH_LOWER Word0"
tn_ll_mmi_read 0x900000
echo "C_BASE_ADDR_FLOW_CACHE_HASH_LOWER Word1"
tn_ll_mmi_read 0x900004
echo "C_BASE_ADDR_FLOW_CACHE_HASH_LOWER Word2"
tn_ll_mmi_read 0x900008
echo "C_BASE_ADDR_FLOW_CACHE_HASH_LOWER Word3"
tn_ll_mmi_read 0x90000C
echo "C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER Word0"
tn_ll_mmi_read 0xB00000
echo "C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER Word1"
tn_ll_mmi_read 0xB00004
echo "C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER Word2"
tn_ll_mmi_read 0xB00008
echo "C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER Word3"
tn_ll_mmi_read 0xB0000C
echo "C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER Word4"
tn_ll_mmi_read 0xB00010
echo "C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER Word5"
tn_ll_mmi_read 0xB00014
echo "C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER Word6"
tn_ll_mmi_read 0xB00018
echo "C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER Word7"
tn_ll_mmi_read 0xB0001C
echo "C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER Word8"
tn_ll_mmi_read 0xB00020
echo "C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER Word0"
tn_ll_mmi_read 0xC00000
echo "C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER Word1"
tn_ll_mmi_read 0xC00004
echo "C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER Word2"
tn_ll_mmi_read 0xC00008
echo "C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER Word3"
tn_ll_mmi_read 0xC0000C
echo "C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER Word4"
tn_ll_mmi_read 0xC00010
echo "C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER Word5"
tn_ll_mmi_read 0xC00014
echo "C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER Word6"
tn_ll_mmi_read 0xC00018
echo "C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER Word7"
tn_ll_mmi_read 0xC0001C
echo "C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER Word8"
tn_ll_mmi_read 0xC00020
echo "C_BASE_ADDR_FLOW_CACHE_EMA_LOWER Word0"
tn_ll_mmi_read 0xC10000
echo "C_BASE_ADDR_FLOW_CACHE_EMA_LOWER Word1"
tn_ll_mmi_read 0xC10004
echo "C_BASE_ADDR_FLOW_CACHE_EMA_LOWER Word2"
tn_ll_mmi_read 0xC10008
echo "C_BASE_ADDR_FLOW_CACHE_EMA_LOWER Word3"
tn_ll_mmi_read 0xC1000C
echo "C_BASE_ADDR_FLOW_CACHE_EMA_LOWER Word4"
tn_ll_mmi_read 0xC10010
echo "C_BASE_ADDR_FLOW_CACHE_EMA_LOWER Word5"
tn_ll_mmi_read 0xC10014
echo "C_BASE_ADDR_FLOW_CACHE_EMA_LOWER Word6"
tn_ll_mmi_read 0xC10018
echo "C_BASE_ADDR_FLOW_CACHE_EMA_LOWER Word7"
tn_ll_mmi_read 0xC1001C
echo "C_BASE_ADDR_FLOW_CACHE_EMA_LOWER Word8"
tn_ll_mmi_read 0xC10020
echo "C_BASE_ADDR_FLOW_CACHE_EMA_CAM Word0"
tn_ll_mmi_read 0xC20000
echo "C_BASE_ADDR_FLOW_CACHE_EMA_CAM Word1"
tn_ll_mmi_read 0xC20004
echo "C_BASE_ADDR_FLOW_CACHE_EMA_CAM Word2"
tn_ll_mmi_read 0xC20008
echo "C_BASE_ADDR_FLOW_CACHE_EMA_CAM Word3"
tn_ll_mmi_read 0xC2000C
echo "C_BASE_ADDR_FLOW_CACHE"
tn_ll_mmi_read 0xC30000

echo "C_SUB_ADDR_TM_QUEUE_THRES_FULL"
tn_ll_mmi_read 0xD00000

echo "C_TM_SCHED_TAS_GATE_ENABLE: TAS disable"
tn_ll_mmi_write 0xD20418 0x00000000
tn_ll_mmi_write 0xD20C18 0x00000000
tn_ll_mmi_write 0xD21418 0x00000000
tn_ll_mmi_write 0xD21C18 0x00000000
tn_ll_mmi_write 0xD22418 0x00000000
tn_ll_mmi_write 0xD22C18 0x00000000
tn_ll_mmi_write 0xD23418 0x00000000
tn_ll_mmi_write 0xD23C18 0x00000000
tn_ll_mmi_write 0xD24418 0x00000000
tn_ll_mmi_write 0xD24C18 0x00000000
tn_ll_mmi_write 0xD25418 0x00000000
tn_ll_mmi_write 0xD25C18 0x00000000
tn_ll_mmi_write 0xD26418 0x00000000
tn_ll_mmi_write 0xD26C18 0x00000000
tn_ll_mmi_write 0xD27418 0x00000000
tn_ll_mmi_write 0xD27C18 0x00000000

echo "C_TM_SCHED_TAS_ADMIN_GATE_STATES: TAS default output xFF"
tn_ll_mmi_write 0xD20438 0x000000FF
tn_ll_mmi_write 0xD20C38 0x000000FF
tn_ll_mmi_write 0xD21438 0x000000FF
tn_ll_mmi_write 0xD21C38 0x000000FF
tn_ll_mmi_write 0xD22438 0x000000FF
tn_ll_mmi_write 0xD22C38 0x000000FF
tn_ll_mmi_write 0xD23438 0x000000FF
tn_ll_mmi_write 0xD23C38 0x000000FF
tn_ll_mmi_write 0xD24438 0x000000FF
tn_ll_mmi_write 0xD24C38 0x000000FF
tn_ll_mmi_write 0xD25438 0x000000FF
tn_ll_mmi_write 0xD25C38 0x000000FF
tn_ll_mmi_write 0xD26438 0x000000FF
tn_ll_mmi_write 0xD26C38 0x000000FF
tn_ll_mmi_write 0xD27438 0x000000FF
tn_ll_mmi_write 0xD27C38 0x000000FF

echo "C_TM_SCHED_TAS_CUR_TIME"
tn_ll_mmi_read 0xD20544

echo "TAS output port 0 for 4 entries"
echo "C_TM_SCHED_GCL_LOWER: the gcl entries"
tn_ll_mmi_write 0xD20000 0x00000055
tn_ll_mmi_write 0xD20004 0x000000AA
tn_ll_mmi_write 0xD20008 0x00000055
tn_ll_mmi_write 0xD2000C 0x000000AA
tn_ll_mmi_write 0xD20010 0x00000055
echo "C_TM_SCHED_GCL_TIME_LOWER"
tn_ll_mmi_write 0xD20200 0x00005000
tn_ll_mmi_write 0xD20204 0x00005010
tn_ll_mmi_write 0xD20208 0x00005020
tn_ll_mmi_write 0xD2020C 0x00005030
tn_ll_mmi_write 0xD20210 0x00005040
echo "C_TM_SCHED_TAS_CONFIG_REG_LOWER"
tn_ll_mmi_write 0xD20400 0x00000004
tn_ll_mmi_write 0xD20404 0x00005000
tn_ll_mmi_write 0xD20408 0x00014000
tn_ll_mmi_write 0xD2040C 0x00000000
tn_ll_mmi_write 0xD20410 0x00005000
tn_ll_mmi_write 0xD20414 0x00005000
tn_ll_mmi_write 0xD20418 0x00000001
tn_ll_mmi_write 0xD2041C 0x00000001
tn_ll_mmi_write 0xD2041C 0x00000000

echo "TAS output port 1 for 4 entries"
echo "C_TM_SCHED_GCL_LOWER: the gcl entries"
tn_ll_mmi_write 0xD20800 0x00000055
tn_ll_mmi_write 0xD20804 0x000000AA
tn_ll_mmi_write 0xD20808 0x00000055
tn_ll_mmi_write 0xD2080C 0x000000AA
tn_ll_mmi_write 0xD20810 0x00000055
echo "C_TM_SCHED_GCL_TIME_LOWER"
tn_ll_mmi_write 0xD20A00 0x00005000
tn_ll_mmi_write 0xD20A04 0x00005010
tn_ll_mmi_write 0xD20A08 0x00005020
tn_ll_mmi_write 0xD20A0C 0x00005030
tn_ll_mmi_write 0xD20A10 0x00005040
echo "C_TM_SCHED_TAS_CONFIG_REG_LOWER"
tn_ll_mmi_write 0xD20C00 0x00000004
tn_ll_mmi_write 0xD20C04 0x00005000
tn_ll_mmi_write 0xD20C08 0x00014000
tn_ll_mmi_write 0xD20C0C 0x00000000
tn_ll_mmi_write 0xD20C10 0x00005000
tn_ll_mmi_write 0xD20C14 0x00005000
tn_ll_mmi_write 0xD20C18 0x00000001
tn_ll_mmi_write 0xD20C1C 0x00000001
tn_ll_mmi_write 0xD20C1C 0x00000000

echo "C_TM_SCHED_TAS_ADMIN_GCL_LEN: read TAS 0"
tn_ll_mmi_read 0xD20400
echo "C_TM_SCHED_TAS_ADMIN_BASE_TIME"
tn_ll_mmi_read 0xD20404
echo "C_TM_SCHED_TAS_ADMIN_CYCLE_TIME"
tn_ll_mmi_read 0xD20408
echo "C_TM_SCHED_TAS_ADMIN_CYCLE_TIME_EXT"
tn_ll_mmi_read 0xD2040C
echo "C_TM_SCHED_TAS_CONFIG_CHANGE_TIME"
tn_ll_mmi_read 0xD20410
echo "C_TM_SCHED_TAS_CYCLE_START_TIME"
tn_ll_mmi_read 0xD20414
echo "C_TM_SCHED_TAS_GATE_ENABLE"
tn_ll_mmi_read 0xD20418
echo "C_TM_SCHED_TAS_CONFIG_CHANGE"
tn_ll_mmi_read 0xD2041C
echo "C_TM_SCHED_TAS_CONFIG_CHANGE_PENDING"
tn_ll_mmi_read 0xD20420
echo "C_TM_SCHED_TAS_CONFIG_CHANGE_ACK"
tn_ll_mmi_read 0xD20424
echo "C_TM_SCHED_TAS_OPER_GCL_LEN"
tn_ll_mmi_read 0xD20428
echo "C_TM_SCHED_TAS_OPER_BASE_TIME"
tn_ll_mmi_read 0xD2042C
echo "C_TM_SCHED_TAS_OPER_CYCLE_TIME"
tn_ll_mmi_read 0xD20430
echo "C_TM_SCHED_TAS_OPER_CYCLE_TIME_EXT"
tn_ll_mmi_read 0xD20434
echo "C_TM_SCHED_TAS_ADMIN_GATE_STATES"
tn_ll_mmi_read 0xD20438
echo "C_TM_SCHED_TAS_OPER_GATE_STATES"
tn_ll_mmi_read 0xD2043C

echo "C_TM_SCHED_PROC_QUEUE_PRIO_LOWER"
tn_ll_mmi_write 0xD20500 0x00000000
echo "C_TM_SCHED_PROC_QUEUE_PRIO_HIGH"
tn_ll_mmi_write 0xD20540 0x00000000

echo "C_BASE_ADDR_TX_ROUTER_LOWER0 Word0"
tn_ll_mmi_read 0xE10000
echo "C_BASE_ADDR_TX_ROUTER_LOWER0 Word1"
tn_ll_mmi_read 0xE10004
echo "C_BASE_ADDR_TX_ROUTER_LOWER0 Word2"
tn_ll_mmi_read 0xE10008
echo "C_BASE_ADDR_TX_ROUTER_LOWER0 Word3"
tn_ll_mmi_read 0xE1000C
echo "C_BASE_ADDR_TX_ROUTER_LOWER1 Word0"
tn_ll_mmi_read 0xE20000
echo "C_BASE_ADDR_TX_ROUTER_LOWER1 Word1"
tn_ll_mmi_read 0xE20004
echo "C_BASE_ADDR_TX_ROUTER_LOWER1 Word2"
tn_ll_mmi_read 0xE20008
echo "C_BASE_ADDR_TX_ROUTER_LOWER1 Word3"
tn_ll_mmi_read 0xE2000C

echo "Done"
