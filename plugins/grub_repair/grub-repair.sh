#!/bin/sh

if [ $# -eq 1 ]; then
    grub-install --force --recheck $1 || exit 1
    update-grub || exit 1
fi
