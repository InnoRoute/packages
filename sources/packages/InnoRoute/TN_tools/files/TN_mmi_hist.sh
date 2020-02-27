#!/bin/bash
source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to print the history of MMI accesses."
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
else
  if [ -z "$(lspci -mm -d 10ee:0000)" ]; then echo "No PCIe connection to FPGA. Exiting."; exit 1; fi

  for entry in {1..512}
  do
    tn_ll_mmi_read $C_BASE_ADDR_BM $C_SUB_ADDR_BM_MMI_ACCESSES
    if [ "$(( $read_data & 0x80000000 ))" -gt "0" ]; then
              let mmi_addr=$(($read_data & 0x00FFFFFF))
              let mmi_wr=$((($read_data &   0x01000000) >> 24))
              let mmi_rd=$((($read_data &   0x02000000) >> 25))
              let mmi_data=$((($read_data & 0x7C000000) >> 26))
      printf "%06x: RD=%d, WR=%d, WR_DATA=..%x\n" $mmi_addr $mmi_wr $mmi_rd $mmi_data
    fi
  done
fi
