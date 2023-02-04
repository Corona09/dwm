#!/usr/bin/env bash

if [ "$USER" = "root" ]; then
	echo "Do not execute this script in root/sudo."
	exit 1
fi

case $1 in
	install)
		rm -rf $HOME/.dwm
		mkdir -p $HOME/.dwm
		cp -f autostart/* $HOME/.dwm/
		chmod +x $HOME/.dwm/autostart.sh

		[[ -f $HOME/.cache/dmenu-applications ]] || {
			cp -f files/dmenu-applications $HOME/.cache/dmenu-applications
		}
		;;
	clean)
		rm -rf $HOME/.dwm
		rm $HOME/.local/bin/xbrightness.sh
		rm $HOME/.local/bin/xgetcolortemp.sh
		;;
	*)
		echo "Usage: sh $0 [install|clean]"
		;;
esac


