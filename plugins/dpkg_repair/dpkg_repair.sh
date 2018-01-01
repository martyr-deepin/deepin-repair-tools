#!/bin/sh

DIR=""
if [ $# -eq 1 ]; then
    DIR=$1
fi

echo "apt -f install" | tee $DIR/script.sh
chmod +x $DIR/script.sh

if [ "$DIR" = "" ]; then
    /script.sh
else
    chroot $DIR ./script.sh
fi

rm $DIR/script.sh
