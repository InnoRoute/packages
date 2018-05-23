#!/bin/bash



source /usr/share/InnoRoute/tn_env.sh

echo "################################"
case $1 in
	0) let phy_prt=`TNbar1 $(($C_BASE_ADDR_HC_0*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_HC_CNT_BAD_FRAMES)) | cut -d " " -f 6` ;;
	1) let phy_prt=`TNbar1 $(($C_BASE_ADDR_HC_1*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_HC_CNT_BAD_FRAMES)) | cut -d " " -f 6` ;;
	2) let phy_prt=`TNbar1 $(($C_BASE_ADDR_HC_2*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_HC_CNT_BAD_FRAMES)) | cut -d " " -f 6` ;;
	3) let phy_prt=`TNbar1 $(($C_BASE_ADDR_HC_3*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_HC_CNT_BAD_FRAMES)) | cut -d " " -f 6` ;;
	4) let phy_prt=`TNbar1 $(($C_BASE_ADDR_HC_4*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_HC_CNT_BAD_FRAMES)) | cut -d " " -f 6` ;;
	5) let phy_prt=`TNbar1 $(($C_BASE_ADDR_HC_5*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_HC_CNT_BAD_FRAMES)) | cut -d " " -f 6` ;;
	6) let phy_prt=`TNbar1 $(($C_BASE_ADDR_HC_6*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_HC_CNT_BAD_FRAMES)) | cut -d " " -f 6` ;;
	7) let phy_prt=`TNbar1 $(($C_BASE_ADDR_HC_7*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_HC_CNT_BAD_FRAMES)) | cut -d " " -f 6` ;;
	8) let phy_prt=`TNbar1 $(($C_BASE_ADDR_HC_8*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_HC_CNT_BAD_FRAMES)) | cut -d " " -f 6`;;
	9) let phy_prt=`TNbar1 $(($C_BASE_ADDR_HC_9*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_HC_CNT_BAD_FRAMES)) | cut -d " " -f 6` ;;
	10) let phy_prt=`TNbar1 $(($C_BASE_ADDR_HC_10*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_HC_CNT_BAD_FRAMES)) | cut -d " " -f 6` ;;
	11) let phy_prt=`TNbar1 $(($C_BASE_ADDR_HC_11*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_HC_CNT_BAD_FRAMES)) | cut -d " " -f 6` ;;
	*) echo "INVALID NUMBER!" ;;
esac

  

 
echo "Port $1, HC_bad_frames:$phy_prt"
prt=$1
printf "Brsn:"
    for badrsn in `seq 0 31`; do
        printf "|0x%s|"  $(echo "$(printf "%04x  " $badrsn)");
    done
    printf "\nDP0 :"
    for badrsn in `seq 0 31`; do
      let badrsn_cnt=`TNbar1 $(($C_BASE_ADDR_STATISTIC_LOWER_0*$C_BASE_ADDR_FACTOR+($prt*32+$badrsn)*4)) | cut -d " " -f 6`
        printf "|0x%s|"  $(echo "$(printf "%04x  " $badrsn_cnt)");
    done
    printf "\nDP1 :"
    for badrsn in `seq 0 31`; do
      let badrsn_cnt=`TNbar1 $(($C_BASE_ADDR_STATISTIC_LOWER_1*$C_BASE_ADDR_FACTOR+($prt*32+$badrsn)*4)) | cut -d " " -f 6`
        printf "|0x%s|"  $(echo "$(printf "%04x  " $badrsn_cnt)");
    done
echo "################################"

