#!/bin/sh

if [ ! -f /var/cache/apt/archives/*.deb ]; then
    exit 0
fi

# size calculate
printf "DiskClean: %s\n" "`du -sh /var/cache/apt/archives`"

apt clean
