#!/bin/bash

# recompile the source if you really want to change default values I guess
# TODO make it so not that

if [ -f desktopgalaxy ]; then
    echo "found desktopgalaxy executable"

    if [ -f ~/.config/autostart/desktopgalaxy.desktop ]; then
        echo "found startup entry"
    else
        echo "[Desktop Entry]
Version=0.2
Type=Application
Name=desktopgalaxy
Comment=Enjoy the stars from your desktop
Icon=`dirname "$(readlink -f "$0")"`/ico.png
Exec=\"`dirname "$(readlink -f "$0")"`/desktopgalaxy\"
Terminal=false
Categories=LiveWallPaper-like;
X-GNOME-Autostart-enabled=true" >> ~/.config/autostart/desktopgalaxy.desktop
    fi

    chmod a+x desktopgalaxy
else
   echo "could not find desktopgalaxy executable!"
fi
