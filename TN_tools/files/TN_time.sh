#!/bin/bash

echo "Please enter the current date and time in the following format:"
echo "Year-Month-Day Hour:Minute:Second"
echo "Example:"
echo "2017-05-17 13:45:67"
read time
date -s "$time"
echo "Setting time to '$time'"
hwclock -w
