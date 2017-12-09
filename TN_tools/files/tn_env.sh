#!/bin/bash
export TN_NO_OF_PHYS=12   # Maximum number for (C_BASE_ADDR_TN_COMMON*$C_BASE_ADDR_FACTOR+C_SUB_ADDR_COMMON_PARAM_PRT_CNT)
export TN_NO_OF_GPHYS=10  # Max
export TN_NO_OF_ALASKAS=2 # Max

export C_BASE_ADDR_FACTOR=256 # BASE ADDRESS factors (to shift the base address left)

#### MDIO access masks
export TN_RGMII_WRITE=0x4000000 # Mask for the R/W bit in MDIO frames
export TN_RGMII_PHY=0x200000    # PHY address multiplier in MDIO frames
export TN_RGMII_REG=0x10000     # Register address multiplier in MDIO frames
# PHY ADDRESSES: 0+1=Alaskas, 16-25=GPHYs

#### Address Space
# This shell script is automatically parsed to extract constants for
# EmbeddedLinux/BuildRootPKGs/InnoRoute/TN_tools/files/tn_env.h in the
# software repository.
# Everything below is based on and has to be consistent with
# the VHDL package /mmi_lib/mmi_pckg.vhd in the FPGA code repository.

export C_MMI_ADDR_MAP_REVISION=3 # Address map version: to be checked against value read from C_BASE_ADDR_TN_COMMON -> C_SUB_ADDR_COMMON_ADDR_MAP_REV

