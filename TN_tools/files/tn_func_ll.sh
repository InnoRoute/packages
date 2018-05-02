#!/bin/bash

#
# Low-Level functions: communication with system components etc.
#

source /usr/share/InnoRoute/tn_env.sh

# PHY MDIO ADDRESSES: 0+1=Alaskas, 16-25=GPHYs

# PHY MDIO counts
export TN_NO_OF_GPHYS=10  # Max
export TN_NO_OF_ALASKAS=2 # Max

# MDIO access masks
export TN_RGMII_WRITE=0x4000000 # Mask for the R/W bit in MDIO frames
export TN_RGMII_PHY=0x200000    # PHY address multiplier in MDIO frames
export TN_RGMII_REG=0x10000     # Register address multiplier in MDIO frames

## Low-level Basic MMI/BAR1 access
tn_ll_mmi_status()
{
  let mmi_status=`TNbar1 $(($C_BASE_ADDR_BM*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_BM_MMI_STATUS)) | cut -d " " -f 6`
  
  let  mmi_err_access_rd=$((($mmi_status &          1) >>  0))
  let  mmi_err_access_wr=$((($mmi_status &          2) >>  1))
  let mmi_err_timeout_rd=$((($mmi_status &          4) >>  2))
  let mmi_err_timeout_wr=$((($mmi_status &          8) >>  3))
  let      mmi_error_cnt=$((($mmi_status &     0xFFF0) >>  4))
  let     mmi_access_cnt=$((($mmi_status & 0xFFFF0000) >> 16))
  #if [[ $mmi_status -eq 0xEEEEEEEE ]]; then
  #  echo " ** MMI Read Timeout **"
  #else
    if [[ $mmi_err_access_rd -ne 0 ]]; then
      echo "## MMI access error for read access";
    fi
    if [[ $mmi_err_access_wr -ne 0 ]]; then
      echo "## MMI access error for write access";
    fi
    if [[ $mmi_err_timeout_rd -ne 0 ]]; then
      echo "## MMI timeout error for read access";
    fi
    if [[ $mmi_err_timeout_wr -ne 0 ]]; then
      echo "## MMI timeout error for write access";
    fi
    if [[ $mmi_error_cnt -ne 0 ]]; then
      echo "## MMI errors counted: $mmi_error_cnt errors during $mmi_access_cnt accesses";
	  echo "   (both counters are saturating: errors at 4095, accesses at 65535)"
    fi
  #fi
}

## TWO CASES: human readable result vs. machine-readable result!

tn_ll_mmi_write()
{
  if [[ $(($base & 0xFFFF)) -ne $base ]]; then
    printf "## Parameter error in tn_ll_mmi_write: 'base' value %08x is out of range\n" $base
	exit
  fi
  if [[ $(($sub & 0x3FFFFF)) -ne $sub ]]; then
    printf "## Parameter error in tn_ll_mmi_write: 'sub' value %08x is out of range\n" $sub
	exit
  fi
  # $write_data can have any 32 bit value

  # Execute write
  TNbar1 $(($base*$C_BASE_ADDR_FACTOR+$sub)) w $write_data > /dev/null

  # Check for completion
  tn_ll_mmi_status
}

tn_ll_mmi_read()
{
  if [[ $(($base & 0xFFFF)) -ne $base ]]; then
    printf "## Parameter error in tn_ll_mmi_write: 'base' value %08x is out of range\n" $base
	exit
  fi
  if [[ $(($sub & 0x3FFFFF)) -ne $sub ]]; then
    printf "## Parameter error in tn_ll_mmi_write: 'sub' value %08x is out of range\n" $sub
	exit
  fi

  let read_data=`TNbar1 $(($base*$C_BASE_ADDR_FACTOR+$sub)) | cut -d " " -f 6`

  # Check for completion
  tn_ll_mmi_status
}

## Low-level MDIO Read Results Register, writing to $read_data -> internal use only
tn_ll_read_mdio_result()
{
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    # This might be a false positive, if the read data is 0xEEEEEEEE indeed
    printf " ** MMI Read Timeout ** tn_ll_read_mdio_result()\n"
  fi
}

