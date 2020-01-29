#!/bin/bash

set -e

# request for notary
RID=`xcrun altool -t osx -f "$1" --primary-bundle-id "$2" --notarize-app -u "$3" -p "$4" | egrep -oi "([0-9a-f]+-)+[0-9a-f]+"`
# wait for notary
while ! xcrun altool --notarization-info $RID -u "$3" -p "$4" | grep "Package Approved" > /dev/null;
do
    sleep 5
    echo "Waiting for notary result..."
done
xcrun stapler staple "$1"
