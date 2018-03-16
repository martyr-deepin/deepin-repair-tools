#!/bin/sh

config_item() {
  if [ -f /etc/default/grub ]; then
    . /etc/default/grub || return
  fi
  eval echo "\$$1"
}

mount_efi_partition() {
	cat /etc/fstab | sed '/^\s*$/d' | grep -v '^#' | awk '{print $1, $2}' | while read path dest;do
		case "${dest}" in
			/boot*)
				mount $path $dest;;
			*)
				;;
		esac
	done
}

BOOTLOADER=${1}
BOOTLOADER_ID="$(config_item GRUB_DISTRIBUTOR | tr A-Z a-z | cut -d' ' -f1)"
BOOTLOADER_ID="${BOOTLOADER_ID:-deepin}"

mount_efi_partition

if [ -d /sys/firmware/efi/vars/ ];then
        # mount efi partition

	if [ x$(cat /sys/firmware/efi/fw_platform_size 2>/dev/null) = 'x32' ]; then
		FORCEPARAM=" --efi-directory=/boot/efi --target=i386-efi --bootloader-id=${BOOTLOADER_ID}"
	else
		FORCEPARAM=" --efi-directory=/boot/efi --target=x86_64-efi --bootloader-id=${BOOTLOADER_ID}"
	fi
else
	FORCEPARAM=" --bootloader-id=${BOOTLOADER_ID} --target=i386-pc ${BOOTLOADER}"
fi

grub-install ${FORCEPARAM} --force --recheck || exit 1
update-grub || exit 1
