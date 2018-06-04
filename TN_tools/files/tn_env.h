#define  C_BASE_ADDR_FACTOR 256 
#define  C_BASE_ADDR_TN_COMMON_L 0x0004
#define  C_BASE_ADDR_TN_COMMON_LOWER 0x0004
#define  C_BASE_ADDR_TN_COMMON 0x0004
#define  C_SUB_ADDR_MDIO_INTERRUPTS 0x08
#define  C_SUB_ADDR_MDIO_INTERRUPT_MASK 0x0C
#define  TN_NO_OF_PHYS 12   
#define  TN_NO_OF_GPHYS 10  
#define  TN_NO_OF_ALASKAS 2 
#define  TN_RGMII_WRITE 0x4000000 
#define  TN_RGMII_PHY 0x200000    
#define  TN_RGMII_REG 0x10000     
#define  C_MMI_DATA_WIDTH 32
#define  C_MMI_ADDR_WIDTH 24
#define  C_MMI_BASE_ADDR_WIDTH 16
#define  C_MMI_SUB_ADDR_WIDTH C_MMI_ADDR_WIDTH-C_MMI_BASE_ADDR_WIDTH
#define  C_MMI_BASE_UPPER C_MMI_ADDR_WIDTH-1
#define  C_MMI_BASE_LOWER C_MMI_SUB_ADDR_WIDTH
#define  C_MMI_SUB_UPPER C_MMI_BASE_LOWER-1
#define  C_MMI_SUB_LOWER 0
#define  C_MMI_ADDR_MAP_REVISION 5
#define  C_FLOW_CACHE_EMH_VERSION 0x0003
#define  C_BASE_ADDR_BM 0x0000
#define  C_SUB_ADDR_BM_MMI_STATUS 0x000
#define  C_SUB_ADDR_BM_MMI_INTERRUPT 0x004
#define  C_SUB_ADDR_BM_MMI_INTERRUPT_EN 0x008
#define  C_BASE_ADDR_MDIO 0x0001
#define  C_SUB_ADDR_MDIO_WRITE 0x00
#define  C_SUB_ADDR_MDIO_READ 0x04
#define  C_SUB_ADDR_MDIO_INTERRUPT 0x08
#define  C_SUB_ADDR_MDIO_INTERRUPT_EN 0x0C
#define  C_BASE_ADDR_PERIPH 0x0002
#define  C_SUB_ADDR_PERIPH_BUTTONS 0x00
#define  C_SUB_ADDR_PERIPH_LEDS 0x04
#define  C_SUB_ADDR_PERIPH_GPIOS 0x08
#define  C_SUB_ADDR_PERIPH_DISPLAY0 0x0C
#define  C_SUB_ADDR_PERIPH_DISPLAY1 0x10
#define  C_SUB_ADDR_PERIPH_DISPLAY2 0x14
#define  C_SUB_ADDR_PERIPH_DISPLAY3 0x18
#define  C_SUB_ADDR_PERIPH_BEEP 0x1C
#define  C_SUB_ADDR_PERIPH_BRIGHTNESS 0x20
#define  C_BASE_ADDR_COMMON_LOWER 0x0004
#define  C_BASE_ADDR_COMMON_UPPER 0x0007
#define  C_SUB_ADDR_COMMON_TN_MAJOR_REV 0x000
#define  C_SUB_ADDR_COMMON_TN_MINOR_REV 0x004
#define  C_SUB_ADDR_COMMON_USER_REV 0x008
#define  C_SUB_ADDR_COMMON_ADDR_MAP_REV 0x00C
#define  C_SUB_ADDR_COMMON_FPGA_ID0 0x010
#define  C_SUB_ADDR_COMMON_FPGA_ID1 0x014
#define  C_SUB_ADDR_COMMON_EFUSE 0x018
#define  C_SUB_ADDR_COMMON_BITGEN_TIME 0x01C
#define  C_SUB_ADDR_COMMON_RW_TEST 0x020
#define  C_SUB_ADDR_COMMON_FEATURES_6TREE 0x100
#define  C_SUB_ADDR_COMMON_FEATURES_ETH_SW 0x104
#define  C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE 0x108
#define  C_SUB_ADDR_COMMON_FEATURES_ACC_DP 0x10C
#define  C_SUB_ADDR_COMMON_FEATURES_TX_ROUTER 0x110
#define  C_SUB_ADDR_COMMON_FEATURES_SFP 0x114
#define  C_SUB_ADDR_COMMON_FEATURES_SODIMM 0x118
#define  C_SUB_ADDR_COMMON_FEATURES_USER 0x11C
#define  C_SUB_ADDR_COMMON_PARAM_PRT_CNT 0x120
#define  C_SUB_ADDR_COMMON_PARAM_DP_CNT 0x124
#define  C_SUB_ADDR_COMMON_PARAM_BUF_CNT 0x128
#define  C_SUB_ADDR_COMMON_PARAM_RX_MIN 0x130
#define  C_SUB_ADDR_COMMON_PARAM_RX_MAX 0x134
#define  C_SUB_ADDR_COMMON_PARAM_TX_PRE 0x138
#define  C_SUB_ADDR_COMMON_PARAM_TX_IFG 0x13C
#define  C_SUB_ADDR_COMMON_PARAM_DISPLAY 0x140
#define  C_SUB_ADDR_COMMON_PARAM_LED_SYNCE 0x144
#define  C_SUB_ADDR_COMMON_PARAM_LED_INTERNAL 0x148
#define  C_SUB_ADDR_COMMON_PARAM_BEEP 0x14C
#define  C_SUB_ADDR_COMMON_PARAM_PHY_RESET 0x150
#define  C_SUB_ADDR_COMMON_PARAM_PINHEADER 0x154
#define  C_SUB_ADDR_COMMON_PARAM_PMOD 0x158
#define  C_SUB_ADDR_COMMON_PARAM_EXT_BYPASS 0x15C
#define  C_SUB_ADDR_COMMON_WIDTH_HC_BAD 0x160
#define  C_SUB_ADDR_COMMON_WIDTH_ETH_SW 0x164
#define  C_SUB_ADDR_COMMON_WIDTH_RX_STAT_BAD 0x168
#define  C_SUB_ADDR_COMMON_FPGA_TEMP 0x200
#define  C_SUB_ADDR_COMMON_FPGA_ALARM 0x204
#define  C_SUB_ADDR_COMMON_CONFIG_CHECK 0x208
#define  C_SUB_ADDR_COMMON_FIFO_OVERFLOW0 0x210
#define  C_SUB_ADDR_COMMON_FIFO_OVERFLOW1 0x214
#define  C_SUB_ADDR_COMMON_FIFO_OVERFLOW2 0x218
#define  C_SUB_ADDR_COMMON_FIFO_OVERFLOW3 0x21C
#define  C_SUB_ADDR_COMMON_FIFO_UNDERRUN0 0x220
#define  C_SUB_ADDR_COMMON_FIFO_UNDERRUN1 0x224
#define  C_SUB_ADDR_COMMON_FIFO_UNDERRUN2 0x228
#define  C_SUB_ADDR_COMMON_FIFO_UNDERRUN3 0x22C
#define  C_SUB_ADDR_COMMON_PCIE_ERROR0 0x230
#define  C_SUB_ADDR_COMMON_PCIE_ERROR1 0x234
#define  C_SUB_ADDR_COMMON_PCIE_TIMEOUT_CNT 0x238
#define  C_SUB_ADDR_COMMON_BUFFER_FULL 0x240
#define  C_SUB_ADDR_COMMON_TXF_FULL 0x244
#define  C_SUB_ADDR_COMMON_CLK_MON_L 0x250
#define  C_SUB_ADDR_COMMON_CLK_MON_H 0x2BC
#define  C_BASE_ADDR_ACC_0 0x1000
#define  C_BASE_ADDR_ACC_1 0x2800
#define  C_BASE_ADDR_NET_LOWER 0x4000
#define  C_BASE_ADDR_NET_UPPER 0x400F
#define  C_SUB_ADDR_NET_TAP 0x00
#define  C_SUB_ADDR_NET_ENABLE 0x10
#define  C_SUB_ADDR_NET_SPEED 0x14
#define  C_SUB_ADDR_NET_DUPLEX 0x18
#define  C_SUB_ADDR_NET_PAUSE 0x1C
#define  C_SUB_ADDR_NET_OWN_MAC_ADDR_L 0x20
#define  C_SUB_ADDR_NET_OWN_MAC_ADDR_H 0x24
#define  C_SUB_ADDR_NET_6T_ADDR_L 0x30
#define  C_SUB_ADDR_NET_6T_ADDR_H 0x34
#define  C_SUB_ADDR_NET_6T_SRC_PTR 0x38
#define  C_SUB_ADDR_NET_6T_DST_PTR 0x3C
#define  C_SUB_ADDR_NET_TX_CONF_VLD 0x40
#define  C_SUB_ADDR_NET_TX_CONF_L 0x50
#define  C_SUB_ADDR_NET_TX_CONF_H 0xCC
#define  C_BASE_ADDR_RTC 0x7F00
#define  C_SUB_ADDR_RTC_BRIDGE_LOW 0x00
#define  C_SUB_ADDR_RTC_BRIDGE_HIGH 0x04
#define  C_SUB_ADDR_RTC_CTRLD_LOW 0x08
#define  C_SUB_ADDR_RTC_CTRLD_HIGH 0x0C
#define  C_SUB_ADDR_RTC_CTRLD_OFFSET_LOW 0x10
#define  C_SUB_ADDR_RTC_CTRLD_OFFSET_HIGH 0x14
#define  C_SUB_ADDR_RTC_CTRLD_RATE 0x18
#define  C_SUB_ADDR_RTC_INTERRUPT 0x1C
#define  C_SUB_ADDR_RTC_INTERRUPT_EN 0x20
#define  C_SUB_ADDR_RTC_CLKSEL 0x24
#define  C_BASE_ADDR_HC_0 0x8000
#define  C_BASE_ADDR_HC_1 0x8001
#define  C_BASE_ADDR_HC_2 0x8002
#define  C_BASE_ADDR_HC_3 0x8003
#define  C_BASE_ADDR_HC_4 0x8004
#define  C_BASE_ADDR_HC_5 0x8005
#define  C_BASE_ADDR_HC_6 0x8006
#define  C_BASE_ADDR_HC_7 0x8007
#define  C_BASE_ADDR_HC_8 0x8008
#define  C_BASE_ADDR_HC_9 0x8009
#define  C_BASE_ADDR_HC_10 0x800A
#define  C_BASE_ADDR_HC_11 0x800B
#define  C_BASE_ADDR_HC_12 0x800C
#define  C_BASE_ADDR_HC_13 0x800D
#define  C_BASE_ADDR_HC_PCIE_0 0x800E
#define  C_BASE_ADDR_HC_PCIE_1 0x800F
#define  C_SUB_ADDR_HC_CNT_BAD_FRAMES 0x00
#define  C_SUB_ADDR_HC_INTERRUPT 0x04
#define  C_SUB_ADDR_HC_INTERRUPT_EN 0x08
#define  C_BASE_ADDR_HT_0 0x8080
#define  C_BASE_ADDR_HT_1 0x8081
#define  C_BASE_ADDR_HT_2 0x8082
#define  C_BASE_ADDR_HT_3 0x8083
#define  C_BASE_ADDR_HT_4 0x8084
#define  C_BASE_ADDR_HT_5 0x8085
#define  C_BASE_ADDR_HT_6 0x8086
#define  C_BASE_ADDR_HT_7 0x8087
#define  C_BASE_ADDR_HT_8 0x8088
#define  C_BASE_ADDR_HT_9 0x8089
#define  C_BASE_ADDR_HT_10 0x808A
#define  C_BASE_ADDR_HT_11 0x808B
#define  C_BASE_ADDR_HT_12 0x808C
#define  C_BASE_ADDR_HT_13 0x808D
#define  C_BASE_ADDR_HT_PCIE_0 0x808E
#define  C_BASE_ADDR_HT_PCIE_1 0x808F
#define  C_BASE_ADDR_ETH_SW_0 0x8100
#define  C_BASE_ADDR_ETH_SW_1 0x8200
#define  C_SUB_ADDR_ETH_SW_COMMON 0x00
#define  C_SUB_ADDR_ETH_SW_NUM_SOF 0x10
#define  C_SUB_ADDR_ETH_SW_NUM_LEARNED 0x14
#define  C_SUB_ADDR_ETH_SW_NUM_AGED_OUT 0x18
#define  C_SUB_ADDR_ETH_SW_NUM_FLOOD_UNKNOWN 0x1C
#define  C_SUB_ADDR_ETH_SW_NUM_FLOOD_FULL 0x20
#define  C_SUB_ADDR_ETH_SW_NUM_PORT_ERROR 0x24
#define  C_SUB_ADDR_ETH_SW_NUM_MATCH_SRC 0x28
#define  C_SUB_ADDR_ETH_SW_NUM_MATCH_DST 0x2C
#define  C_SUB_ADDR_ETH_SW_CMP_SRC_MATCH_L 0x30
#define  C_SUB_ADDR_ETH_SW_CMP_SRC_MATCH_H 0x34
#define  C_SUB_ADDR_ETH_SW_CMP_DST_MATCH_L 0x38
#define  C_SUB_ADDR_ETH_SW_CMP_DST_MATCH_H 0x3C
#define  C_SUB_ADDR_ETH_SW_TABLE_ADDR 0x50
#define  C_SUB_ADDR_ETH_SW_TABLE_DATA_L 0x54
#define  C_SUB_ADDR_ETH_SW_TABLE_DATA_H 0x58
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_7_0 0x60
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_15_8 0x64
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_23_16 0x68
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_31_24 0x6C
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_39_32 0x70
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_47_40 0x74
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_55_48 0x78
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_63_56 0x7C
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_71_64 0x80
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_79_72 0x84
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_87_80 0x88
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_95_88 0x8C
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_103_96 0x90
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_111_104 0x94
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_119_112 0x98
#define  C_SUB_ADDR_ETH_SW_BUCKET_FILL_127_120 0x9C
#define  C_BASE_ADDR_STATISTIC_BADRSN_LOWER_0 0x8300
#define  C_BASE_ADDR_STATISTIC_BADRSN_LOWER_1 0x8500
#define  C_BASE_ADDR_STATISTIC_BADRSN_UPPER_0 0x830F
#define  C_BASE_ADDR_STATISTIC_BADRSN_UPPER_1 0x850F
#define  C_BASE_ADDR_STATISTIC_PKT_CNT_LOWER_0 0x8310
#define  C_BASE_ADDR_STATISTIC_PKT_CNT_LOWER_1 0x8510
#define  C_BASE_ADDR_STATISTIC_PKT_CNT_UPPER_0 0x831F
#define  C_BASE_ADDR_STATISTIC_PKT_CNT_UPPER_1 0x851F
#define  C_BASE_ADDR_STATISTIC_GOOD_FLOWID_LOWER_0 0x8320
#define  C_BASE_ADDR_STATISTIC_GOOD_FLOWID_LOWER_1 0x8520
#define  C_BASE_ADDR_STATISTIC_GOOD_FLOWID_UPPER_0 0x832F
#define  C_BASE_ADDR_STATISTIC_GOOD_FLOWID_UPPER_1 0x852F
#define  C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_0 0x8330
#define  C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_1 0x8530
#define  C_BASE_ADDR_STATISTIC_GOOD_INPRT_UPPER_0 0x8330
#define  C_BASE_ADDR_STATISTIC_GOOD_INPRT_UPPER_1 0x8530
#define  C_BASE_ADDR_USER_MOD 0x8800
#define  C_SUB_ADDR_USER_MOD_MMI_ID 0x00
#define  C_SUB_ADDR_USER_MOD_MMI_BEEP_EN 0x04
#define  C_SUB_ADDR_USER_MOD_MMI_DISPLAY_EN 0x08
#define  C_SUB_ADDR_USER_MOD_MMI_PKT_CNT0 0x0C
#define  C_SUB_ADDR_USER_MOD_MMI_PKT_CNT_WIDE0 0x10
#define  C_SUB_ADDR_USER_MOD_MMI_PKT_CNT1 0x14
#define  C_SUB_ADDR_USER_MOD_MMI_PKT_CNT_WIDE1 0x18
#define  C_BASE_ADDR_NOC_ACTION_LOWER 0x8900
#define  C_BASE_ADDR_NOC_ACTION_UPPER 0x89FF
#define  C_BASE_ADDR_FLOW_CACHE_HASH_LOWER 0x9000
#define  C_BASE_ADDR_FLOW_CACHE_HASH_UPPER 0x9FFF
#define  C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER 0xB000
#define  C_BASE_ADDR_FLOW_CACHE_FIELD_UPPER 0xBFFF
#define  C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER 0xC000
#define  C_BASE_ADDR_FLOW_CACHE_LINEAR_UPPER 0xC0FF
#define  C_BASE_ADDR_FLOW_CACHE_EMA_LOWER 0xC100
#define  C_BASE_ADDR_FLOW_CACHE_EMA_UPPER 0xC1FF
#define  C_BASE_ADDR_FLOW_CACHE_EMA_CAM_LOWER 0xC200
#define  C_BASE_ADDR_FLOW_CACHE_EMA_CAM_UPPER 0xC21F
#define  C_BASE_ADDR_FLOW_CACHE 0xC300
#define  C_BASE_ADDR_TM 0xD000
#define  C_SUB_ADDR_TM_QUEUE_THRES_FULL 0x00
#define  C_SUB_ADDR_TM_PTR_CNT 0x04
#define  C_BASE_ADDR_TM_SCHED_LOWER 0xD200
#define  C_BASE_ADDR_TM_SCHED_UPPER 0xDFFF
#define  C_BLOCK_SIZE_ADDR_TM_SCHED 0x800
#define  C_TM_SCHED_GCL_LOWER 0x000
#define  C_TM_SCHED_GCL_UPPER 0x1FC
#define  C_TM_SCHED_GCL_TIME_LOWER 0x200
#define  C_TM_SCHED_GCL_TIME_UPPER 0x3FC
#define  C_TM_SCHED_TAS_CONFIG_REG_LOWER 0x400
#define  C_TM_SCHED_TAS_CONFIG_REG_UPPER 0x4FC
#define  C_TM_SCHED_PROC_QUEUE_PRIO_LOWER 0x500
#define  C_TM_SCHED_PROC_QUEUE_PRIO_UPPER 0x53C
#define  C_TM_SCHED_PROC_QUEUE_PRIO_HIGH 0x540
#define  C_TM_SCHED_TAS_CUR_TIME 0x544
#define  C_TM_SCHED_TAS_ADMIN_GCL_LEN 0x400
#define  C_TM_SCHED_TAS_ADMIN_BASE_TIME 0x404
#define  C_TM_SCHED_TAS_ADMIN_CYCLE_TIME 0x408
#define  C_TM_SCHED_TAS_ADMIN_CYCLE_TIME_EXT 0x40C
#define  C_TM_SCHED_TAS_CONFIG_CHANGE_TIME 0x410
#define  C_TM_SCHED_TAS_CYCLE_START_TIME 0x414
#define  C_TM_SCHED_TAS_GATE_ENABLE 0x418
#define  C_TM_SCHED_TAS_CONFIG_CHANGE 0x41C
#define  C_TM_SCHED_TAS_CONFIG_CHANGE_PENDING 0x420
#define  C_TM_SCHED_TAS_CONFIG_CHANGE_ACK 0x424
#define  C_TM_SCHED_TAS_OPER_GCL_LEN 0x428
#define  C_TM_SCHED_TAS_OPER_BASE_TIME 0x42C
#define  C_TM_SCHED_TAS_OPER_CYCLE_TIME 0x430
#define  C_TM_SCHED_TAS_OPER_CYCLE_TIME_EXT 0x434
#define  C_TM_SCHED_TAS_ADMIN_GATE_STATES 0x438
#define  C_TM_SCHED_TAS_OPER_GATE_STATES 0x43C
#define  C_BASE_ADDR_TX_ROUTER_LOWER_0 0xE100
#define  C_BASE_ADDR_TX_ROUTER_LOWER_1 0xE200
#define  C_BASE_ADDR_TX_ROUTER_UPPER_0 0xE1FF
#define  C_BASE_ADDR_TX_ROUTER_UPPER_1 0xE2FF
#define	 printconst(constante) printf(#constante ":0x%lx\n",constante)
