#!/bin/bash

let plladdr=0x69

echo "Dumping PLL register contents:"

echo "Serial Control Port Configuration and Device Identification"
for regaddr in 0x0000\
               0x0004\
               0x0005\
               0x0006\
               0x0007\
               0x000A\
               0x000B\
               0x000C\
               0x000D; do pllctl $plladdr $regaddr; done
echo "System Clock"
for regaddr in 0x0100\
               0x0101\
               0x0102\
               0x0103\
               0x0104\
               0x0105\
               0x0106\
               0x0107\
               0x0108; do pllctl $plladdr $regaddr; done
echo "General Configuration"
for regaddr in 0x0200\
               0x0201\
               0x0202\
               0x0203\
               0x0204\
               0x0205\
               0x0206\
               0x0207\
               0x0208\
               0x0209\
               0x020A\
               0x020B\
               0x020C\
               0x020D\
               0x020E\
               0x020F\
               0x0210\
               0x0211\
               0x0300\
               0x0301\
               0x0302\
               0x0303\
               0x0304\
               0x0305\
               0x0306\
               0x0307\
               0x0308\
               0x0309\
               0x030A\
               0x030B\
               0x030C\
               0x030D\
               0x030E\
               0x030F\
               0x0310\
               0x0311\
               0x0312\
               0x0313\
               0x0314\
               0x0315\
               0x0316\
               0x0317\
               0x0318\
               0x0319\
               0x031A\
               0x031B\
               0x031C\
               0x031D\
               0x031E\
               0x031F\
               0x0320\
               0x0321\
               0x0322\
               0x0323\
               0x0324\
               0x0325\
               0x0326\
               0x0327\
               0x0328\
               0x0329\
               0x032A\
               0x032B\
               0x032C\
               0x032D\
               0x032E; do pllctl $plladdr $regaddr; done
echo "Output PLL (APLL)"
for regaddr in 0x0400\
               0x0401\
               0x0402\
               0x0403\
               0x0404\
               0x0405\
               0x0406\
               0x0407\
               0x0408; do pllctl $plladdr $regaddr; done
echo "Output Clock Distribution"
for regaddr in 0x0500\
               0x0501\
               0x0502\
               0x0503\
               0x0504\
               0x0505\
               0x0506\
               0x0507\
               0x0508\
               0x0509\
               0x050A\
               0x050B\
               0x050C\
               0x050D\
               0x050E\
               0x050F\
               0x0510\
               0x0511\
               0x0512\
               0x0513\
               0x0514\
               0x0515; do pllctl $plladdr $regaddr; done
echo "Reference Inputs"
for regaddr in 0x0600\
               0x0601\
               0x0602\
               0x0603; do pllctl $plladdr $regaddr; done
echo "Frame Synchronization Mode"
for regaddr in 0x0640\
               0x0641; do pllctl $plladdr $regaddr; done
echo "DPLL Profile A (for REFA)"
for regaddr in 0x0700\
               0x0701\
               0x0702\
               0x0703\
               0x0704\
               0x0705\
               0x0706\
               0x0707\
               0x0708\
               0x0709\
               0x070A\
               0x070B\
               0x070C\
               0x070D\
               0x070E\
               0x070F\
               0x0710\
               0x0711\
               0x0712\
               0x0713\
               0x0714\
               0x0715\
               0x0716\
               0x0717\
               0x0718\
               0x0719\
               0x071A\
               0x071B\
               0x071C\
               0x071D\
               0x071E\
               0x071F\
               0x0720\
               0x0721\
               0x0722\
               0x0723\
               0x0724\
               0x0725\
               0x0726; do pllctl $plladdr $regaddr; done
echo "DPLL Profile B (for REFB)"
for regaddr in 0x0740\
               0x0741\
               0x0742\
               0x0743\
               0x0744\
               0x0745\
               0x0746\
               0x0747\
               0x0748\
               0x0749\
               0x074A\
               0x074B\
               0x074C\
               0x074D\
               0x074E\
               0x074F\
               0x0750\
               0x0751\
               0x0752\
               0x0753\
               0x0754\
               0x0755\
               0x0756\
               0x0757\
               0x0758\
               0x0759\
               0x075A\
               0x075B\
               0x075C\
               0x075D\
               0x075E\
               0x075F\
               0x0760\
               0x0761\
               0x0762\
               0x0763\
               0x0764\
               0x0765\
               0x0766; do pllctl $plladdr $regaddr; done
echo "DPLL Profile C (for REFC)"
for regaddr in 0x0780\
               0x0781\
               0x0782\
               0x0783\
               0x0784\
               0x0785\
               0x0786\
               0x0787\
               0x0788\
               0x0789\
               0x078A\
               0x078B\
               0x078C\
               0x078D\
               0x078E\
               0x078F\
               0x0790\
               0x0791\
               0x0792\
               0x0793\
               0x0794\
               0x0795\
               0x0796\
               0x0797\
               0x0798\
               0x0799\
               0x079A\
               0x079B\
               0x079C\
               0x079D\
               0x079E\
               0x079F\
               0x07A0\
               0x07A1\
               0x07A2\
               0x07A3\
               0x07A4\
               0x07A5\
               0x07A6; do pllctl $plladdr $regaddr; done
