#!/bin/sh

if [ $# -eq 2 ]; then
    if [ $1 = "/" ]; then
        DIR=""
    else
        DIR=$1
    fi
    SCRIPT=$2
else
    exit 1
fi

cp $SCRIPT $DIR/script.sh
chmod +x $DIR/script.sh

if [ "$DIR" = "" ]; then
    /script.sh
else
    chroot $DIR ./script.sh
fi

rm $DIR/script.sh
