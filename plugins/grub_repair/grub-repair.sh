#!/bin/sh

if [ $# -eq 1 ]; then
    sleep 1
    grub-install --force --recheck $1
    update-grub
fi
