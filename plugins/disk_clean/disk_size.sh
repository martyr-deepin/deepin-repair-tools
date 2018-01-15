#!/bin/sh

df -BK --sync --output=used,size $1 | awk 'NR > 1 { printf "%s %s", substr($1, 1, length($1) - 1), substr($2, 1, length($2) - 1) }'
