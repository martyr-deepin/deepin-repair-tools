#!/bin/sh

if [ $# -eq 1 ]; then
    if [ $1 = "/" ]; then
        DIR=""
    else
        DIR=$1
    fi
else
    exit 1
fi

echo "
apt-get clean
" | tee $DIR/script.sh
chmod +x $DIR/script.sh

if [ "$DIR" = "" ]; then
    /script.sh
else
    chroot $DIR ./script.sh
fi

rm $DIR/script.sh
