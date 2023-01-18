#!/usr/bin/env bash

picom &

dunst &

feh --randomize --bg-fill $HOME/Pictures/wallpaper/ &

sh $HOME/.dwm/adjust-temp.sh &

ps -ef -u $USER | grep "/usr/bin/kwalletd5" | grep -v grep | awk '{print $2}' | xargs kill 2>/dev/null
nm-applet &

conky &

fcitx5 &

ps -ef -u $USER | grep "/usr/bin/sh $HOME/.local/bin/dwm-repeat" | grep -v grep | awk '{print $2}' | xargs kill 2>/dev/null
dwm-repeat &

nextcloud &
