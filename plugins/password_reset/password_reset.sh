#!/bin/sh

if [ $# -ne 2 ]; then
    exit 1
fi

_username=${1}
_password=$(echo "${2}" | base64 -d)

echo "${_username}:${_password}" | chpasswd
