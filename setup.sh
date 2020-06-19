#!/bin/bash

# destination of generated ion alias script
ION_DEST="/usr/local/bin/ion"
# note: this script must be run from the ion root directory
ION_ROOT=$(pwd)

# copy template of shell script to destination
cp ./assets/ion-shell $ION_DEST

# build shell script using dynamically generated path
echo "cd $ION_ROOT && ./main \$1" >> $ION_DEST
# give the new script executable permissions
chmod 755 $ION_DEST

#TODO: add success checking
echo "Successfully created shell script in location: "
echo $ION_DEST

