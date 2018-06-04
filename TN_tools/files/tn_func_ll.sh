#!/bin/bash

#
# Low-Level functions: communication with system components etc.
#

source /usr/share/InnoRoute/tn_env.sh

# PHY MDIO ADDRESSES: 0+1=Alaskas, 16-25=GPHYs

# PHY MDIO counts
export   TN_NO_OF_GPHYS=10 # Max
export TN_NO_OF_ALASKAS=2  # Max

# MDIO access masks
export TN_RGMII_WRITE=0x4000000 # Mask for the R/W bit in MDIO frames
export   TN_RGMII_PHY=0x0200000 # PHY address multiplier in MDIO frames
export   TN_RGMII_REG=0x0010000 # Register address multiplier in MDIO frames

## Low-level Basic MMI/BAR1 access
# tn_ll_mmi_status -> <void>
tn_ll_mmi_status()
{
  # Read MMI status and clear it
  local _mmi_status=`TNbar1 $(($C_BASE_ADDR_BM*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_BM_MMI_STATUS)) | cut -d " " -f 6`
  local  _mmi_err_access_rd=$((($_mmi_status &          1) >>  0))
  local  _mmi_err_access_wr=$((($_mmi_status &          2) >>  1))
  local _mmi_err_timeout_rd=$((($_mmi_status &          4) >>  2))
  local _mmi_err_timeout_wr=$((($_mmi_status &          8) >>  3))
  local      _mmi_error_cnt=$((($_mmi_status &     0xFFF0) >>  4))
  local     _mmi_access_cnt=$((($_mmi_status & 0xFFFF0000) >> 16))
  #echo "mmi_status: $_mmi_status"
  #echo "err_a_r: $_mmi_err_access_rd"
  #echo "err_a_w: $_mmi_err_access_wr"
  #echo "err_t_r: $_mmi_err_timeout_rd"
  #echo "err_t_w: $_mmi_err_timeout_wr"
  #echo "err_cnt: $_mmi_error_cnt"
  #echo "acc_cnt: $_mmi_access_cnt"

  if [ $# -gt 0 ]; then
    echo "## Parameter error in tn_ll_mmi_status: not expecting any parameters, got $#"
    exit
  fi
  
  #if [[ $_mmi_status -eq $((0xEEEEEEEE)) ]]; then
  #  echo " ** MMI Read Timeout **"
  #else
    if [[ $_mmi_err_access_rd -ne 0 ]]; then
      echo "## MMI access error for read access";
    fi
    if [[ $_mmi_err_access_wr -ne 0 ]]; then
      echo "## MMI access error for write access";
    fi
    if [[ $_mmi_err_timeout_rd -ne 0 ]]; then
      echo "## MMI timeout error for read access";
    fi
    if [[ $_mmi_err_timeout_wr -ne 0 ]]; then
      echo "## MMI timeout error for write access";
    fi
    if [[ $_mmi_error_cnt -ne 0 ]]; then
      echo "## MMI errors counted: $_mmi_error_cnt error(s) during $_mmi_access_cnt access(es)";
      #echo "   (both counters are saturating: errors at 4095, accesses at 65535)"
    fi
  #fi
}

## TWO CASES: human readable result vs. machine-readable result!

# tn_ll_mmi_write <base> <sub> <write_data> -> <void>
# or
# tn_ll_mmi_write <fulladdress> <write_data> -> <void>
tn_ll_mmi_write()
{
  if [ $# -eq 2 ]; then # 2 Parameters: <fulladdress> <write_data>
    # Escape and convert to decimal
    local    _address=`printf '%d\n' "$1"`
    local _write_data=`printf '%d\n' "$2"`
    
    if [[ $(($_address & 0xFFFFFF)) -ne $_address ]]; then
      printf "## Parameter error in tn_ll_mmi_write: 'address' value %08x is out of range\n" $_address
      exit
    fi
    # $_write_data can have any 32 bit value
    
    #printf "Writing to address 0x%08x (data 0x%08x)\n" $_address $_write_data
    # Execute write
    TNbar1 $_address w $_write_data > /dev/null
    
    # Check for completion
    tn_ll_mmi_status
  elif [ $# -eq 3 ]; then # 3 Parameters: <base> <sub> <write_data>
    # Escape and convert to decimal
    local       _base=`printf '%d\n' "$1"`
    local        _sub=`printf '%d\n' "$2"`
    local _write_data=`printf '%d\n' "$3"`
    
    if [[ $(($_base & 0xFFFF)) -ne $_base ]]; then
      printf "## Parameter error in tn_ll_mmi_write: 'base' value %08x is out of range\n" $_base
      exit
    fi
    if [[ $(($_sub & 0x3FFFFF)) -ne $_sub ]]; then
      printf "## Parameter error in tn_ll_mmi_write: 'sub' value %08x is out of range\n" $_sub
      exit
    fi
    # $_write_data can have any 32 bit value
    
    #printf "Writing to address 0x%08x (data 0x%08x)\n" $(($_base*$C_BASE_ADDR_FACTOR+$_sub)) $_write_data
    # Execute write
    TNbar1 $(($_base*$C_BASE_ADDR_FACTOR+$_sub)) w $_write_data > /dev/null
    
    # Check for completion
    tn_ll_mmi_status
  else
    echo "## Parameter error in tn_ll_mmi_write: not exactly 2 or 3 parameters, but $#"
    exit
  fi
}

# tn_ll_mmi_read <base> <sub> -> <$read_data>
# or
# tn_ll_mmi_read <fulladdress> -> <$read_data>
tn_ll_mmi_read()
{
  if [ $# -eq 1 ]; then # 1 Parameter: <fulladdress>
    # Escape and convert to decimal
    local _address=`printf '%d\n' "$1"`
    #echo "address: $_address"
    if [[ $(($_address & 0xFFFFFF)) -ne $_address ]]; then
      printf "## Parameter error in tn_ll_mmi_read: 'address' value %08x is out of range\n" $_address
      exit
    fi
  
    #printf "Reading from address 0x%08x\n" $_address
    # return value read_data
    let read_data=`TNbar1 $_address | cut -d " " -f 6`
    #echo "read data: $read_data"
    # Check for completion
    tn_ll_mmi_status
  elif [ $# -eq 2 ]; then # 2 Parameters: <Base> <Sub>
    # Escape and convert to decimal
    local _base=`printf '%d\n' "$1"`
    local  _sub=`printf '%d\n' "$2"`
    #echo "base: $_base"
    #echo "sub: $_sub"
    if [[ $(($_base & 0xFFFF)) -ne $_base ]]; then
      printf "## Parameter error in tn_ll_mmi_read: 'base' value %08x is out of range\n" $_base
      exit
    fi
    if [[ $(($_sub & 0x3FFFFF)) -ne $_sub ]]; then
      printf "## Parameter error in tn_ll_mmi_read: 'sub' value %08x is out of range\n" $_sub
      exit
    fi
  
    #printf "Reading from address 0x%08x\n" $(($_base*$C_BASE_ADDR_FACTOR+$_sub))
    # return value read_data
    let read_data=`TNbar1 $(($_base*$C_BASE_ADDR_FACTOR+$_sub)) | cut -d " " -f 6`
    #echo "read data: $read_data"
    # Check for completion
    tn_ll_mmi_status
  else
    echo "## Parameter error in tn_ll_mmi_read: not exactly 2 or 3 parameters, but $#"
    exit
  fi
}

## Low-level MDIO Read Results Register, writing to $read_data -> internal use only
# tn_ll_read_mdio_result -> <$read_data>
tn_ll_read_mdio_result()
{
  if [ $# -gt 0 ]; then
    echo "## Parameter error in tn_ll_read_mdio_result: not expecting any parameters, got $#"
    exit
  fi

  tn_ll_mmi_read $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_READ
  #if [[ $read_data -eq $((0xEEEEEEEE)) ]]; then
  #  # This might be a false positive, if the read data is 0xEEEEEEEE indeed
  #  printf " ** MMI Read Timeout ** tn_ll_read_mdio_result()\n"
  #fi
}

# Low-level MDIO Register Write
# tn_ll_write_phy <phy> <page> <reg> <write_data> -> <$read_data>
# <page> can have a random value in case of accesses to GPHYs
tn_ll_write_phy()
{
  # Escape and convert to decimal
  local        _phy=`printf '%d\n' "$1"`
  local       _page=`printf '%d\n' "$2"`
  local        _reg=`printf '%d\n' "$3"`
  local _write_data=`printf '%d\n' "$4"`

  if [ $# -ne 4 ]; then
    echo " ** Parameter error in tn_ll_write_phy: not exactly 4 parameters, but $#"
    exit
  fi
  if [[ $(($_phy & 0x1F)) -ne $_phy ]]; then
    printf " ** Parameter error in tn_ll_write_phy: 'phy' value %08x is out of range ((page %d), reg %d, write data 0x%08x)\n" $_phy $_page $_reg $_write_data
    exit
  fi
  if [[ $(($_reg & 0x1F)) -ne $_reg ]]; then
    printf " ** Parameter error in tn_ll_write_phy: 'reg' value %08x is out of range (phy %d, (page %d), write data 0x%08x)\n" $_reg $_phy $_page $_write_data
    exit
  fi
  if [[ $(($_write_data & 0xFFFF)) -ne $_write_data ]]; then
    printf " ** Parameter error in tn_ll_write_phy: 'write_data' value %08x is out of range (phy %d, (page %d), reg %d)\n" $_write_data $_phy $_page $_reg
    exit
  fi

  if   [[ $_phy -ge 0 && $_phy -lt $TN_NO_OF_GPHYS ]]; then
    # GPHY Access
    
    let _phy_addr=$_phy+16
  elif [[ $_phy -ge $TN_NO_OF_GPHYS && $_phy -lt $(( $TN_NO_OF_GPHYS + $TN_NO_OF_ALASKAS )) ]]; then
    # Alaska Access
    if [[ $(($_page & 0xFF)) -ne $_page ]]; then
      printf " ** Parameter error in tn_ll_write_phy: 'page' value %08x is out of range (phy %d, reg %d, write data 0x%08x)\n" $_page $_phy $_reg $_write_data
      exit
    fi

    local _phy_addr=$_phy

    # Set page (via register 22)
    tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE $((1*$TN_RGMII_WRITE+$_phy_addr*$TN_RGMII_PHY+22*$TN_RGMII_REG+$_page))
  else
    printf " ** Parameter error in tn_ll_write_phy: 'phy' value %08x is beyond valid values ((page %d), reg %d, write data 0x%08x)\n" $_phy $_page $_reg $_write_data
    exit
  fi

  # Execute write
  tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE $((1*$TN_RGMII_WRITE+$_phy_addr*$TN_RGMII_PHY+$_reg*$TN_RGMII_REG+$_write_data))

  # Check for completion (overwriting $read_data)
  tn_ll_read_mdio_result
  #if [[ $read_data -eq $((0xEEEEEEEE)) ]]; then
  #  # This might be a false positive, if the read data is 0xEEEEEEEE indeed
  #  printf " ** MMI Read Timeout ** tn_ll_write_phy (phy %d, (page %d), reg %d, write data 0x%08x)\n" $_phy $_page $_reg $_write_data
  #fi
}

# Low-level MDIO Register Read: using $_page (only in case of Alaska accesses), returning results as $read_data
# tn_ll_read_phy <phy> <page> <reg> -> <$read_data>
tn_ll_read_phy()
{
  # Escape and convert to decimal
  local        _phy=`printf '%d\n' "$1"`
  local       _page=`printf '%d\n' "$2"`
  local        _reg=`printf '%d\n' "$3"`

  if [ $# -ne 3 ]; then
    echo " ** Parameter error in tn_ll_read_phy: not exactly 3 parameters, but $#"
    exit
  fi
  if [[ $(($_phy & 0x1F)) -ne $_phy ]]; then
    printf " ** Parameter error in tn_ll_read_phy: 'phy' value %08x is out of range ((page %d), reg %d)\n" $_phy $_page $_reg
    exit
  fi
  if [[ $(($_reg & 0x1F)) -ne $_reg ]]; then
    printf " ** Parameter error in tn_ll_read_phy: 'reg' value %08x is out of range (phy %d, (page %d))\n" $_reg $_phy $_page
    exit
  fi

  if   [[ $_phy -ge 0 && $_phy -lt $TN_NO_OF_GPHYS ]]; then
    # GPHY Access
    
    let _phy_addr=$_phy+16
  elif [[ $_phy -ge $TN_NO_OF_GPHYS && $_phy -lt $(( $TN_NO_OF_GPHYS + $TN_NO_OF_ALASKAS )) ]]; then
    # Alaska Access
    if [[ $(($_page & 0xFF)) -ne $_page ]]; then
      printf " ** Parameter error in tn_ll_read_phy: 'page' value %08x is out of range (phy %d, reg %d)\n" $_page $_phy $_reg
      exit
    fi

    local _phy_addr=$_phy

    # Set page (via register 22)
    tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE $((1*$TN_RGMII_WRITE+$_phy_addr*$TN_RGMII_PHY+22*$TN_RGMII_REG+$_page))
  else
    printf " ** Parameter error in tn_ll_read_phy: 'phy' value %08x is beyond valid values ((page %d), reg %d)\n" $_phy $_page $_reg
    exit
  fi

  # Execute read
  tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE $((0*$TN_RGMII_WRITE+$_phy_addr*$TN_RGMII_PHY+$_reg*$TN_RGMII_REG+0))

  # Check for completion (overwriting $read_data)
  tn_ll_read_mdio_result
  
  #if [[ $read_data -eq $((0xEEEEEEEE)) ]]; then
  #  # This might be a false positive, if the read data is 0xEEEEEEEE indeed
  #  printf " ** MMI Read Timeout ** tn_ll_read_phy (phy %d, (page %d), reg %d)\n" $_phy $_page $_reg
  #fi
}
