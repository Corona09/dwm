#!/usr/bin/env bash

h=`date "+%H"`

if [[ $h -ge 17 ]]; then
	xbrightness.sh = eDP1 0.3 --temp
fi
