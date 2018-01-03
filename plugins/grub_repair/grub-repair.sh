#!/bin/sh

if [ $# -eq 1 ]; then
    grub-install --force --recheck $1
fi
