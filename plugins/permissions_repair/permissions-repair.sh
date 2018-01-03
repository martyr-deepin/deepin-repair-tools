#!/bin/sh

for USER in "$@"
do
    HOME_DIR=`eval echo ~$USER`

    # Fix .Xauthority files
    XAUTH=$HOME_DIR/.Xauthority
    if [ -e $XAUTH ]; then
        chmod 600 $HOME_DIR/.Xauthority
        chown $USER $HOME_DIR/.Xauthority
        chgrp $USER $HOME_DIR/.Xauthority
    fi
done
