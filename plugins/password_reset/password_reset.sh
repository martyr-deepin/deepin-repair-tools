#!/bin/sh

if [ $# -ne 2 ]; then
    exit 1
fi

printf "$1:$2" | chpasswd
