#!/usr/bin/env bash

if [ "$USER" = "root" ]; then
	echo "Do not execute this script in root/sudo."
	exit 1
fi

case $1 in
	install)
		mkdir -p $HOME/.local/bin
		rm -rf $HOME/.local/bin/dwm*
		cp -f scripts/dwm* $HOME/.local/bin/
		chmod +x $HOME/.local/bin/dwm*

		rm -rf $HOME/.dwm
		mkdir -p $HOME/.dwm
		cp -f autostart/* $HOME/.dwm/
		chmod +x $HOME/.dwm/autostart.sh

		cp -f files/dmenu-applications $HOME/.cache/dmenu-applications

		cp -f scripts/xbrightness.sh $HOME/.local/bin/xbrightness.sh
		chmod +x $HOME/.local/bin/xbrightness.sh

		cp -f scripts/xgetcolortemp.sh $HOME/.local/bin/xgetcolortemp.sh
		chmod +x $HOME/.local/bin/xgetcolortemp.sh

		;;
	clean)
		rm -rf $HOME/.dwm
		rm -f $HOME/.local/bin/dwm*
		rm $HOME/.cache/dmenu-applications
		rm $HOME/.local/bin/xbrightness.sh
		rm $HOME/.local/bin/xgetcolortemp.sh
		;;
	*)
		echo "Usage: sh $0 [install|clean]"
		;;
esac


