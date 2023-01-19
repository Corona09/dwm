#!/usr/bin/env bash
set -e

if ! command -v bc &> /dev/null
then
    echo "bc command could not be found, it's needed to run this script."
    exit
fi

get_display_info() {
    xrandr --verbose | grep -m 1 -w "$1 connected" -A8 | grep "$2" | cut -f2- -d: | tr -d ' '
}

# cribbed from redshift, https://github.com/jonls/redshift/blob/master/README-colorramp
GAMMA_VALS=('1.0:0.7:0.4'  # 3000K
            '1.0:0.7:0.5'  # 3500K
            '1.0:0.8:0.6'  # 4000K
            '1.0:0.8:0.7'  # 4500K
            '1.0:0.9:0.8'  # 5000K
            '1.0:0.9:0.9'  # 6000K
            '1.0:1.0:1.0'  # 6500K
            '0.9:0.9:1.0'  # 7000K
            '0.8:0.9:1.0'  # 8000K
            '0.8:0.8:1.0'  # 9000K
            '0.7:0.8:1.0') # 10000K

get_gamma_index() {
    for i in "${!GAMMA_VALS[@]}"; do
        [[ "${GAMMA_VALS[$i]}" = "$1" ]] && echo "$i" && break
    done
}

get_temp_for_gamma() {
    idx=$(get_gamma_index "$1")
    awk '{printf "%.1f", $1 / 10}' <<< "$idx"
}

# gamma values returned by xrandr --verbose are somehow inverted
# https://gitlab.freedesktop.org/xorg/app/xrandr/issues/33
# this function corrects this bug by reverting the calculation
invert_gamma() {
    inv_r=$(cut -d: -f1 <<< "$1")
    inv_g=$(cut -d: -f2 <<< "$1")
    inv_b=$(cut -d: -f3 <<< "$1")
    r=$(awk '{printf "%.1f", 1/$1}' <<< "$inv_r" 2>/dev/null)
    g=$(awk '{printf "%.1f", 1/$1}' <<< "$inv_g" 2>/dev/null)
    b=$(awk '{printf "%.1f", 1/$1}' <<< "$inv_b" 2>/dev/null)
    echo "$r:$g:$b"
}

get_gamma() {
    invert_gamma "$(get_display_info "$1" 'Gamma: ')"
}

DISP=`xrandr | grep -w connected | cut -f1 -d' '`
CURRGAMMA=$(get_gamma "$DISP")
CURRTEMP=$(get_temp_for_gamma "$CURRGAMMA")

echo $CURRTEMP
