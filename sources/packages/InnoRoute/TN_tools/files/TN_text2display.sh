#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <string> is used to display a 16-digit string on the front panel"
  echo "Parameter <string> can consist of those characters: {A-Z, a-z, 0-9, '.'}"
else
  # Convert all characters to 7-segment bitmaps
  word=$1
  text=''
  for i in {1..16}
  do
    letter=${word:i-1:1}
    case $letter in
      A) code=119;;
      B) code=127;;
      C) code=$((127-2-4-64));;
      D) code=$((127-64));;
      E) code=$((255-128-2-4));;
      F) code=$((127-2-4-8));;
      G) code=$((127-2-64));;
      H) code=$((127-1-8));;
      I) code=$((2+4));;
      J) code=$((16+8+4+2));;
      K) code=$((127-1-4));;
      L) code=$((32+16+8));;
      M) code=$((1+16+64+4));;
      N) code=$((127-8-64));;
      O) code=$((127-64));;
      P) code=$((127-4-8));;
      Q) code=$((127-16-4));;
      R) code=$((16+32+1));;
      S) code=$((127-2-16));;
      T) code=$((1+2+4));;
      U) code=$((127-1-64));;
      V) code=$((32+2+8));;
      W) code=$((127-1));;
      X) code=$((1+64+8));;
      Y) code=$((127-1-16));;
      Z) code=$((127-32-4));;
      0) code=$((127-64));;
      1) code=$((2+4));;
      2) code=$((127-32-4));;
      3) code=$((127-32-16));;
      4) code=$((127-1-16-8));;
      5) code=$((127-16-2));;
      6) code=$((127-2));;
      7) code=$((1+2+4));;
      8) code=$((127));;
      9) code=$((127-16));;
      a) code=$((127-32));;
      b) code=$((127-1-2);;
      c) code=$((64+16+8));;
      d) code=$((127-32-1));;
      e) code=$((127-4));;
      f) code=$((127-2-4-8));;
      g) code=$((127-16));;
      h) code=$((127-1-2-8));;
      i) code=$((4));;
      j) code=$((8+4+2));;
      k) code=$((127-2-8));;
      l) code=$((32+16+8));;
      m) code=$((1+16+64+4));;
      n) code=$((16+64+4));;
      o) code=$((16+64+4+8));;
      p) code=$((127-8-4));;
      q) code=$((127-16-8));;
      r) code=$((16+64));;
      s) code=$((32+64+4));;
      t) code=$((32+16+8+64));;
      u) code=$((16+8+4));;
      v) code=$((32+2+8));;
      w) code=$((127-1));;
      x) code=$((1+64+8));;
      y) code=$((127-1-16));;
      z) code=$((127-32-4));;
      .) code=$((128));;
      -) code=$((64));;
      *) code=0 ;;
    esac
    text="$text $code"
  done
  echo "Setting $text"
  /usr/share/InnoRoute/TN_front_display.sh $text
fi
