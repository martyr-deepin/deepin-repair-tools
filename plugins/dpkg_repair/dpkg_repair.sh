#!/bin/sh
export DEBIAN_FRONTEND=noninteractive

# fix dpkg status first
# Potentially configure a few times to overcome cyclic dependencies.
if dpkg --audit 2>&1 || grep 'The following packages';then
{
	dpkg --configure --pending ||
	dpkg --configure --pending ||
	dpkg --configure --pending ||
	dpkg --configure --pending ||
	dpkg --configure --pending
}
fi

# Remove broken package from dpkg database
if dpkg --audit 2>&1 || grep 'The following packages';then
	echo "Remove broken package from dpkg database."
	for pkg in $(dpkg --audit 2>&1 | grep '^ ' | awk '{print $1}');do
		rm -f /var/lib/dpkg/info/${pkg}.*
		dpkg --remove --force-remove-reinstreq ${pkg}
	done
fi

# Dry run apt fix-broken first
apt -f install -s || exit 1
apt -f install --yes || exit 1
