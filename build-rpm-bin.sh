#!/bin/sh

mkdir -p /var/tmp/atilo/usr/bin
cp Release/atilo  /var/tmp/atilo/usr/bin
strip /var/tmp/atilo/usr/bin/atilo
sudo touch /usr/src/redhat/RPMS/i386/atilo-1-2.i386.rpm
sudo chmod a+rw /usr/src/redhat/RPMS/i386/atilo-1-2.i386.rpm

rpmbuild -bb atilo.spec
