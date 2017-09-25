#!/bin/bash

echo "Please enter the current date and time in the following format:"
echo "2017-05-17 17:45:00"
read time
date -s $time
echo "Setting time to '$time'"
hwclock -w
