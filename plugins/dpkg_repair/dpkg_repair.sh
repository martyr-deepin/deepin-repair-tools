#!/bin/sh

apt -f install --yes
dpkg -a --configure
