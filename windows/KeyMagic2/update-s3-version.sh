#!/bin/bash
if [[ $# -ne 1 ]]; then
	echo "$0 <version>"
	exit 1
fi

filename=$(mktemp)

echo -n "$1" > ${filename}
# remove storage.keymagic.net when appropriate
aws s3 cp ${filename} s3://storage.keymagic.net/releases/windows/latest/version.txt --acl public-read --profile thantthet
aws s3 cp ${filename} s3://keymagic/releases/windows/latest/version.txt --acl public-read --profile thantthet
rm ${filename}