##--------------------------------------------------------------------------
##--------------------------------------------------------------------------
## BASE ADDRESSES SECTION --------------------------------------------------
##--------------------------------------------------------------------------
## Declaration of all BASE ADDR and SUB ADDR used within TN ----------------
##--------------------------------------------------------------------------
## Units within same clock domain should have same level of base address  --
## * MMI Clk: TN_COMMON, MDIO ----------------------------------------------
## * Sys Clk: PERIPHERAL ---------------------------------------------------
## * ETH TX Clk: MACs -> ETH RX Clk Sync (local per port) ------------------
## * AXI Clk: floorplan-aware -> Special Clk Sync (TCAM, ACCDP) ------------
##--------------------------------------------------------------------------
## each sub address is relative to a base address --------------------------
##--------------------------------------------------------------------------
## BASE ADDR may increment by 1, SUB ADDR by 4 (32 bits) -------------------
##--------------------------------------------------------------------------

  ## COMMON module, providing basic version information etc.
  export C_BASE_ADDR_TN_COMMON=0x0000 #;
  # Split to _L and _H in mmi_pckg

  export C_SUB_ADDR_COMMON_TN_MAJOR_REV=0x000 #;
  export C_SUB_ADDR_COMMON_TN_MINOR_REV=0x004 #;
  export C_SUB_ADDR_COMMON_USER_REV=0x008 #;
  export C_SUB_ADDR_COMMON_ADDR_MAP_REV=0x00C #;

  export C_SUB_ADDR_COMMON_FPGA_ID0=0x010 #;
  export C_SUB_ADDR_COMMON_FPGA_ID1=0x014 #;
  export C_SUB_ADDR_COMMON_EFUSE=0x018 #;
  export C_SUB_ADDR_COMMON_BITGEN_TIME=0x1C #;


  export C_SUB_ADDR_COMMON_FEATURES_6TREE=0x100 #;
  export C_SUB_ADDR_COMMON_FEATURES_ETH_SW=0x104 #;
  export C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE=0x108 #;
  export C_SUB_ADDR_COMMON_FEATURES_ACC_DP=0x10C #;

  export C_SUB_ADDR_COMMON_FEATURES_TX_ROUTER=0x110 #;
  export C_SUB_ADDR_COMMON_FEATURES_SFP=0x114 #;
  export C_SUB_ADDR_COMMON_FEATURES_SODIMM=0x118 #;

  export C_SUB_ADDR_COMMON_PARAM_PRT_CNT=0x120 #;
  export C_SUB_ADDR_COMMON_PARAM_DP_CNT=0x124 #;
  export C_SUB_ADDR_COMMON_PARAM_BUF_CNT=0x128 #;

  export C_SUB_ADDR_COMMON_PARAM_RX_MIN=0x130 #;
  export C_SUB_ADDR_COMMON_PARAM_RX_MAX=0x134 #;
  export C_SUB_ADDR_COMMON_PARAM_TX_PRE=0x138 #;
  export C_SUB_ADDR_COMMON_PARAM_TX_IFG=0x13C #;

  export C_SUB_ADDR_COMMON_PARAM_DISPLAY=0x140 #;
  export C_SUB_ADDR_COMMON_PARAM_LED_SYNCE=0x144 #;
  export C_SUB_ADDR_COMMON_PARAM_LED_INTERNAL=0x148 #;
  export C_SUB_ADDR_COMMON_PARAM_BEEP=0x14C #;

  export C_SUB_ADDR_COMMON_PARAM_PHY_RESET=0x150 #;
  export C_SUB_ADDR_COMMON_PARAM_PINHEADER=0x154 #;
  export C_SUB_ADDR_COMMON_PARAM_PMOD=0x158 #;
  export C_SUB_ADDR_COMMON_PARAM_EXT_BYPASS=0x15C #;

  export C_SUB_ADDR_COMMON_WIDTH_HC_BAD=0x160 #;
  export C_SUB_ADDR_COMMON_WIDTH_ETH_SW=0x164 #;
  export C_SUB_ADDR_COMMON_WIDTH_RX_STAT_BAD=0x168 #;

  export C_SUB_ADDR_COMMON_FPGA_TEMP=0x200 #;
  export C_SUB_ADDR_COMMON_FPGA_ALARM=0x204 #;
  export C_SUB_ADDR_COMMON_CONFIG_CHECK=0x208 #;

  export C_SUB_ADDR_COMMON_FIFO_OVERFLOW0=0x210 #;
  export C_SUB_ADDR_COMMON_FIFO_OVERFLOW1=0x214 #;
  export C_SUB_ADDR_COMMON_FIFO_OVERFLOW2=0x218 #;
  export C_SUB_ADDR_COMMON_FIFO_OVERFLOW3=0x21C #;

  export C_SUB_ADDR_COMMON_FIFO_UNDERRUN0=0x220 #;
  export C_SUB_ADDR_COMMON_FIFO_UNDERRUN1=0x224 #;
  export C_SUB_ADDR_COMMON_FIFO_UNDERRUN2=0x228 #;
  export C_SUB_ADDR_COMMON_FIFO_UNDERRUN3=0x22C #;

  export C_SUB_ADDR_COMMON_PCIE_ERROR0=0x230 #;
  export C_SUB_ADDR_COMMON_PCIE_ERROR1=0x234 #;
  export C_SUB_ADDR_COMMON_PCIE_TIMEOUT_CNT=0x238 #;

  export C_SUB_ADDR_COMMON_BUFFER_FULL=0x240 #;
  export C_SUB_ADDR_COMMON_TXF_FULL=0x244 #;

  ## MDIO module, providing access to the PHY chips registers
  export C_BASE_ADDR_MDIO=0x0001 #; -- mmi_mdio

  export C_SUB_ADDR_MDIO_WRITE=0x00 #;      -- Command & Write Data
  export C_SUB_ADDR_MDIO_READ=0x04 #;       -- Status & Read Data
  export C_SUB_ADDR_MDIO_INTERRUPTS=0x08 #; -- RO Address for PHY interrupts

  ## Peripherals Module, providing direct access to TrustNode peripherals
  export C_BASE_ADDR_PERIPH=0x0002 #; -- mmi_peripherals

  export C_SUB_ADDR_PERIPH_BUTTONS=0x00 #;    -- RO Address for front and back buttons
  export C_SUB_ADDR_PERIPH_LEDS=0x04 #;       -- RW Address for the LEDs
  export C_SUB_ADDR_PERIPH_GPIOS=0x08 #;      -- RW Address for the GPIO headers
  export C_SUB_ADDR_PERIPH_DISPLAY0=0x0C #;   -- RW Address for the front panel display
  export C_SUB_ADDR_PERIPH_DISPLAY1=0x10 #;   -- RW Address for the front panel display
  export C_SUB_ADDR_PERIPH_DISPLAY2=0x14 #;   -- RW Address for the front panel display
  export C_SUB_ADDR_PERIPH_DISPLAY3=0x18 #;   -- RW Address for the front panel display
  export C_SUB_ADDR_PERIPH_BEEP=0x1C #;       -- RW Address for the beeper
  export C_SUB_ADDR_PERIPH_BRIGHTNESS=0x20 #; -- RW Address for the display brightness

  ## Acceleration Datapath, providing ... --** TBD
  export C_BASE_ADDR_ACC=0x1000 #,
                        #0x2800#);

  ## RGMII module, providing status information from the RGMII ports and enabling RX tap delay adjustment
  export C_BASE_ADDR_RGMII=0x4000 #,  -- mmi_eth_mac 0
                          #0x4400 #,  -- mmi_eth_mac 1
                          #0x4800 #,  -- mmi_eth_mac 2
                          #0x4C00 #,  -- mmi_eth_mac 3
                          #0x5000 #,  -- mmi_eth_mac 4
                          #0x5400 #,  -- mmi_eth_mac 5
                          #0x5800 #,  -- mmi_eth_mac 6
                          #0x5C00 #,  -- mmi_eth_mac 7
                          #0x6000 #,  -- mmi_eth_mac 8
                          #0x6400 #,  -- mmi_eth_mac 9
                          #0x6800 #,  -- mmi_eth_mac 10
                          #0x6C00#); -- mmi_eth_mac 11

  export C_SUB_ADDR_RGMII_TAP=0x00 #;       -- RW
  export C_SUB_ADDR_RGMII_LINK_STAT=0x04 #; -- RO
  export C_SUB_ADDR_RGMII_CLK_SPEED=0x08 #; -- RO
  export C_SUB_ADDR_RGMII_DPX_STAT=0x0C #;  -- RO

  ## Ethernet MAC modules, providing
  export C_BASE_ADDR_MAC=0x4001 #,  -- mmi_eth_mac 0
                        #0x4401 #,  -- mmi_eth_mac 1
                        #0x4801 #,  -- mmi_eth_mac 2
                        #0x4C01 #,  -- mmi_eth_mac 3
                        #0x5001 #,  -- mmi_eth_mac 4
                        #0x5401 #,  -- mmi_eth_mac 5
                        #0x5801 #,  -- mmi_eth_mac 6
                        #0x5C01 #,  -- mmi_eth_mac 7
                        #0x6001 #,  -- mmi_eth_mac 8
                        #0x6401 #,  -- mmi_eth_mac 9
                        #0x6801 #,  -- mmi_eth_mac 10
                        #0x6C01#); -- mmi_eth_mac 11

  export C_SUB_ADDR_MAC_OWN_MAC_ADDR_L=0x00 #; -- lower 32Bit of our own MAC Address
  export C_SUB_ADDR_MAC_OWN_MAC_ADDR_H=0x04 #; -- upper Bits of our own MAC Address
  export C_SUB_ADDR_MAC_6T_ADDR_L=0x10 #;      -- lower 32Bit of 6T Addr
  export C_SUB_ADDR_MAC_6T_ADDR_H=0x14 #;      -- upper Bits of 6T Addr
  export C_SUB_ADDR_MAC_6T_SRC_PTR=0x18 #;     -- Index in 6T Addr
  export C_SUB_ADDR_MAC_6T_DST_PTR=0x1C #;     -- Index in 6T addr

  ## Header Creation modules, providing fragment drop counts
  export C_BASE_ADDR_HC=0x8000 #,   -- header_creation_mmi 0
                        #0x8001 #,  -- header_creation_mmi 1
                        #0x8002 #,  -- header_creation_mmi 2
                        #0x8003 #,  -- header_creation_mmi 3
                        #0x8004 #,  -- header_creation_mmi 4
                        #0x8005 #,  -- header_creation_mmi 5
                        #0x8006 #,  -- header_creation_mmi 6
                        #0x8007 #,  -- header_creation_mmi 7
                        #0x8008 #,  -- header_creation_mmi 8
                        #0x8009 #,  -- header_creation_mmi 9
                        #0x800A #,  -- header_creation_mmi 10
                        #0x800B#); -- header_creation_mmi 11
  export C_BASE_ADDR_HC_PCIE=0x800E #,  -- header_creation_mmi pcie 0
                            #0x800F#); -- header_creation_mmi pcie 1

  export C_SUB_ADDR_HC_CNT_BAD_FRAMES=0x00 #;

  ## Ethernet Switches, providing ... --** TBD
  export C_BASE_ADDR_ETH_SWITCH=0x8100 #,  -- mmi_switch_process 0
                               #0x8200#); -- mmi_switch_process 1

  ##Declaration of all Sub Addresses within ETHERNET SWITCH
  export C_SUB_ADDR_ETH_SW_COMMON=0x00 #;
  export C_SUB_ADDR_ETH_SW_NUM_SOF=0x10 #;
  export C_SUB_ADDR_ETH_SW_NUM_LEARNED=0x14 #;
  export C_SUB_ADDR_ETH_SW_NUM_AGED_OUT=0x18 #;
  export C_SUB_ADDR_ETH_SW_NUM_FLOOD_UNKNOWN=0x1C #;
  export C_SUB_ADDR_ETH_SW_NUM_FLOOD_FULL=0x20 #;
  export C_SUB_ADDR_ETH_SW_NUM_PORT_ERROR=0x24 #;
  export C_SUB_ADDR_ETH_SW_NUM_MATCH_SRC=0x28 #;
  export C_SUB_ADDR_ETH_SW_NUM_MATCH_DST=0x2C #;
  export C_SUB_ADDR_ETH_SW_CMP_SRC_MATCH_L=0x30 #;
  export C_SUB_ADDR_ETH_SW_CMP_SRC_MATCH_H=0x34 #;
  export C_SUB_ADDR_ETH_SW_CMP_DST_MATCH_L=0x38 #;
  export C_SUB_ADDR_ETH_SW_CMP_DST_MATCH_H=0x3C #;
  export C_SUB_ADDR_ETH_SW_TABLE_ADDR=0x50 #;
  export C_SUB_ADDR_ETH_SW_TABLE_DATA_L=0x54 #;
  export C_SUB_ADDR_ETH_SW_TABLE_DATA_H=0x58 #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_7_0=0x60 #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_15_8=0x64 #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_23_16=0x68 #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_31_24=0x6C #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_39_32=0x70 #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_47_40=0x74 #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_55_48=0x78 #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_63_56=0x7C #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_71_64=0x80 #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_79_72=0x84 #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_87_80=0x88 #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_95_88=0x8C #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_103_96=0x90 #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_111_104=0x94 #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_119_112=0x98 #;
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_127_120=0x9C #;

  ## RX Statistics Module, providing drop counter values
  export C_BASE_ADDR_STATISTIC_LOW=0x8300 #,
                                  #0x8500#);
  export C_BASE_ADDR_STATISTIC_HIGH=0x84FF #,
                                   #0x86FF#);

  ## User Module, providing peripherals access control (beeping, dots) and RX processing datapath counters
  export C_BASE_ADDR_USER_MOD=0x8800 #; -- user_module_mmi

  export C_SUB_ADDR_USER_MOD_MMI_ID=0x00 #;            -- RW Address for "unique" pattern
  export C_SUB_ADDR_USER_MOD_MMI_BEEP_EN=0x04 #;       -- RW Address to enable beeping
  export C_SUB_ADDR_USER_MOD_MMI_DISPLAY_EN=0x08 #;    -- RW Address to enable display dots
  export C_SUB_ADDR_USER_MOD_MMI_PKT_CNT0=0x0C #;      -- RO,COR Address for RX DP0 packet counter
  export C_SUB_ADDR_USER_MOD_MMI_PKT_CNT_WIDE0=0x10 #; -- RO Address for wide RX DP0 packet counter
  export C_SUB_ADDR_USER_MOD_MMI_PKT_CNT1=0x14 #;      -- RO,COR Address for RX DP1 packet counter
  export C_SUB_ADDR_USER_MOD_MMI_PKT_CNT_WIDE1=0x18 #; -- RO Address for wide RX DP1 packet counter

  ## Action unit, providing flow action settings
  export C_BASE_ADDR_NOC_ACTION_LOWER=0x8900 #;
  export C_BASE_ADDR_NOC_ACTION_UPPER=0x89FF #;

  ## Flow Cache
  export C_BASE_ADDR_FLOW_CACHE_HASH_LOWER=0x9000 #; --flow cache hash table is defined as range  0x10000 to 0x1fffc
  export C_BASE_ADDR_FLOW_CACHE_HASH_UPPER=0xAFFF #; --flow cache hash table is defined as range  0x10000 to 0x1fffc

  export C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER=0xB000 #; --flow cache rules table is defined as range  0x30000 to 0x37ffc
  export C_BASE_ADDR_FLOW_CACHE_FIELD_UPPER=0xBFFF #; --flow cache rules table is defined as range  0x30000 to 0x37ffc

  export C_BASE_ADDR_FLOW_CACHE_EMA_CAM=0xC000 #;

  export C_BASE_ADDR_FLOW_CACHE_EMA_LOWER=0xC100 #;
  export C_BASE_ADDR_FLOW_CACHE_EMA_UPPER=0xC1FF #;

  export C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER=0xC200 #; --flow cache linear search is defined as range  0x50000 to 0x501fc
  export C_BASE_ADDR_FLOW_CACHE_LINEAR_UPPER=0xC2FF #; --flow cache linear search is defined as range  0x50000 to 0x501fc

  ## Traffic Manager, providing queue threshold settings
  export C_BASE_ADDR_TM=0xD000 #; -- mmi_tm

  export C_SUB_ADDR_TM_QUEUE_THRES_FULL=0x00 #; -- Queue thresholds
