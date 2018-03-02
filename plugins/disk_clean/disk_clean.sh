#!/bin/sh

# /var/cache/apt/archives
# /var/lib/apt/lists ~ 181M
# /var/cache/apt/pkgcache.bin /var/cache/apt/srcpkgcache.bin ~ 181M
# /var/lib/lastore/safecache ~ 200M

# size calculate
TOTAL_SIZE=$(LC_ALL=C find /var/lib/apt/lists /var/cache/apt /var/lib/lastore/safecache  -type f -not -name lock -exec du -ch {} + | grep total)
if [ -z ${TOTAL_SIZE} ]; then
	printf "Exit: Already DiskClean.\n" ; exit 0
fi

printf "DiskClean: %s\n" ${TOTAL_SIZE}
LC_ALL=C find /var/lib/apt/lists /var/cache/apt /var/lib/lastore/safecache -type f -not -name lock -exec rm -vf {} +
