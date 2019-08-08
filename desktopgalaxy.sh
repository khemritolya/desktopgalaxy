#!/bin/bash

# This is a work in progress

# Author: Khemri Tolya

if [ -f desktopgalaxy ]; then
    echo "found desktopgalaxy executable"
    chmod a+x desktopgalaxy
    # save your galaxy generating values here
    # these are (almost) the default ones
    ./desktopgalaxy --fpstarget 20 --seed 1234 --arms 4 --length 200 --coef 1.15
else
   echo "could not find desktopgalaxy executable!"
fi
