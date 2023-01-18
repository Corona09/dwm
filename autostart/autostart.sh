#!/usr/bin/env bash

picom &

dunst &

feh --randomize --bg-fill $HOME/Pictures/wallpaper/ &

ps -ef -u $USER | grep "/usr/bin/kwalletd5" | grep -v grep | awk '{print $2}' | xargs kill
nm-applet &

conky &

fcitx5 &

ps -ef -u $USER | grep "/usr/bin/sh $HOME/.local/bin/dwm-bar-repeat" | grep -v grep | awk '{print $2}' | xargs kill
dwm-bar-repeat &

nextcloud &