echo "DPLL Profile D (for REFD)"
for regaddr in 0x07C0\
               0x07C1\
               0x07C2\
               0x07C3\
               0x07C4\
               0x07C5\
               0x07C6\
               0x07C7\
               0x07C8\
               0x07C9\
               0x07CA\
               0x07CB\
               0x07CC\
               0x07CD\
               0x07CE\
               0x07CF\
               0x07D0\
               0x07D1\
               0x07D2\
               0x07D3\
               0x07D4\
               0x07D5\
               0x07D6\
               0x07D7\
               0x07D8\
               0x07D9\
               0x07DA\
               0x07DB\
               0x07DC\
               0x07DD\
               0x07DE\
               0x07DF\
               0x07E0\
               0x07E1\
               0x07E2\
               0x07E3\
               0x07E4\
               0x07E5\
               0x07E6; do pllctl $plladdr $regaddr; done
echo "Operational Controls"
for regaddr in 0x0A00\
               0x0A01\
               0x0A02\
               0x0A03\
               0x0A04\
               0x0A05\
               0x0A06\
               0x0A07\
               0x0A08\
               0x0A09\
               0x0A0A\
               0x0A0B\
               0x0A0C\
               0x0A0D; do pllctl $plladdr $regaddr; done
echo "Quick In-Out Frequency Soft Pin Configuration"
for regaddr in 0x0C00\
               0x0C01\
               0x0C02\
               0x0C03\
               0x0C04\
               0x0C05\
               0x0C06\
               0x0C07\
               0x0C08; do pllctl $plladdr $regaddr; done
echo "Read-Only Status (Accessible During EEPROM Transactions)"
for regaddr in 0x0D00\
               0x0D01\
               0x0D02\
               0x0D03\
               0x0D04\
               0x0D05\
               0x0D06\
               0x0D07\
               0x0D08\
               0x0D09\
               0x0D0A\
               0x0D0B\
               0x0D0C\
               0x0D0D\
               0x0D0E\
               0x0D0F\
               0x0D10\
               0x0D11\
               0x0D12\
               0x0D13\
               0x0D14; do pllctl $plladdr $regaddr; done
echo "EEPROM Control"
for regaddr in 0x0E00\
               0x0E01\
               0x0E02\
               0x0E03; do pllctl $plladdr $regaddr; done
echo "EEPROM Storage Sequence"
for regaddr in 0x0E10\
               0x0E11\
               0x0E12\
               0x0E13\
               0x0E14\
               0x0E15\
               0x0E16\
               0x0E17\
               0x0E18\
               0x0E19\
               0x0E1A\
               0x0E1B\
               0x0E1C\
               0x0E1D\
               0x0E1E\
               0x0E1F\
               0x0E20\
               0x0E21\
               0x0E22\
               0x0E23\
               0x0E24\
               0x0E25\
               0x0E26\
               0x0E27\
               0x0E28\
               0x0E29\
               0x0E2A\
               0x0E2B\
               0x0E2C\
               0x0E2D\
               0x0E2E\
               0x0E2F\
               0x0E30\
               0x0E31\
               0x0E32\
               0x0E33\
               0x0E34\
               0x0E35\
               0x0E36\
               0x0E37\
               0x0E38\
               0x0E39\
               0x0E3A\
               0x0E3B\
               0x0E3C; do pllctl $plladdr $regaddr; done

###### 0x69 to be replaced by $plladdr in the following line
let refab=`pllctl 0x69 0x0D0B | tr -d '\n' | sed -e 's/0x69 0x0d0bread-->//g'`
printf "0x%02x\n" $refab
if [[ $(( $refab & 1 )) -eq 1 ]]; then
  echo "* REFA slow";
fi
if [[ $(( $refab & 2 )) -eq 2 ]]; then
  echo "* REFA fast";
fi
if [[ $(( $refab & 4 )) -eq 4 ]]; then
  echo "* REFA fault";
fi
if [[ $(( $refab & 8 )) -eq 8 ]]; then
  echo "* REFA valid";
fi
echo "-"
if [[ $(( $refab & 16 )) -eq 16 ]]; then
  echo "* REFB slow";
fi
if [[ $(( $refab & 32 )) -eq 32 ]]; then
  echo "* REFB fast";
fi
if [[ $(( $refab & 64 )) -eq 64 ]]; then
  echo "* REFB fault";
fi
if [[ $(( $refab & 128 )) -eq 128 ]]; then
  echo "* REFB valid";
fi

###### 0x69 to be replaced by $plladdr in the following line
let refcd=`pllctl 0x69 0x0D0C | tr -d '\n' | sed -e 's/0x69 0x0d0cread-->//g'`
printf "0x%02x\n" $refcd
if [[ $(( $refcd & 1 )) -eq 1 ]]; then
  echo "* REFC slow";
fi
if [[ $(( $refcd & 2 )) -eq 2 ]]; then
  echo "* REFC fast";
fi
if [[ $(( $refcd & 4 )) -eq 4 ]]; then
  echo "* REFC fault";
fi
if [[ $(( $refcd & 8 )) -eq 8 ]]; then
  echo "* REFC valid";
fi
echo "-"
if [[ $(( $refcd & 16 )) -eq 16 ]]; then
  echo "* REFD slow";
fi
if [[ $(( $refcd & 32 )) -eq 32 ]]; then
  echo "* REFD fast";
fi
if [[ $(( $refcd & 64 )) -eq 64 ]]; then
  echo "* REFD fault";
fi
if [[ $(( $refcd & 128 )) -eq 128 ]]; then
  echo "* REFD valid";
fi
