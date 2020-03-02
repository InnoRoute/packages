#!/bin/bash
pyang -f jstree -p files/yang files/yang/TNptp4l@2018-10-10.yang >yangmodel.html
pyang -f tree -p files/yang files/yang/TNptp4l@2018-10-10.yang >yangmodel.txt
pyang -f sample-xml-skeleton -p files/yang files/yang/TNptp4l@2018-10-10.yang >yangmodel_sample.xml

