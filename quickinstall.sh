#!/bin/bash

set -xeuo pipefail

readonly target="gd-frequency"
local_install=false

while [[ $# -gt 0 ]]; do 
   case $1 in 
      --local | --usr)
         local_install=true
         shift
         ;;
      *)
         shift
         ;;
   esac
done

make clean
make "$target"

if $local_install; then
   make install PREFIX=~/.local 
else
   sudo make install PREFIX=/usr 
fi
