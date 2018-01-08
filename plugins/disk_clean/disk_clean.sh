#!/bin/sh


# size calculate
printf "DiskClean: %s\n" "`du -sh /var/cache/apt/archives/*.deb`"

apt clean