# Low-level MDIO Register Write: using $phy, $page (only in case of Alaska accesses), $reg, $write_data (modifying $read_data)
tn_ll_write_phy()
{
  if [[ $(($phy & 0x1F)) -ne $phy ]]; then
    printf " ** Parameter error in tn_ll_write_phy: 'phy' value %08x is out of range ((page %d), reg %d, write data 0x%08x)\n" $phy $page $reg $write_data
	exit
  fi
  if [[ $(($reg & 0x1F)) -ne $reg ]]; then
    printf " ** Parameter error in tn_ll_write_phy: 'reg' value %08x is out of range (phy %d, (page %d), write data 0x%08x)\n" $reg $phy $page $write_data
	exit
  fi
  if [[ $(($write_data & 0xFFFF)) -ne $write_data ]]; then
    printf " ** Parameter error in tn_ll_write_phy: 'write_data' value %08x is out of range (phy %d, (page %d), reg %d)\n" $write_data $phy $page $reg
	exit
  fi

  if   [[ $phy -ge 0 && $phy -lt $TN_NO_OF_GPHYS ]]; then
    # GPHY Access
	
	let phy_addr=$phy+16
  elif [[ $phy -ge $TN_NO_OF_GPHYS && $phy -lt $(( $TN_NO_OF_GPHYS + $TN_NO_OF_ALASKAS )) ]];
    # Alaska Access
    if [[ $(($page & 0xFF)) -ne $page ]]; then
      printf " ** Parameter error in tn_ll_write_phy: 'page' value %08x is out of range (phy %d, reg %d, write data 0x%08x)\n" $page $phy $reg $write_data
	  exit
    fi

	let phy_addr=$phy

    # Set page (via register 22)
    TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+$phy_addr*$TN_RGMII_PHY+22*$TN_RGMII_REG+$page)) > /dev/null
  else
    printf " ** Parameter error in tn_ll_write_phy: 'phy' value %08x is beyond valid values ((page %d), reg %d, write data 0x%08x)\n" $phy $page $reg $write_data
	exit
  fi

  # Execute write
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+$phy_addr*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null

  # Check for completion
  tn_ll_read_mdio_result
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    # This might be a false positive, if the read data is 0xEEEEEEEE indeed
    printf " ** MMI Read Timeout ** tn_ll_write_phy (phy %d, (page %d), reg %d, write data 0x%08x)\n" $phy $page $reg $write_data
  fi
}

# Low-level MDIO Register Read: using $phy, $page (only in case of Alaska accesses), $reg, returning results as $read_data
tn_ll_read_phy()
{
  if [[ $(($phy & 0x1F)) -ne $phy ]]; then
    printf " ** Parameter error in tn_ll_read_phy: 'phy' value %08x is out of range ((page %d), reg %d)\n" $phy $page $reg
	exit
  fi
  if [[ $(($reg & 0x1F)) -ne $reg ]]; then
    printf " ** Parameter error in tn_ll_read_phy: 'reg' value %08x is out of range (phy %d, (page %d))\n" $reg $phy $page
	exit
  fi

  if   [[ $phy -ge 0 && $phy -lt $TN_NO_OF_GPHYS ]]; then
    # GPHY Access
	
	let phy_addr=$phy+16
  elif [[ $phy -ge $TN_NO_OF_GPHYS && $phy -lt $(( $TN_NO_OF_GPHYS + $TN_NO_OF_ALASKAS )) ]];
    # Alaska Access
    if [[ $(($page & 0xFF)) -ne $page ]]; then
      printf " ** Parameter error in tn_ll_read_phy: 'page' value %08x is out of range (phy %d, reg %d)\n" $page $phy $reg
	  exit
    fi

	let phy_addr=$phy

    # Set page (via register 22)
    TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+$phy_addr*$TN_RGMII_PHY+22*$TN_RGMII_REG+$page)) > /dev/null
  else
    printf " ** Parameter error in tn_ll_read_phy: 'phy' value %08x is beyond valid values ((page %d), reg %d)\n" $phy $page $reg
	exit
  fi

  # Execute read
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((0*$TN_RGMII_WRITE+$phy_addr*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+0)) > /dev/null

  # Check for completion
  tn_ll_read_mdio_result
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    # This might be a false positive, if the read data is 0xEEEEEEEE indeed
    printf " ** MMI Read Timeout ** tn_ll_read_phy (phy %d, (page %d), reg %d)\n" $phy $page $reg
  fi
}
