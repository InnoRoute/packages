#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <bitmap> is used to set the I/O direction and the output value per pinheader pin"
  echo "Parameter <bitmap> is a binary coded bitmap, with one bit per pin"
  echo "  bits   0-9: TrustNode-internal User-GPIO Output value"
  echo "  bits 10-13: TrustNode-internal Artix-PMod Output value"
  echo "  bits 16-25: TrustNode-internal User-GPIO is Input (not Output)"
  echo "  bits 26-29: TrustNode-internal Artix-PMod is Input (not Output)"
else

  # If a parameter is given, it is assumed that this is the GPIO output value
  let gpio=$1

  printf "Setting GPIO headers to mask/drive 0x%04x\n" $gpio
  for pin in `seq 0 9`; do
    if   [[ $(( $gpio & ( 2**( $pin + 16 ) ) )) -eq $(( 2**( $pin + 16 ) )) ]]; then
      echo -e "* User-IO pin $pin is an input";
    else
      if [[ $(( $gpio & ( 2**( $pin +  0 ) ) )) -eq $(( 2**( $pin +  0 ) )) ]]; then
        echo -e "* User-IO pin $pin is driving a 1 (output)";
      else
        echo -e "* User-IO pin $pin is driving a 0 (output)";
      fi;
    fi;
  done
  
  for pin in `seq 0 3`; do
    if   [[ $(( $gpio & ( 2**( $pin + 26 ) ) )) -eq $(( 2**( $pin + 26 ) )) ]]; then
      echo -e "* PMod pin $pin is an input";
    else
      if [[ $(( $gpio & ( 2**( $pin + 10 ) ) )) -eq $(( 2**( $pin + 10 ) )) ]]; then
        echo -e "* PMod pin $pin is driving a 1 (output)";
      else
        echo -e "* PMod pin $pin is driving a 0 (output)";
      fi;
    fi;
  done
  
  # Sets the output values of the GPIO headers:
  #   lower bits are output data (0-9=Pinheader, 10-13=PMod) and the
  #   upper bits are tristate flags (16-25=Pinheader, 26-29=PMod)
  #   Reading external signals is only possible, if tristate is active,
  #   but driving the output data is only possible, if tristate is inactive
  
  tn_ll_mmi_write $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_GPIOS $gpio

  echo "Done"
fi
