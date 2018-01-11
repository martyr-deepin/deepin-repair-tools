#!/bin/sh

apt -f install --yes || exit 1
dpkg -a --configure || exit 1