#automatic generated from mmi_pckg.vhd
  export C_MMI_ADDR_MAP_REVISION=1 # Address map version
  export C_MMI_DATA_WIDTH=32
  export C_MMI_ADDR_WIDTH=20  #defines the width of the address bus
  export C_MMI_BASE_ADDR_WIDTH=12  #defines the width of the base address part
  export C_MMI_BASE_UPPER=19  #defines the MSB of the base address part
  export C_MMI_BASE_LOWER=8  #defines the LSB of the base address part
  export C_MMI_SUB_ADDR_WIDTH=8  #defines the width of the sub address part
  export C_MMI_SUB_UPPER=7  #defines the MSB of the sub address part
  export C_MMI_SUB_LOWER=0  #defines the LSB of the sub address part
  export C_MMI_PAGE_WIDTH=8 #defines the width of PAGE address part
  export C_BASE_ADDR_TN_COMMON=0x000
  export C_SUB_ADDR_COMMON_TN_MAJOR_REV=0x00
  export C_SUB_ADDR_COMMON_TN_MINOR_REV=0x04
  export C_SUB_ADDR_COMMON_USER_REV=0x08
  export C_SUB_ADDR_COMMON_ADDR_MAP_REV=0x0C
  export C_SUB_ADDR_COMMON_FPGA_ID0=0x10
  export C_SUB_ADDR_COMMON_FPGA_ID1=0x14
  export C_SUB_ADDR_COMMON_EFUSE=0x18
  export C_SUB_ADDR_COMMON_BITGEN_TIME=0x20
  export C_SUB_ADDR_COMMON_CONFIG_CHECK=0x24
  export C_SUB_ADDR_COMMON_FPGA_TEMP=0x28
  export C_SUB_ADDR_COMMON_FPGA_ALARM=0x2C
  export C_SUB_ADDR_COMMON_FEATURES_6TREE=0x30
  export C_SUB_ADDR_COMMON_FEATURES_ETH_SW=0x34
  export C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE=0x38
  export C_SUB_ADDR_COMMON_FEATURES_ACC_DP=0x3C
  export C_SUB_ADDR_COMMON_FEATURES_SFP=0x40
  export C_SUB_ADDR_COMMON_FEATURES_SODIMM=0x44
  export C_SUB_ADDR_COMMON_PARAM_PRT_CNT=0x50
  export C_SUB_ADDR_COMMON_PARAM_DP_CNT=0x54
  export C_SUB_ADDR_COMMON_PARAM_BUF_CNT=0x58
  export C_SUB_ADDR_COMMON_PARAM_RX_MIN=0x60
  export C_SUB_ADDR_COMMON_PARAM_RX_MAX=0x64
  export C_SUB_ADDR_COMMON_PARAM_TX_PRE=0x68
  export C_SUB_ADDR_COMMON_PARAM_TX_IFG=0x6C
  export C_SUB_ADDR_COMMON_WIDTH_HC_BAD=0x70
  export C_SUB_ADDR_COMMON_WIDTH_ETH_SW=0x74
  export C_SUB_ADDR_COMMON_WIDTH_RX_STAT_BAD=0x78
  export C_SUB_ADDR_COMMON_FIFO_OVERFLOW0=0x80
  export C_SUB_ADDR_COMMON_FIFO_OVERFLOW1=0x84
  export C_SUB_ADDR_COMMON_FIFO_UNDERRUN0=0x88
  export C_SUB_ADDR_COMMON_FIFO_UNDERRUN1=0x8C
  export C_SUB_ADDR_COMMON_PCIE_ERROR0=0x90
  export C_SUB_ADDR_COMMON_PCIE_ERROR1=0x94
  export C_SUB_ADDR_COMMON_PCIE_TIMEOUT_CNT=0x98
  export C_SUB_ADDR_COMMON_BUFFER_FULL=0xA0
  export C_SUB_ADDR_COMMON_TXF_FULL=0xA4
  export C_BASE_ADDR_MDIO=0x010 # mmi_mdio
  export C_SUB_ADDR_MDIO_WRITE=0x00 # Command & Write Data
  export C_SUB_ADDR_MDIO_READ=0x04 # Status & Read Data
  export C_SUB_ADDR_RGMII_TAP=0x00 # RW
  export C_SUB_ADDR_RGMII_LINK_STAT=0x04 # RO
  export C_SUB_ADDR_RGMII_CLK_SPEED=0x08 # RO
  export C_SUB_ADDR_RGMII_DPX_STAT=0x0C # RO
  export C_SUB_ADDR_DISPLAYMUX_MMI_disp_src=0x00
  export C_BASE_ADDR_UDISPLAYMUX=0x800 # user_module_mmi
  export C_BASE_ADDR_MAC_0=0x030
  export C_BASE_ADDR_MAC_1=0x031
  export C_BASE_ADDR_MAC_2=0x032
  export C_BASE_ADDR_MAC_3=0x033
  export C_BASE_ADDR_MAC_4=0x034
  export C_BASE_ADDR_MAC_5=0x035
  export C_BASE_ADDR_MAC_6=0x036
  export C_BASE_ADDR_MAC_7=0x037
  export C_BASE_ADDR_MAC_8=0x038
  export C_BASE_ADDR_MAC_9=0x039
  export C_BASE_ADDR_MAC_10=0x03A
  export C_BASE_ADDR_MAC_11=0x03B
  export C_SUB_ADDR_MAC_6T_ADDR_L=0x00 # lower 32Bit of 6T Addr
  export C_SUB_ADDR_MAC_6T_ADDR_H=0x04 # upper Bits of 6T Addr
  export C_SUB_ADDR_MAC_6T_SRC_PTR=0x08 # Index in 6T Addr
  export C_SUB_ADDR_MAC_6T_DST_PTR=0x0C # Index in 6T addr
  export C_SUB_ADDR_MAC_OWN_MAC_ADDR_L=0x10 # lower 32Bit of our own MAC Address
  export C_SUB_ADDR_MAC_OWN_MAC_ADDR_H=0x14 # upper Bits of our own MAC Address
  export C_SUB_ADDR_HC_CNT_BAD_FRAMES=0x00
  export C_SUB_ADDR_ETH_SW_COMMON=0x00
  export C_SUB_ADDR_ETH_SW_NUM_SOF=0x10
  export C_SUB_ADDR_ETH_SW_NUM_LEARNED=0x14
  export C_SUB_ADDR_ETH_SW_NUM_AGED_OUT=0x18
  export C_SUB_ADDR_ETH_SW_NUM_FLOOD_UNKNOWN=0x1C
  export C_SUB_ADDR_ETH_SW_NUM_FLOOD_FULL=0x20
  export C_SUB_ADDR_ETH_SW_NUM_PORT_ERROR=0x24
  export C_SUB_ADDR_ETH_SW_NUM_MATCH_SRC=0x28
  export C_SUB_ADDR_ETH_SW_NUM_MATCH_DST=0x2C
  export C_SUB_ADDR_ETH_SW_CMP_SRC_MATCH_L=0x30
  export C_SUB_ADDR_ETH_SW_CMP_SRC_MATCH_H=0x34
  export C_SUB_ADDR_ETH_SW_CMP_DST_MATCH_L=0x38
  export C_SUB_ADDR_ETH_SW_CMP_DST_MATCH_H=0x3C
  export C_SUB_ADDR_ETH_SW_TABLE_ADDR=0x50
  export C_SUB_ADDR_ETH_SW_TABLE_DATA_L=0x54
  export C_SUB_ADDR_ETH_SW_TABLE_DATA_H=0x58
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_7_0=0x60
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_15_8=0x64
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_23_16=0x68
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_31_24=0x6C
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_39_32=0x70
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_47_40=0x74
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_55_48=0x78
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_63_56=0x7C
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_71_64=0x80
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_79_72=0x84
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_87_80=0x88
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_95_88=0x8C
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_103_96=0x90
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_111_104=0x94
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_119_112=0x98
  export C_SUB_ADDR_ETH_SW_BUCKET_FILL_127_120=0x9C
  export C_BASE_ADDR_FLOW_CACHE_HASH_LOWER=0x100 #U --flow cache hash table is defined as range  0x10000 to 0x1fffc
  export C_BASE_ADDR_FLOW_CACHE_HASH_UPPER=0x1FF #U --flow cache hash table is defined as range  0x10000 to 0x1fffc
  export C_BASE_ADDR_FLOW_CACHE_EMA_CAM=0x200
  export C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER=0x300 #U --flow cache rules table is defined as range  0x30000 to 0x37ffc
  export C_BASE_ADDR_FLOW_CACHE_FIELD_UPPER=0x37F #U --flow cache rules table is defined as range  0x30000 to 0x37ffc
  export C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER=0x500 #U --flow cache linear search is defined as range  0x50000 to 0x501fc
  export C_BASE_ADDR_FLOW_CACHE_LINEAR_UPPER=0x501 #U --flow cache linear search is defined as range  0x50000 to 0x501fc
  export C_BASE_ADDR_FLOW_CACHE_EMA_LOWER=0x600 #U
  export C_BASE_ADDR_FLOW_CACHE_EMA_UPPER=0x61F #U
  export C_BASE_ADDR_USER_MOD=0x700 # user_module_mmi
  export C_SUB_ADDR_USER_MOD_MMI_ID=0x00 # RW  Address for "unique" pattern
  export C_SUB_ADDR_USER_MOD_MMI_BEEP_EN=0x04 # RW  Address to enable beeping
  export C_SUB_ADDR_USER_MOD_MMI_DISPLAY_EN=0x08 # RW  Address to enable display dots
  export C_SUB_ADDR_USER_MOD_MMI_PKT_CNT0=0x0C # RO,COR  Address for RX DP0 packet counter
  export C_SUB_ADDR_USER_MOD_MMI_PKT_CNT1=0x10 # RO,COR  Address for RX DP1 packet counter
  export C_SUB_ADDR_USER_MOD_MMI_PKT_CNT_WIDE0=0x14 # RO  Address for wide RX DP0 packet counter
  export C_SUB_ADDR_USER_MOD_MMI_PKT_CNT_WIDE1=0x18 # RO  Address for wide RX DP1 packet counter
  export C_BASE_ADDR_NOC_ACTION_LOWER=0x800 #U
  export C_BASE_ADDR_NOC_ACTION_UPPER=0x807 #U
  export C_BASE_ADDR_TM=0xA00 # mmi_tm
  export C_SUB_ADDR_TM_QUEUE_THRES_FULL=0x00 # lower 32Bit of 6T Addr
  export C_BASE_ADDR_PERIPH=0xC00 # mmi_peripherals
  export C_SUB_ADDR_PERIPH_BUTTONS=0x00 # RO  Address for front and back buttons
  export C_SUB_ADDR_PERIPH_INTERRUPTS=0x04 # RO  Address for PHY interrupts
  export C_SUB_ADDR_PERIPH_LEDS=0x08 # RW  Address for the LEDs
  export C_SUB_ADDR_PERIPH_GPIOS=0x0C # RW  Address for the GPIO headers
  export C_SUB_ADDR_PERIPH_DISPLAY0=0x10 # RW  Address for the front panel display
  export C_SUB_ADDR_PERIPH_DISPLAY1=0x14 # RW  Address for the front panel display
  export C_SUB_ADDR_PERIPH_DISPLAY2=0x18 # RW  Address for the front panel display
  export C_SUB_ADDR_PERIPH_DISPLAY3=0x1C # RW  Address for the front panel display
  export C_SUB_ADDR_PERIPH_BEEP=0x20 # RW  Address for the beeper
  export C_SUB_ADDR_PERIPH_BRIGHTNESS=0x24 # RW  Address for the display brightness
