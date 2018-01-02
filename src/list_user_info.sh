#!/bin/sh

for USER in `passwd -S -a | awk '{ if ($2 == "P") print $1 }'`
do
    printf "%s %s\n" "$USER" `eval echo ~$USER`
done
